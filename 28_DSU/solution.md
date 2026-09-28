# 28 — Disjoint Set Union: Solutions

Approach and pseudocode first. Full C++ only for **fundamentals**, **implementations**, **hard**
problems and **tricks**. Everything shown has been compiled and run.

---

## Section 1 — Implementation

### 1. DSU with path compression and union by size *(fundamental — write this once)*

```cpp
class DSU {
    vector<int> parent, sz;
    int comps;
public:
    DSU(int n) : parent(n), sz(n, 1), comps(n) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);      // compress on the way back up
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);                // 1. work with the ROOTS from here on
        if (a == b) return false;                // 2. already together
        if (sz[a] < sz[b]) swap(a, b);           // 3. smaller under larger
        parent[b] = a;                           //    b is a ROOT, not the caller's argument
        sz[a] += sz[b];
        comps--;
        return true;
    }
    bool connected(int a, int b) { return find(a) == find(b); }
    int  size(int x)             { return sz[find(x)]; }
    int  count()                 { return comps; }
};
```

Four things earn their place:

- **`a = find(a); b = find(b);`** — reassigning the parameters means every later line is
  automatically talking about roots. This is the discipline that prevents the `parent[b]` bug.
- **`bool` return.** *"Did this actually merge anything?"* is a cycle detector, and Sections 3, 5
  and half of 4 are built on it.
- **`comps`** — one integer, and "how many groups" becomes O(1) instead of a second pass.
- **`sz`** — free, and half the problems below want it.

Verified: 6 singletons → unite(0,1) is new → the same call again returns false → count drops
correctly → after unite(1,2) nodes 0 and 3 are connected → `size(0)` is 4.

**The decisive test**, and the one your file fails: build `{0,1,2}` and `{3,4,5}`, then
`unite(0,4)` — where **4 is not its own root**. Verified: one set afterwards, 4 and 5 still
together, `size(3)` is 6.

---

### 2. Union by rank *(implementation)*

```cpp
bool unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return false;
    if (rnk[a] < rnk[b]) swap(a, b);
    parent[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;      // ONLY on a tie, and ONLY on the root
    return true;
}
```

**Rank rises only when the two ranks are equal.** Attaching a rank-1 tree under a rank-3 tree leaves
the height at 3 — nothing got taller, so nothing should increment.

Verified: after seven unions against one root the rank is **1**; three redundant `unite(0,1)` calls
leave it at 1; and merging two rank-1 trees correctly produces rank 2.

Note rank is a *bound* on height, not the height — path compression flattens the tree without
lowering the stored rank. That is fine; it is only used to decide which side to attach.

---

### 3. Iterative `find` *(implementation — you will need this)*

```cpp
int find(int x) {
    int root = x;
    while (parent[root] != root) root = parent[root];        // pass 1: locate the root
    while (parent[x] != root) { int nxt = parent[x]; parent[x] = root; x = nxt; }
    return root;                                              // pass 2: compress
}
```

Two passes, no stack frames, identical result. Verified on a **200,000-deep** chain: the root comes
back correctly and both the tail and the middle of the chain point straight at it afterwards.

**The recursive version dies on the same input.** Measured on this toolchain: 60,000 survives,
80,000 gives `0xC00000FD` (STACK_OVERFLOW), and the program prints nothing because `cout`'s buffer
is lost with the process. With constraints of 10⁵ this is not a theoretical concern.

The `nxt` temporary is required — overwriting `parent[x]` first would destroy the link you still
need to walk.

---

### 4. Component count

```
comps = n
on every unite that returns true: comps--
```

That is it. **Every problem in Section 2 is this plus one loop.**

---

## Section 2 — Connectivity and counting

### 5 & 6. Number of Provinces — LC 547, LC 323

```cpp
DSU d(n);
for (int i = 0; i < n; ++i)
    for (int j = i + 1; j < n; ++j)
        if (isConnected[i][j]) d.unite(i, j);
return d.count();
```

**`j = i + 1`** — the matrix is symmetric, so the lower triangle is redundant work. O(n² α(n)),
dominated by reading the matrix.

LC 323 is the same with an edge list instead of a matrix: `for each edge: unite(u, v)`. Verified: 2
and 3 on the two LeetCode samples.

---

### 7. Number of Islands, DSU version — LC 200

