# 16 — OOP: Solutions

**This folder keeps more code than its neighbours**, and deliberately: the whole topic is
*implementation*, so the code is the answer rather than an illustration of it. Where an entry is
a design or explain-it question, the answer is prose.

**Everything shown as code was compiled with `g++ -std=gnu++14` and executed — 11 assertions,
all passing.** Measured outputs are quoted.

---

# Section 1 — Must Do

## 1. Encapsulation — private data with accessors

**Approach.** Data members `private`, access through `public` methods. The setters are where you
put validation, which is the actual reason to bother.

```
class Student:
    private:  name, roll, standard
    public:   setRoll(r):  if r <= 0 -> reject;  else this->roll = r
              getRoll():   return roll
```

**Key insight.** A getter/setter pair that does nothing but read and write is no better than a
public field — the value appears the moment there is an **invariant** to protect (a roll number
must be positive, a percentage must be 0–100). Say that in an interview; reciting "encapsulation
means private data" without it is the weak answer.

**Your `student` class in `intro.cpp:54` has the structure right** — private members, public
`set*`/`get*`, `this->name = name` to disambiguate the shadowed parameter.

---

## 2–3. Constructors and a static counter — **implementation, code given**

```cpp
class Bike {
public:
    static int count;                                       // one per CLASS
    int tyre, engine;

    Bike(int t, int e) : tyre(t), engine(e) { ++count; }    // the ONE real constructor
    Bike(int t) : Bike(t, 150) {}                           // delegates
    Bike()      : Bike(17, 150) {}                          // delegates
};
int Bike::count = 0;                                        // definition, outside the class
```

**Key insight — delegate, do not duplicate.** Every constructor funnels into the one that does
the work, so shared setup (here, `++count`) happens exactly once and cannot be forgotten in a
variant. **This is precisely the bug in `oops2.cpp`**: three constructors, only one increments
the counter, and `main` uses one of the other two.

**The initialiser list, not the body.** `: tyre(t), engine(e)` initialises; `{ tyre = t; }`
default-constructs and then assigns. For `int` the difference is invisible; for a `const` member,
a reference, or a base class without a default constructor, the list is the only legal option.

**Static members need a definition at namespace scope.** Declaring `static int count;` inside the
class only declares it — without `int Bike::count = 0;` you get an undefined-reference link error.

**Verified:** with all four construction forms (`Bike(18)`, `Bike(17)`, `Bike(19,200)`, `Bike()`),
`Bike::count` is **4**.

> Delegating constructors are C++11 and compile fine here. If you want to stay pre-C++11, use a
> private `init(t, e)` helper called from each constructor — same principle.

---

## 4. Destructors — when they fire

**Approach.** A destructor runs when an object goes out of scope (automatic storage) or is
`delete`d (dynamic). Order is **reverse of construction**, and under inheritance the derived
destructor runs before the base.

```
{ Bike a(1); Bike b(2); }        -> ~b then ~a
Base* p = new Derived(); delete p;   -> ~Derived then ~Base   (IF the dtor is virtual -- see #8)
```

**Key insight.** Objects created with `new` are **not** destroyed automatically — `delete` is
what calls the destructor. Every `new` in `intro.cpp` and `virtual.cpp` leaks for this reason.
It is harmless in a scratch file and disqualifying in an interview if you do not mention it.

Add `endl` to destructor messages, or three destructors print as one run-on line — which is what
`oops2.cpp` currently does.

---

## 5–6. Rule of Three — **implementation, code given**

