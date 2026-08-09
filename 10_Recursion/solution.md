# 10 — Recursion & Backtracking: Solutions

Full solutions for Sections 1–3 of `questions.md`. **Every function below was compiled with
`g++ -std=gnu++14` and executed against the test cases shown — 155 assertions, all passing.**
The "Verified" lines are measured outputs, not predictions.

Assume `#include <vector>`, `#include <string>`, `#include <algorithm>`, `using namespace std;`.

---

# Section 1 — Must Do

## 1. Print 1 to N and N to 1

```cpp
void countDown(int n) { if (n == 0) return; cout << n; countDown(n - 1); }  // n..1
void countUp  (int n) { if (n == 0) return; countUp(n - 1); cout << n; }    // 1..n
```

**Key insight.** The *only* difference is whether the print happens before or after the
recursive call. Work before the call happens on the way **down**; work after it happens on the
way **back up**, in reverse order. This is pre-order vs. post-order, and it is the single most
transferable idea in the folder — `../21_tree` is this exact distinction on a branching
structure. Your `PreInPost.cpp` already shows all three positions; that file is worth keeping.

**Parameterised alternative** (your `countToN`), where the answer travels *down*:

```cpp
void countUp(int i, int n) { if (i > n) return; cout << i; countUp(i + 1, n); }
```

Note `i > n`, not `i == n`. With `==`, calling `countUp(1, 0)` never terminates.

**Complexity.** O(n) time, O(n) stack depth.

**Verified:** `countDown(5)→"54321"`, `countUp(5)→"12345"`, `countDown(0)→""` (no output, no crash).

---

## 2. Factorial and Sum of first N

```cpp
long long fact(int n) {
    if (n <= 1) return 1;                 // covers 0, 1 and guards negatives
    return (long long)n * fact(n - 1);
}
int sumTo(int n) {
    if (n <= 0) return 0;
    return n + sumTo(n - 1);
}
```

**Key insight — two separate points.**

1. **`n <= 1`, not `n == 1`.** `fact(0)` must return 1, and with `==` it recurses to
   `fact(-1)`, `fact(-2)`, … and dies. Your `01_recursion.cpp:5` writes `x==0 || x==1`, which
   handles both — correct, just more typing than `<=`.
2. **`13!` overflows a 32-bit `int`.** It is 6,227,020,800 against an `int` limit of
   2,147,483,647. The cast must be on the multiplication, not just the return type — with
   `int` operands the overflow happens *before* the widening.

**Complexity.** O(n) time, O(n) stack.

**Verified:** `fact(0)=1`, `fact(1)=1`, `fact(13)=6227020800` (your `int` version gives
`1932053504`), `fact(20)=2432902008176640000`, `sumTo(10)=55`, `sumTo(0)=0`.

---

## 3. Fibonacci — LeetCode 509

```cpp
int fibo(int n) {
    if (n <= 1) return n;                 // fibo(0)=0, fibo(1)=1 in ONE line
    return fibo(n - 1) + fibo(n - 2);
}
```

**Key insight.** `n <= 1` returning `n` collapses both base cases into one. Your version tests
`n == 2 || n == 1` and returns 1, which is right for n ≥ 1 but makes **`fibo(0)` recurse
forever** — verified, it crashes with a stack overflow.

**Complexity.** O(2ⁿ) time, **O(n) space**. Say both — the space is the part people get wrong.

`[dp]` Memoising with an array collapses time to O(n); that is `../26_dp`, not required here.

**Verified:** `fibo(0)=0`, `fibo(1)=1`, `fibo(10)=55`, `fibo(20)=6765`.

---

## 4. Climbing Stairs — LeetCode 70

```cpp
int climbStairs(int n) {
    if (n <= 1) return 1;                 // n==0: one way (stand still)
    return climbStairs(n - 1) + climbStairs(n - 2);
}
```

**Key insight.** Identical recurrence to #3, shifted by one: the number of ways to reach step
`n` is (ways to reach `n-1`, then take a 1-step) + (ways to reach `n-2`, then take a 2-step).
**`n == 0` must return 1, not 0** — there is exactly one way to do nothing. Get that wrong and
every answer is off.

Your `stair()` uses `n==1||n==2 → return n`, which gives correct answers for n ≥ 1 but
**crashes on `stair(0)`** (verified: stack overflow).

**Three steps instead of two** — add one term:

```cpp
int stair3(int n) {
    if (n < 0) return 0;                  // overshot the top: not a valid way
    if (n == 0) return 1;
    return stair3(n - 1) + stair3(n - 2) + stair3(n - 3);
}
```

The `n < 0` guard is what lets you drop the special cases for n = 1 and 2 entirely.

**Complexity.** O(2ⁿ) time, O(n) space.

**Verified:** `climbStairs(0..5) = 1,1,2,3,5,8`; `stair3(0..6) = 1,1,2,4,7,13,24`.

---

## 5. Reverse an array recursively — LeetCode 344

```cpp
void reverseArr(vector<int>& a, int i, int j) {
    if (i >= j) return;                   // >= handles odd AND even lengths
    swap(a[i], a[j]);
    reverseArr(a, i + 1, j - 1);
}
```

**Key insight.** `i >= j`, not `i == j`. Odd lengths end with `i == j` (pointers meet); even
lengths end with `i > j` (pointers cross without ever being equal). Only `>=` catches both.

**Complexity.** O(n) time, O(n) stack (O(1) if written as a loop).

**Verified:** `{1,2,3,4,5}→54321` (odd), `{1,2,3,4}→4321` (even).

---

## 6. Valid Palindrome, recursively — LeetCode 125

```cpp
bool isPal(const string& s, int i, int j) {
    if (i >= j) return true;              // <-- the fix
    if (s[i] != s[j]) return false;
    return isPal(s, i + 1, j - 1);
}
```

**Key insight.** Same `>=` rule as #5, and this is exactly where your `palindrome.cpp:15` goes
wrong. With `i == j`, the string `"abba"` runs `(0,3) → (1,2) → (2,1) → (3,0) → (4,-1)` and
reads `s[-1]`, which is out of bounds. **Verified: your version returns `false` for `"abba"`,
a genuine palindrome.**

