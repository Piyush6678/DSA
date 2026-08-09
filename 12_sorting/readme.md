# 12 — Sorting

Six algorithms, and you will almost never write any of them at work — `std::sort` exists. They
are here because interviewers use them to test whether you can reason about **invariants,
stability and complexity**, and because two of them (merge and quick) are the divide-and-conquer
templates that later problems reuse. Cycle sort in particular is not really a sorting algorithm
at all; it is the key to a whole family of "find the missing/duplicate number in O(1) space"
problems — including four of the six on your own list.

---

## 1. The comparison table

| Algorithm | Best | Average | Worst | Space | Stable? | In place? |
|---|---|---|---|---|---|---|
| Bubble | O(n) | O(n²) | O(n²) | O(1) | **yes** | yes |
| Selection | O(n²) | O(n²) | O(n²) | O(1) | **no** | yes |
| Insertion | O(n) | O(n²) | O(n²) | O(1) | **yes** | yes |
| Merge | O(n log n) | O(n log n) | **O(n log n)** | **O(n)** | **yes** | no |
| Quick | O(n log n) | O(n log n) | **O(n²)** | O(log n) | **no** | yes |
| Cycle | O(n²) | O(n²) | O(n²) | O(1) | no | yes |

Three facts from this table are asked constantly:

- **Bubble and insertion are O(n) on already-sorted input** — but only if bubble has the
  early-exit flag. Without it, it is O(n²) always. Selection is O(n²) *even on sorted input*,
  because it scans the whole remaining array regardless.
- **Merge sort is the only one with a worst case of O(n log n)** — quick sort degrades to O(n²)
  when the pivot is always extreme (e.g. last-element pivot on an already-sorted array).
- **Selection sort is the minimum-swaps algorithm**: exactly n−1 swaps, versus O(n²) for bubble.
  If writes are expensive (flash memory), that matters.

---

## 2. Stability, and why anyone cares

A sort is **stable** if equal elements keep their original relative order.

```
Input:   (Bob,25)  (Amy,22)  (Cat,25)      sort by age
Stable:  (Amy,22)  (Bob,25)  (Cat,25)      Bob still before Cat
Unstable:(Amy,22)  (Cat,25)  (Bob,25)      order between equals lost
```

It matters when you sort by **two keys in sequence**: sort by name, then stably by age, and you
get "by age, and alphabetical within each age". With an unstable sort the first pass is thrown
away.

**What makes merge sort stable is one character**: `if (a[i] <= b[j])` takes from the left half
on a tie. Change it to `<` and the sort still works but is no longer stable.

Selection sort is unstable because it swaps distant elements — `{2a, 2b, 1}` puts `1` where
`2a` was, jumping `2a` past `2b`. `std::sort` is **not** stable; `std::stable_sort` is.

---

## 3. Merge sort — the divide-and-conquer template

