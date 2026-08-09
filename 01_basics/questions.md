# 01 — Basics: Practice Questions

**How to use this file.** Sections 1 → 3 are ordered by interview importance. Do Section 1
before anything else. Solve each one yourself, *then* read the matching entry in
`solution.md` — reading first teaches you nothing.

**Scope rule.** This folder has no loops and no conditionals yet, so every problem here is
solvable with straight-line arithmetic, type conversion and I/O only. Problems needing a
loop are flagged as forward references.

**A note on platform links.** LeetCode problem numbers are stable and given exactly.
**GeeksforGeeks has no numeric problem IDs** — search the given title on
[practice.geeksforgeeks.org](https://practice.geeksforgeeks.org). A few entries are pure
concept drills with no judge equivalent; those are marked *Drill* and you should still
write and run them.

---

## Section 1 — Must Do (highest interview value)

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Read two numbers and print sum, difference, product, quotient, remainder | Easy | GFG | *"Sum of two numbers"* / any judge's I/O warm-up |
| 2 | Swap two numbers — with and without a third variable | Easy | GFG | *"Swap two numbers"* |
| 3 | Convert the Temperature | Easy | **LeetCode 2469** | `leetcode.com/problems/convert-the-temperature` |
| 4 | Add Digits (digital root) | Easy | **LeetCode 258** | `leetcode.com/problems/add-digits` |
| 5 | Count Odd Numbers in an Interval Range | Easy | **LeetCode 1523** | `leetcode.com/problems/count-odd-numbers-in-an-interval-range` |

**Why these five.** #1 and #2 are the I/O and swap idioms you will type in every single
later problem. #3 forces you to confront integer vs floating-point division. #4 and #5 are
the first two problems where the naive loop answer and the O(1) closed-form answer differ —
which is the entire skill interviews test.

---
`
## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 6 | A Number After a Double Reversal | Easy | **LeetCode 2119** | `leetcode.com/problems/a-number-after-a-double-reversal` |
| 7 | Smallest Even Multiple | Easy | **LeetCode 2413** | `leetcode.com/problems/smallest-even-multiple` |
| 8 | Count of Matches in Tournament | Easy | **LeetCode 1688** | `leetcode.com/problems/count-of-matches-in-tournament` |
| 9 | Area of a circle + simple interest (float precision) | Easy | GFG | *"Simple Interest"* — also `01_basics/lec1.cpp:70-80` |
| 10 | ASCII value and character arithmetic | Easy | GFG | *"ASCII values of characters"* |

**Why these five.** #6, #7 and #8 all look like they need a loop or a simulation and all
collapse to a one-line observation — that is the exact muscle interviewers probe with
"can you do better?". #9 is where `float` vs `double` and integer division bite in
practice. #10 is the foundation of every string problem in `09_Strings` (`c - 'a'` as an
array index).

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Add Two Integers | Easy | **LeetCode 2235** | `leetcode.com/problems/add-two-integers` |
| 12 | Find the Maximum Achievable Number | Easy | **LeetCode 2769** | `leetcode.com/problems/find-the-maximum-achievable-number` |
| 13 | Detect whether `a + b` overflows `int` (without computing it) | Medium | *Drill* | core of LeetCode 7's overflow guard |
| 14 | Predict the output: pre vs post increment | Easy | *Drill* | `01_basics/lec1.cpp:36-40` |
| 15 | Split a 3-digit number into its digits without a loop | Easy | *Drill* | leads into `03_Loops` count-digits |

**Why these five.** #11 and #12 are trivial as problems but are the cleanest possible check
that you have the I/O and function-signature mechanics right. #13, #14 and #15 are drills
rather than judge problems — they exist because #13 is a sub-problem of LeetCode 7,
#14 is a perennial written-round question, and #15 is the manual version of the
digit-extraction loop you will write a dozen times in `03_Loops`.

---

## Section 4 — Extra Practice

Solve these after Sections 1–3. The first four need a loop, so come back to them once
you've finished `03_Loops` — they're listed here because they're the natural continuation
of the digit arithmetic above.

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Reverse Integer | Medium | **LeetCode 7** | needs a loop; the overflow guard is #13 above |
| Palindrome Number | Easy | **LeetCode 9** | needs a loop; the "don't convert to string" version is the real question |
| Subtract the Product and Sum of Digits of an Integer | Easy | **LeetCode 1281** | needs a loop |
| Number of Steps to Reduce a Number to Zero | Easy | **LeetCode 1342** | needs a loop |
| Convert Celsius → Fahrenheit **and** Fahrenheit → Celsius | Easy | GFG | *"Convert Celsius To Fahrenheit"* |
| Compound interest — `A = P(1 + r/100)^t` | Easy | GFG | needs `pow()` from `<cmath>` |
| Total seconds → hours : minutes : seconds | Easy | *Drill* | pure `/` and `%` — the classic interview warm-up |
| Nature of the roots of a quadratic (uses the discriminant) | Easy | GFG | *"Quadratic Equation Roots"* — needs conditionals, see `02_Conditional` |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5
Section 2   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 3   [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15
Section 4   [ ] ______ / 8
```
