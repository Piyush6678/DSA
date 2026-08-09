# 06 — Pointers

> **Striver A2Z mapping:** not a numbered step — Striver treats pointers as language
> background. But **every** pointer-heavy topic later in this repo depends on it:
> `../17_linked_list`, `../21_tree`, `../22_bst`, `../27_graphs`.
> Prerequisites: `../01_basics` … `../05_Function`.

A pointer is a variable whose value is a **memory address**. That's the whole idea. Every
difficulty people have with pointers comes from the syntax, not the concept.

**This folder has almost no judge problems, and that's expected.** Pointers are machinery,
not a problem category. They pay off in `../17_linked_list`, where every problem is
pointer manipulation. Treat this folder as building the tool, then go use it.

---

## 1. The three operators

```cpp
int x = 4;
int *p = &x;    // & = "address of"   -> p now holds x's address
cout << p;      // 0x61ff00  (an address)
cout << *p;     // 4         (* = "value at that address" — dereference)
*p = 6;         // writes THROUGH the pointer
cout << x;      // 6         — x itself changed
```

Your `pointer.cpp:20-25` demonstrates exactly this, and I ran it to confirm: `&x` and `p`
print the same address, `*p` prints 4, and after `*p = 6` the variable `x` is 6.

**`*` means two different things**, which is the main source of confusion:

```cpp
int *p = &x;   // in a DECLARATION: "p is a pointer to int"
*p = 6;        // in an EXPRESSION:  "the thing p points at"
```

**Declaration style:** `int* p, q;` declares `p` as a pointer and **`q` as a plain int** —
the `*` binds to the declarator, not the type. Either write `int *p, *q;` or declare one
per line. This is a real bug source, not a style quibble.

---

## 2. Initialise your pointers

```cpp
int *p;            // WILD pointer — holds garbage. Dereferencing is undefined behaviour.
int *p = nullptr;  // safe: explicitly "points at nothing"
int *p = &x;       // safe: points at something real
```

**`nullptr` (C++11) is the one to use.** `NULL` is a macro for `0` and can pick the wrong
overload between `f(int)` and `f(char*)`; `nullptr` has its own type and can't. This
toolchain supports `nullptr` — it defaults to `gnu++14`.

**Always check before dereferencing** a pointer that could be null:

```cpp
if (p != nullptr) { cout << *p; }
if (p) { cout << *p; }              // idiomatic shorthand
```

`pointer.cpp:36-38` is labelled `// null Pointer` but does `int c; int* ptr = &c;` — that
is **not** a null pointer. It's a valid pointer to an *uninitialised variable*, which is a
different (and also risky) thing. See `solution.md`.

---

## 3. How big is a pointer?

**Measured on this repo's toolchain** (`g++ -dumpmachine` → `mingw32`, a 32-bit compiler):

```
sizeof(int*)    = 4
sizeof(char*)   = 4
sizeof(double*) = 4
sizeof(int**)   = 4
```

**Every pointer type is the same size**, because every pointer holds the same thing: one
address. `sizeof(char*) == sizeof(double*)` even though `char` is 1 byte and `double` is 8 —
the *pointee* size differs, the *address* size doesn't.

The value itself is **target-dependent**: 4 bytes on a 32-bit build, 8 on a 64-bit build.
If asked in an interview, say "it depends on the architecture — and all pointer types are
the same size on any given one", then measure rather than recite.

---

## 4. Pointer arithmetic is scaled

```cpp
int *p = arr;
p + 1        // advances by sizeof(int) = 4 BYTES, not 1 byte
```

Adding 1 to a pointer moves it to the **next object of that type**, not the next byte. The
compiler multiplies by `sizeof(T)` for you. So `p + 1` on an `int*` moves 4 bytes; on a
`double*` it would move 8.

What's defined:

| Operation | Meaning |
|---|---|
| `p + n`, `p - n` | move `n` elements |
| `++p`, `--p` | move one element |
| `p2 - p1` | **number of elements** between them |
| `p1 < p2`, `p1 == p2` | ordering / identity |
| `p1 + p2` | **not allowed** — adding two addresses is meaningless |

Traversal without indexing:

```cpp
long long sum = 0;
for (const int *q = p, *end = p + n; q < end; ++q) sum += *q;
```

---

## 5. Pointers and arrays

**An array name decays to a pointer to its first element** in almost every context.

```cpp
int arr[10];
int *p = arr;        // no & needed — arr already converts to int*
arr[i] == *(arr + i) // these are the SAME expression, by definition
```