```cpp
for each cell (i, j) that is land:
    islands++                                       // provisionally its own island
    if (i && grid[i-1][j] == '1' && unite(id, up))   islands--;
    if (j && grid[i][j-1] == '1' && unite(id, left)) islands--;
```

**Only look up and left.** Right and down will union backwards when the scan reaches them, so
checking all four directions is duplicated work — not wrong, just wasteful.

`id = i * cols + j` is the standard grid flattening. `islands--` fires only when `unite` **returns
true**, which is what stops a cell that touches the same island twice from being subtracted twice.

Verified: 1 and 3 on the LeetCode samples, matching the BFS version exactly.

**BFS is the better answer for this problem.** The DSU version is here because it is the only one
that extends to #17.

---

### 8. Graph Valid Tree — LC 261 *(trick — two conditions, one pass)*

```
if edges.size() != n - 1:  return false          # necessary, NOT sufficient
for each edge: if unite fails -> a cycle -> return false
return true
```

**Both checks are needed.** `n-1` edges alone allows a triangle plus an isolated node; connectivity
alone allows extra edges. Together they are exactly the definition of a tree.

With the edge count already checked, "no cycle" implies "connected" — so one failing `unite` is the
only other thing to look for. Equivalently: check `d.count() == 1` at the end.

---

### 9. Make Network Connected — LC 1319

```cpp
if ((int)connections.size() < n - 1) return -1;    // not enough cable, full stop
DSU d(n);
for (auto& c : connections) d.unite(c[0], c[1]);
return d.count() - 1;
```

**Two facts, and the arithmetic is trivial once you have both:** joining `c` components needs
exactly `c-1` cables, and any edge whose `unite` failed is a spare you can move. If there are at
least `n-1` cables in total, there are always enough spares.

Verified: 4 computers with 3 connections → **1**; 6 computers with 2 connections → **−1**.

---

## Section 3 — Cycle detection and MST

### 10 & 11. Cycle detection / Redundant Connection — LC 684

```
for each edge (u, v):
    if not unite(u, v):  this edge closes a cycle
```

**One line, and it works while the edges are still arriving** — which the DFS version cannot do.
O(E α(V)).

LC 684 asks for the **last** removable edge, and because you process edges in input order the
*first* failing `unite` already is it. Verified: `[[1,2],[1,3],[2,3]]` → `2 3`;
`[[1,2],[2,3],[3,4],[1,4],[1,5]]` → `1 4`.

**LC 685 (directed) is genuinely harder** — a node can have two parents, or there can be a cycle, or
both at once, and each needs a different edge removed. Do it after this one.

---

### 12. Kruskal — MST

```cpp
sort(edges.begin(), edges.end(), byWeight);
DSU d(n);
for (auto& e : edges)
    if (d.unite(e.u, e.v)) { total += e.w; if (++taken == n-1) break; }
return taken == n - 1 ? total : -1;
```

**O(E log E), dominated by the sort** — the DSU part is effectively linear. The `break` at `n-1`
edges is a real saving on dense graphs.

**Why greedy is correct** (the cut property): for any partition of the vertices, the lightest edge
crossing it is in some MST. Sorting and taking any edge that joins two components is exactly
repeated application of that. Being able to state it is the follow-up.

Verified at **16**, matching Prim on the same graph, and −1 when disconnected.

---

## Section 4 — Transitive grouping

### 13. Accounts Merge — LC 721 *(hard — the mapping layer is the problem)*

```
owner = map<email, account index>
for each account i, for each email:
    if the email is already owned:  unite(i, owner[email])
    else: owner[email] = i

then: group every email under find(owner[email]), sort within each group, prepend the name
```

**The DSU is over account indices, not emails.** Emails are the *evidence* that two accounts are the
same person; the accounts are what merge. Getting that the wrong way round is the usual wrong turn.

```cpp
map<int, set<string> > merged;
for (auto& kv : owner) merged[d.find(kv.second)].insert(kv.first);
```

**`set<string>` sorts the emails for free** — the problem requires it, and doing it here avoids a
separate sort pass.

Verified against the LeetCode sample: 4 accounts become 3, with `john00@`, `john_newyork@` and
`johnsmith@` merged under one John and `johnnybravo@` left as a separate one.

---

### 14. Equality Equations — LC 990 *(trick — the order of the two passes)*

