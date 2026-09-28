# 25 — Heaps and Priority Queues

A heap answers one question, repeatedly and cheaply:

> **"What is the smallest (or largest) thing I am currently holding?"**

Not "is x present" (that is `../23_sets`) and not "give me everything in order" (that is
`../12_sorting`). A heap keeps **only the extreme** available in O(1) and pays O(log n) to insert
or remove. Sorting gives you far more than that and costs O(n log n) up front; if you only ever
need the top few, you are overpaying.

---

## 1. The structure

A heap is a **complete binary tree** — every level full except the last, which fills left to
right — with a **heap property**:

```
MIN heap: every node <= both of its children      root is the minimum
MAX heap: every node >= both of its children      root is the maximum
```

There is **no left/right ordering**. `a[left] < a[right]` is not required and usually false — that
is what separates a heap from a BST, and it is why a heap cannot answer "is 7 in here?" any faster
than a linear scan.

Because the tree is complete, **it needs no pointers at all** — it fits exactly in an array:

| Indexing | parent of `i` | left child | right child |
|---|---|---|---|
| **0-based** | `(i-1)/2` | `2i + 1` | `2i + 2` |
| **1-based** | `i/2` | `2i` | `2i + 1` |

Your `implementationWuthArray.cpp` uses the 1-based scheme (`arr[101]`, root at index 1) and the
comment at the top states it correctly. **Pick one and stay in it** — mixing the two is the single
most common heap bug, and it is what went wrong in `heapify.cpp`, which uses 1-based *arithmetic*
on a 0-based `vector`.

---

## 2. The two operations

Everything is `siftUp` and `siftDown`.

**`push`** — put the new value at the end, then **sift up** while it is smaller than its parent:

```
a.push_back(v)
i = last index
while i is not the root and a[parent(i)] > a[i]:
    swap(a[parent(i)], a[i]);  i = parent(i)
```

**`pop`** — the root leaves, the **last** element takes its place, then **sift down** while it is
bigger than its smallest child:

```
a[root] = a[last];  remove last
i = root
loop:
    small = the smallest of {i, left(i), right(i)} that exists
    if small == i: stop
    swap(a[i], a[small]);  i = small
```

Both walk one root-to-leaf path, so both are **O(log n)**. `top()` is O(1) — it is just `a[root]`.

**Why the *last* element and not a child?** Because the tree must stay complete. Promoting a child
would leave a hole in the middle of the array and the index arithmetic would stop working.

**Sift down compares against the smaller child, not the first one that is smaller.** Swapping with
the larger child leaves that child violating the property against its new sibling.

---

## 3. `priority_queue` — what you actually write

```cpp
#include <queue>

priority_queue<int> maxH;                                   // MAX heap -- the default
priority_queue<int, vector<int>, greater<int> > minH;        // MIN heap -- the incantation

pq.push(x);      // O(log n)
pq.top();        // O(1)  -- UNDEFINED on an empty queue, always check
pq.pop();        // O(log n) -- returns void; read top() first
pq.size(); pq.empty();
```

**`priority_queue<int>` is a max heap.** This surprises people who expect a "priority queue" to
serve the smallest first. The min-heap form needs all three template arguments, and `greater<int>`
comes from `<functional>` (pulled in by `<queue>` on this toolchain).

**`pop()` returns nothing.** `int x = pq.top(); pq.pop();` — two lines, always.

**Building from a container is one line and O(n)**, not O(n log n):

```cpp
priority_queue<int> pq(v.begin(), v.end());
```

`std::make_heap`, `push_heap`, `pop_heap` and `sort_heap` in `<algorithm>` operate on a plain
vector if you want the heap without the wrapper.

### Custom ordering

```cpp
struct ByAbs { bool operator()(int a, int b) const { return abs(a) < abs(b); } };
priority_queue<int, vector<int>, ByAbs> pq;      // largest |x| on top
```

**The comparator is inverted relative to `sort`.** `less` gives a *max* heap; `greater` gives a
*min* heap. Read it as "the element that compares *true* against everything sinks" — the opposite
of the intuition sorting builds. Verified: with `ByAbs`, pushing `-9, 3, -1, 7` pops
`-9, 7, 3, -1`.

For pairs, `priority_queue<pair<int,int> >` orders by `.first` then `.second` — which is why
`(distance, index)` pairs work with no comparator at all.

---

## 4. Heapify — O(n), not O(n log n)

Turning an arbitrary array into a heap:

```cpp
for (int i = n/2 - 1; i >= 0; --i) siftDown(a, i, n);      // 0-INDEXED
```

Two things worth knowing:

- **Start at `n/2 - 1`** (0-indexed) or `n/2` (1-indexed) — the last node that *has* a child.
  Everything after it is a leaf and is already a valid heap of one element.
- **Go downward, not upward.** Sifting *down* from the bottom is **O(n)**; pushing n elements one
  at a time is O(n log n). The reason is that most nodes are near the bottom and have almost
  nothing to sift through — the sum `Σ n/2^h · h` converges to 2n.

**`i` must not be modified by the inner loop.** Use a separate variable for the descent, or the
outer `for` skips nodes. (`heapify.cpp` reuses `i` for both, which is one of two bugs in that file
— see `solution.md`.)

---

## 5. The patterns

### (a) Top k / kth — a heap of size k, of the *opposite* type

**k largest → keep a MIN heap of size k. k smallest → keep a MAX heap of size k.**

That inversion is the thing to memorise. The heap holds the k best seen so far, and its top is the
**weakest survivor** — the one to evict when something better arrives.

