# 03 — Loops & Basic Maths: Solutions

Full C++14 for Sections 1–3 with approach progressions, dry runs and complexity.
Approach-only for Section 4. Compile with `g++ file.cpp -o file.exe`.

---

# Section 1 — Must Do

## 1. Count Digits

Two related problems share this name — do both.

### 1a. How many digits does `n` have?

```cpp
int countDigits(int n) {
    if (n < 0) n = -n;          // sign doesn't change the digit count
    if (n == 0) return 1;       // "0" has one digit
    int count = 0;
    while (n > 0) { ++count; n /= 10; }
    return count;
}
```

**The two edge cases are the entire question.** `n = 0` returns **0** from a bare `while`
loop, because the condition fails before the body ever runs. `n = -45` also returns 0.
Both are wrong, and both are invisible unless you test them.

The `do-while` form fixes the zero case structurally:

```cpp
int count = 0;
do { ++count; n /= 10; } while (n > 0);
```

**O(1) alternative** — `floor(log10(n)) + 1`. Correct in theory, but `log10` is
floating-point and can land at `2.9999999996` for `n = 1000` on some libraries, giving 3
instead of 4. **Prefer the loop.** If asked for O(1), mention `log10` *and* mention why you
wouldn't ship it.

**Complexity:** O(log₁₀ n) time — at most 10 iterations for `int`. O(1) space.

### 1b. The GFG problem: count digits of N that evenly divide N

This is what *"Count Digits"* on GFG (and Striver A2Z 1.3) actually asks.

```cpp
int evenlyDividingDigits(int n) {
    int original = n;           // save it — the loop destroys n
    int count = 0;
    while (n > 0) {
        int digit = n % 10;
        n /= 10;
        if (digit != 0 && original % digit == 0) ++count;   // skip 0: % 0 is UB
    }
    return count;
}
```

**Two traps:**
1. **`digit != 0` must come first.** A digit of `0` would make `original % digit` a
   division by zero — undefined behaviour, not an exception. Short-circuit `&&` guards it
   (see `../02_Conditional` §3).
2. **Save the original.** The loop consumes `n`, so testing `n % digit` compares against a
   progressively truncated number. `loops2.cpp:24` gets this right with `int count=0, a=n;`.

**Dry run:** `n = 1012` → digits 2, 1, 0, 1.
`1012 % 2 == 0` ✓, `1012 % 1 == 0` ✓, `0` skipped, `1012 % 1 == 0` ✓ → **3**

**Complexity:** O(log n) time, O(1) space.

---

## 2. Reverse Integer — LeetCode 7 (Medium)

> Reverse the digits of a signed 32-bit integer. **If the result overflows 32 bits, return
> 0.**

### Approach 1 — Naive (wrong, but it's what everyone writes first)

```cpp
int reverseNaive(int x) {
    int rev = 0;
    while (x != 0) {
        rev = rev * 10 + x % 10;    // OVERFLOWS silently on 10-digit input
        x /= 10;
    }
    return rev;
}
```

This is **undefined behaviour** on overflow, so you cannot rescue it with a check
afterwards. `if (rev < 0) return 0;` is not a fix — once UB has occurred the compiler is
free to assume it didn't and delete your check.

### Approach 2 — Optimal: guard before each step

```cpp
#include <climits>

int reverse(int x) {
    int rev = 0;
    while (x != 0) {
        int digit = x % 10;
        x /= 10;

        // Would rev * 10 + digit exceed INT_MAX?
        if (rev > INT_MAX / 10 || (rev == INT_MAX / 10 && digit > 7))  return 0;
        // Would it fall below INT_MIN?
        if (rev < INT_MIN / 10 || (rev == INT_MIN / 10 && digit < -8)) return 0;

        rev = rev * 10 + digit;
    }
    return rev;
}
```

**Where the 7 and the −8 come from.** `INT_MAX = 2147483647` — last digit **7**.
`INT_MIN = -2147483648` — last digit **8**. So if `rev` is already exactly `INT_MAX/10`
(= 214748364), appending anything above 7 overflows; below that threshold nothing can.
They are not magic numbers.

**Why negatives need no special case.** C++ `%` takes the sign of the dividend, so
`-123 % 10` is `-3`. The reversal builds up negative digits and lands on `-321` naturally.
Taking `abs(x)` first would be *wrong*, because `abs(INT_MIN)` overflows.

**Dry run:** `x = -123` → digit `-3`, rev `-3`, x `-12` → digit `-2`, rev `-32`, x `-1` →
digit `-1`, rev `-321` ✓

**Dry run (overflow):** `x = 1534236469` → the last step has `rev = 964632435`, which is
already `> INT_MAX/10 = 214748364`, so return **0** ✓