```cpp
for (auto& e : eq) if (e[1] == '=') d.unite(e[0]-'a', e[3]-'a');    // ALL equalities FIRST
for (auto& e : eq) if (e[1] == '!' && d.connected(e[0]-'a', e[3]-'a')) return false;
return true;
```

**Two passes, and the order is the whole problem.** Checking an inequality before a later equality
has merged its two sides tests an incomplete partition and wrongly returns true.

26 letters, so the DSU is fixed-size. Verified: `a==b, b!=a` → false; `b==a, a==b` → true;
`a==b, b==c, a!=c` → **false** (transitivity caught); all-inequalities → true; `a!=a` → false.

---

### 15. Most Stones Removed — LC 947 *(hard — what to union)*

**The answer is `stones − components`.** Every connected group can be reduced to one stone: remove
them in reverse order of when they were connected, and each removal is legal because a
same-row-or-column neighbour still stands.

The naive union is stone-against-stone, O(n²). The trick:

```
union( row(stone),  COLS_OFFSET + col(stone) )
```

**Union the row index with the column index**, offsetting columns by a constant (10001) so row ids
and column ids cannot collide. Two stones sharing a row are then automatically in the same
component through that shared row node. **O(n α)**, and the count of components among *stone-touched*
ids is what you subtract.

Recognising that the rows and columns — not the stones — are the things to union is the entire
problem.

---

### 16. Smallest String With Swaps — LC 1202

```
union every swap pair
group the indices by root
for each group: collect the characters, sort them, write them back into the sorted indices
```

Any set of swaps within a component can realise **any permutation** of that component's characters
— which is why sorting is optimal and no ordering of the swaps matters. O(n log n).

---

## Section 5 — Where a traversal cannot go

### 17. Number of Islands II — LC 305 *(hard — the reason DSU exists)*

```cpp
for each position (r, c):
    if already land: record the current count and continue        // duplicates are possible
    mark it land; islands++
    for each of the 4 neighbours that is land:
        if unite(id, neighbourId) succeeded: islands--
    record islands
```

**Each new cell is its own island until proven otherwise**, and every successful merge with an
existing neighbour cancels one. A cell touching two cells of the *same* island only decrements
once, because the second `unite` returns false — which is the boolean return doing real work.

**O(k α(mn)).** BFS would be O(k · mn) — recomputing every island from scratch after each addition.
This is the problem that makes DSU non-optional.

Verified: `(0,0),(0,1),(1,2),(2,1)` on 3×3 → `1 1 2 3`, and a **repeated position** correctly leaves
the count unchanged.

---

### 18. Swim in Rising Water — LC 778

```
sort all cells by height ascending
add them one at a time, unioning with already-added neighbours
the answer is the height at which cell (0,0) and cell (n-1,n-1) first share a root
```

DSU as a **sweep over a threshold**. O(n² log n), dominated by the sort.