Note `const string&` — passing the string **by value** copies it at every level, turning an
O(n)-space function into O(n²).

**Complexity.** O(n) time, O(n) stack.

**Verified:** `"aba"→true`, `"abba"→true` (even — the case that fails with `==`), `"abca"→false`,
`"a"→true`, `"ab"→false`.

---

## 7. Pow(x, n) — LeetCode 50

**Naive.** Multiply n times: O(n). Your `power()` at `01_recursion.cpp:47` does this correctly.

**Optimal — binary exponentiation.**

```cpp
double powPos(double x, long long n) {
    if (n == 0) return 1.0;
    double half = powPos(x, n / 2);       // <-- recurse ONCE, reuse the result
    double sq = half * half;
    return (n % 2 == 0) ? sq : sq * x;
}
double myPow(double x, int n) {
    long long N = n;                      // widen BEFORE negating
    if (N < 0) { x = 1 / x; N = -N; }
    return powPos(x, N);
}
```

**Key insight — and the bug in your `power2`.** `x^n = (x^(n/2))²`, so you halve the exponent
instead of decrementing it: O(log n). But this only works if you **call the halving function
itself**. `01_recursion.cpp:65` reads:

```cpp
int ans = power(a, b/2);     // calls power (the LINEAR one), not power2
```

The answer still comes out right — `power2(2,10)` returns 1024, verified — but the complexity
collapses back to **O(b)**, which defeats the entire purpose of the function. This is the worst
kind of bug: tests pass, the point is lost.

**The second trap: `n = INT_MIN`.** `-INT_MIN` overflows a 32-bit `int` (there is no `+2147483648`).
Widening to `long long` *before* negating is the fix. Interviewers ask for this specifically.

Also note `half` is computed **once** into a variable. Writing
`return powPos(x,n/2) * powPos(x,n/2);` makes two calls per level and is O(n) again.

**Complexity.** O(log n) time, O(log n) stack.

**Verified:** `myPow(2,10)=1024`, `myPow(2,-2)=0.25`, `myPow(2,0)=1`,
`myPow(2,INT_MIN)≈0` (no overflow), `myPow(1,INT_MIN)=1`, `myPow(0.00001,INT_MAX)≈0`.

---

## 8. Subsets — LeetCode 78

```cpp
void subsetsRec(vector<int>& nums, int i, vector<int>& cur, vector<vector<int> >& res) {
    if (i == (int)nums.size()) { res.push_back(cur); return; }
    subsetsRec(nums, i + 1, cur, res);        // NOT pick
    cur.push_back(nums[i]);
    subsetsRec(nums, i + 1, cur, res);        // PICK
    cur.pop_back();                           // <-- BACKTRACK, mandatory
}
vector<vector<int> > subsets(vector<int> nums) {
    vector<vector<int> > res; vector<int> cur;
    subsetsRec(nums, 0, cur, res);
    return res;
}
```

**Key insight.** `cur` is passed **by reference**, so both branches share one buffer. The
`pop_back()` restores it to exactly the state the caller handed you. Skip it and the "not pick"
branch of the *parent* sees the child's leftovers.

Your `subset1` in `subsets.cpp:6` is the by-value variant and **is correct** — verified, it
produces all 8 subsets of `"abc"`. By value there is no undo to forget, at the cost of an O(n)
copy per node.

**Complexity.** O(2ⁿ · n) time (2ⁿ subsets, each copied), O(n) stack.

**Verified:** `{1,2,3}` → `[] [3] [2] [2,3] [1] [1,3] [1,2] [1,2,3]` (8 subsets);
`{}` → `[[]]` (one empty subset, not zero subsets); 4 elements → 16.

---

## 9. Subsets II — LeetCode 90

**The problem.** `{1,2,2}` must yield 6 subsets, not 8 — `[2]` and `[2,2]` must each appear once.

```cpp
void subsets2Rec(vector<int>& nums, int i, vector<int>& cur, vector<vector<int> >& res) {
    res.push_back(cur);                              // every node IS a subset
    for (int j = i; j < (int)nums.size(); ++j) {
        if (j > i && nums[j] == nums[j - 1]) continue;   // <-- the rule
        cur.push_back(nums[j]);
        subsets2Rec(nums, j + 1, cur, res);
        cur.pop_back();
    }
}
vector<vector<int> > subsetsWithDup(vector<int> nums) {
    sort(nums.begin(), nums.end());                  // duplicates must be adjacent
    vector<vector<int> > res; vector<int> cur;
    subsets2Rec(nums, 0, cur, res);
    return res;
}
```

**Key insight — read the condition carefully.** `j > i` means "not the first choice at this
level". The first `2` at a level is allowed; a *second* `2` at the **same** level would start an
identical branch, so skip it. Picking `2` twice down *different* levels (`[2,2]`) is still fine,
because that goes through `j+1`.

The **sort is not optional** — the skip rule only works if equal values are adjacent. Your
`subset2` sorts *inside* the recursion at `subsets.cpp:38`, which re-sorts a shrinking string at
every node: wasted work, and it does not help because by then the character has already been
taken.

**Complexity.** O(2ⁿ · n) time, O(n) stack.

**Verified:** `{1,2,2}` → `[] [1] [1,2] [1,2,2] [2] [2,2]` (6, not 8);
`{2,1,2}` (unsorted input) → same 6; `{1,1,1,1}` → 5; `{4,4,4,1,4}` → 10.

---

## 10. Combination Sum — LeetCode 39

**Your bug.** `combinationSum.cpp:16` passes `idx` where it should pass `i`:

```cpp
combination(ans, v, c, target - c[i], idx);   // <-- always restarts at idx
```

Because the next level restarts its loop at `idx` rather than at `i`, it can pick elements that
come **before** the one just chosen. **Verified: `c = {2,3,6,7}, target = 7` produces
`[2,2,3]`, `[2,3,2]`, `[3,2,2]`, `[7]` — three orderings of the same combination instead of one.**

**Corrected — pick/not-pick, which makes "unlimited reuse" explicit:**

