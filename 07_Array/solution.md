# 07 — Arrays: Solutions

Full C++14 for Sections 1–3, approach-only for Section 4.
**Every algorithm below was compiled and executed before publication** — ~45 assertions
across two test harnesses. Claimed outputs are measured.

Assume `#include <iostream> <vector> <algorithm> <climits> <map> <unordered_map>` and
`using namespace std;`.

---

# Section 1 — Must Do

## 1. Largest element

```cpp
int largest(const vector<int> &a) {
    if (a.empty()) return -1;              // decide and document the empty case
    int mx = INT_MIN;                      // NOT -1, NOT 0
    for (int x : a) if (x > mx) mx = x;
    return mx;
}
```

**The whole question is the initialiser.** `1_array.cpp:49` uses `int max = -1`, which
returns `-1` for `[-5,-3,-9]` — a value that isn't even in the array. Use `INT_MIN`, or
better `a[0]` (which also handles "the answer must be an element").

**Complexity:** O(n) / O(1). *Verified: `{3,3,6,1,6}` → 6.*

---

## 2. Second largest (no sorting)

```cpp
int secondLargest(const vector<int> &a) {
    long long largest = LLONG_MIN, second = LLONG_MIN;
    for (int x : a) {
        if (x > largest)                 { second = largest; largest = x; }
        else if (x < largest && x > second) { second = x; }   // x < largest EXCLUDES duplicates
    }
    return second == LLONG_MIN ? -1 : (int)second;
}
```

**`x < largest` is the graded condition.** With `x <= largest`, the input `[5,5,2]` returns
`5` — but the second *largest distinct* value is `2`. Duplicates of the maximum must not
count.

**One pass, not two.** The two-pass version (find max, then find max of everything smaller)
is also O(n) and perfectly acceptable; this version is one pass.

**Complexity:** O(n) / O(1). *Verified: `{3,3,6,1,6}` → 3.*

---

## 3. Check if sorted (and rotated) — LeetCode 1752

```cpp
bool isSorted(const vector<int> &a) {
    for (size_t i = 1; i < a.size(); ++i) if (a[i] < a[i-1]) return false;
    return true;
}

// LeetCode 1752: sorted ascending, then rotated some number of times
bool checkSortedRotated(const vector<int> &a) {
    int n = a.size(), breaks = 0;
    for (int i = 0; i < n; ++i)
        if (a[i] > a[(i + 1) % n]) ++breaks;      // wrap-around comparison
    return breaks <= 1;
}
```

**The insight:** a sorted-and-rotated array has **at most one** position where an element
exceeds its successor — the rotation point. A fully sorted array has zero. Two or more
breaks means it isn't a rotation of any sorted array.

The `% n` wrap is what makes it work: it also compares the last element against the first,
catching the case where the rotation point is at the boundary.

**Complexity:** O(n) / O(1). *Verified on sorted and unsorted inputs.*

---

## 4. Remove duplicates from a sorted array — LeetCode 26

```cpp
int removeDuplicates(vector<int> &a) {
    if (a.empty()) return 0;
    int k = 1;                                 // k = number of unique elements so far
    for (size_t i = 1; i < a.size(); ++i)
        if (a[i] != a[k-1]) a[k++] = a[i];     // a[k-1] is the last kept element
    return k;
}
```

**This is the slow/fast two-pointer partition**, and it's the idiom you'll reuse in #6, in
Remove Element, and in half a dozen later problems.

The invariant: **`a[0..k-1]` holds the unique elements found so far, in order.** `i` scans
ahead; whenever it finds something different from the last kept element, that becomes the
next kept element. Because the array is sorted, "different from the previous" is the same as
"not seen before" — which is why sorting matters here.

**Compare against `a[k-1]`, not `a[i-1]`.** Comparing against `a[i-1]` happens to work here,
but the `k-1` form is the one that generalises to "keep at most two of each" (LeetCode 80),
where you compare against `a[k-2]`.

**Dry run** — `[1,1,2,2,2,3,3]`: `i=1` equal, skip. `i=2` (2≠1) → `a[1]=2, k=2`. `i=3,4`
equal, skip. `i=5` (3≠2) → `a[2]=3, k=3`. Result `[1,2,3,...]`, k=3.

**Complexity:** O(n) time, **O(1) space**. *Verified.*

---

## 5. Rotate array by k — LeetCode 189

### Approach 1 — extra array, O(n) space

```cpp
vector<int> tmp(n);
for (int i = 0; i < n; ++i) tmp[(i + k) % n] = a[i];
a = tmp;
```

### Approach 2 — Optimal: reverse three times, O(1) space

```cpp
void rotateRight(vector<int> &a, int k) {
    int n = a.size();
    if (n == 0) return;
    k %= n;                                   // k can exceed n — this is essential
    reverse(a.begin(), a.end());              // whole array
    reverse(a.begin(), a.begin() + k);        // first k
    reverse(a.begin() + k, a.end());          // the rest
}
```

**Why it works.** Right-rotating by `k` means the last `k` elements move to the front,
preserving their relative order. Reversing the whole array brings those `k` elements to the
front but in reverse order; reversing each of the two blocks restores the internal order.

**Dry run** — `[1,2,3,4,5,6,7]`, `k=3`:

| step | result |
|---|---|
| reverse all | `7 6 5 4 3 2 1` |
| reverse first 3 | `5 6 7 4 3 2 1` |
| reverse rest | `5 6 7 1 2 3 4` ✓ |

**`k %= n` is mandatory** — `k = 10` on a 7-element array would make `a.begin() + k` point
past the end, which is undefined behaviour.

**Your `2_array.cpp:21` implements exactly this and is correct.** I traced it: `{1..8}` with
`k=5` gives `{4,5,6,7,8,1,2,3}`, matching your comment.

**Complexity:** O(n) time, **O(1) space**. *Verified: `{1..7}`, k=3 → `5 6 7 1 2 3 4`.*

---

## 6. Move zeroes — LeetCode 283

```cpp
void moveZeroes(vector<int> &a) {
    int j = 0;                                   // next position for a non-zero
    for (size_t i = 0; i < a.size(); ++i)
        if (a[i] != 0) swap(a[j++], a[i]);
}
```

