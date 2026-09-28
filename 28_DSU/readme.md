# 28 — Disjoint Set Union (Union–Find)

One structure, two operations, and a complexity that is effectively constant:

```
find(x)      which group is x in?
unite(a, b)  merge the group containing a with the group containing b
```

That is all it does. It cannot list a group's members, cannot **split** a group, and cannot answer
anything about paths. What it can do is answer *"are these two things already connected?"* in
effectively O(1), **while the connections are still arriving** — and that is a question `../27_graphs`
answers only by re-running a whole traversal.

| | DFS / BFS | DSU |
|---|---|---|
| Components of a fixed graph | O(V + E) | O(E α(V)) |
| Components after *each* new edge | O(E) per edge → **O(E²)** | **O(α(V)) per edge** |
| Shortest path | ✓ | ✗ |
| Undo a merge | n/a | ✗ (without extra machinery) |

**The dynamic case is where DSU wins outright.** If edges arrive one at a time and you need an
answer after each, DSU is the structure; a traversal is not.

---

## 1. The idea: a forest where only the root matters

Every element points at a parent. Follow parents to a **root**, and the root *is* the group's name.
Two elements are in the same group exactly when they reach the same root.

```
par = [0, 0, 1, 3, 3, 5]

  0        3      5          three groups: {0,1,2}  {3,4}  {5}
 / \        \
1   ...      4
|
2
```

The **shape of the tree is meaningless** — only the root identity matters. That freedom is what
makes both optimisations legal: you may rewire anything as long as the reachable root does not
change.

---

## 2. `find` with path compression

```cpp
int find(int x) {
    if (parent[x] == x) return x;
    return parent[x] = find(parent[x]);      // point x straight at the root on the way back up
}
```

Your `DSU.cpp:7` writes this as one expression and it is exactly right:

```cpp
return par[x]=(par[x]==x)?x:find(par,par[x]);
```

**The assignment is the whole optimisation.** Every node on the path gets re-pointed at the root, so
the next `find` on any of them is one hop. A `find` that merely *returns* the root without
reassigning is correct and slow.

**The recursive form overflows on deep chains.** Measured on this toolchain: a chain of 60,000
survives, 80,000 gives `0xC00000FD` (STACK_OVERFLOW) — and it prints nothing, because `cout` is
buffered. The iterative two-pass version has no depth limit:

```cpp
int find(int x) {
    int root = x;
    while (parent[root] != root) root = parent[root];       // pass 1: locate
    while (parent[x] != root) { int nxt = parent[x]; parent[x] = root; x = nxt; }  // pass 2: compress
    return root;
}
```

---

## 3. `unite` — and the two things that must be right

```cpp
bool unite(int a, int b) {
    a = find(a); b = find(b);                // 1. ALWAYS work with the ROOTS
    if (a == b) return false;                // 2. already together -- nothing to do
    if (sz[a] < sz[b]) swap(a, b);           // 3. attach the smaller under the larger
    parent[b] = a;                           //    parent[b], and b IS a root
    sz[a] += sz[b];
    return true;
}
```

**`parent[b]` where `b` has already been reassigned to `find(b)`.** Writing `parent[originalB] = rootA`
re-points one *node* and leaves the rest of its tree behind — **the set splits instead of merging**.
The bug is invisible whenever the node you happen to name is already its own root, which is most
small test cases.

**`if (a == b) return false;`** is not just an optimisation. The boolean return — *"did this
actually merge anything?"* — is what makes DSU a **cycle detector**, and it is the basis of Kruskal
and of LC 684.

---

## 4. Union by size, or union by rank

Both keep the trees shallow by attaching the smaller thing under the larger. They differ in what
"smaller" means:

| | Union by **size** | Union by **rank** |
|---|---|---|
| Tracks | number of nodes in the tree | an upper bound on the height |
| Update | `sz[a] += sz[b]` always | `if (rank[a] == rank[b]) rank[a]++` — **only on a tie** |
| Bonus | `size(x)` is free, and many problems want it | none |
| Same complexity | ✓ | ✓ |

**Prefer union by size.** It is no harder, and a surprising number of problems ("largest component",
"count pairs", "is the whole thing connected") want the size anyway.

Two things that go wrong with rank:

- **The rank belongs to the root**, not to the node you were asked about. `rank[a]++` where `a` is
  the caller's argument ranks an arbitrary node.
- **It increments only when the ranks are equal.** Putting a rank-1 tree under a rank-3 tree does
  not make the taller one taller. Incrementing unconditionally turns rank into a call counter —
  measured: seven unions against one root drove the rank to **7** when the true rank never left 1.

