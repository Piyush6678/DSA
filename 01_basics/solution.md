# 01 — Basics: Solutions

Solutions for every question in `questions.md`. Sections 1–3 get full compilable C++14,
an approach progression where more than one approach exists, a dry run, and complexity.
Section 4 gets the approach and the key insight without full code.

All code compiles with this repo's toolchain:
`g++ file.cpp -o file.exe` (MinGW g++ 6.3.0, defaults to `gnu++14`).

---

# Section 1 — Must Do

## 1. Sum, difference, product, quotient, remainder of two numbers

**The point of this problem** is not the arithmetic — it's `/` and `%` on integers, and
getting a real quotient out of two ints.

```cpp
#include <iostream>
using namespace std;

int main() {
    int a, b;
    cout << "Enter two integers: ";
    cin >> a >> b;

    cout << "Sum        = " << a + b << "\n";
    cout << "Difference = " << a - b << "\n";
    cout << "Product    = " << (long long)a * b << "\n";   // cast BEFORE multiplying

    if (b != 0) {                                 // guard: a % 0 is undefined behaviour
        cout << "Quotient (int)  = " << a / b << "\n";
        cout << "Quotient (real) = " << (double)a / b << "\n";
        cout << "Remainder       = " << a % b << "\n";
    } else {
        cout << "Division by zero is undefined.\n";
    }
    return 0;
}
```

**Three things an interviewer would notice:**

1. `(long long)a * b` — two ints near 2×10⁹ overflow when multiplied. The cast is on the
   *first operand*, so the multiplication itself happens in 64-bit. `(long long)(a*b)` is
   too late.
2. `(double)a / b` — without the cast, `7/2` is `3`, not `3.5`.
3. The `b != 0` guard. `a % 0` is not an exception; it kills the process with SIGFPE.

**Complexity:** O(1) time, O(1) space.

---

## 2. Swap two numbers

### Approach 1 — temporary variable (what you should write in an interview)

```cpp
void swapWithTemp(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}
```

### Approach 2 — arithmetic, no temp

```cpp
void swapWithArithmetic(int &a, int &b) {
    a = a + b;   // OVERFLOW RISK: a+b may exceed int
    b = a - b;   // b = (a+b) - b = original a
    a = a - b;   // a = (a+b) - original a = original b
}
```

### Approach 3 — XOR, no temp

```cpp
void swapWithXor(int &a, int &b) {
    if (&a == &b) return;   // MUST guard: x ^= x zeroes the variable
    a = a ^ b;
    b = a ^ b;   // = (a^b)^b = original a
    a = a ^ b;   // = (a^b)^a = original b
}
```

**The correct interview answer is Approach 1**, and you should say why:

- Approach 2 **overflows** for large values. `a = 2×10⁹, b = 2×10⁹` breaks it, and signed
  overflow is undefined behaviour.
- Approach 3 **fails when both references alias the same variable** — `swapWithXor(x, x)`
  sets `x` to 0. This is the actual gotcha the question exists to test.
- Neither is faster. Modern compilers turn Approach 1 into three register moves, or into
  nothing at all if they can just rename the registers.
- In real C++ you would write `std::swap(a, b)`.

Knowing the tricks matters; *choosing* them does not. Say "temp variable, because XOR
breaks on self-swap and arithmetic overflows" and you've answered the real question.

**Complexity:** O(1) time, O(1) space for all three.

---

## 3. Convert the Temperature — LeetCode 2469

> Given `celsius`, return `[kelvin, fahrenheit]` where
> `kelvin = celsius + 273.15` and `fahrenheit = celsius * 1.80 + 32.00`.

```cpp
#include <iostream>
#include <vector>
using namespace std;

vector<double> convertTemperature(double celsius) {
    return { celsius + 273.15, celsius * 1.80 + 32.00 };
}

int main() {
    vector<double> ans = convertTemperature(36.50);
    cout << ans[0] << " " << ans[1];   // 309.65 97.7
    return 0;
}
```

