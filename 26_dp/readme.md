# 26 — Dynamic Programming

Not a data structure. A **way of organising recursion** so the same subproblem is never solved
twice.

> **DP = recursion + a place to write the answers down.**

That is the whole idea. Everything else in this folder is (a) recognising which problems allow it
and (b) mechanically converting between the three forms it takes.

This folder is the largest in the repo and the one interviews weight most heavily. It is also the
one where reading solutions helps least — the skill is *finding the recurrence*, and you only get
that from failing to find a few.

---

## 1. When does DP apply?

Two conditions, both required:

| Condition | Meaning | Test |
|---|---|---|
| **Overlapping subproblems** | the same subproblem is reached by many paths | draw the recursion tree; do nodes repeat? |
| **Optimal substructure** | the best answer uses best answers to subproblems | can you build the answer from smaller answers? |

`fib(n)` calls `fib(n-2)` twice, `fib(n-3)` three times, `fib(n-4)` five times — the recursion tree
has O(2ⁿ) nodes but only **n distinct values**. That gap is the entire opportunity.

**Merge sort has optimal substructure but no overlap** — the two halves are disjoint, so nothing is
recomputed and memoising buys nothing. That is divide and conquer, not DP. Being able to say why
merge sort is *not* DP is a good sign you understand the definition.

**Greedy vs DP:** greedy commits to a local choice and never reconsiders. It is correct only when
that choice provably cannot be wrong. DP tries all choices and keeps the best. If you cannot prove
the greedy, use DP — and coin change is the standard example where greedy fails (§6).

---

## 2. The three forms

Every DP problem can be written three ways. **Learn to move between them mechanically.**

### (a) Plain recursion — write this first, always

```cpp
int fib(int n) {
    if (n <= 1) return n;                       // base case
    return fib(n-1) + fib(n-2);                 // recurrence
}
```

Exponential and useless as a solution, but it is where the recurrence comes from. **Do not skip it.**
Almost every DP mistake is a wrong recurrence dressed up in a correct table.

### (b) Memoisation (top-down) — add one array

```cpp
int fib(int n, vector<int>& dp) {
    if (n <= 1) return n;                       // base case FIRST
    if (dp[n] != -1) return dp[n];              // already computed?
    return dp[n] = fib(n-1, dp) + fib(n-2, dp); // compute, store, return
}
```

**Three lines added to (a), and nothing else changes.** That is the conversion, and it is the same
every time:

1. base case
2. `if (dp[state] != -1) return dp[state];`
3. `return dp[state] = <the original expression>;`

Your `dp.cpp` gets this exactly right, for both `fib` and `minCostClimbingStairshelper`.

**Choose the sentinel with care.** `-1` only works if `-1` is not a legal answer. For problems that
can legitimately return `-1`, use a separate `bool computed[]` or a value that genuinely cannot
occur.

### (c) Tabulation (bottom-up) — fill the array in dependency order

```cpp
int fib(int n) {
    if (n <= 1) return n;
    vector<int> dp(n + 1, 0);
    dp[0] = 0; dp[1] = 1;                       // base cases become initial values
    for (int i = 2; i <= n; ++i) dp[i] = dp[i-1] + dp[i-2];
    return dp[n];
}
```

**The loop direction must follow the dependencies.** `dp[i]` needs `dp[i-1]` and `dp[i-2]`, so `i`
increases. Writing `dp[i+2]` here reads a cell that has not been filled yet — that is the bug in
`fiboTabulation` in your file, and it produces garbage rather than a crash.

| | Memoisation | Tabulation |
|---|---|---|
| Written as | recursion | loops |
| Computes | only the states you need | every state |
| Risk | **stack overflow** at depth ~10⁵ | none |
| Space optimisation | hard | **easy** — see (d) |
| Easier to write | ✓ (mirrors the recursion) | ✗ (order must be derived) |

**Write memoisation in an interview**, then say "this tabulates to a loop, and then the space drops
to O(1)". That sequence is what they are listening for.

