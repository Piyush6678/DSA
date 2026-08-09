# 18 — Stack

LIFO: last in, first out. The interface is four operations — `push`, `pop`, `top`, `empty` — and
all four are **O(1)**. That is the whole data structure.

What makes stacks worth a folder is the **monotonic stack**, which turns a family of O(n²)
"find the nearest larger/smaller element" problems into O(n). Roughly half of everything below is
that one idea.

---

## 1. Three ways to implement it

| Backing store | `push` | `pop` | Notes |
|---|---|---|---|
| Fixed array | O(1) | O(1) | can **overflow**; fastest, no allocation |
| `vector` | O(1) amortised | O(1) | grows automatically; the sane default |
| Linked list | O(1) | O(1) | no capacity limit, one pointer of overhead per node |

**Push to the *head* of a linked list, not the tail.** Head insertion is O(1); tail insertion
without a tail pointer is O(n), and even with one, *removal* from the tail is O(n) on a singly
linked list. The head is both the insertion and removal point, which is exactly what LIFO wants.

For the array version, track `idx` as the index of the top, `-1` when empty:

```cpp
bool push(int v) { if (idx == CAP - 1) return false; arr[++idx] = v; return true; }
bool pop()       { if (idx == -1)      return false; --idx;          return true; }
int  size()      { return idx + 1; }
```

**`==`, not `=`.** `if (idx = 99)` assigns 99 and evaluates to *true*, so the guard fires every
time and the operation never happens. GCC warns
(`suggest parentheses around assignment used as truth value`), and this exact mistake appears
three times in this folder plus three more in `../20_queu`.

**Overflow vs. underflow:** overflow is pushing onto a full stack; underflow is popping an empty
one. Both messages appear as "underflow" in `userDefinedStackArray.cpp`.

**Report failure, do not just print.** `cout << "underflow"` and then continuing means the caller
has no idea anything went wrong. Return a `bool`, or use an out-parameter, or throw — but the
caller must be able to tell.

---

## 2. `std::stack`

```cpp
#include <stack>
stack<int> st;
st.push(10);      // add on top
st.top();         // read the top -- does NOT remove
st.pop();         // remove the top -- returns VOID
st.size(); st.empty();
```

**`pop()` returns nothing.** Read with `top()` first, then `pop()`. This surprises people coming
from Python or Java.

**`top()` and `pop()` on an empty stack are undefined behaviour**, not an exception. Always guard
with `!st.empty()`.

`std::stack` is an *adaptor* — by default it wraps a `deque`. There is no iteration and no
indexing, deliberately: if you need to walk the elements, a stack is the wrong type.

---

## 3. The monotonic stack — the reason this folder matters

**The problem shape.** "For each element, find the nearest element to its left/right that is
larger/smaller." Brute force is O(n²). A stack makes it O(n).

**The idea.** Keep the stack **monotonic** (all increasing or all decreasing). Before pushing a
new element, pop everything it *dominates* — those elements can never be the answer for anything
further along, because the new element is closer *and* better.

```
for i in 0 .. n-1:
    while stack not empty and a[stack.top()] <= a[i]:
        stack.pop()                 # a[i] is nearer AND bigger -> the popped one is finished
    answer[i] = stack.empty() ? NONE : a[stack.top()]
    stack.push(i)
```

**Every index is pushed once and popped once, so the total work is O(n)** despite the inner
`while`. Say that out loud — the nested loop makes people guess O(n²). It is the same amortised
argument as the sliding window in `../14_Sliding window`.

### The four variants

| Want | Scan direction | Pop while |
|---|---|---|
| Next greater to the **right** | right → left | `a[top] <= a[i]` |
| Previous greater to the **left** | left → right | `a[top] <= a[i]` |
| Next smaller to the **right** | right → left | `a[top] >= a[i]` |
| Previous smaller to the **left** | left → right | `a[top] >= a[i]` |

**`<=` versus `<` is not cosmetic.** For "strictly greater", you must pop equal elements too —
otherwise an equal value is reported as the answer, and equal is not greater. With `<`, the array
`{2,2,3}` reports `2` as the next greater element of the first `2`.

**Store indices, not values.** You almost always need the *distance* (stock span, daily
temperatures, histogram width), and only an index gives you that. Values are one lookup away;
positions are not recoverable.

---

## 4. When a stack is the answer

| Signal in the problem | Why a stack |
|---|---|
| Matching pairs — brackets, tags, quotes | the most recent unmatched opener is on top |
| "Nearest greater/smaller element" | monotonic stack |
| "Undo", "backtrack", "previous state" | LIFO is literally the history |
| Expression parsing / evaluation | operator precedence is a stack discipline (`../19`) |
| Anything naturally recursive, made iterative | the call stack made explicit |

---

## Interview Q&A

**Q1. Stack vs. queue — when do you use which?**
Stack is LIFO: use it when the most recent item is the relevant one — bracket matching, undo,
DFS, backtracking. Queue is FIFO: use it when the oldest item is next — scheduling, BFS,
buffering. "Depth first" and "breadth first" are exactly the difference between the two.

**Q2. Why is a monotonic stack O(n) when it has a nested loop?**
Amortised analysis. Each index is pushed exactly once and popped at most once, so the inner
`while` executes at most n times in total across the entire run — not n times per outer step.
Total work ≤ 2n.

**Q3. Design a stack that returns its minimum in O(1).**
Keep a second stack of minima. On push, also push onto the min-stack if the value is `<=` the
current minimum; on pop, pop the min-stack too if the tops are equal. **The `<=` matters** — with
`<`, duplicate minima are recorded once but popped once, so the minimum is lost early. O(n) extra
space; the O(1)-space variant stores encoded values and is worth knowing as the follow-up.

**Q4. How do you find the largest rectangle in a histogram?**
For each bar, the widest rectangle of that height extends until a strictly shorter bar on either
side. A monotonic increasing stack finds both boundaries in one pass: when you pop a bar, the
element below it in the stack is its left boundary and the current index is its right. O(n).

**Q5. Implement a queue using stacks.**
Two stacks, `in` and `out`. Push to `in`. To pop, if `out` is empty, drain `in` into `out` (which
reverses the order), then pop from `out`. **Amortised O(1)** — each element moves between stacks
at most once — even though a single `pop` can be O(n).

**Q6. Why does `std::stack::pop()` return void?**
Exception safety. Returning the value by copy could throw *after* the element has been removed,
losing it with no way to recover. Splitting into `top()` (read) and `pop()` (remove) means neither
operation can lose data.

**Q7. Where do stacks appear without you writing one?**
The call stack — every function call pushes a frame with its locals and return address. That is
why deep recursion causes a *stack* overflow, and why converting recursion to iteration means
managing an explicit stack yourself.

---

## Your original notes (preserved)

The problem list from this file is kept verbatim. **Both are covered** — see `questions.md`.

```
Balanced brackets
alrgest rect. in histogram 84
```

| Your entry | Now at | Your file |
|---|---|---|
| Balanced brackets | `questions.md` §1 #4 | `ques.cpp:6` — only handles `(`; see `solution.md` |
| Largest rectangle in histogram (84) | `questions.md` §3 #21 | `largestHistogram.cpp` is **empty (0 bytes)** |
