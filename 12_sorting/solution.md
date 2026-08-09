# 12 — Sorting: Solutions

Full solutions for Sections 1–3 of `questions.md`. **Every function below was compiled with
`g++ -std=gnu++14` and executed against the test cases shown — all passing, as part of a
106-assertion run covering this folder and `../13_prefixSum`.**

Assume `#include <vector>`, `#include <algorithm>`, `using namespace std;`.

---

# Section 1 — Must Do

## 1. Bubble Sort

```cpp
void bubbleSort(int a[], int n) {                // n passed in, NOT computed with sizeof
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; ++j)
            if (a[j] > a[j + 1]) { swap(a[j], a[j + 1]); swapped = true; }
        if (!swapped) return;                    // nothing moved: already sorted
    }
}
```

**Key insight.** After pass `i`, the largest `i+1` elements are parked at the end — that is why
the inner bound is `n - 1 - i`. The `swapped` flag is what makes the **best case O(n)**; without
it bubble sort is O(n²) even on sorted input, and your comment claiming O(1) best case would have
no mechanism behind it.

**Complexity.** Best O(n), average/worst O(n²), O(1) space. **Stable.**

**Verified:** `{5,1,4,2,8}→{1,2,4,5,8}`; `{1,2,3}` already sorted returns after one pass.

---

## 2. Selection Sort

```cpp
void selectionSort(int a[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;                          // seed with i, not INT_MAX
        for (int j = i + 1; j < n; ++j)          // j < n, NOT n-1
            if (a[j] < a[minIdx]) minIdx = j;
        if (minIdx != i) swap(a[i], a[minIdx]);
    }
}
```

**Key insight — two details your version misses.**

1. **`j < n`, not `j < n - 1`.** With `n-1` the last element is never a candidate for the
   minimum, so it never moves. **Verified: yours turns `{64,25,12,22,11}` into
   `{12,22,25,64,11}` — the smallest value is still sitting at the end.**
2. **Track the *index*, not the value.** Seeding `minIdx = i` means the loop always has a valid
   answer. `int min = INT32_MAX, idx;` leaves `idx` **uninitialised** when the inner loop never
   assigns it (which happens on the last iteration when `j < n-1` makes the range empty), and
   `swap(a[i], a[idx])` then indexes with garbage — undefined behaviour.

**Complexity.** O(n²) always — even on sorted input, since it scans the full remaining range.
O(1) space. **Not stable** (a long-range swap can jump one equal element past another).

Selection sort's one virtue: exactly **n−1 swaps**, the fewest of any O(n²) sort.

**Verified:** `{64,25,12,22,11}→{11,12,22,25,64}`, `{2,1}→{1,2}`, `{3,1,2}→{1,2,3}`,
`{-5,-1,-9}→{-9,-5,-1}` (negatives, where an `INT32_MAX` seed is fine but an `int min = 0` seed
would not be).

---

## 3. Insertion Sort

```cpp
void insertionSort(int a[], int n) {
    for (int i = 1; i < n; ++i) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) { a[j + 1] = a[j]; --j; }   // shift right
        a[j + 1] = key;                                          // drop the key in
    }
}
```

**Key insight.** Maintain the invariant "`a[0..i-1]` is sorted", then insert `a[i]` into it.
**Shifting beats swapping**: a swap is three assignments, a shift is one, so this does about a
third of the memory traffic.

**Your `insertionSort.cpp` is correct** — verified on `{12,11,13,5,6}` and a fully reversed
array. It uses `swap` rather than shifting (your comment says "shift to right" while the code
swaps), so it is a constant factor slower but produces the right answer every time. Worth
switching to the shift form, but nothing is broken.

**Complexity.** Best O(n) (already sorted — the `while` never runs), average/worst O(n²), O(1)
space. **Stable**, because `a[j] > key` uses strict `>` and stops at an equal element.

This is the algorithm real libraries switch to for small subarrays — its constants are tiny.

