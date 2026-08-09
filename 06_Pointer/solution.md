# 06 — Pointers: Solutions

Full C++14 for Sections 1–3, approach-only for Section 4.
**Every sample below was compiled and executed before publication**; all claimed outputs and
all `const` compile/error verdicts are measured, not predicted.

---

# Section 1 — Must Do

## 1. Pointer fundamentals

```cpp
#include <iostream>
using namespace std;

int main() {
    int x = 4;
    int *p = &x;              // p holds the ADDRESS of x

    cout << &x << "\n";       // 0x61ff00  — address of x
    cout << p  << "\n";       // 0x61ff00  — same address
    cout << *p << "\n";       // 4         — the VALUE at that address
    cout << &p << "\n";       // a DIFFERENT address — p is itself a variable

    *p = 6;                   // write through the pointer
    cout << x  << "\n";       // 6         — x itself changed
    return 0;
}
```

**Verified**: `&x` and `p` print the same address; `*p` prints 4; after `*p = 6`, `x` is 6.
Your `pointer.cpp:20-25` does exactly this and the annotations are correct.

**The mental model.** Three things exist, not two:

| Expression | Meaning | For `int x = 4; int *p = &x;` |
|---|---|---|
| `x` | the value | `4` |
| `&x` | where the value lives | `0x61ff00` |
| `p` | a variable holding that address | `0x61ff00` |
| `*p` | the value at that address | `4` |
| `&p` | where `p` itself lives | a different address |

