# 10 — Recursion & Backtracking

Recursion is not a topic you finish. It is the substrate for `../21_tree`, `../22_bst`,
`../26_dp` and `../27_graphs` — every one of those is recursion with a different shape of state.
Time spent here pays back four folders later, which is why this folder has the largest question
set in the repo so far.

---

## 1. The three parts of every recursive function

```cpp
int fact(int n) {
    if (n <= 1) return 1;             // 1. BASE CASE — stops the recursion
    return n * fact(n - 1);           // 2. RECURSIVE CALL on a SMALLER input
}                                     // 3. the work that combines the result
```

If any of the three is missing or wrong you get one of exactly two failures:

| Symptom | Cause |
|---|---|
| **Stack overflow / crash, no output** | base case unreachable — the argument never reaches it |
| Wrong answer, terminates fine | base case returns the wrong value, or the combine step is wrong |

**"Unreachable" is the important word.** `if (n == 1) return 1;` looks like a base case, but
`fact(0)` walks straight past it to `fact(-1)`, `fact(-2)`, … and dies. Writing `n <= 1` costs
one character and removes a whole class of bug. There are **four** functions in this folder that
crash for exactly this reason — see `solution.md`.

> On this toolchain a stack overflow shows up as exit code `0xC00000FD` and, because `cout` is
> buffered, **you usually get no output at all** — not even the lines that ran before the crash.
> A program that prints nothing and exits instantly is the classic signature. Do not read it as
> "my function returned early".

---

## 2. The recursion tree, and why it is the only debugging tool you need

For `fibo(4)`:

```
                    fibo(4)
              /                \
        fibo(3)                fibo(2)
        /      \               /     \
   fibo(2)   fibo(1)      fibo(1)  fibo(0)
   /     \
fibo(1) fibo(0)
```

Read off directly:

- **Time** = number of nodes. Each node branches twice and the depth is n, so ≈ 2ⁿ.
- **Space** = the longest root-to-leaf path (n), because only one path is on the call stack at a
  time. **Not** the number of nodes — this is the most common complexity mistake in interviews.
- `fibo(2)` appears twice. That repetition is the entire motivation for `../26_dp`.

---

## 3. Parameterised vs. functional recursion

Two ways to carry an answer. Both are correct; interviewers expect you to know both.

```cpp
// PARAMETERISED — the answer travels down as an argument
void sumTo(int n, int sum) {
    if (n == 0) { cout << sum; return; }
    sumTo(n - 1, sum + n);
}

// FUNCTIONAL — the answer is built on the way back up
int sumTo(int n) {
    if (n <= 0) return 0;
    return n + sumTo(n - 1);
}
```

Your `01_recursion.cpp` has both forms side by side (`Sum` and `Sum2`) — that is a good thing to
have written out.

The same distinction produces the **print 1..n / n..1** pair, which is the cleanest possible
demonstration of "before or after the recursive call":

```cpp
void down(int n) { if (n == 0) return; cout << n; down(n - 1); }   // work BEFORE -> n..1
void up  (int n) { if (n == 0) return; up(n - 1); cout << n; }     // work AFTER  -> 1..n
```

Nothing changed but the order of two lines. That is what "the stack unwinds in reverse" means in
practice, and it is exactly the pre-order / post-order distinction you will meet in `../21_tree`.
Your `PreInPost.cpp` demonstrates all three positions in one function — keep that file.

---

## 4. Pick / not-pick — the pattern behind half of all recursion problems

Every subset, subsequence, and combination problem is the same two lines:

```cpp
void rec(int i, vector<int>& cur) {
    if (i == n) { record(cur); return; }
    rec(i + 1, cur);              // NOT pick a[i]
    cur.push_back(a[i]);
    rec(i + 1, cur);              // PICK a[i]
    cur.pop_back();               // <-- BACKTRACK
}
```

**The `pop_back()` is the whole idea of backtracking.** `cur` is shared by reference, so a change
made in one branch is visible in the next. You must undo it before returning, restoring `cur` to
exactly the state you were handed. Every "backtracking" problem is this discipline:
**do → recurse → undo**.

If instead you pass `cur` **by value**, every call gets its own copy and no undo is needed — but
you pay O(n) copying per node. Correct, slower, and it is why `subsets.cpp`'s `subset2` silently
loses all its results: the vector is by value, so the `push_back` happens on a copy that is
thrown away.

2ⁿ subsets, each up to n long, gives **O(2ⁿ · n)** time and **O(n)** stack depth.

---

## 5. The three ways to control duplicates

This is where most people lose marks, so learn the three separately:

| Goal | Technique | Problem |
|---|---|---|
| Each element used **at most once**, no dup subsets | sort, then `if (j > i && a[j] == a[j-1]) continue;` | Subsets II (90), Combination Sum II (40) |
| Each element reusable **unlimited** times | recurse on the **same** index `i` after picking | Combination Sum (39) |
| Each element used once, **order matters** | swap-based permutations, or a `used[]` array | Permutations (46) |

