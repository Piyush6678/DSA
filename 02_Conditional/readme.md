# 02 — Conditionals: Branching Logic

> **Striver A2Z mapping:** Step 1.1 *"Things to Know"* — if-else and switch-case.
> Prerequisite: `../01_basics`.

Conditionals are where correctness starts. Almost every "wrong answer" verdict you will
ever get is a **missed branch**, not a wrong algorithm — an empty input, a negative number,
a tie, a zero divisor. This folder is about learning to enumerate branches exhaustively.

---

## 1. Relational operators produce `bool`

```cpp
a == b   a != b   a < b   a > b   a <= b   a >= b
```

Each yields `true` (1) or `false` (0). C++ will happily let you use *any* integer as a
condition: **`0` is false, every other value is true.**

```cpp
if (n)        { }   // "if n is non-zero"
if (n != 0)   { }   // identical, and says what it means -- prefer this
```

`03_Loops/loops2.cpp:11` uses `if(x)` as a flag test. It works, but `if (x == 1)` or
better `bool isComposite` documents itself.

---

## 2. `if` / `else if` / `else`

```cpp
if (condition) {
    // taken when condition is true
} else if (other) {
    // taken when condition was false AND other is true
} else {
    // taken when everything above was false
}
```

**The ladder is ordered and exclusive.** Only the *first* matching branch runs. That makes
ordering part of the logic:

```cpp
if (marks >= 90) grade = 'A';
else if (marks >= 80) grade = 'B';   // reached only when marks < 90, so no upper bound needed
else if (marks >= 70) grade = 'C';
else grade = 'F';
```

Writing `else if (marks >= 80 && marks < 90)` is redundant — the `< 90` is already implied
by having fallen through the first test. Redundant conditions aren't wrong, but they hide
whether you understand the control flow.

**Always use braces.** This is the "dangling else"/`goto fail` class of bug:

```cpp
if (x > 0)
    a = 1;
    b = 2;      // NOT part of the if -- runs unconditionally. Indentation lied to you.
```

---

## 3. Logical operators and short-circuit evaluation

| Operator | Word form | Meaning |
|---|---|---|
| `&&` | `and` | true when **both** sides are true |
| `||` | `or` | true when **at least one** side is true |
| `!` | `not` | inverts |

C++ genuinely accepts the word forms — `02_Conditional/lec3.cpp:41-52` demonstrates
`and`/`or` alongside `&&`/`||`, and both compile. They are identical in meaning. Prefer
the symbols: that's what every codebase, interviewer and judge uses.

### Short-circuit evaluation — the part that matters

`&&` stops evaluating as soon as it hits a false operand. `||` stops at the first true one.
The right-hand side may **never run**.

This is not an optimisation detail, it is a **correctness tool**:

```cpp
if (i < n && arr[i] == target) { }     // safe: arr[i] is only touched when i < n
if (arr[i] == target && i < n) { }     // CRASH: out-of-bounds read happens first
```

```cpp
if (b != 0 && a % b == 0) { }          // safe: never divides by zero
if (a % b == 0 && b != 0) { }          // undefined behaviour when b == 0
```

**Put the guard first.** You will use this idiom in every linked-list problem
(`if (head != NULL && head->next != NULL)`) and every two-pointer loop.

The trap to watch for is a side effect on the right:

```cpp
if (cheapCheck() || expensiveCheckThatAlsoIncrementsCounter()) { }
```

If `cheapCheck()` returns true, the counter never increments. Never hide a side effect
behind `&&` or `||`.

---

## 4. Nested conditions

```cpp
if (a >= b) {
    if (c > a) cout << c;
    else       cout << a;
} else {
    if (c > b) cout << c;
    else       cout << b;
}
```

That's the max-of-three from `lec3.cpp:56-66`, and it is correct. It makes exactly **2
comparisons** on every path, which is provably optimal for three elements.

The flat alternative is more readable and equally correct:

```cpp
if (a >= b && a >= c)      cout << a;
else if (b >= a && b >= c) cout << b;
else                       cout << c;
```

Both are fine. The one thing to note: use `>=`, not `>`, or three equal values fall
through to the `else` and you'd better have made that branch correct.

---

## 5. The ternary (conditional) operator

```cpp
int max = (a > b) ? a : b;
```

Reads as: *if `a > b`, the expression's value is `a`, otherwise `b`.*

Unlike `if`, the ternary is an **expression** — it has a value, so it can be used in an
initialiser, an argument, or a `return`. That's its real purpose:

```cpp
return (n % 2 == 0) ? n : 2 * n;      // can't do this with if-else in one expression
const int step = fast ? 2 : 1;        // lets you keep `step` const
```

