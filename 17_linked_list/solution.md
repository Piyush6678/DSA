# 17 — Linked Lists: Solutions

Approach, pseudocode, complexity and key insight. Real C++ appears only for the **fundamentals**
(reversal, slow/fast, merge — where the pointer choreography *is* the lesson and prose loses
information), the **trick** problems (Floyd's entrance, delete-without-head, the interleaving
copy), and the **hard** one (reverse in k-groups).

**Everything shown as code was compiled with `g++ -std=gnu++14` and executed** — part of a
77-assertion run across folders 14–17, all passing. Measured outputs are quoted.

Assume `struct ListNode { int val; ListNode* next; };`.

---

# Section 1 — Must Do

## 1. Implement a singly linked list class

**Approach.** Keep `head`, `tail` and `size`. Every mutating operation must update **all three**
and handle the five edge cases in `readme.md` §5.

```
insertAtHead(v):   n = new Node(v);  n->next = head;  head = n
                   if size == 0:  tail = n            # first node is also the tail
                   size++

insertAtEnd(v):    n = new Node(v)
                   if size == 0:  head = tail = n
                   else:          tail->next = n;  tail = n
                   size++

deleteAtHead():    if size == 0:  return              # <-- RETURN, not just a message
                   doomed = head;  head = head->next
                   if head == NULL:  tail = NULL      # list is now empty
                   delete doomed;  size--

deleteAtEnd():     if size == 0:  return
                   if size == 1:  delete head; head = tail = NULL; size = 0; return
                   t = head
                   while t->next != tail:  t = t->next    # find the SECOND TO LAST
                   t->next = NULL;  delete tail;  tail = t;  size--
```

**Key insight — three rules that cover most of the bugs:**

1. **Every early-exit check needs a `return`.** Printing "list is empty" and then continuing is
   worse than not checking, because it turns a silent bug into a crash after a reassuring message.
2. **Deleting the last remaining node must reset `tail`**, not just `head`. Otherwise `tail`
   dangles and the next `insertAtEnd` writes through freed memory.
3. **Every path that changes the length must update `size`.** A `size` that drifts from reality
   corrupts every index-based operation that trusts it.

**Bounds checks come first.** `getElementIdx` must validate the index *before* dereferencing —
`if (idx == 0) return head->val;` ahead of the range check crashes on an empty list.

