# 01 — Basics: C++ Fundamentals

> **Striver A2Z mapping:** Step 1.1 *"Things to Know in C++/Java/Python or any language"* —
> user input/output, data types, and operators.
> Everything here is a prerequisite for every later folder. Interviewers never ask these
> directly, but they *do* silently reject you for getting them wrong inside a real problem.

---

## 1. Anatomy of a C++ program

```cpp
#include <iostream>     // pulls in cin / cout declarations
using namespace std;    // lets you write cout instead of std::cout

int main() {            // execution starts here; returns int to the OS
    cout << "hello";
    return 0;           // 0 = success. Optional in main (implicit), keep it for clarity.
}
```

`using namespace std;` is fine for practice and competitive programming. In production
C++ it is discouraged because it dumps ~1000 names into the global scope and causes
ambiguity (the classic collision is your own `count` variable vs `std::count`). This repo
uses it everywhere — keep doing so here, but know why it's frowned on if asked.

---

## 2. Output — `cout`

```cpp
cout << "Age: " << 21 << endl;   // chaining: << returns cout, so you can keep going
```

**Escape sequences:** `\n` newline, `\t` tab, `\\` backslash, `\"` quote.

**`endl` vs `\n`** — a real interview question:

| | Effect |
|---|---|
| `\n` | writes a newline character |
| `endl` | writes a newline **and flushes** the output buffer |

Flushing forces an actual write to the console every time. In a loop printing 10⁶ lines,
`endl` can be **several times slower** than `\n`. Use `\n` by default; use `endl` only
when you genuinely need the output to appear immediately (e.g. interactive problems).

**Quoting trap.** These are different:

```cpp
cout << 4 + 3;      // prints 7   -> expression is evaluated
cout << "4+3";      // prints 4+3 -> it's a string literal, not maths
```

A variant of this bites in `02_Conditional/lec3.cpp:24`, where
`cout << "absolute value of n is n"` prints the letter `n` instead of the value. To print
a variable you must break out of the quotes: `cout << "abs is " << n;`

---

## 3. Input — `cin`

```cpp
int x;
cout << "Enter a number: ";   // prompt FIRST
cin  >> x;                    // then read
```

**Order matters.** `03_Loops/loops2.cpp` reads before prompting (`cin >> n;` then
`cout << "enter the number";`), so the user stares at a blank screen. Always prompt, then
read.

`cin >> x` **skips leading whitespace** (spaces, tabs, newlines) and stops at the next
whitespace. So `cin >> a >> b >> c;` happily reads `3 7 9` on one line *or* on three lines.

`cin >> ch` for a `char` therefore **cannot read a space**. To read a whole line including
spaces you need `getline(cin, s)` — covered in `09_Strings`.

**Uninitialized reads are undefined behaviour.** `01_basics/lec2.cpp:5-8` declares `int x;`
and immediately computes `int y = x*x;` without ever calling `cin >> x`. `x` holds
whatever garbage was on the stack — the program is not "wrong sometimes", it is formally
undefined.

---

## 4. Data types, sizes and ranges

Sizes on this repo's toolchain — **measured, not assumed**. `g++ -dumpmachine` reports
`mingw32`, so this is a **32-bit** compiler (the ILP32 model) even though Windows itself is
64-bit:

| Type | Bytes | Range | Use when |
|---|---|---|---|
| `bool` | 1 | `true` / `false` | flags |
| `char` | 1 | −128 … 127 | single characters, ASCII maths |
| `short` | 2 | −32,768 … 32,767 | almost never |
| `int` | 4 | ≈ −2.1×10⁹ … 2.1×10⁹ | the default integer |
| `long` | **4 here** | same as `int` | **avoid** — not portable |
| `long long` | 8 | ≈ ±9.2×10¹⁸ | anything that might exceed 2×10⁹ |
| `float` | 4 | ~7 significant digits | almost never |
| `double` | 8 | ~15–16 significant digits | the default real number |
| `long double` | 12 | extended | rarely |
| **any pointer** | **4** | — | see `../06_Pointer` |

**`long` is a trap.** Here it is 4 bytes — identical to `int`, with `LONG_MAX` equal to
`INT_MAX`. On 64-bit Linux (LP64) it is 8. Code that uses `long` for "a big number"
compiles on both and silently overflows on one. **Use `long long`, never `long`.**

**Pointers are 4 bytes on this toolchain, not 8.** If an interviewer asks "what does
`sizeof(int*)` return", the honest answer is *"it depends on the target — 4 on a 32-bit
build, 8 on a 64-bit build"*, and the important part is that **every pointer type is the
same size**, because a pointer is just an address. Verify on your own machine rather than
reciting a number: `cout << sizeof(int*);`

Contest rule of thumb:
- Value fits in **10⁹** → `int`
- Value fits in **10¹⁸** → `long long`
- Beyond that → you need a different algorithm (or big-integer arithmetic)

**`char` is a number.** `'a'` *is* `97`. This makes character maths work:

```cpp
char c = 'a';
cout << c;          // a
cout << (int)c;     // 97
cout << char(3+64); // C     -- 'A' is 65, so 65+2 = 'C'
cout << c - 'a';    // 0     -- the classic 0..25 index for lowercase letters
```

Key ASCII values worth memorising: `'0'` = 48, `'A'` = 65, `'a'` = 97.
Note `'a' - 'A' == 32`, which is why case conversion is `c + 32` / `c - 32`.

---

## 5. Type conversion — the `5/2` trap

**Integer division truncates toward zero.** It does not round.

```cpp
cout << 5 / 2;      // 2    both operands int   -> integer division
cout << 5.0 / 2;    // 2.5  one operand double  -> the int is promoted, real division
cout << 5 / 2.0;    // 2.5  same
cout << -5 / 2;     // -2   truncates toward zero, NOT -3
```

The rule (*usual arithmetic conversions*): if **either** operand is floating point, the
other is promoted and the result is floating point. If **both** are integers, you get
integer division — even if you assign the result to a `double`:

```cpp
double avg = (a + b + c) / 3;      // BUG: integer division happens first
double avg = (a + b + c) / 3.0;    // correct
double avg = (double)(a + b + c) / 3;  // also correct — explicit cast
```

This is the single most common silent bug in beginner C++ and it shows up in real
interview code (computing averages, midpoints, percentages).

---

## 6. Operators

**Arithmetic:** `+ - * / %`. `%` is **integer only** — `5.0 % 2` is a compile error.

**Precedence:** `*`, `/`, `%` bind tighter than `+`, `-`. Unary `-` and `++`/`--` bind
tightest. When in doubt, use parentheses — no interviewer has ever docked a candidate for
clear parentheses.

**Compound assignment:** `x += 3` is `x = x + 3`. Likewise `-= *= /= %=`.

**Comparison:** `== != < > <= >=` — these produce a `bool`.

---

## 7. Modulo semantics — including negatives and zero

Since C++11 the standard guarantees:

> `(a / b) * b + (a % b) == a`, with `/` truncating toward zero.

The consequence: **`a % b` takes the sign of `a`** (the dividend), and the sign of `b` is
irrelevant.

```cpp
 7 %  6  ==  1
 7 % -6  ==  1     // sign of dividend (+)
-7 %  6  == -1     // sign of dividend (-)
-7 % -6  == -1
 3 %  6  ==  3     // if |a| < |b|, result is a
```

Two consequences that cost people offers:

1. **`n % 2 == 1` is WRONG for odd negatives.** `-7 % 2` is `-1`, not `1`. Always test
   `n % 2 != 0`, or `abs(n % 2) == 1`.
2. **`a % 0` is undefined behaviour** — not an exception, not zero. On x86 it raises
   SIGFPE and kills the process. `01_basics/lec1.cpp:68` does exactly this
   (`a=6; b=0; cout << a%b;`). Guard the divisor before dividing, always.

To force a non-negative result (needed constantly in hashing and circular arrays):

```cpp
int idx = ((i % n) + n) % n;
```

---

## 8. Increment and decrement

```cpp
int x = 5;
cout << ++x;   // 6  -> increment first, then use the value  (pre-increment)
cout << x;     // 6
cout << x++;   // 6  -> use the value first, then increment  (post-increment)
cout << x;     // 7
```

**Never write `i++ + ++i` or `x = x++`.** Before C++17 these are *undefined behaviour* —
the compiler is free to produce any answer. Interviewers ask about this to see whether you
know "the compiler will do something reasonable" is not a guarantee.

**Prefer `++i` over `i++` in loops.** For `int` they compile to identical code, but for
iterators and heavy objects `i++` must copy the old value to return it. `++i` is a free
habit that never costs you.

---

## 9. Overflow — the bug that fails real interviews

`int` silently wraps around when it exceeds ≈ 2.1×10⁹. There is no error.

```cpp
int a = 2000000000, b = 2000000000;
cout << a + b;                       // -294967296   (wrapped)
cout << (long long)a + b;            // 4000000000   (cast BEFORE adding)
```

The cast must happen **before** the arithmetic. `(long long)(a + b)` is too late — the
overflow already happened in `int`.

**The famous one — binary search midpoint:**

```cpp
int mid = (low + high) / 2;          // overflows when low+high > INT_MAX
int mid = low + (high - low) / 2;    // correct, and identical result
```

This bug lived in Java's standard library for nine years. Write the second form from day
one; you will need it in `11_linearAndBinarySearch`.

Where overflow shows up early: `13!` exceeds `int`, `21!` exceeds `long long`
(`03_Loops/imp.cpp` correctly uses `long long factorial`, which is the right instinct).

---

## 10. Floating point is approximate

```cpp
cout << (0.1 + 0.2 == 0.3);   // 0  -- false!
```

`0.1` is not representable in binary, exactly like `1/3` is not representable in decimal.
`0.1 + 0.2` is actually `0.30000000000000004`.

**Never compare floats with `==`.** Compare against a tolerance:

```cpp
const double EPS = 1e-9;
if (fabs(a - b) < EPS) { /* equal enough */ }
```