### (d) Space optimisation — only after tabulation works

If `dp[i]` reads only `dp[i-1]` and `dp[i-2]`, you never need the array:

```cpp
int prev2 = 0, prev1 = 1;
for (int i = 2; i <= n; ++i) { int cur = prev1 + prev2; prev2 = prev1; prev1 = cur; }
```

**O(n) → O(1) space.** The 2-D analogue keeps one or two rows instead of the full grid. This is
always the final follow-up, and it is mechanical once the table is right — which is why the order
matters: recursion → memo → table → space.

---

## 3. Finding the recurrence — the actual skill

Two questions, in this order:

1. **What is the state?** The smallest set of variables that fully describes a subproblem. `dp[i]`
   for "using the first i items", `dp[i][j]` for "first i of A and first j of B", `dp[i][w]` for
   "first i items with capacity w left".
2. **What are the choices at this state?** Almost always a small set: *take it or skip it*, *go
   right or go down*, *match these characters or do not*, *cut here or there*.

Then:

```
dp[state] = combine( dp[state after choice 1], dp[state after choice 2], ... )
```

**The state must be complete.** If two different situations map to the same state and want
different answers, the state is missing a dimension — that is the most common wrong turn, and it is
what "at most k transactions" adds to the stock problems.

**The `take / skip` shape covers a third of all DP:**

```
dp[i] = max( value[i] + dp[i - 1 - gap] ,      // take item i
             dp[i - 1] )                        // skip item i
```

House robber, knapsack, subset sum and LIS are all this.

---

## 4. The recognised patterns

| Pattern | State | Examples |
|---|---|---|
| **1-D linear** | `dp[i]` | fib, climbing stairs, house robber, decode ways |
| **Grid / 2-D** | `dp[i][j]` = best way to reach `(i,j)` | unique paths, min path sum, LC 63/64 |
| **Two sequences** | `dp[i][j]` = first i of A, first j of B | LCS, edit distance, distinct subsequences |
| **Knapsack** | `dp[i][w]` = first i items, capacity w | 0/1, unbounded, subset sum, coin change |
| **Interval / MCM** | `dp[i][j]` over a range, split at k | matrix chain, burst balloons, palindrome partition |
| **LIS-family** | `dp[i]` = best ending exactly at i | LIS, russian dolls, max chain |
| **State machine** | `dp[i][state]` | stocks with k transactions / cooldown |
| **Digit DP** | position + tight flag + carried state | count numbers with a property |
| **Bitmask** | `dp[mask]` = subset already used | TSP, assignment problems (n ≤ 20) |
| **DP on trees** | recursion returning a tuple per node | house robber III, tree diameter |

**Recognising the pattern is 80% of solving the problem**, and the only way to build that
recognition is volume within a pattern — which is how `questions.md` is organised.

### The two knapsack loops, which differ by one word

```cpp
// 0/1 -- each item at most once:  iterate capacity BACKWARDS
for (int w = W; w >= wt[i]; --w)  dp[w] = max(dp[w], dp[w - wt[i]] + val[i]);

// unbounded -- reuse allowed:     iterate capacity FORWARDS
for (int w = wt[i]; w <= W; ++w)  dp[w] = max(dp[w], dp[w - wt[i]] + val[i]);
```

Going backwards, `dp[w - wt[i]]` still holds the value from *before* this item was considered, so
the item cannot be taken twice. Going forwards it holds the value from *after*, so it can. **One
loop direction is the entire difference between the two classic knapsacks** — this is worth
writing on a card.

---

## 5. `dp` array sizing and the traps

```cpp
vector<int> dp(100, -1);        // your dp.cpp -- "array should be greater than n+1 size"
```

Your comment is right. Three related traps:

- **Size it `n+1`, not `n`**, whenever `dp[n]` is the answer.
- **`int` overflows.** Verified on this toolchain: `fib(90)` with an `int` dp returns
  **−1581614984**; the true value is 2880067194370816120. Fibonacci exceeds `int` at n = 47. Use
  `long long`, or the modulus the problem specifies.
- **`int arr[n+1] = {0};` is a variable-length array with an initialiser** — a GCC extension, not
  standard C++. It compiles here and will not compile everywhere. `vector<int> arr(n+1, 0);` is the
  portable form and costs nothing.

**Memoisation depth.** Recursion depth n means n stack frames. Around 10⁵ this overflows on
Windows — the crash code is `0xC00000FD` and the program prints **nothing**, because `cout` is
buffered and the buffer is lost. If a memoised solution dies silently on a big input, that is why:
tabulate it.

---

## 6. Where greedy fails — coin change

```
coins = {1, 3, 4},  amount = 6
greedy: 4 + 1 + 1  = 3 coins
DP:     3 + 3      = 2 coins
```

The greedy choice (take the largest coin) is locally best and globally wrong. It happens to be
correct for real currency systems, which is why the intuition is so persistent.

**DP tries every coin at every amount and keeps the best**, which is exactly what greedy refuses to
do. If you cannot prove a greedy choice can never be regretted, you need DP.

---

## Interview Q&A

**Q1. What is dynamic programming?**
Solving a problem by combining solutions to overlapping subproblems, storing each subproblem's
answer so it is computed once. It applies when there are overlapping subproblems *and* optimal
substructure — both, not either.

**Q2. Memoisation or tabulation?**
Memoisation is top-down recursion plus a cache: easier to write, and it only computes the states
you actually reach. Tabulation is bottom-up loops: no recursion depth limit and it space-optimises
easily. Write the memo version first, then convert.

**Q3. How do you find the recurrence?**
Write the plain recursion first. Then ask what the state is (the minimal description of a
subproblem) and what the choices are at that state. The recurrence is "best over the choices" —
the table is just where you put it.

**Q4. Why is merge sort not DP?**
No overlapping subproblems. The two halves are disjoint, so nothing is ever recomputed and a cache
would never hit. Overlap is the whole point of DP.

**Q5. Greedy versus DP?**
Greedy commits to one locally-best choice and never revisits it — correct only when that choice is
provably safe. DP explores all choices. Coin change with `{1,3,4}` and amount 6 is the standard
counterexample: greedy gives 3 coins, the optimum is 2.

**Q6. 0/1 knapsack versus unbounded — what changes?**
The loop direction over capacity in the 1-D version. Backwards means each item is used at most once
(the cell you read is pre-item); forwards allows reuse. That is the entire difference.

**Q7. How do you optimise DP space?**
Look at which cells the recurrence reads. If `dp[i]` reads only the last two, keep two variables.
If `dp[i][j]` reads only row `i-1`, keep one or two rows. Do this only after the full table works —
optimising a wrong recurrence just makes it harder to debug.

**Q8. Your memoised solution fails on large input with no output. Why?**
Stack overflow — one frame per recursion level, and `cout`'s buffer is lost when the process dies,
so it prints nothing at all. Convert to tabulation, which has no recursion.

**Q9. What is the complexity of a DP solution?**
**Number of states × work per state.** For LCS that is O(n·m) states × O(1) each. For MCM it is
O(n²) states × O(n) for the split loop, so O(n³). Counting states is the fastest way to check
whether a formulation is fast enough.

**Q10. Can you always convert memoisation to tabulation?**
Yes, provided you can order the states so every state's dependencies come first — a topological
order of the dependency graph. It is mechanical for grids and sequences; for DP on trees or graphs
the order is a post-order traversal, which is why those are usually left recursive.

---

## Your original notes (preserved)

```
lc 746
```

746 is §1 #3 in `questions.md`, and you have already written both a memoised and a tabulated
version of it in `dp.cpp`. **The memoised one is correct**; the tabulation has a syntax error but
the logic is right — see `solution.md`.
