# 03 — Loops & Basic Maths

> **Striver A2Z mapping:** Step 1.1 (for / while loops) **and all of Step 1.3 "Basic Maths"** —
> count digits, reverse a number, palindrome, GCD, Armstrong, print divisors, check prime.
> Prerequisites: `../01_basics`, `../02_Conditional`.

This is the first folder where **time complexity becomes real**. Up to now every program
was O(1). From here on, the difference between a correct solution and an accepted one is
usually the difference between O(n) and O(√n) or O(log n).

---

## 1. The three loops

### `for` — when you know the bounds

```cpp
for (int i = 1; i <= n; ++i) {
    // body
}
//   ^init    ^condition  ^update
```

Order of execution: `init` once → `condition` → body → `update` → `condition` → …
The condition is checked **before** the first iteration, so a `for` loop can run zero times.

`i` declared in the init is scoped to the loop — it does not exist afterwards. That's a
feature: it prevents the accidental reuse that broke `01_basics/lec1.cpp`.

### `while` — when you don't know the bounds

```cpp
while (n > 0) {
    int digit = n % 10;
    n /= 10;
}
```

Use `while` when the number of iterations depends on work done inside the body. Every digit
problem in this folder is a `while`, because "how many digits does `n` have" is exactly
what the loop is computing.

### `do-while` — when the body must run at least once

```cpp
int n;
do {
    cout << "Enter a positive number: ";
    cin >> n;
} while (n <= 0);      // note the semicolon
```

The condition is checked **after** the body, so it always executes at least once. Its main
uses are input validation and menu loops.

**Where `do-while` is genuinely required:** counting the digits of `0`.

```cpp
int count = 0;
while (n > 0) { ++count; n /= 10; }     // n = 0 -> count = 0  ✗ wrong

int count = 0;
do { ++count; n /= 10; } while (n > 0); // n = 0 -> count = 1  ✓ correct
```

`03_Loops/loops2.cpp:25-31` handles this with an explicit `if (a) … else cout << "1";`,
which works. `do-while` expresses the same thing without the special case.

---

## 2. `break` and `continue`

- **`break`** — leave the loop immediately.
- **`continue`** — skip the rest of this iteration, go to the update and test again.

```cpp
for (int i = 2; i * i <= n; ++i) {
    if (n % i == 0) { isPrime = false; break; }   // found a factor, stop looking
}
```

**`break` is not just an optimisation here.** Without it you keep dividing after the answer
is known — and in a search loop, later iterations can overwrite the result you already
found.

`03_Loops/loops2.cpp:8-10` sets a flag but never breaks, so it always performs all `n/2`
iterations even after finding a factor on the first one.

**`continue` in a `while` loop is dangerous:**

```cpp
while (i < n) {
    if (skip(i)) continue;    // INFINITE LOOP — ++i never runs
    ++i;
}
```

In a `for` loop the update clause still runs on `continue`, so this trap is `while`-specific.

**Breaking out of nested loops.** `break` exits only the *innermost* loop. Three ways out:

```cpp
// 1. Flag (works, but noisy)
bool found = false;
for (int i = 0; i < n && !found; ++i)
    for (int j = 0; j < m; ++j)
        if (grid[i][j] == target) { found = true; break; }

// 2. Extract into a function and return  <-- cleanest, prefer this
// 3. goto done;  <-- legal, and the one case where goto is defensible
```

---

## 3. Nested loops and time complexity

```cpp
for (int i = 1; i <= n; ++i)         // runs n times
    for (int j = 1; j <= n; ++j)     // runs n times per outer iteration
        cout << "*";                 // executed n × n times  ->  O(n²)
```

**How to count:** multiply the iteration counts of nested loops, add the counts of
sequential loops.

```cpp
for (i = 0; i < n; ++i) { }          // n
for (i = 0; i < n; ++i) { }          // + n      = O(n),  not O(n²)

for (i = 0; i < n; ++i)              // n
    for (j = 0; j < i; ++j) { }      // × avg n/2 = n(n-1)/2 = O(n²)
```

A triangular nested loop is still O(n²) — constants don't change the class.

**The complexities you will meet in this folder:**

| Pattern | Complexity | Example |
|---|---|---|
| `for (i = 0; i < n; ++i)` | O(n) | count primes naively |
| `for (i = 2; i*i <= n; ++i)` | **O(√n)** | prime check, print divisors |
| `while (n > 0) n /= 10;` | **O(log₁₀ n)** | digit loops |
| `while (n > 1) n /= 2;` | **O(log₂ n)** | binary exponentiation |
| nested `i`, `j` over n | O(n²) | patterns (`../04_pattern`) |

**Dividing is logarithmic.** Any loop where the variable is *divided* rather than
*decremented* runs a logarithmic number of times. For `int`, `O(log n)` is at most 31
iterations — effectively free. Recognising "this loop divides, so it's log" is the single
most useful complexity instinct at this level.

