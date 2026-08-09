# 02 — Conditionals: Practice Questions

**How to use this file.** Sections 1 → 3 are ordered by interview importance. Solve first,
read `solution.md` after.

**Scope rule.** No loops and no arrays yet — every problem here is decidable with a fixed
number of branches on a fixed number of inputs.

**Why this folder is drill-heavy — read this.** LeetCode has very few pure-conditional
problems, because almost every LeetCode problem hands you an array or a string, which
immediately requires a loop. That is a property of the platform, not a sign that
conditionals are unimportant: branch enumeration is where most wrong-answer verdicts come
from. So this list mixes the real judge problems that *do* exist with classic drills
(marked *Drill*) that every interviewer has asked at some point. **Write and run the
drills** — don't skip them because there's no green checkmark at the end.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** —
search the quoted title on [practice.geeksforgeeks.org](https://practice.geeksforgeeks.org).

---

## Section 1 — Must Do (highest interview value)

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Even or Odd — correct for negative inputs too | Easy | GFG | *"Odd or Even"* |
| 2 | Largest of three numbers | Easy | GFG | *"Maximum of three numbers"* — also `lec3.cpp:56` |
| 3 | Nim Game | Easy | **LeetCode 292** | `leetcode.com/problems/nim-game` |
| 4 | Leap year | Easy | GFG | *"Leap Year"* |
| 5 | Absolute value and sign of a number, without `abs()` | Easy | *Drill* | `lec3.cpp:20` |

**Why these five.** #1 is the single most common silent bug in beginner code
(`n % 2 == 1` misses negatives) and it appears inside dozens of harder problems. #2 is the
canonical "enumerate your branches, including the ties" exercise. #3 looks like game theory
and collapses to one modulo — it's the best short example of *find the invariant instead of
simulating*. #4 is a three-level nested rule that catches people who stop after the first
condition. #5 is where `if` and the ternary meet, and the branchless version teaches you
what a compiler actually does.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 6 | Pass the Pillow | Easy | **LeetCode 2582** | `leetcode.com/problems/pass-the-pillow` |
| 7 | Simple calculator using `switch` (with divide-by-zero guard) | Easy | GFG | *"Simple Calculator"* — also `lec4.cpp:40` |
| 8 | Vowel or consonant | Easy | GFG | *"Vowel or Not"* |
| 9 | Profit, loss, or break-even | Easy | *Drill* | `lec3.cpp:28` |
| 10 | Grade from marks (else-if ladder) | Easy | *Drill* | classic written-round question |

**Why these five.** #6 is the folder's best problem: the O(1) answer needs one modulo plus
one conditional, and almost everyone writes the simulation first. #7 is `switch` done
properly — the `default` branch and the divide-by-zero guard are the graded parts, not the
arithmetic. #8 is the character-range idiom you'll reuse throughout `09_Strings`. #9 and
#10 are ladder-ordering drills: #9 has a three-way outcome people reduce to two, and #10
punishes an unordered ladder with unreachable branches.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Triangle validity and type (equilateral / isosceles / scalene) from 3 sides | Easy | *Drill* | conditional core of **LeetCode 976** |
| 12 | Nature of the roots of a quadratic equation | Easy | GFG | *"Quadratic Equation Roots"* |
| 13 | Day of the week from a number, using `switch` | Easy | *Drill* | `lec4.cpp:5` |
| 14 | How many digits does `n` have? (by range, no loop) | Easy | *Drill* | `lec3.cpp:36` |
| 15 | Character classification: uppercase / lowercase / digit / special | Easy | GFG | *"Character Classification"* |

**Why these five.** #11 is the triangle inequality — the exact condition that makes
LeetCode 976 work, isolated from the sorting. #12 is the discriminant, and the trap is the
`a == 0` case (then it isn't quadratic). #13 is the `switch` you already wrote, revisited
for the `default` branch and input validation. #14 previews the digit-counting loop you'll
generalise in `../03_Loops`. #15 is `isupper`/`islower`/`isdigit` written by hand, which is
what an interviewer wants to see before you reach for `<cctype>`.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Largest Perimeter Triangle | Easy | **LeetCode 976** | uses #11's condition; needs sorting — revisit after `../12_sorting` |
| Minimum Sum of Four Digit Number After Splitting Digits | Easy | **LeetCode 2160** | digit extraction + ordering four values |
| Roman numeral character → value, using `switch` | Easy | *Drill* | the sub-problem inside **LeetCode 13** |
| Days in a month (deliberate `switch` fall-through) | Easy | *Drill* | the canonical *legitimate* fall-through |
| Min and max of two numbers without `if` | Easy | *Drill* | ternary, then the branchless bit trick |
| Electricity bill from slab rates | Easy | *Drill* | cumulative else-if ladder — ordering is the whole problem |
| Income tax from slabs | Easy | *Drill* | same shape as the bill, with a bracket subtlety |
| BMI category | Easy | *Drill* | float comparison + ladder boundaries |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5
Section 2   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 3   [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15
Section 4   [ ] ______ / 8
```