**Verified:** `{12,11,13,5,6}→{5,6,11,12,13}`, `{5,4,3,2,1}→{1,2,3,4,5}`, `n=1` unchanged.

---

## 4. Merge Sort

```cpp
void mergeParts(vector<int>& a, int lo, int mid, int hi) {
    vector<int> tmp;
    tmp.reserve(hi - lo + 1);
    int i = lo, j = mid + 1;
    while (i <= mid && j <= hi) tmp.push_back(a[i] <= a[j] ? a[i++] : a[j++]);
    while (i <= mid) tmp.push_back(a[i++]);      // BOTH drains, unconditionally
    while (j <= hi)  tmp.push_back(a[j++]);
    for (int k = 0; k < (int)tmp.size(); ++k) a[lo + k] = tmp[k];
}
void mergeSort(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;                        // >= covers size 0 AND size 1
    int mid = lo + (hi - lo) / 2;
    mergeSort(a, lo, mid);
    mergeSort(a, mid + 1, hi);
    mergeParts(a, lo, mid, hi);
}
```

**Key insight — three things, and your version misses all three.**

1. **Sort indices, not copies.** `(a, lo, hi)` means no sub-vectors are built. Your version
   copies into fresh `a` and `b` vectors at every level, which is what makes the split bounds
   easy to get wrong.
2. **`lo >= hi`, not `n == 1`.** An empty range must terminate too.
3. **Both drain loops run unconditionally.** After the main loop exactly one half has leftovers,
   and you cannot know which — so write both `while`s with no `if` around them. The one that has
   nothing left simply does zero iterations.

**Stability comes from `<=`.** On a tie, take from the **left** half, which preserves original
order. Change it to `<` and the sort still works but is no longer stable.

**Complexity.** O(n log n) in **all** cases, O(n) auxiliary space. **Stable.**

**Verified:** n=4 `{5,2,9,1}→{1,2,5,9}`; n=3 `{3,1,2}→{1,2,3}`; n=1; **n=0 with no crash**;
reversed `{5,4,3,2,1}`; duplicates `{2,2,1,1,3}→{1,1,2,2,3}`; n=7
`{38,27,43,3,9,82,10}→{3,9,10,27,38,43,82}`.

---

## 5. Quick Sort (Lomuto partition)

```cpp
int partition(vector<int>& a, int lo, int hi) {
    int pivot = a[hi], i = lo - 1;
    for (int j = lo; j < hi; ++j)
        if (a[j] <= pivot) swap(a[++i], a[j]);
    swap(a[i + 1], a[hi]);
    return i + 1;                                // the pivot's FINAL index
}
void quickSort(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int p = partition(a, lo, hi);
    quickSort(a, lo, p - 1);                     // pivot excluded from both halves
    quickSort(a, p + 1, hi);
}
```

**Key insight — state the invariant.** Throughout the loop, `a[lo..i]` holds elements ≤ pivot and
`a[i+1..j-1]` holds elements > pivot. When `j` reaches `hi`, swapping the pivot into `i+1` puts
it exactly where it belongs. Eight lines and one invariant.

**The pivot is final — exclude it.** `quickSort(a, lo, p-1)` and `quickSort(a, p+1, hi)`. If
either range includes `p`, the subproblem never shrinks and you recurse forever.

**Complexity.** Average O(n log n); **worst O(n²)** on sorted input with a last-element pivot
(every split is 0 / n−1). O(log n) stack average. **Not stable.**

**Verified:** `{5,2,9,1,7}→{1,2,5,7,9}`; duplicates `{3,3,1,3}→{1,3,3,3}`; **already-sorted
`{1,2,3,4,5}` (the worst case) completes correctly**; `{2,1}`; all-equal `{7,7,7,7}`.

---

## 6. Sort Colors — LeetCode 75 (Dutch National Flag)