---

## 4. The digit-extraction loop — one pattern, five problems

```cpp
while (n > 0) {
    int digit = n % 10;    // last digit
    n /= 10;               // remove last digit
    // ... do something with `digit`
}
```

This is `../01_basics` #15 generalised. Five of Striver's Basic Maths problems are this
loop with a different body:

| Problem | Body |
|---|---|
| Count digits | `++count;` |
| Sum of digits | `sum += digit;` |
| Reverse a number | `rev = rev * 10 + digit;` |
| Palindrome | reverse, then compare to the original |
| Armstrong | `sum += digit^k;` where `k` = digit count |

Learn the frame once and you've learned five problems. **Every one of them needs the same
two edge cases: `n == 0` and `n < 0`.**

---

## 5. Overflow inside loops

Loops multiply, and multiplication overflows fast:

```cpp
int factorial = 1;
for (int i = 1; i <= n; ++i) factorial *= i;
```

| Type | Overflows at |
|---|---|
| `int` | **13!** = 6,227,020,800 |
| `long long` | **21!** |

`03_Loops/imp.cpp:14` correctly declares `long long factorial` — right instinct. But note
that even `long long` only buys you up to `n = 20`. Beyond that the problem is asking for
something else: the answer modulo 10⁹+7, or the number of trailing zeros (LeetCode 172),
or big-integer arithmetic.

**The reversal loop overflows too:**

```cpp
rev = rev * 10 + digit;      // for a 10-digit input, this can exceed INT_MAX
```

You cannot check afterwards — signed overflow is undefined behaviour. Check **before**:
see `../01_basics/solution.md` #13, and problem #2 in this folder.

---

## 6. Bugs this folder exists to prevent

**Prompting after reading.** `loops2.cpp` does `cin >> n;` *then*
`cout << "enter the number";` in all four blocks. The user faces a blank screen and the
prompt appears after they've already typed.

**Off-by-one.** `i <= n` vs `i < n` is the difference between n and n−1 iterations. Decide
deliberately: use `i < n` for 0-indexed arrays, `i <= n` for 1-indexed maths.

**Modifying the loop variable inside the body.** Legal, and almost always a bug — it makes
the iteration count unpredictable.

**Destroying the input you still need.** Digit loops consume `n`. Save a copy first:

```cpp
int original = n;
while (n > 0) { ... }
if (rev == original) { ... }     // `n` is now 0 — you'd have compared against the wrong thing
```

`loops2.cpp:24` does exactly this correctly (`int count = 0, a = n;`).

**Infinite loops.** `while (n != 0) n -= 2;` never terminates for odd `n`. Prefer `<=`/`>=`
conditions over `!=` when the step might skip the target.

---

## Top 5 Interview Q&A

### Q1. `for` vs `while` vs `do-while` — how do you choose?

They are interchangeable in power; the choice communicates intent.

- **`for`** when the iteration count is known up front from the bounds — traversing
  `0..n-1`, walking an array. Init, condition and update sit on one line, so a reader sees
  the whole contract at a glance.
- **`while`** when termination depends on work done in the body — `while (n > 0) n /= 10;`
  runs however many times it takes. Using a `for` here would be contorted.
- **`do-while`** when the body **must** run at least once: input validation, menu-driven
  programs, and any "process, then check if we're done" loop.

The concrete example worth giving: **counting the digits of 0**. A `while` loop tests
`n > 0` first, exits immediately, and returns 0 — wrong, since "0" has one digit. A
`do-while` runs the body once and returns 1. That's not style, that's correctness.

One more practical difference: a variable declared in a `for` init is scoped to the loop,
so it can't leak or collide. That alone is a reason to prefer `for` when either works.

---

### Q2. Why is the prime check `i * i <= n` and not `i <= n/2`, and what are the edge cases?

**The complexity.** If `n` has a divisor `d > √n`, then `n / d` is a divisor smaller than
`√n`. So divisors come in pairs straddling `√n` — and if no divisor exists at or below
`√n`, none exists at all. Checking up to `√n` is therefore sufficient.

That takes the loop from **O(n/2) to O(√n)**. For `n = 10⁹` that's 500 million iterations
versus about 31,623 — the difference between a timeout and an instant answer.

**Write `i * i <= n`, not `i <= sqrt(n)`,** for two reasons: `sqrt` returns a `double` and
can be off by one after rounding for large `n`, and calling it inside the condition
re-evaluates it every iteration.

**The overflow caveat** worth volunteering: `i * i` itself overflows when `n` approaches
`INT_MAX` (at `i = 46341`, `i*i` exceeds `INT_MAX`). For large `n`, write `i <= n / i`, or
make `i` a `long long`.

**The edge cases — this is what the question is really testing:**

