# 26 — Dynamic Programming: Solutions

Approach and pseudocode first. Full C++ only for **fundamentals**, **implementations**, **hard**
problems and **tricks** — which in DP means the pattern **templates**, since every other problem in
a family is an edit of one. Everything shown has been compiled and run.

---

## Section 1 — Foundations and 1-D DP

### 1. Fibonacci, four ways — *(fundamental; do this first)*

**(a) Plain recursion** — O(2ⁿ). Where the recurrence comes from, and nothing else.

**(b) Memoised** — three lines added:

```cpp
int fibMemo(int n, vector<int>& dp) {
    if (n <= 1) return n;                        // 1. base case
    if (dp[n] != -1) return dp[n];               // 2. already computed?
    return dp[n] = fibMemo(n-1, dp) + fibMemo(n-2, dp);   // 3. compute, store, return
}
```

**(c) Tabulated** — the same recurrence as a loop:

```cpp
int fibTab(int n) {
    if (n <= 1) return n;
    vector<int> dp(n + 1, 0);
    dp[0] = 0; dp[1] = 1;
    for (int i = 2; i <= n; ++i) dp[i] = dp[i-1] + dp[i-2];      // i-2, NOT i+2
    return dp[n];
}
```

**(d) O(1) space** — the table only ever reads two cells back:

```cpp
int fibSpaceOptimised(int n) {
    if (n <= 1) return n;
    int prev2 = 0, prev1 = 1;
    for (int i = 2; i <= n; ++i) { int cur = prev1 + prev2; prev2 = prev1; prev1 = cur; }
    return prev1;
}
```

All four verified at n = 0, 1, 10 and 40 (102334155).

**This four-step sequence is the deliverable, not the answer to `fib`.** Every DP problem below is
solved by walking it: recursion → memo → table → space. Do it once here deliberately and it becomes
automatic.

**Overflow:** `fib(47)` exceeds a 32-bit `int`. Verified on this toolchain, `fib(90)` with an `int`
dp returns **−1581614984** against a true value of 2880067194370816120.

---

### 2 & 3. Climbing Stairs / Min Cost Climbing Stairs — LC 70, LC 746 *(fundamental)*

LC 70 is `fib` with different base cases: `ways[i] = ways[i-1] + ways[i-2]`.

LC 746 attaches a cost, and the O(1)-space form is the one to keep:

```cpp
int minCostClimbingStairs(vector<int>& cost) {
    int n = cost.size();
    int prev2 = cost[0], prev1 = cost[1];
    for (int i = 2; i < n; ++i) {
        int cur = cost[i] + min(prev1, prev2);
        prev2 = prev1; prev1 = cur;
    }
    return min(prev1, prev2);                    // you may start from step 0 OR step 1
}
```

**The final `min` is the part people miss** — you may step onto the top from either of the last two
steps, so the answer is not `dp[n-1]`. Verified: `{10,15,20}` → 15, the 10-element case → 6,
`{0,0,1,1}` → 1, a two-element input → 5.

**Your `dp.cpp` has both the memoised and the tabulated version of this**, and both are logically
correct (details in the bug section). Note yours modifies `cost` in place; the version above does
not, which matters if the caller still needs its input.

---

### 4. Frog jump with at most k distances

```
dp[0] = 0
for i in 1..n-1:
    dp[i] = INF
    for j in 1..k:
        if i - j >= 0:  dp[i] = min(dp[i], dp[i-j] + abs(height[i] - height[i-j]))
```

O(n·k). **The inner loop over choices is what generalises** — #3 has two hard-coded choices, and
this is what happens when there are k of them. Almost every later pattern has this loop.

---

### 5 & 6. House Robber — LC 198, LC 213 *(fundamental — the take/skip shape)*

```cpp
int rob(vector<int>& a) {
    int take = 0, skip = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        int newTake = skip + a[i];               // to take i, I must have skipped i-1
        int newSkip = max(skip, take);           // to skip i, either is fine
        take = newTake; skip = newSkip;
    }
    return max(take, skip);
}
```

**Compute both new values before assigning either.** Overwriting `take` first and then reading it
for `newSkip` is the classic bug, and it silently allows adjacent houses.

Verified: `{1,2,3,1}` → 4, `{2,7,9,3,1}` → 12, a single house → 5, empty → 0.

