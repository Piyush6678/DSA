# 20 — Queue

FIFO: first in, first out. Four operations — `push` (back), `pop` (front), `front`, `empty` — all
**O(1)**, same as a stack. The difference is which end you take from, and that one choice is the
difference between depth-first and breadth-first search.

The interesting content here is not the interface but **the circular buffer**, which is what makes
an array-backed queue O(1) instead of O(n), and the two design problems (queue from stacks, stack
from queues) that get asked by name.

---

## 1. Why a naive array queue is wrong

The obvious implementation keeps `front` at index 0 and shifts everything left on `pop`:

```
pop():  for i in 0 .. size-2:  arr[i] = arr[i+1]      // O(n) -- WRONG
        size--
```

That makes `pop` **O(n)**, which defeats the purpose. Worse, if you instead advance `front` without
wrapping, the array "walks" to the right and reports itself full while the first half sits unused.

**The fix is a circular buffer**: let the indices wrap with `%`.

```cpp
class CircularQueue {
    int arr[CAP];
    int front, count;                          // head index + how many elements
public:
    bool push(int v) {
        if (count == CAP) return false;
        arr[(front + count) % CAP] = v;        // rear is DERIVED, not stored
        ++count;
        return true;
    }
    bool pop() {
        if (count == 0) return false;
        front = (front + 1) % CAP;             // move the head; shift nothing
        --count;
        return true;
    }
    int back() { return arr[(front + count - 1) % CAP]; }
};
```

**Store `front` and `count`, not `front` and `rear`.** With two indices, `front == rear` is
ambiguous — it means both empty and full — and the usual workaround is to waste one slot or keep
a separate flag. Storing a count removes the ambiguity entirely: empty is `count == 0`, full is
`count == CAP`, and the rear is `(front + count - 1) % CAP` whenever you need it.

Both `push` and `pop` are now **O(1)**, and freed slots at the front are reused.

---

## 2. Linked-list queue

```
push(v):  n = new Node(v)
          if empty:  front = back = n
          else:      back->next = n;  back = n
          ++size

pop():    if empty: return false
          old = front;  front = front->next
          if (front == NULL) back = NULL;       // <-- the list is now empty
          delete old;  --size
```

**Key insight — `back` must be reset when the queue empties.** Otherwise it dangles at a freed
node and the next `push` writes through it. This is the same trap as `deleteAtHead` in
`../17_linked_list`.

Push at the **tail**, pop from the **head** — the opposite ends from a stack. That is why a queue
needs a `back` pointer and a stack does not.

**Naming collision warning.** In C++ a class cannot have a member variable and a member function
with the same name. `int front;` plus `int front()` is a **compile error**, not a shadowing
warning. All three implementation files in this folder hit it — see `solution.md`.

---

## 3. `std::queue` and `std::deque`

```cpp
#include <queue>
queue<int> q;
q.push(10);   // add at the BACK
q.front();    // read the front      q.back();  // read the back
q.pop();      // remove the front -- returns VOID
q.size(); q.empty();
```

As with `std::stack`, **`pop()` returns nothing** and `front()`/`pop()` on an empty queue are
undefined behaviour, not exceptions. There is no iteration — if you need to inspect the middle,
a queue is the wrong type.

### `deque` — double-ended queue

```cpp
#include <deque>
deque<int> d;
d.push_back(1);  d.push_front(0);
d.pop_back();    d.pop_front();
d[i];                                // random access, unlike queue or stack
```

All four ends are O(1), **and** it supports indexing. That combination is why the monotonic deque
in `../14_Sliding window` §4 is possible at all: you need to remove from the front (expiry) and
the back (domination) in the same loop.

`std::queue` and `std::stack` are *adaptors* built on `deque` by default.

---

## 4. The two design problems

Both get asked by name, and the interesting part is the complexity argument, not the code.

### Queue using two stacks — amortised O(1)