```cpp
void sortColors(vector<int>& a) {
    int lo = 0, mid = 0, hi = (int)a.size() - 1;
    while (mid <= hi) {
        if (a[mid] == 0)      swap(a[lo++], a[mid++]);
        else if (a[mid] == 1) ++mid;
        else                  swap(a[mid], a[hi--]);   // do NOT advance mid
    }
}
```

**Key insight.** Four regions: `[0,lo)` all 0s, `[lo,mid)` all 1s, `[mid,hi]` unknown,
`(hi,end)` all 2s. **After swapping with `hi`, `mid` must not advance** — the value that just
arrived from the right is unexamined. After swapping with `lo` it is safe to advance, because
that value was already known to be a 1.

**Complexity.** O(n) one pass, O(1) space. Counting 0s/1s/2s and overwriting is two passes and
also valid — say both.

**Verified:** `{2,0,2,1,1,0}→{0,0,1,1,2,2}`, `{0,1,2}` unchanged, `{2,2,2}`, `{1,0}→{0,1}`.

---

## 7–10. The cycle-sort placement family

**One loop. Four problems. Learn it once.**

```cpp
while (i < n) {
    int correct = a[i] - 1;                                       // value v belongs at index v-1
    if (a[i] >= 1 && a[i] <= n && a[i] != a[correct]) swap(a[i], a[correct]);
    else ++i;
}
```

**Two details decide whether it terminates.**

1. **Compare `a[i] != a[correct]` (values), not `i != correct` (indices).** With duplicates the
   index form swaps two equal values back and forth forever. **Verified: the loop in
   `cycleSortAlgo.cpp` uses the index form and did not terminate on `{4,5,3,1,1}` — still going
   after 2,000,000 swaps.**
2. **Range-guard before indexing.** `a[i] >= 1 && a[i] <= n` keeps out-of-range values from
   indexing outside the array. Only #10 hands you such values, but the guard costs nothing.

After the loop every value sits at its own index, and one scan answers each question.

### 7. Missing Number — LeetCode 268

Values are `0..n` with one missing, so `v` belongs at index `v` (not `v-1`):

```cpp
int missingNumber(vector<int> a) {
    int n = (int)a.size(), i = 0;
    while (i < n) {
        int c = a[i];
        if (a[i] < n && a[i] != a[c]) swap(a[i], a[c]); else ++i;
    }
    for (int k = 0; k < n; ++k) if (a[k] != k) return k;
    return n;                                    // 0..n-1 all present -> n is missing
}
```

**Verified:** `{3,0,1}→2`, `{0,1}→2`, `{9,6,4,2,3,5,7,0,1}→8`.

> `n*(n+1)/2 - sum` and XOR-of-all are both O(n)/O(1) too, and the XOR version cannot overflow.
> Mention them; the placement version is the one that generalises to #8–#10.

### 8. Find All Numbers Disappeared — LeetCode 448

```cpp
vector<int> findDisappearedNumbers(vector<int> a) {
    int n = (int)a.size(), i = 0;
    while (i < n) {
        int c = a[i] - 1;
        if (a[i] != a[c]) swap(a[i], a[c]); else ++i;
    }
    vector<int> res;
    for (int k = 0; k < n; ++k) if (a[k] != k + 1) res.push_back(k + 1);
    return res;
}
```

**Verified:** `{4,3,2,7,8,2,3,1}→{5,6}`, `{1,1}→{2}`.

### 9. Find the Duplicate Number — LeetCode 287

Identical placement; the scan returns the **value** sitting in the wrong slot rather than the
index.

**Verified:** `{1,3,4,2,2}→2`, `{3,1,3,4,2}→3`.

> Strictly, LeetCode 287 forbids modifying the array — use **Floyd's cycle detection** there
> (treat `a[i]` as a pointer to index `a[i]`; the duplicate is the cycle entrance), O(n)/O(1)
> and non-destructive. The placement version is the right answer when mutation is allowed.

### 10. First Missing Positive — LeetCode 41