```cpp
void combRec(vector<int>& c, int i, int target, vector<int>& cur, vector<vector<int> >& res) {
    if (target == 0) { res.push_back(cur); return; }
    if (i == (int)c.size() || target < 0) return;
    if (c[i] <= target) {
        cur.push_back(c[i]);
        combRec(c, i, target - c[i], cur, res);   // SAME index -> reuse allowed
        cur.pop_back();
    }
    combRec(c, i + 1, target, cur, res);          // move on -> never comes back
}
```

**Key insight.** Staying on index `i` is what permits unlimited reuse; moving to `i+1` is what
guarantees you never revisit an earlier element, and *that* is what stops permutations of the
same multiset appearing. Both properties come from the index, not from a duplicate check.

The loop form works too — just pass `i`, not `idx`:

```cpp
for (int j = i; j < (int)c.size(); ++j) {
    if (c[j] > target) continue;
    cur.push_back(c[j]);
    combRec(c, j, target - c[j], cur, res);       // j, not i, and not 0
    cur.pop_back();
}
```

**Complexity.** O(2^(target/min) · k) time, O(target/min) stack.

**Verified:** `({2,3,6,7},7)` → `[2,2,3] [7]` — exactly two, no permutations;
`({2,3,5},8)` → `[2,2,2,2] [2,3,3] [3,5]`; `({2},1)` → empty; `({7,3,2},18)` → 7 combinations.

---

## 11. Combination Sum II — LeetCode 40

Each number used **at most once**, and no duplicate combinations. Both rules at once.

```cpp
void comb2Rec(vector<int>& c, int i, int target, vector<int>& cur, vector<vector<int> >& res) {
    if (target == 0) { res.push_back(cur); return; }
    for (int j = i; j < (int)c.size(); ++j) {
        if (j > i && c[j] == c[j - 1]) continue;   // rule 1: no dup at this level (from #9)
        if (c[j] > target) break;                  // pruning: sorted, so all later ones fail too
        cur.push_back(c[j]);
        comb2Rec(c, j + 1, target - c[j], cur, res);   // rule 2: j+1, each used once
        cur.pop_back();
    }
}
```

**Key insight.** This is #9's skip rule plus #10's index discipline, but with `j + 1` instead of
`j`. Compare the three side by side — that comparison *is* the lesson:

| Problem | Next index | Duplicate skip |
|---|---|---|
| Subsets II (90) | `j + 1` | yes |
| Combination Sum (39) | `j` | not needed (input distinct) |
| Combination Sum II (40) | `j + 1` | yes |

`break` rather than `continue` on `c[j] > target` is valid **only because the array is sorted** —
every later element is at least as large, so none can fit.

**Complexity.** O(2ⁿ · k) time, O(n) stack.

**Verified:** `({10,1,2,7,6,1,5},8)` → `[1,1,6] [1,2,5] [1,7] [2,6]` (note `[1,7]` appears once
even though there are two 1s); `({2,5,2,1,2},5)` → `[1,2,2] [5]`.

---

## 12. Generate Parentheses — LeetCode 22

```cpp
void genRec(int open, int close, string cur, vector<string>& res) {
    if (open == 0 && close == 0) { res.push_back(cur); return; }
    if (open > 0)     genRec(open - 1, close, cur + '(', res);
    if (close > open) genRec(open, close - 1, cur + ')', res);
}
vector<string> generateParenthesis(int n) {
    vector<string> res;
    genRec(n, n, "", res);
    return res;
}
```

**Key insight.** Two rules generate only valid strings, so no validity check is ever needed:

- You may open a bracket whenever any remain (`open > 0`).
- You may close one only when **more closes remain than opens** (`close > open`), which is
  exactly the condition "an unmatched `(` is currently on the table".

**Your `generateParenthesis.cpp` is correct.** Verified: n=3 gives the 5 expected strings, n=4
gives 14 and n=5 gives 42 — the Catalan numbers. Your formulation reaches the same rule by a
different route (an explicit `if (o > c) return;` prune plus `if (o < c)` before closing);
it is a valid encoding of the same invariant.

**Complexity.** O(4ⁿ / √n) — the nth Catalan number — times O(n) to copy each string. O(n) stack.

**Verified:** `n=1 → ()`; `n=2 → (()), ()()`; `n=3 → ((())), (()()), (())(), ()(()), ()()()`;
`n=4 → 14`; `n=5 → 42`.

---

## 13. Permutations — LeetCode 46

**Optimal — swap in place.**

```cpp
void permRec(vector<int>& a, int i, vector<vector<int> >& res) {
    if (i == (int)a.size()) { res.push_back(a); return; }
    for (int j = i; j < (int)a.size(); ++j) {
        swap(a[i], a[j]);
        permRec(a, i + 1, res);
        swap(a[i], a[j]);                 // undo
    }
}
```

**Key insight.** "Everything from index `i` onward is unplaced." Each element in turn is swapped
into slot `i`, and the swap is undone on the way out. **O(1) extra space per level.**

**Your string version** (`Permutationl.cpp`) is the substring form and **is correct** — verified,
`"abc"` gives all 6. It costs O(n) extra space per node because
`original.substr(0,i) + original.substr(i+1)` allocates a new string every time. Worth knowing
both: the substring form reads better, the swap form is what to write when asked about space.

One thing to tidy in yours: the base case at `Permutationl.cpp:8-10` has no `return`. It is
harmless (the `for` loop below has nothing to iterate over when `original` is empty) but it
relies on that coincidence rather than stating the intent.

**Complexity.** O(n! · n) time, O(n) stack.

**Verified:** `permute({1,2,3})` → 6; `permStr("abc")` → `abc, acb, bac, bca, cab, cba`.

---

# Section 2 — Important

## 14. Tower of Hanoi

```cpp
void toh(int n, char from, char helper, char to) {
    if (n == 0) return;
    toh(n - 1, from, to, helper);            // move n-1 out of the way
    cout << from << "->" << to << endl;      // move the biggest
    toh(n - 1, helper, from, to);            // bring n-1 back on top
}
```

**Key insight.** You cannot trace this past n = 3, and you should not try. Trust that
`toh(n-1, …)` does its job and reason only about the three lines. **The roles rotate** —
in the first call the destination becomes the helper; in the second the source becomes the
helper. That rotation is the whole algorithm.