Dijkstra with `max` instead of `+` solves it too (`../27_graphs` §4 #27), and comparing them is the
exercise. The DSU form is the one that generalises to **offline queries**: sort the queries by
threshold alongside the edges and answer them all in one sweep — which is LC 1697.

---

## Bugs in your file — with evidence

`DSU.cpp` was compiled and run. **Nothing in `28_DSU/` was edited.**

The file compiles. `main` is empty, so nothing has ever executed — the functions were tested in a
harness against a reference DSU.

### `DSU.cpp:14` and `:17` — `par[b]` links the NODE, not its root

```cpp
int para=find(par,a);
int parb=find(par,b);
if(rank[para]>=rank[parb]){
    rank[a]++;
    par[b]=para;          // <-- should be par[parb] = para
}else{
    rank[b]++;
    par[a]=parb;          // <-- should be par[para] = parb
}
```

You compute `para` and `parb` correctly on lines 10–11 and then do not use them on the left-hand
side. Re-pointing `b` moves **one node**; the rest of `b`'s tree still roots at `parb`, so the two
sets do not merge — **`b` is pulled out on its own.**

The bug hides whenever the node you name is already its own root, which is true in most small tests.
Building `{0,1,2}` and `{3,4,5}`:

```
before        roots:  0 0 0 3 3 3      (2 sets)
Union(0,3)  -- 3 IS a root  ->  roots: 0 0 0 0 0 0   (1 set)   <-- looks fine
```

Now the same merge naming a non-root member instead:

```
Union(0,4)  -- 4 is NOT a root, its root is 3
after         roots:  0 0 0 3 0 3      (2 sets)  <-- WRONG, should be 1
```

**Measured consequences:** after `Union(0,4)`, `find(4) != find(5)` — two nodes that were in the
same set are no longer — and `find(0) != find(5)`, so the merge did not happen either. The reference
DSU on identical input gives one set.

Fix:

```cpp
if (para == parb) return;                 // see below
if (rank[para] < rank[parb]) swap(para, parb);
par[parb] = para;
if (rank[para] == rank[parb]) rank[para]++;
```

### `DSU.cpp:13` and `:16` — `rank[a]++` ranks the wrong node, and does it unconditionally

Two faults in one statement.

**Wrong node:** the rank is a property of the **root**. `rank[a]` where `a` is whatever the caller
passed in ranks an arbitrary member.

**Wrong condition:** rank should rise **only when the two ranks are equal**. Attaching a shorter
tree under a taller one does not increase the taller one's height.

**Measured** — `Union(0,1)` through `Union(0,7)`:

```
correct  rank[] : 1 0 0 0 0 0 0 0
yours    rank[] : 7 0 0 0 0 0 0 0
```

and three redundant `Union(0,1)` calls take `rank[0]` from 1 to **3**. Rank has become a call
counter. Correctness survives — rank only picks which side to attach — but the O(α) guarantee does
not, because the wrong side gets chosen and the trees can grow toward O(n) chains.

### `DSU.cpp:12` — no early return when `a` and `b` are already in the same set

`if (para == parb) return;` is missing. Three consequences:

1. Redundant unions keep inflating `rank` (measured above).
2. `par[b] = para` on an already-merged pair is a pointless write.
3. **You lose the cycle detector.** Returning `bool` — *"did this merge anything?"* — is what makes
   Kruskal, LC 684, LC 261 and LC 305 work. Without it, every one of those needs a separate `find`
   comparison first, which is the same work done twice.

### `DSU.cpp` — recursive `find` and no size tracking

Neither is a bug, but both are worth knowing:

- The recursive `find` **aborts with `0xC00000FD` on chains deeper than ~60,000** (measured). With
  union by rank working correctly you will never build such a chain — but the *broken* rank makes it
  reachable, so the two issues compound.
- Adding a `size[]` array alongside `rank[]` costs one line and makes "how big is this component"
  free. Half the problems in `questions.md` want it.

### `DSU.cpp:22` — `main` is empty

Six lines would have caught both bugs:

```cpp
vector<int> par(6), rank(6, 0);
for (int i = 0; i < 6; ++i) par[i] = i;
Union(par, rank, 0, 1); Union(par, rank, 0, 2);
Union(par, rank, 3, 4); Union(par, rank, 3, 5);
Union(par, rank, 0, 4);                                   // 4 is not a root
for (int i = 0; i < 6; ++i) cout << find(par, i) << " ";  // expect six identical values
```

---

## What you got right

**`find` is correct, and it is the harder half.**

```cpp
return par[x]=(par[x]==x)?x:find(par,par[x]);
```

Path compression written as a single expression: recurse to the root, and **assign the result back
into `par[x]` on the way out**. That assignment is the entire optimisation, and it is the part people
leave out — a `find` that only *returns* the root is correct and asymptotically much worse. Getting
this right in one line, unaided, is the good news in this file.

**The comment is the right mental model.**

```cpp
// return the group/cluster x belongs to
```

`find` returns an **identifier for a group**, and the identifier happens to be a node. Thinking of it
as "which group" rather than "walk to the top of the tree" is what makes `find(a) == find(b)` read
naturally, and it is why the shape of the tree is allowed to change freely.

**You reached for union by rank rather than plain union.** Attaching arbitrarily is the version most
people write first, and it degrades to O(n) chains. You knew the optimisation existed and attempted
it — the two bugs are in the *bookkeeping*, not in the idea. Both faults (wrong index, wrong
condition) are one-token fixes.

**Passing `par` and `rank` by reference** — `vector<int>&` on every parameter — is correct and
non-obvious. Passing by value would copy the arrays on every call and silently discard every
compression, which is exactly the failure mode that makes DSU look slow for no visible reason.

**The two functions are cleanly separated**, with `find` doing no merging and `Union` doing no
searching beyond the two lookups it needs. That separation is what will let you drop in the
iterative `find` from §1 #3 without touching `Union` at all.
