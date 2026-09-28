# 26 — Dynamic Programming: Practice Questions

> **Why 52 ranked problems — by far the largest set in the repo, and it is still the floor.** DP is
> not a technique you learn once; it is **ten patterns that share a vocabulary and nothing else.**
> Someone fluent in knapsack is helpless on interval DP. The count is what it is because each
> pattern needs enough repetitions for the *recognition* to become automatic, and recognition is the
> whole skill — once you know which pattern a problem is, the recurrence takes two minutes.
>
> The breakdown: **1-D linear** 12 (the foundation, and where recursion→memo→table→O(1) becomes
> mechanical), **grid** 5, **two sequences** 7 (LCS and edit distance are the templates that ten
> other problems are edits of), **knapsack** 7 (including the *one loop direction* that separates
> 0/1 from unbounded), **LIS-family** 7, **stock state machines** 5 (five problems, one framework —
> doing them together is worth triple doing them apart), and **interval/partition DP** 9 (the
> hardest family, and the one people skip).
>
> Fifty-two is about five per pattern. Below that a pattern gets one or two problems, which is
> enough to solve those two and not enough to recognise the third. This is also the folder where
> the count is genuinely load-bearing rather than a target — Striver's DP sheet is 56, and the
> overlap here is deliberate.

**Platform note.** LeetCode numbers are exact. GFG has no numeric IDs — search the quoted title.

**Scope note.** Everything needs only `01`–`26`. **`[impl]`** marks a structural drill.
**`[fwd]`** marks a problem that also needs a later folder.

**Your `readme.md` lists one problem, `lc 746`** — it is §1 #3, and both a memoised and a tabulated
version already exist in `dp.cpp`.

---

## Section 1 — Foundations and 1-D DP

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Fibonacci: recursion → memo → table → O(1) | Easy | *Drill* `[impl]` | `dp.cpp:5` — **your memo version is correct** |
| 2 | Climbing Stairs | Easy | **LeetCode 70** | `climbing-stairs` — fib in disguise |
| 3 | Min Cost Climbing Stairs | Easy | **LeetCode 746** | `min-cost-climbing-stairs` — **on your list** |
| 4 | Frog jump with at most k distances | **Medium** | *Drill* | #3 generalised — the inner loop appears |
| 5 | House Robber | **Medium** | **LeetCode 198** | `house-robber` — the take/skip shape |
| 6 | House Robber II | **Medium** | **LeetCode 213** | `house-robber-ii` — a circle; run #5 twice |
| 7 | Maximum Subarray | **Medium** | **LeetCode 53** | `maximum-subarray` — Kadane is a DP |
| 8 | Best Time to Buy and Sell Stock | Easy | **LeetCode 121** | `best-time-to-buy-and-sell-stock` |
| 9 | Decode Ways | **Medium** | **LeetCode 91** | `decode-ways` — the base cases are the problem |
| 10 | Jump Game | **Medium** | **LeetCode 55** | `jump-game` — DP works; greedy is O(n) |
| 11 | Word Break | **Medium** | **LeetCode 139** | `word-break` — `dp[i]` over prefixes + a set |
| 12 | Perfect Squares | **Medium** | **LeetCode 279** | `perfect-squares` — coin change in disguise |

**Why these twelve. #1 is the most valuable exercise in the entire folder** and it is not a judge
problem. Write `fib` four times — plain recursion, memoised, tabulated, and with two variables —
and time all four at n = 40. That conversion sequence is what you will do on every DP problem for
the rest of your life, and doing it once on a problem you already understand means you are learning
the *conversion*, not the problem.

#2 and #3 are the same recurrence with a cost attached; #4 generalises the two choices to k, which
is where the inner `for` loop over choices appears — the shape almost every later pattern uses.

**#5 is the take/skip archetype.** `dp[i] = max(a[i] + dp[i-2], dp[i-1])`. Knapsack, subset sum and
LIS are all this recurrence with a different constraint bolted on, so it is worth being able to
write from memory. #6 is #5 with the ends adjacent — run the linear version twice, once excluding
the first house and once excluding the last.

#7 is Kadane's algorithm, and it is worth *deriving* as a DP (`dp[i]` = best subarray ending exactly
at i) rather than memorising, because "best ending exactly at i" is the LIS state too. #8 is #7 on
the differences.

**#9 is where careless base cases get punished** — `"0"`, `"06"` and `"10"` all need thought, and
most wrong submissions are base-case bugs rather than recurrence bugs. #10 is here to show DP is not
always the answer: the O(n²) DP is correct, the greedy is O(n). #11 and #12 both have the "try every
split / every coin" inner loop that Section 2 formalises.

