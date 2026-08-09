# 13 — Prefix Sum: Practice Questions

> **Why 18 ranked problems.** Prefix sum is one identity, but it branches into five things that
> do not transfer to each other automatically: the plain 1D range query; the **prefix + hash map**
> pattern (which is the one that shows up in hard interviews and needs four problems before it
> is reflexive); the **prefix/suffix pair**, where you sweep in both directions; the **difference
> array**, which is the same identity run backwards; and **2D prefix**. Eighteen covers each with
> enough repetition to stick. Padding further would just be more subtraction — the topic is
> genuinely narrower than arrays or binary search, and the count reflects that honestly.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title.

**Scope note.** Problems marked **`[hash]`** need `unordered_map`, which arrives in
`../24_maps`. They are kept here because prefix + hash map is the defining application of this
topic — `solution.md` gives the map solution *and*, where one exists, an in-scope alternative.
Problems marked **`[sort]`** call `std::sort`, which you have from `../12_sorting`.

**All four problems from your `readme.md` are in Sections 1–2.**

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Running Sum of 1d Array | Easy | **LeetCode 1480** | `running-sum-of-1d-array` — **on your list**; `prefixsumAlgo.cpp` |
| 2 | Range Sum Query — Immutable | Easy | **LeetCode 303** | `range-sum-query-immutable` — the reason prefix sums exist |
| 3 | Find Pivot Index | Easy | **LeetCode 724** | `find-pivot-index` — cf. `ques.cpp` |
| 4 | Equilibrium Point / partition into equal halves | Easy | GFG | *"Equilibrium Point"* — **`ques.cpp` solves this correctly** |
| 5 | Product of Array Except Self | **Medium** | **LeetCode 238** | `product-of-array-except-self` — **on your list**; prefix × suffix |
| 6 | Subarray Sum Equals K | **Medium** | **LeetCode 560** | `subarray-sum-equals-k` `[hash]` — **the** prefix-sum problem |
| 7 | Longest Subarray with Sum K | **Medium** | GFG | *"Longest Sub-Array with Sum K"* `[hash]` — store the **first** index |
| 8 | Contiguous Array (equal 0s and 1s) | **Medium** | **LeetCode 525** | `contiguous-array` `[hash]` — map 0 → −1 |

**Why these eight.** #1 and #2 are the identity itself; #2 is the problem that names the
technique, and the `pre[0] = 0` padding that makes `l == 0` need no special case is the detail to
take from it. #3 and #4 look like the same problem and are **not**: #3 excludes the pivot element
from both sides, #4 splits the array in two with nothing excluded. Your `ques.cpp` answers #4
correctly, and running it against #3's test cases would fail — worth understanding why.

#5 is the prefix/suffix sweep, and the no-division constraint is the whole question. **#6 is the
most important problem in the folder** — the prefix + hash map identity turns O(n²) into O(n),
and #7 and #8 are the same trick with two variations that each catch people: #7 needs the
*earliest* index for each sum (never overwrite), and #8 needs the 0 → −1 relabelling that turns
"equal counts" into "sum zero". Do #6, #7, #8 consecutively.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 9 | Subarray Sums Divisible by K | **Medium** | **LeetCode 974** | `subarray-sums-divisible-by-k` `[hash]` — **negative modulo trap** |
| 10 | Minimum Penalty for a Shop | **Medium** | **LeetCode 2483** | `minimum-penalty-for-a-shop` — **on your list** |
| 11 | Reducing Dishes | **Hard** | **LeetCode 1402** | `reducing-dishes` — **on your list**; `[sort]` + suffix sums |
| 12 | Maximum Subarray (Kadane) | **Medium** | **LeetCode 53** | `maximum-subarray` — the prefix-minimum view |
| 13 | Range Sum Query 2D — Immutable | **Medium** | **LeetCode 304** | `range-sum-query-2d-immutable` — 2D prefix |
| 14 | Corporate Flight Bookings | **Medium** | **LeetCode 1109** | `corporate-flight-bookings` — the difference array |
| 15 | Range Addition | **Medium** | GFG | *"Range Addition"* — difference array, stated plainly |

**Why these seven.** **#9 is the sharpest trap in the folder**: `(-7) % 5` is `-2` in C++, not
`3`, so the naive modulo grouping silently loses matches. `((r % k) + k) % k` is the fix, and it
is worth meeting here rather than in a contest. #10 and #11 are your listed problems and both are
prefix/suffix in disguise — #10 tracks a running penalty in one pass, #11 needs the observation
that adding a dish at the front re-counts every dish after it, which is a suffix sum.

#12 is here to connect two things you have already met: Kadane's from `../07_Array` is exactly
"maximum `pre[r] − min(pre[l])` over `l < r`", which is the prefix-sum reading of the same
algorithm. #13 is 2D inclusion–exclusion. **#14 and #15 are the difference array** — the same
identity run backwards, and #14 is the one that makes it click because the "one read at the end"
structure is stated in the problem.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 16 | Count Number of Nice Subarrays | **Medium** | **LeetCode 1248** | `count-number-of-nice-subarrays` — #6 on parity |
| 17 | Binary Subarrays With Sum | **Medium** | **LeetCode 930** | `binary-subarrays-with-sum` — #6 again; also a two-window solution |
| 18 | Maximum Size Subarray Sum Equals k | **Medium** | **LeetCode 325** | `maximum-size-subarray-sum-equals-k` `[hash]` — #7 on LeetCode |

**Why these three.** All three are #6 and #7 wearing costumes, and that is the point: once you
recognise "count/find subarrays where some running quantity hits a target", the machinery is
identical. #16 replaces the sum with a count of odd numbers; #17 restricts values to 0/1 and
admits a second, map-free solution (`atMost(k) − atMost(k−1)`) that is worth knowing because it
generalises to sliding-window problems in `../14_Sliding window`. #18 is #7 with a judge attached
so you can verify yourself.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Left and right sum differences | Easy | **LeetCode 2574** | prefix/suffix in its simplest form |
| Find the Highest Altitude | Easy | **LeetCode 1732** | running sum, track the maximum |
| Number of Ways to Split Array | **Medium** | **LeetCode 2270** | one pass, compare left vs. right |
| Ways to Make a Fair Array | **Medium** | **LeetCode 1664** | **four** prefix sums (odd/even × left/right) |
| Trapping Rain Water | **Hard** | **LeetCode 42** | prefix-max and suffix-max; then two pointers |
| Candy | **Hard** | **LeetCode 135** | left-to-right then right-to-left sweep |
| Subarray Product Less Than K | **Medium** | **LeetCode 713** | prefix products overflow — use a window |
| XOR Queries of a Subarray | **Medium** | **LeetCode 1310** | prefix XOR; the inverse is XOR itself |
| Count Triplets with XOR | **Medium** | GFG | *"Count triplets with XOR"* — prefix XOR + counting |
| Matrix Block Sum | **Medium** | **LeetCode 1314** | 2D prefix with clamped bounds |
| Maximum Sum Rectangle in a Matrix | **Hard** | GFG | *"Maximum sum rectangle"* — column prefix + Kadane |
| Number of Submatrices That Sum to Target | **Hard** | **LeetCode 1074** | 2D prefix + #6 per column pair |
| Range sum with q queries | Easy | *Drill* | build once, answer q queries in O(1) each |
| Prefix XOR / prefix count of a value | Easy | *Drill* | prove to yourself that min/max **don't** work |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8
Section 2   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15
Section 3   [ ] 16  [ ] 17  [ ] 18
Section 4   [ ] ______ / 14
```
