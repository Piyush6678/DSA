# 07 — Arrays: Practice Questions

> **Why 30 ranked problems.** This is Striver A2Z Step 3 in full (minus the three matrix
> problems, which are in `../08_2d array`). Arrays are the single largest source of interview
> questions, and the count is driven by the seven core patterns — two pointers in both forms,
> sliding window, prefix sum, Kadane's, Dutch flag, Moore's voting — each of which needs
> several problems before it transfers, plus the hard-tier problems that *combine* two
> patterns. Thirty is what covers every pattern with enough repetition to make it automatic,
> without two problems teaching the same thing. This is the folder where a smaller list would
> genuinely cost you interviews.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title.

**Forward dependencies.** Some optimal solutions need hashing (`../24_maps`) or sorting
(`../12_sorting`), which Striver introduces before arrays but this repo places later. Those
are marked **[hash]** or **[sort]**. `solution.md` gives an in-scope approach for each as
well, so nothing here is blocked.

---

## Section 1 — Must Do (Easy tier — Striver 3.1)

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Largest element in an array | Easy | GFG | *"Largest Element in Array"* — also `1_array.cpp:49` |
| 2 | Second largest element (without sorting) | Easy | GFG | *"Second Largest"* |
| 3 | Check if an array is sorted (and rotated) | Easy | **LeetCode 1752** | `check-if-array-is-sorted-and-rotated` |
| 4 | Remove duplicates from a sorted array | Easy | **LeetCode 26** | `remove-duplicates-from-sorted-array` |
| 5 | Rotate array by k places | **Medium** | **LeetCode 189** | `rotate-array` — also `2_array.cpp:21` |
| 6 | Move zeroes to the end | Easy | **LeetCode 283** | `move-zeroes` |
| 7 | Missing number | Easy | **LeetCode 268** | `missing-number` |
| 8 | Max consecutive ones | Easy | **LeetCode 485** | `max-consecutive-ones` |
| 9 | Single number | Easy | **LeetCode 136** | `single-number` |
| 10 | Union of two sorted arrays | Easy | GFG | *"Union of Two Sorted Arrays"* |

**Why these ten.** They're the vocabulary. #1 and #2 look trivial and teach the two things
that break beginner code — `INT_MIN` initialisation and handling duplicates in "second
largest". #4 and #6 are both the **slow/fast two-pointer partition**, which is the single
most reused array idiom; get it here and five later problems are free. #5 has three
solutions (extra array, juggling, reverse-three-times) and the reverse trick is the one
worth knowing — you already wrote it in `2_array.cpp`. #7 and #9 both have an O(1)-space
trick (arithmetic sum; XOR) that beats the obvious hash. #10 is the merge step of merge sort,
which you'll need in `../12_sorting`.

---

## Section 2 — Important (Medium tier — Striver 3.2)

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Two Sum | Easy | **LeetCode 1** | `two-sum` — your brute force is in the old `readme.md` **[hash]** |
| 12 | Sort Colors (0s, 1s, 2s) | **Medium** | **LeetCode 75** | `sort-colors` — Dutch flag; also `3_array.cpp:63` |
| 13 | Majority Element (> n/2) | Easy | **LeetCode 169** | `majority-element` |
| 14 | Maximum Subarray (Kadane's) | **Medium** | **LeetCode 53** | `maximum-subarray` |
| 15 | Best Time to Buy and Sell Stock | Easy | **LeetCode 121** | `best-time-to-buy-and-sell-stock` |
| 16 | Rearrange array elements by sign | **Medium** | **LeetCode 2149** | `rearrange-array-elements-by-sign` — cf. `3_array.cpp:9` |
| 17 | Next Permutation | **Medium** | **LeetCode 31** | `next-permutation` — on your old list |
| 18 | Leaders in an array | Easy | GFG | *"Leaders in an array"* |
| 19 | Longest Consecutive Sequence | **Medium** | **LeetCode 128** | `longest-consecutive-sequence` **[hash]** |
| 20 | Longest subarray with sum K | **Medium** | GFG | *"Longest Sub-Array with Sum K"* **[hash]** |
| 21 | Subarray Sum Equals K | **Medium** | **LeetCode 560** | `subarray-sum-equals-k` **[hash]** |

**Why these eleven.** This is the interview core. **#12, #13 and #14 are the three named
algorithms** — Dutch national flag, Moore's voting, Kadane's — and all three are O(n)/O(1)
with a non-obvious correctness argument, which is exactly what interviewers probe. #11 is the
most-asked question in existence and its real lesson is *why you can't sort* (you need
indices). #15 is Kadane's in disguise, and seeing that is the point. **#17 is the most
mechanically demanding problem in this section** — a four-step algorithm where every step has
an off-by-one, and it's on your own to-do list. **#20 and #21 together teach the single most
transferable idea here**: sliding window when all values are non-negative, prefix-sum + hash
map when signs are mixed. #19 looks like it needs sorting and doesn't.

