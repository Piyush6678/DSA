# 11 — Linear & Binary Search: Practice Questions

> **Why 30 ranked problems.** Binary search is three separate skills that share ten lines of
> code: searching an **array** (where the trap is the loop invariant), searching an **answer
> space** (where the trap is writing a `feasible()` that actually depends on its argument), and
> searching a **2D grid**. The answer-space family alone needs five problems before the shape
> becomes recognisable — it is the one that does not look like binary search at all, and four
> problems on your own list belong to it. Add the lower/upper-bound primitives that everything
> else is built from, and the rotated-array family where the *pattern* transfers but the
> comparison changes each time, and 30 is where new ideas stop appearing.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title.

**Scope note.** Everything here needs only `01`–`10`. Two problems marked **`[hash]`** have a
map-based alternative that arrives in `../24_maps`; the binary-search solution is given and is
the one being taught.

**All ten problems from your `readme.md` are in Sections 1–2.**

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Binary Search | Easy | **LeetCode 704** | `binary-search` — **on your list**; `binary search.cpp:5` |
| 2 | Lower bound / Upper bound | Easy | GFG | *"Floor in a Sorted Array"* / *"Ceil The Floor"* — the primitives |
| 3 | Search Insert Position | Easy | **LeetCode 35** | `search-insert-position` — `lowerBound` verbatim |
| 4 | Find First and Last Position | **Medium** | **LeetCode 34** | `find-first-and-last-position-of-element-in-sorted-array` — **on your list** |
| 5 | Count occurrences of a number | Easy | GFG | *"Number of occurrence"* — `upperBound - lowerBound` |
| 6 | Sqrt(x) | Easy | **LeetCode 69** | `sqrtx` — **on your list**; **overflow trap** |
| 7 | Peak Index in a Mountain Array | **Medium** | **LeetCode 852** | `peak-index-in-a-mountain-array` — **on your list** |
| 8 | Search in Rotated Sorted Array | **Medium** | **LeetCode 33** | `search-in-rotated-sorted-array` — **on your list** |
| 9 | Find Minimum in Rotated Sorted Array | **Medium** | **LeetCode 153** | `find-minimum-in-rotated-sorted-array` |
| 10 | Single Element in a Sorted Array | **Medium** | **LeetCode 540** | `single-element-in-a-sorted-array` |
| 11 | Koko Eating Bananas | **Medium** | **LeetCode 875** | `koko-eating-bananas` — **on your list**; the answer-space archetype |
| 12 | Capacity To Ship Packages Within D Days | **Medium** | **LeetCode 1011** | `capacity-to-ship-packages-within-d-days` — **on your list** |
| 13 | Minimum Time to Complete Trips | **Hard** | **LeetCode 2187** | `minimum-time-to-complete-trips` — **on your list** |

**Why these thirteen.** #1 is where you fix `mid` and the loop guard permanently — everything
downstream depends on it. **#2 is the highest-leverage entry in the folder**: `lowerBound` and
`upperBound` make #3, #4 and #5 into one-liners, so learning them properly replaces learning
three problems. #4 is the standard follow-up to #1 and the first time "don't return on a match,
keep searching" appears. #6 looks trivial and is the overflow question in disguise.

#7 is your introduction to searching by a *predicate* (`am I still ascending?`) rather than by
value — no target exists to compare against. #8–#9 are the rotated family: #8's insight (one
half is always sorted) is the transferable one; #9 is the same array with a different question
and the comparison must be against `a[hi]`, not `a[lo]`. #10 is the cleverest of the group —
index **parity** is the predicate.

**#11–#13 are binary search on the answer, and they are the point of the folder.** #11 is the
cleanest statement of it. #12 adds the "lower bound must be the max element" subtlety. #13 needs
`long long` and forces you to get the direction of a division right — yours has it inverted.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 14 | Search in Rotated Sorted Array II | **Medium** | **LeetCode 81** | `search-in-rotated-sorted-array-ii` — duplicates break the trick |
| 15 | Find Peak Element | **Medium** | **LeetCode 162** | `find-peak-element` — no mountain guarantee |
| 16 | Find K Closest Elements | **Medium** | **LeetCode 658** | `find-k-closest-elements` — **on your list**; binary search the *window* |
| 17 | Sum of Square Numbers | **Medium** | **LeetCode 633** | `sum-of-square-numbers` — **on your list**; **yours is correct** |
| 18 | Split Array Largest Sum | **Hard** | **LeetCode 410** | `split-array-largest-sum` — identical to #12 |
| 19 | Allocate Minimum Number of Pages | **Hard** | GFG | *"Allocate Minimum Pages"* — the same problem again, Striver 5.2 |
| 20 | Aggressive Cows | **Hard** | GFG | *"Aggressive Cows"* — feasibility is a *maximum*, so the direction flips |
| 21 | Nth Root of a Number | **Medium** | GFG | *"Find Nth root of M"* — #6 generalised, overflow again |
| 22 | Search a 2D Matrix | **Medium** | **LeetCode 74** | `search-a-2d-matrix` — cf. `../08_2d array` #5 |
| 23 | Find Smallest Missing Non-negative | **Medium** | GFG | *"Find the smallest missing number"* — `binary search.cpp:61` |
| 24 | Kth Missing Positive Number | Easy | **LeetCode 1539** | `kth-missing-positive-number` — the "gap count" predicate |

