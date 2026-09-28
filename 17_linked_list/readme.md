 # 17 — Linked Lists

The first data structure you build rather than borrow. Everything here is pointer rewiring, which
is why `../06_Pointer` and `../16_oops` come first — a `Node` is a class, and a list is a chain of
`Node*`.

Linked lists are also where interviews stop testing algorithms and start testing **care**. There
is rarely anything clever to find; there is always an edge case (empty list, single node, the head
itself) that separates working code from nearly-working code.

---

## 1. The node, and why `Node*` and not `Node`

```cpp
class Node {
public:
    int val;
    Node* next;
    Node(int val) { this->val = val; this->next = NULL; }
};
```

`next` **must** be a pointer. A `Node next;` member would be infinitely large — each node
containing a whole node containing a whole node. A pointer is a fixed 4 bytes (on this 32-bit
toolchain) that merely *refers* to the next one, which is what makes the recursion of the type
possible.

Your `nodeClass .cpp` builds a list from stack objects (`Node a(10); a.next = &b;`) and
`NodePointer.cpp` builds one with `new`. Both are instructive:

| | stack nodes | heap nodes (`new`) |
|---|---|---|
| Lifetime | until the scope ends | until you `delete` |
| Can outlive the function? | **no** — returning `&a` is a dangling pointer | yes |
| Cleanup | automatic | **your job** |

Real lists always use `new`, because a list built on the stack cannot be returned.

---

## 2. Array vs. linked list

| | Array / `vector` | Linked list |
|---|---|---|
| Access `i`-th | **O(1)** | O(n) — you must walk |
| Insert/delete at front | O(n) | **O(1)** |
| Insert/delete at back | O(1) amortised | O(1) **with a tail pointer**, else O(n) |
| Insert/delete in the middle | O(n) | O(1) *if you already hold the node before it* |
| Memory | contiguous, cache-friendly | scattered, one pointer of overhead per element |

**The caveat on middle insertion is the real answer.** "O(1) insertion" is only true once you have
a pointer to the predecessor, and getting there is O(n). That is why linked lists lose to vectors
far more often than the table suggests — cache locality usually beats asymptotics at realistic
sizes.

