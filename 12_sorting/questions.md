# 12 — Sorting: Practice Questions

> **Why 22 ranked problems.** Sorting splits into three things that do not overlap: (a)
> implementing the six algorithms, which is about invariants and complexity rather than problem
> solving; (b) the **cycle-sort placement family** — 268, 448, 287, 41 — which is one loop
> solving four O(1)-space problems and is the highest-value idea in the folder; and (c) problems
> where sorting is the *setup* and a greedy or two-pointer pass is the actual solution. Six
> implementations plus four cycle-sort plus a dozen application problems is 22. Fewer, and you
> skip either merge-sort's applications or the cycle family; more, and you are sorting arrays for
> practice, which is not a skill.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title.

**Scope note.** Everything needs only `01`–`11`. Problems marked **`[hash]`** have a map-based
alternative that arrives in `../24_maps`; the sorting solution is the one being taught here.

**All six problems from your `readme.md` are in Sections 1–2**, with 268/287/448/41 kept
consecutive because they are one technique.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Implement Bubble Sort | Easy | GFG | *"Bubble Sort"* — with the early-exit flag; `bubblesort.cpp` |
| 2 | Implement Selection Sort | Easy | GFG | *"Selection Sort"* — `selectionSort.cpp` |
| 3 | Implement Insertion Sort | Easy | GFG | *"Insertion Sort"* — `insertionSort.cpp`, **yours is correct** |
| 4 | Implement Merge Sort | **Medium** | GFG | *"Merge Sort"* — `mergeSort.cpp` |
| 5 | Implement Quick Sort | **Medium** | GFG | *"Quick Sort"* — `quickSort.cpp` |
| 6 | Sort Colors (Dutch National Flag) | **Medium** | **LeetCode 75** | `sort-colors` — three-way partition |
| 7 | Missing Number | Easy | **LeetCode 268** | `missing-number` — **on your list**; cycle placement |
| 8 | Find All Numbers Disappeared in an Array | Easy | **LeetCode 448** | `find-all-numbers-disappeared-in-an-array` — **on your list** |
| 9 | Find the Duplicate Number | **Medium** | **LeetCode 287** | `find-the-duplicate-number` — **on your list** |
| 10 | First Missing Positive | **Hard** | **LeetCode 41** | `first-missing-positive` — **on your list**; needs the range guard |
| 11 | Count Inversions | **Medium** | GFG | *"Count Inversions"* — merge sort's best-known application |

**Why these eleven.** #1–#5 are the implementations, and the value is in the details, not the
loops: #1's early-exit flag is what makes best case O(n); #2 is where `j < n` vs `j < n-1`
decides whether the last element ever gets placed; #4's base case must be `lo >= hi`, not
`n == 1`; #5's partition invariant is the thing to be able to state out loud. #6 is the
three-way partition — one pass, O(1) space, and the `hi` swap must **not** advance `mid`.

**#7–#10 are one technique and belong together.** Write the placement loop once, then each
problem differs only in the final scan. #10 is the hard one purely because the input can contain
negatives and values beyond `n`, so the range guard becomes load-bearing. Doing them out of order
or apart is the main reason people find #10 hard.

#11 is why merge sort earns its place: the merge step counts inversions for free, turning an
O(n²) problem into O(n log n).

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 12 | Merge Sorted Array | Easy | **LeetCode 88** | `merge-sorted-array` — fill from the back; cf. `../07_Array` |
| 13 | Sort an Array | **Medium** | **LeetCode 912** | `sort-an-array` — submit your own merge/quick |
| 14 | Majority Element | Easy | **LeetCode 169** | `majority-element` — **on your list**; Moore's voting |
| 15 | Assign Cookies | Easy | **LeetCode 455** | `assign-cookies` — **on your list**; sort both, two pointers |
| 16 | Merge Intervals | **Medium** | **LeetCode 56** | `merge-intervals` — **sorting is the whole solution** |
| 17 | Non-overlapping Intervals | **Medium** | **LeetCode 435** | `non-overlapping-intervals` — sort by **end**, not start |
| 18 | Largest Number | **Medium** | **LeetCode 179** | `largest-number` — the custom comparator problem |
| 19 | Sort Array By Parity | Easy | **LeetCode 905** | `sort-array-by-parity` — two-pointer partition |
| 20 | Kth Largest Element in an Array | **Medium** | **LeetCode 215** | `kth-largest-element-in-an-array` — quickselect |