Together, path compression **and** union by size give **O(α(n))** amortised, where α is the inverse
Ackermann function. α(n) ≤ 4 for any n you will ever see, so it is constant in practice — but it is
*amortised* and it is *not* actually O(1), which is the pedantic point interviewers occasionally
want.

**Either optimisation alone gives O(log n). Neither gives O(n).**

---

## 5. What DSU is for

| Signal in the problem | Why DSU |
|---|---|
| "are these two connected?" asked **repeatedly** | that is literally `find(a) == find(b)` |
| edges arrive **one at a time** and you answer after each | a traversal would restart every time |
| "group these things that are transitively related" | equality is transitive; so is union |
| "how many groups / how large is this group?" | maintain a counter and a size array |
| "which edge creates a cycle?" | the first `unite` that returns false |
| MST | Kruskal is sort + DSU |

**And what it is not for:** shortest paths, directed graphs (connectivity there means SCC, which is
not symmetric), anything needing the members of a group without an extra pass, and anything needing
a *split*. Union is one-way.

**"Transitive grouping" is the phrase to listen for.** Accounts merge, equations, similar strings,
stones on shared rows — all of them are "these two belong together, therefore everything they
already belong with belongs together too", which is exactly what union does.

---

## 6. The variants worth knowing

- **Component count** — start at `n`, decrement on every successful `unite`. Turns "how many
  islands / provinces / friend circles" into one integer with no second pass.
- **DSU on a grid** — map cell `(r, c)` to `r * cols + c` and you have a DSU over `m*n` elements.
  This is how LC 305 (islands appearing one at a time) is solved at all; BFS cannot do it.
- **DSU with an offset / weighted DSU** — store the relation to the parent alongside the pointer,
  so you can answer "how does `a` relate to `b`" rather than just "are they related". This is how
  LC 399 (evaluate division) and bipartite-checking-by-DSU work.
- **DSU on strings** — map each distinct string to an integer id first, with a `map<string,int>`.
  The DSU itself never sees a string.

---

## Interview Q&A

**Q1. What does DSU do and what is its complexity?**
It maintains a partition of `n` elements under merges, supporting `find` (which set?) and `union`
(merge two sets). With path compression and union by size/rank, both are **O(α(n)) amortised** —
effectively constant, α(n) ≤ 4 for any real input.

**Q2. What is path compression?**
During `find`, re-point every node on the path directly at the root. It costs nothing extra (you
already walked the path) and it flattens the tree for every future query. Without it a chain of
unions can build an O(n)-deep tree.

**Q3. Union by rank or by size — and why either?**
Always attach the smaller/shorter tree under the larger, so depth grows as slowly as possible.
Rank tracks an upper bound on height; size tracks node count. Same complexity — size is preferable
because problems often want the component size anyway.

**Q4. Why is it α(n) and not O(1)?**
Because the bound is amortised over a sequence of operations, not per operation, and Tarjan proved
Θ(α(n)) is tight for this structure. A single `find` can still cost O(log n); it is the total across
m operations that is O(m α(n)).

**Q5. Detect a cycle in an undirected graph with DSU.**
For each edge, if both endpoints already share a root, that edge closes a cycle. Otherwise unite
them. O(E α(V)), one pass, no recursion — and it works while the edges are still arriving, which
DFS cannot.

**Q6. Why does that not work for directed graphs?**
Union is symmetric and directed reachability is not. `a → b` does not imply `b → a`, so merging them
loses the information the cycle question depends on. Directed cycles need the three-colour DFS or
Kahn's algorithm (`../27_graphs` §6).

**Q7. Explain Kruskal's algorithm.**
Sort all edges by weight; take each edge whose endpoints are in different components, merging as you
go; stop at `n-1` edges. DSU is what makes "different components" an O(α) test. O(E log E), and the
sort dominates.

**Q8. Can you undo a union?**
Not with path compression — it destroys the original structure. **Rollback DSU** keeps a stack of
the changes and skips compression (using union by rank only), giving O(log n) per operation with
undo. That is what offline dynamic-connectivity solutions use.

**Q9. Count the number of connected components as edges arrive.**
Initialise a counter to `n` and decrement on each successful union. O(1) per query at any point.
Re-running DFS after every edge would be O(E) each time.

**Q10. How would you apply DSU to a grid?**
Flatten `(r, c)` to `r * cols + c`. Union each land cell with its already-processed left and top
neighbours — right and bottom are unnecessary because they will union backwards when reached. This
is the only practical solution to LC 305.

---

## Your file

`DSU.cpp` has `find` right and `Union` wrong in two ways — see `solution.md` for the measured
evidence and the corrected version. There is no `readme.md` problem list in this folder, so
`questions.md` is sourced from the standard DSU set.