---

## Section 2 — Grids, two sequences, and knapsack

### 2a — Grid DP

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 13 | Unique Paths | **Medium** | **LeetCode 62** | `unique-paths` — `dp.cpp:45` is the brute force |
| 14 | Unique Paths II | **Medium** | **LeetCode 63** | `unique-paths-ii` — obstacles are zeros |
| 15 | Minimum Path Sum | **Medium** | **LeetCode 64** | `minimum-path-sum` — `min` instead of `+` |
| 16 | Triangle | **Medium** | **LeetCode 120** | `triangle` — bottom-up is much cleaner |
| 17 | Minimum Falling Path Sum | **Medium** | **LeetCode 931** | `minimum-falling-path-sum` — three choices |

**Why these five.** #13 is already in your `dp.cpp` as an exponential recursion — memoise it, then
tabulate it, then reduce it to one row. That is the same four-step sequence as #1, on a 2-D table,
and doing it deliberately here means you never have to think about it again.

#14 and #15 are #13 with one line changed each, which is exactly why they are grouped. **#16 is the
one that teaches direction**: filling from the bottom row upward removes all the boundary special
cases that the top-down version needs. If a grid DP is getting messy, try reversing the direction
before adding cases.

### 2b — Two sequences

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 18 | Longest Common Subsequence | **Medium** | **LeetCode 1143** | `longest-common-subsequence` — **the template** |
| 19 | Longest Common Substring | **Medium** | GFG | *"Longest Common Substring"* — contiguous, so the reset differs |
| 20 | Print the LCS | **Medium** | *Drill* | walk the table backwards from `dp[n][m]` |
| 21 | Edit Distance | **Medium** | **LeetCode 72** | `edit-distance` — three choices |
| 22 | Delete Operation for Two Strings | **Medium** | **LeetCode 583** | `delete-operation-for-two-strings` — `n + m − 2·LCS` |
| 23 | Shortest Common Supersequence | **Hard** | **LeetCode 1092** | `shortest-common-supersequence` — build it from the table |
| 24 | Distinct Subsequences | **Hard** | **LeetCode 115** | `distinct-subsequences` — counting, not maximising |

**Why these seven. #18 is the single most reusable table in DP.** `dp[i][j]` = the answer for the
first `i` of A and the first `j` of B; if the characters match, extend the diagonal, otherwise take
the better of dropping one character from either side. #22, #23 and LC 516 are all a two-line edit
of it, which is the point of doing them consecutively.

**#19 is the trap.** *Substring* means contiguous, so a mismatch **resets to 0** rather than
inheriting — and the answer is the maximum over the whole table, not `dp[n][m]`. One word in the
problem statement, two changes in the code.

#20 is worth doing once: DP tables usually get you a *number*, and reconstructing the *answer*
means walking backwards through the decisions. #21 is the other template — three operations,
three cells, and the base row/column are "delete everything" and "insert everything". #24 changes
`max` to `+` and turns the same table into a counting problem, which is a jump worth making
consciously.

### 2c — Knapsack

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 25 | 0/1 Knapsack | **Medium** | GFG | *"0 - 1 Knapsack Problem"* — **the template** |
| 26 | Subset Sum equal to a target | **Medium** | GFG | *"Subset Sum Problem"* — knapsack with booleans |
| 27 | Partition Equal Subset Sum | **Medium** | **LeetCode 416** | `partition-equal-subset-sum` — #26 with target = sum/2 |
| 28 | Minimum Subset Sum Difference | **Medium** | GFG | *"Minimum sum partition"* — read the whole last row |
| 29 | Target Sum | **Medium** | **LeetCode 494** | `target-sum` — `+`/`−` rewritten as a subset sum |
| 30 | Coin Change | **Medium** | **LeetCode 322** | `coin-change` — unbounded, minimising |
| 31 | Coin Change II | **Medium** | **LeetCode 518** | `coin-change-ii` — unbounded, **counting**; loop order matters |

**Why these seven. #25 and #30 differ by one loop direction** (readme §4) and everything else in
this group is a relabelling:

| Problem | It is really |
|---|---|
| #26 subset sum | 0/1 knapsack where value = weight, asking "can we hit exactly W?" |
| #27 partition | #26 with `target = total/2`, plus an odd-total early exit |
| #28 min difference | #26, then scan the reachable sums for the one closest to `total/2` |
| #29 target sum | #26 after the algebra `S₊ = (total + target) / 2` |
| #31 coin change II | unbounded knapsack counting combinations instead of minimising |