**Same slow/fast partition as #4.** `j` marks where the next non-zero goes; `i` scans. The
`swap` (rather than assignment) is what pushes zeroes rightward for free — the element being
swapped *out* of position `j` is always either a zero or `a[i]` itself.

**Relative order of the non-zeroes is preserved**, which the problem requires. A two-pointer
version that swaps from both ends would be faster on writes but would scramble the order —
read the problem before choosing.

**Dry run** — `[0,1,0,3,12]`: `i=0` zero, skip. `i=1` → swap(a[0],a[1]) → `[1,0,0,3,12]`,
j=1. `i=3` → swap(a[1],a[3]) → `[1,3,0,0,12]`, j=2. `i=4` → swap(a[2],a[4]) →
`[1,3,12,0,0]` ✓

**Complexity:** O(n) / O(1). *Verified.*

---

## 7. Missing number — LeetCode 268

> `[0..n]` with exactly one number missing.

```cpp
int missingNumber(const vector<int> &a) {
    int n = a.size();
    long long expected = (long long)n * (n + 1) / 2;     // sum of 0..n
    for (int x : a) expected -= x;
    return (int)expected;
}
```

**Or with XOR, which cannot overflow at all:**

```cpp
int missingNumberXor(const vector<int> &a) {
    int r = a.size();                     // start with n
    for (int i = 0; i < (int)a.size(); ++i) r ^= i ^ a[i];
    return r;
}
```

**The arithmetic version needs `long long`.** For `n = 10⁵`, `n(n+1)/2 ≈ 5×10⁹` overflows
`int` — the *answer* is small but the *intermediate* isn't. Same lesson as `nCr` in
`../05_Function` #7.

**The XOR version sidesteps that entirely**, because `x ^ x == 0` means every present number
cancels its index and only the missing one survives. Mention it as the overflow-proof
alternative — that's the answer interviewers like.

**Complexity:** O(n) time, **O(1) space** — beats the O(n)-space hash-set approach.
*Verified: `{3,0,1}` → 2.*

---

## 8. Max consecutive ones — LeetCode 485

```cpp
int findMaxConsecutiveOnes(const vector<int> &a) {
    int best = 0, cur = 0;
    for (int x : a) { cur = (x == 1) ? cur + 1 : 0; best = max(best, cur); }
    return best;
}
```

**The reset-to-zero on a break is the whole algorithm**, and it's structurally identical to
Kadane's (#14): maintain a running quantity, reset it when continuing can't help, track the
best seen.

**Complexity:** O(n) / O(1). *Verified: `{1,1,0,1,1,1}` → 3.*

---

## 9. Single number — LeetCode 136

> Every element appears twice except one.

```cpp
int singleNumber(const vector<int> &a) {
    int r = 0;
    for (int x : a) r ^= x;
    return r;
}
```

**Why XOR works** — three properties: `x ^ x = 0`, `x ^ 0 = x`, and XOR is commutative and
associative. So the order doesn't matter: every pair cancels to 0, and the lone element XORs
against 0 to give itself.