**Prefer `double` over `float`.** `float` carries only ~7 significant digits, which is not
enough for most problems, and modern CPUs are no faster with it.

---

## Top 5 Interview Q&A

### Q1. Why does `5/2` print `2` but `5.0/2` print `2.5`?

Because `/` is overloaded on operand types. With two `int` operands C++ performs *integer
division*, which truncates the fractional part toward zero. With one operand a `double`,
the *usual arithmetic conversions* promote the `int` to `double` first and real division
is performed.

The interview follow-up is always the average bug: `double avg = (a+b)/2;` still truncates,
because the division happens in `int` **before** the assignment converts the result.
The fixes are `/2.0` or `(double)(a+b)/2`.

Also worth stating: truncation is toward **zero**, not toward negative infinity, so
`-5/2 == -2` (not `-3`). That distinction matters when you index arrays with computed
values.

---

### Q2. When does `int` overflow, and how do you write code that can't?

`int` is 32-bit two's complement, so it holds up to `2^31 - 1 ≈ 2.14×10⁹`. Exceeding it
wraps silently — signed overflow is technically undefined behaviour, so the compiler may
even optimise on the assumption it never happens.

Three defensive habits:

1. **Size from the constraints.** If the problem says `n ≤ 10^5` and `a[i] ≤ 10^9`, then a
   sum can reach `10^14` — use `long long` for the accumulator even though the elements
   fit in `int`.
2. **Cast before you compute:** `(long long)a * b`, not `(long long)(a * b)`.
3. **Never compute `(low + high) / 2`.** Use `low + (high - low) / 2`.

The strongest answer mentions that the check inside LeetCode 7 (*Reverse Integer*) —
`if (rev > INT_MAX/10) return 0;` — tests for overflow *before* it occurs, because testing
after is meaningless when the behaviour is undefined.

---

### Q3. What does `%` do with negative numbers, and what about `% 0`?

Since C++11, integer division truncates toward zero and `%` is defined so that
`(a/b)*b + a%b == a`. The result therefore **carries the sign of the dividend**:
`-7 % 2 == -1`, `7 % -2 == 1`.

Two practical consequences:

- **`n % 2 == 1` is a bug** for negative odd numbers. Use `n % 2 != 0`.
- **Wrapping an index needs a double modulo:** `((i % n) + n) % n`, because a plain
  `i % n` can be negative.

`a % 0` (and `a / 0`) is **undefined behaviour**, not an exception. On x86 it raises SIGFPE
and terminates the process. Always guard the divisor — the calculator in
`02_Conditional/lec4.cpp` does this correctly for `/`.

---

### Q4. What's the difference between `++x` and `x++`, and which should you use?

`++x` increments then yields the **new** value; `x++` yields a **copy of the old** value
then increments. As standalone statements they are identical.

Prefer `++x`:
- For built-in types, compilers emit identical code — it costs nothing.
- For iterators and user-defined types, `operator++(int)` (post) must construct and return
  a copy of the old object. In a tight loop over a container that copy is real work.

The follow-up is usually `i++ + ++i` or `x = x++`. The correct answer is that these are
**undefined behaviour** before C++17 (unsequenced modification and read of the same
object) — not "implementation-defined", not "compiler-dependent in a documented way".
There is no right answer to predict; the correct move is to refuse to write it.

---

### Q5. Why is `0.1 + 0.2 != 0.3`, and how do you compare floating-point numbers?

`float` and `double` are IEEE-754 binary formats. A finite binary fraction can only
represent numbers of the form `k/2^m`, and `0.1` is not one — just as `1/3` has no finite
decimal expansion. `0.1` is stored as the nearest representable double, and the tiny errors
accumulate: `0.1 + 0.2` evaluates to `0.30000000000000004`.

Never use `==` on floating-point values. Compare with an epsilon:

```cpp
if (fabs(a - b) < 1e-9) { /* equal */ }
```

Strong follow-ups to volunteer:
- **Use `double`, not `float`** — 15–16 significant digits vs ~7, at no speed cost.
- **Never use floating point for money** — use integer cents.
- **Avoid floating point when integers will do.** For "is `x` a perfect square", prefer
  an integer binary search over `sqrt(x)`, because `sqrt` on a large `long long` can be
  off by one after rounding.

---

## Common mistakes checklist

- [ ] Prompt before `cin`, not after
- [ ] Every variable initialised before it is read
- [ ] `/2.0` (or a cast) whenever you want a real average
- [ ] `long long` chosen from the constraints, and cast **before** multiplying
- [ ] `low + (high-low)/2`, never `(low+high)/2`
- [ ] `n % 2 != 0` for odd, never `n % 2 == 1`
- [ ] Divisor checked non-zero before `/` or `%`
- [ ] `fabs(a-b) < EPS` instead of `a == b` for doubles
- [ ] `\n` instead of `endl` inside loops

---

## Next

`questions.md` — 15 ranked problems + extra practice.
`solution.md` — worked solutions with approach progression and complexity.
Then move to `../02_Conditional`.