**Complexity:** O(log₁₀ x) time — at most 10 iterations. O(1) space.

**Interview framing.** This problem is rated Medium entirely because of the guard. State
up front: *"signed overflow is undefined behaviour, so I have to check the precondition
rather than the result."* That one sentence is what separates a pass from a fail here.

---

## 3. Palindrome Number — LeetCode 9

> Is `x` the same read forwards and backwards? Follow-up: without converting to a string.

### Approach 1 — Convert to string

```cpp
bool isPalindromeString(int x) {
    string s = to_string(x);
    int i = 0, j = s.size() - 1;
    while (i < j) if (s[i++] != s[j--]) return false;
    return true;
}
```

**Complexity:** O(log x) time, **O(log x) space**. Works, but the follow-up exists to
eliminate the extra space.

### Approach 2 — Reverse fully and compare

```cpp
bool isPalindromeFullReverse(int x) {
    if (x < 0) return false;
    long long rev = 0;             // long long avoids the overflow question entirely
    int original = x;
    while (x > 0) { rev = rev * 10 + x % 10; x /= 10; }
    return rev == original;
}
```

**Complexity:** O(log x) time, O(1) space. Correct — but note it needs `long long`, because
a palindrome check on `2147483647` would build `7463847412`, which overflows `int`.

### Approach 3 — Optimal: reverse only half

```cpp
bool isPalindrome(int x) {
    // Negatives are never palindromes: -121 reversed is 121-
    // Any non-zero multiple of 10 is out: 10 reversed is 01 = 1
    if (x < 0 || (x % 10 == 0 && x != 0)) return false;

    int reversedHalf = 0;
    while (x > reversedHalf) {
        reversedHalf = reversedHalf * 10 + x % 10;
        x /= 10;
    }
    // Even length: x == reversedHalf         (1221 -> x=12, rev=12)
    // Odd length : x == reversedHalf / 10    (121  -> x=1,  rev=12, drop the middle digit)
    return x == reversedHalf || x == reversedHalf / 10;
}
```

**Why half is enough.** Build the reversal from the back while shrinking the number from
the front. When the shrinking half becomes smaller than the built half, you've passed the
midpoint. **Reversing half can never overflow**, so no `long long` and no guard — that is
the real reason to prefer this, not the constant-factor speedup.

**Dry run** — `x = 1221`:

| x | reversedHalf | x > rev? |
|---|---|---|
| 1221 | 0 | yes → rev = 1, x = 122 |
| 122 | 1 | yes → rev = 12, x = 12 |
| 12 | 12 | **no** → stop |

`x == reversedHalf` → **true** ✓

**Dry run** — `x = 121`: rev = 1, x = 12 → rev = 12, x = 1 → stop.
`1 == 12`? no. `1 == 12/10 = 1`? **yes** ✓

**The three pre-checks are the graded part:**
- `x < 0` → false (the minus sign breaks symmetry)
- `x % 10 == 0 && x != 0` → false (trailing zero means a leading zero when reversed)
- `x == 0` → **true**, which is why the second check excludes it

**Complexity:** O(log x) time, O(1) space.

---

## 4. GCD / HCF of two numbers

### Approach 1 — Brute force

```cpp
int gcdBrute(int a, int b) {
    int g = 1;
    for (int i = 1; i <= min(a, b); ++i)
        if (a % i == 0 && b % i == 0) g = i;
    return g;
}
```

**Complexity:** O(min(a,b)) — for 10⁹ inputs, a billion iterations. Too slow.

### Approach 2 — Euclidean by repeated subtraction

```cpp
int gcdSubtraction(int a, int b) {
    while (a != b) {
        if (a > b) a -= b;
        else       b -= a;
    }
    return a;
}
```

**Complexity:** O(max(a,b)) in the worst case — `gcd(1, 10⁹)` performs a billion
subtractions. Better in spirit, still too slow. **Never hangs? It does** — this version
loops forever if either input is 0.

### Approach 3 — Optimal: Euclidean with modulo

```cpp
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Recursive form — same algorithm
int gcdRec(int a, int b) { return b == 0 ? a : gcdRec(b, a % b); }
```

**Why it works.** Any common divisor of `a` and `b` also divides `a - b`, and by extension
`a - kb` for any `k` — that is, `a % b`. So `gcd(a, b) = gcd(b, a % b)`. Each step replaces
the pair with a strictly smaller one, and `gcd(a, 0) = a` terminates it.

**Why it's fast.** `a % b < b`, and it can be shown that two consecutive steps at least
halve the larger value. So the iteration count is **O(log min(a,b))** — about 45 steps for
inputs near 10⁹. The worst case is consecutive Fibonacci numbers.

