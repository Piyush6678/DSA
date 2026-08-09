# 05 — Functions (and Basic Recursion)

> **Striver A2Z mapping:** Step 1.1 *"Functions"* + **all of Step 1.4 "Basic Recursion"**.
> Prerequisites: `../01_basics` … `../04_pattern`.
>
> Your `lec9.cpp` already recurses (`factorial`), so basic recursion is covered here.
> The deeper material — subsequences, backtracking, recursion trees with branching — lives
> in `../10_Recursion`.

Functions are where you stop writing programs and start writing **components**. Every
LeetCode submission you will ever write is a single function with a fixed signature, so
the habits here — parameter passing, naming, decomposition — are the habits an interviewer
watches for the entire hour.

---

## 1. Anatomy

```cpp
int sum(int a, int b) {     // return type, name, parameter list
    return a + b;           // body
}
//  ^ signature: sum(int, int)
```

- **Return type** — `void` if the function returns nothing.
- **Parameters** — the variables the caller fills in.
- **Arguments** — the actual values passed at the call site.

**A function must be declared before it is used.** Two ways:

```cpp
int sum(int a, int b);          // declaration (prototype) — ends with a semicolon

int main() { cout << sum(2,3); }

int sum(int a, int b) { return a + b; }   // definition, can come later
```

Or simply define it above `main`, which is what `lec9.cpp` does. Prototypes matter when
two functions call each other, or when you split code across files.

---

## 2. Parameter passing — the single most important section

Three ways to pass, and they are not interchangeable.

### Pass by value — the function gets a **copy**

```cpp
void swapValue(int a, int b) { int t = a; a = b; b = t; }   // does NOTHING to the caller
```

`06_Pointer/pointer.cpp:10` demonstrates exactly this, and its comment `//5 6` is correct —
the caller's variables are untouched. The function swapped its own private copies and then
threw them away.

### Pass by reference — the parameter **is** the caller's variable

```cpp
void swapRef(int &a, int &b) { int t = a; a = b; b = t; }   // really swaps
swapRef(x, y);                                              // call looks normal
```

`lec9.cpp:48` (`swapNumbers(int &a, int &b)`) is this form, and it is correct.

### Pass by pointer — the function gets the **address**

```cpp
void swapPtr(int *a, int *b) { int t = *a; *a = *b; *b = t; }
swapPtr(&x, &y);                                            // caller must take addresses
```

`pointer.cpp:15` is this form. **It is labelled "pass by reference" in the comment, but it
is pass by pointer** — a distinction interviewers care about. See `../06_Pointer`.

### Which to use

| Want | Use | Why |
|---|---|---|
| Read a small value (`int`, `char`, `bool`, `double`) | **by value** | copying 4–8 bytes is free |
| Read a large object without copying | **`const T&`** | no copy, and `const` proves you won't modify it |
| Modify the caller's variable | **`T&`** | cleaner than pointers at the call site |
| Modify, and "no object" is a valid state | **`T*`** | a pointer can be `nullptr`; a reference cannot |

**`lec9.cpp:6` has a real (if minor) inefficiency:**

```cpp
void greet2(string name)          // copies the whole string on every call
void greet2(const string &name)   // no copy, and the compiler enforces read-only
```

For `int` this would be pointless. For `string`, `vector`, or any class type, `const T&` is
the default you should reach for.

---

## 3. Default arguments

```cpp
int power(int base, int exp = 2) { ... }
power(5);      // exp = 2
power(5, 3);   // exp = 3
```

**Rules:**
- Defaults must be the **trailing** parameters. `f(int a = 1, int b)` does not compile —
  there'd be no way to supply `b` alone.
- Give the default in the **declaration**, not in both declaration and definition —
  repeating it is a compile error.

`../27_graphs/bfs.cpp:10` uses `bool bi_dir = true` this way, which is idiomatic.

---

## 4. Function overloading

Same name, different **parameter list**:

```cpp
int  mini(int a, int b)          { return a < b ? a : b; }
double mini(double a, double b)  { return a < b ? a : b; }
int  mini(int a, int b, int c)   { return mini(mini(a,b), c); }
```

The compiler picks by **number and types** of arguments — this is *overload resolution*,
and it happens at compile time.

**You cannot overload on return type alone.** `int f();` and `double f();` is a compile
error, because `f();` as a statement gives the compiler nothing to choose from.

**Ambiguity is a compile error, not a coin flip.** With `f(int)` and `f(double)` defined,
the call `f('a')` is fine (`char` promotes to `int` — an exact-ish match), but
`f(int)` + `f(long)` called as `f(2.5)` is ambiguous and won't build.

---

## 5. Scope, lifetime, and `static`

| | Where visible | When created / destroyed |
|---|---|---|
| **Local** | inside its block | on entry / on exit — fresh every call |
| **Global** | whole file | program start / program end |
| **`static` local** | inside its block | **created once**, survives between calls |

```cpp
void counter() {
    static int calls = 0;   // initialised ONCE, on the first call
    ++calls;
    cout << calls;
}
counter(); counter(); counter();   // prints 1 2 3
```

**Globals are a smell.** `../27_graphs/bfs.cpp` uses global `graph`, `visited`, `v` — which
is normal for competitive programming and fine there, but in an interview prefer passing
state as parameters. Global mutable state is the first thing a reviewer flags.

---

## 6. The call stack

Every call pushes a **stack frame** holding the parameters, locals, and the return address.
Returning pops it.

```
main()            <- frame 3
  factorial(3)    <- frame 2
    factorial(2)  <- frame 1
      factorial(1)  <- base case, starts unwinding
```

The stack is **small** — typically 1 MB — so:

- **Deep recursion overflows it.** Roughly 10⁴–10⁵ frames is the practical ceiling; a
  recursion of depth 10⁶ will crash with a stack overflow, not a wrong answer.
- **Large local arrays overflow it.** `int arr[10000000];` inside a function is a crash;
  the same array as a global, or heap-allocated, is fine. (See `../06_Pointer` §heap.)

This is why "recursion depth" is a real constraint you must check against the problem's
`n`, and why an iterative rewrite is sometimes mandatory rather than stylistic.

---

## 7. Recursion

Two parts, always:

```cpp
int factorial(int n) {
    if (n <= 1) return 1;            // BASE CASE  — stops the recursion
    return n * factorial(n - 1);     // RECURSIVE CASE — moves toward the base
}
```

**Every recursion needs both, and the recursive case must make progress toward the base.**
`lec9.cpp:16` has this correct (`x == 0 || x == 1` → 1).

### The three questions to ask about any recursion

1. **What's the base case?** (When do I stop?)
2. **What's the smaller problem?** (What do I hand to the next call?)
3. **How do I combine?** (What do I do with the answer I get back?)

For factorial: stop at 1; the smaller problem is `n-1`; combine by multiplying by `n`.

### Recursion is not free

```cpp
int fib(int n) {
    if (n <= 1) return n;
    return fib(n-1) + fib(n-2);      // TWO recursive calls -> exponential
}
```

`fib(5)` calls `fib(3)` twice, `fib(2)` three times, `fib(1)` five times. The recursion
tree has ≈2ⁿ nodes, so this is **O(2ⁿ)** — `fib(50)` will not finish in your lifetime,
while the loop version is instant.

**One recursive call per level → O(n). Two → O(2ⁿ) unless you memoise.** That single
sentence is the bridge from this folder to `../26_dp`.

---

## 8. Bugs this folder exists to prevent

**An empty `main()`.** `lec9.cpp:76-84` defines nine functions and calls none of them.
Nothing here has ever run. Always exercise what you write — see the bugs section in
`solution.md`.