Because indexing is *defined* as `*(a + i)`, and addition commutes, `arr[2]` and `2[arr]`
are both legal and equal. I verified both print `3` for `arr = {1,2,3,4,5}`. Never write the
second — but knowing why it works means you understand what `[]` really is.

**Where decay bites — `sizeof`:**

```cpp
int local[10];
sizeof(local);              // 40  — the whole array
void f(int arr[]) {
    sizeof(arr);            // 4   — it's just an int*, the size is GONE
}
```

**Verified**: 40 in `main`, 4 inside the function. GCC even warns:
*"'sizeof' on array function parameter 'arr' will return size of 'int\*'"*.

This is why **every C-style array parameter needs a separate length argument**. A function
receiving `int arr[]` cannot know how long it is. `std::vector` (from `../07_Array`) exists
largely to fix this.

---

## 6. Pointer vs reference

```cpp
void byPtr(int *a) { if (a) *a = 5; }    // may be null; needs *; call as byPtr(&x)
void byRef(int &a) { a = 5; }            // never null; no *; call as byRef(x)
```

| | Pointer | Reference |
|---|---|---|
| Can be null | **yes** | no |
| Must be initialised | no | **yes** |
| Can be reseated | yes | **no** — binds once, forever |
| Needs `*` to use | yes | no |
| Arithmetic | yes | no |
| Call site shows mutation | `f(&x)` — yes | `f(x)` — no |

**Use a reference by default. Use a pointer when "nothing" is a valid value** — that's
exactly what `nullptr` communicates — or when you need to reseat it (walking a linked list).

`pointer.cpp:15` names its function `swapReference` but takes `int* a, int* b`. **That is
pass-by-pointer.** It works correctly; the name is wrong, and it's a distinction
interviewers ask about directly.

---

## 7. `const` and pointers — read right to left

**Verified by compiling each case:**

```cpp
const int *p;          // pointer to const int
int *const p = &x;     // const pointer to int
const int *const p=&x; // const pointer to const int
```

| Declaration | Change `*p`? | Change `p`? |
|---|---|---|
| `const int *p` | **error** | ok |
| `int *const p` | ok | **error** |
| `const int *const p` | **error** | **error** |

**The trick: read the declaration right to left.** `int * const p` → "p is a `const`
pointer to `int`". `const int * p` → "p is a pointer to `int` that is `const`".

**Rule of thumb:** `const` *before* the `*` protects the **data**; `const` *after* the `*`
protects the **pointer**.

The one you'll use constantly is the first: `void f(const int *arr, int n)` promises the
function won't modify the caller's data, and the compiler enforces it.

---

## 8. Double pointers

A pointer to a pointer — `int **pp`.

```cpp
int x = 5;
int *p = &x;
int **pp = &p;

*pp    // is p    (an int*)
**pp   // is x    (an int)
```

**Why you'd want one.** To modify a *pointer* inside a function, you need the pointer's
address — exactly like modifying an `int` requires the `int`'s address:

```cpp
void allocate(int **out, int n) { *out = new int[n]; }   // changes the CALLER's pointer

int *data = nullptr;
allocate(&data, 4);        // data now points at the new block
```

Without the extra `*`, the function would modify its own copy and the caller's pointer would
stay null. `pointer.cpp:41-42` declares `int** ptr2 = &ptr1;` correctly, but never uses it —
so the *reason* for double pointers never appears.

*(In C++ you'd usually take `int*&` — a reference to a pointer — instead. Same effect,
cleaner call site: `allocate(data, 4);`)*

---

## 9. Stack vs heap, `new` and `delete`

| | Stack | Heap (free store) |
|---|---|---|
| Allocated by | declaring a local | `new` |
| Freed by | going out of scope — **automatic** | `delete` — **you must** |
| Size | small (~1 MB) | large (limited by RAM) |
| Size known at | compile time | **runtime** |
| Speed | very fast | slower |

```cpp
int *p   = new int(7);      // single int, initialised to 7
int *arr = new int[n];      // n ints — n can be a RUNTIME value

delete   p;                 // matches new
delete[] arr;               // matches new[]  — the [] is NOT optional
p = nullptr;                // avoid a dangling pointer
```

**`new[]` must be paired with `delete[]`.** Mismatching them is undefined behaviour.

**The three classic pointer bugs:**

```cpp
// 1. MEMORY LEAK — allocated, never freed
int *p = new int[100];
p = nullptr;                  // the block is now unreachable and unfreeable

// 2. DANGLING POINTER — freed, still used
int *p = new int(5);
delete p;
cout << *p;                   // undefined behaviour

// 3. DOUBLE FREE — freed twice
delete p;
delete p;                     // undefined behaviour
```