| n | Correct answer | Why naive code fails |
|---|---|---|
| `n < 2` | not prime | the loop body never runs, so a flag-based check reports "prime" |
| `n = 2` | **prime** | it's the only even prime; `i*i <= 2` is false immediately, correctly leaving it prime |
| `n = 4` | not prime | smallest composite — catches an `i*i < n` (strict) off-by-one |

`03_Loops/loops2.cpp` has exactly this bug: with `for (i = 2; i <= n/2; ++i)`, an input of
`0` or `1` gives `n/2 = 0`, the loop never runs, the flag stays 0, and it prints *"Given
number is prime"*. **You must special-case `n < 2` before the loop.**

---

### Q3. How do you work out the time complexity of nested loops?

Multiply the iteration counts of nested loops; add those of sequential loops. Then drop
constants and lower-order terms.

```cpp
for (i = 0; i < n; ++i)
    for (j = 0; j < m; ++j)  ...      // O(n·m)

for (i = 0; i < n; ++i)
    for (j = i; j < n; ++j)  ...      // n + (n-1) + ... + 1 = n(n+1)/2 = O(n²)

for (i = 1; i < n; i *= 2)  ...       // O(log n) — i DOUBLES, it doesn't increment
```

**The rule that catches people out: look at how the loop variable changes, not at the
bound.** `for (i = 1; i < n; i *= 2)` looks like it goes to `n` but runs only log₂n times.
Conversely `for (i = 0; i < n; ++i) { j = 0; while (j < n) j++; }` looks like two separate
loops but is O(n²).

The best answer names the two sub-cases of the triangular loop — that `n(n+1)/2` is still
O(n²) because constant factors don't change the class, and that this is *exactly* why an
O(n²) algorithm on n = 10⁵ is too slow (10¹⁰ operations) while O(n log n) is fine
(~1.7×10⁶).

---

### Q4. `break` vs `continue`, and how do you break out of nested loops?

`break` exits the enclosing loop entirely. `continue` abandons the current iteration and
jumps to the loop's update/condition.

Two things make this more than a definition question:

**1. `continue` in a `while` loop can hang.** In `while (i < n) { if (...) continue; ++i; }`
the increment is skipped, so the loop spins forever. In a `for` loop, `continue` still runs
the update clause, so the same code is safe. This asymmetry is a real bug source.

**2. `break` only exits one level.** For nested loops there are three options, in order of
preference:

- **Extract the loops into a function and `return`.** Cleanest — the return value carries
  the answer out, no flags to maintain.
- **A flag tested in the outer condition** (`for (i = 0; i < n && !found; ++i)`). Works,
  but adds state.
- **`goto done;`** — jumping *forward* out of nested loops to a label is the one widely
  accepted use of `goto` in C++, and the Linux kernel uses it heavily for cleanup paths.
  Say it exists and that you'd prefer the function extraction.

C++ has no labelled `break` (Java does), which is why this question is asked at all.

---

### Q5. Where does integer overflow bite inside loops, and how do you handle it?

Anywhere a loop **multiplies**, because products grow exponentially while sums grow
linearly.

**Factorial** is the canonical case: `13!` overflows `int`, `21!` overflows `long long`. So
`long long` buys you only 8 more inputs — which tells you that any problem asking for
`n!` with `n > 20` is not actually asking for the factorial. It wants the answer **modulo
10⁹+7**, or the **number of trailing zeros** (LeetCode 172, which is
`n/5 + n/25 + n/125 + …`, no factorial computed at all), or big-integer arithmetic.

Recognising that reframing is the point of the question.

**Reversing a number** is the second case: `rev = rev * 10 + digit` overflows on a 10-digit
input. Since signed overflow is undefined behaviour, you cannot detect it after the fact —
`if (rev < 0)` is not a valid test. You must check the precondition:

```cpp
if (rev > INT_MAX / 10 || (rev == INT_MAX / 10 && digit > 7)) return 0;
```

**The three habits:**
1. Size your accumulator from the constraints, not from the element type — summing 10⁵
   values of 10⁹ each needs `long long` even though every element fits in `int`.
2. Cast **before** the arithmetic: `(long long)a * b`.
3. When the true answer doesn't fit any type, the problem wants something else — modular
   arithmetic, a count, or a closed form.

---

## Common mistakes checklist

- [ ] Prompt **before** `cin`
- [ ] `n < 2` handled before any prime loop
- [ ] `n == 0` and `n < 0` handled in every digit loop
- [ ] Original value saved before a loop consumes it
- [ ] `break` after the answer is found in a search loop
- [ ] `i * i <= n` (not `i <= sqrt(n)`, not `i <= n/2`)
- [ ] `long long` for any accumulator that multiplies
- [ ] Overflow checked **before** `rev = rev*10 + digit`
- [ ] `continue` in a `while` still reaches the increment
- [ ] Loop variable not modified inside the body

---

## Next

`questions.md` → `solution.md` → `../04_pattern`.