**Your `problems.cpp:44` is correct**, including the argument rotation. Verified against the
canonical sequence for n=3 and the move count `2ⁿ - 1` for n=4. Your parameter order is
`(source, helper, destination, n)` rather than putting `n` first — unconventional, but
consistent and correct.

**Complexity.** O(2ⁿ) time (and the move count is provably minimal), O(n) stack.

**Verified:** n=1 → `A->C`; n=3 → `A->C A->B C->B A->C B->A B->C A->C` (7 moves);
n=4 → 15 moves; n=0 → 0 moves.

---

## 15. K-th Symbol in Grammar — LeetCode 779

**Why the naive approach fails.** Row `n` has 2ⁿ⁻¹ characters. For n = 30 that is over 500
million — you cannot build the string.

```cpp
int kthGrammar(int n, int k) {
    if (n == 1) return 0;
    int parent = kthGrammar(n - 1, (k + 1) / 2);
    if (k % 2 == 1) return parent;        // odd position: copies the parent
    return 1 - parent;                    // even position: flips it
}
```

**Key insight.** Each symbol expands to two: `0 → 01`, `1 → 10`. So position `k` in row `n` comes
from position `⌈k/2⌉ = (k+1)/2` in row `n-1`. If `k` is odd it is the *first* child (same as the
parent); if even, the *second* (flipped). You walk one path up the tree, never building it.

**Your `kthGrammar(779).cpp` is correct.** Verified against rows 4 and 5 of the Thue–Morse
sequence: `01101001` and `0110100110010110`. Your `k/2 + 1` for odd `k` equals `(k+1)/2`, and
your `if (ans) return 0; return 1;` is `1 - ans` written out. Both fine.

**Complexity.** O(n) time, O(n) stack — *not* O(2ⁿ).

**Verified:** row 4 = `01101001`, row 5 = `0110100110010110`, `kthGrammar(30,1)=0`.

---

## 16. Count and Say — LeetCode 38

**Your bug — one missing character.** `countAndSay.cpp:14`:

```cpp
ans+to_string(count);      // computes ans + count, then THROWS IT AWAY
```

It should be `ans += to_string(count);`. GCC does **not** warn about this even with `-Wall`,
because `operator+` on `std::string` is a normal function call whose result may legitimately be
ignored — which is precisely why the bug survived.

It is also latent: `countAndSay(3)` returns `"21"`, which is **correct**, because the final two
appends outside the loop happen to cover the single run. It only breaks from n = 4 on.
**Verified: your version gives `countAndSay(4) = "211"` (want `"1211"`) and
`countAndSay(5) = "221"` (want `"111221"`).**

**Corrected:**

```cpp
string countAndSay(int n) {
    if (n == 1) return "1";
    string prev = countAndSay(n - 1);
    string res;
    int count = 1;
    for (int i = 1; i <= (int)prev.size(); ++i) {           // note <= : one extra step
        if (i < (int)prev.size() && prev[i] == prev[i - 1]) ++count;
        else { res += to_string(count); res += prev[i - 1]; count = 1; }
    }
    return res;
}
```

**Key insight.** Running `i` to `prev.size()` **inclusive** makes the loop flush the final run
itself, so there is no duplicated append after the loop. Your version handles the last run with
two extra statements outside the loop, which works but is the part that hid the bug.

This is recursion on the *previous answer* rather than on a smaller input — the recursion depth
is n, but each level's work grows.

**Complexity.** O(n · L) where L is the final length (which grows ~1.3× per step), O(n) stack.

**Verified:** `1→"1"`, `2→"11"`, `3→"21"`, `4→"1211"`, `5→"111221"`, `6→"312211"`,
`7→"13112221"`.

---

## 17–18. Subsequences with sum K — print / count / any

The same recursion with three different return types. Doing them together is the point.

```cpp
// PRINT ALL
void subseqSum(vector<int>& a, int i, int sum, int target,
               vector<int>& cur, vector<vector<int> >& res) {
    if (i == (int)a.size()) { if (sum == target) res.push_back(cur); return; }
    cur.push_back(a[i]);
    subseqSum(a, i + 1, sum + a[i], target, cur, res);
    cur.pop_back();
    subseqSum(a, i + 1, sum, target, cur, res);
}

// COUNT
int countSubseqSum(vector<int>& a, int i, int sum, int target) {
    if (i == (int)a.size()) return sum == target ? 1 : 0;
    return countSubseqSum(a, i + 1, sum + a[i], target)
         + countSubseqSum(a, i + 1, sum, target);
}

// ANY ONE — stop at the first hit
bool anySubseqSum(vector<int>& a, int i, int sum, int target) {
    if (i == (int)a.size()) return sum == target;
    if (anySubseqSum(a, i + 1, sum + a[i], target)) return true;   // <-- early exit
    return anySubseqSum(a, i + 1, sum, target);
}
```

**Key insight.** The `bool` version is the one that transfers. Returning `true` immediately
prunes the entire remaining subtree — that early exit is what makes real backtracking (N-Queens,
Sudoku) tractable rather than merely correct. The counting version deliberately explores
everything, so it has no early exit to give.

**Complexity.** O(2ⁿ) time, O(n) stack (the print version adds O(n) for `cur`).

**Verified:** `({1,2,1}, K=2)` → `[1,1]` and `[2]`; count = 2; `any` → true; `any` for K=7 →
false; `({5,2,3,10,6,8}, K=10)` → count 3.

---

## 19. Subsequences of a string

```cpp
void subseq(string cur, const string& s, int i, vector<string>& res) {
    if (i == (int)s.size()) { res.push_back(cur); return; }
    subseq(cur, s, i + 1, res);          // skip s[i]
    subseq(cur + s[i], s, i + 1, res);   // take s[i]
}
```

**Fixed length k** — this is what `susequence.cpp` is reaching for:

```cpp
void subseqK(string cur, const string& s, int i, int k, vector<string>& res) {
    if ((int)cur.size() > k) return;                    // prune: already too long
    if (i == (int)s.size()) { if ((int)cur.size() == k) res.push_back(cur); return; }
    subseqK(cur, s, i + 1, k, res);
    subseqK(cur + s[i], s, i + 1, k, res);
}
```