**LC 213** puts the houses in a circle, so the first and last are adjacent. Run the linear version
twice — once on `a[0..n-2]`, once on `a[1..n-1]` — and take the max. Handle `n == 1` separately.

The `take/skip` recurrence here is the same one knapsack, subset sum and LIS use.

---

### 7. Maximum Subarray — LC 53 (Kadane)

**Derive it as a DP:** let `dp[i]` = the best subarray sum **ending exactly at i**. Then
`dp[i] = max(a[i], dp[i-1] + a[i])` — either extend the previous best or start fresh here. The
answer is `max` over all `i`, not `dp[n-1]`.

Reduced to one variable that is Kadane's algorithm. **"Best ending exactly at i" is also the LIS
state** (§3a) — noticing the shared shape is worth more than either algorithm.

---

### 8. Best Time to Buy and Sell Stock — LC 121

One pass tracking the lowest price so far and the best profit against it. O(n)/O(1). It is #7 run
on the *differences* between consecutive prices, which is worth verifying on paper once.

Everything about it generalises in §3b — treat this as `k = 1`.

---

### 9. Decode Ways — LC 91

`dp[i]` = ways to decode the first `i` characters.

```
dp[i] += dp[i-1]   if s[i-1] is '1'..'9'                    (a single-digit letter)
dp[i] += dp[i-2]   if s[i-2..i-1] reads as 10..26           (a two-digit letter)
```

**The base cases are the whole problem.** `dp[0] = 1` (the empty string decodes one way), and a
leading `'0'` makes the answer 0. Test `"0"`, `"06"`, `"10"`, `"100"`, `"2101"` before submitting —
most wrong answers here are base-case bugs, not recurrence bugs.

---

### 10. Jump Game — LC 55

DP: `reachable[i]` is true if some earlier reachable `j` has `j + a[j] >= i`. O(n²).

**Greedy is O(n):** track the furthest index reachable so far; if the loop index ever passes it,
return false. This problem is in the folder to show that a correct DP is not always the right
answer — knowing when the greedy is *provable* is the skill.

---

### 11. Word Break — LC 139

```
words = set of dictionary words
dp[0] = true
for i in 1..n:
    for j in 0..i-1:
        if dp[j] and s[j..i-1] is in words:  dp[i] = true; break
```

O(n²) substrings × O(L) hashing. **`dp[j] && the piece from j to i`** is the front-partition shape
that §3c formalises — cut a prefix, recurse on the rest.

The Trie version (`../22_bst/advanced_tree_readme.md` §4) avoids the substring construction and is
the natural follow-up.

---

### 12. Perfect Squares — LC 279

`dp[n] = 1 + min over squares s <= n of dp[n - s]`. This is **coin change with coins
{1, 4, 9, 16, …}** — recognising that is the exercise. O(n√n).

---

## Section 2 — Grids, two sequences, knapsack

### 13–17. Grid DP — LC 62, 63, 64, 120, 931

The whole family in one template, space-optimised to a single row:

```cpp
int uniquePaths(int m, int n) {
    vector<int> row(n, 1);
    for (int i = 1; i < m; ++i)
        for (int j = 1; j < n; ++j)
            row[j] += row[j-1];        // row[j] is still "from above"; row[j-1] is "from the left"
    return row[n-1];
}
```

**The one-row trick works because `row[j]` has not been overwritten yet** — it still holds the
previous row's value — while `row[j-1]` has, so it holds this row's. Reading the same array in two
time states is the whole idea, and it is the 2-D version of `prev1`/`prev2`.

Verified: 3×7 → 28, 3×2 → 3, 1×1 → 1, 10×10 → 48620.

The variations:

| Problem | Change |
|---|---|
| **63** obstacles | an obstacle cell is 0 and contributes nothing |
| **64** min path sum | `+` becomes `min`, and the cell's own value is added |
| **120** triangle | fill **bottom-up** — it removes every boundary case |
| **931** falling path | three predecessors (`j-1`, `j`, `j+1`) instead of two |

**LC 174 Dungeon Game must be filled backwards**, from the destination to the start, because the
constraint (health must stay positive *throughout*) cannot be evaluated forwards. It is the best
lesson in the folder that **direction is a design decision**, not a formality.

---

### 18. Longest Common Subsequence — LC 1143 *(fundamental — the template)*

