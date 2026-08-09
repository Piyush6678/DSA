# 16 — OOP: Practice Questions

> **Why 16, and why they are mostly not judge problems.** OOP is the one topic in this repo that
> interviewers do not test with algorithm puzzles. They test it three ways: **"explain X"**
> (virtual destructors, Rule of Three, the diamond), **"what does this print"** (code reading,
> where the answer turns on static vs. dynamic binding), and **"design a class for Y"**. So this
> set is deliberately drill- and design-heavy, like `../06_Pointer` — and for the same reason:
> OOP is machinery, and the problems that exercise it are the data structures in `../17` through
> `../28`. Sixteen covers the six mechanisms that recur (access control, constructors, the Rule of
> Three, virtual dispatch, abstract interfaces, operator overloading) plus the design questions
> that get asked by name. Padding with LeetCode problems that merely *use* a class would teach
> nothing about OOP.

**Platform note.** Most entries here are **Drills** — write them, compile them, predict the
output before running. That is not a cop-out: predicting output is exactly how these are tested.
Where a real judge problem exists it is named.

**Scope note.** Everything needs only `01`–`15`. Two entries are marked **`[fwd]`** — they are
the design questions whose natural answers are the data structures in later folders.

---

## Section 1 — Must Do

| # | Topic | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Class with private data + getters/setters | Easy | *Drill* | encapsulation; `intro.cpp:54` |
| 2 | Constructors: default, parameterised, delegating, initialiser list | Easy | *Drill* | `oops2.cpp` — **your counter bug lives here** |
| 3 | Static members and a working object counter | Easy | *Drill* | `oops2.cpp:6` |
| 4 | Destructor order and when it fires | Easy | *Drill* | `oops2.cpp:31` |
| 5 | **Rule of Three** — a class owning a raw array | **Medium** | *Drill* | the most-asked C++ design question |
| 6 | Shallow vs. deep copy — demonstrate the double free | **Medium** | *Drill* | why #5 exists |
| 7 | Virtual functions: predict the output | **Medium** | *Drill* | `virtual.cpp`, `functionoverriding.cpp` |
| 8 | **Virtual destructor** — prove the leak | **Medium** | *Drill* | the most common real-world C++ leak |

**Why these eight.** #1–#4 are the mechanics, and #2/#3 together are where your `oops2.cpp` goes
wrong — three constructors, only one of which touches the counter. Fixing it with a **delegating
constructor** rather than by copy-pasting the increment is the lesson.

**#5 and #6 are a pair and the highest-value entries in the folder.** Do #6 first: write a class
holding a `new int[]`, copy it, modify the copy, watch the original change, then watch the
program crash on double-free. Then write #5 to fix it. Being able to *cause* the bug is what
makes the Rule of Three memorable rather than a recited list.

**#7 and #8 are the same pair for `virtual`.** #7 is pure code reading — your two files already
demonstrate both sides (one virtual, one not) and one of the comments is wrong, which makes it a
better exercise than it looks. #8 is the destructor rule, and you should verify it by counting
destructor messages, not by trusting the explanation.

---

## Section 2 — Important

| # | Topic | Difficulty | Platform | Where |
|---|---|---|---|---|
| 9 | Abstract class with a pure virtual method | **Medium** | *Drill* | `Shape` → `Circle` / `Rect` |
| 10 | Access specifiers through inheritance modes | **Medium** | *Drill* | `inheritance.cpp:24` |
| 11 | The diamond problem, with and without `virtual` | **Medium** | *Drill* | `diamond.cpp` — **your comment is backwards** |
| 12 | Function overloading and its resolution rules | Easy | *Drill* | `functionoverloading.cpp` |
| 13 | Operator overloading: `+`, `==`, `<<` | **Medium** | *Drill* | needed for `sort` on custom types |
| 14 | `this`, and returning `*this` for chaining | Easy | *Drill* | `intro.cpp:40` uses `this` correctly |

**Why these six.** #9 is how C++ expresses an interface, and the polymorphic-container pattern —
`vector<Shape*>` where each element does its own thing — is what runtime polymorphism is *for*.
#10 is a table you should be able to reconstruct rather than memorise; your `class A` with
`a_private`/`a_protected`/`a_public` is already the right experiment, so finish it by trying to
touch each one from `B`.

**#11 is worth real time because your file gets it backwards.** The comment says virtual
inheritance gives two copies of `A`; it gives exactly one — that is the entire purpose. Prove it
by comparing `&d.B::a` with `&d.C::a`. #13 matters practically: without `operator<` your class
cannot be sorted, and a comparator that is not a strict weak ordering can crash `std::sort`
(`../12_sorting` §6).

---

## Section 3 — Good to Know

| # | Topic | Difficulty | Platform | Where |
|---|---|---|---|---|
| 15 | Design an LRU Cache | **Medium** | **LeetCode 146** | `lru-cache` `[fwd]` — the classic OOP design question |
| 16 | Design a Stack / Queue class | **Medium** | GFG | *"Implement Stack using array"* `[fwd]` — `../18_stack`, `../20_queu` |

**Why these two.** These are the only genuine judge problems in the folder, and both are *design*
questions where the class boundary is the answer. **#15 is asked constantly** — the insight is
that no single container gives you O(1) lookup *and* O(1) recency reordering, so you compose two
(a hash map plus a doubly linked list) and the class exists to keep them in sync. It needs
`../17_linked_list` and `../24_maps`, so treat it as a target to come back to; reading the design
now is still worth it. #16 is the same idea in miniature and you will write it properly in
`../18_stack`.

---

## Section 4 — Extra Practice

| Topic | Difficulty | Note |
|---|---|---|
| `const` member functions and `const` objects | **Medium** | which methods can a `const Foo&` call? |
| Copy constructor vs. assignment — which fires when | **Medium** | `Foo b = a;` is construction, `b = a;` is assignment |
| Friend functions and classes | Easy | when breaking encapsulation is justified |
| Virtual function table — what is stored where | **Medium** | why `sizeof` grows when you add `virtual` |
| Object slicing | **Medium** | pass a `Derived` **by value** as `Base` and watch it lose itself |
| Constructor/destructor call order under inheritance | Easy | base ctor first, base dtor **last** |
| Can a constructor be virtual? Why not? | Easy | the vtable is set up *by* the constructor |
| Pure virtual destructor with a body | **Medium** | how to make a class abstract with no other method |
| `struct` vs `class` in C++ | Easy | one word of difference — default access |
| Multiple inheritance without a diamond | Easy | mixing two unrelated bases |
| `static` local variable inside a member function | Easy | cf. `../05_Function` §1 #14 |
| Rule of Five: move ctor and move assignment | **Hard** | C++11; note the toolchain here is C++14 |
| Design: parking lot / deck of cards | **Medium** | classic open-ended design interview |
| Implement `Node` for a linked list | Easy | *Drill* — `../17_linked_list/nodeClass .cpp`, and the next folder |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8
Section 2   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13  [ ] 14
Section 3   [ ] 15  [ ] 16
Section 4   [ ] ______ / 14
```