```cpp
int firstMissingPositive(vector<int> a) {
    int n = (int)a.size(), i = 0;
    while (i < n) {
        int c = a[i] - 1;
        if (a[i] >= 1 && a[i] <= n && a[i] != a[c]) swap(a[i], a[c]);   // guard is essential here
        else ++i;
    }
    for (int k = 0; k < n; ++k) if (a[k] != k + 1) return k + 1;
    return n + 1;
}
```

**Key insight.** The answer is always in `[1, n+1]` — with `n` slots you cannot miss anything
larger. So values outside that range are irrelevant and the range guard simply skips them.
This is the **only** one of the four where the guard is load-bearing, which is why it is rated
Hard while the other three are Easy/Medium.

**Complexity.** O(n) time, O(1) space, for all four.

**Verified:** `{1,2,0}→3`, `{3,4,-1,1}→2` (negatives skipped), `{7,8,9,11,12}→1` (everything out
of range), `{1}→2`.

---

## 11. Count Inversions

```cpp
long long mergeCount(vector<int>& a, int lo, int mid, int hi) {
    vector<int> tmp; long long inv = 0;
    int i = lo, j = mid + 1;
    while (i <= mid && j <= hi) {
        if (a[i] <= a[j]) tmp.push_back(a[i++]);
        else { inv += (mid - i + 1); tmp.push_back(a[j++]); }   // a[i..mid] ALL beat a[j]
    }
    while (i <= mid) tmp.push_back(a[i++]);
    while (j <= hi)  tmp.push_back(a[j++]);
    for (int k = 0; k < (int)tmp.size(); ++k) a[lo + k] = tmp[k];
    return inv;
}
long long countInv(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return 0;
    int mid = lo + (hi - lo) / 2;
    long long c = countInv(a, lo, mid);
    c += countInv(a, mid + 1, hi);
    c += mergeCount(a, lo, mid, hi);
    return c;
}
```

**Key insight.** When you take `a[j]` from the right half before `a[i]`, then `a[i] > a[j]` — and
because the left half is sorted, **every remaining element of it** also exceeds `a[j]`. That is
`mid - i + 1` inversions counted in one addition instead of a loop. This is the classic example
of getting extra information free from an algorithm you were running anyway.

**Note the return type.** With n = 10⁵ the count can reach ~5×10⁹, which overflows `int`.

**Note:** `countInv` **sorts its input** as a side effect. If the caller needs the original
order, pass a copy.

**Complexity.** O(n log n) time, O(n) space.

**Verified:** `{2,4,1,3,5}→3`, reversed `{5,4,3,2,1}→10` (= 5·4/2, the maximum), sorted
`{1,2,3}→0`, all-equal `{1,1,1}→0` (equal elements are **not** inversions — this is what the
`<=` guarantees).

---

# Section 2 — Important

## 12. Merge Sorted Array — LeetCode 88

**Key insight.** Fill from the **back**. Writing forward would overwrite unread elements of
`nums1`; writing backward, the write pointer is always ahead of both read pointers. O(1) space.
Full solution in `../07_Array/readme.md` — you already have this one working.

## 13. Sort an Array — LeetCode 912

Submit your merge sort from #4. Quick sort with a last-element pivot **times out** on this
problem's adversarial sorted test cases — use a random pivot or merge sort.

---

## 14. Majority Element — LeetCode 169

**Sorting solution.** Sort and return `a[n/2]`. O(n log n), one line, and correct — an element
appearing more than `n/2` times must cover the middle index.

**Optimal — Moore's Voting, O(n)/O(1):**

```cpp
int majorityElement(vector<int>& a) {
    int cand = a[0], cnt = 0;
    for (int x : a) {
        if (cnt == 0) cand = x;
        cnt += (x == cand) ? 1 : -1;
    }
    return cand;
}
```