```cpp
int lcs(string a, string b) {
    int n = a.size(), m = b.size();
    vector<vector<int> > dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            dp[i][j] = (a[i-1] == b[j-1]) ? dp[i-1][j-1] + 1
                                          : max(dp[i-1][j], dp[i][j-1]);
    return dp[n][m];
}
```

`dp[i][j]` = the LCS of the first `i` characters of A and the first `j` of B. Match → extend the
diagonal; mismatch → drop a character from one side and take the better. **The `+1` offset between
table indices and string indices is where off-by-ones live** — `dp[i][j]` is about `a[i-1]`.

O(n·m) time and space; O(min(n,m)) space keeping two rows. Verified including empty strings and
`"bsbininm"`/`"jmjkbkjkv"` → 1.

**Four problems are a two-line edit of this table:** LC 583 is `n + m − 2·LCS`, LC 1092 builds the
supersequence by walking it backwards, LC 516 is the LCS of the string with its own reverse, and
LC 115 changes `max` to `+` to count instead of maximise.

---

### 19. Longest Common **Substring** — the trap

```
if a[i-1] == b[j-1]:  dp[i][j] = dp[i-1][j-1] + 1
else:                 dp[i][j] = 0                    # RESET -- contiguity broke
answer = max over the WHOLE table                     # not dp[n][m]
```

Two changes from #18: the mismatch resets to zero instead of inheriting, and the answer is the
table maximum. **The word "substring" versus "subsequence" is the only difference in the problem
statement** and it changes both.

---

### 20. Print the LCS

Walk backwards from `dp[n][m]`: if the characters match, prepend it and go diagonally; otherwise
move to the larger of `dp[i-1][j]` and `dp[i][j-1]`. O(n + m) after the table is built.

DP normally gives you a *number*; reconstructing the *answer* means replaying the decisions. Worth
doing once so it is not mysterious when a problem demands the actual sequence.

---

### 21. Edit Distance — LC 72 *(implementation — the second template)*

```cpp
int editDistance(string a, string b) {
    int n = a.size(), m = b.size();
    vector<vector<int> > dp(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i <= n; ++i) dp[i][0] = i;         // delete everything
    for (int j = 0; j <= m; ++j) dp[0][j] = j;         // insert everything
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            dp[i][j] = (a[i-1] == b[j-1])
                     ? dp[i-1][j-1]                    // free: characters already agree
                     : 1 + min(dp[i-1][j-1],           // replace
                          min(dp[i-1][j],              // delete from a
                              dp[i][j-1]));            // insert into a
    return dp[n][m];
}
```

**Three neighbours, three operations** — memorise which cell is which and the rest is boilerplate.
The base row and column are not decoration: they encode "turn a prefix into the empty string".

Verified: `"horse"→"ros"` = 3, `"intention"→"execution"` = 5, identical strings = 0, empty → 3.

LC 44 (wildcard) and LC 10 (regex) are this table with extra cases for `*` and `?`.

---

### 24. Distinct Subsequences — LC 115

Same table, counting instead of maximising:

```
if a[i-1] == b[j-1]:  dp[i][j] = dp[i-1][j-1] + dp[i-1][j]     # use this a[i-1], or skip it
else:                 dp[i][j] = dp[i-1][j]                     # can only skip
```

**`max` → `+` turns an optimisation DP into a counting DP.** Making that jump consciously is worth
more than the problem, because the same substitution converts half of §2c.

Use `long long` or the specified modulus — subsequence counts explode.

---

### 25. 0/1 Knapsack *(fundamental — the third template)*

```cpp
int knapsack(vector<int>& wt, vector<int>& val, int W) {
    vector<int> dp(W + 1, 0);
    for (size_t i = 0; i < wt.size(); ++i)
        for (int w = W; w >= wt[i]; --w)                     // BACKWARDS
            dp[w] = max(dp[w], dp[w - wt[i]] + val[i]);
    return dp[W];
}
```

**Iterating capacity backwards is what makes it 0/1.** `dp[w - wt[i]]` then still holds the value
from *before* item `i` was considered, so the item cannot be counted twice. Forwards, it holds the
post-item value and the item becomes reusable:

```cpp
for (int w = wt[i]; w <= W; ++w) dp[w] = max(dp[w], dp[w - wt[i]] + val[i]);   // UNBOUNDED
```

