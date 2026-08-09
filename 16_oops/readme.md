# 16 — Object-Oriented Programming in C++

This folder is different from every other one in the repo. There are almost no judge problems
for OOP — it is assessed in interviews by **design questions and code reading**, and by whether
you can implement a class correctly rather than whether you can solve a puzzle. So `questions.md`
here is design-and-drill heavy, and `solution.md` keeps more real code than the folders around
it, because in an implementation topic the code *is* the answer.

It is also the last thing you need before `../17_linked_list`: a `Node` is a class, and every
data structure from here to `../28_DSU` is written as one.

---

## 1. The four pillars, stated the way an interviewer wants

| Pillar | One-line definition | Mechanism in C++ |
|---|---|---|
| **Encapsulation** | bundle data with the functions that operate on it, and control access | `private` / `protected` / `public`, getters and setters |
| **Abstraction** | expose *what* something does, hide *how* | pure virtual functions, header/implementation split |
| **Inheritance** | a derived class reuses and extends a base | `class D : public B` |
| **Polymorphism** | one interface, many behaviours | overloading (compile time), `virtual` (run time) |

**Encapsulation vs. abstraction** is the follow-up question, and the distinction is real:
encapsulation is about *access* (making a field private), abstraction is about *interface*
(hiding that a stack is backed by an array rather than a list). You can have one without the
other.

---

## 2. Access specifiers, including through inheritance

| | accessible in the class | in a derived class | outside |
|---|---|---|---|
| `private` | yes | **no** | no |
| `protected` | yes | **yes** | no |
| `public` | yes | yes | yes |

A private member **is inherited** — it occupies space in the derived object — it is simply not
*accessible*. That distinction gets asked.

The inheritance mode caps what the derived class exposes:

| | base `public` becomes | base `protected` becomes |
|---|---|---|
| `class D : public B` | public | protected |
| `class D : protected B` | protected | protected |
| `class D : private B` | private | private |

`class` defaults to `private` inheritance, `struct` to `public`. Always write `public` explicitly.

---

## 3. Constructors

```cpp
class Bike {
public:
    static int count;              // one per CLASS, not per object
    int tyre, engine;

    Bike(int t, int e) : tyre(t), engine(e) { ++count; }   // initialiser list
    Bike(int t) : Bike(t, 150) {}                          // delegating (C++11)
    Bike()      : Bike(17, 150) {}
};
int Bike::count = 0;               // static members need a definition outside the class
```

**Use the initialiser list, not assignment in the body.** Members are constructed in declaration
order *before* the body runs; assigning inside the body constructs them once and then overwrites.
For `const` members and references it is not a style choice — they cannot be assigned at all.

**Delegating constructors are the fix for duplicated setup.** If three constructors each need to
bump a counter or validate an argument, have two of them call the third. Duplicating the work by
hand is how a counter ends up being incremented in only one of them — which is the bug in
`oops2.cpp`.

**Static members belong to the class.** `static int count;` is shared by every object, and needs
a definition at namespace scope (`int Bike::count = 0;`) or you get a linker error.

---

## 4. The Rule of Three — the most-asked C++ design question

**If your class manages a raw resource, the compiler-generated copy is wrong.** The default copy
constructor copies the *pointer*, so two objects end up owning the same memory: modifying one
changes the other, and the second destructor double-frees.

If you write any one of these three, you almost certainly need all three:

1. destructor
2. copy constructor
3. copy assignment operator

```cpp
Buffer& operator=(const Buffer& o) {
    if (this == &o) return *this;          // 1. self-assignment guard
    int* tmp = new int[o.n];               // 2. allocate BEFORE freeing
    for (int i = 0; i < o.n; ++i) tmp[i] = o.data[i];
    delete[] data;                         // 3. only now release the old buffer
    data = tmp; n = o.n;
    return *this;
}
```

All three lines are load-bearing. Without the self-check, `a = a` frees its own buffer and then
copies from freed memory. Allocating before deleting also makes the operation safe if `new`
throws.

> C++11 adds move construction and move assignment, making it the **Rule of Five**. The modern
> advice is the **Rule of Zero**: use `vector`, `string` and smart pointers so you never write
> any of them. Say all three in an interview — it shows you know the history and the current
> practice.

---

## 5. Virtual functions, and the destructor rule

```cpp
class Vehicle { public: virtual void show() { cout << "vehicle"; } };
class Bike : public Vehicle { public: void show() { cout << "bike"; } };

Vehicle* p = new Bike();
p->show();        // "bike"    -- virtual: resolved at RUN time by the object
```