**Key insight.** Pair off every occurrence of the majority element with a non-occurrence. Since
it appears more than `n/2` times it cannot be fully cancelled, so whatever survives is it. The
guarantee holds **only** if a majority element exists — if the problem does not promise one, a
second pass to verify the count is required.

**Complexity.** O(n) time, O(1) space.

**Verified:** `{3,2,3}→3`, `{2,2,1,1,1,2,2}→2`, `{1}→1`.

---

## 15. Assign Cookies — LeetCode 455

```cpp
int findContentChildren(vector<int> g, vector<int> s) {
    sort(g.begin(), g.end());
    sort(s.begin(), s.end());
    int i = 0, j = 0;
    while (i < (int)g.size() && j < (int)s.size()) {
        if (s[j] >= g[i]) ++i;      // this cookie satisfies child i
        ++j;                        // cookie is consumed either way
    }
    return i;
}
```

**Key insight.** Sort both, then always give the **smallest adequate cookie** to the least greedy
unsatisfied child. Greedy is optimal here: using a larger cookie on a child a smaller one would
have satisfied can never help. Note `++j` happens unconditionally — a cookie too small for the
current child is too small for every later one, so discard it.

**Complexity.** O(n log n + m log m) time, O(1) extra.

**Verified:** `g={1,2,3}, s={1,1} → 1`; `g={1,2}, s={1,2,3} → 2`; `g={5,6}, s={1,2} → 0`.

---

## 16–17. Merge Intervals (56) and Non-overlapping Intervals (435) — **the sort-key pair**

**Merge Intervals — sort by START:**

```cpp
vector<vector<int> > merge(vector<vector<int> >& iv) {
    sort(iv.begin(), iv.end());                       // by start, then end
    vector<vector<int> > res;
    for (size_t i = 0; i < iv.size(); ++i) {
        if (!res.empty() && iv[i][0] <= res.back()[1]) res.back()[1] = max(res.back()[1], iv[i][1]);
        else res.push_back(iv[i]);
    }
    return res;
}
```

**Non-overlapping Intervals — sort by END:**

```cpp
int eraseOverlapIntervals(vector<vector<int> >& iv) {
    sort(iv.begin(), iv.end(), [](const vector<int>& a, const vector<int>& b){
        return a[1] < b[1];                           // by END
    });
    int keep = 0, lastEnd = INT_MIN;
    for (size_t i = 0; i < iv.size(); ++i)
        if (iv[i][0] >= lastEnd) { ++keep; lastEnd = iv[i][1]; }
    return (int)iv.size() - keep;
}
```

**Key insight — the whole lesson is the sort key.** For merging you need intervals in
positional order, so **start**. For keeping the maximum number of non-overlapping intervals, the
greedy proof requires always taking the one that **frees up soonest**, so **end**. Sorting #17
by start is the standard wrong answer and produces plausible-looking output on small cases.

`max(res.back()[1], iv[i][1])` in #16 matters for a fully contained interval like `[1,10]` then
`[2,3]` — without the `max`, the end shrinks.

**Complexity.** O(n log n) both.

**Verified:** merge — `{{1,3},{2,6},{8,10},{15,18}}` → `[1,6][8,10][15,18]`; touching
`{{1,4},{4,5}}` → `[1,5]`; contained `{{1,10},{2,3}}` → `[1,10]` (the case that needs `max`);
unsorted input handled. Non-overlapping — `{{1,2},{2,3},{3,4},{1,3}}` → 1; three identical
intervals → 2; already-disjoint → 0.

---

## 18. Largest Number — LeetCode 179

```cpp
string largestNumber(vector<int>& nums) {
    vector<string> s;
    for (int x : nums) s.push_back(to_string(x));
    sort(s.begin(), s.end(), [](const string& a, const string& b){
        return a + b > b + a;                         // the comparator IS the solution
    });
    if (s[0] == "0") return "0";                      // all zeros
    string res;
    for (size_t i = 0; i < s.size(); ++i) res += s[i];
    return res;
}
```