Verified on the same input: `{10,20,30}` / `{60,100,120}` with W = 50 gives **220** with the
backward loop and **300** with the forward one. One word, two different classic problems.

Write the 2-D `dp[i][w]` version first if the 1-D collapse is not obvious — the space optimisation
is the last step, not the first.

---

### 26–29. The subset-sum relabellings

```cpp
bool canPartition(vector<int>& nums) {              // LC 416
    int total = 0;
    for (size_t i = 0; i < nums.size(); ++i) total += nums[i];
    if (total % 2) return false;                    // an odd total can never split evenly
    int target = total / 2;
    vector<bool> dp(target + 1, false);
    dp[0] = true;                                   // the empty subset reaches 0
    for (size_t i = 0; i < nums.size(); ++i)
        for (int s = target; s >= nums[i]; --s)     // BACKWARDS -- 0/1
            if (dp[s - nums[i]]) dp[s] = true;
    return dp[target];
}
```

Verified: `{1,5,11,5}` yes, `{1,2,3,5}` no, `{1,1}` yes, odd total no.

The rest of the group is this table read differently:

- **#28 minimum subset sum difference** — same table; scan the reachable sums `s ≤ total/2` and
  return `total - 2s` for the largest.
- **#29 target sum** — assigning `+`/`−` to every element means the positive part `S₊` satisfies
  `S₊ − (total − S₊) = target`, so `S₊ = (total + target) / 2`. Count subsets summing to `S₊`.
  **Reject a non-integer or out-of-range `S₊` before starting.**

---

### 30 & 31. Coin Change I and II — LC 322, LC 518

**LC 322 — unbounded, minimising:**

```cpp
int coinChange(vector<int>& coins, int amount) {
    const int INF = 1000000000;
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;
    for (int a = 1; a <= amount; ++a)
        for (size_t c = 0; c < coins.size(); ++c)
            if (coins[c] <= a && dp[a - coins[c]] + 1 < dp[a])
                dp[a] = dp[a - coins[c]] + 1;
    return dp[amount] >= INF ? -1 : dp[amount];
}
```

**Use a large finite `INF`, not `INT_MAX`** — `INT_MAX + 1` overflows to a negative number and
becomes the "best" answer. Verified: `{1,2,5}` amount 11 → 3, `{2}` amount 3 → −1, amount 0 → 0,
and the greedy-trap case `{186,419,83,408}` amount 6249 → **20**.

**LC 518 — unbounded, counting, and the loop order is the whole problem:**

```
for each coin:                    # coin loop OUTSIDE
    for a = coin .. amount:       # amount loop INSIDE
        dp[a] += dp[a - coin]
```

This counts **combinations** — `{1,2}` and `{2,1}` are one answer. Swap the loops and you count
**permutations** (which is LC 377). The samples pass either way; the hidden tests do not.

---

## Section 3 — LIS, stocks, interval DP

### 32 & 33. Longest Increasing Subsequence — LC 300

**O(n²):** `dp[i]` = the length of the best increasing subsequence **ending exactly at i**;
`dp[i] = 1 + max(dp[j])` over all `j < i` with `a[j] < a[i]`. The answer is the max over all `i`.

**O(n log n) — the trick:**

```cpp
int lengthOfLIS(vector<int>& a) {
    vector<int> tails;                       // tails[k] = smallest tail of an LIS of length k+1
    for (size_t i = 0; i < a.size(); ++i) {
        vector<int>::iterator it = lower_bound(tails.begin(), tails.end(), a[i]);
        if (it == tails.end()) tails.push_back(a[i]);      // extends the longest run
        else *it = a[i];                                    // improves an existing length
    }
    return tails.size();
}
```

