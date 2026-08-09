# 05 — Functions & Basic Recursion: Solutions

Full C++14 for Sections 1–3, approach-only for Section 4.
**Every code sample below was compiled and executed before publication**; the dry runs and
claimed outputs are actual program output, not predictions.

---

# Section 1 — Must Do

## 1. Swap two numbers — three ways

```cpp
#include <iostream>
using namespace std;

void swapValue(int a, int b)   { int t = a; a = b; b = t; }   // does NOTHING to the caller
void swapRef(int &a, int &b)   { int t = a; a = b; b = t; }   // works
void swapPtr(int *a, int *b)   { int t = *a; *a = *b; *b = t; } // works

int main() {
    int x = 5, y = 6;
    swapValue(x, y);   cout << x << " " << y << "\n";   // 5 6  <- unchanged
    swapRef(x, y);     cout << x << " " << y << "\n";   // 6 5
    swapPtr(&x, &y);   cout << x << " " << y << "\n";   // 5 6  <- swapped back
    return 0;
}
```

**Verified output:** `5 6` / `6 5` / `5 6` — exactly as annotated.

**Why `swapValue` fails.** `a` and `b` are *copies* living in the function's own stack frame.
It genuinely swaps them, then the frame is destroyed on return and the work is discarded.
The caller's `x` and `y` were never involved. `06_Pointer/pointer.cpp:10` demonstrates this
and its comment `//5 6` is correct.

**Reference vs pointer — the practical differences:**

| | `swapRef(x, y)` | `swapPtr(&x, &y)` |
|---|---|---|
| Call site | looks like pass-by-value | visibly takes addresses |
| Can be null | **no** — must bind to a real object | yes, so needs a null check |
| Can be reseated | no | yes |
| Needs `*` in the body | no | yes, everywhere |