The hash-map solution is O(n) time and O(n) space; this is O(n)/**O(1)**. That gap is the
entire point of the problem.

**Complexity:** O(n) / O(1). *Verified: `{4,1,2,1,2}` → 4.*

---

## 10. Union of two sorted arrays

```cpp
vector<int> unionSorted(const vector<int> &a, const vector<int> &b) {
    vector<int> res;
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        int v;
        if      (a[i] < b[j]) v = a[i++];
        else if (b[j] < a[i]) v = b[j++];
        else                  { v = a[i++]; j++; }        // equal: take once, advance both
        if (res.empty() || res.back() != v) res.push_back(v);   // skip internal duplicates
    }
    while (i < a.size()) { if (res.empty() || res.back() != a[i]) res.push_back(a[i]); ++i; }
    while (j < b.size()) { if (res.empty() || res.back() != b[j]) res.push_back(b[j]); ++j; }
    return res;
}
```

**Two distinct duplicate sources**, and you must handle both: the same value in *both*
arrays (the `else` branch), and the same value repeated *within* one array (the
`res.back() != v` check). Solutions that handle only the first fail on `[1,1,2]` ∪ `[2,3]`.

**Don't forget the tail loops** — when one array runs out, the rest of the other must still
be appended.

This is the **merge** step of merge sort, which you'll write again in `../12_sorting` and in
#29.

**Complexity:** O(n+m) time, O(n+m) output space, O(1) extra.
*Verified: `{1,2,3,4,5}` ∪ `{2,3,4,4,5,6}` → `1 2 3 4 5 6`.*

---

# Section 2 — Important

## 11. Two Sum — LeetCode 1

### Approach 1 — Brute force (this is your version, from the old `readme.md`)

```cpp
for (int i = 0; i < n; ++i)
    for (int j = i+1; j < n; ++j)
        if (nums[i] + nums[j] == target) return {i, j};
```

**O(n²) time, O(1) space.** Correct, and it's the right thing to say first in an interview.

### Approach 2 — Optimal: one pass with a hash map **[hash — `../24_maps`]**

```cpp
vector<int> twoSum(const vector<int> &a, int target) {
    unordered_map<int,int> seen;                  // value -> index
    for (int i = 0; i < (int)a.size(); ++i) {
        int need = target - a[i];
        if (seen.count(need)) return {seen[need], i};
        seen[a[i]] = i;                           // insert AFTER checking
    }
    return {-1,-1};
}
```

**Insert after checking, not before.** Otherwise `target = 6` with `a[i] = 3` finds *itself*
as its own complement and returns `{i, i}`.

**Why you cannot just sort and two-pointer.** The problem asks for **indices**, and sorting
destroys them. That constraint is the reason this problem exists — it's the cleanest example
of "the output format dictates the algorithm". (3Sum, #23, asks for *values*, so sorting is
free there.)

**Complexity:** O(n) average time, O(n) space. *Verified: `{2,7,11,15}`, target 9 → `[0,1]`.*

---

## 12. Sort Colors — Dutch National Flag — LeetCode 75

```cpp
void sortColors(vector<int> &a) {
    int lo = 0, mid = 0, hi = (int)a.size() - 1;
    while (mid <= hi) {
        if      (a[mid] == 0) swap(a[lo++], a[mid++]);
        else if (a[mid] == 1) mid++;
        else                  swap(a[mid], a[hi--]);   // do NOT advance mid here
    }
}
```

**The four-region invariant** — this is what makes it provably correct:

```
[0 .. lo-1]    all 0s
[lo .. mid-1]  all 1s
[mid .. hi]    UNKNOWN  <- shrinks each iteration
[hi+1 .. n-1]  all 2s
```

**`mid` does not advance in the `== 2` case.** The value swapped in from `hi` has never been
examined, so it must be processed. Advancing `mid` there is the classic bug and produces a
wrong answer on inputs like `[2,0,1]`.

**`mid <= hi`, not `mid < hi`** — the last unknown element sits at `mid == hi` and still
needs classifying.

**One pass, O(1) space.** The two-pass counting approach (count 0s/1s/2s, then overwrite) is
also valid and simpler, and your `3_array.cpp:36-44` does exactly that for the 0/1 case —
but it reads the array twice, and the interviewer's follow-up is always "can you do it in
one pass?"

**Dry run** — `[2,0,2,1,1,0]`: `mid=0` sees 2 → swap with hi=5 → `[0,0,2,1,1,2]`, hi=4.
`mid=0` sees 0 → swap with lo=0 (self), lo=mid=1. `mid=1` sees 0 → lo=mid=2. `mid=2` sees 2
→ swap with hi=4 → `[0,0,1,1,2,2]`, hi=3. `mid=2` sees 1 → mid=3. `mid=3` sees 1 → mid=4 >
hi → stop. Result `0 0 1 1 2 2` ✓

**Complexity:** O(n) / O(1). *Verified on `{2,0,2,1,1,0}`, `{2,2,2}`, `{0,1,2}`.*

> **Your `3_array.cpp` Dutch flag has the right algorithm but a broken `swap`.** See the bugs
> section — it returns `010` for `{0,1,2}`.

---

## 13. Majority Element (> n/2) — Moore's Voting — LeetCode 169

```cpp
int majorityElement(const vector<int> &a) {
    int count = 0, candidate = 0;
    for (int x : a) {
        if (count == 0) candidate = x;
        count += (x == candidate) ? 1 : -1;
    }
    return candidate;              // guaranteed correct only IF a majority exists
}
```

**Why it works — the cancellation argument.** Think of it as each occurrence of the majority
element cancelling one non-majority element. Since the majority appears **more than n/2**
times, it strictly outnumbers everything else combined, so it cannot be fully cancelled —
whatever survives is the majority.

More precisely: `count` tracks the surplus of the current candidate over everything seen
since the last reset. When `count` hits 0, the segment consumed so far is perfectly balanced,
so the majority of the *whole* array is still the majority of the *remainder*.

**The guarantee is conditional.** If no element exceeds n/2, this returns an arbitrary value.
When the problem doesn't promise a majority exists, **add a verification pass** counting the
candidate's occurrences — that's O(n) and O(1), so it costs nothing.

**Dry run** — `[2,2,1,1,1,2,2]`: cand=2,c=1 → c=2 → 1≠2,c=1 → c=0 → cand=1,c=1 → 2≠1,c=0 →
cand=2,c=1. Returns 2 ✓ (2 appears 4 times of 7).

**Complexity:** O(n) time, **O(1) space** — versus O(n) space for the hash-map count.
*Verified.*

---

## 14. Maximum Subarray — Kadane's — LeetCode 53

```cpp
int maxSubArray(const vector<int> &a) {
    long long best = LLONG_MIN, cur = 0;
    for (int x : a) {
        cur += x;
        best = max(best, cur);        // record BEFORE resetting
        if (cur < 0) cur = 0;         // a negative prefix never helps what follows
    }
    return (int)best;
}
```

**The correctness argument:** `cur` is the best sum of a subarray *ending here*. Extending a
negative running sum is strictly worse than starting fresh at the next element, so dropping
it is safe — not a heuristic, a proof.

**The all-negative case is the interview trap.** Initialising `best = 0` returns 0 for
`[-3,-1,-2]`; the correct answer is `-1`. Two things fix it: start `best` at `LLONG_MIN` (or
`a[0]`), and take the max **before** the reset. I verified both:
`[-2,1,-3,4,-1,2,1,-5,4]` → **6**, and `[-3,-1,-2]` → **−1**.

**To return the subarray itself**, track `start` (set to `i+1` whenever you reset) and record
`(start, i)` whenever `best` improves.

**Complexity:** O(n) / O(1).

---

## 15. Best Time to Buy and Sell Stock — LeetCode 121

```cpp
int maxProfit(const vector<int> &p) {
    int minSoFar = INT_MAX, best = 0;
    for (int x : p) {
        minSoFar = min(minSoFar, x);       // cheapest price seen so far
        best = max(best, x - minSoFar);    // sell today at the best profit
    }
    return best;
}
```

**The reframing that makes it trivial:** at each day, the best you can do is sell today,
having bought at the cheapest price seen *before* today. Track that minimum as you go.

**`best` starts at 0**, not `INT_MIN` — the problem allows *not trading*, so profit is never
negative. Note this is the opposite of #14's initialisation, and for a good reason: read what
the problem permits.

**This is Kadane's in disguise** — run Kadane's on the array of consecutive differences
`p[i] - p[i-1]` and you get the same answer. Saying that out loud is a strong signal.

**Complexity:** O(n) / O(1). *Verified: `{7,1,5,3,6,4}` → 5; `{7,6,4,3,1}` → 0.*

---

## 16. Rearrange array elements by sign — LeetCode 2149

> Equal counts of positives and negatives; result must alternate starting with a positive,
> preserving relative order within each sign.

```cpp
vector<int> rearrangeArray(const vector<int> &a) {
    int n = a.size();
    vector<int> res(n);
    int p = 0, q = 1;                          // even slots for +, odd slots for -
    for (int x : a) {
        if (x > 0) { res[p] = x; p += 2; }
        else       { res[q] = x; q += 2; }
    }
    return res;
}
```

**Two index cursors stepping by 2** — that's the whole trick. Positives fill 0,2,4,…;
negatives fill 1,3,5,…. Relative order is preserved automatically because each sign's cursor
only moves forward.

**This is O(n) space and that's necessary here.** An in-place version that preserves relative
order requires rotations and is O(n²); the O(1)-space two-pointer swap version (like your
`moveNegative`) *scrambles* the order. Read whether order matters.

**The harder variant** — unequal counts — appends the leftovers after alternating runs out.
Ask which version is being asked.

**Complexity:** O(n) / O(n). *Verified: `{3,1,-2,-5,2,-4}` → `3 -2 1 -5 2 -4`.*

---

## 17. Next Permutation — LeetCode 31

> The next lexicographically greater arrangement; if none exists, the smallest (sorted).

```cpp
void nextPermutation(vector<int> &a) {
    int n = a.size(), i = n - 2;

    // 1. Find the rightmost 'pivot' where a[i] < a[i+1]
    while (i >= 0 && a[i] >= a[i+1]) --i;

    // 2. If one exists, find the rightmost element greater than it, and swap
    if (i >= 0) {
        int j = n - 1;
        while (a[j] <= a[i]) --j;
        swap(a[i], a[j]);
    }

    // 3. Reverse the suffix (it was non-increasing, so this makes it smallest)
    reverse(a.begin() + i + 1, a.end());
}
```

**The reasoning, step by step:**

1. The suffix after the pivot is **non-increasing** — it's already the largest arrangement of
   those elements, so no change confined to it can increase the permutation. The pivot is the
   rightmost position that can be increased.
2. To increase by the *smallest* amount, swap the pivot with the **smallest element in the
   suffix that still exceeds it**. Scanning from the right finds it first, because the suffix
   is non-increasing.
3. After the swap the suffix is *still* non-increasing, and we want it as small as possible —
   so reverse it into ascending order. `reverse` suffices; no sort needed.

**When `i` becomes −1** the whole array is non-increasing (the last permutation). Step 2 is
skipped and step 3 reverses everything, giving the sorted array — exactly the required
wrap-around. **The code handles it with no special case**, which is why `i + 1` is written
that way.

**`a[i] >= a[i+1]` and `a[j] <= a[i]` use non-strict comparisons** to handle duplicates
correctly. Getting these wrong breaks `[1,1,5]`.

**Dry run** — `[1,2,3]`: pivot at i=1 (2<3). j=2 (3>2). Swap → `[1,3,2]`. Reverse suffix of
length 1 → `[1,3,2]` ✓
*Verified: `[1,2,3]`→`1 3 2`, `[3,2,1]`→`1 2 3`, `[1,1,5]`→`1 5 1`.*

**Complexity:** O(n) time, **O(1) space**.

---

## 18. Leaders in an array

> An element is a leader if it is **greater than everything to its right**.

```cpp
vector<int> leaders(const vector<int> &a) {
    vector<int> res;
    int mx = INT_MIN;
    for (int i = (int)a.size() - 1; i >= 0; --i)     // scan RIGHT to LEFT
        if (a[i] > mx) { res.push_back(a[i]); mx = a[i]; }
    reverse(res.begin(), res.end());                 // restore left-to-right order
    return res;
}
```

**Scanning right to left is the whole idea.** "Greater than everything to its right" is
expensive to check going forwards (O(n²)); going backwards, "everything to the right" is just
the running maximum you already have. **When a property depends on a suffix, iterate
backwards** — that's the transferable lesson, and it reappears in #28 and #30.

The rightmost element is always a leader (nothing is to its right), which `INT_MIN`
initialisation handles for free.

**Complexity:** O(n) time, O(n) output. *Verified: `{10,22,12,3,0,6}` → `22 12 6`.*

---

## 19. Longest Consecutive Sequence — LeetCode 128 **[hash]**

```cpp
int longestConsecutive(const vector<int> &a) {
    if (a.empty()) return 0;
    unordered_set<int> s(a.begin(), a.end());
    int best = 0;
    for (int x : s) {
        if (s.count(x - 1)) continue;            // not a sequence START — skip
        int y = x, len = 1;
        while (s.count(y + 1)) { ++y; ++len; }
        best = max(best, len);
    }
    return best;
}
```

**`if (s.count(x-1)) continue;` is what makes this O(n).** Without it, every element walks
its whole sequence and you get O(n²). With it, **only the smallest element of each sequence
ever starts a walk**, so across the entire loop each element is visited at most twice — once
in the outer scan, once in exactly one inner walk.

That amortisation argument is the interview question. Candidates who write the inner loop
without the guard get the right answer and the wrong complexity.

**The sorting alternative** is O(n log n): sort, then scan counting runs, skipping
duplicates. Simpler, slightly slower, O(1) extra space. Both are acceptable answers — say
which trade-off you're taking.

**Complexity:** O(n) average time, O(n) space.
*Verified: `{100,4,200,1,3,2}` → 4 (the run 1,2,3,4).*

---

## 20. Longest subarray with sum K

**This problem has two answers and knowing which applies is the point.**

### Case A — all elements non-negative: sliding window, O(1) space

```cpp
int longestSubarrayPositive(const vector<int> &a, long long k) {
    size_t l = 0; long long sum = 0; int best = 0;
    for (size_t r = 0; r < a.size(); ++r) {
        sum += a[r];
        while (l <= r && sum > k) sum -= a[l++];      // shrink from the left
        if (sum == k) best = max(best, (int)(r - l + 1));
    }
    return best;
}
```

**Why it needs non-negativity:** the window is only valid because growing `r` can never
*decrease* the sum and shrinking `l` can never *increase* it. That monotonicity is what makes
"shrink until the sum is small enough" correct. Introduce a negative and it collapses.

### Case B — any signs: prefix sum + hash map, O(n) space

```cpp
int longestSubarrayAny(const vector<int> &a, long long k) {
    map<long long,int> firstIndex;        // prefix sum -> EARLIEST index with that sum
    long long sum = 0; int best = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        sum += a[i];
        if (sum == k) best = i + 1;                                    // whole prefix works
        if (firstIndex.count(sum - k)) best = max(best, i - firstIndex[sum - k]);
        if (!firstIndex.count(sum)) firstIndex[sum] = i;               // keep the EARLIEST only
    }
    return best;
}
```

**`if (!firstIndex.count(sum))` is the graded line.** For the *longest* subarray you want the
**earliest** index with each prefix sum, so never overwrite. (For the *shortest*, you'd
overwrite every time.)

**Dry run** — `{10,5,2,7,1,9}`, k=15: prefixes 10,15,17,24,25,34. At i=1, sum=15=k → best=2.
At i=4, sum=25, and `25-15=10` was first seen at index 0 → best = 4−0 = **4** (the subarray
5,2,7,1) ✓ *Verified.*

**Complexity:** A is O(n)/O(1); B is O(n log n) with `map` or O(n) average with
`unordered_map`, O(n) space.

---

## 21. Subarray Sum Equals K — LeetCode 560 **[hash]**

> **Count** all subarrays summing to `k` (values may be negative).

```cpp
int subarraySum(const vector<int> &a, int k) {
    unordered_map<long long,int> count;
    count[0] = 1;                        // the empty prefix — ESSENTIAL
    long long sum = 0; int res = 0;
    for (int x : a) {
        sum += x;
        res += count[sum - k];           // how many earlier prefixes give a valid subarray
        count[sum]++;
    }
    return res;
}
```

**The core identity:** a subarray ending at `r` sums to `k` exactly when some earlier prefix
equals `sum - k`. So the number of such subarrays ending here is the *count* of earlier
prefixes with that value.

**`count[0] = 1` is the line everyone forgets.** It represents the empty prefix and is what
lets a subarray starting at index 0 be counted. Without it, `[1,2,3]` with `k=3` returns 1
instead of 2 — it misses `[1,2]`.

**Note the difference from #20:** there we stored *first index* (for longest); here we store
*frequency* (for count). Same pattern, different bookkeeping — driven by what's being asked.

**Complexity:** O(n) average time, O(n) space.
*Verified: `{1,1,1}`, k=2 → 2; `{1,2,3}`, k=3 → 2.*

---

# Section 3 — Good to Know

## 22. Majority Element II (> n/3) — LeetCode 229

```cpp
vector<int> majorityElementII(const vector<int> &a) {
    int c1 = 0, c2 = 0, n1 = INT_MIN, n2 = INT_MIN;
    for (int x : a) {
        if      (x == n1) ++c1;
        else if (x == n2) ++c2;
        else if (c1 == 0) { n1 = x; c1 = 1; }
        else if (c2 == 0) { n2 = x; c2 = 1; }
        else { --c1; --c2; }                  // cancel THREE distinct values at once
    }
    vector<int> res;
    c1 = c2 = 0;
    for (int x : a) { if (x == n1) ++c1; else if (x == n2) ++c2; }   // MUST verify
    if (c1 > (int)a.size()/3) res.push_back(n1);
    if (c2 > (int)a.size()/3) res.push_back(n2);
    sort(res.begin(), res.end());
    return res;
}
```

**Why at most two answers exist:** if three distinct values each appeared more than n/3
times, the total would exceed n. So the answer set has size 0, 1, or 2 — which is why two
candidate slots suffice.

**The order of the `if` chain matters.** The `x == n1` / `x == n2` checks must come *before*
the `c1 == 0` / `c2 == 0` checks, or a value equal to an existing candidate could be
installed into the other slot, leaving duplicate candidates.

**The verification pass is mandatory here**, unlike #13 where a majority is guaranteed. The
voting phase only narrows the field to two *possibilities*.

**Complexity:** O(n) time, **O(1) space**.
*Verified: `{3,2,3}` → `3`; `{1,2}` → `1 2`.*

---

## 23. 3Sum — LeetCode 15 **[sort]**

```cpp
vector<vector<int>> threeSum(vector<int> a) {
    sort(a.begin(), a.end());
    vector<vector<int>> res;
    int n = a.size();
    for (int i = 0; i < n - 2; ++i) {
        if (i > 0 && a[i] == a[i-1]) continue;              // skip duplicate first elements
        int l = i + 1, r = n - 1;
        while (l < r) {
            long long s = (long long)a[i] + a[l] + a[r];    // cast: three ints can overflow
            if      (s < 0) ++l;
            else if (s > 0) --r;
            else {
                res.push_back({a[i], a[l], a[r]});
                while (l < r && a[l] == a[l+1]) ++l;        // skip duplicate seconds
                while (l < r && a[r] == a[r-1]) --r;        // skip duplicate thirds
                ++l; --r;
            }
        }
    }
    return res;
}
```

**Sorting is free here** because the problem asks for *values*, not indices — the exact
opposite of Two Sum (#11).

**The structure:** fix the first element, then the remaining "find a pair summing to
`-a[i]`" is Two-Sum-on-a-sorted-array, which converging two pointers solve in O(n).

**Duplicate handling is the graded part**, and there are three separate places:
1. Skip repeated `a[i]` at the top of the outer loop (`i > 0 &&` guard is required, or you'd
   read `a[-1]`).
2. After recording a triple, skip repeated `a[l]`.
3. After recording, skip repeated `a[r]`.

Miss any one and you emit duplicate triples. Using a set to deduplicate afterwards works but
costs O(n log n) extra and reads as not having thought it through.

**Complexity:** **O(n²)** time (n outer × n two-pointer), O(1) extra beyond the output.
*Verified: `{-1,0,1,2,-1,-4}` → `[-1,-1,2]`, `[-1,0,1]`.*

---

## 24. 4Sum — LeetCode 18 **[sort]**

```cpp
vector<vector<int>> fourSum(vector<int> a, long long target) {
    sort(a.begin(), a.end());
    vector<vector<int>> res;
    int n = a.size();
    for (int i = 0; i < n - 3; ++i) {
        if (i > 0 && a[i] == a[i-1]) continue;
        for (int j = i + 1; j < n - 2; ++j) {
            if (j > i + 1 && a[j] == a[j-1]) continue;      // note: j > i+1, not j > 0
            int l = j + 1, r = n - 1;
            while (l < r) {
                long long s = (long long)a[i] + a[j] + a[l] + a[r];
                if      (s < target) ++l;
                else if (s > target) --r;
                else {
                    res.push_back({a[i], a[j], a[l], a[r]});
                    while (l < r && a[l] == a[l+1]) ++l;
                    while (l < r && a[r] == a[r-1]) --r;
                    ++l; --r;
                }
            }
        }
    }
    return res;
}
```

**Exactly 3Sum with one more loop.** The generalisation is worth stating: k-Sum is
`(k-2)` nested loops plus a two-pointer scan, giving **O(n^(k-1))**.

**Two things that specifically break 4Sum:**
- **`j > i+1`**, not `j > 0`, in the inner duplicate skip. With `j > 0` you'd wrongly skip a
  legitimate `a[j]` that merely equals `a[i]`.
- **The `long long` cast is mandatory here**, not optional. LeetCode's constraints allow
  four values near 10⁹, summing to 4×10⁹ — which overflows `int`. This is a real failing
  test case, not a hypothetical.

**Complexity:** **O(n³)** time, O(1) extra beyond output.
*Verified: `{1,0,-1,0,-2,2}`, target 0 → three quadruples.*

---

## 25. Largest subarray with sum 0 **[hash]**

```cpp
int longestZeroSum(const vector<int> &a) {
    map<long long,int> firstIndex;
    long long sum = 0; int best = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        sum += a[i];
        if (sum == 0) best = i + 1;                                  // whole prefix sums to 0
        else if (firstIndex.count(sum)) best = max(best, i - firstIndex[sum]);
        else firstIndex[sum] = i;
    }
    return best;
}
```

**This is #20 Case B with `k = 0`**, which simplifies beautifully: `sum - k == sum`, so you're
just looking for **a repeated prefix sum**. If the running sum is the same at indices `i` and
`j`, everything strictly between them sums to zero.

**Keep the earliest index** (the `else` before the assignment) — same reason as #20.

**Complexity:** O(n log n) with `map`, O(n) average with `unordered_map`. O(n) space.
*Verified: `{15,-2,2,-8,1,7,10,23}` → 5.*

---

## 26. Merge Intervals — LeetCode 56 **[sort]**

```cpp
vector<vector<int>> merge(vector<vector<int>> iv) {
    if (iv.empty()) return {};
    sort(iv.begin(), iv.end());                    // sorts by start, then end
    vector<vector<int>> res;
    res.push_back(iv[0]);
    for (size_t i = 1; i < iv.size(); ++i) {
        if (iv[i][0] <= res.back()[1])                             // overlaps the last merged
            res.back()[1] = max(res.back()[1], iv[i][1]);          // EXTEND, don't overwrite
        else
            res.push_back(iv[i]);
    }
    return res;
}
```

**Sorting by start time is what makes one pass sufficient.** After sorting, any interval that
overlaps the current merged block must be the *next* one — you never need to look further
ahead or backtrack.

**`max(res.back()[1], iv[i][1])` is the graded line.** Writing `res.back()[1] = iv[i][1]`
breaks on a **nested** interval: `[1,10]` followed by `[2,3]` would shrink the merged range
to `[1,3]`. Sorting guarantees ordered *starts*, not ordered *ends*.

**`<=` vs `<`** decides whether touching intervals like `[1,4]` and `[4,5]` merge. LeetCode 56
says they do. Read the problem.

**Complexity:** **O(n log n)** (sort dominates), O(n) output.
*Verified: `{{1,3},{2,6},{8,10},{15,18}}` → `[1,6] [8,10] [15,18]`.*

---

## 27. Merge two sorted arrays — LeetCode 88 and the O(1) variant

### Case A — LeetCode 88: `nums1` has room at the back

**This is your solution from the old `readme.md`, and it is correct:**

```cpp
void merge(vector<int> &a, int m, vector<int> &b, int n) {
    int i = m - 1, j = n - 1, k = m + n - 1;
    while (i >= 0 && j >= 0) {
        if (a[i] > b[j]) a[k--] = a[i--];
        else             a[k--] = b[j--];
    }
    while (j >= 0) a[k--] = b[j--];      // leftover b; leftover a is already in place
}
```

**Filling from the back is the entire insight.** Going forwards would overwrite elements of
`a` that haven't been merged yet, forcing a temp array. Going backwards, the write cursor `k`
is always at or beyond both read cursors, so nothing is clobbered — **O(1) extra space**.

**Only the `b` tail loop is needed.** If `a` still has elements left, they're already sitting
in their final positions. Writing the second loop anyway is harmless but unnecessary — you
worked this out correctly.

### Case B — two separate arrays, no spare room: the gap method

```cpp
void mergeGap(vector<int> &a, vector<int> &b) {
    int n = a.size(), m = b.size(), total = n + m;
    int gap = (total + 1) / 2;
    while (gap > 0) {
        int i = 0, j = gap;
        while (j < total) {
            int *pi = (i < n) ? &a[i] : &b[i - n];      // treat the two arrays as one
            int *pj = (j < n) ? &a[j] : &b[j - n];
            if (*pi > *pj) swap(*pi, *pj);
            ++i; ++j;
        }
        gap = (gap == 1) ? 0 : (gap + 1) / 2;
    }
}
```

**This is Shell sort's gap idea applied to a merge.** Compare elements `gap` apart across the
*conceptual* concatenation, swapping when out of order, then halve the gap. When the gap
reaches 1 and no swaps remain, both arrays are sorted and correctly partitioned.

**`(gap + 1) / 2` rounds up** — plain `gap / 2` can stall at 1 and loop forever, or skip the
final pass.

**Complexity:** Case A is O(n+m)/O(1). Case B is **O((n+m)·log(n+m))** time, **O(1) space** —
you trade a log factor for using no extra memory at all.
*Verified: `a={1,4,8,10}`, `b={2,3,9}` → `a=1 2 3 4`, `b=8 9 10`.*

---

## 28. Maximum Product Subarray — LeetCode 152

**Kadane's does NOT transfer, and understanding why is the problem.** With addition, a
negative prefix is always bad. With multiplication, a large *negative* product becomes the
*maximum* the moment another negative appears — so you cannot discard it.

```cpp
int maxProduct(const vector<int> &a) {
    long long best = LLONG_MIN, pre = 1, suf = 1;
    int n = a.size();
    for (int i = 0; i < n; ++i) {
        if (pre == 0) pre = 1;                 // a zero resets the run
        if (suf == 0) suf = 1;
        pre *= a[i];                           // running product from the LEFT
        suf *= a[n - 1 - i];                   // running product from the RIGHT
        best = max(best, max(pre, suf));
    }
    return (int)best;
}
```

**Why prefix *and* suffix products suffice.** The maximum-product subarray always extends to
one end of a zero-delimited block:

- With an **even** number of negatives in the block, the whole block is the answer.
- With an **odd** number, you must drop everything up to and including the first negative
  (giving a suffix) **or** everything from the last negative onward (giving a prefix). One of
  those two is optimal.

So scanning products from both ends and taking the best covers every case.

**Zeros reset both runs.** A zero makes the product 0, and any subarray crossing it is 0, so
each zero-free block is handled independently — that's what the `if (pre == 0) pre = 1;`
lines do.

**The alternative** — track running max *and* min, swapping them on a negative — is equally
valid and more famous. Both are O(n)/O(1).

**Complexity:** O(n) / O(1).
*Verified: `{2,3,-2,4}` → 6; `{-2,0,-1}` → 0; `{-2,3,-4}` → 24.*

---

## 29. Count Inversions

> Pairs `(i, j)` with `i < j` and `a[i] > a[j]`.

```cpp
long long countInv(vector<int> &a, int l, int r) {
    if (l >= r) return 0;
    int m = (l + r) / 2;
    long long c = countInv(a, l, m) + countInv(a, m + 1, r);

    vector<int> tmp;
    int i = l, j = m + 1;
    while (i <= m && j <= r) {
        if (a[i] <= a[j]) tmp.push_back(a[i++]);
        else { c += (m - i + 1); tmp.push_back(a[j++]); }   // THE counting line
    }
    while (i <= m) tmp.push_back(a[i++]);
    while (j <= r) tmp.push_back(a[j++]);
    for (int t = 0; t < (int)tmp.size(); ++t) a[l + t] = tmp[t];
    return c;
}
```

**`c += (m - i + 1)` is the whole algorithm.** During the merge, both halves are already
sorted. When `a[j]` (from the right half) is smaller than `a[i]`, then `a[j]` is smaller than
**every remaining element of the left half** — because the left half is sorted. That's
`m - i + 1` inversions counted in a single operation, which is what makes this O(n log n)
instead of O(n²).

**`<=` not `<`** in the comparison: equal elements are not inversions, and using `<` would
count them.

**`long long` for the count** — an array of 10⁵ reversed elements has ~5×10⁹ inversions,
which overflows `int`.

**Note this destroys the input** (it sorts it). Copy first if you still need the original —
I hit this in my own test harness.

**Complexity:** **O(n log n)** time, O(n) space.
*Verified: `{5,4,3,2,1}` → 10; `{2,4,1,3,5}` → 3.*

---

## 30. Trapping Rain Water — LeetCode 42

### Approach 1 — Brute force

For each bar, scan left and right for the tallest. Water above it is
`min(maxLeft, maxRight) - h[i]`. **O(n²) time, O(1) space.**

### Approach 2 — Precomputed prefix/suffix maxima

Build `leftMax[]` and `rightMax[]` in two passes, then sum. **O(n) time, O(n) space.**

### Approach 3 — Optimal: two pointers, O(1) space

```cpp
int trap(const vector<int> &h) {
    int l = 0, r = (int)h.size() - 1;
    int leftMax = 0, rightMax = 0, res = 0;
    while (l < r) {
        if (h[l] <= h[r]) {
            if (h[l] >= leftMax) leftMax = h[l];
            else res += leftMax - h[l];
            ++l;
        } else {
            if (h[r] >= rightMax) rightMax = h[r];
            else res += rightMax - h[r];
            --r;
        }
    }
    return res;
}
```

**Why the `h[l] <= h[r]` test makes it correct.** Water above bar `i` is
`min(leftMax, rightMax) - h[i]`. The difficulty is that you don't know `rightMax` while
scanning left.

The trick: **when `h[l] <= h[r]`, you know `rightMax >= h[r] >= h[l]`.** So the *left*
maximum is the binding constraint at position `l`, and you can compute the water there using
`leftMax` alone — without ever knowing the true `rightMax`. Symmetrically for the other
branch.

That is the whole argument, and being able to state it is what separates a memorised solution
from an understood one.

**Dry run** — `[0,1,0,2,1,0,1,3,2,1,2,1]`: the answer is **6** ✓ *Verified.*

**Complexity:** O(n) time, **O(1) space**.

**In an interview, walk all three approaches.** Going brute → prefix arrays → two pointers
shows the optimisation path, and this problem is specifically designed to reward that.

---

# Section 4 — Extra Practice (approach only)

### Reverse Pairs — LeetCode 493 (Hard)
Pairs with `a[i] > 2·a[j]`, `i < j`. Same merge-sort skeleton as #29, but the counting
**cannot** be folded into the merge comparison — the condition `a[i] > 2·a[j]` isn't the one
the merge uses. Add a **separate counting pass** with its own two pointers over the two
sorted halves *before* merging. Use `long long` for `2·a[j]`, which overflows `int`.
**O(n log n) time, O(n) space.**

### Count subarrays with XOR = K — GFG (Hard)
Identical to #21 with `^` replacing `+`. The identity becomes: a subarray has XOR `k` when
`prefixXor[r] ^ prefixXor[l] == k`, i.e. `prefixXor[l] == prefixXor[r] ^ k` (because XOR is
its own inverse). Map from prefix-XOR to frequency, seeded with `count[0] = 1`.
**O(n) average time, O(n) space.**

### Find the repeating and missing number — GFG
Two unknowns, so you need two equations. Let `S = Σa − Σ(1..n)` and `P = Σa² − Σ(1..n)²`.
Then `S = x − y` and `P = x² − y² = (x−y)(x+y)`, so `x + y = P / S`. Solve the pair.
**Use `long long`** — `Σi²` for `n = 10⁵` is ~3×10¹⁴. The XOR approach (partition by a
differing bit) is an alternative with no overflow risk. **O(n) time, O(1) space.**

### Container With Most Water — LeetCode 11 (Medium)
Converging two pointers. Area is `min(h[l], h[r]) × (r − l)`. **Always move the shorter
side**, because moving the taller one strictly reduces the width while the height stays
capped by the shorter bar — so it can never improve. A cleaner version of #30's argument.
**O(n) time, O(1) space.**

### Product of Array Except Self — LeetCode 238 (Medium)
Prefix products left-to-right, suffix products right-to-left, multiply. Do it in **O(1) extra
space** by writing prefixes into the output array, then sweeping backwards with a running
suffix variable. Division is banned precisely because a single zero breaks it. Same
prefix/suffix idea as #28. **O(n) time, O(1) extra.**

### Best Time to Buy and Sell Stock II — LeetCode 122 (Medium)
Unlimited transactions: sum every positive consecutive difference,
`Σ max(0, p[i] − p[i-1])`. The greedy is correct because any profitable multi-day run
decomposes into its positive daily steps, and buying/selling on the same day is free.
**O(n) time, O(1) space.**

### Remove Element — LeetCode 27 (Easy)
The slow/fast partition from #4 and #6, with the condition `a[i] != val`. If order doesn't
matter, the swap-with-the-end variant does fewer writes. **O(n) time, O(1) space.**

### Search Insert Position — LeetCode 35 (Easy)
Binary search returning `low` when not found — `low` lands exactly on the insertion point.
Use `mid = low + (high-low)/2` (`../01_basics` §9). Your gateway to
`../11_linearAndBinarySearch`. **O(log n) time, O(1) space.**

### Sort an array of 0s and 1s — GFG
Your `3_array.cpp:36-60` has both: the two-pass count-and-overwrite, and the two-pointer
version. Both are correct. The two-pointer one exploits that values are only 0/1 by
*assigning* rather than swapping — clever but fragile; it doesn't generalise to #12.
**O(n) time, O(1) space.**

### Move all negatives to one side — GFG
A two-way partition (Dutch flag with two buckets). **Your `3_array.cpp:9` version
infinite-loops on any input containing a zero** — see the bugs section. The fix is to treat
`>= 0` as one bucket rather than testing `> 0` and `< 0` separately. Note this does *not*
preserve relative order; if that's required, see #16. **O(n) time, O(1) space.**

---

# Bugs in your current code

Per the repo's convention I have not edited your `.cpp` files. All three compile cleanly;
these are logic bugs, all confirmed by running the code.

### `07_Array/3_array.cpp` — two serious bugs

| Line | Problem | Fix |
|---|---|---|
| 3-7 | **The additive `swap` corrupts an element when both pointers are the same.** `*a=*a+*b; *b=*a-*b; *a=*a-*b;` with `a == b` sets the value to **0**. Verified: `swapAdd(&x,&x)` with `x=7` gives **0** | use `std::swap`, or a temp variable |
| 63-80 | **Consequence: the Dutch flag returns a wrong answer.** When `mid == hi` and the value is 2, it self-swaps and becomes 0. Verified: `{0,1,2}` → **`010`**, `{2,2,2}` → **`022`** | fix the swap; the algorithm itself is correct |
| 9-27 | **`moveNegative` infinite-loops on any array containing a zero.** `0` is neither `> 0` nor `< 0`, so neither pointer ever advances. Verified: `{0,-1,2,-3}` hangs forever | test `arr[i] >= 0` / `arr[j] < 0` so zero belongs to a bucket |

**Note the 23-element test array in your file happens to produce the correct output** — I ran
it and it does. That is exactly what makes the swap bug dangerous: it passes your test and
fails on `{0,1,2}`.

The additive swap also **overflows** for large values (`*a + *b` can exceed `INT_MAX`), which
is undefined behaviour. There is no situation in which it beats `std::swap` — compilers emit
three register moves or nothing at all.

### `07_Array/2_array.cpp`

| Line | Problem | Fix |
|---|---|---|
| 47-51 | **Out-of-bounds read *and write*.** `v` has 4 elements (indices 0–3). `cout << v[4]` is undefined behaviour, and `v[4] = 3;` **writes past the end and corrupts the heap**. The comment calls it a "garbage value" — it's worse than that | `v.push_back(3);` to grow, or `v.resize(5)` first |
| 16 | `lastOccurence(vector<int> v, int trgt)` takes the vector **by value** — copies every element on every call | `const vector<int> &v` |
| 6-10 | `display(int a[])` hardcodes `5` because the array decayed to a pointer and the length is gone | `void display(const int a[], int n)` — see `../06_Pointer` §5 |
| 66-72 | The reverse uses the additive swap trick (`r[p1]=r[p1]+r[p2]; …`) — same overflow and self-aliasing issues as above | `swap(r[p1], r[p2])` |

**What's right here:** `rotateArray` (line 21) is **correct** — the reverse-reverse-reverse
method with `k %= v.size()` is the optimal O(1)-space approach, and your comment
`{4,5,6,7,8,1,2,3}` is accurate. That's solution #5. And `change(int b[])` demonstrating that
array modifications are visible to the caller is a genuinely good pointer-decay illustration.

### `07_Array/1_array.cpp`

| Line | Problem | Fix |
|---|---|---|
| 51 | `int max = -1` returns `-1` for an all-negative array | `INT_MIN`, or `arr[0]` |
| 33 | `int arr[n]` with runtime `n` is a **VLA** — a GCC extension, not standard C++, and it allocates on the ~1 MB stack | `vector<int> arr(n);` |
| 51 | The variable is named `max`, which shadows `std::max` under `using namespace std` | rename to `mx` |
| 24 | `int s = sizeof(x)/sizeof(x[0]);` is computed and never used | use it, or drop it |

**What's right:** using `sizeof(x)/sizeof(x[0])` at line 24 **in the scope where the array was
declared** is correct usage — that idiom only breaks inside a function, which is precisely the
distinction `../06_Pointer` §5 draws. Good instinct.
