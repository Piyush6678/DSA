# 14 — Sliding Window: Practice Questions

> **Why 22 ranked problems.** Sliding window is one loop shape, but it splits into four things
> that do not transfer automatically: the **fixed** window (no shrink loop at all), the
> **variable** window in its two mirror-image forms (longest-valid vs. shortest-reaching — the
> single most common place people get it backwards), the **monotonic deque** for max/min, and the
> **`atMost(k) − atMost(k−1)`** conversion. Each needs three or four problems before the choice
> between them is automatic; the deque in particular is a different data structure, not a
> variation. Twenty-two covers all four with the string-window problems that make the frequency
> state concrete.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title.

**Scope note.** Problems marked **`[hash]`** keep window state in an `unordered_map`
(`../24_maps`). Most have an in-scope `int freq[26]` or `freq[128]` version instead — the
alphabet is fixed — and `solution.md` says which. **`[deque]`** marks problems needing
`std::deque`, which is standard-library and needs no earlier folder.

**Both problems from your `readme.md` are in Sections 1–2** (#4, #12).

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Maximum sum subarray of size K | Easy | GFG | *"Max Sum Subarray of size K"* — `ques.cpp:4` |
| 2 | Maximum Average Subarray I | Easy | **LeetCode 643** | `maximum-average-subarray-i` — #1 with a judge |
| 3 | First negative in every window of size K | Easy | GFG | *"First negative integer in every window of size k"* — `ques.cpp:30` |
| 4 | Minimum Size Subarray Sum | **Medium** | **LeetCode 209** | `minimum-size-subarray-sum` — **on your list**; shortest-window template |
| 5 | Longest Substring Without Repeating Characters | **Medium** | **LeetCode 3** | `longest-substring-without-repeating-characters` — cf. `../09_Strings` #6 |
| 6 | Max Consecutive Ones III | **Medium** | **LeetCode 1004** | `max-consecutive-ones-iii` — "at most k flips" |
| 7 | Longest Repeating Character Replacement | **Medium** | **LeetCode 424** | `longest-repeating-character-replacement` — the `maxFreq` trick |
| 8 | Fruit Into Baskets | **Medium** | **LeetCode 904** | `fruit-into-baskets` `[hash]` — at most 2 distinct |

**Why these eight.** #1–#3 are the fixed window, where there is no shrink loop and the whole
lesson is the one-line add/drop update. #3 is the one that needs a deque even at fixed size,
which is a useful early hint that §4's structure exists.

**#4 is the template problem for the shortest-window form** and the place to fix, permanently,
that you record the answer *inside* the `while` before shrinking. #5 is the longest-window form —
the mirror image — and doing it right after #4 is what makes the difference stick. #6, #7 and #8
are all "longest window with at most k violations", which is the single most common variable-window
shape in interviews. **#7 is the sharpest of them**: you never recompute `maxFreq` when the window
shrinks, and the reason that is still correct (the answer only ever grows) is a genuinely good
interview discussion.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 9 | Permutation in String | **Medium** | **LeetCode 567** | `permutation-in-string` — fixed window + freq array |
| 10 | Find All Anagrams in a String | **Medium** | **LeetCode 438** | `find-all-anagrams-in-a-string` — #9, all matches |
| 11 | Binary Subarrays With Sum | **Medium** | **LeetCode 930** | `binary-subarrays-with-sum` — the `atMost` conversion |
| 12 | Grumpy Bookstore Owner | **Medium** | **LeetCode 1052** | `grumpy-bookstore-owner` — **on your list** |
| 13 | Subarrays with K Different Integers | **Hard** | **LeetCode 992** | `subarrays-with-k-different-integers` `[hash]` — `atMost` again |
| 14 | Number of Substrings Containing All Three Characters | **Medium** | **LeetCode 1358** | `number-of-substrings-containing-all-three-characters` |
| 15 | Count Number of Nice Subarrays | **Medium** | **LeetCode 1248** | `count-number-of-nice-subarrays` — also `../13_prefixSum` §16 |
| 16 | Longest Substring with At Most K Distinct Characters | **Medium** | **LeetCode 340** | `longest-substring-with-at-most-k-distinct-characters` `[hash]` |

**Why these eight.** #9 and #10 are the same fixed window (the pattern length never changes) and
they are where a frequency array becomes the window state rather than a single number — that
generalisation is the point. **#11 and #13 are the `atMost` pair**: #11 is the gentle version and
#13 is the same trick on a problem that is otherwise genuinely hard, so doing them in that order
converts a Hard into a Medium.

**#12 is the cleverest problem in the folder** and worth extra time: you do not slide a window
over the totals, you slide it over the *gain* — the customers you would rescue by using the
secret technique there — and add that to a fixed base. Recognising that the answer decomposes
into "what I get anyway" plus "the best window of what I can save" is a transferable move. #14 is
the "shortest valid" form counted rather than measured, and #15 shows the same problem being
solvable by prefix sums *or* a window, which is a good comparison to make deliberately.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 17 | Sliding Window Maximum | **Hard** | **LeetCode 239** | `sliding-window-maximum` `[deque]` — **the** deque problem |
| 18 | Minimum Window Substring | **Hard** | **LeetCode 76** | `minimum-window-substring` — the hardest window there is |
| 19 | Longest Subarray with Absolute Diff ≤ Limit | **Medium** | **LeetCode 1438** | `longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit` `[deque]` |
| 20 | Maximum Points You Can Obtain from Cards | **Medium** | **LeetCode 1423** | `maximum-points-you-can-obtain-from-cards` — the **inverted** window |
| 21 | Minimum Number of K Consecutive Bit Flips | **Hard** | **LeetCode 995** | `minimum-number-of-k-consecutive-bit-flips` — window + difference array |
| 22 | Shortest Subarray with Sum at Least K | **Hard** | **LeetCode 862** | `shortest-subarray-with-sum-at-least-k` `[deque]` — **negatives break #4** |

**Why these six.** #17 is the monotonic deque in its purest form and must be written from memory.
**#18 is the canonical hard window** — variable size, a frequency map as state, and a `formed`
counter so validity is checked in O(1) rather than by scanning the map. #19 needs **two** deques
at once (one for the max, one for the min), which is the natural next step from #17.

**#20 is the trick.** You take cards from the two ends, so the *taken* elements are not
contiguous — but the ones you leave behind are. Invert it: find the minimum-sum window of size
`n − k` and subtract. That reframing is the whole problem. **#22 is the most instructive
entry in this section**: it is #4 with negative numbers allowed, which breaks the sliding window
outright, and the fix is a monotonic deque over the *prefix sums*. Doing #4 and #22 together is
the clearest possible demonstration of §1's precondition.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Contains Duplicate II | Easy | **LeetCode 219** | fixed window of size k, membership only |
| Max Consecutive Ones | Easy | **LeetCode 485** | no window needed — a run counter; good contrast |
| Substrings of Size Three with Distinct Chars | Easy | **LeetCode 1876** | fixed k=3 |
| Defuse the Bomb | Easy | **LeetCode 1652** | circular fixed window — index with `% n` |
| Minimum Recolors to Get K Consecutive Black | Easy | **LeetCode 2379** | fixed window, count `W`s |
| Maximum Erasure Value | **Medium** | **LeetCode 1695** | longest window with all-distinct + a running sum |
| Replace the Substring for Balanced String | **Medium** | **LeetCode 1234** | window *outside* the constraint — like #20 |
| Frequency of the Most Frequent Element | **Medium** | **LeetCode 1838** | sort, then window with a cost budget |
| Sliding Window Median | **Hard** | **LeetCode 480** | **forward reference** — two heaps, `../25_heap` |
| Longest Nice Subarray | **Medium** | **LeetCode 2401** | window state is a **bitmask** — cf. `../15_bitwise` |
| Count Subarrays With Score Less Than K | **Hard** | **LeetCode 2302** | `sum × length` is monotone, so a window works |
| First negative — no deque | Easy | *Drill* | fixed window with a queue of indices; fixes `ques.cpp:30` |
| Max-sum window and its start index | Easy | *Drill* | fixes the uninitialised `idx` at `ques.cpp:6` |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8
Section 2   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15  [ ] 16
Section 3   [ ] 17  [ ] 18  [ ] 19  [ ] 20  [ ] 21  [ ] 22
Section 4   [ ] ______ / 13
```
