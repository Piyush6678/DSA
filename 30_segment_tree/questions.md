# 30 — Segment Trees: Practice Questions

> **Why 25 ranked problems.** Segment trees are implementation-heavy but conceptually one template with two
> extensions (lazy propagation and coordinate compression). 25 is what it takes to drill the core build-update-query
> template, cover the classic range-query applications that appear in interviews, practice lazy propagation until it
> stops being error-prone, and reach the hard compositions where segment trees combine with sorting, binary search,
> or other structures.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search the
quoted title.

**Scope note.** Everything needs only `01`–`29`. Segment trees often combine with other advanced topics like coordinate compression, line sweep geometry, and dynamic programming (`../26_dp`). Some problems solvable by segment trees are also solvable with Fenwick Trees / BIT or heaps (`../25_heap`).

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Implement a segment tree (sum) | Easy | *Drill* | build, update, query |
| 2 | Implement a segment tree (min/max) | Easy | *Drill* | |
| 3 | Range Sum Query - Mutable | **Medium** | **LeetCode 307** | |
| 4 | Range Minimum Query | **Medium** | GFG | *"Range Minimum Query"* |
| 5 | Range Sum Query - Immutable | Easy | **LeetCode 303** | for contrast with prefix sum |
| 6 | Range Sum Query 2D - Immutable | **Medium** | **LeetCode 304** | for contrast |
| 7 | Implement lazy propagation (range add + sum) | **Medium** | *Drill* | |
| 8 | Range XOR Queries | **Medium** | CSES / GFG | *"XOR of numbers in a range"* |
| 9 | Kth Smallest Element in a Sorted Matrix | **Medium** | **LeetCode 378** | heap is simpler, segment tree is alternative |
| 10 | Count of Smaller Numbers After Self | **Hard** | **LeetCode 315** | |

**Why these ten.** #1 and #2 are the raw implementation — you cannot do anything else without
them, and getting the three-case query logic (total overlap, no overlap, partial overlap) automatic
is the entire point. **#3 is the single most important problem in the folder** — it is literally #1
wrapped in a LeetCode interface, and it is the problem that proves you can translate the template
into a working submission. #4 swaps the merge function from `+` to `min()` and that one-line
change is worth seeing explicitly.

**#5 and #6 are deliberately NOT segment tree problems.** They exist for contrast — to make you
recognise when a prefix sum (`../13_prefixSum`) is enough and a segment tree is overkill. The
decision "do I need a segment tree here?" is as important as knowing how to build one, and these
two problems are the training set for that decision. #7 is lazy propagation, the second
fundamental template — without it, range updates cost O(n log n) and the structure loses its
purpose. #8 tests a different merge operation (XOR) and also teaches that XOR has a prefix-based
O(1) solution for static arrays, reinforcing the #5/#6 lesson. #10 is the most-asked Hard that
reduces to a segment tree used as a **frequency array** with coordinate compression — a technique
that appears in half of Section 2.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | My Calendar I | **Medium** | **LeetCode 729** | |
| 12 | My Calendar II | **Medium** | **LeetCode 731** | |
| 13 | My Calendar III | **Hard** | **LeetCode 732** | |
| 14 | Reverse Pairs | **Hard** | **LeetCode 493** | also solvable with merge sort |
| 15 | Count of Range Sum | **Hard** | **LeetCode 327** | |
| 16 | Range Module | **Hard** | **LeetCode 715** | |
| 17 | Falling Squares | **Hard** | **LeetCode 699** | |
| 18 | Number of Longest Increasing Subsequence | **Medium** | **LeetCode 673** | DP + segment tree optimization |

**Why these eight.** #11–#13 are a progressive series — the Calendar trilogy — where each level
adds one constraint: #11 is simple interval overlap detection (a `set` suffices), #12 needs
counting overlaps (reject if ≥ 3), and **#13 is a genuine segment tree problem** — range-add +
range-max query, and the answer is the global max after each booking. Doing all three
consecutively teaches you to recognise when a simpler structure suffices and when you genuinely
need a segment tree.

**#14 and #15 are the coordinate-compression pair.** Both require mapping huge values into a
compact range before the segment tree can index them — #14 counts `nums[i] > 2 * nums[j]` pairs
(merge sort is cleaner, but the segment tree approach drills compression), and #15 does the same
with prefix sums falling in a range. #16 is lazy propagation with **range assignment** (set to
true/false), not range addition — the pushDown logic replaces rather than accumulates, and
confusing the two is the standard bug. #17 combines coordinate compression with range-max query
and range assignment update. **#18 is the DP-optimisation entry** — it shows that a segment tree
storing `{length, count}` pairs can turn the O(n²) LIS-count DP into O(n log n), and recognising
that opportunity is harder than implementing it.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 19 | Create Sorted Array through Instructions | **Hard** | **LeetCode 1649** | |
| 20 | Longest Increasing Subsequence II | **Hard** | **LeetCode 2407** | segment tree optimized DP |
| 21 | Rectangle Area II | **Hard** | **LeetCode 850** | line sweep + segment tree |
| 22 | The Skyline Problem | **Hard** | **LeetCode 218** | multiple approaches, segment tree is one |
| 23 | Minimum Number of Increments on Subarrays to Form a Target Array | **Hard** | **LeetCode 1526** | |
| 24 | Count Good Meals | **Medium** | **LeetCode 1711** | related thinking; hash map is simpler |
| 25 | Online Majority Element In Subarray | **Hard** | **LeetCode 1157** | segment tree + randomization |

**Why these seven.** These are the hardest compositions, and most have cleaner non-segment-tree
solutions — the value of solving them with a segment tree is recognising the pattern, not
replacing the better approach. #19 is #10's twin (segment tree as frequency array). **#20 is the
best single test of the DP-optimisation pattern** from #18 — the segment tree stores max LIS
length per value, and for each element you query `[val-k, val-1]` for the best predecessor.

#21 is line sweep + segment tree on y-coordinates — the segment tree counts the total covered
length, and the sweep multiplies that by delta-x. #22 (Skyline) has a cleaner multiset/heap
solution and is included because interviewers sometimes ask "can you do it with a segment tree?"
— the answer is range-max-update + point-query, and knowing that is worth more than coding it.
#23 and #24 are deliberately weak segment tree problems — greedy and hash map are far simpler —
included to reinforce the #5/#6 lesson that **not using a segment tree is sometimes the right
answer**. #25 is competitive programming territory: the segment tree stores a Boyer-Moore
candidate per node, the merge is the "survivor" logic, and verification uses binary search on
sorted index lists.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Range GCD Query | **Medium** | GFG | |
| Range Update and Range Query using BIT | **Medium** | GFG | for BIT comparison |
| Merge Sort Tree (count elements ≤ k in range) | **Hard** | GFG / *Drill* | |
| Persistent Segment Tree basics | **Hard** | *Drill* | |
| Kth number in a range (persistent seg tree) | **Hard** | GFG | |
| Interval update with range assignment | **Medium** | *Drill* | |
| Segment tree with node storing {sum, max, min} | **Medium** | *Drill* | |
| Inversion Count using segment tree | **Medium** | GFG | *"Count Inversions"* |
| Count of distinct elements in every window | **Medium** | GFG | |
| Sum of all subarray minimums | **Medium** | **LeetCode 907** | monotonic stack is better, segment tree is educational |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 2   [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15  [ ] 16  [ ] 17  [ ] 18
Section 3   [ ] 19  [ ] 20  [ ] 21  [ ] 22  [ ] 23  [ ] 24  [ ] 25
Section 4   [ ] ______ / 10
```
