# 17 — Linked Lists: Practice Questions

> **Why 30 ranked problems.** Linked lists have almost no algorithmic depth — the whole topic is
> **pointer rewiring under edge cases** — but they have enormous interview *breadth*, and the
> techniques compose rather than substitute. There are four: the **dummy head** (which removes the
> head special case), **slow/fast pointers** (middle, cycle, nth-from-end, palindrome — one loop,
> four problems), **three-pointer reversal**, and **merge**. The hard problems are all two or three
> of these stacked, so you need each one reflexive before they combine. Thirty also happens to be
> what it takes to cover your own 18-problem list plus the implementation work that has to come
> first — your list is a good one and it is nearly the whole Section 1 and 2 already.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search the
quoted title.

**Scope note.** Everything needs only `01`–`16`. Two problems marked **`[heap]`** have an optimal
solution using a priority queue (`../25_heap`); a merge-based alternative in scope is given.

**All 18 problems from your `readme.md` are included**, and the four you marked **(imp)** are all
in Section 1.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Implement a singly linked list class | Easy | *Drill* | `linkedList.cpp` — insert/delete/get at head, tail, index |
| 2 | Traverse, count, and print recursively | Easy | *Drill* | `NodePointer.cpp` |
| 3 | Reverse a Linked List | Easy | **LeetCode 206** | `reverse-linked-list` — **on your list (imp)** |
| 4 | Middle of the Linked List | Easy | **LeetCode 876** | `middle-of-the-linked-list` — **on your list (imp)**; `lc876.cpp` is empty |
| 5 | Linked List Cycle | Easy | **LeetCode 141** | `linked-list-cycle` — **on your list (imp)** |
| 6 | Linked List Cycle II | **Medium** | **LeetCode 142** | `linked-list-cycle-ii` — **on your list (imp)**; the entrance proof |
| 7 | Remove Nth Node From End | **Medium** | **LeetCode 19** | `remove-nth-node-from-end-of-list` — **on your list**; dummy head |
| 8 | Merge Two Sorted Lists | Easy | **LeetCode 21** | `merge-two-sorted-lists` — **on your list** |
| 9 | Remove Duplicates from Sorted List | Easy | **LeetCode 83** | `remove-duplicates-from-sorted-list` — **on your list** |
| 10 | Delete a node given only that node | **Medium** | **LeetCode 237** | `delete-node-in-a-linked-list` — **on your list**; the trick |

**Why these ten.** #1 and #2 are the implementation work everything else assumes — and #1 is where
your existing class needs the five edge cases from `readme.md` §5, two of which currently crash.

**#3 is the single most important problem in the folder.** Reversal appears inside #16, #19, #20
and half the hard problems; if the three-pointer dance is not automatic, those become impossible
rather than merely hard. #4–#6 are one technique (slow/fast) applied three ways, and doing them
consecutively is what makes that obvious — #6 in particular is worth deriving rather than
memorising. #7 is the dummy head's motivating problem: without it, "remove the first node" needs
its own branch. #8 is the merge step you will reuse in #21 and #22. #10 is a genuine trick — you
are not given the head, so you cannot unlink the node; you copy the *next* node's value into it
and delete that one instead.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Add Two Numbers | **Medium** | **LeetCode 2** | `add-two-numbers` — **on your list** |
| 12 | Rotate List | **Medium** | **LeetCode 61** | `rotate-list` — **on your list**; `k %= n` |
| 13 | Partition List | **Medium** | **LeetCode 86** | `partition-list` — **on your list**; two dummy heads |
| 14 | Remove Linked List Elements | Easy | **LeetCode 203** | `remove-linked-list-elements` — dummy head again |
| 15 | Intersection of Two Linked Lists | Easy | **LeetCode 160** | `intersection-of-two-linked-lists` — **on your list** |
| 16 | Palindrome Linked List | Easy | **LeetCode 234** | `palindrome-linked-list` — **on your list**; #3 + #4 |
| 17 | Remove Duplicates from Sorted List II | **Medium** | **LeetCode 82** | `remove-duplicates-from-sorted-list-ii` — delete **all** copies |
| 18 | Odd Even Linked List | **Medium** | **LeetCode 328** | `odd-even-linked-list` — two chains, then splice |
| 19 | Swap Nodes in Pairs | **Medium** | **LeetCode 24** | `swap-nodes-in-pairs` — #25 with k=2 |

**Why these nine.** #11 is schoolbook addition with a carry, and the trap is a carry surviving past
both lists (`999 + 1`). #12's `k %= n` is mandatory — `k` can exceed the length — and you must find
the tail to close the loop before cutting it. **#13, #18 and #19 are all "build two chains with two
dummy heads, then join them"**, which is a pattern worth naming; #13 also has to preserve relative
order, which rules out swapping values.