**Your bug.** `susequence.cpp:13-14` hardcodes `3` in both recursive calls instead of passing
`k` through:

```cpp
subset1(ans, original.substr(1), v, 3);      // should be k
subset1(ans+s, original.substr(1), v, 3);    // should be k
```

So the parameter is accepted and then ignored — the function always returns subsequences of
length 3 regardless of what you asked for. **Verified: `subsetK("abcd", k=2)` returns 4 results
(the length-3 subsequences) instead of 6.** Note that a `k=1` test would appear to "pass" at 4
by coincidence, which is why testing one value proves nothing here.

**Complexity.** O(2ⁿ · n) time, O(n) stack.

**Verified:** `subseq("abc")` → 8 including the empty string; `subseqK("abcd", k=2)` →
`ab, ac, ad, bc, bd, cd` (6); `k=3` → 4; `k=0` → 1 (the empty subsequence).

---

## 20. Unique Paths — LeetCode 62

```cpp
int uniquePaths(int m, int n) {
    if (m == 1 || n == 1) return 1;       // single row or column: one path
    return uniquePaths(m - 1, n) + uniquePaths(m, n - 1);
}
```

**Key insight.** From any cell you came either from above or from the left, so the counts add.
A single row or column admits exactly one path — that is the base case, and it is why no
`m == 0` case is needed.

**Your `maize.cpp` is correct.** Verified `maize(4,4) = 20 = C(6,3)` and `maize(3,3) = 6`. Your
`printpath` is also correct — verified 2 paths for a 2×2 and 6 for a 3×3. (The base case at
`maize.cpp:11` has no `return`, but the guard on the next line catches the recursive calls, so
it works.)

`[dp]` Memoising makes it O(m·n); the closed form is `C(m+n-2, m-1)`.

**Complexity.** O(2^(m+n)) time, O(m+n) stack.

**Verified:** `uniquePaths(3,7)=28`, `(3,3)=6`, `(1,5)=1`; path enumeration for 2×2 and 3×3.

---

## 21. Letter Combinations of a Phone Number — LeetCode 17

```cpp
void phoneRec(const string& d, int i, string cur, vector<string>& res) {
    static const char* map9[] = {"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
    if (i == (int)d.size()) { if (!d.empty()) res.push_back(cur); return; }
    const char* letters = map9[d[i] - '0'];
    for (int k = 0; letters[k]; ++k)
        phoneRec(d, i + 1, cur + letters[k], res);
}
```

**Key insight.** The branching factor varies by digit (3 or 4), so this is a loop inside the
recursion rather than a fixed two-way split. **The `if (!d.empty())` guard matters**: for empty
input the answer is an empty list, not a list containing `""`. LeetCode tests that.

**Complexity.** O(4ⁿ · n) time, O(n) stack.

**Verified:** `"23"` → 9 combinations `ad…cf`; `""` → 0 results; `"79"` → 16 (4×4).

---

## 22. Binary strings with no consecutive 1s

```cpp
void binStr(string cur, int n, vector<string>& res) {
    if ((int)cur.size() == n) { res.push_back(cur); return; }
    cur += '0'; binStr(cur, n, res); cur.pop_back();
    if (cur.empty() || cur.back() != '1') { cur += '1'; binStr(cur, n, res); cur.pop_back(); }
}
```

**Key insight.** Constrain at the point of *placement*, not by generating everything and
filtering. A `'0'` is always legal; a `'1'` only when the previous character is not `'1'`. The
count is the Fibonacci sequence, which is a nice thing to notice out loud.

**Your `binarystrings.cpp` is correct.** Verified: n=2 → 3 strings, n=3 → 5, n=4 → 8, n=5 → 13.

One note: `ans.size() == n` compares `size_t` with `int` and GCC warns. Cast to `(int)` — with
a negative `n` the comparison would go badly wrong.

**Complexity.** O(Fib(n) · n) time, O(n) stack.

**Verified:** n=2 → `00, 01, 10` (no `11`); counts 3, 5, 8, 13 for n = 2..5.

---

## 23. Palindrome Partitioning — LeetCode 131

```cpp
void partRec(const string& s, int i, vector<string>& cur, vector<vector<string> >& res) {
    if (i == (int)s.size()) { res.push_back(cur); return; }
    for (int j = i; j < (int)s.size(); ++j) {
        if (isPal(s, i, j)) {                        // prune: only recurse on valid cuts
            cur.push_back(s.substr(i, j - i + 1));
            partRec(s, j + 1, cur, res);
            cur.pop_back();
        }
    }
}
```

**Key insight.** Try every cut point, but **only recurse when the prefix is already a
palindrome**. That check is a prune, not a filter — it kills the branch before you descend,
which is the difference between this being feasible and being 2ⁿ⁻¹ dead ends. Reuses `isPal`
from #6.

**Complexity.** O(2ⁿ · n) time, O(n) stack.

**Verified:** `"aab"` → 2 partitions (`[a,a,b]`, `[aa,b]`); `"aaa"` → 4; `"abc"` → 1.

---

# Section 3 — Good to Know

## 24. N-Queens — LeetCode 51

```cpp
bool safe(vector<int>& col, int r, int c) {
    for (int i = 0; i < r; ++i)
        if (col[i] == c || abs(col[i] - c) == r - i) return false;
    return true;
}
void qRec(int n, int r, vector<int>& col, vector<vector<string> >& res) {
    if (r == n) {                                     // col[] is a complete solution
        vector<string> board;
        for (int i = 0; i < n; ++i) {
            string row(n, '.');
            row[col[i]] = 'Q';
            board.push_back(row);
        }
        res.push_back(board);
        return;
    }
    for (int c = 0; c < n; ++c)
        if (safe(col, r, c)) { col[r] = c; qRec(n, r + 1, col, res); }
}
```

**Key insight — two of them.**

1. **One queen per row is built into the structure.** Recursing on the row index means you never
   have to check row conflicts at all; the state is a single `col[r]` per row, not an n×n board.
2. **The diagonal test is one line.** Two cells share a diagonal iff the row difference equals
   the column difference: `abs(col[i] - c) == r - i`. No direction vectors, no scanning.

