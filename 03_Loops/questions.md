# 03 — Loops & Basic Maths: Practice Questions

**This is the most important of the first four folders.** Sections 1 and 2 are Striver A2Z
Step 1.3 in full, and every one of these appears in real interviews — not as the whole
question, but as a sub-step inside a harder one.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** —
search the quoted title on [practice.geeksforgeeks.org](https://practice.geeksforgeeks.org).

---

## Section 1 — Must Do (highest interview value)

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Count Digits | Easy | GFG | *"Count Digits"* — Striver A2Z 1.3 |
| 2 | Reverse Integer | **Medium** | **LeetCode 7** | `leetcode.com/problems/reverse-integer` |
| 3 | Palindrome Number | Easy | **LeetCode 9** | `leetcode.com/problems/palindrome-number` |
| 4 | GCD / HCF of two numbers | Easy | GFG | *"LCM And GCD"* — Striver A2Z 1.3 |
| 5 | Check for Prime | Easy | GFG | *"Prime Number"* — Striver A2Z 1.3 |

**Why these five.** They are the five sub-routines that show up *inside* other problems for
the rest of your prep. #2 is Medium purely because of overflow — the reversal itself is
four lines, and the overflow guard is the interview. #4's Euclidean algorithm is the first
genuinely non-obvious algorithm in this repo and reappears in fractions, LCM, and modular
inverse. #5's `i*i <= n` is your first O(√n) argument, and its `n < 2` edge case is the bug
currently sitting in `loops2.cpp:6`.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 6 | Print all Divisors of a number | Easy | GFG | *"All Divisors of a Number"* — Striver A2Z 1.3 |
| 7 | Armstrong Number | Easy | GFG | *"Armstrong Numbers"* — Striver A2Z 1.3 |
| 8 | Happy Number | Easy | **LeetCode 202** | `leetcode.com/problems/happy-number` |
| 9 | Pow(x, n) | **Medium** | **LeetCode 50** | `leetcode.com/problems/powx-n` |
| 10 | Count Primes | **Medium** | **LeetCode 204** | `leetcode.com/problems/count-primes` |

**Why these five.** #6 is #5's √n pairing trick reused to *enumerate* rather than just
test. #7 is the digit loop with a twist. #8 is your first **cycle detection** problem —
the hash-set answer is fine, the Floyd's-tortoise-and-hare answer is the one that gets you
past `17_linked_list`. #9 is **binary exponentiation**, the single most reusable algorithm
in this folder (modular exponentiation, matrix power, fast Fibonacci all derive from it).
#10 is the **Sieve of Eratosthenes** — the standard answer to "now do it for all numbers up
to n", and the one place where an O(n log log n) precomputation beats n separate O(√n)
checks.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Fizz Buzz | Easy | **LeetCode 412** | `leetcode.com/problems/fizz-buzz` |
| 12 | Subtract the Product and Sum of Digits of an Integer | Easy | **LeetCode 1281** | `leetcode.com/problems/subtract-the-product-and-sum-of-digits-of-an-integer` |
| 13 | Sqrt(x) | Easy | **LeetCode 69** | `leetcode.com/problems/sqrtx` |
| 14 | Power of Two | Easy | **LeetCode 231** | `leetcode.com/problems/power-of-two` |
| 15 | Number of Steps to Reduce a Number to Zero | Easy | **LeetCode 1342** | `leetcode.com/problems/number-of-steps-to-reduce-a-number-to-zero` |

**Why these five.** #11 is the famous screening question and it is *not* about FizzBuzz —
it's about whether you check `15` before `3` and `5`. #12 is the digit loop with two
accumulators. #13 is where a loop becomes a **binary search**, previewing
`../11_linearAndBinarySearch`. #14 introduces `n & (n-1)`, the bit trick you'll use
constantly in `../15_bitwise`. #15 has a neat O(1)-ish reformulation in terms of bit counts.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Perfect Number | Easy | **LeetCode 507** | divisor sum — reuses #6's √n pairing |
| Self Dividing Numbers | Easy | **LeetCode 728** | digit loop + divisibility, watch the zero digit |
| Ugly Number | Easy | **LeetCode 263** | repeated division by 2, 3, 5 |
| Power of Three / Power of Four | Easy | **LeetCode 326 / 342** | contrast the loop, the log, and the bit-trick answers |
| Excel Sheet Column Number / Title | Easy / Medium | **LeetCode 171 / 168** | base-26 conversion; the *title* direction is 1-indexed, which is the trap |
| Factorial Trailing Zeroes | Medium | **LeetCode 172** | `n/5 + n/25 + n/125 + …` — never compute the factorial |
| Divide Two Integers | Medium | **LeetCode 29** | division without `/`; repeated doubling + the `INT_MIN / -1` overflow case |
| Sum of all divisors from 1 to N | Easy | GFG | *"Sum of all divisors from 1 to n"* — Striver A2Z; the O(n) harmonic trick beats the O(n√n) loop |
| Factorial, nth Fibonacci, sum of first N naturals | Easy | GFG | the three warm-up loops — also `imp.cpp` |

---

## Progress tracker

```
Section 1   [done  ] 1   [done  ] 2   [done  ] 3   [done ] 4   [done] 5
Section 2   [ done ] 6   [done  ] 7   [done  ] 8   [done] 9   [done] 10
Section 3   [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15
Section 4   [ ] ______ / 9
```