**#16 is the first genuine composition** — find the middle (#4), reverse the second half (#3),
compare, and (in good answers) restore the list before returning. #15 has a lovely O(1)-space
solution that looks like magic until you see the length argument. #17 is #9's harder twin: keeping
one copy is easy, deleting *every* copy of a duplicated value needs a dummy head and a
look-ahead.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 20 | Reverse Linked List II (positions m..n) | **Medium** | **LeetCode 92** | `reverse-linked-list-ii` — **on your list** |
| 21 | Reorder List | **Medium** | **LeetCode 143** | `reorder-list` — **on your list**; three techniques stacked |
| 22 | Sort List | **Medium** | **LeetCode 148** | `sort-list` — **on your list**; merge sort on a list |
| 23 | Merge k Sorted Lists | **Hard** | **LeetCode 23** | `merge-k-sorted-lists` — **on your list** `[heap]` |
| 24 | Merge Nodes in Between Zeros | **Medium** | **LeetCode 2181** | `merge-nodes-in-between-zeros` |
| 25 | Reverse Nodes in k-Group | **Hard** | **LeetCode 25** | `reverse-nodes-in-k-group` — the hardest common one |
| 26 | Copy List with Random Pointer | **Medium** | **LeetCode 138** | `copy-list-with-random-pointer` — interleaving trick |
| 27 | Flatten a Multilevel Doubly Linked List | **Medium** | **LeetCode 430** | `flatten-a-multilevel-doubly-linked-list` |
| 28 | Spiral Matrix IV | **Medium** | **LeetCode 2326** | `spiral-matrix-iv` — **on your list**; `../08_2d array` + list traversal |
| 29 | Design Doubly Linked List | **Medium** | **LeetCode 707** | `design-linked-list` — the `prev` pointer |
| 30 | LRU Cache | **Medium** | **LeetCode 146** | `lru-cache` `[heap]`→map — the payoff for #29 |

**Why these eleven.** #20 is #3 restricted to a window, and the reason it is harder is bookkeeping:
you need the node *before* position `m` and the node *at* `n+1` to splice the reversed section
back. #21 stacks three techniques — find middle, reverse second half, interleave — and is the best
single test of whether Section 1 stuck.

**#22 is why merge sort matters for lists**: quick sort needs random access to reach a pivot, which
is O(n) per access on a list; merge sort only needs splitting and sequential merging, and it
rewires instead of copying, so it needs no O(n) buffer. **#25 is the hardest problem most people
meet on this topic** and it is worth writing carefully — the "fewer than k nodes remain" check must
happen *before* you reverse anything, or you leave the tail mangled.

#26's trick is delightful: interleave each copy right after its original, so `orig->random->next`
*is* the copy's random target, then unweave. **#29 and #30 belong together** — the `prev` pointer
is what makes O(1) unlinking possible, and #30 is the reason anyone wants that. #28 connects this
folder back to `../08_2d array`: the spiral traversal is unchanged, only the source of values
differs.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Delete the middle node | **Medium** | **LeetCode 2095** | #4 plus keeping the predecessor |
| Remove duplicates from an **unsorted** list | **Medium** | GFG | *"Remove duplicates from an unsorted linked list"* — needs a set |
| Length of a loop | Easy | GFG | *"Find length of Loop"* — count from the meeting point |
| Nth node from the end | Easy | GFG | *"Nth node from end of linked list"* — #7 without deleting |
| Insert into a Sorted Circular Linked List | **Medium** | **LeetCode 708** | the wrap-around case is the whole problem |
| Rotate a doubly linked list | **Medium** | GFG | *"Rotate Doubly linked list by N nodes"* |
| Add 1 to a number represented as a list | **Medium** | GFG | *"Add 1 to a Linked List Number"* — reverse, carry, reverse back |
| Subtract two linked lists | **Medium** | GFG | *"Subtraction in Linked List"* |
| Segregate 0s, 1s and 2s in a list | **Medium** | GFG | *"Given a linked list of 0s, 1s and 2s"* — Dutch flag, `../07_Array` |
| Clone a linked list with next and random | **Medium** | GFG | same as #26 |
| Split a Circular Linked List into two halves | **Medium** | GFG | *"Split a Circular Linked List into two halves"* |
| Josephus using a circular list | **Medium** | GFG | *"Josephus Circle"* — cf. `../10_Recursion` §4 |
| Check if a list is circular | Easy | *Drill* | walk until you return to the head, or hit NULL |
| Reverse a list recursively | **Medium** | *Drill* | O(n) stack — contrast with #3's O(1) |
| Print a list in reverse without reversing it | Easy | *Drill* | recursion; **fixes `NodePointer.cpp:30`** |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 2   [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15  [ ] 16  [ ] 17  [ ] 18  [ ] 19
Section 3   [ ] 20  [ ] 21  [ ] 22  [ ] 23  [ ] 24  [ ] 25  [ ] 26  [ ] 27  [ ] 28  [ ] 29  [ ] 30
Section 4   [ ] ______ / 15
```