Note there is no explicit "undo" here — assigning `col[r] = c` on the next iteration overwrites
the previous choice, so the restore is implicit. With a full board representation you would need
an explicit reset.

**Complexity.** Roughly O(n!) with heavy pruning, O(n) space for `col`.

**Verified:** n=1 → 1, n=2 → 0, n=3 → 0, n=4 → 2, n=6 → 4, n=8 → **92**. The first board for
n=4 is `.Q.. / ...Q / Q... / ..Q.`.

---

## 25. Sudoku Solver — LeetCode 37

```cpp
bool solve(vector<vector<char> >& b) {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (b[i][j] == '.') {
                for (char c = '1'; c <= '9'; ++c)
                    if (isValid(b, i, j, c)) {
                        b[i][j] = c;
                        if (solve(b)) return true;    // <-- propagate success upward
                        b[i][j] = '.';                // <-- undo on failure
                    }
                return false;                         // no digit fits: this branch is dead
            }
    return true;                                      // no empty cell left: solved
}
```

**Key insight — the `bool` return is the entire difference from #24.** N-Queens wants *all*
solutions, so it explores everything. Sudoku wants *one*, so the moment a recursive call returns
`true` you must return `true` immediately without trying the remaining digits. The
`return false` after the digit loop is equally important: it says "this cell cannot be filled",
which unwinds to the previous cell and makes it try its next candidate.

`isValid` checks row, column, and the 3×3 box; the box's top-left corner is
`(i/3)*3, (j/3)*3` — the same index identity as `../08_2d array`.

**Complexity.** Exponential in theory; instant in practice on real puzzles thanks to pruning.

**Verified:** solves the standard LeetCode 37 board (row 0 → `534678912`, row 8 → `345286179`),
and re-checking all 81 cells afterwards confirms every one is legal. A board with no solution
returns `false` instead of hanging.

> `isValid` in one loop: `b[r][k]`, `b[k][c]`, and `b[(r/3)*3 + k/3][(c/3)*3 + k%3]` check the
> row, the column and the box with a single `k` from 0 to 8. The third is the `../08_2d array`
> index identity doing real work.

---

## 26. Rat in a Maze

```cpp
void ratDfs(vector<vector<int> >& m, int i, int j, string path, vector<string>& res) {
    int n = (int)m.size();
    if (i < 0 || i >= n || j < 0 || j >= n || m[i][j] == 0) return;   // bounds AND blocked
    if (i == n-1 && j == n-1) { res.push_back(path); return; }
    m[i][j] = 0;                                     // block: prevents revisiting
    ratDfs(m, i+1, j, path + "D", res);
    ratDfs(m, i, j-1, path + "L", res);
    ratDfs(m, i, j+1, path + "R", res);
    ratDfs(m, i-1, j, path + "U", res);
    m[i][j] = 1;                                     // UNBLOCK on the way out
}
```

**Key insight.** Marking the cell blocked before recursing is what stops infinite cycles — this
grid, unlike `maize.cpp`'s, allows all four directions, so without it the rat walks back and
forth forever. Unblocking afterwards is what allows a *different* path to use the same cell.
Mark-recurse-unmark is the grid version of push-recurse-pop.

The direction order D, L, R, U is conventional because it yields paths in lexicographic order.

**Complexity.** O(4^(n²)) worst case, O(n²) stack.

**Verified:** the classic 4×4 maze → `DDRDRR, DRDDRR`; a fully blocked diagonal → no paths;
blocked start → no paths (and no crash); open 2×2 → `DR, RD`.

---

## 27. Word Search — LeetCode 79

```cpp
bool wordDfs(vector<vector<char> >& b, const string& w, int i, int j, int k) {
    if (k == (int)w.size()) return true;
    if (i < 0 || i >= (int)b.size() || j < 0 || j >= (int)b[0].size()) return false;
    if (b[i][j] != w[k]) return false;
    char save = b[i][j];
    b[i][j] = '#';                                   // mark visited
    bool found = wordDfs(b,w,i+1,j,k+1) || wordDfs(b,w,i-1,j,k+1)
              || wordDfs(b,w,i,j+1,k+1) || wordDfs(b,w,i,j-1,k+1);
    b[i][j] = save;                                  // UNDO
    return found;
}
```

**Key insight.** Storing the visited flag **inside the grid** (overwriting with `'#'`, restoring
after) avoids a separate `visited` matrix — the same "keep your bookkeeping in the input" trick
as Set Matrix Zeroes in `../08_2d array`. The `||` chain short-circuits, so as soon as one
direction succeeds the rest are never explored.

**Complexity.** O(R·C·4^L) time, O(L) stack.

**Verified:** the standard 3×4 board — `"ABCCED"`→true, `"SEE"`→true, `"ABCB"`→false (proving a
cell cannot be reused); 1×1 boards; `"aaa"` on `{a,a}` → false.

---

## 28. Sort an array recursively

```cpp
void insertSorted(vector<int>& a, int val) {
    if (a.empty() || a.back() <= val) { a.push_back(val); return; }
    int t = a.back(); a.pop_back();
    insertSorted(a, val);
    a.push_back(t);                       // put it back on the way up
}
void sortRec(vector<int>& a) {
    if (a.size() <= 1) return;
    int t = a.back(); a.pop_back();
    sortRec(a);                           // sort the rest
    insertSorted(a, t);                   // insert this one into its place
}
```

**Key insight.** Two mutually supporting recursions and **no loops at all**. This is recursive
insertion sort; the same skeleton sorts a `stack` (where you genuinely have no random access,
which is the version usually asked). The pattern — pop, recurse, push back on the way up — is
the same "work after the call" idea from #1.

**Complexity.** O(n²) time, O(n) stack.

**Verified:** `{5,2,9,1,5,6}` → `1 2 5 5 6 9`; `{30,-5,18,14,-3}` → `-5 -3 14 18 30`;
empty array → no crash.

---

# Section 4 — approach only

- **Merge Sort / Quick Sort** — divide & conquer: split, recurse twice, combine. Merge sort does
  its work in the combine step; quick sort does it in the split. You write both in `../12_sorting`.