**Prefer the reference.** Use a pointer when "no object" is a meaningful argument. The one
real drawback of references is that `swapRef(x, y)` gives the reader no hint that `x` will
change — which is exactly why some style guides (Google's, historically) require pointers
for out-parameters.

**In real C++ you would write `std::swap(x, y)`** from `<utility>`.

**Complexity:** O(1) time, O(1) space.

---

## 2. Print 1 to N, and N to 1, without a loop

```cpp
void print1toN(int n) {
    if (n < 1) return;        // base case
    print1toN(n - 1);         // recurse FIRST
    cout << n << " ";         // print on the way back UP
}

void printNto1(int n) {
    if (n < 1) return;        // base case
    cout << n << " ";         // print on the way DOWN
    printNto1(n - 1);
}
```

**Verified output:** `print1toN(5)` → `1 2 3 4 5` · `printNto1(5)` → `5 4 3 2 1`

**This pair is more important than it looks.** The two functions are *identical* except for
the order of two lines. That one swap flips the output completely, and the reason is the
call stack:

```
printNto1(3)              print1toN(3)
  print "3"                 call print1toN(2)
  printNto1(2)                call print1toN(1)
    print "2"                   call print1toN(0) -> returns
    printNto1(1)                print "1"     <- deepest frame prints first
      print "1"               print "2"
                            print "3"         <- outermost frame prints last
```

Work done **before** the recursive call happens top-down; work done **after** happens
bottom-up, as the stack unwinds. That is precisely the difference between **pre-order and
post-order traversal** in `../21_tree`, and it's why this problem is in Striver's sheet at
all. Learn it here where there's no tree to distract you.

**Complexity:** O(n) time, **O(n) space** — n stack frames. The loop version is O(1) space.

---

## 3. Sum of the first N natural numbers

### Approach 1 — Functional recursion (the return value carries the answer)

```cpp
long long sumFirstN(int n) {
    if (n <= 0) return 0;              // base case
    return n + sumFirstN(n - 1);       // combine on the way back up
}
```

### Approach 2 — Parameterised recursion (an accumulator carries the answer)

```cpp
void sumParam(int n, long long acc) {
    if (n <= 0) { cout << acc; return; }
    sumParam(n - 1, acc + n);          // tail call — nothing happens after
}
// call as sumParam(n, 0)
```

### Approach 3 — Optimal: closed form, O(1)

```cpp
long long sumFormula(int n) {
    return (long long)n * (n + 1) / 2;   // cast BEFORE multiplying
}
```

**Verified:** all three give `55` for `n = 10`.

**Why both recursion styles matter.** Approach 1 builds the answer *as the stack unwinds*;
Approach 2 builds it *on the way down* and the base case just reports it. Striver
distinguishes these deliberately, because backtracking problems in `../10_Recursion` are
almost all parameterised — you carry a partial answer down and report it at the leaf.

**The `long long` cast in Approach 3 is not decoration.** `n * (n+1)` overflows `int` once
`n` exceeds about 46,340, even though the *final* answer would fit. Cast before the
multiply, per `../01_basics` §9.

**Complexity:** #1 and #2 are O(n) time / **O(n) space** (stack). #3 is O(1) / O(1).
For `n = 10⁶` the recursions risk a stack overflow and the formula is instant — a concrete
example of "recursion is a way of thinking, not always of implementing".

---

## 4. Factorial of N

```cpp
long long factorial(int n) {
    if (n < 0)  return -1;             // GUARD: negatives never reach the base case
    if (n <= 1) return 1;              // base case: 0! = 1! = 1
    return (long long)n * factorial(n - 1);
}
```

**Verified:** `factorial(0) = 1`, `factorial(13) = 6227020800`,
`factorial(20) = 2432902008176640000`.

**Two fixes over `lec9.cpp:16`:**

1. **`long long`, not `int`.** Your version returns `int`, so `factorial(13)` produces
   `1932053504` — I ran it to confirm. That is not an error message; it is a plausible
   wrong number, which is worse.
2. **The `n < 0` guard.** Your base case is `x == 0 || x == 1`. With `x = -1` the recursion
   goes `-1, -2, -3, …` and never reaches it — **infinite recursion, then a stack
   overflow crash**. Any recursion whose base case is an equality test needs to consider
   what happens when the argument starts on the wrong side of it.

**The ceiling is still low:** `long long` overflows at `21!`. If a problem asks for `n!`
with `n > 20` it wants something else — the answer mod 10⁹+7, the trailing-zero count
(LeetCode 172), or big integers. See `../03_Loops` Q5.

**Complexity:** O(n) time, O(n) space (stack). The iterative version is O(1) space.

---

## 5. Nth Fibonacci Number — LeetCode 509

### Approach 1 — Naive recursion (correct, and unusably slow)

```cpp
long long fibNaive(int n) {
    if (n <= 1) return n;
    return fibNaive(n - 1) + fibNaive(n - 2);   // TWO calls -> branching
}
```

**Complexity: O(2ⁿ) time**, O(n) space. `fib(10)` is instant; `fib(50)` would take
years.

**Why it explodes.** Draw the tree for `fib(5)`:

```
                fib(5)
          /              \
       fib(4)            fib(3)
      /     \           /     \
   fib(3)  fib(2)    fib(2)  fib(1)
   /   \    /   \     /   \
fib(2) f(1) f(1) f(0) f(1) f(0)
```

`fib(3)` is computed **twice**, `fib(2)` **three times**, `fib(1)` **five times** — each
from scratch. The tree has ≈2ⁿ nodes while there are only `n` distinct subproblems. That
gap *is* dynamic programming.

### Approach 2 — Memoisation (top-down DP)

```cpp
long long fibMemo(int n, long long memo[]) {
    if (n <= 1) return n;
    if (memo[n] != -1) return memo[n];          // already solved
    return memo[n] = fibMemo(n-1, memo) + fibMemo(n-2, memo);
}
```

**O(n) time, O(n) space.** One line of caching turns 2ⁿ into n. This is your first DP —
`../26_dp` is this idea applied systematically.

### Approach 3 — Optimal: iterative, O(1) space

```cpp
long long fibIter(int n) {
    if (n <= 1) return n;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i) { long long c = a + b; a = b; b = c; }
    return b;
}
```

**Verified:** `fibIter(0) = 0`, `fibIter(10) = 55`, `fibIter(50) = 12586269025`.

**O(n) time, O(1) space, no stack risk.** Only the last two values are ever needed, so
storing all `n` is waste.

**Note `long long`:** `fib(47) = 2971215073` already exceeds `INT_MAX`. LeetCode 509 caps
`n` at 30 so `int` survives there, but the habit shouldn't depend on the constraint.

**There is also an O(log n) method** — matrix exponentiation, using the binary
exponentiation from #9 on the matrix `[[1,1],[1,0]]`. Worth mentioning in an interview;
rarely worth writing.

---

## 6. GCD, recursively

```cpp
int gcdRec(int a, int b) {
    return b == 0 ? a : gcdRec(b, a % b);      // tail recursion
}
```

**Verified:** `gcdRec(48, 18) = 6`, `gcdRec(17, 5) = 1`.

**This is tail recursion** — the recursive call is the *entire* return expression, with no
pending work afterwards. That makes the loop form a mechanical translation:

```cpp
int gcdIter(int a, int b) {
    while (b != 0) { int t = b; b = a % b; a = t; }
    return a;
}
```

`lec9.cpp:66` has the iterative version, and it's correct.

**On tail-call optimisation:** a compiler *may* reuse the stack frame for a tail call,
making the recursion O(1) space. GCC does this at `-O2`. **The C++ standard does not
require it**, and at `-O0` you get real frames. So never rely on TCO for correctness on
deep recursion — if depth is a concern, write the loop.

**Complexity:** O(log min(a,b)) time. O(log) space as written, O(1) if the compiler applies
TCO or you use the loop. Derivation in `../03_Loops` #4.

---

# Section 2 — Important

## 7. nCr without overflow — the most valuable problem here

### Approach 1 — What `lec9.cpp:56` does (broken)

```cpp
int combination(int n, int r) {
    return factorial(n) / (factorial(r) * factorial(n - r));   // OVERFLOWS
}
```

**I ran this. Measured results:**

| Call | Returns | Correct answer |
|---|---|---|
| `combination(5, 2)` | 10 | 10 ✓ |
| `combination(13, 2)` | **24** | **78** ✗ |
| `combination(20, 10)` | **11** | **184756** ✗ |

The answers are small — `C(20,10)` is only 184,756, comfortably inside `int` — but the
*intermediate* `20!` is ~2.4×10¹⁸ and `factorial` returns `int`. The computation destroys
itself before the division can rescue it.

**The general lesson: a correct formula can still be a wrong algorithm.** Where the
intermediates go matters as much as where the answer lands.

### Approach 2 — Optimal: multiplicative, interleaving multiply and divide

```cpp
long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    if (r > n - r) r = n - r;              // C(n,r) == C(n,n-r): fewer iterations
    long long res = 1;
    for (int i = 0; i < r; ++i)
        res = res * (n - i) / (i + 1);     // divide at EVERY step, not at the end
    return res;
}
```

**Verified:** `nCr(5,2)=10`, `nCr(13,2)=78`, `nCr(20,10)=184756`, `nCr(30,15)=155117520`.

**Why the division is always exact.** After `i+1` iterations `res` holds `C(n, i+1)`, which
is an integer by definition. And before the division, `res * (n-i)` equals
`C(n,i) × (n-i)`, which is provably divisible by `i+1`. So no truncation ever occurs —
this is not "close enough", it is exact integer arithmetic.

**Why `if (r > n-r) r = n-r`.** `C(30,25)` needs 25 iterations; `C(30,5)` needs 5 and gives
the same answer. It also keeps the running value smaller for longer.

**Dry run** — `nCr(5,2)`: `r=2`. `i=0`: `res = 1*5/1 = 5` (= C(5,1) ✓). `i=1`:
`res = 5*4/2 = 10` (= C(5,2) ✓).

### Approach 3 — Pascal's recurrence (the recursive view)

```cpp
long long nCrRec(int n, int r) {
    if (r == 0 || r == n) return 1;
    return nCrRec(n-1, r-1) + nCrRec(n-1, r);     // O(2^n) without memoisation
}
```

Correct, exponential, and never overflows intermediates. Memoise it into a 2D table and you
have the standard O(n·r) DP — which is exactly problem #8.

**Complexity:** Approach 2 is **O(min(r, n−r))** time, O(1) space — the one to write.

---

## 8. Pascal's Triangle — LeetCode 118 / 119

Your `lec9.cpp:26` version, which I ran and confirmed **correct**:

```cpp
void printPascalsTriangle(int n) {
    for (int i = 0; i < n; i++) {
        long long coefficient = 1;
        for (int space = 1; space <= n - i; space++) cout << " ";
        for (int j = 0; j <= i; j++) {
            cout << coefficient << " ";
            coefficient = coefficient * (i - j) / (j + 1);   // C(i,j+1) from C(i,j)
        }
        cout << "\n";
    }
}
```

**Verified output for `n = 5`:**

```
     1 
    1 1 
   1 2 1 
  1 3 3 1 
 1 4 6 4 1 
```

**The recurrence being used** is the same exact-division trick as #7:

`C(i, j+1) = C(i, j) × (i − j) / (j + 1)`

So each row is generated in O(row length) with **no factorials and no overflow of
intermediates**. This is a genuinely good implementation — better than the `nCr`-per-cell
approach most people write first, which is O(n²·r).

**My one change:** make `coefficient` a `long long`. Row 34 of Pascal's triangle exceeds
`INT_MAX`, so `int` silently corrupts from there on.

**The additive view** — the one LeetCode 118 expects, and the one that generalises to DP:

> Each entry is the sum of the two entries above it: `row[j] = prev[j-1] + prev[j]`

That's the same "each cell depends on two cells above" shape you'll see in every 2D DP
grid. Both views are worth holding.

**Complexity:** O(n²) time — which is optimal, since the output *is* n²/2 numbers.
O(1) extra space for the printing version; O(n) for the row-returning version (LeetCode 119).

---

## 9. Pow(x, n) — recursive binary exponentiation — LeetCode 50

```cpp
double powRec(double x, long long n) {
    if (n == 0) return 1.0;                 // base case
    double half = powRec(x, n / 2);         // ONE recursive call, not two
    return (n % 2 == 0) ? half * half : half * half * x;
}

double myPow(double x, int n) {
    long long N = n;                        // widen BEFORE negating
    if (N < 0) return 1.0 / powRec(x, -N);
    return powRec(x, N);
}
```

**Verified:** `myPow(2,10) = 1024`, `myPow(2,-2) = 0.25`, `myPow(2,INT_MIN) = 0`,
`myPow(1,INT_MIN) = 1`.

**Compute `half` once and reuse it.** Writing
`return powRec(x, n/2) * powRec(x, n/2);` is the single most common mistake here: it makes
**two** recursive calls per level instead of one, turning O(log n) into O(n). Same trap as
Fibonacci in #5 — the fix is to store the result in a local.

**`long long N = n;` is the graded line.** `n` can be `INT_MIN = -2147483648`, and `-INT_MIN`
does not fit in an `int` — undefined behaviour. LeetCode has that exact test case. Widen
first, then negate.

**Dry run** — `powRec(2, 10)`:

```
powRec(2,10) -> half = powRec(2,5)
  powRec(2,5) -> half = powRec(2,2)
    powRec(2,2) -> half = powRec(2,1)
      powRec(2,1) -> half = powRec(2,0) = 1 ; odd  -> 1*1*2 = 2
    even -> 2*2 = 4
  odd -> 4*4*2 = 32
even -> 32*32 = 1024  ✓
```

Five levels for exponent 10, i.e. `⌊log₂10⌋+1`.

**Complexity:** **O(log n)** time, O(log n) space (stack). The iterative version in
`../03_Loops` #9 is O(1) space — prefer it when depth matters.

---

## 10. Function overloading — `mini`

```cpp
int    mini(int a, int b)           { return a < b ? a : b; }
double mini(double a, double b)     { return a < b ? a : b; }
int    mini(int a, int b, int c)    { return mini(mini(a, b), c); }   // reuses the 2-arg form
```

**Verified:** `mini(3,7) = 3`, `mini(2.5,1.5) = 1.5`, `mini(9,4,6) = 4`.

**What each overload demonstrates:**
- **Different types** — resolution by argument type
- **Different arity** — resolution by argument count
- **The 3-argument version calling the 2-argument version** — overloads can compose

**What does NOT work, and why:**

```cpp
double mini(int a, int b);   // ERROR: differs from int mini(int,int) only by return type
mini(3, 2.5);                // ERROR: ambiguous — int->double or double->int, no winner
```

Return type is not part of overload resolution, because `mini(3,2);` as a statement gives
the compiler no context to choose from.

**In modern C++ you'd write one template** instead of two bodies:

```cpp
template <typename T> T mini(T a, T b) { return a < b ? a : b; }
```

Worth saying in an interview — it shows you know overloading is the manual version of
something the language automates. (`std::min` already exists, of course.)

**Complexity:** O(1).

---

## 11. Count primes in a range by reusing `isPrime`

```cpp
bool isPrime(int n) {                       // written once, in ../03_Loops
    if (n < 2) return false;
    if (n < 4) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; (long long)i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

int countPrimesInRange(int a, int b) {      // this function knows nothing about primality
    int count = 0;
    for (int i = a; i <= b; ++i)
        if (isPrime(i)) ++count;
    return count;
}
```

**Verified:** `countPrimesInRange(1,20) = 8` (2,3,5,7,11,13,17,19),
`countPrimesInRange(10,20) = 4`.

**This is the actual point of the folder.** `countPrimesInRange` contains no primality
logic. It cannot be wrong about primes, because it doesn't know what one is. All the
edge cases — `n < 2`, the `√n` bound, skipping evens — live in **one** function, tested
once.

Compare with inlining the primality test into the loop: you'd have a nested loop, a flag, a
`break`, and the `n < 2` case to remember *again*. That's precisely the bug in
`../03_Loops/loops2.cpp:6`, and decomposition is what prevents it recurring.

**The interview framing:** when you're asked to solve something with visible sub-parts, name
the helper first (`isPrime`, `isValid`, `distance`) and write the top-level function
against it. Interviewers read that as design sense, and it gives you a clean place to
discuss complexity.

**Complexity:** O((b−a)·√b) time, O(1) space. **When the range is large and dense, a Sieve
is better** — O(n log log n) once, then O(1) per query (`../03_Loops` #10). Choosing between
them *is* the interview question.

---

# Section 3 — Good to Know

## 12. Climbing Stairs — LeetCode 70

> You can climb 1 or 2 steps at a time. How many distinct ways to reach step `n`?

### Approach 1 — Recursion (the modelling step)

```cpp
int climbRec(int n) {
    if (n <= 2) return n;                       // 1 step: 1 way; 2 steps: 2 ways
    return climbRec(n-1) + climbRec(n-2);
}
```

**O(2ⁿ)** — identical shape to naive Fibonacci.

**The reasoning that produces this recurrence** is the whole problem: to arrive at step `n`
your **last** move was either a 1-step (from `n-1`) or a 2-step (from `n-2`). Those two sets
of paths are disjoint and cover everything, so you add them. *"Classify by the last
decision"* is the standard way to find a recurrence, and it's how you'll attack most of
`../26_dp`.

### Approach 2 — Optimal: iterative, O(1) space

```cpp
long long climbStairs(int n) {
    if (n <= 2) return n;
    long long a = 1, b = 2;                    // ways(1), ways(2)
    for (int i = 3; i <= n; ++i) { long long c = a + b; a = b; b = c; }
    return b;
}
```

**Verified:** `climbStairs(2) = 2`, `climbStairs(3) = 3`, `climbStairs(5) = 8`.

**It *is* Fibonacci, shifted by one** — `ways(n) = fib(n+1)`. Recognising a known
recurrence in disguise is worth saying out loud; it's how you get to the optimal solution
in thirty seconds instead of five minutes.

**Complexity:** O(n) time, O(1) space.

---

## 13. N-th Tribonacci Number — LeetCode 1137

> `T₀ = 0, T₁ = 1, T₂ = 1`, and `Tₙ = Tₙ₋₁ + Tₙ₋₂ + Tₙ₋₃`.

```cpp
int tribonacci(int n) {
    if (n == 0) return 0;
    if (n <= 2) return 1;
    int a = 0, b = 1, c = 1;
    for (int i = 3; i <= n; ++i) { int d = a + b + c; a = b; b = c; c = d; }
    return c;
}
```

**Verified:** `tribonacci(4) = 4`, `tribonacci(25) = 1389537`.

**Why this is here and not filed as "more Fibonacci".** It tests whether #5 generalised for
you: the branching factor goes 2 → 3, so the naive recursion goes from O(2ⁿ) to **O(3ⁿ)**,
and the rolling window goes from two variables to three. If you understood the *shape*
rather than memorising two variables, this takes one minute.

**Note the base cases are asymmetric** — `T₀ = 0` but `T₁ = T₂ = 1`, so `n <= 2` cannot be
folded into a single `return n`. Read the definition, don't pattern-match from Fibonacci.

**Complexity:** O(n) time, O(1) space.

---

## 14. Return two values from one function

A C++ function returns one value. Three ways around it:

### Approach 1 — Reference out-parameters

```cpp
void minMax(int a, int b, int c, int &mn, int &mx) {
    mn = min(a, min(b, c));
    mx = max(a, max(b, c));
}
// int lo, hi;  minMax(9, 4, 6, lo, hi);   -> lo = 4, hi = 9
```

**Verified:** `minMax(9,4,6, mn, mx)` gives `mn = 4`, `mx = 9`.

### Approach 2 — `std::pair`

```cpp
#include <utility>
pair<int,int> minMaxPair(int a, int b, int c) {
    return make_pair(min(a,min(b,c)), max(a,max(b,c)));
}
// pair<int,int> r = minMaxPair(9,4,6);   -> r.first = 4, r.second = 9
```

### Approach 3 — A struct (best when there are more than two, or names matter)

```cpp
struct Range { int lo, hi; };
Range minMaxStruct(int a, int b, int c) { return { min(a,min(b,c)), max(a,max(b,c)) }; }
// r.lo and r.hi say what they mean; r.first and r.second do not
```

**Which to use.** `pair` is fine for two values whose meaning is obvious from context and is
what the STL itself returns (`map::insert`, `equal_range`). **A struct is better the moment
the names carry information** — `.lo`/`.hi` beats `.first`/`.second` for a reader. Reference
out-parameters are the C-style approach; they're still right when you want to avoid a copy
or fill an existing object, but they make the call site ambiguous about what changes.

*(In C++17 you'd add structured bindings — `auto [lo, hi] = minMaxPair(...)`. **That does
not compile on this repo's C++14 toolchain**, so it's noted rather than used.)*

**Complexity:** O(1) for all three.

---

## 15. `static` local variable — counting calls

```cpp
int callCount() {
    static int calls = 0;      // initialised ONCE, on the first call ever
    return ++calls;
}

int main() {
    int r1 = callCount();      // 1
    int r2 = callCount();      // 2
    int r3 = callCount();      // 3
    cout << r1 << " " << r2 << " " << r3;
}
```

**Verified output:** `1 2 3`.

**Scope and lifetime are different things**, and this is the cleanest demonstration:

| | `int calls = 0;` | `static int calls = 0;` |
|---|---|---|
| **Scope** (who can see it) | the function | the function — *unchanged* |
| **Lifetime** (how long it lives) | one call | the whole program |
| Initialised | every call | once, on first entry |

A plain local would reset to 0 every time and the function would always return 1. A global
would have the right lifetime but the wrong scope — anything could modify it. `static`
gives you persistence *without* exposure.

### A real bug I hit while testing this — worth your time

My first test harness was a macro:

```cpp
#define CHK(e, exp) cout << ((e)==(exp) ? "ok " : "FAIL ") << #e << " = " << (e) << "\n";
CHK(callCount(), 1)      // reported FAIL, and printed 1
```

Two defects, both classic:

1. **The macro evaluates `e` twice**, so `callCount()` — a function *with a side effect* —
   ran twice per check. Macros substitute text; they don't evaluate arguments once like
   functions do.
2. **The order of the two calls was unspecified.** In C++14 the operands of a `<<` chain
   have no guaranteed evaluation order, so GCC evaluated the *printing* call before the
   *comparison* call. The output disagreed with the verdict.

That's the same hazard as `i++ + ++i` from `../01_basics` Q4 — and it appeared in my own
test code, not in the function under test. **The function was correct all along.** Two
lessons: never put a side-effecting call in a macro argument, and never rely on evaluation
order within an expression.

**Complexity:** O(1).

---

## 16. Sum of digits and reverse a number — recursively

```cpp
int sumDigits(int n) {
    if (n < 0) n = -n;                       // handle the sign once, at the top
    if (n == 0) return 0;                    // base case
    return n % 10 + sumDigits(n / 10);       // last digit + sum of the rest
}

// Reverse needs an accumulator, because digits are produced most-significant-last.
int revHelper(int n, int rev) {
    if (n == 0) return rev;
    return revHelper(n / 10, rev * 10 + n % 10);
}
int reverseRec(int n) {
    return n < 0 ? -revHelper(-n, 0) : revHelper(n, 0);
}
```

**Verified:** `sumDigits(1234)=10`, `sumDigits(0)=0`, `sumDigits(-45)=9`,
`reverseRec(1234)=4321`, `reverseRec(-123)=-321`, `reverseRec(120)=21`.

**Why `sumDigits` needs no accumulator but `reverse` does.** Addition is associative and
commutative, so it doesn't matter what order the digits come back in — you can combine on
the way up. Reversal is **positional**: each digit's place value depends on how many digits
follow it, which the unwinding stack doesn't know. Carrying `rev` down means each digit is
placed the moment it's seen.

That distinction — *can this be combined on the way up, or must state be carried down?* —
is exactly what separates functional from parameterised recursion, and it decides the shape
of most `../10_Recursion` solutions.

**Note the sign handling** is done **once at the entry point**, not inside the recursion.
Putting `if (n < 0)` inside `revHelper` would re-test it at every level for no benefit —
and would be wrong, since `-n` at each step would flip the sign repeatedly.

**Complexity:** O(log n) time, O(log n) space (stack depth = digit count ≤ 10).
The iterative versions in `../03_Loops` are O(1) space.

---

# Section 4 — Extra Practice (approach only)

### Print your name N times — GFG, Striver 1.4
`void printName(int i, int n) { if (i > n) return; cout << "name\n"; printName(i+1, n); }`
The simplest recursion that exists — no return value, no combining. Its only job is to make
you write a base case. **O(n) time, O(n) space.**

### Tower of Hanoi — GFG (Medium)
Move `n` disks from A to C using B, never placing a larger disk on a smaller one.
`hanoi(n, from, to, aux)`: move `n-1` from `from`→`aux`, move disk `n` `from`→`to`, move
`n-1` from `aux`→`to`. **Two recursive calls per level, so 2ⁿ−1 moves — provably optimal.**
The value is that the recursion is genuinely easier than any iterative formulation; it's
the best argument for recursion as a tool. **O(2ⁿ) time, O(n) space.**

### Power of Two / Three / Four, recursively — LeetCode 231 / 326 / 342
`isPowerOfTwo(n)`: `n == 1` → true; `n <= 0 || n % 2 != 0` → false; else recurse on `n/2`.
Compare against the loop and the `n & (n-1)` trick from `../03_Loops` #14. The lesson is
that all three are O(log n) except the bit trick, which is O(1) — recursion adds stack cost
and buys nothing here. **O(log n) time, O(log n) space.**

### Check if a number is prime, recursively — *Drill*
`isPrimeRec(n, i)` returning true when `i*i > n`, false when `n % i == 0`, else recursing on
`i+1` (or `i+2` skipping evens). **Deliberately contrived** — this is a loop wearing a
costume, it costs O(√n) stack frames instead of O(1), and it's here so you can articulate
*when recursion is the wrong tool*. **O(√n) time, O(√n) space.**

### Sum of an array's elements, recursively — GFG · **forward reference**
`sum(arr, n) = arr[n-1] + sum(arr, n-1)`, base case `n == 0` → 0. Needs `../07_Array`.
Note the two idiomatic parameterisations — shrink from the end (`n-1`) or advance an index
(`i+1`) — and that the second is what generalises to two-pointer recursion.
**O(n) time, O(n) space.**

### Reverse an array, recursively — GFG, Striver 1.4 · **forward reference**
Two-pointer recursion: swap `arr[l]` and `arr[r]`, recurse on `(l+1, r-1)`, base case
`l >= r`. Needs `../07_Array`. The single-pointer variant (`i` and `n-i-1`) is the same
algorithm with one parameter. **O(n) time, O(n) space.**

### Check if a string is a palindrome, recursively — GFG, Striver 1.4 · **forward reference**
Identical two-pointer recursion to the array reversal, comparing instead of swapping; base
case `l >= r` → true. Needs `../09_Strings`. The follow-up that makes it interesting is
ignoring non-alphanumerics and case (LeetCode 125). **O(n) time, O(n) space.**

### Binary search, recursively — LeetCode 704 · **forward reference**
`search(arr, lo, hi, target)`: base `lo > hi` → −1; `mid = lo + (hi-lo)/2`; recurse left or
right. Needs `../07_Array` and `../11_linearAndBinarySearch`. **This is tail recursion**,
so the iterative version is a direct translation with O(1) space — which is why binary
search is essentially always written iteratively. **O(log n) time, O(log n) space recursive.**

### Find your machine's real recursion limit — *Drill*
`void dive(long long d) { if (d % 10000 == 0) cout << d << "\n"; dive(d+1); }` — run it and
watch where it crashes. You'll get a concrete number (typically 10⁴–10⁵ frames on a 1 MB
stack) instead of a vague sense that "deep recursion is bad". Add a large local array to the
function and watch the limit collapse. **Crashes by design.**

---

# Bugs in your current code

Per the repo's convention I have not edited your `.cpp` files. `lec9.cpp` compiles cleanly
(`g++ -fsyntax-only -Wall` produces no warnings), so these are logic and design issues —
all confirmed by running the code.

### `05_Function/lec9.cpp`

| Line | Problem | Fix |
|---|---|---|
| 76-84 | **`main()` is empty.** Nine functions are defined and **none is ever called** — nothing in this file has ever executed | call each one; `printPascalsTriangle(5)` alone would have confirmed it works |
| 16 | **`factorial` returns `int`**, so `factorial(13)` returns `1932053504` instead of `6227020800`. Verified by running it | `long long factorial(int n)` and `(long long)n * factorial(n-1)` |
| 16 | **No negative guard.** `x == 0 \|\| x == 1` is never reached from a negative `x`, so `factorial(-1)` recurses forever → stack overflow crash | `if (n < 0) return -1;` before the base case |
| 56 | **`combination` overflows badly.** Measured: `combination(13,2)` → **24** (want 78); `combination(20,10)` → **11** (want 184756) | use the multiplicative form in solution #7 |
| 6 | `greet2(string name)` copies the string on every call | `void greet2(const string &name)` |
| 1-2 | `string` is used but only `<iostream>` is included. It compiles here because `<iostream>` transitively pulls in `<string>`, but the standard does not guarantee that | add `#include <string>` |

**The `combination` bug is the one to fix first.** It returns small, plausible numbers, so
nothing looks wrong until you check an answer you already know.

### What you got right — keep these habits

- **`printPascalsTriangle` (line 26) is correct**, and better than the obvious approach. I
  ran it; the output is exact for `n = 5`. Using the recurrence
  `C(i,j+1) = C(i,j)·(i−j)/(j+1)` avoids factorials entirely, and the integer division is
  provably exact at every step. Most people write `nCr` per cell and get an O(n²·r)
  algorithm that overflows — you didn't. (One upgrade: make `coefficient` a `long long`;
  row 34 exceeds `INT_MAX`.)
- **`swapNumbers(int &a, int &b)` (line 48)** uses a reference correctly — this is the form
  to prefer over pointers.
- **`gcd` (line 66)** is a correct iterative Euclidean algorithm, with the right `b != 0`
  termination.
- **`mini` (line 13)** using a ternary is idiomatic and branch-free-friendly.
- Splitting `combination` into a call to `factorial` was the right *instinct* — decomposition
  is exactly the skill this folder teaches. The helper just needs to not overflow.