**Both branches must have compatible types.** `cond ? 1 : "one"` is a compile error —
there is no common type for `int` and `const char*`.

**Nest at most once.** This is legal and unreadable:

```cpp
x = a>b ? (a>c ? a : c) : (b>c ? b : c);   // stop here
```

Anything deeper belongs in an `if-else` ladder.

---

## 6. `switch`

```cpp
switch (expression) {
    case 1:
        cout << "one";
        break;          // without this, control FALLS THROUGH to case 2
    case 2:
        cout << "two";
        break;
    default:
        cout << "other";
}
```

### Rules that get asked

1. **The expression must be an integral or enum type** — `int`, `char`, `short`,
   `long long`, `bool`, `enum`. **Not** `float`, **not** `double`, **not** `std::string`.
   (`double` is excluded because exact equality on floating point is unreliable — see
   `../01_basics` §10.)
2. **Case labels must be compile-time constants** and must be **unique**. `case x:` where
   `x` is a variable does not compile.
3. **`break` is not optional** — omitting it means execution continues into the next case.
4. **`default` is optional** but you should almost always write it. `lec4.cpp` does, for
   both the weekday switch and the calculator — good habit.

### Fall-through is a feature, used deliberately

```cpp
switch (month) {
    case 1: case 3: case 5: case 7:
    case 8: case 10: case 12:
        days = 31; break;
    case 4: case 6: case 9: case 11:
        days = 30; break;
    case 2:
        days = isLeap ? 29 : 28; break;
    default:
        days = -1;
}
```

Stacked labels with no code between them are the idiomatic way to say "these cases share a
body". Accidental fall-through — a body followed by a missing `break` — is the bug.

---

## 7. `switch` vs `if-else if`

| | `if-else if` | `switch` |
|---|---|---|
| Condition type | any boolean expression | one integral/enum value |
| Ranges (`x > 10`) | yes | no |
| Typical cost | O(k) comparisons | O(1) via jump table, when cases are dense |
| Readability with many cases | degrades | stays flat |

For a dense set of constants (1–7, or a set of `char` operators) compilers emit a **jump
table**: they compute an index and jump directly, with no comparisons at all. For sparse
cases they may emit a binary search over the labels, which is O(log k). Either way it
beats a long `if-else` chain.

For ranges and compound conditions, `if-else if` is the only option.

---

## 8. Bugs this folder exists to prevent

**`=` instead of `==`.**

```cpp
if (n = 5) { }    // compiles! assigns 5 to n, then tests 5, which is always true
if (n == 5) { }   // what you meant
```

