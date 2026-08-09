# 06 — Pointers: Practice Questions

> **Why 14 ranked problems, and why most are drills.** Pointers are *machinery*, not a
> problem category — there is no "pointer section" on LeetCode, because the problems that
> genuinely exercise pointers are linked lists and trees, which arrive in `../17_linked_list`
> and `../21_tree`. So the honest count is driven by distinct mechanisms, not by available
> judge problems: dereference-and-modify, the three parameter modes, pointer arithmetic,
> array decay, `const` placement, double pointers, heap allocation, the three lifetime bugs,
> function pointers, and null safety. That's 14. Padding it with linked-list problems you
> can't solve yet would be dishonest; they're listed in Section 4 as forward references.

**Platform note.** Items marked *Drill* have no judge equivalent — **write and run them
anyway**, because the compiler is the only thing that will tell you whether you understood
`const` placement or array decay. LeetCode numbers are exact where given.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Pointer fundamentals: declare, dereference, modify a variable through a pointer | Easy | *Drill* | `pointer.cpp:20-25` |
| 2 | Swap two numbers: by value vs by pointer vs by reference — and explain why one fails | Easy | *Drill* | `pointer.cpp:10-18` |
| 3 | Dynamically allocate an array of `n` ints, fill it, sum it, free it | Easy | *Drill* | `new[]` / `delete[]` |
| 4 | Traverse a block using **pointer arithmetic only** — no `[]` anywhere | Easy | *Drill* | — |
| 5 | Write the same function twice: once taking `int*`, once taking `int&` | Easy | *Drill* | — |

**Why these five.** #1 is the entire concept in six lines; if `*p = 6` changing `x` doesn't
feel obvious, nothing later will. **#2 is the most important problem in the folder** — it's
the reason the three parameter modes exist, and your `pointer.cpp` labels the pointer
version "pass by reference", which is the exact confusion this problem cures. #3 is the
first time size is a *runtime* value, which is what the heap is for, and it forces you to
pair `new[]` with `delete[]`. #4 makes you feel that `p+1` moves `sizeof(int)` bytes, not
one — after which `arr[i] == *(arr+i)` stops being a factoid. #5 puts the two mechanisms
side by side so the trade-off table in `readme.md` §6 becomes something you've experienced.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 6 | Double pointer: a function that allocates and hands the block back via an out-parameter | **Medium** | *Drill* | `pointer.cpp:41-42` declares one but never uses it |
| 7 | `const` placement: predict which of six statements compile, then check | **Medium** | *Drill* | — |
| 8 | Spot the bug: memory leak, dangling pointer, double free | **Medium** | *Drill* | — |
| 9 | Function pointers: pass a comparator into a function | **Medium** | *Drill* | ancestor of `sort`'s comparator |
| 10 | Swap two **pointers** (not the values they point to) | Easy | *Drill* | — |

**Why these five.** #6 is the only way the *purpose* of `int**` lands — your file declares
one correctly but never uses it, so the "why" is missing. #7 is worth doing as a written
prediction *before* compiling; getting 6/6 means you can read declarations, and that skill
transfers directly to reading real headers. #8 is the highest-frequency C++ interview topic
in this whole folder — every backend interviewer asks about leaks and dangling pointers.
#9 is where you learn that a function has an address too, which demystifies the comparators
in `../12_sorting`. #10 looks like #2 and is different in a way that catches people: you're
changing the pointers, not the pointees, so nothing about `m` and `n` changes.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Array decay: print `sizeof(arr)` inside `main` and inside a function | Easy | *Drill* | — |
| 12 | Implement `strlen` using pointer arithmetic | Easy | GFG | *"Implement strlen"* — partial forward ref to `../09_Strings` |
| 13 | Reverse a dynamically allocated block in place, using two pointers | Easy | *Drill* | the two-pointer pattern, in miniature |
| 14 | Write a function that survives being passed `nullptr` | Easy | *Drill* | defensive-programming habit |

**Why these four.** #11 produces a number that surprises everyone the first time (40 becomes
4) and permanently explains why array parameters need a length argument. #12 is the classic
"walk until the sentinel" loop, and `return p - s;` is a satisfying use of pointer
subtraction returning a *count*. #13 is your first **two-pointer** algorithm — the technique
that dominates `../07_Array` and `../14_Sliding window` — in the simplest possible setting.
#14 is a habit rather than an algorithm: every function taking a pointer should decide
explicitly what null means, and say so.

---

## Section 4 — Extra Practice

The linked-list problems are where pointers actually pay off. They're listed here so you
know what you're building toward — **come back after `../17_linked_list`**.

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Reverse a Linked List | Easy | **LeetCode 206** | **forward reference** — three pointers, the canonical exercise |
| Merge Two Sorted Lists | Easy | **LeetCode 21** | **forward reference** — pointer surgery with a dummy head |
| Linked List Cycle | Easy | **LeetCode 141** | **forward reference** — Floyd's, which you already met in `../03_Loops` #8 |
| Middle of the Linked List | Easy | **LeetCode 876** | **forward reference** — also `../17_linked_list/lc876.cpp` |
| Push onto a list via `Node **head` | **Medium** | *Drill* | **forward reference** — why the head pointer needs a double pointer |
| Dynamically allocate a 2D block, two ways | **Medium** | *Drill* | array-of-row-pointers vs one flat block with `i*cols + j`; forward ref to `../08_2d array` |
| Implement a `memcpy`-style byte copy | **Medium** | *Drill* | `char*` casting; why overlapping ranges need `memmove` |
| Rewrite #3 using `std::unique_ptr` and `std::vector` | **Medium** | *Drill* | RAII — what modern C++ does instead of `new`/`delete` |
| Measure `sizeof` for every pointer type on your machine | Easy | *Drill* | confirms they're all equal, and tells you your build's word size |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5
Section 2   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 3   [ ] 11  [ ] 12  [ ] 13  [ ] 14
Section 4   [ ] ______ / 9
```