**Why these nine.** #12 is the merge step standing alone, and doing it in O(1) space (filling
from the back) is the version interviewers want. #14 and #15 are your listed problems: **#14's
Moore's voting is not a sorting solution at all** — sorting gives an easy O(n log n) answer and
the O(n)/O(1) voting algorithm is the one worth knowing, so do both and compare. #15 is the
cleanest "sort then two-pointer greedy" in existence.

**#16 and #17 are the pair that teaches sort-key selection.** Merge Intervals sorts by **start**;
Non-overlapping Intervals sorts by **end**. Same data, opposite key, and getting #17 wrong by
sorting on start is the standard mistake — the greedy proof only works on end times. #18 is the
comparator problem: sort by `a+b > b+a` as strings, which is genuinely surprising and a good test
of whether you understand what a comparator is. #20 introduces quickselect, O(n) average, which
is partition without the second recursive call.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 21 | Minimum Swaps to Sort | **Medium** | GFG | *"Minimum Swaps to Sort"* — cycle counting |
| 22 | Reverse Pairs | **Hard** | **LeetCode 493** | `reverse-pairs` — #11 with `a[i] > 2*a[j]` |

**Why these two.** #21 is cycle sort's actual theorem: the minimum swap count is
`n − (number of cycles)` in the permutation, and computing it means pairing each value with its
sorted position. #22 is the natural hard follow-up to #11 — the same merge-sort scaffold, but the
counting pass must be **separate** from the merging pass because the comparison
(`a[i] > 2LL * a[j]`) is different from the one that decides merge order. Trying to fuse them is
the trap, and the `2 * a[j]` needs `long long`.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Sort an array of 0s and 1s only | Easy | *Drill* | two pointers; #6 with one fewer value |
| Sort Characters By Frequency | **Medium** | **LeetCode 451** | also in `../09_Strings` §2 #17 |
| Relative Sort Array | Easy | **LeetCode 1122** | custom comparator using a rank table |
| Sort Array by Increasing Frequency | Easy | **LeetCode 1636** | two-key comparator `[hash]` |
| H-Index | **Medium** | **LeetCode 274** | sort descending, then scan for the crossover |
| Meeting Rooms | Easy | GFG | *"Meeting Rooms"* — sort by start, check adjacent overlaps |
| Meeting Rooms II | **Medium** | GFG | *"Minimum Platforms"* — the two-sorted-array sweep |
| Wiggle Sort | **Medium** | **LeetCode 280** | one pass; no full sort needed |
| Pancake Sorting | **Medium** | **LeetCode 969** | only prefix reversals allowed |
| Heap Sort | **Medium** | GFG | *"Heap Sort"* — **forward reference**, needs `../25_heap` |
| Counting Sort | Easy | GFG | *"Counting Sort"* — O(n+k), beats the comparison bound |
| Radix Sort | **Medium** | GFG | *"Radix Sort"* — stable counting sort per digit |
| Sort a linked list | **Medium** | **LeetCode 148** | **forward reference** — merge sort, `../17_linked_list` |
| Check if an array is sorted | Easy | *Drill* | one pass; also the recursion drill in `../10_Recursion` |
| Sort a stack using recursion | **Medium** | GFG | *"Sort a stack"* — cf. `../10_Recursion` §3 #28 |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6
            [ ] 7   [ ] 8   [ ] 9   [ ] 10  [ ] 11
Section 2   [ ] 12  [ ] 13  [ ] 14  [ ] 15  [ ] 16  [ ] 17  [ ] 18  [ ] 19  [ ] 20
Section 3   [ ] 21  [ ] 22
Section 4   [ ] ______ / 15
```