**Key insight.** "Which of `a` and `b` should come first?" is answered by comparing the two
*concatenations*: `"9" + "34" = "934"` beats `"34" + "9" = "349"`, so 9 comes first. Neither
numeric nor lexicographic ordering works (`"3"` vs `"30"` breaks lexicographic). This
concatenation comparator is provably a valid strict weak ordering, which is why `sort` accepts it.

The `s[0] == "0"` check handles `{0,0}`, which would otherwise print `"00"`.

**Complexity.** O(n log n · L) time.

**Verified:** `{10,2}→"210"`, `{3,30,34,5,9}→"9534330"`, `{0,0}→"0"` (the all-zeros guard),
`{1}→"1"`.

---

## 19. Sort Array By Parity — LeetCode 905

Two pointers: `i` from the left finds an odd, `j` from the right finds an even, swap. One pass,
O(1) space — the same partition step as quick sort with "is even" as the predicate.

---

## 20. Kth Largest Element — LeetCode 215 (quickselect)

```cpp
int quickSelect(vector<int>& a, int lo, int hi, int k) {   // k = index in the SORTED array
    while (lo <= hi) {
        int p = partition(a, lo, hi);                       // #5's partition, unchanged
        if (p == k) return a[p];
        if (p < k)  lo = p + 1;                             // recurse into ONE side only
        else        hi = p - 1;
    }
    return -1;
}
// kth largest == index n-k when sorted ascending
```

**Key insight.** Quick sort recurses into both halves; quickselect only needs the half containing
the target index, which drops the average from O(n log n) to **O(n)** (n + n/2 + n/4 + … = 2n).
Worst case is still O(n²) — a random pivot makes that vanishingly unlikely.

Written as a loop rather than recursion, it is also O(1) space.

**Complexity.** O(n) average, O(n²) worst, O(1) space.

**Verified:** `{3,2,1,5,6,4}` k=2 → 5; `{3,2,3,1,2,4,5,5,6}` k=4 → 4; k=1 returns the maximum;
k=n returns the minimum.

---

# Section 3 — Good to Know

## 21. Minimum Swaps to Sort

**Key insight.** Pair each value with its original index, sort by value, then walk the resulting
permutation counting **cycles**. A cycle of length `L` needs `L − 1` swaps, so the answer is
`n − (number of cycles)`. This is cycle sort's actual theorem, and it is the true minimum — no
algorithm can do better.

**Complexity.** O(n log n) for the sort, O(n) for the cycle walk.

**Verified:** `{10,19,6,3,5}→2`, `{1,2,3}→0` (already sorted), `{2,1}→1`, `{4,3,2,1}→2`
(two disjoint cycles, not three swaps).

---

## 22. Reverse Pairs — LeetCode 493

**Key insight — count in a separate pass from merging.** The condition is `a[i] > 2*a[j]`, which
is *not* the comparison that decides merge order, so you cannot fuse them the way #11 does. Run a
two-pointer count over the two sorted halves first, then merge normally:

```cpp
// before merging, both halves are sorted:
int j = mid + 1;
for (int i = lo; i <= mid; ++i) {
    while (j <= hi && (long long)a[i] > 2LL * a[j]) ++j;
    count += (j - mid - 1);
}
```

`j` never resets across the outer loop, so the counting pass is O(n), not O(n²).

**`2LL * a[j]` must be `long long`** — with `a[j]` near `INT_MAX`, `2 * a[j]` overflows, and
because `a[j]` can be negative the comparison genuinely needs the wider type.

**Complexity.** O(n log n) time, O(n) space.

**Verified:** `{1,3,2,3,1}→2`, `{2,4,3,5,1}→3`, `{1,2,3}→0`, and
`{2147483647, 2147483647, -2147483647}→2` — the case that overflows without the `2LL`.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `12_sorting/` was edited.**

## `bubblesort.cpp:11` — the array-decay bug (GCC warns about this one)