**Why these eleven.** #14 exists to show you where #8's trick *breaks*: when
`a[lo] == a[mid] == a[hi]` you cannot tell which half is sorted, and the honest answer is to
shrink both ends and accept O(n) worst case. Knowing the limitation is worth as much as knowing
the technique. #15 drops the mountain guarantee — the same code works, which is a genuinely
surprising result worth understanding.

**#18, #19 and #20 are deliberately the same problem three times.** Split-array, book
allocation and aggressive cows are one algorithm with three stories, and doing them
consecutively is what makes the answer-space pattern stick. #20 is the odd one out: you want the
*largest* feasible distance, so the update directions reverse — that inversion is the exam.
#16 is a lovely non-obvious use: binary search over *window start positions*, not values. #23
uses the `a[mid] == mid` predicate, which is the same idea as #10.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 25 | Median of Two Sorted Arrays | **Hard** | **LeetCode 4** | `median-of-two-sorted-arrays` — binary search the *partition* |
| 26 | Kth Element of Two Sorted Arrays | **Hard** | GFG | *"K-th element of two sorted Arrays"* — #25 generalised |
| 27 | Minimum Days to Make m Bouquets | **Medium** | **LeetCode 1482** | `minimum-number-of-days-to-make-m-bouquets` |
| 28 | Find the Smallest Divisor Given a Threshold | **Medium** | **LeetCode 1283** | `find-the-smallest-divisor-given-a-threshold` — #11 relabelled |
| 29 | Search a 2D Matrix II | **Medium** | **LeetCode 240** | `search-a-2d-matrix-ii` — staircase, **not** binary search |
| 30 | Row with Maximum Number of 1s | **Medium** | GFG | *"Row with max 1s"* — `lowerBound` per row, or staircase |

**Why these six.** **#25 is the hardest binary search there is** and the one worth real
investment: you binary search the *cut position* in the shorter array, and the check is four
comparisons on the elements either side of both cuts. Getting `INT_MIN`/`INT_MAX` sentinels
right for the empty-side cases is most of the difficulty. #26 is the same machinery with the
median replaced by an arbitrary k, and doing it second makes #25 feel less magical.

#27 and #28 are more answer-space reps at a point where you should be able to write them
quickly — if #28 does not feel like #11 within a minute, do #11 again. **#29 is here as a
counterexample**: it looks like a binary search problem and the O(R+C) staircase beats the
O(R log C) row-by-row binary search. Recognising when *not* to reach for a technique is a real
skill. #30 closes the loop by combining `lowerBound` with the staircase.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Floor and Ceil in a sorted array | Easy | GFG | *"Floor in a Sorted Array"* — the two bounds, renamed |
| Guess Number Higher or Lower | Easy | **LeetCode 374** | binary search against an API instead of an array |
| First Bad Version | Easy | **LeetCode 278** | the boundary shape in its purest form |
| Valid Perfect Square | Easy | **LeetCode 367** | #6 with an exact test; same overflow trap |
| Arranging Coins | Easy | **LeetCode 441** | binary search on `k(k+1)/2` — watch the overflow |
| Find Smallest Letter Greater Than Target | Easy | **LeetCode 744** | `upperBound` with wraparound |
| Peak Element in a 2D Grid | **Medium** | **LeetCode 1901** | binary search on columns, max per column |
| Minimize Max Distance to Gas Station | **Hard** | GFG | *"Minimize Max Distance to Gas Station"* — real-valued answer space |
| Median in a Row-wise Sorted Matrix | **Hard** | GFG | *"Median in a row-wise sorted Matrix"* — count-based predicate |
| Capacity to ship — print the split | **Medium** | *Drill* | extend #12 to report the actual day groups |
| Binary search, recursively | Easy | *Drill* | one branch per level; O(log n) stack, cf. `../10_Recursion` |
| Count negative numbers in a sorted matrix | Easy | **LeetCode 1351** | staircase again |
| Ternary search | **Medium** | *Drill* | why it is **slower** than binary despite fewer iterations |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7
            [ ] 8   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13
Section 2   [ ] 14  [ ] 15  [ ] 16  [ ] 17  [ ] 18  [ ] 19  [ ] 20  [ ] 21  [ ] 22  [ ] 23  [ ] 24
Section 3   [ ] 25  [ ] 26  [ ] 27  [ ] 28  [ ] 29  [ ] 30
Section 4   [ ] ______ / 13
```