```cpp
void mergeParts(vector<int>& a, int lo, int mid, int hi) {
    vector<int> tmp;
    int i = lo, j = mid + 1;
    while (i <= mid && j <= hi) tmp.push_back(a[i] <= a[j] ? a[i++] : a[j++]);
    while (i <= mid) tmp.push_back(a[i++]);      // drain whichever half remains
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

**Sort indices, not copies.** Passing `(array, lo, hi)` avoids building sub-vectors at every
level. The version in `mergeSort.cpp` copies into fresh `a` and `b` vectors each call — that
still works in principle, but it is where its two bugs come from, and it costs O(n log n)
allocation.

**`if (lo >= hi) return;`, not `if (n == 1) return;`.** An empty range must also terminate.
`n == 1` alone recurses forever on an empty input — verified, `mergeSort.cpp` dies with a stack
overflow on a 4-element array for exactly this reason.

**Both drain loops are mandatory.** After the main loop one half still has elements; you cannot
know which, so write both `while`s unconditionally. Wrapping them in `if` tests is where the
other `mergeSort.cpp` bug lives.

---

## 4. Quick sort — and why Lomuto is the one to memorise

```cpp
int partition(vector<int>& a, int lo, int hi) {
    int pivot = a[hi], i = lo - 1;               // i = last index of the "small" region
    for (int j = lo; j < hi; ++j)
        if (a[j] <= pivot) swap(a[++i], a[j]);
    swap(a[i + 1], a[hi]);                       // put the pivot in its final place
    return i + 1;
}
void quickSort(vector<int>& a, int lo, int hi) {
    if (lo >= hi) return;
    int p = partition(a, lo, hi);
    quickSort(a, lo, p - 1);                     // pivot is DONE — excluded from both sides
    quickSort(a, p + 1, hi);
}
```

**The invariant.** At every point in the loop, `a[lo..i]` are ≤ pivot and `a[i+1..j-1]` are >
pivot. Eight lines, one invariant, no nested pointer dance — that is why it is the version to
know. Hoare's is faster but much easier to get wrong under pressure.

**After partitioning, the pivot is in its final position.** Both recursive calls exclude it. If
either one includes `p`, the range never shrinks and you get infinite recursion.

**Worst case O(n²)** on already-sorted input with a last-element pivot — every partition splits
0/n−1. Random or median-of-three pivot selection fixes it in practice.

---

## 5. Cycle sort — the real reason this folder matters

Cycle sort is O(n²) and nobody uses it to sort. But its **placement loop** solves an entire
family of interview problems in O(n) time and O(1) space:

```cpp
while (i < n) {
    int correct = a[i] - 1;                       // value v belongs at index v-1
    if (a[i] >= 1 && a[i] <= n && a[i] != a[correct]) swap(a[i], a[correct]);
    else ++i;
}
```

**Two details decide whether this terminates.**

1. **Compare `a[i] != a[correct]`, not `i != correct`.** With duplicates, the index test swaps
   the same two equal values back and forth forever. The value test notices the target slot
   already holds the right value and moves on. **Verified: the loop in `cycleSortAlgo.cpp` uses
   the index form and does not terminate on `{4,5,3,1,1}` — still running after two million
   swaps.**
2. **Range-guard before indexing.** `a[i] >= 1 && a[i] <= n` stops out-of-range values (negatives,
   huge numbers) from indexing outside the array. LeetCode 41 hands you exactly those.

Once every value sits at its own index, one scan answers the question:

| Problem | After placement, scan for | Answer |
|---|---|---|
| **268** Missing Number | first `a[k] != k` | `k` |
| **448** Disappeared Numbers | every `a[k] != k+1` | collect `k+1` |
| **287** Find the Duplicate | first `a[k] != k+1` | `a[k]` |
| **41** First Missing Positive | first `a[k] != k+1` | `k+1` |

Four problems, one loop, different final scan. All four are on your `readme.md` list — they
belong together and are best done consecutively.

---

## 6. Comparators — what you actually use in practice

```cpp
sort(v.begin(), v.end());                                        // ascending
sort(v.begin(), v.end(), greater<int>());                        // descending
sort(v.begin(), v.end(), [](int a, int b){ return a > b; });     // lambda
sort(p.begin(), p.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
    if (a.first != b.first) return a.first < b.first;            // by first ascending
    return a.second > b.second;                                  // ties: second descending
});
```

**The comparator must be a strict weak ordering** — in practice, return `true` only for
"strictly before". Writing `>=` instead of `>` makes `comp(x,x)` true, which breaks the ordering
contract and can crash `std::sort` with an out-of-bounds read. That is a real segfault, not a
wrong answer.

---

## Interview Q&A

**Q1. Which sorts are stable, and why does it matter?**
Bubble, insertion and merge are stable; selection, quick and heap are not. It matters for
multi-key sorting: sort by the secondary key first, then stably by the primary, and ties keep
the secondary order. An unstable sort discards the first pass.

**Q2. Why is merge sort O(n log n) in the worst case but quick sort O(n²)?**
Merge sort always splits exactly in half — the recursion depth is log n regardless of input.
Quick sort's split depends on the pivot; if the pivot is always the smallest or largest element
you get depth n, and n levels of O(n) partitioning is O(n²). Sorted input with a last-element
pivot is the classic trigger.

**Q3. If merge sort has the better worst case, why is `std::sort` based on quick sort?**
Constants and memory. Quick sort is in place (O(log n) stack) and has excellent cache locality;
merge sort needs an O(n) buffer. Real implementations use *introsort*: quick sort, switching to
heap sort when the recursion gets too deep (guaranteeing O(n log n)) and to insertion sort for
small ranges.

**Q4. Can you sort in better than O(n log n)?**
Not with comparisons — the decision-tree lower bound is Ω(n log n). But **non-comparison** sorts
beat it by exploiting structure: counting sort is O(n+k) for small integer ranges, radix sort is
O(d·(n+k)) for fixed-width keys. Cycle sort's placement loop is the same idea: it uses the
value-equals-index relationship instead of comparing.

**Q5. What is the minimum number of swaps to sort an array?**
n minus the number of cycles in the permutation. Selection sort achieves n−1 swaps in the worst
case; cycle sort achieves the true minimum, which is what it was designed for.

**Q6. How do you count inversions in an array?**
Piggyback on merge sort. During the merge, when you take `b[j]` from the right half before
`a[i]`, every remaining element of the left half (`mid - i + 1` of them) forms an inversion with
it. Total in O(n log n) instead of O(n²).

**Q7. Why does `sizeof(arr)/sizeof(arr[0])` fail inside a function?**
Because an array parameter **decays to a pointer**. `void f(int arr[])` is exactly
`void f(int* arr)`, so `sizeof(arr)` is the pointer size, not the array size. On this 32-bit
toolchain that is 4, and `4/4` gives `n == 1` — so the function silently does nothing. GCC warns:
`'sizeof' on array function parameter will return size of 'int*'`. Always pass the length
separately, or use a `vector`.

---

## Your original notes (preserved)

The problem list from this file is kept verbatim. **All six are covered** — see `questions.md`.

```
169 Majority element 

455 Asign cookies
268 Missing number(cycle sort )
287 duplicate number
448 numbers diappeared in an array
41 first missing positives
```

| Your entry | Now at |
|---|---|
| 169 Majority Element | `questions.md` §2 #14 — Moore's voting, O(1) space |
| 455 Assign Cookies | `questions.md` §2 #15 — sort both, two pointers |
| 268 Missing Number | `questions.md` §1 #7 — cycle-sort placement |
| 287 Find the Duplicate | `questions.md` §1 #9 |
| 448 Disappeared Numbers | `questions.md` §1 #8 |
| 41 First Missing Positive | `questions.md` §1 #10 — the hard one; needs the range guard |

Your grouping of 268/287/448/41 under "cycle sort" is exactly right — they are one technique,
and §1 #7–#10 keeps them consecutive for that reason.