**#31 has the nastiest bug in the folder**: with the coin loop outside and the amount loop inside
you count *combinations*; swap them and you count *permutations*. The problem wants combinations.
Get this wrong and the sample still passes.

---

## Section 3 — LIS, stocks, and interval DP

### 3a — The LIS family

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 32 | Longest Increasing Subsequence, O(n²) | **Medium** | **LeetCode 300** | `longest-increasing-subsequence` |
| 33 | LIS in O(n log n) | **Hard** | **LeetCode 300** | the `lower_bound` version — a **trick** |
| 34 | Print the LIS | **Medium** | *Drill* | a parent array alongside `dp` |
| 35 | Largest Divisible Subset | **Medium** | **LeetCode 368** | `largest-divisible-subset` — sort, then LIS |
| 36 | Longest String Chain | **Medium** | **LeetCode 1048** | `longest-string-chain` — sort by length, then LIS |
| 37 | Number of Longest Increasing Subsequences | **Medium** | **LeetCode 673** | `number-of-longest-increasing-subsequence` — length **and** count |
| 38 | Russian Doll Envelopes | **Hard** | **LeetCode 354** | `russian-doll-envelopes` — the tie-break is everything |

**Why these seven. The LIS state is "best subsequence ending exactly at i"**, which is a different
shape from "best using the first i" and is why this family needs its own block. The answer is then
the max over all `i` rather than `dp[n]`.

**#33 is the only genuinely non-obvious algorithm in the folder.** Maintain `tails[k]` = the
smallest possible tail of an increasing subsequence of length `k+1`, and binary search each new
element into it. The array is **not** an LIS — it is only the right *length*, which is the fact
people trip on. O(n log n).

#35 and #36 are LIS after a sort, and recognising that is the whole problem — they look nothing
like #32 until you see it. **#38 is #33 with a vicious detail**: sort by width ascending and, on
equal widths, height **descending**, so that two envelopes of the same width can never both be
picked. Without that tie-break the answer is silently too large.

### 3b — Stocks: five problems, one state machine

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 39 | Best Time to Buy and Sell Stock II | **Medium** | **LeetCode 122** | `best-time-to-buy-and-sell-stock-ii` — unlimited |
| 40 | Best Time to Buy and Sell Stock III | **Hard** | **LeetCode 123** | `best-time-to-buy-and-sell-stock-iii` — at most 2 |
| 41 | Best Time to Buy and Sell Stock IV | **Hard** | **LeetCode 188** | `best-time-to-buy-and-sell-stock-iv` — at most k |
| 42 | Buy and Sell Stock with Cooldown | **Medium** | **LeetCode 309** | `best-time-to-buy-and-sell-stock-with-cooldown` |
| 43 | Buy and Sell Stock with Transaction Fee | **Medium** | **LeetCode 714** | `best-time-to-buy-and-sell-stock-with-transaction-fee` |

**Do these five in one sitting.** They are the same `dp[day][holding?][transactionsUsed]` machine
with a different constraint each time, and solved together they take an afternoon; solved a month
apart they are five separate problems you re-derive five times. #41 subsumes all of them — write it
last, then check that `k = 1` reproduces #8 and `k = ∞` reproduces #39.

This is also the clearest illustration of readme §3's "the state must be complete": #40 needs a
transaction count in the state and #42 needs a cooldown flag, and in both cases leaving it out
gives a solution that passes the samples.

### 3c — Interval and partition DP

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 44 | Matrix Chain Multiplication | **Hard** | GFG | *"Matrix Chain Multiplication"* — **the template** |
| 45 | Burst Balloons | **Hard** | **LeetCode 312** | `burst-balloons` — think **last**, not first |
| 46 | Palindrome Partitioning II | **Hard** | **LeetCode 132** | `palindrome-partitioning-ii` — front partition |
| 47 | Partition Array for Maximum Sum | **Medium** | **LeetCode 1043** | `partition-array-for-maximum-sum` |
| 48 | Longest Palindromic Subsequence | **Medium** | **LeetCode 516** | `longest-palindromic-subsequence` — LCS with the reverse |
| 49 | Longest Palindromic Substring | **Medium** | **LeetCode 5** | `longest-palindromic-substring` |
| 50 | Palindromic Substrings | **Medium** | **LeetCode 647** | `palindromic-substrings` — count them all |
| 51 | Minimum Cost to Cut a Stick | **Hard** | **LeetCode 1547** | `minimum-cost-to-cut-a-stick` — MCM with sentinels |
| 52 | Maximal Rectangle | **Hard** | **LeetCode 85** | `maximal-rectangle` `[fwd]` — rows + `../18_stack` |