**Complexity.** Head insert/delete O(1); tail insert O(1) with a tail pointer, **tail delete O(n)**
for a singly linked list (you need the predecessor) — that asymmetry is the argument for a doubly
linked list (§3 #29).

---

## 2. Traverse, count, print recursively

```
display(head):     t = head;  while t:  print t->val;  t = t->next
recDisplay(head):  if head == NULL: return;  print head->val;  recDisplay(head->next)
revDisplay(head):  if head == NULL: return;  revDisplay(head->next);  print head->val
```

**Key insight — the same before/after distinction as `../10_Recursion` §1.** Printing *before* the
recursive call gives forward order; printing *after* gives reverse order, because the stack unwinds
backwards. The two functions differ by the order of two lines and nothing else.

**The recursive call must be to itself.** `revDisplay` calling `recDisplay` gives neither order —
that is the bug in `NodePointer.cpp:33`.

Note the iterative `while (temp->next != NULL)` prints one node **short** — it stops before the
last. Use `while (temp != NULL)` to print all of them.

**Complexity.** O(n) time; iterative O(1) space, recursive **O(n) stack**.

---

## 3. Reverse a Linked List — LeetCode 206 — **fundamental, code given**

```cpp
ListNode* reverseList(ListNode* head) {
    ListNode* prev = NULL;
    while (head) {
        ListNode* next = head->next;   // 1. SAVE the rest before destroying the link
        head->next = prev;             // 2. reverse this one pointer
        prev = head;                   // 3. advance both
        head = next;
    }
    return prev;                       // prev is the new head
}
```

**Key insight.** Saving `next` first is the entire problem — reverse the pointer before saving and
the remainder of the list is unreachable. Return `prev`, not `head`: when the loop ends `head` is
`NULL` and `prev` is the last node visited, which is the new head.

Handles the empty list for free (`prev` stays `NULL`).

**This is the most reused function in the folder** — #16, #20, #21 all call it. Get it automatic.

**Recursive version**, for the follow-up:

```
if head == NULL or head->next == NULL:  return head
newHead = reverse(head->next)
head->next->next = head        # the node after me should point BACK at me
head->next = NULL              # and I become the tail
return newHead
```

O(n) time but **O(n) stack** — the iterative version is the better answer.

**Complexity.** O(n) time, **O(1) space**.

**Verified:** `1→2→3→4→5` becomes `5→4→3→2→1`; single node unchanged; empty list returns empty.

---

## 4. Middle of the Linked List — LeetCode 876 — **fundamental, code given**

```cpp
ListNode* middleNode(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
    return slow;
}
```

**Key insight — the loop guard decides which middle you get.** `fast` covers two nodes per step,
so `slow` lands halfway.

| Guard | Even-length result | Use for |
|---|---|---|
| `while (fast && fast->next)` | **second** middle | LeetCode 876 |
| `while (fast->next && fast->next->next)` | **first** middle | splitting for merge sort (#22), palindrome (#16) |

Both orders in `fast && fast->next` matter — `&&` short-circuits, so `fast` is checked before
`fast->next` is dereferenced. Swap them and an even-length list crashes.

**Complexity.** O(n) time, O(1) space.

**Verified:** `{1,2,3,4,5}→3`; `{1,2,3,4}→3` (the second middle); single node → itself.

**`lc876.cpp` is currently an empty `main()`** — this is the one to write first.

---

## 5–6. Linked List Cycle I & II — LeetCode 141, 142 — **trick, code given**

```cpp
ListNode* detectCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {                       // they met -> a cycle exists
            ListNode* p = head;                   // reset ONE pointer to the head
            while (p != slow) { p = p->next; slow = slow->next; }   // both move ONE step
            return p;                             // they meet at the ENTRANCE
        }
    }
    return NULL;
}
```

**Key insight — why the reset works.** Let `a` = head to entrance, `b` = entrance to meeting point,
`c` = cycle length. At the meeting, `slow` has gone `a + b` and `fast` has gone `2(a + b)`; their
difference is a whole number of laps:

```
2(a+b) − (a+b) = a + b = k·c        ⟹        a = k·c − b
```

`k·c − b` is exactly the distance from the meeting point *forward* to the entrance. So a pointer
starting at the head and one starting at the meeting point, both moving one step at a time, arrive
together.

**Deriving this is the interview**, not reciting the procedure. #5 is the same loop returning
`true` at the meeting instead of continuing.

**A hash set of visited nodes also works** and is O(n) space — the weaker answer.

**Complexity.** O(n) time, **O(1) space**.

**Verified:** on `1→2→3→4` with the tail linked back to node 2, `hasCycle` is true and
`detectCycle` returns the node with value **2**; an acyclic list and a single node both return
false/NULL.

---

## 7. Remove Nth Node From End — LeetCode 19 — **fundamental (dummy head), code given**

```cpp
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0);
    dummy.next = head;
    ListNode *fast = &dummy, *slow = &dummy;
    for (int i = 0; i < n; ++i) fast = fast->next;   // open a gap of n
    while (fast->next) { fast = fast->next; slow = slow->next; }
    slow->next = slow->next->next;                   // slow is the PREDECESSOR
    return dummy.next;
}
```

**Key insight — this is the dummy head's motivating problem.** Removing the *first* node needs its
predecessor, which does not exist. The dummy supplies one, so a single code path handles "remove
the head" and "remove the middle" identically. Without it you need a separate `if (n == length)`
branch, and that branch is where the bug goes.

Both pointers start at the **dummy**, not at `head` — that off-by-one is what makes `slow` land on
the predecessor rather than the target.

Free the removed node in real code: save it before rewiring, `delete` after.

**Complexity.** O(n) time, one pass, O(1) space.

**Verified:** removing the 2nd from end of `1→2→3→4→5` gives `1→2→3→5`; removing the only node
gives an empty list; removing the head of a 2-node list gives `2`.

---

## 8. Merge Two Sorted Lists — LeetCode 21 — **fundamental, code given**

```cpp
ListNode* mergeTwoLists(ListNode* a, ListNode* b) {
    ListNode dummy(0);
    ListNode* t = &dummy;
    while (a && b) {
        if (a->val <= b->val) { t->next = a; a = a->next; }
        else                  { t->next = b; b = b->next; }
        t = t->next;
    }
    t->next = a ? a : b;                  // attach whatever remains -- no loop needed
    return dummy.next;
}
```

**Key insight — two things.** The dummy head again removes "which list starts the result?" And
`t->next = a ? a : b;` replaces the two drain loops an array merge needs: a list's tail is already
correctly linked, so you attach it whole in O(1) rather than copying element by element. That is a
genuine advantage lists have over arrays.

`<=` rather than `<` keeps the merge **stable**, same as `../12_sorting` §4.

**Complexity.** O(n + m) time, O(1) extra space.

**Verified:** `{1,2,4}` and `{1,3,4}` merge to `1→1→2→3→4→4`; merging with an empty list returns
the other.

---

## 9. Remove Duplicates from Sorted List — LeetCode 83

```
cur = head
while cur and cur->next:
    if cur->val == cur->next->val:
        doomed = cur->next
        cur->next = doomed->next          # skip it; do NOT advance cur
        delete doomed
    else:
        cur = cur->next
```

**Key insight.** Do **not** advance `cur` after deleting — three or more equal values in a row need
several deletions from the same position. Advancing unconditionally is the standard bug and leaves
`1→1→1` as `1→1`.

No dummy head needed: the head itself is never removed, because the first of each run survives.
Contrast #17, where every copy goes and the head *can* be removed — so it does need one.

**Complexity.** O(n) time, O(1) space.

---

## 10. Delete Node in a Linked List — LeetCode 237 — **trick**

**The situation.** You are given only the node to delete, not the head. You cannot unlink it —
that needs its predecessor, and you have no way to reach one.

```
node->val  = node->next->val      # copy the successor's value INTO this node
doomed     = node->next
node->next = doomed->next         # then unlink the successor
delete doomed
```

**Key insight — you delete a different node than the one you were given.** Impersonate the
successor by stealing its value, then remove the successor instead. The list ends up correct;
only the identity of the freed node differs.

This is why the problem guarantees the node **is not the tail** — a tail node has no successor to
impersonate, and the trick has no fallback.

**Complexity.** O(1).

---

# Section 2 — Important

## 11. Add Two Numbers — LeetCode 2

**Approach.** Digits are stored least-significant first, which is exactly the order addition wants.
Walk both with a carry.

```
dummy;  t = &dummy;  carry = 0
while a or b or carry:                       # the `or carry` is the trap
    s = carry + (a ? a->val : 0) + (b ? b->val : 0)
    carry = s / 10
    t->next = new Node(s % 10);  t = t->next
    advance a and b if non-null
return dummy.next
```

**Key insight.** `carry` in the loop condition is what handles `999 + 1 = 1000`, where the result
is longer than either input. Dropping it silently truncates the answer.

**Complexity.** O(max(n,m)) time.

---

## 12. Rotate List — LeetCode 61

```
if head == NULL or head->next == NULL:  return head
n = length;  tail = last node
tail->next = head                    # close it into a ring
k %= n                               # MANDATORY -- k can exceed n
steps = n - k                        # new tail is `steps` from the old head
newTail = head;  repeat steps-1 times: newTail = newTail->next
newHead = newTail->next
newTail->next = NULL                 # cut the ring
return newHead
```

**Key insight.** Closing the list into a ring first turns "rotate" into "cut in a different place",
which removes all the pointer juggling. **`k %= n` is not optional** — `k` may be far larger than
the list, and without it the walk runs off the end.

**Complexity.** O(n) time, O(1) space.

---

## 13, 18, 19. Two-chain problems — Partition (86), Odd Even (328), Swap Pairs (24)

**One pattern.** Build two independent chains with their own dummy heads, then splice.

```
dummyLess, dummyGreater;  l = &dummyLess;  g = &dummyGreater
for each node:
    append to l or g depending on the predicate
g->next = NULL                        # <-- ESSENTIAL: cut the old tail
l->next = dummyGreater.next
return dummyLess.next
```

**Key insight — terminate the second chain.** The last node appended to `g` still carries its
original `next` pointer, which points back into the *other* chain. Forgetting `g->next = NULL`
creates a cycle, and the symptom is an infinite loop on printing rather than a wrong value.

**#13 must preserve relative order**, which is why you rewire nodes rather than swap values.
**#18** partitions by index parity instead of value. **#19** is #25 with `k = 2`.

**Complexity.** O(n) time, O(1) space, one pass.

---

## 14. Remove Linked List Elements — LeetCode 203

Dummy head, then #9's skip-without-advancing loop with `val == target` as the predicate. The dummy
is required here because the head itself may match.

---

## 15. Intersection of Two Linked Lists — LeetCode 160 — **trick, code given**

```cpp
ListNode* getIntersectionNode(ListNode* a, ListNode* b) {
    if (!a || !b) return NULL;
    ListNode *p = a, *q = b;
    while (p != q) {
        p = p ? p->next : b;        // at the end of A, jump to B's head
        q = q ? q->next : a;        // at the end of B, jump to A's head
    }
    return p;                       // the intersection, or NULL together
}
```

**Key insight.** After switching once, both pointers have travelled exactly `lenA + lenB` steps, so
they are **synchronised** regardless of the original length difference — and they meet at the
intersection. If there is none, both reach `NULL` on the same step and the loop exits with
`p == q == NULL`.

Note the switch uses the *other list's* head, and the check is `p ? ... : b` — testing the pointer,
not `p->next` — which is what makes the no-intersection case terminate instead of looping forever.

No lengths computed, no extra space. The alternative (measure both, advance the longer by the
difference) is equally valid and less elegant.

**Complexity.** O(n + m) time, O(1) space.

**Verified:** two lists sharing a `8→4→5` tail intersect at **8**; two disjoint lists return NULL.

---

## 16. Palindrome Linked List — LeetCode 234 — **composition, code given**

```cpp
bool isPalindrome(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast->next && fast->next->next) {      // FIRST middle
        slow = slow->next; fast = fast->next->next;
    }
    ListNode* second = reverseList(slow->next);   // reverse the back half
    ListNode *p = head, *q = second;
    bool ok = true;
    while (q) { if (p->val != q->val) { ok = false; break; } p = p->next; q = q->next; }
    slow->next = reverseList(second);             // RESTORE the list
    return ok;
}
```

**Key insight — three points.**

1. **Compose #3 and #4.** Find the middle, reverse the second half, walk the two halves together.
2. **Iterate on `q` (the shorter, reversed half)**, not on `p`. On an odd-length list the first
   half has the extra node, and looping on `p` walks past the end.
3. **Restore the list before returning.** The problem does not require it, but destroying your
   input is a design smell an interviewer will flag — and re-reversing costs one line.

Note the loop guard here is `fast->next && fast->next->next`, giving the **first** middle, which is
what puts the extra node in the front half.

**Complexity.** O(n) time, **O(1) space**. Copying to a vector and two-pointering is O(n) space and
the weaker answer.

**Verified:** `1→2→2→1` true, `1→2→3→2→1` true, `1→2` false, single node true — **and the list is
unchanged afterwards**, confirmed by dumping it after the call.

---

## 17. Remove Duplicates from Sorted List II — LeetCode 82

**Approach.** Delete **every** copy of any duplicated value, keeping only values that appear once.

```
dummy->next = head;  prev = &dummy;  cur = head
while cur:
    if cur->next and cur->val == cur->next->val:
        v = cur->val
        while cur and cur->val == v:  cur = cur->next     # skip the WHOLE run
        prev->next = cur                                  # prev does NOT advance
    else:
        prev = cur;  cur = cur->next
```

**Key insight.** `prev` only advances when a node **survives**. The dummy head is mandatory here
(unlike #9) because a run at the very front means the head itself is deleted.

**Complexity.** O(n) time, O(1) space.

---

# Section 3 — Good to Know

## 20. Reverse Linked List II — LeetCode 92

**Approach.** Dummy head, walk to the node *before* position `m`, then reverse `n − m + 1` nodes
using the head-insertion trick, which avoids splicing afterwards.

```
dummy->next = head;  prev = &dummy
repeat m-1 times:  prev = prev->next        # node before the section
cur = prev->next
repeat n-m times:                            # move cur's successor to the FRONT of the section
    move = cur->next
    cur->next = move->next
    move->next = prev->next
    prev->next = move
```

**Key insight.** Repeatedly pulling the *next* node to the front of the section reverses it in
place, and because `prev` and `cur` never move, the section stays correctly attached at both ends
throughout — no separate splice step, so no chance of losing the tail.

**Complexity.** O(n) time, O(1) space, one pass.

---

## 21. Reorder List — LeetCode 143

**Approach.** Three techniques stacked: find the middle (#4), reverse the second half (#3),
interleave.

```
mid = middle (FIRST middle)
second = reverse(mid->next);  mid->next = NULL      # split cleanly
p = head;  q = second
while q:
    pn = p->next;  qn = q->next
    p->next = q;   q->next = pn
    p = pn;  q = qn
```

**Key insight.** `mid->next = NULL` before interleaving — without the cut, the first half still
runs into the (now reversed) second half and you build a cycle. Save both `next` pointers before
rewiring either, exactly as in #3.

This is the best single test of whether Section 1 stuck; if #3 and #4 are automatic it is ten
minutes, and if not it is impossible.

**Complexity.** O(n) time, O(1) space.

---

## 22. Sort List — LeetCode 148

**Approach.** Merge sort. Split at the middle, sort both halves, merge with #8.

```
sortList(head):
    if head == NULL or head->next == NULL:  return head
    slow = head;  fast = head->next               # fast starts ONE AHEAD
    while fast and fast->next:  slow = slow->next;  fast = fast->next->next
    mid = slow->next;  slow->next = NULL          # cut
    return mergeTwoLists(sortList(head), sortList(mid))
```

**Key insight — `fast = head->next`, not `head`.** Starting `fast` one node ahead makes `slow` stop
at the **first** middle, guaranteeing both halves are non-empty. With `fast = head` a two-node list
splits into the whole list plus nothing, and the recursion never terminates.

**Why merge sort and not quick sort:** lists have no random access, so reaching a pivot is O(n) per
access. Merge sort needs only sequential traversal and splitting, and it *rewires* rather than
copies — so unlike the array version it needs no O(n) buffer.

**Complexity.** O(n log n) time, **O(log n) stack**.

**Verified:** `4→2→1→3` sorts to `1→2→3→4`; a list with negatives sorts correctly; empty list
returns empty.

---

## 23. Merge k Sorted Lists — LeetCode 23 `[heap]`

**Optimal (needs `../25_heap`).** A min-heap of the k current heads; pop the smallest, append it,
push its successor. **O(N log k)**.

**In scope now — divide and conquer.** Merge lists pairwise, halving k each round:

```
while lists.size() > 1:
    merged = []
    for i in steps of 2:  merged.push(mergeTwoLists(lists[i], lists[i+1] or NULL))
    lists = merged
return lists[0]
```

**Key insight.** Same **O(N log k)** as the heap, using only #8. Merging one at a time instead is
O(N·k) — the first list gets traversed k times — which is the difference between passing and
timing out.

---

## 24. Merge Nodes in Between Zeros — LeetCode 2181

Single pass: accumulate values until the next `0`, then emit one node. Reuse the input's nodes to
keep it O(1) space.

---

## 25. Reverse Nodes in k-Group — LeetCode 25 — **hard, code given**

```cpp
ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode* node = head;
    for (int i = 0; i < k; ++i) {              // CHECK FIRST: are there k nodes?
        if (!node) return head;                // fewer than k -> leave this group alone
        node = node->next;
    }
    ListNode* prev = reverseKGroup(node, k);   // recursively handle the rest
    ListNode* cur = head;
    for (int i = 0; i < k; ++i) {              // reverse exactly k, onto the reversed tail
        ListNode* next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}
```

**Key insight — the count happens before any rewiring.** The problem says a trailing group of
fewer than `k` nodes stays as it is. If you reverse first and check later, you have already
mangled it and cannot cheaply undo. Walking `k` nodes up front costs one extra pass per group and
removes the entire difficulty.

The second insight is that `prev` is seeded with the **already-reversed remainder** rather than
`NULL`, so the reversal loop attaches this group to the rest automatically — no splicing.

Recursion costs O(n/k) stack. The iterative version tracks a `groupPrev` pointer and is O(1) space;
this one is far easier to get right under pressure.

**Complexity.** O(n) time, O(n/k) stack.

**Verified:** `1→2→3→4→5` with k=2 gives `2→1→4→3→5` (the trailing 5 untouched); with k=3 gives
`3→2→1→4→5`; with k > length the list is returned unchanged.

---

## 26. Copy List with Random Pointer — LeetCode 138 — **trick**

**Approach — three passes, O(1) extra space.**

```
1. interleave:  for each original node o:  c = new Node(o->val);  c->next = o->next;  o->next = c
                -> A -> A' -> B -> B' -> C -> C'

2. wire randoms: for each original o:  if o->random:  o->next->random = o->random->next
                -> o->next IS o's copy, and o->random->next IS the copy of o's random target

3. unweave:      restore every o->next and link the copies together
```

**Key insight.** Placing each copy immediately after its original makes the mapping
"original → copy" a single `->next` hop, so no hash map is needed. Line 2 is the whole trick and
worth reading twice: `o->random->next` is the copy of whatever `o->random` pointed at.

The map-based solution (`old → new`, two passes) is O(n) space and completely acceptable — this one
is the follow-up when asked for O(1).

---

## 27. Flatten a Multilevel Doubly Linked List — LeetCode 430

Iterate; on a node with a `child`, splice the child list in between the node and its `next`,
fixing `prev` pointers on both seams and clearing `child`. A stack-based version is equivalent and
easier to reason about.

---

## 28. Spiral Matrix IV — LeetCode 2326

**Approach.** The boundary-shrinking spiral from `../08_2d array/solution.md` #3, unchanged.
Only the source of values differs: pull from the list, and write `-1` once it runs out.

**Key insight.** Fill the matrix with `-1` first, then overwrite while the list lasts. The spiral
guards (`if (top <= bottom)`, `if (left <= right)`) are the same ones your `spiral.cpp` was
missing — see that folder.

---

## 29. Design a Doubly Linked List — LeetCode 707

**Approach.** Each node carries `prev` and `next`. Use **sentinel head and tail** nodes so no
insertion or deletion is ever a special case.

```
insert x before y:      x->prev = y->prev;  x->next = y
                        y->prev->next = x;  y->prev = x
remove x:               x->prev->next = x->next
                        x->next->prev = x->prev
```

**Key insight — removal is O(1) with no search.** Given the node, `prev` gives you the predecessor
directly; a singly linked list would need an O(n) walk to find it. That single property is why the
LRU cache in #30 needs a doubly linked list, and it is the cleanest justification for the extra
pointer.

With sentinels, `x->prev` and `x->next` are never `NULL` for a real node, so both snippets above
work at the ends without a branch.

---

## 30. LRU Cache — LeetCode 146

**Approach.** Hash map `key → node pointer`, plus a doubly linked list in recency order.

```
get(key):     miss -> -1
              hit  -> move the node to the FRONT, return its value
put(key, v):  exists -> update, move to front
              else   -> if full: evict the node at the BACK and erase its key from the map
                        insert a new node at the front and record it in the map
```

**Key insight — neither structure alone suffices.** The map gives O(1) lookup but no order; the
list gives O(1) reordering but O(n) lookup. Composing them, with the map storing pointers *into*
the list, gives O(1) for both. The class exists to keep the two consistent — every eviction must
remove the key from the map as well as the node from the list, and forgetting that is the classic
bug.

Design discussion in `../16_oops/solution.md` §3 #15.

---

# Section 4 — approach only

- **Delete the middle node (2095)** — #4 while keeping the predecessor; a one-node list becomes
  empty.
- **Remove duplicates from an unsorted list** — needs a hash set; O(n) time, O(n) space. Sorting
  first destroys the required order.
- **Length of a loop** — from the meeting point in #6, walk forward counting until you return to
  it.
- **Insert into a sorted circular list (708)** — three cases: normal position, at the wrap-around
  (max→min boundary), and an all-equal list where you insert anywhere. The third is what catches
  people.
- **Add 1 to a list number** — reverse, add with carry, reverse back; or recurse and carry on the
  way up, which avoids both reversals.
- **Segregate 0s, 1s and 2s** — three dummy heads and splice, the #13 pattern. Counting and
  overwriting values is also valid and simpler if node identity does not matter.
- **Split a circular list into halves** — #4 on a circular list, then close both halves.
- **Reverse recursively** — the version in #3; O(n) stack is the point of the contrast.
- **Print in reverse without reversing** — recurse to the end, print on the way back up. **This is
  what `NodePointer.cpp:30` is trying to do.**

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `17_linked_list/` was edited.**

`linkedList.cpp`'s `main` produces exactly the output its comments predict — all six lines match,
verified. The bugs below are all in paths `main` never exercises, which is precisely why they
survived.

## `linkedList.cpp:96` — `deleteAtHead` crashes on an empty list

```cpp
void deleteAtHead(){
    if(size==0){ cout <<"You'r list is empty"; }    // <-- no return
    head=head->next;                                 // runs anyway; head is NULL
    size--;
}
```

**Verified: ACCESS_VIOLATION (`0xC0000005`) on a freshly constructed list.** The message prints
and then the program dereferences `NULL`.

Two further faults: it never sets `tail = NULL` when the last node goes (leaving `tail` dangling),
and it never `delete`s the removed node.

**Fix:** `if (size == 0) return;`, then after `head = head->next;` add
`if (head == NULL) tail = NULL;`, and delete the old node.

## `linkedList.cpp:104` — `deleteAtENd` crashes on a one-element list, and never updates `size`

```cpp
void deleteAtENd(){
    if(size==0){ ... return ;}
    Node* temp =head;
    while(temp->next!=tail){ temp=temp->next; }     // size==1: head IS tail
    temp->next=NULL;
    tail=temp;                                       // <-- size never decremented
}
```

**Two separate defects, both verified:**

1. **One-element list → ACCESS_VIOLATION.** With `head == tail`, `temp->next` is `NULL`, so the
   condition `temp->next != tail` is true, `temp` becomes `NULL`, and the next iteration
   dereferences it.
2. **`size` is never decremented.** Measured:

   | after | list contents | `size` field | actual |
   |---|---|---|---|
   | 3 inserts | `10 20 30` | 3 | 3 |
   | `deleteAtENd()` | `10 20` | **3** | 2 |
   | `deleteAtENd()` | `10` | **3** | 1 |

   Your `main` happens not to expose this, because the operations that follow (`insertAtHead`,
   `insertAtIdx(2,...)`, `getElementIdx(2)`) all still work with `size` one too large. But
   `insertAtIdx(size, v)` and `getElementIdx(size-1)` now target the wrong place, and
   `deleteAtIdx(size-1)` no longer routes to `deleteAtENd`.

**Fix:** handle `size == 1` before the walk, and `size--` on every path.

## `linkedList.cpp:81` — `getElementIdx` dereferences before validating

```cpp
Node* temp =head;
if(idx==0)return temp->val;        // <-- runs before the bounds check below
if(idx==size-1)return tail->val;
if(idx<0 || idx>=size){ ... }      // too late
```

**Verified: `getElementIdx(0)` on an empty list → ACCESS_VIOLATION.** Move the range check to the
top; it is the first thing the function should do.

## `linkedList.cpp:114` — `deleteAtIdx` leaks, and trusts a wrong `size`

`temp->next = temp->next->next;` unlinks without freeing. And because it dispatches on
`idx == size-1`, it inherits the `size` drift from `deleteAtENd`.

The index walk itself is correct — `for (int i=1; i<idx; i++)` lands `temp` on the predecessor.

## `NodePointer.cpp:30` — `revDisplay` calls the wrong function

```cpp
void revDisplay(Node* head){
    if (head==NULL)return ;
    recDisplay(head->next);        // <-- should be revDisplay
    cout<<head->val<<" ";
}
```

**Verified on `10→20→30`: prints `20 30 10`.** The correct reverse order is `30 20 10`.

`recDisplay(head->next)` prints the tail *forwards* and then appends the current node, so the
output is "everything after me, in order, then me". One character fixes it.

`revDisplay` is never called from `main`, which is why it went unnoticed.

## `NodePointer.cpp` / `nodeClass .cpp` — smaller notes

- **`nodeClass .cpp:33`** — `while(temp->next!=NULL)` prints one node **short** (verified: `10 20 30`
  for a four-node list). Your comment says `//10 20 30`, so this is understood — but
  `while (temp != NULL)` is the form to use going forward, and `display()` in `NodePointer.cpp:18`
  already gets it right.
- **The filename `nodeClass .cpp` has a space before the extension.** It compiles, but it needs
  quoting on every command line. Worth renaming.
- **Every `new Node` in `NodePointer.cpp` leaks** — no `delete` anywhere. Fine for a demo; say so
  in an interview.

## `lc876.cpp` — empty

The file contains only an empty `main()`. LeetCode 876 is the problem you marked **(imp)** in your
own list, and it is `questions.md` §1 #4 — solution in #4 above.

---

## What you got right

- **`linkedList.cpp`'s happy path is completely correct** — all six of your predicted outputs
  match, verified line by line:

  ```
  10 20 30 40   /   10 20 30   /   5 10 20 30   /   5 10 15 20 30   /   15   /   5 10 20 30
  ```

  Writing the expected output as a comment next to each call is genuinely good practice — it is
  what let me confirm intent rather than guess it, and it is how you would catch a regression.

- **Maintaining a `tail` pointer** makes `insertAtEnd` O(1) instead of O(n). That is the right
  design decision and many first implementations miss it.

- **`insertAtIdx` delegating to `insertAtHead` / `insertAtEnd`** for the boundary indices is
  exactly the right structure — the general case only has to handle the middle, which is why that
  branch is correct.

- **`deleteAtIdx` dispatching to `deleteAtHead` / `deleteAtENd`** is the same good instinct.

- **The `Node` constructor sets `next = NULL`.** Leaving it uninitialised is the single most common
  linked-list bug and you avoided it in all three files.

- **`protected` for `head`/`tail` rather than `public`** (`linkedList.cpp:13`) is a deliberate
  encapsulation choice — it keeps callers from corrupting the invariants while leaving room for a
  derived class. That is a `../16_oops` idea applied on purpose.

- **`NodePointer.cpp` has both iterative and recursive display side by side**, which is the same
  before/after-the-call comparison as `PreInPost.cpp` in `../10_Recursion`. `recDisplay` is
  correct.

- **`nodeClass .cpp` builds the list from stack objects and `NodePointer.cpp` from `new`** — having
  written both is worth more than being told the difference. The commented-out block at
  `nodeClass .cpp:38` exploring `Node temp = a;` (copying the node rather than pointing at it) is
  exactly the right question to have asked.