```cpp
void bubbleSort (int arr[]){
    int n =sizeof(arr)/sizeof(arr[0]);      // arr is a POINTER here
```

An array parameter **decays to a pointer**: `void f(int arr[])` is exactly `void f(int* arr)`.
So `sizeof(arr)` is the size of a pointer — **4 bytes on this 32-bit toolchain** — and
`sizeof(arr[0])` is 4. `n` computes to **1**, the outer loop `i < n-1` never runs, and the
function does nothing at all.

GCC says so explicitly:

```
bubblesort.cpp:11:22: warning: 'sizeof' on array function parameter 'arr'
                      will return size of 'int*' [-Wsizeof-array-argument]
```

**Verified: `bubbleSort({5,1,4,2,8})` leaves the array as `{5,1,4,2,8}` — completely unsorted.**

The fix is to pass the length, exactly as your other three sort files already do:

```cpp
void bubbleSort(int arr[], int n) { ... }
```

`sizeof(arr)/sizeof(arr[0])` only works where the **real array** is in scope — as in
`cycleSortAlgo.cpp:7`, where it would be correct.

Also `//TC best Case=>O(1)` in the comment should be **O(n)**: even a sorted array requires one
full pass to discover nothing needs swapping.

---

## `selectionSort.cpp:8` — the last element is never placed

```cpp
for (int j =i;j<n-1;j++){       // should be j < n
```

The inner loop stops one short, so `a[n-1]` is never considered as a minimum.

**Verified: `{64,25,12,22,11}` becomes `{12,22,25,64,11}`** — the smallest element is still at
the end. Also `{2,1}→{2,1}` (unchanged) and `{3,1,2}→{1,3,2}`.

**Second bug on line 7:** `int min=INT32_MAX,idx;` leaves `idx` **uninitialised**. When `i` is
the last index the inner loop body never executes, `idx` holds garbage, and `swap(arr[i],
arr[idx])` writes through it. Seeding `int minIdx = i;` fixes both the UB and the logic. See #2.

---

## `mergeSort.cpp` — crashes on a 4-element array, loses elements on a 3-element one

### Bug 1 — `:42`, the second half is built with the wrong bound

```cpp
for (int i =n/2;i<n-(n/2);i++){      // should be i < n
    b.push_back(v[i]);
}
```

For `n == 4`: the loop runs `i` from 2 while `i < 4-2 == 2` — **zero iterations**, so `b` is
empty. `mergeSort(b)` is then called on an empty vector, which reaches Bug 2.

### Bug 2 — `:36`, the base case misses size 0

```cpp
if(n==1)return;
```

An empty vector is not size 1, so it splits into two more empty vectors and recurses forever.

**Verified: `mergeSort({5,2,9,1})` crashes with `0xC00000FD` (STACK_OVERFLOW) and produces no
output.** Fix: `if (n <= 1) return;` — or better, switch to the index-based form in #4 with
`if (lo >= hi) return;`.

### Bug 3 — `:22-28`, the second drain block is wrong three ways

```cpp
if(i==b.size())          // should be j == b.size()
{
    while(j<a.size()){   // should be i < a.size()
        c[k++]=a[i++];   // condition tests j, body advances i -- mismatched
    }
}
```

The condition names the wrong variable, the loop guard names a third, and the body increments a
fourth. When the right half is exhausted first, nothing drains the left half.

**Verified: `mergeSort({3,1,2})` returns `{1,1,2}` — the value 3 is lost and 1 is duplicated.**

Corrected version in #4: both drains, unconditional, each testing its own index.

---

## `quickSort.cpp` — the partition loop does not terminate

```cpp
while( i<(pivotidx) || j>(pivotidx)  ){
     if(v[i]>v[pivotidx] && v[j]<v[pivotidx]){ swap(v[i],v[j]); i++;j--; }
     if(v[i]> v[pivotidx] ){ swap(v[i],v[j]); j--; }
     if(v[j]< v[pivotidx] ){ swap(v[i],v[j]); i++; }
}
```