**First cause the bug (#6).** A class holding `int* data` with no copy constructor: copy it,
modify the copy, and the original changes too — both objects hold the same pointer. Then both
destructors run and the second `delete[]` is a **double free**.

**Then fix it (#5):**

```cpp
class Buffer {
    int* data; int n;
public:
    Buffer(int n_) : data(new int[n_]), n(n_) { }

    Buffer(const Buffer& o) : data(new int[o.n]), n(o.n) {     // 1. copy CONSTRUCTOR
        for (int i = 0; i < n; ++i) data[i] = o.data[i];
    }

    Buffer& operator=(const Buffer& o) {                       // 2. copy ASSIGNMENT
        if (this == &o) return *this;                          //    self-assignment guard
        int* tmp = new int[o.n];                               //    allocate BEFORE freeing
        for (int i = 0; i < o.n; ++i) tmp[i] = o.data[i];
        delete[] data;
        data = tmp; n = o.n;
        return *this;                                          //    return *this for chaining
    }

    ~Buffer() { delete[] data; }                               // 3. DESTRUCTOR
};
```

**Key insight — three details in `operator=`, all load-bearing:**

1. **`if (this == &o) return *this;`** — without it, `a = a` deletes its own buffer and then
   copies from freed memory.
2. **Allocate the new buffer before deleting the old one.** If `new` throws, the object is still
   intact; delete-then-allocate leaves a dangling pointer.
3. **Return `Buffer&`, not `void` or by value** — that is what makes `a = b = c` work, and
   returning by value would copy needlessly.

`delete[]`, not `delete`, for array allocations — mismatching them is undefined behaviour.

**Verified:** copying a `Buffer` and mutating the copy leaves the original unchanged (deep copy
confirmed); assignment adopts the source's size; `a = a` survives with its contents intact.

> **Rule of Five / Rule of Zero.** C++11 adds move construction and move assignment. The modern
> answer is to not write any of them: hold a `vector<int>` instead of `int*` and the compiler's
> defaults are all correct. Name all three positions in an interview.

---

## 7. Virtual functions — predict the output

**The rule.** Without `virtual`, a call through a pointer is resolved from the **pointer's
declared type** at compile time. With `virtual`, it is resolved from the **object's actual type**
at run time.

| Code | Prints | Why |
|---|---|---|
| `A* p = &b; p->show();` — `show()` **not** virtual | the **base** version | static binding on `A*` |
| `Vehicle* p = new Bike; p->show();` — `show()` **is** virtual | the **derived** version | dynamic binding |
| `Vehicle* p = new Vehicle; p->show();` — virtual | the **base** version | the object really is a `Vehicle` |
| `b.A::show();` | the base version | explicit qualification overrides dispatch |

**Both your files are correct code.** `functionoverriding.cpp` prints `mai a ka show hu` —
verified, and matching your comment — because `show()` is not virtual there. `virtual.cpp` prints
`Bike ka show` then `mai vehicle ka show hu` — the second is where your comment is wrong; see the
bugs section.

**The `override` keyword** (C++11) makes the compiler check that you really are overriding
something. A typo in the signature silently creates a *new* function instead of overriding;
`override` turns that into an error. Use it.

---

## 8. Virtual destructor — **the measurement**

```cpp
struct Base       { ~Base(){ cout << "~Base "; } };           // NOT virtual
struct Der : Base { ~Der() { cout << "~Der ";  } };
Base* p = new Der();  delete p;
```

**Measured output: `~Base `** — `~Der` never runs. Anything the derived class allocated leaks,
and formally the behaviour is **undefined**.

Change one word:

```cpp
struct Base       { virtual ~Base(){ cout << "~Base "; } };
```

**Measured output: `~Der ~Base `** — correct, derived first then base.

**Key insight.** If a class has *any* virtual function it is meant to be used through base
pointers, so its destructor must be virtual too. This is the most common memory leak in real C++
codebases, and the cost is one vtable pointer per object.

**`virtual.cpp` has this bug** — `Vehicle` has a virtual `show()` and a non-virtual destructor, so
both its `new`ed objects leak (and are never `delete`d at all, which leaks them twice over).

---

# Section 2 — Important

## 9. Abstract class — **implementation, code given**

```cpp
class Shape {
public:
    virtual double area() const = 0;     // pure virtual -> Shape cannot be instantiated
    virtual ~Shape() {}                  // still required (see #8)
};
class Circle : public Shape {
    double r;
public:
    Circle(double r_) : r(r_) {}
    double area() const { return 3.14159265358979 * r * r; }
};
```

**Key insight — the payoff is the polymorphic container:**

```cpp
vector<Shape*> shapes;
shapes.push_back(new Circle(1.0));
shapes.push_back(new Rect(2, 3));
double total = 0;
for (size_t i = 0; i < shapes.size(); ++i) total += shapes[i]->area();   // each does its own
```

One loop, no type checks, no `if (isCircle)`. Adding a `Triangle` requires changing nothing here.
That is what runtime polymorphism buys, and it is the answer to "why not just use a switch?"

`= 0` makes the class abstract — `Shape s;` will not compile. A derived class that does not
implement every pure virtual stays abstract itself.

**Verified:** a `vector<Shape*>` holding a unit circle and a 2×3 rectangle totals **9.1415…**
(π + 6).

---

## 10. Access specifiers through inheritance

Reconstruct the table in `readme.md` §2 by experiment rather than reading it:

```
class A { private: int p; protected: int q; public: int r; };
class B : public A {
    void f() {
        // p = 1;   <-- ERROR: private is inherited but NOT accessible
        q = 1;      // OK: protected
        r = 1;      // OK: public
    }
};
int main() {
    B b;
    // b.q = 1;     <-- ERROR: protected is not public
    b.r = 1;        // OK
}
```

**Key insight.** A private member **is** inherited — it occupies space in every `B` — it is simply
inaccessible. "Not inherited" and "not accessible" are different claims and the distinction gets
asked. Your comment in `inheritance.cpp:25` says private "cannot be access cannot be inherited";
the first half is right, the second is the common misstatement.

---

## 11. The diamond problem — **the measurement**

Using your own `diamond.cpp` classes (`B` and `C` both `virtual public A`):

```
&d.B::a  ==  &d.C::a        -> SAME address: exactly ONE shared A
d.a = 7;                    -> compiles with no ambiguity error
sizeof(D)  with virtual     = 24
sizeof(D2) without virtual  = 20        (two copies of A, but no vtable pointers)
```

**Key insight.** `virtual` inheritance exists **to eliminate** the duplicate base. Without it, `D`
has two `A` subobjects, `d.a` is ambiguous and will not compile, and you must write `d.B::a` or
`d.C::a` to disambiguate. With it there is one shared `A` and `d.a` is unambiguous.

The size result is the counter-intuitive part and worth keeping: the virtual version is *larger*
here, because virtual bases are reached through per-object pointers, and that overhead exceeds the
4 bytes saved by not duplicating a single `int`. Virtual inheritance pays off when the shared base
is substantial — and correctness, not size, is the reason to use it.

**Your comment says the opposite of what the code does** — see the bugs section.

---

## 12. Function overloading

**The rule.** Same name, **different parameter lists** (count, types or order), same scope,
resolved at compile time.

**What does NOT count as an overload:**

- **Return type alone.** `int f(); double f();` is a compile error — the call site cannot
  disambiguate.
- Differing only by a top-level `const` on a by-value parameter (`f(int)` vs `f(const int)`).

**What does:** a `const` member function vs. a non-`const` one is a valid overload pair, selected
by whether the object is `const`.

**Beware ambiguity through promotion.** With `add(int)` and `add(double)`, a call `add('a')` picks
`int`; with `add(float)` and `add(double)`, `add(1.0f)` picks `float`, but `add(1)` is ambiguous.

**In `oops2.cpp`, `Bike honda(17.5);` calls `Bike(int)`** — `17.5` narrows to `17` silently.
An `explicit` constructor or a `Bike(double)` overload would prevent it.

---

## 13. Operator overloading — **implementation, code given**

```cpp
class Vec2 {
public:
    int x, y;
    Vec2(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
    Vec2 operator+(const Vec2& o) const { return Vec2(x + o.x, y + o.y); }
    bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
    bool operator<(const Vec2& o) const  { return x != o.x ? x < o.x : y < o.y; }
};
std::ostream& operator<<(std::ostream& os, const Vec2& v) {     // must be a FREE function
    return os << "(" << v.x << "," << v.y << ")";
}
```

**Key insight — three rules:**

1. **`operator<<` must be a free function**, not a member. The left operand is the stream, and you
   cannot add members to `std::ostream`. Return the stream by reference so `cout << a << b` chains.
2. **Mark comparison and arithmetic operators `const`.** Otherwise they cannot be called on a
   `const Vec2&`, which is how they will be passed to `std::sort`.
3. **`operator<` must be a strict weak ordering** — irreflexive (`!(a < a)`), antisymmetric,
   transitive. Writing `<=` instead of `<` breaks it and can make `std::sort` read out of bounds.
   Same warning as `../12_sorting` §6.

Without `operator<` (or a comparator), your class cannot be sorted or used as a `std::map` key.

**Verified:** `Vec2(1,2) + Vec2(3,4)` gives `(4,6)`; `operator==` agrees.

---

## 14. `this` and method chaining

**Approach.** `this` is a pointer to the object the method was called on. Returning `*this` by
reference lets calls chain.

```
Builder& setName(string n)  { this->name = n; return *this; }
Builder& setAge(int a)      { this->age  = a; return *this; }
// usage:  b.setName("x").setAge(3);
```

**Key insight.** The common use of `this->` is disambiguating a member from a parameter with the
same name — `this->name = name;`, which is what your `intro.cpp:60` and `linkedList.cpp:8` both
do correctly. Returning `Builder&` (not `Builder`) is what makes chaining free rather than copying
the object at every step.

`this` is not available in a `static` member function — there is no object.

---

# Section 3 — Good to Know

## 15. LRU Cache — LeetCode 146 `[fwd]`

**Approach.** Two structures kept in sync by one class:

```
hash map:  key -> iterator/pointer to a node
doubly linked list:  nodes in recency order, most recent at the front

get(key):
    if key not in map:  return -1
    move that node to the FRONT of the list      # O(1) with a doubly linked list
    return its value

put(key, value):
    if key exists:  update value, move node to front
    else:
        if size == capacity:  remove the node at the BACK, erase its key from the map
        push a new node at the front, record it in the map
```

**Key insight — why two structures.** A hash map gives O(1) lookup but has no order. A list gives
O(1) reordering but O(n) lookup. Neither alone meets the requirement, so you compose them: the map
stores *pointers into* the list, and the class exists to keep the two consistent. That composition
is the interview.

**Why doubly linked, not singly:** to unlink a node in O(1) you need its predecessor, which only a
`prev` pointer gives you. That is the clearest practical justification for doubly linked lists —
worth carrying into `../17_linked_list`.

Needs `../17_linked_list` and `../24_maps`. Come back to it; reading the design now still pays.

---

## 16. Design a Stack / Queue class `[fwd]`

**Approach.** Public interface `push` / `pop` / `top` / `empty` / `size`; private array or list
underneath. The point is **abstraction**: callers cannot tell which backing store you used, so you
can change it without touching them.

Two details that come up in `../18_stack` and `../20_queu`:

- **Underflow.** `pop()` on an empty stack must not read `arr[-1]`. Return a status, throw, or
  document the precondition — but decide, do not ignore it.
- A **queue on a plain array** wastes space as the front advances; the fix is a **circular** buffer
  with `(rear + 1) % capacity`.

---

# Section 4 — approach only

- **`const` member functions** — `int get() const;` promises not to modify the object, and is the
  only kind callable on a `const Foo&`. Getters should all be `const`.
- **Copy construction vs. assignment** — `Foo b = a;` constructs (copy constructor);
  `b = a;` on an existing `b` assigns (`operator=`). The `=` in a declaration is construction, not
  assignment; that trips people up.
- **Friend functions** — grant one external function access to privates. Justified for
  `operator<<` and symmetric binary operators; a smell everywhere else.
- **Virtual table** — one vtable per class, one hidden vtable *pointer* per object. That pointer
  is why `sizeof` jumps by 4 (here) the moment you add a virtual function.
- **Object slicing** — passing a `Derived` **by value** as a `Base` copies only the base part; the
  derived data is discarded and virtual calls go to the base. Pass by reference or pointer.
- **Construction/destruction order** — base constructor first, then members in **declaration**
  order, then the body. Destruction is exactly the reverse.
- **Virtual constructor?** No. Virtual dispatch needs the vtable pointer, which the constructor
  itself installs — the type is not established yet. The "virtual constructor idiom" is a virtual
  `clone()` method.
- **Pure virtual destructor** — `virtual ~Base() = 0;` plus a definition `Base::~Base(){}`. It is
  how you make a class abstract when it has no other method to mark pure.
- **`struct` vs `class`** — default access only: `struct` is public, `class` is private. Same for
  default inheritance mode.
- **Rule of Five** — adds `Foo(Foo&&)` and `operator=(Foo&&)`, which steal the resource instead of
  copying it. C++11; the toolchain here is C++14 so they are available.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `16_oops/` was edited.**

## `oops2.cpp` — the object counter never counts

```cpp
Bike(int tyreSize,int engineSize){ ... noOfBikes++; }    // :14  increments
Bike(int ts):tyreSize(ts),engineSize(150){}              // :20  does NOT
Bike():tyreSize(17),engineSize(150){}                    // :21  does NOT
```

`main` constructs three bikes with the **one-argument** constructor, which is one of the two that
never touch the counter.

**Verified: the program prints `0`, `0`, `0`. Your comments predict `1`, `2`, `3`.**

**Fix — delegate rather than copy the increment into each:**

```cpp
Bike(int t, int e) : tyreSize(t), engineSize(e) { ++noOfBikes; }
Bike(int t) : Bike(t, 150) {}
Bike()      : Bike(17, 150) {}
```

Two smaller points in the same file:

- **`Bike honda(17.5);` at `:46`** silently narrows `17.5` to `17` and calls `Bike(int)`. If that
  was meant to be a `double` tyre size, the member type is wrong; if not, `explicit` on the
  constructor would have caught it.
- **`~Bike()` at `:31` prints without `endl`**, so the three destructor messages run together as
  `destructor call huadestructor call huadestructor call hua` — verified.

## `virtual.cpp:27` — the comment contradicts the code

```cpp
a=new Vehicle ;
a->show();// Bike ka show
```

The object *is* a `Vehicle`, so virtual dispatch correctly calls `Vehicle::show`.

**Verified: the program prints `Bike ka show mai vehicle ka show hu`.** The first call (line 25,
on a `new Bike`) matches its comment; the second does not.

This matters because the comment describes the misconception the file exists to disprove — with
`virtual`, the **object's** type decides, not the pointer's.

Also in this file: `Vehicle` has a virtual function but a **non-virtual destructor** (#8), and
neither `new` is ever `delete`d — two leaks, one of which would be undefined behaviour even if you
did delete it.

## `diamond.cpp:11` — the comment is backwards

```cpp
class B: virtual public A { ... };
class C: virtual public A { ... };
class D: public B, public C {
    //D has 2 instance of A          <-- with `virtual`, D has exactly ONE
```

**Verified on your classes:** `&d.B::a` and `&d.C::a` are the **same address**, and `d.a`
compiles without an ambiguity error — both of which prove a single shared `A`. Remove `virtual`
from `B` and `C` and you get the two copies the comment describes, and `cout << a;` inside
`show()` stops compiling.

The commented-out `B::a` / `C::a` lines above it are the *non*-virtual workaround, so the file
reads as though the two situations were swapped. Worth fixing, because this comment is the entire
teaching point of the file.

`main` is empty, so none of it has been exercised.

## `functionoverriding.cpp:14` — a comment typo worth fixing

`//  overriudes the function  of A` — but `show()` is **not** virtual in `A`, so this is
**shadowing**, not overriding. The code is correct and prints `mai a ka show hu` (verified,
matching your line 28 comment); it is the word "overrides" that is wrong, and the distinction is
exactly what the file demonstrates. The same comment appears in `virtual.cpp:14`, where it *is*
overriding.

## `inheritance.cpp:25` — "cannot be inherited" is not quite right

```cpp
private: // cannot be access cannot be inherited
```

Private members **are** inherited — every `B` object contains `a_private` and pays for its size —
they are just not *accessible* from the derived class. The practical effect is the same; the
statement is not, and interviewers ask about precisely this.

## `intro.cpp` — leaks and an unused member

- `Player *urvi = new Player;` at `:125` is never `delete`d.
- `changeScore(Player a)` at `:81` takes the player **by value**, so the `+10` applies to a copy
  and the caller's score is unchanged. **Your comment `//pass by value` at `:114` says exactly
  this**, so it is deliberate — good. Worth writing the by-reference version next to it for
  contrast.
- `class helmet {}` at `:14` is empty and unused.

---

## What you got right

- **`intro.cpp`'s `student` class is a correct encapsulation example** — private members, public
  accessors, and `this->name = name` to resolve the shadowed parameter. That is the idiomatic form.

- **`changeScore` demonstrating pass-by-value, with the comment saying so**, is good pedagogy
  rather than a bug. You predicted the behaviour and labelled it.

- **`functionoverriding.cpp` and `virtual.cpp` are a deliberately matched pair** — the same class
  shape, once without `virtual` and once with. That is the right way to learn the distinction, and
  both programs behave exactly as the mechanism dictates. Only one comment is wrong.

- **`oops2.cpp` shows four constructor forms** — parameterised, overloaded, initialiser-list, and
  a commented-out default — plus a destructor and a static member, in under 60 lines. The
  *coverage* is right; only the counter placement is wrong.

- **`inheritance.cpp`'s `class A` with one member per access specifier** is exactly the experiment
  needed for §2 #10. Finish it by trying to touch each from `B` and from `main`.

- **`diamond.cpp` uses `virtual public A` on both `B` and `C`** — which is the correct fix for the
  diamond. The code is right; only the comment describes the wrong situation.

- **`ques.cpp`'s array of objects** (`Cricketer cricketers[2]`) is a useful thing to have tried:
  note it **copies** `virat` and `rohit` into the array, so the later `cricketers[0].name = ...`
  assignments do not touch the originals. Worth confirming with a print — it is object slicing's
  gentler cousin, and the reason `vector<Base*>` is used for polymorphic containers (#9).