Where they genuinely win: an **LRU cache** (`../16_oops` §3 #15), where you already hold the node
via a hash map and need O(1) unlinking.

---

## 3. The three techniques

### Slow and fast pointers

```
slow = head, fast = head
while fast and fast->next:
    slow = slow->next
    fast = fast->next->next
```

`fast` moves twice as quickly, so when it reaches the end, `slow` is at the middle. This one loop
solves: **middle of the list**, **cycle detection**, **nth from the end**, and **palindrome
check**.

The loop guard is the part to get exactly right. `while (fast && fast->next)` leaves `slow` at the
**second** middle for even lengths; `while (fast->next && fast->next->next)` leaves it at the
**first**. Different problems want different ones — LeetCode 876 wants the second, splitting a
list for merge sort wants the first.

### The dummy head

```cpp
ListNode dummy(0);
dummy.next = head;
// ... work with dummy.next ...
return dummy.next;
```

Whenever an operation might modify the **head itself** — deleting the first node, inserting at the
front, removing the nth from the end — a dummy node in front removes the special case entirely.
Every node now has a predecessor, so one code path handles all of them.

This single trick eliminates most linked-list edge-case bugs. Use it by default.

### Three-pointer reversal

```
prev = NULL
while head:
    next = head->next        # 1. SAVE the rest before you destroy the link
    head->next = prev        # 2. reverse this one
    prev = head              # 3. advance both
    head = next
return prev                  # prev is the new head
```

Saving `next` **first** is the whole thing. Reverse the pointer before saving and the rest of the
list is unreachable.

---

## 4. Floyd's cycle detection, and why it works

Detecting a cycle is easy: if `slow` and `fast` ever meet, there is one. **Finding the entrance**
is the trick:

> After they meet, reset one pointer to the head. Advance both **one step at a time**. They meet
> at the cycle entrance.

**Why.** Let the distance from head to the entrance be `a`, and let the meeting point be `b` steps
into the cycle, with the cycle of length `c`. When they meet, `slow` has travelled `a + b` and
`fast` has travelled `2(a + b)`, and their difference is a whole number of laps:

```
2(a + b) - (a + b) = a + b = k·c        ->      a = k·c - b
```

So `a` (head → entrance) equals the distance from the meeting point onward to the entrance. Two
pointers moving at the same speed from those two places arrive together.

Being able to derive that, not just recite the procedure, is what the question is for.

---

## 5. The edge cases that actually break code

Test **every** list operation against these five. Most linked-list bugs are one of them:

1. **Empty list** (`head == NULL`)
2. **Single node** (`head == tail`)
3. **Operating on the head** (delete first, insert at front)
4. **Operating on the tail** (delete last — needs the *second to last* node)
5. **Two nodes** — the smallest case where "middle" and "predecessor" are ambiguous

`linkedList.cpp` handles the ordinary path correctly and fails cases 1 and 2 — verified, both
crash. That is the normal ratio, and it is why the dummy-head trick is worth the extra line.

---

## 6. Memory

Every `new Node` needs a matching `delete`. When you unlink a node you must free it *after*
rewiring, and you must save the pointer first:

```cpp
Node* doomed = temp->next;
temp->next = doomed->next;      // rewire around it
delete doomed;                  // then free it
```

Deleting before rewiring reads freed memory. None of the delete operations in `linkedList.cpp`
free anything, so every removal leaks — harmless in a scratch file, raised in an interview.

---

## Interview Q&A

**Q1. Reverse a linked list — iteratively and recursively.**
Iteratively with three pointers (§3), O(n) time and **O(1) space**. Recursively: reverse the rest,
then `head->next->next = head; head->next = NULL;` — same time but **O(n) stack**. The iterative
version is the better answer; mention the recursive one and its space cost.

**Q2. Find the middle in one pass.**
Slow/fast pointers. Say which middle your loop guard returns for an even-length list — being
precise about that is the point of the question.

**Q3. Detect a cycle, and find where it starts.**
Floyd's: slow/fast to detect; then reset one pointer to the head and advance both one step to find
the entrance. O(n)/O(1). The hash-set solution is O(n) space and the weaker answer. Derivation
in §4.

**Q4. Why is a doubly linked list needed for an LRU cache?**
To unlink a node in O(1) you need its predecessor. A singly linked list only gives you that by
walking from the head — O(n) — which destroys the whole point. The `prev` pointer is what makes
removal O(1) once the hash map has handed you the node.

**Q5. How do you find the nth node from the end in one pass?**
Two pointers `n` apart: advance `fast` by `n`, then move both until `fast` hits the end. `slow` is
then n from the end. Use a **dummy head** so that removing the first node needs no special case.

**Q6. Detect the intersection of two lists.**
Walk both; when a pointer hits the end, redirect it to the *other* list's head. Both then travel
`lenA + lenB` and meet at the intersection (or at `NULL` together). O(n+m) time, O(1) space, and
no length calculation.

**Q7. Why merge sort rather than quick sort for a linked list?**
Merge sort needs only sequential access and splitting, both of which lists do well, and it does
not need the O(n) buffer it needs for arrays — you rewire instead of copy. Quick sort needs random
access to pick and reach a pivot, which is O(n) per access. Merge sort is O(n log n) with O(log n)
stack.

---

## Your original notes (preserved)

The problem list from this file is kept verbatim. **All 18 are covered** — see `questions.md`.

```
LC
237 Delete Node in a linked list
876(imp) middle of linked list
19 remove the nth node 
160Insertion of 2 linked list
141 (imp) Linked list cycle  
142 (imp) Linked list cycle 2  
83 Remove Duplicate Sorted list
61 Roate List
2326 Spiral Matrix 4 
21 Merge two sorted list
23 Merge k sorted list
148 Sort list
86 Partition list
206 Reverse linked list (imp) 
234 Palindrome linked list
92 Reverse Linkedlist 2
143 Reoreder List

medium 
2 ->add number 
```

| Your entry | Now at | | Your entry | Now at |
|---|---|---|---|---|
| 206 Reverse (imp) | §1 #3 | | 21 Merge two sorted | §1 #8 |
| 876 Middle (imp) | §1 #4 | | 83 Remove duplicates | §1 #9 |
| 141 Cycle (imp) | §1 #5 | | 2 Add Two Numbers | §2 #11 |
| 142 Cycle II (imp) | §1 #6 | | 61 Rotate List | §2 #12 |
| 19 Remove nth from end | §1 #7 | | 86 Partition List | §2 #13 |
| 237 Delete Node | §2 #14 | | 160 Intersection | §2 #15 |
| 234 Palindrome | §2 #16 | | 92 Reverse II | §3 #19 |
| 143 Reorder List | §3 #20 | | 148 Sort List | §3 #21 |
| 23 Merge k sorted | §3 #22 | | 2326 Spiral Matrix IV | §3 #23 |

The four you marked **(imp)** — 206, 876, 141, 142 — are all in Section 1, which agrees with your
own ranking. `lc876.cpp` is currently an empty `main()`.