- **Binary search, recursively** — one branch per level, not two, so it is O(log n) time *and*
  O(log n) stack. Use `mid = lo + (hi - lo) / 2`; `(lo + hi) / 2` overflows for large indices.
  **Verified:** finds first, last, middle and absent elements, and returns −1 on an empty range.
- **Combination Sum III (216)** — #11 with a second constraint (exactly k numbers). Prune on
  both count and sum.
- **Permutations II (47)** — sort, then apply #9's `j > i && a[j] == a[j-1]` skip inside the
  permutation loop.
- **Subset Sum (any one)** — the `bool` version from #17–18; return `true` up the stack on the
  first hit.
- **Unique BSTs** — the count is Catalan again, same as #12: pick each value as root, multiply
  the left and right counts, sum over all roots.
- **Josephus** — `josephus(n,k) = (josephus(n-1,k) + k) % n` with `josephus(1,k) = 0`
  (0-indexed). **Verified:** `(5,2)→2`, `(7,3)→3`, `(1,5)→0`. Add 1 for a 1-indexed answer.
- **Print all grid paths** — your `maize.cpp:9`, already working.
- **Word Break (139)** — try every prefix; if it is a dictionary word, recurse on the rest.
  Exponential until memoised `[dp]`.
- **Unique Paths II (63)** — #20 plus `if (grid[i][j] == 1) return 0;` for obstacles.
- **k-length strings over an alphabet** — your `problems.cpp:54` `kstrings`, working.
- **Remove all occurrences of a character** — your `problems.cpp:14`, working.
- **Is an array sorted** — `a[i] <= a[i+1] && isSorted(a, i+1)`; base case `i >= size()-1`.
  **Verified:** true/false/single/empty all correct.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `10_Recursion/` was edited.**

Three crash outright. Because `cout` is buffered, a stack overflow discards pending output, so
these programs print **nothing at all** — do not mistake that for "it did nothing".

## `subsets.cpp` — crashes on run, no output (`0xC00000FD`, STACK_OVERFLOW)

`subsets.cpp:22-24` — **missing `return`**:

```cpp
void subsetNum1( int arr[] ,int n,int i ,vector<int>& v){
    if(i==n){
        for (int i :v ){cout<<i<<" ";}
    }                          // <-- no return: execution falls through
    subsetNum1(arr,n,i+1,v);   // i becomes n+1, and i==n is never true again
    v.push_back(arr[i]);       // also reads arr[i] out of bounds
    subsetNum1(arr,n,i+1,v);
}
```

Once `i` passes `n` the base case can never fire again, so it recurses until the stack dies.
`main` calls this, so **the whole program produces no output** — including nothing from the
`subset1` call that ran successfully before it.

Corrected: add `return;` inside the `if`, and follow #8's structure (push, recurse, **pop**):

```cpp
void subsetNum1(int arr[], int n, int i, vector<int>& v) {
    if (i == n) {
        for (int x : v) cout << x << " ";
        cout << endl;
        return;                            // <-- the fix
    }
    subsetNum1(arr, n, i + 1, v);          // not pick
    v.push_back(arr[i]);
    subsetNum1(arr, n, i + 1, v);          // pick
    v.pop_back();                          // <-- backtrack
}
```

Without the `pop_back`, `v` would accumulate across branches and print garbage.

**Verified after the fix:** `{1,2,3}` prints the 8 subsets `(empty), 3, 2, 23, 1, 13, 12, 123`
and terminates cleanly.

### `subsets.cpp:34` — `subset2` has three separate problems

```cpp
void subset2(string ans, string original ,vector<string > v,bool flag){
```

1. **`v` is passed by value.** Every `v.push_back(ans)` writes to a copy that is destroyed on
   return. The caller's vector is always empty — the function cannot produce a result. Needs
   `vector<string>& v`. (Your `subset1` on line 6 gets this right, which makes this a slip
   rather than a misunderstanding.)
2. **`:35` has no `return`.** After `v.push_back(ans)` on an empty string, execution continues
   to `original[0]` and `original.substr(1)` — `substr(1)` on an empty string throws
   `std::out_of_range`.
3. **`:47` and `:51` — the two branches are identical.** Both do the same two calls, so the
   `if (s == original[1])` test has no effect at all. The duplicate-handling logic is not
   actually implemented.

See #9 for the working version; the rule you were reaching for is
`if (j > i && a[j] == a[j-1]) continue;` on a **sorted** input.

---

## `01_recursion.cpp` — three functions crash, one is quietly pointless

### `:56` `fibo(0)` → STACK_OVERFLOW

```cpp
if (n==2|| n==1) return 1;
```

`fibo(0)` misses both, recursing to `fibo(-1)`, `fibo(-2)`, … **Verified: crashes.** Fix:
`if (n <= 1) return n;`.

### `:76` `stair(0)` → STACK_OVERFLOW

Same shape: `if (n==1 || n==2) return n;` never catches 0. **Verified: crashes.** Fix:
`if (n <= 1) return 1;`.

### `:82` `stair2` — wrong answers *and* it crashes

```cpp
int stair2(int n ){
    if (n>3)return n;              // <-- returns n for every n > 3
    if(n==3)return 4;
    return stair2(n-1)+stair2(n-2)+stair(n-3);   // <-- calls stair, not stair2
}
```

Two bugs. `if (n > 3) return n;` short-circuits every interesting input — **verified:
`stair2(4)` returns 4, and the correct answer is 7**. And for n ≤ 2 it falls through to the
recursive line, which reaches `stair2(0)`, `stair2(-1)`, … — **verified: crashes**. The last
term also calls `stair` (the 2-jump version) instead of `stair2`.

The corrected `stair3` is in #4. Verified: `1, 1, 2, 4, 7, 13, 24` for n = 0..6.

### `:62` `power2` — correct answer, wrong complexity

```cpp
int ans =power(a,b/2);     // calls power, not power2
```

Covered in #7. It returns the right number (`power2(2,10) = 1024`, verified) while being O(b)
instead of O(log b) — so the one thing the function exists to demonstrate does not happen.

### `:47` `power` — the special case is unnecessary