**The habit that prevents #2 and #3:** set the pointer to `nullptr` immediately after
`delete`. Deleting a null pointer is explicitly safe and does nothing.

*(Modern C++ answers all of this with `std::unique_ptr` / `std::shared_ptr` and
`std::vector`, which free automatically. Worth naming in an interview even though this repo
works at the raw level.)*

---

## 10. Function pointers

A pointer can point at code, not just data:

```cpp
int cmpAsc(int a, int b)  { return a - b; }
int cmpDesc(int a, int b) { return b - a; }

int pick(int a, int b, int (*cmp)(int, int)) {   // takes a FUNCTION as a parameter
    return cmp(a, b) <= 0 ? a : b;
}
pick(3, 9, cmpAsc);    // 3
pick(3, 9, cmpDesc);   // 9
```

**Verified.** This is how `qsort` takes a comparator, and it's the ancestor of the lambdas
you'll pass to `sort` in `../12_sorting`. The declaration `int (*cmp)(int,int)` needs the
parentheses around `*cmp` — without them, `int *cmp(int,int)` declares a *function returning
`int*`*.

---

## Interview Q&A

### Q1. What's the difference between a pointer and a reference, and which should you use?

A pointer is a **variable holding an address**; a reference is an **alias** — another name
for an existing object.

| | Pointer | Reference |
|---|---|---|
| Can be null | yes | **no** |
| Must be initialised at declaration | no | **yes** |
| Can be reseated to another object | yes | **no** |
| Needs dereferencing (`*`) | yes | no |
| Supports arithmetic | yes | no |
| Has its own address/size | yes | typically no storage of its own |

**Default to a reference.** It cannot be null, so a function taking `T&` needs no null
check, and the compiler guarantees the caller passed something real. That eliminates an
entire bug category.

**Reach for a pointer when you need what a reference forbids:**
- **"No object" is a valid state** — `nullptr` expresses "not found", "end of list", "no
  parent". This is why every linked list and tree in this repo uses pointers.
- **You need to reseat it** — walking a list requires `curr = curr->next`, which a reference
  cannot do.
- **You need arithmetic** — traversing a raw buffer.

The follow-up worth volunteering: for parameters, `const T&` is the default for class types
(no copy, read-only enforced), `T&` for out-parameters, and `T*` only when null is
meaningful. Some style guides mandate pointers for out-parameters purely so the call site
reads `f(&x)` and visibly signals mutation.

---

### Q2. What does `sizeof(int*)` return, and why are all pointer types the same size?

It returns the size of an **address** on the target architecture — 4 bytes on a 32-bit
build, 8 on a 64-bit build. On this repo's toolchain I measured **4**, because
`g++ -dumpmachine` reports `mingw32`, a 32-bit compiler, despite the OS being 64-bit.

**All pointer types are the same size** — `sizeof(char*) == sizeof(double*) ==
sizeof(int**)` — because a pointer stores one thing: a memory address. The size of the
*pointee* determines what dereferencing reads and how far `p+1` moves, not how big the
pointer is.

The correct interview answer is therefore *"it depends on the target, and it's the same for
every pointer type"* rather than a number. A candidate who says "8" has memorised one
platform.

Two related facts that make a stronger answer:
- **Function pointers** are not guaranteed to be the same size as data pointers, though they
  are on mainstream platforms.
- **Pointers to members** are typically *larger*, because they may need to encode an offset
  plus virtual dispatch information.

---

### Q3. Explain `const int*`, `int* const`, and `const int* const`.

**Read the declaration right to left.**

| Declaration | Read as | `*p = 6;` | `p = &y;` |
|---|---|---|---|
| `const int *p` | pointer to const int | **error** | ok |
| `int *const p` | const pointer to int | ok | **error** |
| `const int *const p` | const pointer to const int | **error** | **error** |

I verified all six cases by compiling them; the table is measured, not recalled.

**The rule of thumb:** `const` **before** the `*` protects the **pointed-to data**; `const`
**after** the `*` protects the **pointer itself**.

`const int *p` and `int const *p` are identical — both put `const` before the `*`. The
second form is arguably more consistent with the right-to-left reading, but the first is far
more common.

**Where it matters in practice:** `void process(const int *data, int n)` is a contract that
the function will not modify the caller's buffer, enforced at compile time. That's the
version you should write by default for read-only parameters — it documents intent *and*
lets the compiler catch violations. `int *const` is rare by comparison.

---

### Q4. What are a memory leak, a dangling pointer, and a double free — and how do you avoid all three?

- **Memory leak** — heap memory allocated with `new` and never `delete`d, and the last
  pointer to it is lost. The memory is unreachable and unreleasable until the process exits.
  In a long-running service, leaks accumulate until it's killed.