**`int` return types on factorial-like functions.** `lec9.cpp:16` returns `int`, so
`factorial(13)` silently returns `1932053504` instead of `6227020800`. Verified by running
it.

**Computing `n!` when you only need `nCr`.** `lec9.cpp:56` does
`factorial(n)/(factorial(r)*factorial(n-r))`, which overflows long before `nCr` does.
`combination(20,10)` returns **11** instead of 184756. Verified. See `solution.md` #7.

**Copying by value what should be `const&`.** `greet2(string name)`.

**Forgetting `#include <string>`.** `lec9.cpp` uses `string` but only includes `<iostream>`.
It compiles here because `<iostream>` happens to pull `<string>` in transitively, but that
is not guaranteed by the standard and breaks on other toolchains.

---

## Interview Q&A

### Q1. Pass by value, by reference, or by pointer — how do you choose?

**By value** copies the argument. Use it for small built-ins (`int`, `char`, `double`) where
the copy is 4–8 bytes and free, and where the function genuinely shouldn't affect the
caller.

**By reference (`T&`)** makes the parameter an alias for the caller's object — no copy, and
modifications are visible to the caller. Use it when you need to modify, or (as `const T&`)
when the object is expensive to copy.

**By pointer (`T*`)** passes an address. Functionally similar to a reference, with three
differences that decide the choice:

| | Reference | Pointer |
|---|---|---|
| Can be null | **No** | Yes |
| Can be reseated | No | Yes |
| Call site | `f(x)` — looks like by-value | `f(&x)` — visibly an address |

