# 29 — Thirty Theoretical Questions, With Answers

The coding problems in `questions.md` have no solutions here — that is deliberate, and it is the
whole point of a revision set. **This file answers the other half of the interview**: the
conceptual questions asked while you are drawing on the whiteboard, in the ten minutes before the
coding starts, or as the follow-up to a solution you have just written.

They are the questions a problem list cannot cover, because they are not about any one problem.

**Every C++ claim below was compiled and run on this repo's toolchain** (MinGW g++ 6.3.0,
32-bit, `-std=gnu++14`). Measured output is quoted where it matters.

**How to use it:** cover the answer, say yours out loud, then compare. If you cannot say it out
loud in under a minute, you do not know it — the failure mode with theory is recognising an answer
rather than being able to produce one.

---

# A. Complexity and analysis

### Q1. What is Big-O, and how is it different from Θ and Ω?

**Big-O is an upper bound**, Ω is a lower bound, Θ is both — a tight bound.

`f(n) = O(g(n))` means `f` grows *no faster* than `g` beyond some point, up to a constant. So
"binary search is O(n²)" is technically **true and useless**; the honest statement is
Θ(log n).

In practice everyone says O when they mean Θ, and that is accepted. **Where the distinction earns
its keep is on lower bounds**: "comparison sorting is Ω(n log n)" is a statement about the
*problem*, not any algorithm — no comparison sort can ever beat it. That is a much stronger claim
than "merge sort is O(n log n)".

Be ready to say which case you mean, too: quicksort is O(n²) worst case and Θ(n log n) average, and
those are different questions.

---

### Q2. What is amortised complexity? Give a real example.

**Amortised** is the average cost per operation over a *worst-case sequence*, not over random input.
It is not the average case — there is no probability involved.

`vector::push_back` is the canonical example. Most calls are O(1); occasionally the buffer is full
and everything is copied, costing O(n). But **the capacity doubles**, so a copy of n elements is
followed by n free pushes. Total for n pushes is `n + n/2 + n/4 + … < 2n`, so **O(1) amortised**.

Measured here — the growth factor really is 2:

```
capacity 1 -> 2 -> 4 -> 8 -> 16 -> 32 -> 64 -> 128 -> 256 -> 512 -> 1024
11 reallocations for 1000 push_backs
```

The doubling is what makes it work. **Growing by a constant 10 each time gives O(n) amortised**, not
O(1), because the copies never get rarer.