`p` is a variable like any other — it has its own address. That's what makes double
pointers possible (#6).

**Watch the declaration syntax:**

```cpp
int* p, q;     // p is int*, q is a plain int  <- almost never what you meant
int *p, *q;    // both are pointers
```

The `*` binds to the declarator, not the type. Declare one per line if in doubt.

**Complexity:** O(1).

---

## 2. Swap: by value vs by pointer vs by reference

```cpp
void swapValue(int a, int b)  { int t = a; a = b; b = t; }      // does nothing
void swapPtr(int *a, int *b)  { if (!a || !b) return;
                                int t = *a; *a = *b; *b = t; }  // works
void swapRef(int &a, int &b)  { int t = a; a = b; b = t; }      // works

int main() {
    int a = 5, b = 6;
    swapValue(a, b);   cout << a << " " << b << "\n";   // 5 6  <- unchanged
    swapPtr(&a, &b);   cout << a << " " << b << "\n";   // 6 5
    swapRef(a, b);     cout << a << " " << b << "\n";   // 5 6
    return 0;
}
```

**Verified output:** `5 6` / `6 5` / `5 6`.

**Why `swapValue` cannot work.** Arguments are copied into the function's own stack frame.
It swaps its private copies, then the frame is destroyed on return. The caller's `a` and `b`
were never reachable from inside.

**Why the other two do.** Both give the function a way back to the caller's storage — an
address in one case, an alias in the other. Neither copies the values it's meant to change.

### The naming issue in your file

`pointer.cpp:15` is:

```cpp
//pass by reference
void swapReference(int* a, int* b){ int temp = *a; *a=*b; *b=temp; return; }
```

**This is pass by *pointer*, not pass by reference.** The behaviour is correct — I ran it —
but the terminology matters, because interviewers ask for the distinction explicitly. True
pass-by-reference is:

```cpp
void swapReference(int &a, int &b) { int temp = a; a = b; b = temp; }
swapReference(x, y);      // note: no & at the call site
```

**The `if (!a || !b) return;` guard** in `swapPtr` has no counterpart in the reference
version — and that asymmetry *is* the trade-off. A reference cannot be null, so there is
nothing to check. A pointer can, so you must.

**Complexity:** O(1) time and space for all three.

---

## 3. Dynamic array: allocate, fill, sum, free

```cpp
#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "How many elements? ";
    cin >> n;                          // size is a RUNTIME value — this is why we need the heap

    int *arr = new int[n];             // allocate on the heap
    if (!arr) return 1;

    for (int i = 0; i < n; ++i) arr[i] = i + 1;

    long long sum = 0;                 // long long: n elements up to 1e9 each overflows int
    for (int i = 0; i < n; ++i) sum += arr[i];
    cout << "Sum = " << sum << "\n";

    delete[] arr;                      // [] because it was new[]
    arr = nullptr;                     // prevents dangling use and double free
    return 0;
}
```

**Verified:** filling `new int[5]` with 1..5 sums to 15.

**Why the heap at all.** `int arr[n];` with a runtime `n` is not standard C++ (it's a GCC
extension — variable-length arrays). And the stack is ~1 MB, so `int arr[1000000]` as a
local is a crash. The heap is large and runtime-sized; the price is that **you must free it
yourself**.

**Three rules, each guarding a specific bug:**

| Rule | Prevents |
|---|---|
| `new[]` pairs with `delete[]` | undefined behaviour from `delete` on an array |
| `delete` exactly once | double free |
| `= nullptr` right after | dangling pointer, and makes a second `delete` a safe no-op |

**What you'd actually write in modern C++:**

```cpp
#include <vector>
vector<int> arr(n);      // frees itself, knows its own size, no delete anywhere
```

`std::vector` is the correct answer in production and in most interviews. Learning `new[]`
first is still worth it, because it's what `vector` does internally and what
`../17_linked_list` will do by hand.

**Complexity:** O(n) time, O(n) heap space.

---

## 4. Traverse using pointer arithmetic only

```cpp
long long sumBlock(const int *p, int n) {
    long long sum = 0;
    for (const int *end = p + n; p < end; ++p)   // walk the pointer, never index
        sum += *p;
    return sum;
}
```

**Verified:** `sumBlock(arr, 5)` on `{1,2,3,4,5}` returns 15.

**What this makes concrete:**

- **`p + n` is scaled.** It advances `n * sizeof(int)` = `n * 4` bytes. The compiler does
  the multiplication; you write element counts, not byte counts.
- **`p < end` is a legal pointer comparison** within one array. Pointer ordering is only
  defined inside a single allocation (plus one past the end).
- **`++p` moves one element.**

**Indexing is defined as pointer arithmetic**, which I confirmed by printing all three:

```cpp
arr[2]      // 3
*(arr + 2)  // 3   — this IS the definition of arr[2]
2[arr]      // 3   — legal, because addition commutes. Never write this.
```

`2[arr]` compiling is not a quirk to memorise; it's proof that `[]` is nothing but
`*(a + i)`.

**Note `const int *p`** — the function promises not to modify the caller's data, and the
compiler enforces it. This is the `const` placement you'll use most (see #7).

**Complexity:** O(n) time, O(1) space.

---

## 5. The same function, by pointer and by reference

```cpp
void doubleItPtr(int *x) {
    if (x == nullptr) return;      // MUST check — a pointer can be null
    *x = *x * 2;                   // needs dereferencing everywhere
}

void doubleItRef(int &x) {
    x = x * 2;                     // reads like a normal variable
}

int main() {
    int a = 5;
    doubleItPtr(&a);   cout << a << "\n";   // 10   — call site shows the &
    doubleItRef(a);    cout << a << "\n";   // 20   — call site looks like by-value
    doubleItPtr(nullptr);                   // safe, does nothing
    // doubleItRef(???);                    // there is no null reference to pass
    return 0;
}
```

**The three differences that decide which to use:**

1. **Null.** `doubleItPtr(nullptr)` is a legal call that must be handled. There is no
   equivalent for the reference — the compiler guarantees a real object, so the check is
   unnecessary *by construction*.
2. **Call-site visibility.** `doubleItPtr(&a)` visibly signals "this may change `a`".
   `doubleItRef(a)` does not — you must read the signature to know. This is the one genuine
   argument *for* pointer out-parameters, and why some style guides require them.
3. **Syntax noise.** The reference version has no `*` at all.

**Default to the reference; switch to a pointer when null is a meaningful input.** And for
read-only class-type parameters, `const T&` — no copy, no null, read-only enforced.

**Complexity:** O(1).

---

# Section 2 — Important

## 6. Double pointer: allocate through an out-parameter

```cpp
void allocAndFill(int **out, int n) {
    *out = new int[n];                    // writes to the CALLER's pointer
    for (int i = 0; i < n; ++i) (*out)[i] = i * i;
}

int main() {
    int *data = nullptr;
    allocAndFill(&data, 4);               // pass the ADDRESS OF the pointer
    for (int i = 0; i < 4; ++i) cout << data[i] << " ";   // 0 1 4 9
    delete[] data;
    return 0;
}
```

**Verified output:** `0 1 4 9`.

**Why the second `*` is mandatory.** Consider the broken version:

```cpp
void allocBroken(int *out, int n) { out = new int[n]; }   // WRONG
int *data = nullptr;
allocBroken(data, 4);          // data is STILL nullptr — and the block is leaked
```

`out` is a *copy* of the caller's pointer. Reassigning the copy leaves `data` untouched, and
the allocated block is now unreachable — a leak. **This is the identical failure mode as
`swapValue` in #2**, one level up: to modify an `int` you need `int*`; to modify an `int*`
you need `int**`.

**Note the parentheses:** `(*out)[i]`, not `*out[i]`. `[]` binds tighter than `*`, so
`*out[i]` means `*(out[i])` — treating `out` as an array of pointers. Wrong, and it compiles.

**The C++ way** — a reference to a pointer:

```cpp
void allocAndFill(int *&out, int n) { out = new int[n]; ... }
allocAndFill(data, 4);        // cleaner call site, and `out` cannot be null
```

Learn the double pointer anyway: `Node **head` is everywhere in C linked-list code, and
`char **argv` is in every `main`.

**Complexity:** O(n) time, O(n) heap.

---

## 7. `const` placement — predict, then verify

**Predict all six before reading on.** Then compile. I did; this table is measured:

```cpp
int x = 1, y = 2;
```

| # | Declaration + operation | Result |
|---|---|---|
| 1 | `const int *p = &x;` then `*p = 6;` | **compile error** |
| 2 | `const int *p = &x;` then `p = &y;` | compiles |
| 3 | `int *const p = &x;` then `*p = 6;` | compiles |
| 4 | `int *const p = &x;` then `p = &y;` | **compile error** |
| 5 | `const int *const p = &x;` then `*p = 6;` | **compile error** |
| 6 | `const int *const p = &x;` then `p = &y;` | **compile error** |

**The rule: read right to left.**

- `const int *p` → "`p` is a pointer to an `int` that is `const`" → **the data is frozen**
- `int *const p` → "`p` is a `const` pointer to `int`" → **the pointer is frozen**
- `const int *const p` → both frozen

**Rule of thumb:** `const` **before** the `*` protects the **pointee**; **after** the `*`
protects the **pointer**.

`const int *p` and `int const *p` are **identical** — both have `const` left of the `*`.
The first spelling is far more common.

**Where this matters in real code:** `void f(const int *data, int n)` is a compiler-enforced
promise not to modify the caller's buffer. That's the form to reach for on every read-only
pointer parameter, and it's what `sumBlock` in #4 uses. `int *const` is comparatively rare.

**Complexity:** O(1) — this is a compile-time exercise.

---

## 8. Spot the bug: leak, dangling, double free

```cpp
// ---- BUG 1: MEMORY LEAK ----
void leak() {
    int *p = new int[100];
    p = nullptr;              // the block is now unreachable — can never be freed
}                             // FIX: delete[] p;  BEFORE reassigning

// ---- BUG 2: DANGLING POINTER ----
void dangling() {
    int *p = new int(5);
    delete p;
    cout << *p;               // undefined behaviour — reading freed memory
}                             // FIX: p = nullptr; after delete, and check before use

// ---- BUG 3: DOUBLE FREE ----
void doubleFree() {
    int *p = new int(5);
    delete p;
    delete p;                 // undefined behaviour — corrupts the allocator
}                             // FIX: p = nullptr; after the first delete

// ---- BUG 4: RETURNING A DANGLING POINTER ----
int* returnsGarbage() {
    int local = 42;
    return &local;            // `local` dies at the return — the address is invalid
}                             // FIX: return by value, or allocate on the heap

// ---- BUG 5: MISMATCHED delete ----
void mismatch() {
    int *arr = new int[10];
    delete arr;               // undefined behaviour — needs delete[]
}
```

**Why each is genuinely dangerous, not merely untidy:**

- A **leak** doesn't crash — it grows. A server leaking a few KB per request is fine in
  testing and dies in production.
- A **dangling read** often returns plausible stale data, so the program continues with
  wrong values rather than failing loudly.
- A **double free** corrupts the allocator's bookkeeping, so the crash surfaces in an
  unrelated allocation later. These are among the hardest bugs to trace.
- **Bug 4 is the one interviewers use most**, because it looks fine. `local` lives in the
  stack frame, which is destroyed on return; the address remains but points to memory that
  will be overwritten by the next call.

**The single habit that kills bugs 2, 3 and 5:**

```cpp
delete[] p;      // matching form
p = nullptr;     // immediately
```

`delete nullptr;` is explicitly defined to do nothing, so a second delete becomes harmless.

**The structural fix** is not to manage raw memory at all — `std::vector` for buffers,
`std::unique_ptr` for single objects. Both free in their destructors, so they release
correctly even when an exception unwinds the stack, which manual `delete` does not.
Naming **RAII** here is what a senior interviewer is listening for.

**Complexity:** O(1) — this is a code-reading exercise.

---

## 9. Function pointers

```cpp
int cmpAsc(int a, int b)  { return a - b; }
int cmpDesc(int a, int b) { return b - a; }

// Takes a FUNCTION as a parameter
int pick(int a, int b, int (*cmp)(int, int)) {
    return cmp(a, b) <= 0 ? a : b;
}

int main() {
    cout << pick(3, 9, cmpAsc)  << "\n";   // 3
    cout << pick(3, 9, cmpDesc) << "\n";   // 9
    return 0;
}
```

**Verified:** `3` and `9`.

**The declaration syntax is the hard part:**

```cpp
int (*cmp)(int, int);   // cmp is a POINTER TO a function taking (int,int), returning int
int  *cmp(int, int);    // cmp is a FUNCTION taking (int,int), returning int*
```

The parentheses around `*cmp` are what make the difference. Without them, `*` binds to the
return type.

**Why this exists.** Passing a function lets the *caller* decide policy while the callee
keeps the algorithm. That's exactly how `qsort` works in C, and it's the direct ancestor of:

```cpp
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });   // ../12_sorting
```

A lambda is a nicer, faster way to express the same idea (nicer because it can capture,
faster because it can be inlined — a function pointer usually can't be). But understanding
that **a function has an address** is what makes callbacks, comparators and virtual dispatch
stop being magic.

**Complexity:** O(1) per call, plus the callee's cost.

---

## 10. Swap two pointers (not the values)

```cpp
void swapPointers(int **a, int **b) {
    int *t = *a;
    *a = *b;
    *b = t;
}

int main() {
    int m = 1, n = 2;
    int *pm = &m, *pn = &n;

    swapPointers(&pm, &pn);

    cout << *pm << " " << *pn << "\n";   // 2 1  <- the POINTERS were swapped
    cout << m   << " " << n   << "\n";   // 1 2  <- the VALUES are untouched
    return 0;
}
```

**Verified:** `2 1` then `1 2`.

**This is the problem that separates the two levels.** Compare with #2:

| | What changes | What stays |
|---|---|---|
| `swapPtr(&m, &n)` (#2) | `m` and `n` — the **values** | which variable each pointer points at |
| `swapPointers(&pm, &pn)` | `pm` and `pn` — the **pointers** | `m` and `n` |

After `swapPointers`, `pm` points at `n` and `pn` points at `m`. Nothing in memory moved —
only the two addresses were exchanged.

**Why it needs `int**`:** to modify an `int*`, you need that pointer's address. Same rule as
#6, and the same rule as needing `int*` to modify an `int`.

**Why this matters beyond the drill.** Rearranging pointers instead of copying data is the
core efficiency argument for linked structures: swapping two 8-byte addresses is O(1)
regardless of how large the pointed-to objects are. Every linked-list reversal in
`../17_linked_list` is pointer rearrangement, not data movement.

**Complexity:** O(1) time and space.

---

# Section 3 — Good to Know

## 11. Array decay and `sizeof`

```cpp
void showSizes(int arr[]) {                        // identical to int *arr
    cout << "inside:  " << sizeof(arr) << "\n";    // 4  — it's a pointer
}

int main() {
    int local[10];
    cout << "in main: " << sizeof(local) << "\n";  // 40 — 10 ints
    showSizes(local);
    return 0;
}
```

**Verified:** `40` in `main`, `4` inside the function. GCC even warns without being asked:

```
warning: 'sizeof' on array function parameter 'arr' will return size of 'int*'
         [-Wsizeof-array-argument]
```

**What happened.** Passing an array to a function **decays** it to a pointer to its first
element. The length is not part of the pointer, so it is simply gone. `void f(int arr[])`
and `void f(int *arr)` are *the same declaration* — the `[]` is documentation, nothing more.

**The three consequences you must internalise:**

1. **Every array parameter needs a length parameter.** `void f(int *arr, int n)`. There is
   no way to recover `n` inside.
2. **The idiom `sizeof(arr)/sizeof(arr[0])` only works in the scope where the array was
   declared.** Inside a function it silently computes `4/4 = 1`.
3. **This is why `std::vector` exists.** It carries its size with it, so `v.size()` works
   everywhere and the whole class of bug disappears.

Decay does *not* happen for `sizeof`, for `&arr` (address of the whole array), or when
binding to a reference-to-array — which is how templates can deduce a length.

**Complexity:** O(1) — compile-time.

---

## 12. Implement `strlen` with pointer arithmetic

```cpp
size_t myStrlen(const char *s) {
    const char *p = s;
    while (*p) ++p;        // walk to the terminating '\0'
    return p - s;          // pointer difference = number of ELEMENTS
}
```

**Verified:** `myStrlen("hello")` returns 5.

**Three things worth noticing:**

1. **`while (*p)` is the sentinel loop.** A C string ends with `'\0'`, whose value is `0`,
   which is `false`. So `while (*p)` reads "until the terminator". Writing
   `while (*p != '\0')` is identical and clearer — prefer it while learning.
2. **`p - s` returns a count of elements, not bytes.** For `char` they coincide (1 byte
   each), which is why this is the cleanest possible demonstration of pointer subtraction.
   On an `int*` the same expression would return 5 for a 20-byte span.
3. **`const char *`** — the function reads and never writes, and says so.

**The length is not stored anywhere.** `strlen` is **O(n)**, every single call, because it
must walk to the sentinel. That's why

```cpp
for (int i = 0; i < strlen(s); ++i)     // O(n^2) — strlen runs EVERY iteration
```

is a genuine performance bug, and

```cpp
for (int i = 0, len = strlen(s); i < len; ++i)   // O(n)
```

is the fix. `std::string::size()` is O(1) because it stores the length — another reason the
C++ type is preferred (`../09_Strings`).

**Complexity:** O(n) time, O(1) space.

---

## 13. Reverse a block in place with two pointers

```cpp
void reverseBlock(int *p, int n) {
    if (!p || n <= 1) return;
    int *left = p;
    int *right = p + n - 1;        // n-1, not n: the LAST element
    while (left < right) {
        int t = *left; *left = *right; *right = t;
        ++left;
        --right;
    }
}
```

**Verified:** `{1,2,3,4,5}` becomes `5 4 3 2 1`.

**This is your first two-pointer algorithm**, and the pattern is everywhere from here on —
palindrome checks, container-with-most-water, three-sum, and every linked-list reversal.

**The three details that make it correct:**

1. **`p + n - 1`**, not `p + n`. `p + n` is one past the end — legal to *form*, undefined to
   *dereference*.
2. **`left < right`**, not `left != right`. With an odd count the two pointers land on the
   same middle element and `<` stops correctly; with `!=` on an even count they cross without
   ever being equal, and the loop runs off both ends.
3. **The `n <= 1` guard.** For `n = 0`, `p + n - 1` is `p - 1` — a pointer before the block,
   which is undefined behaviour merely to compute.

**Dry run** — `{1,2,3,4,5}`:

| left → | right → | after swap |
|---|---|---|
| idx 0 (1) | idx 4 (5) | `5 2 3 4 1` |
| idx 1 (2) | idx 3 (4) | `5 4 3 2 1` |
| idx 2 | idx 2 | `left < right` false → stop |

The middle element of an odd-length block is never touched, which is correct — it's already
in place.

**Complexity:** O(n) time, **O(1) space** — the in-place property is the point.

---

## 14. A function that survives `nullptr`

```cpp
// Returns true and writes the result only if the inputs are usable.
bool safeDivide(const int *a, const int *b, int *result) {
    if (a == nullptr || b == nullptr || result == nullptr) return false;
    if (*b == 0) return false;              // the OTHER precondition
    *result = *a / *b;
    return true;
}

int main() {
    int x = 10, y = 2, out = 0;
    if (safeDivide(&x, &y, &out))       cout << out << "\n";     // 5
    if (!safeDivide(&x, nullptr, &out)) cout << "rejected null\n";
    int zero = 0;
    if (!safeDivide(&x, &zero, &out))   cout << "rejected /0\n";
    return 0;
}
```

**Every function taking a pointer must decide what null means and act on that decision.**
There are only three honest options:

| Policy | When |
|---|---|
| **Reject it** — return an error/`false` | null is a plausible caller mistake |
| **Treat it as "nothing"** — do nothing, return | null is a meaningful "absent" value |
| **Forbid it** — document a precondition, don't check | hot path where the check costs more than it's worth |

The one thing you may not do is *ignore the question* and dereference. That's a crash at
best and silent corruption at worst.

**Note the second guard, `*b == 0`.** Null-safety is not the only precondition — division by
zero is undefined behaviour too (`../01_basics` §7). Checking one and forgetting the other
is a common half-fix.

**If null is never valid, use a reference instead** — `bool safeDivide(const int &a, ...)` —
and the check becomes unnecessary because the language guarantees it. **Choosing the
reference is better than writing the check.**

### A trap specific to out-parameters

Do not read the out-parameter in the **same expression** as the call:

```cpp
cout << safeDivide(&x, &y, &out) << " out=" << out;   // BUG: may print the OLD out
bool ok = safeDivide(&x, &y, &out);                   // correct: the call is sequenced first
cout << ok << " out=" << out;
```

In C++14 the operands of a `<<` chain have **no guaranteed evaluation order**, so `out` can
be read *before* the call that fills it. I hit this while testing this exact function — it
reported `true` alongside a stale `out`, and the function was fine all along. Same family as
`i++ + ++i` (`../01_basics` Q4). **An out-parameter is only valid on the next statement.**

**Complexity:** O(1).

---

# Section 4 — Extra Practice (approach only)

### Reverse a Linked List — LeetCode 206 · **forward reference**
Three pointers — `prev`, `curr`, `next` — walking once: save `next = curr->next`, flip
`curr->next = prev`, then advance `prev = curr`, `curr = next`. Return `prev`. The whole
problem is that you must save `next` *before* you overwrite the link, or you lose the rest
of the list. This is #10's "rearrange pointers, don't move data" at full scale.
**O(n) time, O(1) space.**

### Merge Two Sorted Lists — LeetCode 21 · **forward reference**
Use a **dummy head** node so you never special-case "the result list is still empty", then
a `tail` pointer you keep appending to. Compare heads, attach the smaller, advance. The
dummy-head trick removes about ten lines of null-handling and is worth internalising.
**O(n+m) time, O(1) space.**

### Linked List Cycle — LeetCode 141 · **forward reference**
Floyd's tortoise and hare — **you already wrote this** in `../03_Loops` #8 for Happy Number.
Slow moves one node, fast moves two; they meet iff there's a cycle. Recognising that the
same algorithm applies to a sequence of *numbers* and a sequence of *nodes* is the actual
insight. **O(n) time, O(1) space.**

### Middle of the Linked List — LeetCode 876 · **forward reference**
Same two-pointer setup, different stopping condition: when `fast` reaches the end, `slow` is
at the middle. Also `../17_linked_list/lc876.cpp`. The subtlety is which middle you get for
an even-length list, decided by whether you test `fast && fast->next` or `fast->next &&
fast->next->next`. **O(n) time, O(1) space.**

### Push onto a list via `Node **head` · **forward reference**
`void push(Node **head, int val)` — allocate, set `newNode->next = *head`, then
`*head = newNode`. Taking `Node *head` instead reassigns a local copy and the caller's head
never moves: **exactly the bug from #6**, and the single most common linked-list error.
The C++ alternative is `Node *&head`. **O(1).**

### Dynamically allocate a 2D block, two ways · forward ref to `../08_2d array`
**(a) Array of row pointers:** `int **grid = new int*[rows];` then a `new int[cols]` per row.
Indexing is natural (`grid[i][j]`) but rows are scattered in memory, so cache locality is
poor — and you must `delete[]` every row *before* the outer array.
**(b) One flat block:** `int *grid = new int[rows*cols];` indexed as `grid[i*cols + j]`.
Uglier indexing, one allocation, contiguous and cache-friendly. **(b) is what real numeric
code does**, and what `vector<vector<int>>` fails to give you.

### Implement a `memcpy`-style byte copy
Cast both pointers to `char*` so arithmetic advances one **byte** at a time, then copy `n`
bytes in a loop. The interesting part is that it breaks on **overlapping** ranges — copying
forward into a destination that starts inside the source overwrites bytes not yet read.
That's precisely why `memmove` exists: it detects the overlap and copies backwards when
needed. **O(n) time, O(1) space.**

### Rewrite #3 with `unique_ptr` and `vector`
`vector<int> arr(n);` — sized at runtime, frees itself, knows its own length, and is
exception-safe. Or `unique_ptr<int[]> arr(new int[n]);` if you want raw storage without
manual `delete`. Compare the line counts against #3 and note that **neither version can
leak**, because the destructor runs on every exit path including an exception. This is
**RAII**, and it's the modern C++ answer to everything in #8. **Same complexity, zero
lifetime bugs.**

### Measure `sizeof` for every pointer type
`sizeof(char*)`, `sizeof(int*)`, `sizeof(double*)`, `sizeof(int**)`, `sizeof(void*)` — all
equal on any given build. On this toolchain they are all **4** (`mingw32`, a 32-bit
compiler). Do it on your own machine rather than memorising a number; then you'll answer the
interview question correctly instead of confidently.

---

# Bugs in your current code

Per the repo's convention I have not edited your `.cpp` files. `pointer.cpp` compiles and
runs; `g++ -Wall` reports two warnings, both confirmed.

**Actual program output**, which I captured by running it:

```
0x61ff00        <- cout << &x
0x61ff00        <- cout << p
4               <- cout << *p
65665           <- three prints run together (see below)
```

That last line is `6` + `56` + `65`: `x` after `*p = 6`, then `a`/`b` unchanged by
`swapValue`, then `a`/`b` swapped by `swapReference`. **Your annotations are all correct** —
but the missing separators make the output unreadable, which is the first fix.

### `06_Pointer/pointer.cpp`

| Line | Problem | Fix |
|---|---|---|
| 15 | **`swapReference(int* a, int* b)` is pass-by-*pointer*, not pass-by-reference.** Behaviour is right, the name and comment are wrong — and interviewers ask for this distinction by name | either rename to `swapPointer`, or change the signature to `int &a, int &b` and call it as `swapReference(a, b)` |
| 36-38 | Labelled `// null Pointer`, but `int c; int* ptr = &c;` is **not** a null pointer — it's a valid pointer to an **uninitialised** variable. Two different concepts, and the risky one isn't demonstrated | `int *ptr = nullptr;` and show the `if (ptr)` guard |
| 37 | `-Wall` confirms: *"unused variable 'ptr'"* | use it or drop it |
| 41-42 | `int** ptr2 = &ptr1;` is **syntactically correct** but never used, so the *reason* for double pointers never appears. `-Wall` confirms *"unused variable 'ptr2'"* | print `**ptr2`, or write the out-parameter example from solution #6 |
| 30, 33 | `cout << a << b;` with no separator prints `56` — ambiguous between "5 and 6" and "fifty-six" | `cout << a << " " << b << "\n";` |
| 4-8 | `sumPointer(int a, int b)` takes its arguments **by value**, then takes the addresses of those local copies. It works, but it's equivalent to `return a + b;` — no caller data is reached through the pointers | take `const int *a, const int *b` and call it as `sumPointer(&x, &y)` |
| — | `sumPointer` is **never called** from `main()` | call it |
| — | **No `new` / `delete` anywhere.** Heap allocation, the main practical reason pointers exist, isn't covered | add solution #3 |

### What you got right — keep these habits

- **The core demonstration (lines 20-25) is correct and well annotated.** `&x` and `p`
  printing the same address, `*p` giving the value, and `*p = 6` changing `x` — that is the
  whole concept, correctly shown, and your comments match what I measured.
- **`swapValue` is a genuinely good teaching example.** Writing the version that *doesn't*
  work, and annotating it `//5 6`, is better pedagogy than only showing the working one —
  and the annotation is correct.
- **The double-pointer declaration `int** ptr2 = &ptr1;` is syntactically right.** Many
  people write `int** ptr2 = ptr1;` or `&*ptr1`. You just need to *use* it.
- Separating the file into labelled sections (`//pass by Value`, `//Double Pointer`) makes it
  readable as a lesson. Keep that structure; just make each section runnable.