- **Dangling pointer** — a pointer still holding an address after that memory has been
  freed (or after the object went out of scope). Dereferencing it is undefined behaviour:
  it may return stale data, may crash, may silently corrupt another allocation.
- **Double free** — calling `delete` twice on the same address. Undefined behaviour, and it
  typically corrupts the allocator's internal structures, so the crash appears somewhere
  unrelated.

**Prevention, in increasing order of effectiveness:**

1. **Pair every `new` with exactly one `delete`, and `new[]` with `delete[]`.** Mismatching
   the array forms is itself undefined behaviour.
2. **Set the pointer to `nullptr` right after deleting.** `delete nullptr;` is explicitly
   defined as a no-op, so this kills both the dangling-pointer and double-free classes in
   one line.
3. **Don't manage raw memory at all.** Use `std::vector` instead of `new[]`, and
   `std::unique_ptr` / `std::shared_ptr` instead of `new`. These free in their destructors,
   so the memory is released even if an exception unwinds the stack — which manual
   `delete` does not survive.

Point 3 is the answer a senior interviewer is listening for: **RAII**, tying a resource's
lifetime to an object's scope. Manual `new`/`delete` in modern C++ is a code smell outside
of implementing a container.

---

### Q5. Why does `p + 1` not move by one byte, and what is array decay?

**Pointer arithmetic is scaled by `sizeof(T)`.** `p + 1` means "the next *object* of type
`T`", so on an `int*` it advances 4 bytes and on a `double*` 8. The compiler inserts the
multiplication. This is what makes `arr[i]` and `*(arr + i)` equivalent — indexing *is*
scaled pointer arithmetic, which is also why `p2 - p1` yields a count of **elements**, not
bytes.

**Array decay** is the implicit conversion of an array to a pointer to its first element,
which happens in almost every context — including when an array is passed to a function.

The consequence people get caught by is `sizeof`:

```cpp
int local[10];
sizeof(local);          // 40 — the array
void f(int arr[]) { sizeof(arr); }   // 4 — it's an int*, the length is gone
```

I measured both. GCC even warns: *"'sizeof' on array function parameter will return size of
'int\*'"*.

So **`void f(int arr[])` and `void f(int *arr)` are the same declaration** — the `[]` is
cosmetic, and the function cannot know the length. That is why C-style array parameters
always need an accompanying size argument, and why `std::vector` (which carries its own
size) is preferred everywhere it can be.

Decay does *not* happen for `sizeof`, `&` (taking the address of the whole array), or when
binding to a reference-to-array — which is how templates can deduce an array's length.

---

### Q6. When do you actually need a double pointer?

When you need to modify a **pointer** owned by the caller. Modifying an `int` inside a
function requires the `int`'s address (`int*`); by the same logic, modifying an `int*`
requires that pointer's address — an `int**`.

```cpp
void allocate(int **out, int n) { *out = new int[n]; }

int *data = nullptr;
allocate(&data, 4);        // data now points at the block
```

Without the second `*`, the function would receive a *copy* of the pointer, reassign the
copy, and the caller's `data` would still be null — the identical failure mode as
pass-by-value swap.

The three places it shows up for real:
- **Out-parameters that allocate** — the C idiom, and how many C APIs return buffers.
- **Modifying a linked list's head** — `void push(Node **head, int val)`. If you take
  `Node *head` and reassign it, the caller's head pointer never changes. This is *the*
  classic linked-list bug (`../17_linked_list`).
- **Arrays of pointers** — `char **argv`, or a dynamically allocated 2D array as an array of
  row pointers.

**In C++ prefer a reference to a pointer** — `void allocate(int *&out, int n)` — which does
the same job with a cleaner call site (`allocate(data, 4)`) and no risk of passing a null
`out`. The double pointer is the C way; know it, because you'll read it constantly.

---

## Common mistakes checklist

- [ ] Every pointer initialised — to `&something` or to `nullptr`
- [ ] Null-checked before every dereference that could be null
- [ ] `int *p, *q;` — not `int* p, q;`
- [ ] `delete[]` for `new[]`, `delete` for `new`
- [ ] Pointer set to `nullptr` immediately after `delete`
- [ ] Array length passed alongside any array parameter
- [ ] `const T*` for read-only parameters
- [ ] `**` (or `*&`) when a function must modify the caller's pointer
- [ ] Don't return the address of a local variable — it dangles the instant you return

---

## Next

`questions.md` → `solution.md` → `../07_Array`, then `../17_linked_list`, where all of this
finally earns its keep.