**Dry run** — `gcd(48, 18)`:

| a | b | a % b |
|---|---|---|
| 48 | 18 | 12 |
| 18 | 12 | 6 |
| 12 | 6 | 0 |
| 6 | **0** | stop → **6** ✓ |

**Free bonus — LCM:**

```cpp
long long lcm(int a, int b) {
    return (long long)a / gcd(a, b) * b;    // divide FIRST, then multiply
}
```

`a * b / gcd` overflows for large inputs; `a / gcd * b` cannot, because `gcd` divides `a`
exactly, so no precision is lost and the intermediate stays small. That reordering is
exactly the kind of detail interviewers look for.

**Complexity:** O(log min(a,b)) time, O(1) space (O(log) stack for the recursive form).

---

## 5. Check for Prime

### Approach 1 — Check every number below n

```cpp
bool isPrimeSlow(int n) {
    if (n < 2) return false;
    for (int i = 2; i < n; ++i)
        if (n % i == 0) return false;
    return true;
}
```
**O(n).**

### Approach 2 — Stop at n/2

```cpp
for (int i = 2; i <= n / 2; ++i) ...
```
**O(n/2)** — still O(n). No divisor can exceed `n/2`, but this is a weak bound.

### Approach 3 — Optimal: stop at √n

```cpp
bool isPrime(int n) {
    if (n < 2) return false;                 // 0, 1 and negatives are NOT prime
    if (n < 4) return true;                  // 2 and 3 are prime
    if (n % 2 == 0) return false;            // even numbers > 2 are out
    for (int i = 3; i * i <= n; i += 2)      // only odd candidates remain
        if (n % i == 0) return false;
    return true;
}
```

**Why √n suffices.** If `n = d × e` with `d ≤ e`, then `d ≤ √n` — because if both factors
exceeded `√n`, their product would exceed `n`. So divisors pair up around `√n`: find none
at or below `√n` and there are none at all.

For `n = 10⁹`: **31,623 iterations instead of 500,000,000.**

**Write `i * i <= n`, not `i <= sqrt(n)`.** `sqrt` returns a `double`, can be off by one
after rounding for large `n`, and re-evaluates every iteration. *(Caveat: `i * i` itself
overflows `int` once `n` nears `INT_MAX`. For those ranges use `i <= n / i`, or make `i` a
`long long`.)*

Skipping evens with `i += 2` halves the work again — a constant factor, but free.

**Dry run** — `n = 97`: `i = 3, 5, 7, 9` (`81 ≤ 97`); at `i = 11`, `121 > 97` → stop, no
divisor found → **prime** ✓

**Edge cases — this is what the question tests:**

| n | Answer |
|---|---|
| −5, 0, 1 | not prime |
| **2** | **prime** (the only even prime) |
| 3 | prime |
| 4 | not prime (smallest composite — catches a strict `i*i < n`) |

**Complexity:** O(√n) time, O(1) space.

---

# Section 2 — Important

## 6. Print all Divisors

### Approach 1 — Brute force

```cpp
vector<int> divisorsBrute(int n) {
    vector<int> res;
    for (int i = 1; i <= n; ++i)
        if (n % i == 0) res.push_back(i);
    return res;
}
```
**O(n) time.** Already sorted, but too slow for large `n`.

### Approach 2 — Optimal: pair up around √n

```cpp
#include <vector>
#include <algorithm>

vector<int> divisors(int n) {
    vector<int> res;
    for (int i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            res.push_back(i);
            if (i != n / i) res.push_back(n / i);   // avoid duplicating √n
        }
    }
    sort(res.begin(), res.end());                   // pairs come out unsorted
    return res;
}
```

**Why it works.** Divisors come in pairs `(i, n/i)`. Enumerate only the smaller member of
each pair — everything up to `√n` — and derive the larger for free.

**The `i != n / i` guard** is the graded line. For a perfect square, `i` and `n/i` are the
same number at `i = √n`, and without the guard you'd emit it twice. `n = 36` would report
`6` twice.

**Dry run** — `n = 36`:

| i | i·i ≤ 36 | 36 % i | push |
|---|---|---|---|
| 1 | ✓ | 0 | 1, 36 |
| 2 | ✓ | 0 | 2, 18 |
| 3 | ✓ | 0 | 3, 12 |
| 4 | ✓ | 0 | 4, 9 |
| 5 | ✓ | 1 | — |
| 6 | ✓ (36≤36) | 0 | 6 only — `6 == 36/6`, guard fires |
| 7 | ✗ | | stop |

Sorted: `1 2 3 4 6 9 12 18 36` ✓