**`tails` is not an LIS.** It is only guaranteed to have the right *length* — its contents can be a
mixture that never occurred as a subsequence. Trying to print `tails` as the answer is the standard
mistake; printing the LIS needs a parent array alongside the O(n²) version (#34).

Why it works: keeping the *smallest possible* tail for each length leaves the most room for future
elements, and `tails` is sorted by construction, so `lower_bound` applies.

`lower_bound` (not `upper_bound`) gives the **strictly** increasing LIS; `upper_bound` gives the
non-decreasing variant. Verified: `{10,9,2,5,3,7,101,18}` → 4 by both methods, `{7,7,7,7}` → 1
(strict), empty → 0.

---

### 35–38. LIS in disguise

- **#35 Largest Divisible Subset** — sort ascending, then LIS with `a[i] % a[j] == 0` as the
  condition. Divisibility is transitive on a sorted list, which is why checking only the previous
  chosen element is enough.
- **#36 Longest String Chain** — sort by length, then LIS with "is a predecessor" as the condition.
- **#37 Number of LIS** — carry `count[i]` beside `dp[i]`: on a strictly better `j` **replace** the
  count, on an equal-length `j` **add** it. The replace/add distinction is the whole problem.
- **#38 Russian Doll Envelopes** — sort by width ascending and, **on equal widths, height
  descending**, then run #33 on the heights. The descending tie-break makes two same-width
  envelopes impossible to both pick. Without it the answer is silently too large.

---

### 39–43. Stocks — one state machine *(trick, code given)*

The general state is `dp[day][holding?][transactionsUsed]`. For **at most two transactions** it
collapses to four running values:

```cpp
int maxProfit3(vector<int>& p) {
    int buy1 = INT_MIN, sell1 = 0, buy2 = INT_MIN, sell2 = 0;
    for (size_t i = 0; i < p.size(); ++i) {
        buy1  = max(buy1,  -p[i]);            // holding, after the 1st buy
        sell1 = max(sell1, buy1 + p[i]);      // free,    after the 1st sell
        buy2  = max(buy2,  sell1 - p[i]);     // holding, after the 2nd buy
        sell2 = max(sell2, buy2 + p[i]);      // free,    after the 2nd sell
    }
    return sell2;
}
```

**Updating all four in one pass, in this order, is intentional** — each line may use the value the
line above just produced, which correctly models buying and selling on the same day (a no-op worth
zero). Verified: `{7,1,5,3,6,4}` → 7, `{3,3,5,0,0,3,1,4}` → 6, a falling market → 0, a monotone
rise → 4.

The rest of the family:

| Problem | Change |
|---|---|
| **121** at most 1 | keep only `buy1`/`sell1` — or just track the minimum so far |
| **122** unlimited | sum every positive consecutive difference; O(n), no state needed |
| **188** at most k | the same loop with `2k` values, or a `dp[k][2]` table |
| **309** cooldown | after selling you must idle a day — buying reads `sell` from **two** days ago |
| **714** fee | subtract the fee once per completed transaction |

**Write #41 (LC 188) last** and check that `k = 1` reproduces LC 121 and a large `k` reproduces
LC 122. If it does, the framework is right.

---

### 44. Matrix Chain Multiplication *(hard — the interval template)*

`dp[i][j]` = the minimum cost to multiply matrices `i..j`. The last multiplication splits the range
at some `k`, and you try every split:

```cpp
int mcm(vector<int>& dim) {                          // n matrices, dim has n+1 entries
    int n = dim.size() - 1;
    vector<vector<int> > dp(n + 1, vector<int>(n + 1, 0));
    for (int len = 2; len <= n; ++len)               // by INCREASING range length
        for (int i = 1; i + len - 1 <= n; ++i) {
            int j = i + len - 1;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; ++k) {            // the split point
                int cost = dp[i][k] + dp[k+1][j] + dim[i-1]*dim[k]*dim[j];
                if (cost < dp[i][j]) dp[i][j] = cost;
            }
        }
    return dp[1][n];
}
```

**Iterate by range length**, so both halves are always already computed — that is the topological
order for interval DP, and it is the piece that has no analogue in the earlier patterns.
**O(n²) states × O(n) splits = O(n³)**, which is the budget to expect from any interval formulation.

Verified: `{40,20,30,10,30}` → 26000, `{10,20,30,40,30}` → 30000, two matrices → 6000.

---

### 45. Burst Balloons — LC 312 *(hard — reverse the decision)*

Asking "which balloon do I burst **first**?" fails: bursting changes the neighbours of everything
that remains, so the subproblems are not independent.

Asking "which balloon do I burst **last** in this range?" works, because that balloon's neighbours
are then exactly the range boundaries, which are fixed. The two sides become independent
subproblems.

```
pad the array with 1 at both ends
dp[i][j] = max over k in (i, j) of  dp[i][k] + dp[k][j] + a[i]*a[k]*a[j]
```

**"Reverse the order of the decision" is the interval-DP move**, and this is the problem that
teaches it. O(n³).

---

### 46 & 47. Front partition — LC 132, LC 1043

A gentler interval shape: cut a **prefix** and recurse on the rest.

```
dp[i] = best over j >= i of  ( value(i..j) + dp[j+1] )
```

For LC 132, `value` is 1 cut if `s[i..j]` is a palindrome and impossible otherwise —
**precompute an `isPalindrome[i][j]` table first**, or the palindrome checks make it O(n³). For
LC 1043 it is `(j-i+1) * max(a[i..j])` with `j - i + 1 <= k`.

Do these before #45 if MCM was rough; the shape is the same and the recurrence is easier to see.

---

### 48–50. Palindromes — LC 516, LC 5, LC 647

**#48 Longest Palindromic *Subsequence* is one line given #18:** it is `LCS(s, reverse(s))`.

**#49/#50 are about *substrings*, and expanding around each centre beats the DP table:**

```cpp
string longestPalindrome(string s) {
    if (s.empty()) return "";
    int bestL = 0, bestLen = 1;
    for (int c = 0; c < (int)s.size(); ++c)
        for (int d = 0; d < 2; ++d) {                  // d=0 odd centre, d=1 even centre
            int l = c, r = c + d;
            while (l >= 0 && r < (int)s.size() && s[l] == s[r]) { --l; ++r; }
            if (r - l - 1 > bestLen) { bestLen = r - l - 1; bestL = l + 1; }
        }
    return s.substr(bestL, bestLen);
}
```

**2n − 1 centres, because a palindrome can be centred on a character or between two.** Forgetting
the even case is the standard bug and it silently misses `"bb"`. O(n²) time but **O(1) space**,
against the DP table's O(n²) space. LC 647 is the same loop counting expansions instead of tracking
the longest.

Verified: `"cbbd"` → `"bb"`, `"forgeeksskeegfor"` → `"geeksskeeg"`, `"ac"` → length 1.

*(Manacher's algorithm does it in O(n). Know the name; it is rarely required.)*

---

### 51 & 52 — briefly

- **#51 Minimum Cost to Cut a Stick** — MCM with the trick of adding sentinel cuts at 0 and L and
  sorting, so every range has well-defined boundaries. Then it is #44 exactly.
- **#52 Maximal Rectangle** — treat each row as the base of a histogram of consecutive ones above
  it, and run **largest-rectangle-in-histogram** (`../18_stack`'s monotonic stack) on every row.
  O(rows × cols). More a stack problem than a DP one, and it is here because people reach for DP
  first and get stuck.

---

## Bugs in your file — with evidence

`dp.cpp` was compiled and run. **Nothing in `26_dp/` was edited.**

### `dp.cpp:40` — the file does not compile

```cpp
cost[i]+=min(cost[i-1],cost[i-2]))
```

One `)` too many and no `;`. **Measured:**

```
26_dp/dp.cpp: In function 'int minCostClimbingStairs(std::vector<int>&, std::vector<int>&)':
26_dp/dp.cpp:40:38: error: expected ';' before ')' token
     cost[i]+=min(cost[i-1],cost[i-2]))
                                      ^
```

Nothing in the file runs until this is fixed — including the `fib` call in `main`, which is
otherwise correct. **With the line repaired to `cost[i] += min(cost[i-1], cost[i-2]);` and nothing
else changed, the tabulation was tested and is correct**: `{10,15,20}` → 15, the ten-element case →
6, `{0,0,1,1}` → 1.

### `dp.cpp:19` — `arr[i+2]` reads past the end and forwards in time

```cpp
for(int i =2;i<=n;i++){
    arr[i]=arr[i-1]+arr[i+2];        // should be arr[i-2]
}
```

Two separate faults in one typo. `arr[i+2]` is **out of bounds** for `i >= n-1`, and even in range
it reads a cell that has **not been computed yet** — a tabulation must only read backwards.

**Measured** (with a `return arr[n]` added so the result is observable):

| Call | Yours | Correct |
|---|---|---|
| `fiboTabulation(6)` | **1882206360** | 8 |
| `fiboTabulation(10)` | **1886406384** | 55 |

Uninitialised stack memory, not a crash — which is worse, because it looks like a number.

### `dp.cpp:14` — the function returns `void`

`fiboTabulation` computes `arr` and then discards it. Even fixed, it produces nothing observable.
Return `arr[n]`.

### `dp.cpp:15` — a VLA with an initialiser

```cpp
int arr[n+1]={0};
```

Variable-length arrays are a **GCC extension**, not standard C++, and combining one with `= {0}` is
extension-on-extension. It compiles here and will fail on MSVC and on stricter settings. Use
`vector<int> arr(n+1, 0);` — no cost, portable, and it also gets you bounds checking with `.at()`
while debugging.

Related: for `n = 0` or `n = 1`, `arr[1] = 1` on line 17 writes past a one-element array.

### `dp.cpp:39-41` — the tabulation mutates the caller's input

```cpp
for(int i=2;i<n;i++){ cost[i]+=min(cost[i-1],cost[i-2]); }
```

`cost` is a non-const reference, so the caller's vector is overwritten. **Measured** with the line
repaired: after `minCostClimbingStairs` on `{10,15,20}`, the caller's vector reads `10 15 30`.

The answer is right, and destroying the input is a real cost — a second call on the same vector
gives a different answer. Use a local `dp` array, or the two-variable version in §1 #3.

### `dp.cpp:34` — an unused parameter

`minCostClimbingStairs(cost, dp)` takes `dp` and never touches it; the tabulation works in `cost`.
Leftover from the memoised version above it. Harmless, but it makes the signature lie.

### `dp.cpp:45-59` — `uniquePath` is exponential

`uniquePathHelper` has no memo and no table, so it is O(2^(m+n)). `uniquePath(18,18)` would take
minutes. **Measured as correct** — `(3,7)` → 28, `(3,2)` → 3, `(1,1)` → 1, `(7,3)` → 28 — so the
recurrence is right and only the caching is missing. It is §2 #13 waiting for step 2 of the
four-step sequence.

### `dp.cpp:7` — declared and never used

`int ans2,ans1;` in `fib`. GCC flags it with `-Wall`.

### `dp.cpp:5` / `main` — `int` overflow

`fib` returns `int`, and Fibonacci exceeds a 32-bit `int` at n = 47. **Measured:** `fib(90)`
returns **−1581614984**. The `dp` vector is sized 100, so the call is "legal" and the arithmetic is
not. Use `long long`.

---

## What you got right

**Your memoised `fib` is exactly correct** — verified at n = 0, 1, 6, 10 and 40 (102334155). All
three parts are in the right order and the right form:

```cpp
if(n==1||n==0) return n;          // base case FIRST
if(dp[n]!=-1) return dp[n];       // cache check SECOND
return dp[n]=fib(n-1,dp)+fib(n-2,dp);   // compute-store-return in one expression
```

**Checking the base case before the cache is the correct order** (a base case need not be cached),
and `return dp[n] = ...` is the idiomatic single-expression form rather than three separate
statements. This is the template the entire folder is built on and you have it right.

**`minCostClimbingStairshelper` is also correct** — verified on `{10,15,20}` → 15 and on the
ten-element case → 6, including the `min(helper(n-1), helper(n-2))` at the call site, which is the
part of LC 746 people get wrong. Base case `if (i==0 || i==1) return cost[i]` is exactly right.

**You wrote the same problem two ways and kept both.** The memoised call is commented out on line
36 with the tabulation below it. That is the right instinct — the conversion between the forms
*is* the skill (readme §2), and having both versions of one problem side by side is worth more than
two problems solved one way each.

**`uniquePathHelper` has the right recurrence and the right base cases.** `sr>er || sc>ec → 0` and
`sr==er && sc==ec → 1` are correct, and it was verified against four LeetCode cases. The commented-
out `uniquePath(m-1,n) + uniquePath(m,n-1)` version above it shows you tried the direct formulation
first and then switched to explicit coordinates — which is the better choice, because the
coordinates are the DP state and the `(m,n)` version obscures that.

**`// array should be greater the n+1 size`** — this comment is correct and it is the kind of note
worth writing. Sizing `dp` to `n+1` when `dp[n]` is the answer is one of the two or three most
common DP crashes, and you flagged it for yourself before it bit you.

**The commented-out alternatives throughout the file** (the direct `uniquePath` recursion, the
memoised `minCostClimbingStairs` call, the `pair<string,int>` loop in `../23_sets`) are a good
habit. They are a record of what you tried, and in DP specifically the abandoned formulation is
often the one an interviewer asks you to describe.