`if (a==0 ||a==1) return a;` — for `a == 1` this returns 1, which is right; for `a == 0` with
`b > 0` it returns 0, also right. So it is harmless, but the `b == 0` check above it already
covers the only case that mattered. Deleting the line changes no answers.

---

## `palindrome.cpp` — both functions are wrong, and `main` never calls either

### `:7-11` `isPalindrome` — two bugs on adjacent lines

```cpp
int i =0,j=s.size();          // should be s.size() - 1
while(i<j){
    if (s[i]!=s[j]) return false;
    i++;j++;                  // should be j--
}
```

`j` starts one past the last character, and then **both** pointers move *forward*, so they never
converge. **Verified: `isPalindrome("aba")` returns false.** Corrected in #6 (or use the
iterative form in `../09_Strings/solution.md` #3).

### `:15` `isPlaindromeRec` — the `i == j` base case

```cpp
if(i==j)return true;
```

Even-length strings cross without ever being equal. **Verified: `isPlaindromeRec("abba",0,3)`
returns `false` for a genuine palindrome**, after reading `s[-1]` out of bounds. Fix: `i >= j`.

---

## `GCD.cpp:8` — division by zero when `a == 0`

```cpp
if(b%a==0) return a;
```

`GCD(0, 5)` evaluates `5 % 0`. **Verified: crashes with `0xC0000094`,
INTEGER_DIVIDE_BY_ZERO.** `GCD(4,8)` works, which is why the file looks fine.

The conventional Euclid formulation puts the guard where it belongs:

```cpp
int gcd(int a, int b) {
    if (b == 0) return a;          // b, not a -- and tested before any %
    return gcd(b, a % b);
}
```

**Verified:** `gcd(4,8)=4`, `gcd(8,4)=4`, `gcd(0,5)=5`, `gcd(5,0)=5`, `gcd(12,18)=6` — no crash.

---

## `combinationSum.cpp:16` — duplicate combinations

Covered in #10. Passing `idx` instead of `i` yields `[2,3,2]` and `[3,2,2]` alongside `[2,2,3]`.
Also, `main` is `int main (){return 0;}` — the function has never actually been run.

---

## `countAndSay.cpp:14` — missing `=`

Covered in #16. `ans+to_string(count);` should be `ans+=to_string(count);`. Correct through
n=3, wrong from n=4.

---

## `susequence.cpp:13-14` — the `k` parameter is ignored

Covered in #19. Both recursive calls hardcode `3`. `main` is empty, so this was never run.

---

## `subarrays.cpp` — two different problems

### `:12` iterative version — the inner bound is off by one

```cpp
for (int j =i;j<k;j++){        // should be j <= k
```

**Verified on `{1,2,3,4}`:** prints an empty line, then `1`, `12`, `123`, then an empty line,
`2`, `23`, … — it emits a blank line for each start and **never prints the longest subarray from
each start** (`1234`, `234`, `34`, `4` are all missing). Correct output is
`1, 12, 123, 1234, 2, 23, 234, 3, 34, 4`.

### `:25` `subArrayRec` — right idea, wrong on repeated values

This is a genuinely clever attempt, and on **distinct** values it works: verified, `{1,2,3}`
produces all 6 correct subarrays (plus one spurious empty line, because the empty selection
reaches the base case).

But the contiguity test compares **values**, not indices:

```cpp
if(ans[ans.size()-1]==v[idx-1])
```

If `ans` ends at some index `p` and `v[idx-1]` happens to hold the same *value* as `v[p]`, the
test passes and a **non-contiguous** selection is extended. **Verified:** `{4,4,9}` emits 7
subarrays where there are 6, with `49` appearing twice — once legitimately (indices 1,2) and
once from indices 0 and 2, which is not a subarray. `{1,1,1,1}` emits 15 instead of 10 — it
degenerates into the power set.

**The deeper point:** subarrays are contiguous, so they do not need recursion at all. Two
nested loops give all n(n+1)/2 of them directly. Recursion is the tool for *subsequences*
(2ⁿ, gaps allowed) — see `readme.md` §7 for the distinction, which is the actual lesson here.

---

## What you got right

These are the habits worth keeping:

- **`generateParenthesis.cpp` is correct** — verified: 5 strings for n=3, 14 for n=4, 42 for
  n=5, the Catalan numbers. You arrived at the validity invariant by your own route
  (`if (o>c) return;` plus `if (o<c)` before closing) rather than the textbook `close > open`,
  and it is genuinely equivalent. Generating only valid strings instead of generating-then-filtering
  is the right instinct.

- **`kthGrammar(779).cpp` is correct** — verified against rows 4 and 5 of Thue–Morse. This is
  the hardest problem in the folder to get right, and you avoided the trap of building the row.
  Recognising that only the parent matters is the whole insight.

- **Tower of Hanoi in `problems.cpp:44` is correct**, argument rotation and all — verified
  against the canonical 7-move sequence for n=3 and 15 moves for n=4. This is the problem people
  most often get subtly wrong by mixing up helper and destination.

- **`binarystrings.cpp` is correct** — verified 3/5/8/13 strings for n=2..5. Constraining at
  placement time rather than filtering afterwards is exactly right.

- **`maize.cpp` is correct**, both `maize` (20 for 4×4 = C(6,3)) and `printpath` (6 paths for
  3×3, verified against the expected set).

- **`Permutationl.cpp` is correct** — all 6 permutations of `"abc"`.

- **`subsets.cpp`'s `subset1` is correct** — all 8 subsets of `"abc"`, verified.

- **`problems.cpp`'s `kstrings` is correct** — 27 strings of length 3 over `{0,1,2}`, all
  distinct. The digit order is unconventional (it fills from the back) but the set is complete.

- **`problems.cpp`'s `removeChar` uses an index, and the `substr` version is commented out
  above it.** That is the right choice and the wrong one is sitting right there for comparison —
  `substr` at every level is O(n²). Whether or not it was deliberate, keep the index version.

- **`01_recursion.cpp` has `Sum` and `Sum2` side by side** — the parameterised and functional
  forms of the same computation. Writing both out is exactly how to internalise the distinction.

- **`PreInPost.cpp`** demonstrates all three work-positions around two recursive calls in one
  function. That is the tree traversal orders in miniature; it will make `../21_tree` much
  easier.