```
push(x):  in.push(x)
pop():    if out is empty:  drain ALL of `in` into `out`     # this reverses the order
          return out.pop()
```

A single `pop` can be O(n), but **each element moves from `in` to `out` at most once in its
lifetime**, so n operations cost O(n) total — **amortised O(1)**. That is the answer being tested.

**The `if (out.empty())` guard is essential.** Draining while `out` still has elements would put
newer items in front of older ones.

### Stack using one queue — O(n) push

```
push(x):  q.push(x)
          rotate the queue (size-1) times      # bring x to the FRONT
pop():    q.pop()
```

A queue hands you the oldest element, so make the newest one oldest at push time. **No
amortisation here** — every push really is O(n). The asymmetry between the two conversions is
worth stating: stacks can simulate a queue cheaply, queues cannot simulate a stack cheaply.

---

## 5. Where queues actually show up

| Use | Why FIFO |
|---|---|
| **BFS** (`../27_graphs`) | explore all nodes at distance k before distance k+1 |
| Level-order tree traversal (`../21_tree`) | same thing on a tree |
| Task scheduling, print spoolers, buffers | fairness — first request served first |
| **Sliding window max** (`../14_Sliding window`) | a *deque*, for expiry at the front |

**BFS is the reason queues matter.** Swap the queue for a stack in a BFS and you get DFS — the
data structure *is* the traversal order.

---

## Interview Q&A

**Q1. Why is a circular queue better than a linear one?**
A linear array queue either shifts every element on `pop` (O(n)) or lets `front` walk right until
the array reports itself full with most slots empty. Wrapping the indices with `%` makes both
`push` and `pop` O(1) and reuses freed space.

**Q2. How do you distinguish full from empty in a circular queue?**
`front == rear` is ambiguous. Three fixes: keep a **count** (cleanest — empty is `count == 0`,
full is `count == CAP`); waste one slot so full means `(rear+1) % CAP == front`; or keep a boolean
flag. Prefer the count.

**Q3. Implement a queue using stacks. What is the complexity?**
Two stacks, `in` and `out`; push to `in`, and when `out` is empty drain `in` into it. **Amortised
O(1)** — each element crosses once — even though one `pop` can be O(n).

**Q4. Implement a stack using queues.**
One queue: after pushing, rotate it `size-1` times so the newest element is at the front. Push is
O(n), pop and top are O(1). Unlike Q3 there is no amortisation — every push pays the full cost.

**Q5. Queue vs. deque vs. priority queue?**
Queue: FIFO, insert back, remove front. Deque: insert and remove at **both** ends in O(1), plus
random access. Priority queue: removes the **largest** (or smallest) element, not the oldest —
O(log n) per operation, backed by a heap (`../25_heap`). Different guarantees, not variations.

**Q6. Why does BFS need a queue?**
BFS must finish every node at distance `k` before starting distance `k+1`. A queue gives exactly
that order because nodes are dequeued in the order discovered. Substituting a stack makes it DFS —
the container determines the traversal.

**Q7. Can you build a queue with O(1) worst-case using stacks?**
Not with the simple two-stack method — its O(1) is amortised, and one operation can be O(n). A
worst-case O(1) version exists (incrementally moving one element per operation instead of draining
in a burst) and is a good follow-up answer.

---

## Your original notes (preserved)

The problem list from this file is kept verbatim. **All five are covered** — see `questions.md`.

```
622  design a circular deque
1700 number of Students unable to each lunch
232 implementation using stacks
239 sliding maximum value
649 dota 2 senate 
```

| Your entry | Now at |
|---|---|
| 622 Design Circular Queue / Deque | `questions.md` §1 #3 |
| 232 Implement Queue using Stacks | `questions.md` §1 #4 |
| 1700 Students Unable to Eat Lunch | `questions.md` §2 #9 |
| 649 Dota2 Senate | `questions.md` §2 #11 |
| 239 Sliding Window Maximum | `questions.md` §3 #14 — also `../14_Sliding window` §3 #17 |