---

## Section 3 — Good to Know (Hard tier — Striver 3.3)

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 22 | Majority Element II (> n/3) | **Medium** | **LeetCode 229** | `majority-element-ii` |
| 23 | 3Sum | **Medium** | **LeetCode 15** | `3sum` **[sort]** |
| 24 | 4Sum | **Medium** | **LeetCode 18** | `4sum` **[sort]** |
| 25 | Largest subarray with sum 0 | **Medium** | GFG | *"Largest subarray with 0 sum"* **[hash]** |
| 26 | Merge Intervals | **Medium** | **LeetCode 56** | `merge-intervals` **[sort]** |
| 27 | Merge two sorted arrays without extra space | **Hard** | **LeetCode 88** | `merge-sorted-array` — your solution is in the old `readme.md` |
| 28 | Maximum Product Subarray | **Medium** | **LeetCode 152** | `maximum-product-subarray` |
| 29 | Count Inversions | **Hard** | GFG | *"Count Inversions"* — merge sort, cf. `../12_sorting` |
| 30 | Trapping Rain Water | **Hard** | **LeetCode 42** | `trapping-rain-water` — on your old list |

**Why these nine.** #22 generalises Moore's voting from one candidate to two and forces you
to prove why *at most two* elements can exceed n/3. #23 and #24 are the canonical
**sort-then-two-pointers** problems, and the graded part is duplicate skipping, not the sum.
#27 has a beautiful O(1)-space answer (the gap method) that almost nobody produces cold. **#28
is the best trap in the list** — Kadane's does *not* transfer, because a large negative times
another negative becomes the maximum, so you must track the minimum too. **#29 and #30 are
the two that most often decide a hard interview**: #29 is "modify merge sort to count", the
gateway to divide-and-conquer counting; #30 has four distinct solutions (brute, prefix
arrays, two pointers, monotonic stack) and walking an interviewer from O(n²) to O(n)/O(1) is
about as good as an array answer gets.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Reverse Pairs | **Hard** | **LeetCode 493** | #29's technique with a second counting pass — the natural follow-up |
| Count subarrays with XOR = K | **Hard** | GFG | *"Subarray with XOR K"* — prefix-sum pattern with `^` instead of `+` |
| Find the repeating and missing number | **Medium** | GFG | *"Find Missing And Repeating"* — two equations in two unknowns, O(1) space |
| Container With Most Water | **Medium** | **LeetCode 11** | converging two pointers; a cleaner cousin of #30 |
| Product of Array Except Self | **Medium** | **LeetCode 238** | prefix × suffix, no division — the trick behind #28 |
| Best Time to Buy and Sell Stock II | **Medium** | **LeetCode 122** | greedy: take every upward step |
| Remove Element | Easy | **LeetCode 27** | the same slow/fast partition as #4 and #6 |
| Search Insert Position | Easy | **LeetCode 35** | your first binary search — leads into `../11_linearAndBinarySearch` |
| Sort an array of 0s and 1s | Easy | GFG | the two-pass and two-pointer versions in `3_array.cpp:30-60` |
| Move all negatives to one side | Easy | GFG | `3_array.cpp:9` — a two-way partition, i.e. Dutch flag with two buckets |

---

## Progress tracker

```
Section 1  [ ]1  [ ]2  [ ]3  [ ]4  [ ]5  [ ]6  [ ]7  [ ]8  [ ]9  [ ]10
Section 2  [ ]11 [ ]12 [ ]13 [ ]14 [ ]15 [ ]16 [ ]17 [ ]18 [ ]19 [ ]20 [ ]21
Section 3  [ ]22 [ ]23 [ ]24 [ ]25 [ ]26 [ ]27 [ ]28 [ ]29 [ ]30
Section 4  [ ] ______ / 10
```
