# 25 — Heaps: Practice Questions

> **Why 22 ranked problems.** A heap is one data structure but six distinct problem shapes, and the
> shapes do not transfer: **top-k / kth** (5 problems — including the size-k inversion, min-heap for
> largest, that everyone gets backwards once), **k-way merge** (3), **two heaps for a running
> median** (2), **greedy scheduling** (5 — where the heap is easy and the *greedy argument* is the
> real work), **implementation** (4 — heapify, sift up/down, in-place heap sort, a hand-rolled
> class; three of these already exist in this folder and all three have bugs), and **when a heap is
> the wrong answer** (3 — quickselect, bucket-by-frequency, in-order on a BST). Twenty-two is
> roughly four per shape. The last group is not padding: LC 347 has an O(n) bucket solution that
> beats the O(n log k) heap, and not knowing that is a worse interview outcome than not knowing the
> heap.

**Platform note.** LeetCode numbers are exact. **`[prem]`** is LeetCode Premium — the same problem
is free on GFG under the quoted title. GFG has no numeric IDs.

**Scope note.** Everything needs only `01`–`25`. **`[impl]`** marks a structural exercise rather
than a judge problem.

**All seven problems from your `readme.md` are covered**: 1046, 215, 973, 347 in Section 1; 658,
1636, 378 in Section 2.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | `priority_queue` operations drill | Easy | *Drill* | extend `basic.cpp` |
| 2 | Last Stone Weight | Easy | **LeetCode 1046** | `last-stone-weight` — **on your list** |
| 3 | Kth Largest Element in an Array | **Medium** | **LeetCode 215** | `kth-largest-element-in-an-array` — **on your list** |
| 4 | K Closest Points to Origin | **Medium** | **LeetCode 973** | `k-closest-points-to-origin` — **on your list** |
| 5 | Top K Frequent Elements | **Medium** | **LeetCode 347** | `top-k-frequent-elements` — **on your list** |
| 6 | Heapify an array in O(n) | **Medium** | *Drill* `[impl]` | `heapify.cpp` — **yours does not work** |
| 7 | Heap sort, in place | **Medium** | *Drill* `[impl]` | `heap_sort.cpp` — **yours discards its result** |

**Why these seven.** #1 is fifteen minutes and prevents three separate confusions: that
`priority_queue<int>` is a **max** heap, that `pop()` returns nothing, and that `top()` on an empty
queue is undefined rather than an error. Print, do not assume.

#2 is the pattern in its simplest form — pop two, push the difference — and the only subtlety is
that equal stones both vanish. **#3 is the archetype**, and the thing to internalise is the
inversion: *k largest wants a **min** heap of size k*, because you need cheap access to the weakest
survivor. Write it, then write "k smallest" with a max heap, back to back, or the inversion will
not stick.

#4 is #3 with a computed key (squared distance — **no `sqrt`**, it is monotonic and slower). **#5
is deliberately placed among the must-dos so you meet its trap early**: the heap solution is
O(n log k) and the bucket-by-frequency solution is O(n), because a frequency cannot exceed n and so
can index an array directly. Solve it both ways.

**#6 and #7 are your own files and neither currently works.** #6 is where the O(n) build comes from
and where 0-vs-1 indexing has to be chosen and kept. #7 is the in-place algorithm — build a max
heap, then repeatedly swap the root to the end and sift down over a shrinking prefix. Your version
uses a `priority_queue` instead, which is correct as an idea but is not heap sort and, as written,
throws its answer away.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 8 | Implement a min-heap class | **Medium** | *Drill* `[impl]` | `implementationWuthArray.cpp` — **does not compile** |
| 9 | Minimum cost to connect ropes | **Medium** | GFG | *"Minimum Cost of ropes"* — `ques.cpp` |
| 10 | Sort a nearly sorted (k-sorted) array | **Medium** | GFG | *"Nearly sorted algorithm"* — `ques.cpp` |
| 11 | Find K Closest Elements | **Medium** | **LeetCode 658** | `find-k-closest-elements` — **on your list** |
| 12 | Sort Array by Increasing Frequency | Easy | **LeetCode 1636** | `sort-array-by-increasing-frequency` — **on your list** |
| 13 | Kth Smallest Element in a Sorted Matrix | **Medium** | **LeetCode 378** | `kth-smallest-element-in-a-sorted-matrix` — **on your list** |
| 14 | Merge k Sorted Lists | **Hard** | **LeetCode 23** | `merge-k-sorted-lists` |
| 15 | Kth Largest Element in a Stream | Easy | **LeetCode 703** | `kth-largest-element-in-a-stream` — design |

**Why these eight.** #8 is the implementation problem: `push` with sift-up, `pop` with
sift-down-from-the-last-element, and the guards. Yours has the algorithm right and **does not
compile** for a naming reason — details in `solution.md`.