```cpp
for (int x : nums) { minH.push(x); if (minH.size() > k) minH.pop(); }
// minH.top() is now the kth largest
```

**O(n log k) time, O(k) space** — better than sorting when k ≪ n, and it works on a stream where
sorting cannot. Your `ques.cpp` `main()` does exactly this for both directions and both answers
are correct.

### (b) Merge k sorted sequences

Push the head of each sequence; repeatedly pop the smallest and push its successor. The heap never
holds more than k items, so it is **O(N log k)** for N total elements.

Merging pairwise instead is O(N·k). This is the pattern behind external sorting, LSM-tree
compaction, and LC 23.

### (c) Two heaps — the running median

A **max heap for the smaller half** and a **min heap for the larger half**, kept within one element
of each other in size. The median is the top of the bigger heap, or the average of both tops.

O(log n) insert, **O(1) query**. It is the standard answer to "median of a stream" and the reason
"two heaps" is worth naming as a pattern.

### (d) Greedy scheduling

Repeatedly take the currently best option: the two shortest ropes, the earliest-ending meeting, the
most frequent remaining task. **The heap is what makes "currently best" cheap** — the greedy
argument is separate and is the part that has to be justified.

### (e) A sliding constraint

Sort a nearly-sorted array (each element at most k away from its place) with a min heap of size
`k+1`. Furthest-building, IPO and reorganise-string are all "keep a bounded pool and always take
the extreme from it".

---

## 6. When *not* to use a heap

| Want | Better than a heap |
|---|---|
| the kth element, one query, array in memory | **quickselect** — O(n) average (`../12_sorting`) |
| top k by *frequency* | **bucket by count** — O(n), since a count cannot exceed n |
| everything in sorted order | just sort — O(n log n) either way, smaller constant |
| membership, or "is x present" | a set — a heap cannot do this faster than O(n) |
| the kth smallest in a **BST** | in-order walk — O(h + k) (`../22_bst`) |

**LC 347 is the sharpest example.** The heap solution is O(n log k) and is what most people write;
bucketing by frequency is O(n) and shorter. Knowing when the heap is *not* the answer is worth as
much as knowing when it is.

---

## Interview Q&A

**Q1. Heap or BST — what is the difference?**
A heap only orders parent against child, so it gives the extreme in O(1) but cannot search. A BST
orders left-subtree < node < right-subtree, so it can search in O(h) but has no O(1) extreme. A
heap is also always balanced by construction; a BST is not.

**Q2. Why is a heap stored in an array?**
Because it is a *complete* tree, so the level-order positions are contiguous — no gaps, no
pointers. Children of `i` are at `2i+1` and `2i+2` (0-based). That is also why insertion has to go
at the end: completeness must be preserved.

**Q3. Complexities?**
`top` O(1); `push` and `pop` O(log n); **building from n elements O(n)** with bottom-up heapify;
heap sort O(n log n) with O(1) extra space. The O(n) build is the one people get wrong.

**Q4. Why is bottom-up heapify O(n) rather than O(n log n)?**
Most nodes are near the leaves and sift down almost nothing. Summing the work by level gives
`Σ (n/2^h)·h ≤ 2n`. Pushing n elements individually really is O(n log n), because there most
elements sift *up* through the full height.

**Q5. Find the kth largest element in a stream.**
A min heap of size k. Push, and pop when the size exceeds k; the top is the answer at all times.
O(log k) per element, O(k) space. For a fixed in-memory array, quickselect is O(n) average and
better — but it cannot handle a stream, which is the distinction being tested.

**Q6. Median of a data stream.**
Two heaps: max heap for the lower half, min heap for the upper, sizes balanced within one. Insert
by pushing into one and funnelling its top into the other, then rebalance. O(log n) insert, O(1)
median.

**Q7. Is heap sort stable? Is it used in practice?**
Not stable — sifting moves equal elements past each other. It is used as the fallback in
**introsort** (`std::sort`), which starts with quicksort and switches to heap sort when the
recursion gets too deep, guaranteeing O(n log n) worst case. Standalone it loses to quicksort on
cache locality.

**Q8. `priority_queue<int>` — min or max?**
**Max.** For a min heap you need `priority_queue<int, vector<int>, greater<int> >`. The comparator
is inverted relative to `sort`, which is the standard trip-up.

**Q9. Merge k sorted lists.**
A min heap holding one element from each list. Pop the smallest, push its successor. O(N log k)
time, O(k) space. Pairwise merging is O(N·k); divide-and-conquer merging is also O(N log k) and
uses no heap.

**Q10. Can you delete an arbitrary element from a heap?**
Not in O(log n) without help — you cannot find it. With an auxiliary map from value to index you
can: swap it with the last element, shrink, then sift it up *or* down (you do not know which).
Otherwise use **lazy deletion** — mark it and skip it when it surfaces at the top.

---

## Your original notes (preserved)

**All seven are covered** — see `questions.md`.

```
215..378 347 1636 658 973 1046 
```

| Your entry | Now at | | Your entry | Now at |
|---|---|---|---|---|
| 1046 Last Stone Weight | §1 #2 | | 215 Kth Largest in an Array | §1 #3 |
| 973 K Closest Points | §1 #4 | | 347 Top K Frequent | §1 #5 |
| 658 Find K Closest Elements | §2 #11 | | 1636 Sort by Increasing Frequency | §2 #12 |
| 378 Kth Smallest in a Sorted Matrix | §2 #13 | | | |