So: **use a reference by default; use a pointer when "no object" is a legitimate argument**
(that's what `nullptr` expresses), or when you need to change what the pointer points to.

The follow-up worth volunteering: **`const T&` is the correct default for any class type**
— `vector`, `string`, a struct. It avoids the copy *and* documents that the function won't
modify the argument, so the compiler enforces your intent. Passing a `vector<int>` by value
into a helper is one of the most common performance mistakes in interview code.

---

### Q2. How does the compiler resolve an overloaded function call, and what can't you overload on?

Resolution happens at **compile time**, based on the number and types of the arguments. The
compiler builds a set of candidates, discards those that can't accept the arguments, and
ranks the rest: exact match beats promotion (`char`→`int`), which beats standard conversion
(`int`→`double`), which beats a user-defined conversion. If two candidates tie, the call is
**ambiguous — a compile error**, not a silent pick.

**You cannot overload on return type alone.** `int f();` and `double f();` collide, because
in the statement `f();` there is no context to choose from. Return type only participates
once a candidate has already been selected.

**You also cannot overload on things that don't change the signature** — top-level `const`
on a by-value parameter (`f(int)` and `f(const int)` are the same function), or a default
argument (`f(int)` vs `f(int, int = 0)` is ambiguous when called with one argument).

The practically useful version: overloading is for *the same operation on different types*
— `mini(int,int)` and `mini(double,double)`. If the two overloads would do conceptually
different things, give them different names instead. And in modern C++ you'd often reach
for a **template** rather than writing the same body twice.

---

### Q3. What are the rules for default arguments, and where do they bite?

Defaults must be the **trailing** parameters — once one has a default, everything to its
right must too. Otherwise there'd be no syntax to skip a middle argument.

Specify the default in the **declaration** (the header, if you have one), not in both the
declaration and the definition — repeating it is a compile error.

**Where it bites:** default arguments are resolved **at the call site, at compile time**,
using the *static* type. That means a default argument on a `virtual` function is **not**
overridden polymorphically — you get the base class's default with the derived class's
body, which is almost never what anyone wants. That's a real, surprising bug, and it's why
many style guides ban defaults on virtuals outright. (Virtual dispatch itself is
`../16_oops`.)

Second bite: adding a default to an existing function silently changes what every
under-specified call does, without any of them failing to compile.

---

### Q4. What is the call stack, and what actually causes a stack overflow?

Each function call pushes a **stack frame** — the parameters, the local variables, and the
return address — onto a fixed-size region of memory. Returning pops the frame. The stack is
typically about **1 MB**, and it is *not* growable at runtime.

Two things overflow it:

1. **Recursion that is too deep.** At roughly 10s–100s of bytes per frame, the practical
   ceiling is about 10⁴–10⁵ frames. So a recursion whose depth is `n` is unsafe once
   `n` reaches ~10⁶ — and problems routinely have `n = 10⁶`. **Check the recursion depth
   against the constraints**; if it's linear in a large `n`, convert to iteration or an
   explicit stack.
2. **Large local arrays.** `int arr[10000000];` as a local is ~40 MB on a 1 MB stack — an
   immediate crash. The same array as a global, `static`, or heap-allocated (`new int[n]`)
   is fine, because those don't live on the stack.

The distinction to state clearly: a stack overflow is a **crash**, not a wrong answer. If
your recursive solution segfaults on large input but works on small input, depth is the
first thing to check — and infinite recursion (a missing or unreachable base case) is the
degenerate version of the same failure.

---

### Q5. What makes a correct recursion, and why is naive Fibonacci exponential?

A correct recursion needs a **base case** that terminates, and a **recursive case that
provably moves toward it**. Missing either gives infinite recursion and a stack overflow.

I think about it as three questions: *when do I stop*, *what smaller problem do I hand
down*, and *how do I combine the result*. For `factorial`: stop at `n <= 1`, recurse on
`n-1`, combine by multiplying by `n`.

**Naive Fibonacci is exponential because it branches.**

```cpp
int fib(int n) { return n <= 1 ? n : fib(n-1) + fib(n-2); }
```

Each call spawns **two** more, so the recursion tree roughly doubles at every level — about
2ⁿ nodes. And the subtrees overlap massively: `fib(5)` computes `fib(3)` twice and `fib(2)`
three times, all from scratch.

The rule to state: **one recursive call per level is O(n); two or more is exponential
unless the overlapping subproblems are cached.** Adding memoisation collapses `fib` to O(n)
because each distinct `n` is computed once — and that observation is exactly the doorway to
dynamic programming.

The complementary point: Fibonacci also has an O(n) iterative solution with O(1) space and
no stack risk. **Recursion is a way of thinking, not always the way of implementing.**

---

### Q6. When would you deliberately choose iteration over recursion?

Three concrete situations:

1. **Depth is large.** Anything linear in `n` with `n` up to 10⁵ or more risks a stack
   overflow. A loop has no such limit.
2. **The recursion is tail-recursive and trivially a loop.** `gcd`, linear search, and
   summing 1..n gain nothing from recursion. C++ compilers *may* optimise tail calls, but
   the standard does not require it — never rely on it.
3. **Performance in a hot path.** Each call has real overhead: pushing a frame, saving
   registers, jumping. For a tight numeric loop that overhead is measurable.

Conversely, choose recursion when the **problem itself is recursive** — trees, graphs,
divide-and-conquer, backtracking. Writing an iterative tree traversal means managing an
explicit stack, i.e. re-implementing by hand exactly what recursion gives you for free, and
the result is longer and more error-prone.

The honest summary: recursion buys clarity when the data is recursive, and costs stack
safety when the depth is proportional to input size. Decide from the constraints.

---

## Common mistakes checklist

- [ ] `main()` actually calls what you wrote
- [ ] `const T&` for `string`/`vector`/class parameters, by value for `int`-sized things
- [ ] Return type wide enough — `long long` for factorial-like growth
- [ ] `nCr` computed multiplicatively, never as three factorials
- [ ] Every recursion has a reachable base case
- [ ] Recursion depth checked against the constraints
- [ ] No large arrays as function locals
- [ ] Defaults are trailing, and declared in exactly one place
- [ ] `#include <string>` when you use `string`

---

## Next

`questions.md` → `solution.md` → `../06_Pointer`.