**Complexity:** **O(√n)** to collect, **O(√n log √n)** for the sort — the sort dominates
but is still far below O(n). O(√n) space (a number has at most ~1344 divisors below 10⁹).

**If you don't need sorted output**, skip the sort and you're at a clean O(√n). Read the
problem statement.

---

## 7. Armstrong Number

> An `k`-digit number equals the sum of its digits each raised to the power `k`.
> `153 = 1³ + 5³ + 3³` ✓  ·  `9474 = 9⁴ + 4⁴ + 7⁴ + 4⁴` ✓

```cpp
#include <cmath>

bool isArmstrong(int n) {
    if (n < 0) return false;

    int k = 0;
    for (int t = n; t > 0; t /= 10) ++k;        // count digits (n = 0 -> k = 0)
    if (n == 0) return true;                    // 0 = 0^1, conventionally Armstrong

    long long sum = 0;                          // long long: 9^9 * 9 exceeds int
    for (int t = n; t > 0; t /= 10) {
        int digit = t % 10;
        long long p = 1;
        for (int i = 0; i < k; ++i) p *= digit; // integer power — do NOT use pow()
        sum += p;
    }
    return sum == n;
}
```

**Two passes are required** — you need the digit count `k` *before* you can raise anything
to the power `k`. Trying to do it in one pass is the most common wrong turn.

**Do not use `pow(digit, k)`.** `pow` returns a `double`, and for values like `pow(5,3)` it
can produce `124.99999999999999`, which truncates to 124. Integer exponentiation by a small
loop is exact and faster. This is a real, frequently-hit bug — not a theoretical concern.

**`long long` for the sum:** the largest 10-digit case sums ten terms of up to `9¹⁰ ≈ 3.5×10⁹`,
which overflows `int` immediately.

**Dry run** — `n = 153`: `k = 3`. Digits 3, 5, 1 → `27 + 125 + 1 = 153` → **true** ✓

**Complexity:** O(k²) where `k` = digit count (≤ 10), so effectively **O(log²n)**, or O(1)
for `int`. O(1) space.