Other examples in this repo: the monotonic stack (`../18_stack`), the two-stack queue
(`../20_queu`), the BST iterator (`../22_bst` §2 #17), and DSU's α(n).

---

### Q3. Give the best, average and worst case of quicksort, and explain the gap.

**Best and average Θ(n log n); worst Θ(n²).**

The worst case is when the pivot is always the smallest or largest element — the partition splits
into 0 and n−1, so the recursion is n deep with O(n) work at each level. **On already-sorted input
with a first-element pivot, that is exactly what happens** — which is the trap, because sorted input
is the common case, not an exotic one.

Three fixes, in increasing order of quality:

1. **Random pivot** — makes the bad case improbable rather than input-dependent.
2. **Median of three** — first, middle and last; cheap, and it kills the sorted-input case
   specifically.
3. **Introsort** — start with quicksort, count the recursion depth, and switch to heap sort past
   `2 log n`. That is what `std::sort` does, and it makes the worst case O(n log n) *guaranteed*
   while keeping quicksort's constant factor.

The space is O(log n) for the stack if you recurse into the smaller side first, O(n) if you do not.

---

### Q4. Does the recursion stack count towards space complexity?

**Yes.** It is memory your program is using, and it is what actually fails first.

- Recursive tree traversal: O(h) — **O(log n) balanced, O(n) degenerate**. Quote it as a range.
- Merge sort: O(n) for the buffer, plus O(log n) stack.
- DFS on a graph: O(V).
- Memoised DP: O(states) for the table, plus O(depth) for the stack — **two separate terms**.

**This is not pedantry.** Measured on this toolchain: a recursive DSU `find` on a chain survives
60,000 frames and aborts at 80,000 with `0xC00000FD` (STACK_OVERFLOW). Because `cout` is buffered,
the program prints **nothing at all** — a silent death that looks like an infinite loop.

Constraints of 10⁵ are routine, which is precisely why "convert the memoisation to tabulation" is a
real answer and not a stylistic preference.

---

### Q5. Why is comparison sorting Ω(n log n), and how do counting and radix sort get under it?

**The decision-tree argument.** A comparison sort learns only through yes/no comparisons. There are
`n!` possible orderings, so any correct algorithm's decision tree needs at least `n!` leaves. A
binary tree with `n!` leaves has depth ≥ `log₂(n!)`, and by Stirling `log₂(n!) = Θ(n log n)`. That
depth is the number of comparisons in the worst case.

**It is a bound on the problem**, not on any algorithm. No cleverness escapes it.

**Counting sort escapes by not comparing.** It uses each value as an **array index**, which is a
strictly more powerful operation than a comparison. O(n + k) for values in `[0, k)`. Radix sort
applies counting sort digit by digit: O(d(n + b)).

The catch is the assumption: the keys must be small integers, or decomposable into them. `k = 10⁹`
means a 10⁹-element counting array, which is worse than sorting. **"Sort n integers, all in
[0, 100]"** is the signal — that is Θ(n) and mentioning it is the whole point of the question.

---

# B. Core data structures

### Q6. Array versus linked list.

| | Array / `vector` | Linked list |
|---|---|---|
| Index access | **O(1)** | O(n) |
| Insert/erase at the front | O(n) | **O(1)** |
| Insert/erase in the middle | O(n) | O(1) *once you hold the node* |
| Memory | contiguous, no overhead | a pointer (4 bytes here, 8 on 64-bit) per node |
| Cache behaviour | **excellent** | poor — nodes scattered |

**The honest answer includes cache locality.** A linked list's O(1) middle insert assumes you
already have the iterator; finding it is O(n), which usually erases the advantage. And for
traversal, an array is often **an order of magnitude faster in practice** despite identical
asymptotics, because each cache line fetch brings 16 useful `int`s instead of one node.

Measured here: `sizeof(void*)` is **4** — this is a 32-bit toolchain, so a `list<int>` node costs
12 bytes to hold 4 bytes of payload.

Real answer: default to `vector`. Reach for a list when you have stable references to elements and
splice frequently — which is exactly the LRU cache (`../24_maps` §3 #18) and almost nothing else.

---

### Q7. How does a hash table work, and how are collisions handled?

Hash the key to an integer, reduce it modulo the bucket count, store the entry there. Average O(1)
for insert, find and erase.

**Collisions are unavoidable** — pigeonhole: infinitely many keys, finitely many buckets. Two
strategies:

- **Chaining** — each bucket is a linked list (what `std::unordered_map` does). Simple, degrades
  gracefully, costs a pointer per node.
- **Open addressing** — on a collision, probe for another slot (linear, quadratic, or double
  hashing). Better cache behaviour, no pointers, but erasing needs tombstones and it degrades badly
  past ~70% load.

**Load factor** = elements / buckets. Above roughly 1.0, `unordered_map` rehashes — allocating a
bigger bucket array and re-inserting everything. That is O(n), amortised away by the same doubling
argument as Q2.

**The worst case is O(n)** when every key lands in one bucket. With attacker-controlled keys this
is a real denial-of-service vector ("hash flooding"), which is why languages randomise their hash
seeds. In a contest it is why some people use `map` on adversarial tests.

---

### Q8. `map` versus `unordered_map`.

| | `unordered_map` | `map` |
|---|---|---|
| Structure | hash table | **red-black tree** |
| find / insert / erase | **O(1) average**, O(n) worst | **O(log n) guaranteed** |
| Iteration order | arbitrary | **sorted by key** |
| `lower_bound` / `upper_bound` | ✗ | ✓ |
| Key requirement | `std::hash` + `==` | `operator<` |

Measured — inserting keys `50, 10, 30, 20, 40`:

```
map          : 10 20 30 40 50
unordered_map: 40 50 10 30 20
```

**Default to `unordered_map`.** Switch to `map` when you need sorted iteration, `lower_bound`, a
worst-case guarantee, or a key type with `<` but no hash (`pair`, `vector`).

**`unordered_map<pair<int,int>, X>` does not compile** — there is no `std::hash` for `pair`. Use
`map`, or encode the pair into one key.

---

### Q9. Stack versus queue — and where is each actually used?

LIFO versus FIFO. Both O(1) at both ends they support.

**Stack** — anything where the most recent thing must be resolved first: the call stack itself,
expression parsing and matching brackets (`../19_infix prefix postfix`), undo, DFS, monotonic-stack
problems (next greater element, histogram), and backtracking.

**Queue** — anything processed in arrival order: BFS, task scheduling, producer/consumer buffers,
and level-order traversal.

**The deep version of this question is "which one gives shortest paths?"** — a queue, because BFS
explores by distance. Swapping it for a stack turns BFS into DFS and the shortest-path property is
gone. That single substitution is the difference between the two algorithms.

A **deque** does both, and is what sliding-window maximum needs (`../18_stack`).

---

### Q10. Heap versus BST versus hash table — when each?

| Question | Structure |
|---|---|
| "is x present?" | **hash table** — O(1) |
| "what is the minimum/maximum?" | **heap** — O(1) peek, O(log n) removal |
| "what is the smallest key ≥ x?" | **BST** — hash cannot do this at all |
| "give me everything in order" | **BST** — in-order is sorted |
| "kth smallest, repeatedly" | **augmented BST** with subtree sizes |

**A heap cannot search.** It only orders parent against child, so finding an arbitrary element is
O(n) — the same as an unsorted array. Interviewers ask this because "heap" sounds like it should be
searchable.

**A hash table has no order.** No successor, no range query, no sorted output. That is the price of
O(1).

**A BST is the compromise**: O(log n) for everything, and it is the only one of the three that
supports "nearest" queries. That is why `std::map` and every database index are trees.

---

### Q11. What is an abstract data type, and how is it different from a data structure?

An **ADT** is a specification: the operations and their meaning, with no implementation. A **data
structure** is a concrete way to provide it.

"Stack" is an ADT — push, pop, top, LIFO. It can be a `vector`, a linked list, or a fixed array;
all three are stacks. "Priority queue" is an ADT; a binary heap is one implementation, and so is a
sorted list (with different complexities).

**Why it matters in an interview:** it lets you separate "what I need" from "what I will use". Say
*"I need a priority queue"* first and *"I will implement it as a binary heap because push and pop
are both O(log n)"* second. That ordering shows you chose rather than defaulted.

In C++ the distinction is visible in the library: `std::stack` and `std::queue` are **container
adaptors** — ADTs wrapping a container you can pick.

---

### Q12. How do you choose a data structure for a problem?

Ask, in order:

1. **What is the operation I do most?** Optimise that; everything else is secondary.
2. **Do I need order?** If yes, hash tables are out.
3. **Is the data static or does it change?** Static allows sorting once, then binary search.
   Changing means a tree or a heap. **This is the question people skip**, and it is exactly why a
   prefix-sum array loses to a segment tree the moment updates appear.
4. **What is the key type and its range?** Small integers in a known range beat every clever
   structure — a plain array indexed by the value.
5. **What are the constraints?** n ≤ 20 says bitmask; n ≤ 10³ allows O(n²); n ≤ 10⁵ demands
   O(n log n); n ≤ 10⁹ means the answer cannot depend on n at all — binary search on the answer, or
   maths.

**The constraints tell you the complexity, and the complexity tells you the structure.** Reading
them first is the single most useful habit in a timed interview.

---

# C. Sorting and searching

### Q13. What is a stable sort, and when does it matter?

Stable means **equal elements keep their relative input order**.

It matters whenever you sort by more than one key. Sort by name, then stably sort by department,
and you get departments in order with names sorted inside each — **multi-key sorting for free**. An
unstable sort scrambles the first pass.

`std::sort` is **not stable**; `std::stable_sort` is. Measured here — 5 distinct keys with many
ties, sorted by key only:

```
n=32   sort() preserved input order of equal keys? NO
       at index 0, sort() put original element #31 where stable_sort() put #2
```

(With small or regularly-patterned inputs they often agree, because introsort falls back to
insertion sort below 16 elements. **Agreeing on your test is not a guarantee.**)

Merge sort, insertion sort and counting sort are naturally stable. Quicksort, heap sort and
selection sort are not.

---

### Q14. Merge sort versus quicksort.

| | Merge sort | Quicksort |
|---|---|---|
| Worst case | **Θ(n log n)** | Θ(n²) |
| Average | Θ(n log n) | Θ(n log n), **smaller constant** |
| Space | O(n) | O(log n) stack |
| Stable | **yes** | no |
| Cache | sequential, good | in-place, **excellent** |

**Quicksort is usually faster despite the worse bound** — it sorts in place with great locality,
while merge sort pays for a buffer and the copying in and out.

Pick merge sort when you need **stability**, a **worst-case guarantee**, or you are sorting a
**linked list** (merge sort needs no random access and can be done with O(1) extra space on a list —
quicksort on a list is awful). Merge sort is also the basis of **external sorting** for data that
does not fit in memory, and its merge step is what `../25_heap`'s k-way merge generalises.

Counting inversions is merge sort's other job (`../12_sorting`).

---

### Q15. What does `std::sort` actually use?

**Introsort** — a hybrid:

1. Quicksort while the recursion is shallow.
2. Past a depth of about `2·log₂(n)`, switch to **heap sort** — capping the worst case at
   O(n log n).
3. Below about 16 elements, **insertion sort**, which beats everything at that size because of its
   tiny constant.

So the answer to "quicksort's worst case is O(n²), why does the library use it?" is: **the library
does not use plain quicksort.** It gets quicksort's average speed with heap sort's guarantee.

`std::stable_sort` is different — a merge sort that uses a temporary buffer, or an in-place merge
sort at O(n log²n) if the allocation fails.

---

### Q16. Write binary search and name the bug everyone writes.

```cpp
int lo = 0, hi = n - 1;
while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;        // NOT (lo + hi) / 2
    if (a[mid] == target) return mid;
    if (a[mid] < target) lo = mid + 1;
    else                 hi = mid - 1;
}
return -1;
```

**`(lo + hi) / 2` overflows** when both are large. Measured:

```
lo=2000000000 hi=2000000001
(lo+hi)/2    = -147483647      <-- signed overflow, and it is silent
lo+(hi-lo)/2 = 2000000000
```

This is a real bug that sat in the JDK's `binarySearch` for nine years.

**The other bug is the loop condition.** `<=` with `hi = n-1` and `mid ± 1` terminates. Mixing
`<` with `hi = mid` and no `± 1` also works but is a *different* template — mixing halves of the two
gives an infinite loop. **Pick one and never improvise.**

The invariant to state: *"the answer, if it exists, is always inside `[lo, hi]`."* Every branch must
preserve it, and the loop must shrink the range every iteration.

---

### Q17. What does "binary search on the answer" mean?

When the answer is a **number in a range** and you can *check* a candidate faster than you can
*compute* the answer, binary search over the candidates instead of over an array.

It applies when the check is **monotone**: if `x` works then everything above it works (or
everything below). That monotonicity is the whole precondition — state it before writing code.

```
lo = smallest conceivable answer, hi = largest
while lo < hi:
    mid = lo + (hi - lo) / 2
    if feasible(mid): hi = mid           # mid might be the answer -- keep it
    else:             lo = mid + 1
return lo
```

Examples in this repo: Koko eating bananas (#37 — "can she finish at speed k?"), ship packages in
D days (#40), split array largest sum (#41), and kth smallest in a sorted matrix.

**The signal is a problem saying "minimise the maximum" or "maximise the minimum".** That phrasing
almost always means this technique.

---

# D. Trees and graphs

### Q18. Why do self-balancing trees exist?

Because a plain BST built from **sorted input** degenerates into a linked list: height n, every
operation O(n). And sorted input is the common case — records loaded by id, by timestamp, by name.

AVL and red-black trees restore balance with **rotations**, which change the height without changing
the in-order sequence. That is why a rotation is legal at all.

| | AVL | Red-black |
|---|---|---|
| Balance | strict, height ≈ 1.44 log n | loose, height ≤ 2 log n |
| Lookups | **faster** | slower |
| Insert/delete | more rotations | **at most 2–3** |
| Used by | read-heavy indexes | `std::map`, Java `TreeMap`, Linux CFS |

**`std::map` chose red-black because it favours mixed read/write workloads.** Databases use
**B+ trees** instead, for a different reason entirely: the cost is disk pages, not comparisons, so a
node is sized to fill a page and holds hundreds of keys — turning a 20-level binary tree into a
3-level one. Details in `../22_bst/advanced_tree_readme.md`.

---

### Q19. BFS versus DFS.

Both O(V + E). The difference is order and memory.

**BFS** — a queue, explores by distance. Use it for: shortest path in an **unweighted** graph,
level-by-level processing, and when the answer is likely close to the source. Memory is O(width),
which on a wide graph is worse than DFS.

**DFS** — a stack (or recursion), goes deep first. Use it for: reachability, enumerating paths,
cycle detection, topological order, connected components, and anything about **structure** (bridges,
articulation points, SCC). Memory is O(depth).

**The one-line discriminator: only BFS gives shortest paths**, and only when every edge has the same
weight. With weights it is Dijkstra, and swapping BFS's queue for a stack silently destroys the
property.

Practical caveat: recursive DFS on 10⁵ nodes overflows the stack (Q4). Use an explicit stack.

---

### Q20. There are five shortest-path algorithms. Which, and when?

| Algorithm | Handles | Complexity | Choose it when |
|---|---|---|---|
| **BFS** | unweighted | O(V + E) | every edge costs 1 |
| **0-1 BFS** | weights 0 or 1 | O(V + E) | a deque: `push_front` for 0, `push_back` for 1 |
| **Dijkstra** | non-negative | O((V+E) log V) | the default weighted case |
| **Bellman–Ford** | **negative** edges | O(V·E) | negatives present, or you must *detect* a negative cycle |
| **Floyd–Warshall** | all pairs | O(V³) | small V and you want every pair |
| **Topo + relax** | any weights, **DAG only** | O(V + E) | it is a DAG — beats Dijkstra and allows negatives |

**Dijkstra fails on negative edges** because it finalises a node when it is popped, assuming no
cheaper route can appear later — a negative edge makes one appear. Naming that is the check the
question is really running.

**Bellman–Ford's V−1 rounds** come from: a shortest path has at most V−1 edges. A **V-th** round
that still improves something proves a negative cycle — that is the detection, built in.

**Floyd–Warshall's `k` loop must be outermost.** `k` means "intermediates 0..k are now allowed";
moving it inside gives a different and wrong algorithm.

The trap case is LC 787 (**#112**): it looks like Dijkstra, and Dijkstra gets it wrong, because the
cheapest route may use too many stops.

---

### Q21. What is a minimum spanning tree, and Prim or Kruskal?

A spanning tree is an acyclic connected subgraph touching every vertex — exactly V−1 edges. The
**minimum** one minimises total edge weight.

**Prim** grows one tree outward, always taking the cheapest edge leaving it. Needs a **heap**.
O(E log V). Better on **dense** graphs.

**Kruskal** sorts all edges and takes any that joins two different components. Needs **DSU**.
O(E log E). Better on **sparse** graphs and when the input is already an edge list.

Both are correct by the **cut property**: for any partition of the vertices, the lightest edge
crossing it belongs to some MST. Being able to state that is the follow-up.

**An MST is not a shortest-path tree.** It minimises the *total* weight, not the distance between
any particular pair. Confusing them is the standard trap.

---

### Q22. When does a topological sort exist, and how do you compute one?

**Only for a DAG** — a directed acyclic graph. A cycle means mutual dependency and no valid linear
order.

**Kahn (BFS):** compute in-degrees, queue everything at zero, pop and decrement. **If fewer than V
nodes come out, there was a cycle** — the cycle detection is free.

**DFS:** post-order, then reverse. A node finishes after all its descendants, so reversing puts it
before them.

Prefer Kahn: no recursion, free cycle check, and it extends cleanly — swap the queue for a min heap
to get the lexicographically smallest order, or process one in-degree wave at a time to count
"minimum semesters".

The order is generally **not unique**. If a problem implies it is, there is an extra constraint you
have missed.

---

### Q23. DSU is "O(α(n))". Why not just say O(1)?

Because it is **amortised**, and because α is not actually a constant — it just never exceeds about
4 for any n that fits in the universe.

With **path compression** and **union by size or rank**, a sequence of m operations on n elements
costs O(m · α(n)). A *single* `find` can still be O(log n); it is the total that is near-linear.
Tarjan proved Θ(α(n)) is tight, so it is not that we lack a better analysis — O(1) is genuinely
false.

**Either optimisation alone gives O(log n). Both together give α(n). Neither gives O(n).**

The honest interview answer is: *"effectively constant — α(n) ≤ 4 in practice — but it is amortised
and provably not O(1)."* Saying just "O(1)" invites the follow-up.

---

# E. Recursion, DP, and C++

### Q24. Recursion versus iteration — and what is tail recursion?

Every recursion can be rewritten iteratively; the compiler is doing it with an implicit stack.

**Recursion** is clearer when the problem is defined recursively — trees, divide and conquer,
backtracking. **Iteration** avoids the stack limit and the per-call overhead.

**Tail recursion** is when the recursive call is the *last* thing the function does, with no pending
work after it. Such a call can reuse the current stack frame instead of pushing a new one, making it
a loop. GCC does this at `-O2`.

```cpp
int fact(int n, int acc = 1) { return n <= 1 ? acc : fact(n - 1, n * acc); }   // tail
int fact(int n)              { return n <= 1 ? 1 : n * fact(n - 1); }          // NOT tail
```

The second has a pending multiply after the call returns, so the frame must stay.

**Do not rely on it.** It is an optimisation, not a language guarantee, it is off at `-O0`, and
C++ does not require it. If depth is the concern, write the loop.

---

### Q25. Memoisation versus tabulation.

Two ways to implement the same recurrence.

**Memoisation** is top-down: the plain recursion plus a cache. Easier to write — three lines added
to a recursion you already have — and it computes **only the states you actually reach**, which
matters when the state space is sparse.

**Tabulation** is bottom-up loops. No recursion, so **no stack limit**, and it space-optimises
easily because you can see which cells the recurrence reads.

| | Memoisation | Tabulation |
|---|---|---|
| Written as | recursion + cache | loops |
| Computes | reachable states only | every state |
| Risk | **stack overflow** | none |
| Space optimisation | hard | **easy** |
| Requires | nothing | a valid **dependency order** |

**The interview move is to write the memoised version, then say "this tabulates to a loop, and then
the space drops to O(1)".** That sequence — recursion → memo → table → space — is what they are
listening for.

Real example from this repo: `../26_dp`'s memoised `RemovingDigits` is correct but
**aborts with `0xC00000FD` at n = 10⁶**, which is exactly the problem's constraint. The tabulated
version is not optional there.

---

### Q26. Greedy or DP — how do you know greedy is safe?

Greedy commits to a locally best choice and never reconsiders. It is correct **only** when you can
prove that choice can never need undoing. Two properties:

- **Greedy choice property** — a globally optimal solution exists that contains the locally best
  choice.
- **Optimal substructure** — what remains after that choice is the same problem, smaller.

**The counterexample to have ready:** coin change with coins `{1, 3, 4}` and amount 6. Greedy takes
4, then 1, then 1 — three coins. The optimum is `3 + 3` — two. Greedy happens to work on real
currency, which is exactly why the intuition is so persistent and so wrong.

Where greedy *is* provable: interval scheduling by earliest end time (an exchange argument), Huffman
coding, Kruskal and Prim (the cut property), and Dijkstra (non-negative weights).

**If you cannot state the exchange argument, use DP.** DP tries every choice, so it cannot be wrong
about this — it is only slower.

---

### Q27. Stack memory versus heap memory. What causes a stack overflow?

**Stack** — automatic storage: local variables, parameters, return addresses. Allocation is
literally moving a pointer, so it is essentially free, and it is freed automatically when the frame
exits. It is small and **fixed** — typically 1–8 MB.

**Heap** — dynamic storage: `new`, `malloc`, and the buffers inside `vector`/`string`. Large,
limited by RAM, but allocation costs real work and you (or a smart pointer) must free it.

**A stack overflow is running out of that fixed region.** Three causes:

1. **Recursion too deep** — the usual one. Measured here: ~60,000 frames survive, 80,000 aborts
   with `0xC00000FD`.
2. **Infinite recursion** — a missing or unreachable base case.
3. **A huge local array** — `int a[1000000];` inside a function is 4 MB of *stack*. Make it global
   or `static`, or use a `vector` (whose data lives on the heap).

**The symptom to recognise: the program prints nothing.** `cout` is buffered, and the buffer dies
with the process — so output you "know" was produced never appears. A program that produces no
output at all is far more likely to have crashed than to have computed nothing.

---

### Q28. Pass by value, by reference, by pointer.

```cpp
void byValue(vector<int> v);          // COPIES the whole vector
void byRef(vector<int>& v);           // no copy; can modify the caller's object
void byConstRef(const vector<int>& v);// no copy; cannot modify  <-- the default for reading
void byPtr(vector<int>* v);           // no copy; can be null; needs -> and a null check
```

**`const&` is the default for anything bigger than a pointer.** Passing a `vector` by value copies
every element — for a 10⁶-element vector inside a loop, that is the whole performance story.

Reference versus pointer: a **reference cannot be null and cannot be reseated**, which makes it the
safer choice. Use a pointer when absence is meaningful (`nullptr` as "no node"), when you need to
rebind, or for C interop. Tree and list nodes are pointers precisely because `NULL` means something.

**This bit the code in this repo.** `../25_heap/heap_sort.cpp` takes `vector<int> v` by value —
measured: `{5,1,4,2,8}` is completely unchanged after the call, because everything the function did
happened to a copy. And a DSU passing `par` by value would silently discard every path compression.

---

### Q29. Shallow copy versus deep copy.

A **shallow** copy duplicates the members; pointer members end up pointing at the *same* underlying
object. A **deep** copy duplicates what the pointers point to as well.

The default compiler-generated copy constructor is **shallow**. For a class holding a raw pointer
that is usually a bug in three ways: both objects mutate the same buffer, both destructors free it
(**double free**), and one being destroyed leaves the other **dangling**.

That is the **Rule of Three**: if you need a destructor, you almost certainly need a copy
constructor and a copy assignment operator too. (Rule of Five once you add moves.)

**The clean answer is to not own raw pointers** — `vector`, `string` and `unique_ptr` all do the
right thing already.

The interview version of this is **LC 138 Copy List with Random Pointer** (#48): a shallow copy
would leave the new nodes' `random` pointers aimed at the *old* list. The map from old node to new
node is precisely the deep-copy machinery.

---

### Q30. What is undefined behaviour, and what is iterator invalidation?

**Undefined behaviour** means the standard imposes no requirement at all — the program may crash,
produce garbage, or appear to work. The last is the dangerous one, because it works on your machine
and fails in production.

Common sources: reading out of bounds, dereferencing null or dangling pointers, signed integer
overflow, using an uninitialised variable, and modifying the same object twice in one expression.

Measured on this toolchain — both silent, neither diagnosed:

```
INT_MAX + 1 = -2147483648
13!         = 1932053504        (true value 6227020800)
```

**Iterator invalidation** is a specific and very common case. Operations that reallocate destroy
every existing iterator, pointer and reference into the container:

```
size=2 capacity=2
data pointer moved: YES        <-- after one push_back; any old iterator is now dangling
after reserve(100), pointer moved: no
```

The rules worth carrying:

- **`vector`** — any reallocation invalidates **everything**; `erase` invalidates from the erase
  point onward. `reserve` up front prevents the reallocation case, as measured above.
- **`map` / `set`** — only the erased element's iterator is invalidated. Everything else survives,
  including across insertions. This is why the LRU cache can hold node pointers safely.
- **`unordered_map`** — a **rehash** invalidates all *iterators*, but **references to elements stay
  valid** (the nodes themselves do not move).

The classic bug is erasing while iterating:

```cpp
for (auto it = v.begin(); it != v.end(); ++it)
    if (bad(*it)) v.erase(it);          // UB: it is invalid, and ++it is on a dead iterator

for (auto it = v.begin(); it != v.end(); )
    it = bad(*it) ? v.erase(it) : it + 1;    // correct: erase RETURNS the next valid iterator
```

---

## Six sentences worth memorising verbatim

If you take nothing else from this file:

1. **"O is an upper bound, Θ is tight; comparison sorting is Ω(n log n), which is a bound on the
   problem, not on any algorithm."**
2. **"Amortised is the average over a worst-case sequence, not over random input — `push_back` is
   O(1) amortised because the capacity doubles."**
3. **"Dijkstra finalises a node when it is popped, so a negative edge discovered later breaks it —
   that is Bellman–Ford's job."**
4. **"Greedy is only correct with an exchange argument; coin change with `{1,3,4}` and amount 6 is
   the counterexample."**
5. **"The recursion stack counts as space: O(h), which is O(log n) balanced and O(n)
   degenerate."**
6. **"A program that prints nothing has almost certainly crashed — `cout` is buffered and the
   buffer dies with the process."**