**#9 and #10 are also yours, and both have one-character bugs** that make them return the wrong
answer silently. #9 is the classic greedy — always merge the two shortest — and the greedy argument
(the shortest ropes participate in the most merges, so they must be merged first) is the part worth
rehearsing. #10 is "a heap of size k+1 slides along the array", and the connection to
`../14_Sliding window` is the point.

**#11 is the one where the heap is the *worse* answer.** A size-k heap is O(n log k); the array is
sorted, so **binary search for the window start** is O(log n + k). Solve it with the heap first
because that is the natural instinct, then do it properly — comparing the two is the exercise.

#12 combines `../24_maps` with a custom comparator (count ascending, then value descending), which
is the fiddliest comparator in the folder. **#13 has three solutions** — a size-k heap, a k-way
merge over rows, and **binary search on the answer value** (`../11_linearAndBinarySearch`) which is
O(n log(max−min)) and best. #14 is the canonical k-way merge. #15 is #3 turned into an API, which
is exactly the shape interviewers use for follow-ups.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 16 | Find Median from Data Stream | **Hard** | **LeetCode 295** | `find-median-from-data-stream` — two heaps |
| 17 | Task Scheduler | **Medium** | **LeetCode 621** | `task-scheduler` — heap, or a formula |
| 18 | Meeting Rooms II | **Medium** | **LeetCode 253** `[prem]` | GFG *"Minimum Platforms"* — min heap of end times |
| 19 | Reorganize String | **Medium** | **LeetCode 767** | `reorganize-string` — most frequent first |
| 20 | Furthest Building You Can Reach | **Medium** | **LeetCode 1642** | `furthest-building-you-can-reach` — a heap of regrets |
| 21 | IPO | **Hard** | **LeetCode 502** | `ipo` — two structures, sort plus heap |
| 22 | Convert a BST to a max heap | **Medium** | GFG | *"Convert BST to Max Heap"* — `bsttoheap.cpp` — **yours is correct** |

**Why these seven. #16 is the pattern worth its own name.** Two heaps facing each other, balanced
within one element; the funnelling insert (push into one, move its top to the other, rebalance) is
what keeps the halves genuinely split. O(log n) insert, O(1) median.

#17, #18 and #19 are the greedy family, and in all three the heap is the *easy* part. #17 has a
closed-form answer that beats the heap entirely — deriving it is the real problem. #18 is the
classic interval question: sort by start, keep a min heap of end times, and the heap's size is the
number of rooms in use. #19 needs "always place the most frequent remaining character that is not
the previous one", plus a feasibility check before you start.

**#20 is the most interesting problem in the section.** You walk forward using ladders greedily,
and when you run out you **retroactively convert the smallest ladder use into bricks** — a min heap
of "decisions I might regret". That "heap of past choices you can undo" idea generalises far beyond
this problem. #21 is two structures at once — sorted by capital, heap by profit — and is the
capstone.

**#22 is already in your folder and it works.** Verified: reverse in-order to collect values
descending, then a pre-order overwrite. Extending it to a *min* heap is a two-line change and a
good check that you know why it works.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Kth Smallest Element in an Array | **Medium** | *Drill* | max heap of size k — #3 mirrored |
| K Largest Elements | **Medium** | GFG | *"K largest elements"* — the whole set, not just the kth |
| Sort Characters By Frequency | **Medium** | **LeetCode 451** | map + heap, or buckets |
| Ugly Number II | **Medium** | **LeetCode 264** | min heap + a set, or three pointers |
| Super Ugly Number | **Medium** | **LeetCode 313** | #264 generalised |
| Find K Pairs with Smallest Sums | **Medium** | **LeetCode 373** | k-way merge over pairs |
| Smallest Range Covering K Lists | **Hard** | **LeetCode 632** | k-way merge with a window |
| Maximum Performance of a Team | **Hard** | **LeetCode 1383** | sort by one key, heap on the other — like #21 |
| Single-Threaded CPU | **Medium** | **LeetCode 1834** | events sorted, heap for what is available |
| Seat Reservation Manager | **Medium** | **LeetCode 1845** | min heap as a free list |
| Minimum Number of Refueling Stops | **Hard** | **LeetCode 871** | the same "heap of regrets" as #20 |
| Sliding Window Maximum | **Hard** | **LeetCode 239** | heap works; the **deque** in `../18_stack` is O(n) |
| Design Twitter | **Medium** | **LeetCode 355** | maps + a heap merge — from `../24_maps` §3 #24 |
| Connect n ropes, but **minimise the maximum** | **Medium** | *Drill* | why the greedy changes when the objective does |
| Build a heap by pushing vs by heapify | Easy | *Drill* | time both at n = 10⁶; the gap is the O(n) claim |
| Delete an arbitrary element from a heap | **Medium** | *Drill* | needs a value→index map, or lazy deletion |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7
Section 2   [ ] 8   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15
Section 3   [ ] 16  [ ] 17  [ ] 18  [ ] 19  [ ] 20  [ ] 21  [ ] 22
Section 4   [ ] ______ / 16
```