**Why these nine — and why not to skip them.** Interval DP is the family most people never get to,
and it is where hard interviews live. **The shape is different from everything above**: the state is
a *range* `[i, j]`, and the recurrence loops over a **split point** `k` inside it. That is O(n²)
states × O(n) splits = O(n³), and recognising that budget is how you know you have the right
formulation.

**#44 is the template and #45 is the one that teaches the technique.** In burst balloons the
natural question "which balloon do I burst first?" leads nowhere, because bursting changes the
neighbours of everything. Asking **which balloon do I burst last** fixes the boundaries of both
sub-ranges and the recurrence appears. *"Reverse the order of the decision"* is the interval-DP
move.

#46 and #47 are **front partition** — a slightly different shape where you cut a prefix and recurse
on the rest — and are the gentler entry point; do them before #45 if #44 was rough. #48 is a
one-liner given #18 (LCS of the string with its own reverse), and putting it next to #49/#50 shows
how differently *subsequence* and *substring* behave. #51 is #44 with the trick of adding sentinel
cuts at both ends so the ranges are well defined.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Min Falling Path Sum II | **Hard** | **LeetCode 1289** | avoid the O(n³) by tracking best and second-best |
| Cherry Pickup II | **Hard** | **LeetCode 1463** | two robots, so the state has two columns |
| Dungeon Game | **Hard** | **LeetCode 174** | must be filled **backwards** — a good direction lesson |
| Ones and Zeroes | **Medium** | **LeetCode 474** | knapsack with **two** capacities |
| Last Stone Weight II | **Medium** | **LeetCode 1049** | #28 wearing a disguise |
| Combination Sum IV | **Medium** | **LeetCode 377** | permutations — the loop order opposite to #31 |
| Rod Cutting | **Medium** | GFG | *"Rod Cutting"* — unbounded knapsack, renamed |
| Wildcard Matching | **Hard** | **LeetCode 44** | two-sequence DP with `*` and `?` |
| Regular Expression Matching | **Hard** | **LeetCode 10** | the same, and harder |
| Interleaving String | **Medium** | **LeetCode 97** | `dp[i][j]` over two sources |
| Longest Arithmetic Subsequence | **Medium** | **LeetCode 1027** | state is `(index, difference)` — a map per index |
| Maximum Product Subarray | **Medium** | **LeetCode 152** | carry **both** max and min; negatives swap them |
| Unique Binary Search Trees | **Medium** | **LeetCode 96** | Catalan — and `../22_bst` §4 |
| House Robber III | **Medium** | **LeetCode 337** | **DP on a tree** — `../21_tree` §5 #35 |
| Binary Tree Cameras | **Hard** | **LeetCode 968** | DP on a tree, three states — `../21_tree` §5 #36 |
| Partition to K Equal Sum Subsets | **Medium** | **LeetCode 698** | **bitmask DP**, `dp[mask]` |
| Shortest Path Visiting All Nodes | **Hard** | **LeetCode 847** `[fwd]` | bitmask + BFS — `../27_graphs` |
| Count numbers with a digit property | **Hard** | *Drill* | **digit DP** — position, tight flag, carried state |
| Time all four fib versions at n = 40 | Easy | *Drill* | the point of §1 #1, measured |

---

## Progress tracker

```
S1 1-D       [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6
             [ ] 7   [ ] 8   [ ] 9   [ ] 10  [ ] 11  [ ] 12
S2 grid      [ ] 13  [ ] 14  [ ] 15  [ ] 16  [ ] 17
S2 2-seq     [ ] 18  [ ] 19  [ ] 20  [ ] 21  [ ] 22  [ ] 23  [ ] 24
S2 knapsack  [ ] 25  [ ] 26  [ ] 27  [ ] 28  [ ] 29  [ ] 30  [ ] 31
S3 LIS       [ ] 32  [ ] 33  [ ] 34  [ ] 35  [ ] 36  [ ] 37  [ ] 38
S3 stocks    [ ] 39  [ ] 40  [ ] 41  [ ] 42  [ ] 43
S3 interval  [ ] 44  [ ] 45  [ ] 46  [ ] 47  [ ] 48
             [ ] 49  [ ] 50  [ ] 51  [ ] 52
S4 extra     [ ] ______ / 19
```