Without `virtual`, the call is resolved at **compile** time from the pointer's declared type and
prints `"vehicle"`. That is *shadowing*, not overriding — and it is what `functionoverriding.cpp`
demonstrates.

**Overloading vs. overriding**, which are constantly confused:

| | Overloading | Overriding |
|---|---|---|
| Where | same class | base and derived |
| Signatures | must **differ** | must be **identical** |
| Resolved | compile time | run time (if `virtual`) |
| Also called | static / early binding | dynamic / late binding |

### The destructor rule

> **A base class with any virtual function needs a virtual destructor.**

Deleting a derived object through a base pointer with a non-virtual destructor is **undefined
behaviour**, and in practice the derived destructor simply never runs:

| | output of `delete basePtr` |
|---|---|
| non-virtual destructor | `~Base` — the derived part leaks |
| `virtual ~Base()` | `~Der ~Base` — correct order |

Both measured. This is the most common real-world C++ memory leak.

### Pure virtual and abstract classes

```cpp
class Shape {
public:
    virtual double area() const = 0;    // pure virtual -> Shape cannot be instantiated
    virtual ~Shape() {}                 // still needs the virtual destructor
};
```

`= 0` makes the class abstract; any derived class must implement `area()` or it stays abstract
too. This is how C++ expresses an interface.

---

## 6. The diamond problem

```
    A
   / \
  B   C
   \ /
    D
```

Without `virtual`, `D` contains **two** separate copies of `A`, and `d.a` is ambiguous — it will
not compile. With `virtual public A` on both `B` and `C`, `D` contains exactly **one** shared `A`.

Measured on this toolchain, using your `diamond.cpp` classes:

```
&d.B::a == &d.C::a          -> same address: ONE shared A
sizeof(D) with virtual      = 24     (includes vtable pointers)
sizeof(D) without virtual   = 20     (two copies of A, no vtable)
```

Note the virtual version is *larger* despite having fewer copies of `A` — virtual bases are
located through pointers stored in the object, which costs more than the 4-byte `int` it saves
here. It pays off when the shared base is big.

---

## Interview Q&A

**Q1. What are the four pillars, and how does C++ implement each?**
See §1. The point of the question is whether you name the *mechanism* — `private` for
encapsulation, pure virtual for abstraction, `: public B` for inheritance, `virtual` for runtime
polymorphism — rather than reciting definitions.

**Q2. Overloading vs. overriding?**
Overloading: same name, **different** parameters, same class, resolved at compile time.
Overriding: same name, **identical** signature, derived class, resolved at run time *only if the
base declares it `virtual`*. Without `virtual` you get shadowing, and calls through a base
pointer go to the base version.

**Q3. Why does a base class need a virtual destructor?**
So that `delete` through a base pointer runs the derived destructor. Without it the behaviour is
undefined and in practice only `~Base` runs, leaking everything the derived class owned. The rule
of thumb: if a class has any virtual function, it is meant to be used polymorphically, so its
destructor must be virtual.

**Q4. Explain the Rule of Three.**
If a class manages a raw resource you must write the destructor, copy constructor and copy
assignment together — the compiler-generated versions do a shallow pointer copy, giving shared
ownership, aliasing bugs and a double free. C++11 extends it to five with move operations; the
Rule of Zero says to avoid the situation by using RAII types instead.

**Q5. What is the diamond problem and how does C++ solve it?**
`D` inheriting from `B` and `C`, both inheriting `A`, gives `D` two copies of `A` and makes `A`'s
members ambiguous. `virtual` inheritance (`class B : virtual public A`) makes them share a single
`A`. The cost is an extra indirection to reach the virtual base.

**Q6. Can a constructor be virtual? Can a destructor be pure virtual?**
No to the first — virtual dispatch needs a vtable pointer, which is set up *by* the constructor,
so the object's type is not established yet. Yes to the second, but you must still provide a body,
because derived destructors call it. It is the idiom for making a class abstract when it has no
other pure virtual function.

**Q7. Why prefer the initialiser list over assigning in the constructor body?**
Members are initialised before the body runs, so assigning in the body means constructing then
overwriting — two operations instead of one, which matters for non-trivial types. For `const`
members, references, and base classes without a default constructor, the initialiser list is the
**only** option.

**Q8. Shallow copy vs. deep copy?**
Shallow copies the pointer (both objects reference the same memory); deep allocates new memory
and copies the contents. The compiler's default copy constructor is shallow — which is exactly
why the Rule of Three exists.

---

## No `readme.md` was present

This folder had no problem list of its own, so nothing needed preserving. `questions.md` is
built from the standard C++ OOP interview set plus implementation drills scoped to `01`–`15`.