*(The GFG version restricts to 3-digit numbers, in which case `k` is hardcoded to 3.
The general version above is Striver's.)*

---

## 8. Happy Number — LeetCode 202

> Repeatedly replace `n` by the sum of the squares of its digits. `n` is happy if this
> reaches **1**; unhappy if it loops forever.

The whole problem is: *the process either reaches 1 or enters a cycle* — so you need
**cycle detection**.

### Approach 1 — Hash set

```cpp
#include <unordered_set>

int sumOfSquaredDigits(int n) {
    int sum = 0;
    while (n > 0) { int d = n % 10; sum += d * d; n /= 10; }
    return sum;
}

bool isHappySet(int n) {
    unordered_set<int> seen;
    while (n != 1 && seen.count(n) == 0) {
        seen.insert(n);
        n = sumOfSquaredDigits(n);
    }
    return n == 1;
}
```

**Complexity:** O(log n) time, **O(log n) space**. Perfectly acceptable — but the follow-up
asks for O(1) space.

### Approach 2 — Optimal: Floyd's cycle detection (tortoise and hare)

```cpp
bool isHappy(int n) {
    int slow = n, fast = n;
    do {
        slow = sumOfSquaredDigits(slow);              // one step
        fast = sumOfSquaredDigits(sumOfSquaredDigits(fast));  // two steps
    } while (slow != fast);
    return slow == 1;
}
```

**Why it works.** The sequence is a *functional graph*: every value has exactly one
successor, so the path must eventually repeat — it's a "rho" shape, a tail leading into a
cycle. Move one pointer at 1 step/iteration and another at 2. If there's a cycle they
**must** meet inside it (the fast one gains exactly one position per iteration, so it can
never jump over the slow one). If the sequence reaches 1, then 1 → 1 forever is itself a
cycle of length 1, and they meet **at** 1.

So `slow == fast` always happens, and the answer is just whether they met at 1.

**Why it terminates at all.** For any `n < 1000`, the digit-square sum is at most
`3 × 81 = 243`. So the sequence is trapped below 243 within a couple of steps and cannot
run away.

**Dry run** — `n = 19`: `19 → 82 → 68 → 100 → 1` ✓ happy.
`n = 4`: `4 → 16 → 37 → 58 → 89 → 145 → 42 → 20 → 4` — cycle → unhappy ✓

**`do-while`, not `while`.** `slow` and `fast` both start at `n`, so a `while (slow != fast)`
loop would exit immediately. This is the one problem where `do-while` is structurally
required.

**Complexity:** O(log n) time, **O(1) space**.

This is your first Floyd's algorithm. You'll use the identical technique on
`../17_linked_list` (detect a cycle in a linked list, LeetCode 141/142) and on
*Find the Duplicate Number* (LeetCode 287). Learn it properly here.

---

## 9. Pow(x, n) — LeetCode 50 (Medium)

> Compute `x` raised to the power `n`. `n` may be negative.

### Approach 1 — Brute force

```cpp
double myPowBrute(double x, int n) {
    double res = 1;
    for (int i = 0; i < abs(n); ++i) res *= x;
    return n < 0 ? 1 / res : res;
}
```
**O(n) time.** With `n = 2³¹−1` this is 2 billion multiplications — TLE. And `abs(INT_MIN)`
is undefined behaviour.

### Approach 2 — Optimal: binary exponentiation

```cpp
double myPow(double x, int n) {
    long long N = n;             // widen FIRST: -INT_MIN overflows int
    if (N < 0) { x = 1 / x; N = -N; }

    double result = 1.0;
    while (N > 0) {
        if (N % 2 == 1) result *= x;   // this bit is set -> take this power of x
        x *= x;                        // x, x², x⁴, x⁸, ...
        N /= 2;
    }
    return result;
}
```

**Why it works.** Write the exponent in binary: `x¹³ = x^(1101₂) = x⁸ · x⁴ · x¹`. Square
`x` at each step to walk through `x, x², x⁴, x⁸, …`, and multiply into the result exactly
when the corresponding bit of `n` is set. `⌊log₂n⌋ + 1` iterations instead of `n`.

**The `long long N = n;` line is the interview.** `n` can be `INT_MIN = -2147483648`, and
`-INT_MIN` does not fit in an `int` — it's undefined behaviour. Widening before negating is
the fix. Candidates who write `n = -n` on an `int` fail the `INT_MIN` test case, and it is
in LeetCode's test set.

**Dry run** — `x = 2, n = 10` (binary `1010`):

| N | N odd? | result | x |
|---|---|---|---|
| 10 | no | 1 | 4 |
| 5 | **yes** | 4 | 16 |
| 2 | no | 4 | 256 |
| 1 | **yes** | **1024** | — |

`2¹⁰ = 1024` ✓

**Complexity:** **O(log n)** time, O(1) space.

**Why this matters far beyond this problem.** The same skeleton gives you **modular
exponentiation** (`result = result * x % MOD` — the backbone of hashing and modular
inverse), **matrix exponentiation** (nth Fibonacci in O(log n)), and it's the reason
`../26_dp` can compute large powers at all. Of everything in this folder, this is the
algorithm to know cold.

---

## 10. Count Primes — LeetCode 204 (Medium)

> Count primes **strictly less than** `n`.

### Approach 1 — Test each number

```cpp
int countPrimesBrute(int n) {
    int count = 0;
    for (int i = 2; i < n; ++i) if (isPrime(i)) ++count;   // isPrime from #5
    return count;
}
```
**O(n√n).** For `n = 5×10⁶` that's ~10¹⁰ operations — far too slow.

### Approach 2 — Optimal: Sieve of Eratosthenes

```cpp
#include <vector>

int countPrimes(int n) {
    if (n < 3) return 0;                       // n = 0, 1, 2 -> no primes strictly below n

    vector<bool> isComposite(n, false);        // index i means "is i composite?"
    int count = 0;

    for (int i = 2; i < n; ++i) {
        if (isComposite[i]) continue;
        ++count;                               // i survived -> it's prime
        // Start at i*i: every smaller multiple already has a smaller prime factor.
        for (long long j = (long long)i * i; j < n; j += i)
            isComposite[j] = true;
    }
    return count;
}
```

**Why it works.** Instead of asking "is `i` prime?" for each `i`, take each prime you find
and **cross off all its multiples**. Anything never crossed off has no prime factor, so
it's prime.

**Two optimisations that are actually required, not optional:**

1. **Start the inner loop at `i*i`, not `2*i`.** Any multiple `k·i` with `k < i` already
   has a prime factor smaller than `i`, so it was crossed off in an earlier pass. Starting
   at `i²` is what takes the sieve from O(n log n) to O(n log log n).
2. **`(long long)i * i`** — for `n` near 5×10⁶ this is fine in `int`, but for larger sieves
   `i*i` overflows. Get in the habit.

**Dry run** — `n = 20`. `i=2` prime, cross 4,6,8,…,18. `i=3` prime, cross 9,15
(6,12,18 already gone). `i=4` composite, skip. `i=5` prime, cross 25 — out of range, done.
Survivors: 2,3,5,7,11,13,17,19 → **8** ✓

**Complexity:** **O(n log log n)** time — very nearly linear. **O(n) space** for the
`vector<bool>` (which is bit-packed, so `n = 10⁷` costs about 1.2 MB, not 10 MB).

**The trade-off to state out loud.** One prime test is O(√n) and O(1) space. Testing *all*
numbers below `n` is O(n√n). The sieve pays O(n) space once to answer every query in the
range. **Sieve when you need many primes; `isPrime` when you need one.** That is the actual
interview question hiding behind this problem.

---

# Section 3 — Good to Know

## 11. Fizz Buzz — LeetCode 412

```cpp
#include <vector>
#include <string>

vector<string> fizzBuzz(int n) {
    vector<string> res;
    for (int i = 1; i <= n; ++i) {
        if      (i % 15 == 0) res.push_back("FizzBuzz");
        else if (i % 3  == 0) res.push_back("Fizz");
        else if (i % 5  == 0) res.push_back("Buzz");
        else                  res.push_back(to_string(i));
    }
    return res;
}
```

**The entire question is ladder order.** Test `% 15` **first**. Reverse it and 15 matches
`% 3` and prints `"Fizz"`, and `"FizzBuzz"` never appears — the branch is unreachable, the
same failure mode as the grade ladder in `../02_Conditional` #10.

`% 15` works because 3 and 5 are coprime, so "divisible by both" is exactly "divisible by
their LCM". For non-coprime pairs you'd need `i % 3 == 0 && i % 5 == 0`, which is also fine
here and arguably clearer.

**The string-building variant** interviewers sometimes ask for, which scales to more rules:

```cpp
string s;
if (i % 3 == 0) s += "Fizz";
if (i % 5 == 0) s += "Buzz";
res.push_back(s.empty() ? to_string(i) : s);
```

No `% 15` case at all — the concatenation handles it. Adding a `% 7 → "Bazz"` rule costs
one line here and four branches in the ladder version.

**Complexity:** O(n) time, O(n) space for the output.

---

## 12. Subtract the Product and Sum of Digits — LeetCode 1281

```cpp
int subtractProductAndSum(int n) {
    int product = 1, sum = 0;         // product starts at 1, sum at 0
    while (n > 0) {
        int digit = n % 10;
        product *= digit;
        sum     += digit;
        n /= 10;
    }
    return product - sum;
}
```

**The only trap is initialising `product` to 0**, which makes every answer `-sum`. `1` is
the multiplicative identity, `0` is the additive one.

**Dry run** — `n = 234`: digits 4, 3, 2. product `1·4·3·2 = 24`, sum `4+3+2 = 9`
→ `24 - 9 = 15` ✓

**Overflow:** constraints cap `n` at 10⁵ (5 digits), so the product is at most `9⁵ = 59049`.
Safe in `int`. For a 10-digit input, `9¹⁰ ≈ 3.5×10⁹` would overflow — check the constraints
before assuming.

**Complexity:** O(log n) time, O(1) space.

---

## 13. Sqrt(x) — LeetCode 69

> Return `⌊√x⌋`, the integer square root. No built-in `sqrt`.

### Approach 1 — Linear scan

```cpp
int mySqrtLinear(int x) {
    int i = 0;
    while ((long long)(i + 1) * (i + 1) <= x) ++i;
    return i;
}
```
**O(√x).** For `x = 2³¹−1` that's ~46,341 iterations — passes, but misses the point.

### Approach 2 — Optimal: binary search

```cpp
int mySqrt(int x) {
    if (x < 2) return x;                    // 0 -> 0, 1 -> 1

    int low = 1, high = x / 2, ans = 1;     // √x ≤ x/2 for all x ≥ 2
    while (low <= high) {
        int mid = low + (high - low) / 2;   // NOT (low+high)/2 — overflow
        long long sq = (long long)mid * mid;

        if (sq == x)      return mid;
        else if (sq < x) { ans = mid; low = mid + 1; }   // mid works, try bigger
        else              high = mid - 1;                // too big
    }
    return ans;
}
```

**Why binary search applies.** `mid * mid` is **monotonically increasing** in `mid`. That
monotonicity is the only precondition binary search needs — the array doesn't have to
exist, it can be an implicit "answer space". This reframing (*binary search on the answer*)
is one of the highest-value patterns in all of DSA, and you'll meet it again in
`../11_linearAndBinarySearch` on problems like Koko Eating Bananas.

**Two mandatory details:**
1. **`low + (high - low) / 2`** — the classic overflow avoidance from `../01_basics` §9.
2. **`(long long)mid * mid`** — for `mid` near 46,341 the square exceeds `INT_MAX`. Cast
   before multiplying.

**Dry run** — `x = 8`: `low=1, high=4`. `mid=2`, `4 < 8` → `ans=2, low=3`. `mid=3`,
`9 > 8` → `high=2`. Loop ends → **2** ✓ (`⌊√8⌋ = 2`)

**Complexity:** **O(log x)** time, O(1) space.

---

## 14. Power of Two — LeetCode 231

### Approach 1 — Repeated division

```cpp
bool isPowerOfTwoLoop(int n) {
    if (n <= 0) return false;
    while (n % 2 == 0) n /= 2;
    return n == 1;
}
```
**O(log n).** Perfectly good.

### Approach 2 — Optimal: one bit trick

```cpp
bool isPowerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}
```

**Why it works.** A power of two has **exactly one bit set**: `8 = 1000₂`. Subtracting 1
flips that bit off and turns every bit below it on: `7 = 0111₂`. The two share no bits, so
the AND is 0.

For a non-power like `12 = 1100₂`, `11 = 1011₂`, and `12 & 11 = 1000₂ ≠ 0`.

**`n > 0` is mandatory.** `n = 0` gives `0 & -1 == 0`, which would wrongly report true. And
negatives are never powers of two.

**Dry run:** `n = 16 = 10000₂`, `n-1 = 15 = 01111₂`, AND = `0` → **true** ✓
`n = 6 = 110₂`, `n-1 = 5 = 101₂`, AND = `100₂ = 4 ≠ 0` → **false** ✓

**Complexity:** **O(1)** time, O(1) space.

`n & (n-1)` — "clear the lowest set bit" — is the most reusable bit trick there is. It also
gives you popcount in O(number of set bits): `while (n) { n &= n-1; ++count; }`. See
`../15_bitwise`.

---

## 15. Number of Steps to Reduce a Number to Zero — LeetCode 1342

```cpp
int numberOfSteps(int num) {
    int steps = 0;
    while (num > 0) {
        if (num % 2 == 0) num /= 2;
        else              num -= 1;
        ++steps;
    }
    return steps;
}
```

**Complexity:** O(log n) time, O(1) space.

**The bit-level reformulation** — worth knowing because it explains *why* it's O(log n):

```cpp
int numberOfStepsBits(int num) {
    if (num == 0) return 0;
    int bits = 0, setBits = 0;
    for (int t = num; t > 0; t >>= 1) { ++bits; if (t & 1) ++setBits; }
    return (bits - 1) + setBits;
}
```

Halving is a right shift; subtracting 1 from an odd number clears its lowest bit. So every
bit position costs one shift (that's `bits - 1` shifts to consume them all) and every set
bit costs one extra subtraction. Total: `(bits − 1) + setBits`.

**Dry run** — `num = 14 = 1110₂`: 4 bits, 3 set → `3 + 3 = 6`.
Trace: `14→7→6→3→2→1→0` = **6 steps** ✓

**Complexity:** still O(log n) — but it explains the shape of the answer, which is what an
interviewer is probing when they ask "can you do better?"

---

# Section 4 — Extra Practice (approach only)

### Perfect Number — LeetCode 507 (Easy)
A number equal to the sum of its **proper** divisors (excluding itself): `28 = 1+2+4+7+14`.
Reuse #6's √n pairing: for each `i` with `i*i <= num`, add `i` and `num/i`, then subtract
`num` at the end (since `i = 1` contributes `num` as its pair). Watch the perfect-square
double-count and start from `i = 1` with `num > 1` guarded.
**O(√n) time, O(1) space.**

### Self Dividing Numbers — LeetCode 728 (Easy)
For each candidate in `[left, right]`, run the digit loop and check `num % digit == 0` for
every digit. **A digit of 0 disqualifies the number immediately** — and you must test for
it before dividing, since `% 0` is undefined behaviour. Same guard as #1b.
**O((right−left) · log(right)) time, O(1) extra space.**

### Ugly Number — LeetCode 263 (Easy)
Divide out all factors of 2, then 3, then 5; the number is ugly iff what remains is `1`.
Handle `n <= 0` as false before the loop — otherwise `n = 0` loops forever, since `0 % 2`
is `0` and `0 / 2` is `0`. **O(log n) time, O(1) space.**

### Power of Three / Power of Four — LeetCode 326 / 342
Power of three: repeated division, or the O(1) trick `1162261467 % n == 0` (that's 3¹⁹, the
largest power of 3 in `int`, and it works only because 3 is prime). Power of four: it's a
power of two **with the set bit in an even position** — `n > 0 && (n & (n-1)) == 0 && (n & 0x55555555)`.
Contrasting these three styles against #14 is the real exercise. **O(log n) or O(1).**

### Excel Sheet Column Number / Title — LeetCode 171 / 168
Number → title is base-26, but **1-indexed** (`A`=1, not 0), so you must do `--n` before
each `n % 26` or `Z` maps to the wrong letter. Title → number is the straightforward
Horner's-rule direction: `result = result * 26 + (c - 'A' + 1)`. The 1-indexing is the
entire difficulty — this is a "bijective base-26" system, not ordinary base-26.
**O(log₂₆ n) time.**

### Factorial Trailing Zeroes — LeetCode 172 (Medium)
**Do not compute the factorial** — `n!` overflows immediately. A trailing zero comes from a
factor of 10 = 2 × 5, and factors of 2 are always more plentiful, so the answer is the
number of 5s in the prime factorisation: `n/5 + n/25 + n/125 + …` (Legendre's formula).
Each term counts numbers contributing an *additional* factor of 5. **O(log₅ n) time, O(1) space.**

### Divide Two Integers — LeetCode 29 (Medium)
Division without `/` or `%`. Subtract the divisor doubled repeatedly (`divisor`, `2×`, `4×`,
…), largest chunk first — the same binary decomposition as #9's exponentiation. **The
graded case is `dividend = INT_MIN, divisor = -1`**: the true answer `2147483648` doesn't
fit in `int`, so the problem asks you to return `INT_MAX`. Work in `long long` throughout.
**O(log²n) time, O(1) space.**

### Sum of all divisors from 1 to N — GFG (Striver A2Z)
Naive: for each `i` in `1..n`, sum its divisors — O(n√n). The trick is to **flip the
iteration**: instead of asking "what divides `i`?", ask "how many numbers ≤ n does `i`
divide?" The answer is `⌊n/i⌋`, so the total is `Σ i·⌊n/i⌋` for `i` in `1..n`.
**O(n) time, O(1) space** — and the same flip powers divisor-count sieves.

### Factorial, nth Fibonacci, sum of first N naturals
The three warm-up loops in `imp.cpp`. Factorial needs `long long` and overflows at 21.
Fibonacci needs only two rolling variables, not an array — O(1) space. Sum of first N is
`n(n+1)/2` in O(1), and the loop version exists only to be compared against it; note that
`n*(n+1)` overflows `int` for `n` above ~46,340, so compute it in `long long`.

---

# Bugs in your current code

Per the repo's convention I have not edited your `.cpp` files. All three compile cleanly
(verified with `g++ -fsyntax-only`) — these are logic bugs.

### `03_Loops/loops2.cpp`

| Line | Problem | Fix |
|---|---|---|
| 6-9 | **Prime check reports `0` and `1` as prime.** With `n ≤ 1`, `n/2` is 0, the loop body never runs, the flag `x` stays 0, and the code prints *"Given number is prime"* | add `if (n < 2) { /* not prime */ }` before the loop |
| 8 | No `break` after finding a factor — always performs all `n/2` iterations even when `n` is even | `if (n % i == 0) { x = 1; break; }` |
| 6 | `i <= n/2` is **O(n)**; `i * i <= n` is **O(√n)** | see solution #5 |
| 5, 23, 34, 44 | `cin >> n;` comes **before** the `cout` prompt, in all four blocks — the user sees a blank screen and the prompt appears after they've typed | swap the two lines |
| 36-42, 46-52 | Sum-of-digits and reverse both use `while (n > 0)`, so a negative input silently produces `0` | take `abs` first, or handle the sign explicitly |

What's **right** here: line 24 saves `int a = n;` before the loop consumes `n`, and lines
30-33 special-case the digit count of `0`. Both are exactly the habits this folder is
about.

### `03_Loops/imp.cpp`

| Line | Problem | Fix |
|---|---|---|
| 36-40 | **The power loop is wrong.** `for (i=2; i<=b; i++) a *= a;` squares `a` repeatedly, computing `a^(2^(b-1))`, not `a^b`. For `a=2, b=3` it gives **16**, not 8 | `long long res = 1; for (int i = 0; i < b; ++i) res *= a;` — or better, binary exponentiation (solution #9) |
| 36-40 | The result is **never printed** — the `else` branch computes into `a` and the program ends | add the `cout` |
| 34 | Prompt `"enter the number "` asks for one value but `cin >> a >> b` reads two | reword the prompt |
| 26-33 | Fibonacci uses `int`, which overflows at **n = 47** (`fib(47) = 2971215073 > INT_MAX`) | use `long long` |
| 14 | `long long factorial` is correct — but still overflows at **n = 21**, silently | document the limit, or return the answer mod 10⁹+7 |

The factorial block is otherwise well written: it rejects negative input explicitly, which
is the branch most people skip.

### `03_Loops/loops1.cpp`

No bugs. The AP and GP loops are correct, and using a running `term *= 2` for the GP rather
than recomputing `pow(2, i)` is the right instinct — it's O(1) per term and exact, where
`pow` is neither.