This is legal C++ because assignment is an expression that yields the assigned value. Turn
on `-Wall` and the compiler warns you. Some codebases write `if (5 == n)` ("Yoda
condition") so the typo becomes a compile error.

**`n % 2 == 1` for odd numbers.** `-7 % 2` is `-1`, so this silently misses every negative
odd number. Use `n % 2 != 0`. See `../01_basics` §7.

**`a > b > c`.** This compiles and is always wrong — see interview Q3 below.

**Comparing doubles with `==`.** See `../01_basics` §10.

**Unreachable branches.** `if (x > 10) … else if (x > 5) … else if (x > 20) …` — the third
branch can never run, because anything over 20 was already caught by the first test.
Order your ladder from most specific to least.

---

## Top 5 Interview Q&A

### Q1. When would you use `switch` instead of `if-else if`, and what are its limits?

Use `switch` when you are dispatching on **one integral value against a set of constants** —
menu options, operator characters, enum states, opcode handling.

**Why it's faster:** with dense case labels the compiler builds a jump table and turns the
whole construct into a single indexed jump — O(1) regardless of case count — whereas an
`if-else if` ladder evaluates conditions one by one, O(k). For sparse labels the compiler
typically emits a binary search, still O(log k).

**Its limits, which is the real question:**
- The controlling expression must be integral or enum. **No `float`, no `double`, no
  `std::string`.** Floating point is excluded because `switch` needs exact equality, which
  is unreliable on IEEE-754.
- Case labels must be **compile-time constants** and **unique** — you cannot switch on a
  range like `x > 10`.
- **Every case needs `break`**, or control falls through into the next one.

The strongest answer adds that fall-through is sometimes deliberate — stacking
`case 1: case 3: case 5:` with a shared body is the idiomatic way to group cases — so the
bug is specifically *a case with a body and no `break`*, not fall-through as such.

---

### Q2. What is short-circuit evaluation, and why does operand order matter?

`&&` evaluates its left operand first and **skips the right entirely** if the left is
false. `||` skips the right if the left is true.

The reason this is a correctness feature rather than a micro-optimisation is that it lets
you write a **guard** and the thing it guards in one expression:

```cpp
if (i < n && arr[i] == target)      // safe
if (arr[i] == target && i < n)      // out-of-bounds read — UB, may or may not crash
```

```cpp
if (node != NULL && node->val == x) // safe
if (b != 0 && a % b == 0)           // never divides by zero
```

Swap the operands in any of those and the program is broken. So the ordering is part of
the logic, not a style preference.

The follow-up: **never put a side effect on the right-hand side.** In
`if (cheap() || expensive_that_mutates())`, the mutation silently doesn't happen whenever
`cheap()` is true. This produces bugs that only appear when input data changes.

Worth mentioning if pushed: `&` and `|` are the *bitwise* operators and do **not**
short-circuit — both sides always evaluate. Using `&` where you meant `&&` removes your
guard.

---

### Q3. Why does `if (a > b > c)` compile, and what does it actually do?

It compiles because `>` is left-associative and yields a `bool`, and `bool` implicitly
converts to `int`. So `a > b > c` parses as `(a > b) > c`:

1. `a > b` evaluates to `true` or `false`
2. that converts to `1` or `0`
3. the result is compared against `c`

So `if (5 > 3 > 1)` computes `(5>3)` → `true` → `1`, then `1 > 1` → **false**. The
mathematically obvious statement evaluates to false.

The correct form is an explicit conjunction:

```cpp
if (a > b && b > c)
```

This is worth knowing beyond the trivia: it's the same class of bug as `=` vs `==` —
C++'s permissive implicit conversions let a meaningless expression compile. The defence is
the same: build with `-Wall -Wextra` (which warns here), and be suspicious of any condition
that reads like maths notation.

---

### Q4. What's wrong with `if (n = 5)`, and how do you prevent it?

`=` is assignment, not comparison. `n = 5` **assigns** 5 to `n` and the expression's value
is the assigned value, `5`. Since `5` is non-zero, the condition is always true — and `n`
has been silently clobbered.

It compiles because in C++ assignment is an expression, and any non-zero arithmetic value
is a valid condition. That design is deliberate — it's what makes `while ((c = getchar()) != EOF)`
possible — but it means the typo is legal code.

Three defences, in order of practical value:

1. **Compile with warnings on.** `-Wall` emits *"suggest parentheses around assignment used
   as truth value"*. Treat warnings as errors in real projects.
2. **Yoda conditions** — write `if (5 == n)`. If you slip and type `=`, `5 = n` is a
   compile error because you cannot assign to a literal. Ugly, but some codebases mandate
   it.
3. **`const`-qualify** anything that shouldn't change; the assignment then fails to
   compile.

---

### Q5. What are the rules of the ternary operator, and when should you avoid it?

`condition ? valueIfTrue : valueIfFalse` is an **expression**, which is its whole reason to
exist: it produces a value, so it can appear where a statement cannot — in an initialiser,
a function argument, or a `return`.

```cpp
const int step = fast ? 2 : 1;               // lets `step` stay const
return (n % 2 == 0) ? n : 2 * n;             // single-expression return
```

**The rules that get tested:**
- The second and third operands must have a **common type**. `cond ? 1 : "one"` does not
  compile.
- If they're `int` and `double`, the result is `double` — the usual arithmetic conversions
  apply, so `cond ? 1 : 2.0` yields `1.0`, not `1`.
- Only **one** of the two branches is evaluated, exactly like `if-else` — so
  `b != 0 ? a/b : 0` is safe.
- It has very low precedence, below assignment's operands — parenthesise when embedding it.

**When to avoid it:** anything nested more than one level deep, and anything where the
branches have side effects rather than values. `a ? f() : g()` used purely for its effects
should be an `if`. The ternary's job is to *produce a value*; when it isn't producing one,
it's just a cryptic `if`.

---

## Common mistakes checklist

- [ ] Braces on every `if`, even one-liners
- [ ] `==` not `=` in conditions; build with `-Wall`
- [ ] `a > b && b > c`, never `a > b > c`
- [ ] Guard on the **left** of `&&` (`i < n && arr[i]…`)
- [ ] `n % 2 != 0` for odd, not `n % 2 == 1`
- [ ] `break` in every `switch` case that has a body
- [ ] `default` branch present
- [ ] Divisor checked non-zero before `/` or `%`
- [ ] `else if` ladder ordered most-specific first — no unreachable branches
- [ ] Every branch actually prints/returns something (the `else` is not optional in a
      function that must return)

---

## Next

`questions.md` → `solution.md` → `../03_Loops`.
