# 05 — Functions & Basic Recursion: Practice Questions

> **Why 16 ranked problems.** This topic has roughly fifteen genuinely distinct techniques —
> the three parameter-passing modes, linear recursion, parameterised vs functional
> recursion, branching recursion and its cost, tail recursion, divide-and-conquer recursion,
> overflow-safe combinatorics, decomposition/reuse, overloading, default arguments, static
> locals, and multiple return values. One problem per technique, plus the two that only
> make sense as a pair (Fibonacci naive vs iterative), lands at 16. Fewer would leave a
> technique untested; more would repeat one.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title on [practice.geeksforgeeks.org](https://practice.geeksforgeeks.org). Items
marked *Drill* have no judge equivalent; write and run them anyway.

**Scope.** Arrays, strings and vectors arrive in `../07_Array` and `../09_Strings`, so
nothing here requires them. The recursion problems that *do* need an array (reverse an
array, binary search) are listed in Section 4 as **forward references** — do them when you
get there.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Swap two numbers — by value, by reference, and by pointer | Easy | *Drill* | `lec9.cpp:48`, `../06_Pointer/pointer.cpp:10` |
| 2 | Print 1 to N, and N to 1, without a loop | Easy | GFG | *"Print 1 To N Without Loop"*, *"Print N to 1 without loop"* — Striver 1.4 |
| 3 | Sum of the first N natural numbers, recursively | Easy | GFG | *"Sum of first n terms"* — Striver 1.4 |
| 4 | Factorial of N | Easy | GFG | *"Factorial"* — Striver 1.4; also `lec9.cpp:16` |
| 5 | Nth Fibonacci number | Easy | **LeetCode 509** | `leetcode.com/problems/fibonacci-number` — Striver 1.4 |
| 6 | GCD / HCF, recursively | Easy | GFG | *"LCM And GCD"*; also `lec9.cpp:66` |

**Why these six.** #1 is the reason functions have three parameter-passing modes, and the
only way to internalise the difference is to write all three and watch two of them work.
#2 is the purest possible recursion — no return value, so the *only* thing being tested is
base case and direction; doing both directions teaches that the work can happen **before**
or **after** the recursive call, which is the whole idea behind pre-order vs post-order
later. #3 introduces a return value to combine, and has an O(1) closed form to compare
against. #4 is #3 with multiplication, which is where overflow appears. **#5 is the most
important problem here** — it's the first branching recursion, it's O(2ⁿ), and seeing that
blow up is what motivates memoisation and all of `../26_dp`. #6 is **tail recursion**, where
the recursive call is the last thing that happens and the loop version is trivially
equivalent.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 7 | nCr / binomial coefficient **without overflow** | **Medium** | *Drill* | fixes the bug at `lec9.cpp:56` |
| 8 | Pascal's Triangle — print `n` rows | Easy | **LeetCode 118 / 119** | `pascals-triangle`, `pascals-triangle-ii`; also `lec9.cpp:26` |
| 9 | Pow(x, n) — recursive binary exponentiation | **Medium** | **LeetCode 50** | `leetcode.com/problems/powx-n` |
| 10 | Function overloading: `mini` for `int`, `double`, and three arguments | Easy | *Drill* | `lec9.cpp:13` |
| 11 | Count primes in `[a, b]` by reusing an `isPrime` helper | Easy | *Drill* | decomposition; `isPrime` from `../03_Loops` |

**Why these five.** **#7 is the highest-value problem in this folder** because your current
code gets it wrong in a way that returns a plausible-looking number — `combination(20,10)`
gives 11 instead of 184756 — and the fix teaches you to reorder a computation so the
intermediate values stay small. #8 is the same combinatorics viewed as a recurrence rather
than a formula, and it's the first time you'll see "each entry depends on two entries
above", which is the shape of every 2D DP table. #9 is divide-and-conquer recursion: the
problem halves rather than shrinking by one, which is what makes it O(log n) instead of
O(n). #10 is the only way to actually feel overload resolution. #11 looks trivial and is
the point of the whole folder — a function you wrote in `../03_Loops` becomes a black box
you now reuse without re-reading.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 12 | Climbing Stairs | Easy | **LeetCode 70** | `leetcode.com/problems/climbing-stairs` |
| 13 | N-th Tribonacci Number | Easy | **LeetCode 1137** | `leetcode.com/problems/n-th-tribonacci-number` |
| 14 | Return **two** values from one function (min *and* max) | Easy | *Drill* | reference out-parameters |
| 15 | `static` local variable: count how many times a function was called | Easy | *Drill* | lifetime vs scope |
| 16 | Sum of digits, and reverse a number — recursively | Easy | GFG | *"Sum of Digits"*; iterative versions in `../03_Loops` |

**Why these five.** #12 is Fibonacci wearing a disguise, and recognising that — *"the number
of ways to reach step n is ways(n-1) + ways(n-2)"* — is a genuine interview skill; it's also
the canonical first DP problem. #13 extends the branching factor from 2 to 3, which tests
whether you understood #5 or memorised it. #14 answers a question people fumble ("C++
functions return one value — so how do I return two?") with the three real answers:
reference out-params, a `pair`, or a struct. #15 is the clearest demonstration that **scope
and lifetime are different things**. #16 revisits `../03_Loops` problems recursively, so you
can compare the two shapes of the same solution side by side.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Print your name N times | Easy | GFG | *"Print GFG n times"* — Striver 1.4, the simplest recursion that exists |
| Tower of Hanoi | **Medium** | GFG | *"Tower Of Hanoi"* — the classic multi-branch recursion; 2ⁿ−1 moves |
| Power of Two / Three / Four, recursively | Easy | **LeetCode 231 / 326 / 342** | compare against the loop and bit-trick forms in `../03_Loops` |
| Check if a number is prime, recursively | Easy | *Drill* | contrived on purpose — shows when recursion is the *wrong* tool |
| Sum of an array's elements, recursively | Easy | GFG | **forward reference** — needs `../07_Array` |
| Reverse an array, recursively | Easy | GFG | **forward reference** — needs `../07_Array`; Striver 1.4 |
| Check if a string is a palindrome, recursively | Easy | GFG | **forward reference** — needs `../09_Strings`; Striver 1.4 |
| Binary search, recursively | Easy | **LeetCode 704** | **forward reference** — needs `../07_Array` and `../11_linearAndBinarySearch` |
| Find the maximum recursion depth on your machine | — | *Drill* | recurse with a counter until it crashes; you'll learn your real stack limit |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6
Section 2   [ ] 7   [ ] 8   [ ] 9   [ ] 10  [ ] 11
Section 3   [ ] 12  [ ] 13  [ ] 14  [ ] 15  [ ] 16
Section 4   [ ] ______ / 9
```