The middle row is where `combinationSum.cpp` goes wrong — passing `idx` instead of `i` to the
next call lets the loop reselect *earlier* elements, producing `[2,3,2]` and `[3,2,2]` as well as
`[2,2,3]`. Details in `solution.md`.

---

## 6. Recursion vs. iteration

| | Recursion | Iteration |
|---|---|---|
| Extra space | **O(depth)** stack frames | O(1) |
| Speed | function-call overhead per node | faster |
| Reads well for | trees, backtracking, divide & conquer | linear scans |

Any recursion **can** be rewritten iteratively (with your own explicit stack in the worst case),
but for tree and backtracking problems the recursive version is so much clearer that it is the
expected answer. Use recursion where the *problem* is recursive; use a loop to count to n.

**Tail recursion** — where the recursive call is the very last thing that happens — can be turned
into a loop by the compiler. `-O2` does this reliably; at `-O0` (the default here) it does not, so
depth still costs stack even for tail calls.

---

## Interview Q&A

**Q1. What is the space complexity of a recursive function?**
O(maximum depth of the recursion tree), because only one root-to-leaf path exists on the stack at
any moment — plus whatever each frame holds. For `fibo(n)`, time is O(2ⁿ) but space is only
**O(n)**. Candidates routinely say O(2ⁿ) for both; that is wrong and interviewers listen for it.

**Q2. Why prefer `if (n <= 1)` over `if (n == 1)` as a base case?**
`==` is an exact hit that a decrementing argument can jump straight over — `fact(0)` skips
`n == 1` and recurses forever. `<=` is a *barrier*: everything at or below it stops. Barriers
are unconditionally safer than exact hits.

**Q3. What exactly is backtracking?**
DFS over a space of partial solutions, where you **undo** each choice after exploring it. The
undo is what lets one mutable buffer serve the whole tree instead of copying it at every node.
Pruning — abandoning a branch that provably cannot lead to a solution, like `if (target < 0)
return;` — is what makes it fast enough to be practical.

**Q4. Why is naive `fibo` exponential when there are only n distinct inputs?**
Because it recomputes them. `fibo(2)` is evaluated once for every path that reaches it, and the
number of such paths grows like the Fibonacci numbers themselves. Memoising — cache the result
the first time — collapses it to O(n). That single change is the whole of top-down DP.

**Q5. How do you generate all permutations, and why is swapping better than the substring method?**
Swap `a[i]` with each `a[j]` for `j ≥ i`, recurse on `i+1`, swap back. It is **O(1) extra space**
per level. The alternative — building `rest.substr(0,i) + rest.substr(i+1)` — allocates a fresh
string at every node, making it O(n) extra space and O(n) extra time per node. Your
`Permutationl.cpp` uses the substring form; it is correct and easier to read, and it is worth
knowing the swap version as the follow-up answer.

**Q6. When does recursion cost more than the stack depth suggests?**
When each frame holds something big. Passing a `string` or `vector` **by value** copies it per
call, so depth-n recursion holding an n-element copy is O(n²) space, not O(n). Passing by
`const&` (or by `&` with backtracking) fixes it. This is the `s.substr(1)` trap from
`../09_Strings` §4.

**Q7. What's the difference between subsets, subsequences, and subarrays?**
- **Subarray** — contiguous. n(n+1)/2 of them. Two nested loops, no recursion needed.
- **Subsequence** — order preserved, gaps allowed. 2ⁿ. Pick / not-pick.
- **Subset** — a set, so order is irrelevant. Also 2ⁿ, and for an array with distinct elements
  the enumeration is identical to subsequences.

Confusing the first with the other two is a real trap — `subarrays.cpp` in this folder tries to
generate *subarrays* with *subsequence* machinery, which is why it misbehaves on repeated values.

---

## Your original notes (preserved)

The problem list that was in this file is kept verbatim below. **All six are covered** — see
`questions.md` §1 and §2.

```
#Leet code 
-78 Subsets,
90 subsets2
39 COMBINATION SUM 
22 GENERATE PARENTHESIS
779 Kth symbol in grammar
38 Count and say
```

| Your entry | Now at | Your file |
|---|---|---|
| 78 Subsets | `questions.md` §1 #8 | `subsets.cpp` — `subset1` correct, `subsetNum1` crashes |
| 90 Subsets II | `questions.md` §1 #9 | `subsets.cpp` — `subset2` broken |
| 39 Combination Sum | `questions.md` §1 #10 | `combinationSum.cpp` — produces duplicates |
| 22 Generate Parentheses | `questions.md` §1 #12 | `generateParenthesis.cpp` — **correct** |
| 779 Kth Symbol in Grammar | `questions.md` §2 #15 | `kthGrammar(779).cpp` — **correct** |
| 38 Count and Say | `questions.md` §2 #16 | `countAndSay.cpp` — one missing `=` |