**Why this is question #3 and not a throwaway.** The whole problem is type discipline. If
you declare `celsius` as `int`, or write `celsius * 9/5 + 32`, you get the wrong answer:
`9/5` is integer division, which is `1`. The correct forms are `* 1.80` or `* 9.0/5.0`.

**Dry run:** `celsius = 36.50` → kelvin `36.50 + 273.15 = 309.65`;
fahrenheit `36.50 × 1.8 = 65.7`, `+ 32 = 97.7`. ✓

**Complexity:** O(1) time, O(1) space (the returned vector holds a fixed 2 elements).

---

## 4. Add Digits (digital root) — LeetCode 258

> Repeatedly sum the digits of `num` until one digit remains. Follow-up: do it in O(1)
> without a loop.

### Approach 1 — Brute force: simulate

```cpp
int addDigitsBrute(int num) {
    while (num >= 10) {
        int sum = 0;
        while (num > 0) { sum += num % 10; num /= 10; }
        num = sum;
    }
    return num;
}
```

**Complexity:** O(log num) per pass, O(log* num) passes — effectively O(log num).
Space O(1). *(This uses loops, so it's really `03_Loops` material — included for contrast.)*

### Approach 2 — Optimal: digital root formula, O(1)

```cpp
int addDigits(int num) {
    if (num == 0) return 0;
    return 1 + (num - 1) % 9;
}
```

**Why it works.** Write a number in base 10:
`num = d₀·10⁰ + d₁·10¹ + d₂·10² + …`

Since `10 ≡ 1 (mod 9)`, every power `10^k ≡ 1 (mod 9)`. So

`num ≡ d₀ + d₁ + d₂ + … (mod 9)`

The digit sum preserves the value mod 9 — and so does repeating it. The final one-digit
result is therefore congruent to `num` mod 9, and lies in `1..9` for `num > 0`.

Mapping "`num mod 9`, but with 0 becoming 9" onto `1..9` is exactly `1 + (num-1) % 9`.
The `-1 … +1` shift is what avoids a special case for multiples of 9.

**Dry run:**
- `num = 38` → `1 + 37 % 9` = `1 + 1` = `2`. Check: 3+8=11 → 1+1=2 ✓
- `num = 9` → `1 + 8 % 9` = `1 + 8` = `9` ✓
- `num = 18` → `1 + 17 % 9` = `1 + 8` = `9` ✓ (1+8=9)
- `num = 0` → special-cased to `0` ✓

**Complexity:** O(1) time, O(1) space.

This is the canonical "the brute force is a loop, the answer is one line of number theory"
problem. If you only produce Approach 1 in an interview, expect "can you do it without a
loop?"

---

## 5. Count Odd Numbers in an Interval Range — LeetCode 1523

> Count odd numbers in `[low, high]` inclusive.

### Approach 1 — Brute force

```cpp
int countOddsBrute(int low, int high) {
    int count = 0;
    for (int i = low; i <= high; ++i)
        if (i % 2 != 0) ++count;
    return count;
}
```

**Complexity:** O(high − low) time. With `high` up to 10⁹ this is far too slow — and that
is the whole point of the problem.

### Approach 2 — Optimal: counting formula, O(1)

```cpp
int countOdds(int low, int high) {
    return (high + 1) / 2 - low / 2;
}
```

**Why it works.** Let `f(x)` = "how many odd numbers are in `[0, x)`" = `x / 2`
(integer division). The odd numbers below `x` are 1, 3, 5, … so there are exactly
`⌊x/2⌋` of them.

We want odds in the closed interval `[low, high]`, i.e. odds in `[0, high+1)` minus odds
in `[0, low)`:

`answer = f(high + 1) − f(low) = (high+1)/2 − low/2`

**Dry run:**
- `low=3, high=7` → `8/2 − 3/2` = `4 − 1` = `3`. Check: {3,5,7} ✓
- `low=8, high=10` → `11/2 − 8/2` = `5 − 4` = `1`. Check: {9} ✓
- `low=1, high=1` → `2/2 − 1/2` = `1 − 0` = `1` ✓

**Note on `n % 2 != 0`** in the brute force — the constraints guarantee `low ≥ 0`, but the
habit matters. `i % 2 == 1` would silently miss every negative odd number.

**Complexity:** O(1) time, O(1) space.

---

# Section 2 — Important

## 6. A Number After a Double Reversal — LeetCode 2119

> Reverse `num`'s digits, reverse the result, and return whether you get `num` back.

### Approach 1 — Brute force: actually reverse twice

```cpp
int reverseNum(int n) {
    int rev = 0;
    while (n > 0) { rev = rev * 10 + n % 10; n /= 10; }
    return rev;
}
bool isSameAfterReversalsBrute(int num) {
    return reverseNum(reverseNum(num)) == num;
}
```

**Complexity:** O(log num) time, O(1) space.

### Approach 2 — Optimal: one observation, O(1)

```cpp
bool isSameAfterReversals(int num) {
    return num % 10 != 0 || num == 0;
}
```

**Why it works.** Reversing loses information in exactly one situation: **trailing zeros**.
`1800` reversed is `0081` = `81`, and reversing `81` gives `18`, not `1800` — the leading
zeros produced by the first reversal simply vanish.

Any number with no trailing zero survives both reversals unchanged. So the answer is
"does `num` end in 0?" — with `0` itself as the one exception, since `0` reversed is `0`.

**Dry run:**
- `num = 526` → `526 % 10 = 6 ≠ 0` → `true` ✓
- `num = 1800` → `1800 % 10 = 0` and `num ≠ 0` → `false` ✓
- `num = 0` → `0 % 10 == 0` but `num == 0` → `true` ✓

**Complexity:** O(1) time, O(1) space.

The `|| num == 0` clause is the entire difficulty. Candidates who write
`return num % 10 != 0;` fail on the single test case `num = 0`. Always ask yourself what
your one-liner does at the boundary.

---

## 7. Smallest Even Multiple — LeetCode 2413

> Return the smallest positive integer that is a multiple of both `2` and `n`.

```cpp
int smallestEvenMultiple(int n) {
    return (n % 2 == 0) ? n : 2 * n;
}
```

**Why it works.** The smallest common multiple of 2 and `n` is `lcm(2, n)`, and
`lcm(a,b) = a*b / gcd(a,b)`. Here `gcd(2, n)` is `2` when `n` is even and `1` when `n` is
odd, giving `n` and `2n` respectively.

Stated plainly: if `n` is already even it is its own answer; if `n` is odd you must
multiply by 2 to introduce the factor of 2.

**One-liner variant** (bit manipulation, `15_bitwise` material):

```cpp
int smallestEvenMultiple(int n) { return n << (n & 1); }
```

`n & 1` is `1` for odd and `0` for even, so this shifts left by 1 (doubling) only for odd
`n`. Clever, but the ternary is clearer — prefer clarity unless asked for the trick.

**Complexity:** O(1) time, O(1) space.

---

## 8. Count of Matches in Tournament — LeetCode 1688

> `n` teams. Each round, if the count is even every team is paired (`n/2` matches,
> `n/2` advance); if odd, one team gets a bye (`(n-1)/2` matches, `(n-1)/2 + 1` advance).
> Return the total matches played.

### Approach 1 — Brute force: simulate the rounds

```cpp
int numberOfMatchesBrute(int n) {
    int matches = 0;
    while (n > 1) {
        if (n % 2 == 0) { matches += n / 2;       n = n / 2; }
        else            { matches += (n - 1) / 2; n = (n - 1) / 2 + 1; }
    }
    return matches;
}
```

**Complexity:** O(log n) time, O(1) space.

### Approach 2 — Optimal: counting argument, O(1)

```cpp
int numberOfMatches(int n) {
    return n - 1;
}
```

**Why it works.** Forget the rounds entirely and count *eliminations*. Every match
eliminates **exactly one** team. The tournament ends with exactly one winner, so exactly
`n - 1` teams must be eliminated, so exactly `n - 1` matches are played — regardless of
how the byes fall.

**Dry run:** `n = 7` → answer `6`. Simulate to confirm: 7 teams → 3 matches (4 advance) →
2 matches (2 advance) → 1 match (1 advance). Total `3+2+1 = 6` ✓

**Complexity:** O(1) time, O(1) space.

This is a favourite interview problem precisely because the simulation is easy and the
insight is not. The transferable lesson: **when a process is hard to track, look for a
quantity that changes by exactly 1 each step.** You will use the same trick on tree-edge
counting and union-find in `28_DSU`.

---

## 9. Area of a circle and simple interest

```cpp
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    const double PI = 3.14159265358979323846;

    double r;
    cout << "Enter radius: ";
    cin >> r;
    cout << fixed << setprecision(4);
    cout << "Area          = " << PI * r * r << "\n";
    cout << "Circumference = " << 2 * PI * r << "\n";

    double p, rate, t;
    cout << "Enter principal, rate, time: ";
    cin >> p >> rate >> t;
    double si = (p * rate * t) / 100.0;          // 100.0, not 100
    cout << "Simple Interest = " << si << "\n";
    cout << "Amount          = " << p + si << "\n";
    return 0;
}
```

**Four deliberate choices here:**

1. **`double`, not `float`.** `01_basics/lec1.cpp` uses `float pi = 3.1415`, which gives
   ~7 significant digits and a visibly wrong area for a large radius. `double` gives 15–16.
2. **`PI` to full precision**, not `3.14`. If a problem gives you a tolerance of `1e-6`,
   `3.1415` fails it.
3. **`/ 100.0`**, not `/ 100`. If `p`, `rate`, `t` were ints, `/100` would truncate.
4. **`fixed << setprecision(4)`** from `<iomanip>` — by default `cout` prints only 6
   significant digits, so `309.65432` shows as `309.654`. Judges usually demand a
   specific number of decimals.

**Complexity:** O(1) time, O(1) space.

---

## 10. ASCII value and character arithmetic

```cpp
#include <iostream>
using namespace std;

int main() {
    char c;
    cout << "Enter a character: ";
    cin >> c;

    cout << "ASCII value : " << (int)c << "\n";
    cout << "As char     : " << c << "\n";

    // Case conversion by arithmetic ('a' - 'A' == 32)
    if (c >= 'a' && c <= 'z') cout << "Uppercase: " << char(c - 32) << "\n";
    if (c >= 'A' && c <= 'Z') cout << "Lowercase: " << char(c + 32) << "\n";

    // The idiom you will use in EVERY string problem:
    if (c >= 'a' && c <= 'z') cout << "Index 0-25: " << c - 'a' << "\n";

    // Digit character -> numeric value
    if (c >= '0' && c <= '9') cout << "Numeric value: " << c - '0' << "\n";
    return 0;
}
```

**The three facts to memorise:** `'0'` = 48, `'A'` = 65, `'a'` = 97.

**The two idioms that matter far more than the ASCII table:**

```cpp
c - 'a'    // maps 'a'..'z' to 0..25  -> index into int freq[26]
c - '0'    // maps '1' to the integer 1, not 49
```

`c - 'a'` is how every "count character frequencies" problem is written (`23_sets`,
`24_maps`, and every anagram question). `c - '0'` is how you parse digits out of a string
without `stoi`. Neither requires knowing that `'a'` is 97 — that's the point of writing
`'a'` instead of `97`.

**Portability note:** the standard does not guarantee ASCII, but it *does* guarantee
`'0'`–`'9'` are contiguous. Letters are contiguous in ASCII (which is universal in
practice) but not in EBCDIC. Write `c - 'a'`, never `c - 97`.

**Complexity:** O(1) time, O(1) space.

---

# Section 3 — Good to Know

## 11. Add Two Integers — LeetCode 2235

```cpp
int sum(int num1, int num2) {
    return num1 + num2;
}
```

**Complexity:** O(1) / O(1). Constraints are `-100 ≤ num ≤ 100`, so no overflow risk.

The only thing worth saying: if the constraints had been `|num| ≤ 2×10⁹`, the return type
would need to be `long long` and the body `(long long)num1 + num2`. **Read the constraints
before choosing types** — that habit is the actual lesson.

---

## 12. Find the Maximum Achievable Number — LeetCode 2769

> You may perform this operation at most `t` times: increase or decrease `x` by 1, and
> simultaneously increase or decrease `num` by 1. Return the maximum `x` that can become
> equal to `num`.

```cpp
int theMaximumAchievableX(int num, int t) {
    return num + 2 * t;
}
```

**Why it works.** To maximise `x`, spend every operation moving the two values toward each
other as fast as possible: decrease `x` by 1 **and** increase `num` by 1 in the same step.
The gap closes by **2** per operation, so with `t` operations you can start `2t` above
`num` and still meet it.

**Dry run:** `num = 4, t = 1` → `4 + 2 = 6`. Check: `x=6, num=4` → one op → `x=5, num=5` ✓

**Complexity:** O(1) time, O(1) space.

---

## 13. Detect whether `a + b` overflows `int`

You cannot test `if (a + b > INT_MAX)` — if it overflowed, the addition was already
undefined behaviour and the comparison is meaningless. **You must test before computing.**

```cpp
#include <iostream>
#include <climits>
using namespace std;

// Returns true if a + b would overflow a 32-bit signed int.
bool addOverflows(int a, int b) {
    if (b > 0 && a > INT_MAX - b) return true;   // positive overflow
    if (b < 0 && a < INT_MIN - b) return true;   // negative overflow
    return false;
}

// Same idea for multiplication.
bool mulOverflows(int a, int b) {
    if (a == 0 || b == 0) return false;
    long long product = (long long)a * b;        // 64-bit is wide enough to hold it
    return product > INT_MAX || product < INT_MIN;
}
```

**The key algebraic move:** rewrite `a + b > INT_MAX` as `a > INT_MAX - b`. The right-hand
side is computable without overflow (for `b > 0`, `INT_MAX - b` is in range), so the
comparison is safe.

**Where you'll meet this for real — LeetCode 7, Reverse Integer:**

```cpp
int reverse(int x) {
    int rev = 0;
    while (x != 0) {
        int digit = x % 10;
        x /= 10;
        // Check BEFORE the multiply-and-add:
        if (rev >  INT_MAX / 10 || (rev ==  INT_MAX / 10 && digit >  7)) return 0;
        if (rev <  INT_MIN / 10 || (rev ==  INT_MIN / 10 && digit < -8)) return 0;
        rev = rev * 10 + digit;
    }
    return rev;
}
```

`INT_MAX` is `2147483647`, so its last digit is `7`; `INT_MIN` is `-2147483648`, last digit
`8`. That's where the magic `7` and `-8` come from — they are not arbitrary.

**Complexity:** O(1) for the checks, O(log x) for the full reverse. Space O(1).

**The one-line summary for an interview:** *"Signed overflow is undefined behaviour, so I
check the precondition rather than the result — I rearrange the inequality so both sides
stay in range."*

---

## 14. Predict the output: pre vs post increment

```cpp
#include <iostream>
using namespace std;

int main() {
    int x = 5;
    cout << ++x << "\n";   // 6  -- increment, then read
    cout << x   << "\n";   // 6
    cout << x++ << "\n";   // 6  -- read, then increment
    cout << x   << "\n";   // 7
    return 0;
}
```

Output: `6 6 6 7` — which matches the comment in `01_basics/lec1.cpp:40`.

**The part that actually gets asked.** What does this print?

```cpp
int i = 5;
cout << i++ + ++i;      // UNDEFINED BEHAVIOUR before C++17
int x = 5;
x = x++;                // UNDEFINED BEHAVIOUR before C++17
```

**The correct answer is "undefined behaviour — there is no answer to predict."** `i` is
modified twice with no sequence point between the modifications, so the standard imposes
no requirement at all: different compilers, optimisation levels, or surrounding code can
each produce a different result, and the compiler is entitled to assume it never happens.

Candidates who confidently answer "12" have memorised one compiler's behaviour and
misunderstood the language. Candidates who say "it's UB, and I'd rewrite it as two
statements" have answered correctly.

**Complexity:** O(1).

---

## 15. Split a 3-digit number into its digits without a loop

```cpp
#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter a 3-digit number: ";
    cin >> n;

    int ones     = n % 10;
    int tens     = (n / 10) % 10;
    int hundreds = (n / 100) % 10;

    cout << "Hundreds: " << hundreds << "\n";
    cout << "Tens    : " << tens     << "\n";
    cout << "Ones    : " << ones     << "\n";
    cout << "Sum     : " << hundreds + tens + ones << "\n";
    cout << "Reversed: " << ones * 100 + tens * 10 + hundreds << "\n";
    return 0;
}
```

**The two operations that drive every digit problem in DSA:**

| Operation | Meaning |
|---|---|
| `n % 10` | the **last** digit |
| `n / 10` | the number **with the last digit removed** |

That's the whole toolkit. Chaining them (`(n/10) % 10` = "drop one digit, then take the
last") reaches any position.

**Dry run:** `n = 456` → ones `456 % 10 = 6`; tens `(456/10) % 10 = 45 % 10 = 5`;
hundreds `(456/100) % 10 = 4 % 10 = 4` ✓

**Why this matters.** The loop version you'll write in `03_Loops` —

```cpp
while (n > 0) { int d = n % 10; n /= 10; /* use d */ }
```

— is literally this, generalised. Count digits, sum of digits, reverse a number, palindrome
check, Armstrong number: all five are this loop with a different body. Understand the pair
`% 10` / `/ 10` here and those five problems become one problem.

**Complexity:** O(1) time, O(1) space.

---

# Section 4 — Extra Practice (approach only)

### Reverse Integer — LeetCode 7 (Medium)
Build the reversal with `rev = rev * 10 + n % 10` while stripping with `n /= 10`. The
problem is *entirely* the overflow guard — see solution #13 above for the exact check.
Handle negatives by letting `%` carry the sign (C++ `%` on a negative gives a negative
digit, so the reversal comes out negative automatically — no special case needed).
**O(log n) time, O(1) space.**

### Palindrome Number — LeetCode 9 (Easy)
Any negative number is `false` (the `-` makes it asymmetric). Converting to a string is
the easy answer; the interview answer reverses **only half** the digits — build `rev` from
the back while shrinking `n` from the front, stop when `rev >= n`, then compare `n == rev`
(even length) or `n == rev/10` (odd length). Reversing half can never overflow, which is
the real reason to prefer it. **O(log n) time, O(1) space.**

### Subtract the Product and Sum of Digits — LeetCode 1281 (Easy)
One pass over the digits maintaining two accumulators, `product` (init `1`) and `sum`
(init `0`). Return `product - sum`. The only trap is initialising `product` to `0`.
**O(log n) time, O(1) space.**

### Number of Steps to Reduce a Number to Zero — LeetCode 1342 (Easy)
While `n > 0`: if even `n /= 2`, else `n -= 1`, counting steps. The O(1)-ish insight:
this is `(number of bits) + (number of set bits) - 1`, because every bit costs one shift
and every set bit costs one extra subtraction. **O(log n) time, O(1) space.**

### Celsius ↔ Fahrenheit (both directions)
`F = C * 9.0/5.0 + 32` and `C = (F - 32) * 5.0/9.0`. The entire exercise is writing
`9.0/5.0` rather than `9/5` — the latter is `1`, and the bug is invisible until you check
a value. **O(1).**

### Compound interest
`A = P * pow(1 + r/100.0, t)` then `CI = A - P`. Needs `#include <cmath>`. Note `pow`
returns `double` and is not exact for integer exponents — for integer `t` a loop or
binary exponentiation (LeetCode 50, in `03_Loops`) is both faster and exact. **O(1)** with
`pow`, **O(log t)** with binary exponentiation.

### Total seconds → hours : minutes : seconds
`h = s / 3600; m = (s % 3600) / 60; sec = s % 60;`. The same `/` and `%` pair as #15, with
mixed radixes instead of base 10. Print with `setw(2) << setfill('0')` for `01:05:09`
formatting. **O(1).**

### Nature of the roots of a quadratic
Discriminant `d = b*b - 4*a*c`. `d > 0` → two distinct real roots
`(-b ± sqrt(d)) / (2*a)`; `d == 0` → one repeated root `-b / (2*a)`; `d < 0` → complex
roots `-b/(2a) ± sqrt(-d)/(2a) i`. Guard `a != 0` first (otherwise it's linear, not
quadratic) and use `double` throughout — `b*b` in `int` overflows for `b` around 50,000.
Needs conditionals, so pair it with `02_Conditional`. **O(1).**

---

# Bugs in your current code

Per the repo's convention I have not edited your `.cpp` files — these are noted here so you
can fix them yourself. Both files in this folder **fail to compile**, verified with
`g++ -fsyntax-only`.

### `01_basics/lec1.cpp` — does not compile

| Line | Problem | Fix |
|---|---|---|
| 57 | `float x = 3.1;` conflicts with `int x;` at line 15 — you cannot redeclare a name in the same scope | rename to `float fx = 3.1;` |
| 77 | `float p, r, t, si;` redeclares `r`, already declared as `float r = 5;` at line 72 | rename the radius to `radius` |
| 68 | `a = 6; b = 0; cout << a % b;` — **`% 0` is undefined behaviour**, raises SIGFPE at runtime | guard with `if (b != 0)` |
| 72 | `float pi = 3.1415` — only ~7 significant digits, and π truncated to 5 | `const double PI = 3.14159265358979323846;` |

Your modulo comments at lines 44–56 are **correct** — `7 % -6 == 1` and `-7 % 6 == -1`.
Good instinct to test the negative cases.

The general lesson: C++ has **block scope**, not function-wide "declare once per name per
lecture". Either give each experiment a unique name, or wrap each in its own `{ }` block:

```cpp
{ int x = 5;   cout << x; }   // this x dies here
{ float x = 3.1; cout << x; } // so this x is legal
```

### `01_basics/lec2.cpp` — does not compile

| Line | Problem | Fix |
|---|---|---|
| 12 | `int x;` redeclares the `int x;` from line 5 | use the existing `x`, or scope them separately |
| 16 | `int y;` redeclares the `int y = x*x;` from line 8 | same |
| 8 | `int y = x * x;` reads `x` **before** any `cin >> x` — uninitialised read, undefined behaviour | move `cin >> x;` above it |

Line 8 is the more serious one, because unlike the redeclarations it would compile fine in
a slightly different arrangement and then produce garbage silently.

### Also worth knowing

`char a = 'a';` at `lec2.cpp:22` has the comment `z=128`. `'z'` is **122**, not 128 — and
128 would not even fit in a signed `char` (range −128…127). The correct row is
`'a'`=97 … `'z'`=122.