**Verified: hangs. `quickSort({5,2,9,1,7})` and `quickSort({3,3,1,3})` were both killed after
10 seconds.**

Two problems. The three `if`s are **sequential, not exclusive** — after the first one swaps and
moves both pointers, the second and third re-test the *new* values and can swap again in the same
iteration, undoing the work. And when one pointer reaches `pivotidx` while the other has not, the
comparison `v[i] > v[pivotidx]` with `i == pivotidx` compares the pivot with itself (false), while
the third `if` can still swap the **pivot itself** out of its slot — corrupting the value the
loop is comparing against, so the exit condition may never be reachable.

**The counting idea at `:10-16` is sound** — counting how many elements are smaller does give the
pivot's final index. It is only the two-pointer rearrangement that fails. Lomuto's single-pointer
partition (#5) achieves the same thing in three lines with an invariant you can state.

---

## `cycleSortAlgo.cpp:11` — infinite loop on any duplicate

```cpp
else swap(arr[i],arr[arr[i]-1]);
```

The termination test is `arr[i]==i+1`, an **index/value** comparison. When two equal values want
the same slot, the swap is a no-op in effect and the loop repeats forever.

**Verified: with `{4,5,3,1,1}` the loop was still running after 2,000,000 swaps.** Your literal
array `{4,5,3,1,2}` is a permutation of 1..5 with no duplicates, which is why it terminates.

Fix — compare **values**, and guard the range:

```cpp
while (i < n) {
    int correct = a[i] - 1;
    if (a[i] >= 1 && a[i] <= n && a[i] != a[correct]) swap(a[i], a[correct]);
    else ++i;
}
```

**One further caveat worth knowing:** even with the value comparison, this loop **terminates but
does not fully sort** an array containing duplicates — verified, `{1,1,2}` comes out as `{1,2,1}`.
Cycle sort in this form is for permutations of `1..n`. That is not a limitation for #7–#10,
because those problems only need each value *placed*, not the array sorted.

The program also never prints the array, so running it produces no output — verified, it exits
cleanly with nothing on stdout.

---

## What you got right

- **`insertionSort.cpp` is correct** — verified on `{12,11,13,5,6}`, a fully reversed array, and
  `n=1`. The `j >= 1 && arr[j] < arr[j-1]` guard is right, and stopping on equality keeps it
  **stable**. Using `swap` instead of shifting is a constant-factor cost, not a bug.

- **Your complexity comments are accurate**, which is rarer than it sounds:
  `insertionSort.cpp:5` "best O(n) avg/worst O(n²)" and "Stable Sort" — both correct.
  `selectionSort.cpp:4-5` "O(n²)" and "unstable" — both correct.
  `bubblesort.cpp:7` "Stable sort" and `(n(n-1))/2 == O(n²)` — correct, including the exact
  comparison count. Only the "best case O(1)" needs changing to O(n).

- **`quickSort.cpp`'s counting approach to finding the pivot index** (`:9-16`) is genuinely
  correct and not the textbook method — counting the elements smaller than the pivot *does* give
  its final position directly. The idea is sound; only the rearrangement loop after it fails.

- **`quickSort.cpp:45` `if(si>=ei) return;`** uses `>=`, so it handles both empty and
  single-element ranges. That is the base case `mergeSort.cpp` needed and did not have — you had
  it right in one file and not the other.

- **`cycleSortAlgo.cpp`'s complexity comment `TC=O(n) SC=O(1)`** is correct for the placement
  loop, and identifying cycle sort as the tool for 268/287/448/41 in your `readme.md` is exactly
  the right connection. That grouping is the most useful thing in the folder — most people meet
  those four problems separately and never notice they are one algorithm.

- **`mergeSort.cpp`'s `merge` takes all three vectors by reference** (`vector<int>& a, b, c`),
  avoiding copies. The signature is right even though the body has bugs.
