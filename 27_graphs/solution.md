# 27 — Graphs: Solutions

Approach and pseudocode first. Full C++ only for **fundamentals**, **implementations**, **hard**
problems and **tricks** — which for graphs means the algorithm templates, since every other problem
in a group is an edit of one. Everything shown has been compiled and run.

Representation used throughout: `vector<vector<int> > adj` for unweighted,
`vector<vector<pair<int,int> > > adj` for weighted, with `adj[u] = (neighbour, weight)`.

---

## Section 1 — Representation and traversal

### 1. Build all three representations *(implementation)*

You have three of the four already. The conversions are the exercise:

| From → to | How |
|---|---|
| edge list → list | `adj[u].push_back(v)`, and `adj[v].push_back(u)` if undirected |
| edge list → matrix | `m[u][v] = 1` (or the weight) |
| matrix → list | scan the row, keep the non-zeros — **O(V²)** whatever the edge count |
| list → matrix | walk every adjacency — O(V + E), then the matrix costs O(V²) space |

**The asymmetry is the lesson.** Going to a matrix always costs O(V²); going to a list costs what
the graph actually contains. That is the whole argument for the list.

---

### 2. BFS *(fundamental)*

```cpp
vector<int> bfsDist(vector<vector<int> >& adj, int src) {
    vector<int> dist(adj.size(), INT_MAX);
    queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        for (size_t i = 0; i < adj[cur].size(); ++i) {
            int nei = adj[cur][i];
            if (dist[nei] == INT_MAX) {        // unvisited
                dist[nei] = dist[cur] + 1;
                q.push(nei);                    // marked at ENQUEUE time
            }
        }
    }
    return dist;
}
```

**O(V + E) time, O(V) space.** Verified on the author's own 7-node sample: from node 0 the
distances are `0 1 1 2 4 2 3`, and from node 4 they are `4 5 3 2 0 4 1`.

Using `dist` as the visited marker means the two can never disagree — a separate `visited` set is
a second source of truth for no benefit.

---

### 3. DFS and all simple paths *(fundamental — and where your file breaks)*

```cpp
void allPaths(vector<vector<int> >& adj, int cur, int dest,
              vector<bool>& onPath, vector<int>& path, vector<vector<int> >& out) {
    path.push_back(cur);
    if (cur == dest) { out.push_back(path); path.pop_back(); return; }
    onPath[cur] = true;
    for (size_t i = 0; i < adj[cur].size(); ++i) {
        int nei = adj[cur][i];
        if (!onPath[nei]) allPaths(adj, nei, dest, onPath, path, out);   // nei, NOT cur
    }
    onPath[cur] = false;                        // UNDO -- this node may be on another path
    path.pop_back();
}
```

Two things, and your file gets one of each wrong:

- **Test the neighbour.** `onPath[cur]` was just set true, so testing it means the loop body never
  executes.
- **`onPath`, not `visited`.** A node not on the *current* path can legitimately appear on a
  different one. Clearing it on the way out is what makes enumeration complete — the same
  backtracking discipline as `../10_Recursion` §4 and `../21_tree` §2 #15.

Verified: two simple paths from 0 to 6 in the sample graph — `0 1 5 2 3 6` and `0 2 3 6` — and zero
paths across a disconnected pair.

---

### 4. Centre of Star Graph — LC 1791 *(trick)*

```
return the value that appears in BOTH edges[0] and edges[1]
```

Three comparisons, O(1). In a star every edge touches the centre, so any two edges share exactly
one node — the centre. **Building an adjacency list or counting degrees is O(V) work for a problem
that has none**, and that is what the question is checking.

---

### 5. Connected components — *(yours works)*

```
count = 0
for each node i:
    if not visited[i]:  count++;  dfs(i)
```

The outer loop is the whole algorithm — DFS explores one component, and the loop finds the next
unvisited node to start from. O(V + E).

Verified on your `connectedComponent.cpp`: 1 component on the sample graph, 4 on `{0,1} {2,3} {4}
{5}`, 5 on five isolated nodes, 0 on the empty graph.

---

### 6. Keys and Rooms — LC 841

Reachability from node 0, where `rooms[i]` *is* the adjacency list. DFS or BFS from 0, then check
whether every room was visited. O(V + E).

The only thing to notice is that it is #5 asking a different question about the same traversal.

---

## Section 2 — Grids and implicit graphs

### 7. Flood Fill — LC 733 *(fundamental, with one trap)*

```cpp
vector<vector<int> > floodFill(vector<vector<int> > img, int sr, int sc, int newColor) {
    int old = img[sr][sc];
    if (old == newColor) return img;            // WITHOUT THIS: infinite loop
    int m = img.size(), n = img[0].size();
    queue<pii> q; q.push(make_pair(sr,sc)); img[sr][sc] = newColor;
    int dr[] = {-1,1,0,0}, dc[] = {0,0,-1,1};
    while (!q.empty()) {
        int r = q.front().first, c = q.front().second; q.pop();
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr>=0 && nr<m && nc>=0 && nc<n && img[nr][nc] == old) {
                img[nr][nc] = newColor; q.push(make_pair(nr,nc));
            }
        }
    }
    return img;
}
```

**The `old == newColor` guard is the entire difficulty.** Recolouring a cell to the colour it
already has means it still matches `old`, so it is enqueued again, forever. Verified: filling a
grid of zeros with 0 terminates and returns the grid unchanged.

The `dr`/`dc` arrays are the reusable part — every other problem in this section uses them
verbatim.

---

### 8 & 9. Number of Islands / Max Area — LC 200, LC 695 *(fundamental)*

```cpp
for every cell:
    if it is land:
        count++
        flood the whole island, marking each cell as water when ENQUEUED
```

**Mark on enqueue.** Marking on dequeue lets the same cell be pushed by several neighbours before
it is processed, and the island is counted correctly but the queue grows to O(mn) duplicates.

Verified on both LeetCode samples: 1 and 3.

LC 695 is the same sweep returning the size of each flood instead of incrementing a counter. There
is a **DSU version** of #8 in `../28_DSU` §2 — worth doing both, because the DSU one is the only
form that extends to LC 305 (islands appearing one at a time).

---

### 10. Rotting Oranges — LC 994 *(fundamental — multi-source BFS)*

```cpp
seed the queue with EVERY rotten orange, and count the fresh ones
while the queue is not empty and fresh > 0:
    sz = q.size()                       // one level == one minute
    process exactly sz cells, rotting their fresh neighbours
    minutes++
return fresh == 0 ? minutes : -1
```

**Every source goes in before the loop starts.** That is what makes it one BFS instead of k, and
the wavefront then spreads from all of them simultaneously — which is exactly the physics of the
problem.

**`int sz = q.size()` is the same level-boundary line as `../21_tree` §3.** Without it you cannot
tell where one minute ends.

**Guard the "no fresh oranges" case** — the answer is 0, not the number of levels the loop would
otherwise run. Verified: sample 1 → 4, sample 2 → −1 (one orange walled off), all-rotten → 0.

---

### 11. 01 Matrix — LC 542

Your own note says it: **take 0 as a source**. Every zero is a BFS source; the first time a cell is
reached, that is its distance to the *nearest* zero.

```
push every 0, dist = 0;  every 1 starts at -1 (unvisited)
BFS outward; dist[nei] = dist[cur] + 1
```

O(mn). The naive version — BFS from each 1 looking for a 0 — is O((mn)²) and is the solution this
one replaces. **Inverting the direction of the search is the trick**, and it is the same idea as
#12.

Verified against the LeetCode sample: `0 0 0 / 0 1 0 / 1 2 1`.

---

### 12 & 16. Surrounded Regions / Number of Enclaves — LC 130, LC 1020

**Solve the complement.** Instead of hunting for regions that are enclosed, start from the border
and mark everything reachable from it — those are the ones that are *not* enclosed. Everything left
over is.

```
for each border cell that is land:  flood it and mark SAFE
then: every unmarked land cell is surrounded
```

O(mn). Trying to detect enclosure directly means tracking whether a flood ever touched the edge,
which works but is fiddlier and easy to get wrong on a region that touches the border twice.

LC 1020 is the same walk counting the leftovers instead of flipping them.

---

### 13. Coloring A Border — LC 1034

A cell is on the border of its component when it is on the grid edge, **or** at least one of its
four neighbours is a different colour.

**Collect the border cells first, recolour afterwards.** Recolouring during the traversal changes
the very comparison the border test depends on — a neighbour already repainted looks like a
different colour and its partner is wrongly flagged. That ordering is the whole problem.

---

### 14. Minimum Knight Moves — LC 1197

BFS on an **infinite** board with 8 knight moves. Two things make it tractable:

- **Symmetry:** the answer for `(x, y)` equals the answer for `(|x|, |y|)`, so fold into one
  quadrant.
- **Bound the search.** Allow a small negative margin (about `-2`) rather than the full plane —
  optimal knight routes occasionally step backwards near the origin, but never far.

Without the fold, BFS explores four times the area for the same answer.

---

### 15. Word Ladder — LC 127 *(hard — the graph is not given)*

Nodes are words; two words are adjacent when they differ in exactly one letter. **The adjacency is
generated, never stored.**

```
BFS from beginWord, level = 1
for the current word, for each position i, for each letter c in a..z:
    candidate = word with position i replaced by c
    if candidate is in the dictionary set: enqueue, ERASE it from the set
```

**Erasing from the dictionary as you enqueue is the visited set** — one container doing two jobs,
and it prevents the same word entering at a later level.

O(N · L · 26) where L is the word length. The bucket optimisation — pre-group words under wildcard
keys like `h*t` — replaces the 26-letter scan with a map lookup and is the follow-up.

**Bidirectional BFS** — search from both ends and stop when the frontiers meet — roughly square
-roots the explored volume. Worth naming.

---

## Section 3 — Cycle detection and topological sort

### 17. Clone Graph — LC 133 *(trick)*

You cannot set a clone's neighbour list until the neighbours exist. An `unordered_map<Node*, Node*>`
from original to copy removes the problem:

```
clone(node):
    if node is in the map: return map[node]        // ALSO the visited check
    copy = new Node(node->val)
    map[node] = copy                                // register BEFORE recursing
    for each nei: copy->neighbours.push_back(clone(nei))
    return copy
```

**Register the copy in the map before recursing**, or a cycle re-enters `clone` on a node that is
half-built and recurses forever. Same structure as LC 138 in `../24_maps` §2 #9.

---

### 18. Cycle detection, undirected

```cpp
if (parent[nei] == UNVISITED) { parent[nei] = cur; enqueue(nei); }
else if (nei != parent[cur]) return true;      // visited, and not where I came from
```

The parent check excludes the edge you arrived on — otherwise every single edge looks like a
two-node cycle. Verified: the sample graph has a cycle, a tree does not, a single edge does not, and
a cycle in the *second* component is still found because the outer loop restarts everywhere.

**DSU gives this in one line** — if `unite(u, v)` returns false the edge closes a cycle. See
`../28_DSU` §1 #4.

---

### 19. Cycle detection, directed *(trick — three colours)*

```cpp
bool dirCycle(vector<vector<int> >& adj, int u, vector<int>& colour) {
    colour[u] = 1;                             // 1 = on the current recursion stack
    for (size_t i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i];
        if (colour[v] == 1) return true;       // back edge -> cycle
        if (colour[v] == 0 && dirCycle(adj, v, colour)) return true;
    }
    colour[u] = 2;                             // 2 = finished, and provably cycle-free
    return false;
}
```

**Two colours are not enough.** With only visited/unvisited, the diamond `0→1, 0→2, 1→2` reports a
cycle that is not there: node 2 is revisited, but from a *finished* branch, not an ancestor.

Verified: a chain has no cycle, adding a back edge creates one, a diamond has none, a self-loop
does.

---

### 20–22. Topological sort — LC 207, LC 210 *(fundamental)*

```cpp
vector<int> topoKahn(vector<vector<int> >& adj) {
    int n = adj.size();
    vector<int> indeg(n, 0), order;
    for (int u = 0; u < n; ++u)
        for (size_t i = 0; i < adj[u].size(); ++i) indeg[adj[u][i]]++;
    queue<int> q;
    for (int i = 0; i < n; ++i) if (indeg[i] == 0) q.push(i);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (size_t i = 0; i < adj[u].size(); ++i)
            if (--indeg[adj[u][i]] == 0) q.push(adj[u][i]);
    }
    if ((int)order.size() != n) return vector<int>();    // fewer than V -> a cycle
    return order;
}
```

**The length check is the cycle detector**, free of charge. Nodes inside a cycle never reach
in-degree 0, so they never enter the queue. LC 207 returns `order.size() == n`; LC 210 returns the
order itself. Same function.

O(V + E). Verified: a valid order comes out with every edge pointing forward, and a 3-cycle returns
empty.

**Variations that reuse it:** swap the `queue` for a `priority_queue` to get the lexicographically
smallest order; count levels instead of nodes for "minimum semesters".

---

### 23. Alien Dictionary — LC 269 *(hard)*

The sort is the easy half. **Building the graph is the problem:**

```
for each adjacent pair of words (a, b):
    find the first position where they differ -> edge a[i] -> b[i], then STOP
    if no difference and a is LONGER than b -> the input is invalid, return ""
```

Two traps: comparing every character pair instead of stopping at the first difference gives edges
that do not follow, and the prefix case (`"abc"` before `"ab"`) is impossible and must be rejected
before sorting. Then run Kahn over the letters that actually appear.

---

### 24. Eventual Safe States — LC 802

A node is safe when every path from it terminates — equivalently, when it reaches no cycle.

**Reverse every edge and run Kahn.** In the reversed graph, peeling nodes with in-degree 0 is
peeling nodes whose out-edges in the original all led to already-safe nodes. Everything Kahn emits
is safe; whatever is left sits on or feeds a cycle.

The direct version is the same three-colour DFS as #19, memoising "is this node safe".

---

## Section 4 — Shortest paths

### 25 & 26. Dijkstra — LC 743 *(implementation)*

```cpp
vector<int> dijkstra(vector<vector<pii> >& adj, int src) {   // adj[u] = (v, w)
    vector<int> dist(adj.size(), INT_MAX);
    priority_queue<pii, vector<pii>, greater<pii> > pq;      // MIN heap, (distance, node)
    dist[src] = 0;
    pq.push(make_pair(0, src));
    while (!pq.empty()) {
        int d = pq.top().first, u = pq.top().second; pq.pop();
        if (d > dist[u]) continue;                           // stale entry -- skip
        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i].first, w = adj[u][i].second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    return dist;
}
```

Three details:

- **`(distance, node)` in that order**, so the pair's natural ordering sorts by distance and no
  comparator is needed.
- **`greater<pii>` for a min heap** — the inversion from `../25_heap` §3.
- **`if (d > dist[u]) continue;`** — `priority_queue` has no decrease-key, so the idiom is to push a
  better entry and discard the outdated one when it surfaces.

**O((V + E) log V).** Verified: `0 3 1 4 7` on a 5-node weighted graph, and unreachable nodes stay
`INT_MAX`.

LC 743 is this with "the answer is the maximum finite distance, or −1 if anything is unreachable".

---

### 27. Path With Minimum Effort — LC 1631 *(trick — Dijkstra on a max)*

The cost of a path is the **largest single step on it**, not the sum. Dijkstra still applies,
because the relaxation is still monotone:

```
effort[v] = min( effort[v], max(effort[u], |height[v] - height[u]|) )
```

**`max` where you would normally write `+`.** Recognising that Dijkstra generalises to any monotone
combination — not just addition — is worth more than the problem. LC 778 is the same substitution.

Binary search on the answer plus a connectivity check is an equally valid O(mn log C) solution, and
in `../28_DSU` it becomes sort-plus-union.

---

### 28. Cheapest Flights Within K Stops — LC 787 *(hard — the Dijkstra trap)*

**Dijkstra gives the wrong answer here.** It finalises a node at its cheapest cost, but the
cheapest route may use too many stops, and the dearer-but-shorter route is then never explored.

Bellman–Ford relaxed exactly `k+1` times is the fix:

```
dist[src] = 0
repeat k+1 times:
    tmp = copy of dist                       # <-- the copy is essential
    for each edge (u, v, w):
        if dist[u] + w < tmp[v]:  tmp[v] = dist[u] + w
    dist = tmp
```

**Relax from a snapshot of the previous round.** Without the copy, an edge relaxed earlier in the
*same* round feeds the next one, and a path of more than `k+1` edges sneaks through. That single
`tmp` is the difference between right and wrong.

---

### 29. Bellman–Ford *(implementation)*

```cpp
for (int it = 0; it < n - 1; ++it)                     // n-1 rounds
    for (each edge e)
        if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v])
            dist[e.v] = dist[e.u] + e.w;

for (each edge e)                                      // one more round
    if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v])
        return false;                                  // still improving -> negative cycle
```

**Why `n-1`:** a shortest path has at most `n-1` edges, and round *i* finalises every path of
length *i*. **The `n`th round is the detector** — if anything still improves, no shortest path
exists because you can loop to reduce the cost forever.

**Guard `dist[e.u] != INF`** or `INF + w` overflows and creates phantom improvements.

O(V·E). Verified: correct distances through a negative edge (`0 4 1 5`), and a negative cycle
detected.

---

### 30. Floyd–Warshall *(implementation)*

```cpp
for (int k = 0; k < n; ++k)                // k OUTERMOST
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (d[i][k] + d[k][j] < d[i][j]) d[i][j] = d[i][k] + d[k][j];
```

`k` means "paths may now use intermediates 0..k". **Putting `k` inside is not a slower
Floyd–Warshall, it is a different and wrong algorithm** — the subproblems get consumed before they
are complete.

**O(V³), all pairs.** Worth it for V ≤ ~400. A negative value on the diagonal afterwards means a
negative cycle through that node. Verified: `0→3` correctly routes the long way for cost 9 rather
than taking the direct 10.

---

### 31. Shortest Path in a DAG

Topological sort, then relax edges in that order — **one pass, O(V + E)**.

Because every predecessor of a node is finalised before the node is reached, no priority queue is
needed and **negative weights are fine**. This beats Dijkstra on a DAG in both complexity and
generality, and it is the answer whenever a problem says "directed acyclic".

The longest path in a DAG is the same algorithm with `max` — and longest path in a *general* graph
is NP-hard, which is a good contrast to have ready.

---

## Section 5 — MST, bipartite, advanced

### 33 & 34. Prim and Kruskal *(implementation)*

**Prim** — grow one tree, always taking the cheapest edge leaving it:

```cpp
priority_queue<pii, vector<pii>, greater<pii> > pq;    // (weight, node)
pq.push(make_pair(0, 0));
while (!pq.empty() && taken < n) {
    int w = pq.top().first, u = pq.top().second; pq.pop();
    if (inMST[u]) continue;
    inMST[u] = true; total += w; taken++;
    for (auto& e : adj[u]) if (!inMST[e.first]) pq.push(make_pair(e.second, e.first));
}
return taken == n ? total : -1;                        // -1 == the graph is disconnected
```

**Kruskal** — sort every edge, add it if its ends are in different components (DSU):

```
sort edges by weight
for each edge: if unite(u, v) succeeded, take it
stop after n-1 edges
```

Both verified at **16** on the same graph, and both return −1 when the graph is disconnected. Prim
is `../25_heap`; Kruskal is `../28_DSU`. The `taken == n` check is what distinguishes "no MST" from
"an MST of weight 0".

---

### 35. Min Cost to Connect All Points — LC 1584

The graph is implicit: **every pair of points is an edge**, weighted by Manhattan distance. So
E = V², the graph is dense, and **Prim is the right choice** — Kruskal would sort V² edges.

Building the edge list explicitly for 1000 points is 500,000 edges; Prim never needs it, because it
can compute distances on demand.

---

### 37 & 38. Bipartite — LC 785, LC 886

```
colour the start 0; every neighbour gets the opposite colour
a neighbour that already has YOUR colour -> not bipartite
restart for every uncoloured node (the graph may be disconnected)
```

O(V + E). Verified: a 4-cycle is bipartite, a triangle is not, every tree is.

**The characterisation is the interview answer:** bipartite ⟺ no odd cycle. LC 886 is the same
algorithm after building the "dislike" edges, and DSU solves it too — union each person with the
*complement* group of their dislikes.

---

### 41 & 42. Bridges and articulation points — LC 1192 *(hard — Tarjan)*

```cpp
void bridgeDfs(...) {
    tin[u] = low[u] = timer++;
    for (int v : adj[u]) {
        if (v == parent) continue;
        if (tin[v] != -1) low[u] = min(low[u], tin[v]);        // back edge
        else {
            bridgeDfs(v, u, ...);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) bridges.push_back({u, v});    // THE test
        }
    }
}
```

- **`tin[u]`** — when the DFS first reached `u`.
- **`low[u]`** — the earliest `tin` reachable from `u`'s subtree using at most one back edge.
- **`low[v] > tin[u]`** — nothing under `v` can climb back to `u` or above it, so `(u,v)` is the
  only connection: a **bridge**.

**Articulation points are the same DFS with `>=` instead of `>`**, plus a special case: the root is
an articulation point exactly when it has more than one DFS child. One comparison apart, and doing
them together is what makes the difference stick.

Verified: on a triangle with a two-edge tail, the bridges are `(1,3)` and `(3,4)`; a bare cycle has
none.

---

### 43. Strongly Connected Components — Kosaraju *(hard)*

```
1. DFS the graph, pushing each node onto a stack when it FINISHES
2. reverse every edge
3. pop nodes from the stack; each DFS in the reversed graph marks exactly one SCC
```

**Why it works:** the finish-time order of the original graph is a topological order of the
*condensation* (the DAG of SCCs). Processing in that order on the reversed graph means each DFS
cannot escape its own component.

O(V + E), two passes. Verified: 3 SCCs on a graph with one 3-cycle plus two singletons; a chain
gives V singletons.

**Tarjan** does it in one pass with the same `low`/`tin` machinery as #41 — worth knowing that they
are the same idea.

---

### 45 & 46 — briefly

- **#45 Mother Vertex** — the last node to finish in a DFS sweep is the only candidate; run one
  more DFS from it to verify. O(V + E), and the "only candidate" argument is the point.
- **#46 Euler path** — a counting argument, not an algorithm. Circuit: connected and *every* degree
  even. Path: connected and *exactly two* odd degrees (the endpoints). Directed: in-degree equals
  out-degree everywhere for a circuit. **Hierholzer** actually builds it (LC 332).

---

## Bugs in your files — with evidence

All six `.cpp` files were compiled and run. **Nothing in `27_graphs/` was edited.**

All six compile. The three representation files run correctly. `bfs.cpp`, `dfs.cpp` and
`connectedComponent.cpp` never call their own functions from `main`, so running them produces
nothing — the functions were tested in a harness instead.

### `dfs.cpp:33` and `dfs.cpp:48` — the check is on the wrong node

```cpp
       visited.insert(src);
    for (auto nei : graph[src] ){
        if(not visited.count(src)){        // <-- src. should be nei.
           allPath(nei,end,path);
```

`visited.insert(src)` runs on the line above, so `visited.count(src)` is always 1 and
`not 1` is always false. **The loop body never executes.**

**Measured** on your own sample graph (`0-2 0-1 1-5 2-5 2-3 3-6 6-4`):

| Call | Yours | Correct |
|---|---|---|
| `anyPath(0,6)` | **false** | true |
| `anyPath(0,4)` | **false** | true |
| `allPath(0,6)` | **0 paths** | 2 (`0 1 5 2 3 6` and `0 2 3 6`) |

`anyPath(3,3)` returns true and a genuinely disconnected pair returns false — so **two of the four
test cases pass by accident**, which is why this bug survives casual testing. The function returns
the right answer whenever the right answer happens to be "the trivial one".

### `dfs.cpp:31` / `:46` — `visited` is never cleared between calls

`visited` is a global and `anyPath` never erases anything on the way out. A second call sees the
first call's marks. `allPath` does erase (line 40), correctly — the two functions disagree with each
other. Once the neighbour check is fixed, `anyPath` also needs the erase, or it will report false
for a path that exists.

Related: `anyPath` needs `visited.insert` **before** the loop and an `erase` after, which is the
`onPath` discipline from §1 #3.

### `bfs.cpp:23` — `INT8_MAX` is 127

```cpp
dist.resize(v,INT8_MAX);
```

**Measured:** on a 4-node graph with node 0 connected only to node 1, the result is
`0 1 127 127`. 127 is a perfectly legal distance in any graph with a path that long, so
"unreachable" and "127 steps away" become indistinguishable. Use `INT_MAX` from `<climits>`.

(This is the same family as the `INT16_MIN` sentinels in `../21_tree/nodeTree.cpp` and the
`INT16_MAX` in `../24_maps/topviewBt.cpp` — worth treating as a habit to break rather than three
separate slips.)

### `bfs.cpp:23` — `resize` does not overwrite existing elements

`resize(v, X)` only fills positions that did not exist. If the caller passes a vector that is
already `v` long, **nothing is initialised**.

**Measured** with a caller-supplied `vector<int> dist(4, 999)`:

```
dist before : 999 999 999 999
dist after  : 0 1 999 999        <-- nodes 2 and 3 kept the stale values
```

`dist.assign(v, INT_MAX)` is the fix — `assign` overwrites unconditionally.

### `bfs.cpp:20` — the `dest` parameter is never used

`bfs(int src, int dest, vector<int>& dist)` ignores `dest` entirely — the function computes
distances to *everything*, which is the more useful behaviour. Drop the parameter, or return early
when `dest` is dequeued.

### `bfs.cpp` / `dfs.cpp` / `connectedComponent.cpp` — `main` never calls the function

All three read a graph and stop. `bfs.cpp`'s `main` does not even call `display()`. Nothing has been
executed, which is presumably why the bugs above are still there — **one call in `main` would have
surfaced all of them.**

### `28_DSU/DSU.cpp:13,16` — `par[b] = para` links the NODE, not its root

```cpp
if(rank[para]>=rank[parb]){ rank[a]++;  par[b]=para; }
else                      { rank[b]++;  par[a]=parb; }
```

`para` and `parb` are computed correctly on lines 10–11, and then **`b` is used where `parb` was
meant.** Setting `par[b]` re-points one node; everything else in `b`'s tree still roots at `parb`.
**The set splits instead of merging.**

This is a genuinely nasty bug because it is invisible whenever the node you name *happens* to be its
own root. Two sets built as `{0,1,2}` and `{3,4,5}`:

```
before:     roots  0 0 0 3 3 3      (2 sets)
Union(0,3)  -- 3 IS its own root -> works, 1 set        <-- looks correct
Union(0,4)  -- 4 is NOT its own root
after:      roots  0 0 0 3 0 3      (2 sets)            <-- WRONG, should be 1
```

**Measured:** after `Union(0,4)`, nodes 4 and 5 are no longer in the same set, and 0 and 5 are not
either — node 4 was pulled out on its own. The fix is `par[parb] = para;` and `par[para] = parb;`.

### `28_DSU/DSU.cpp:13,16` — `rank[a]++` ranks the wrong node, unconditionally

Two faults in one line. The rank belongs to the **root** (`para`), not the queried node `a`; and it
should increase **only when the two ranks are equal** — attaching a shorter tree under a taller one
does not make the taller one taller.

**Measured** — after `Union(0,1)` through `Union(0,7)`:

```
correct   rank[] : 1 0 0 0 0 0 0 0
author's  rank[] : 7 0 0 0 0 0 0 0
```

and three redundant `Union(0,1)` calls drive `rank[0]` to 3 when the true rank never leaves 1. Rank
becomes a call counter rather than a tree-height bound, so union-by-rank stops choosing the right
side and the trees can degrade toward O(n) chains.

### `28_DSU/DSU.cpp:9` — no early return when `a` and `b` are already joined

`if (para == parb) return;` is missing. Without it, redundant unions still mutate `rank` (above) and
you lose the free "did this edge close a cycle?" signal that Kruskal and LC 684 both depend on.

### `28_DSU/DSU.cpp` — `main` is empty

Nothing was ever run. Both bugs above are the kind that one six-line test would have caught.

---

## What you got right

**All three representations are correct, and building three was the right call.** `adjency_list.cpp`
runs and produces exactly the right structure — verified against the 7-node sample in your own
comment:

```
0->2 ,1 ,     1->0 ,5 ,     2->0 ,5 ,3 ,     3->2 ,6 ,
4->6 ,        5->1 ,2 ,     6->3 ,4 ,
```

**The `bool bi_dir = true` default parameter is a genuinely good design decision.** One `add_edge`
serves directed and undirected graphs, the call site declares which, and there is no duplicated
code path to drift. Most tutorials write two functions.

**`connectedComponent.cpp` is completely correct** — verified on four cases: 1 component on the
sample graph, 4 on `{0,1} {2,3} {4} {5}`, 5 on five isolated nodes, 0 on an empty graph. The
structure is exactly right: a helper DFS that marks a whole component, and an outer loop over all
vertices that counts how many times it has to start over. Your comment `// go to every vertex` is
the correct one-line summary of why the outer loop exists.

Note that this file also checks `!visited.count(nei)` — **the neighbour, correctly** — which is the
very line `dfs.cpp` gets wrong. You wrote it right once. Comparing the two files side by side is the
fastest way to see the bug.

**`bfs.cpp`'s BFS is structurally correct.** It marks visited **at enqueue time**, which is the
detail most people get wrong, and it sets `dist[nei] = dist[curr] + 1` at the same moment. Verified:
on the sample graph it produces `0 1 1 2 4 2 3`, which is exactly right. Only the sentinel and the
`resize` are wrong — the algorithm underneath is sound.

**`allPath` has the backtracking right.** `path.pop_back()` and `visited.erase(src)` on the way out
(lines 39–40) is the `onPath` discipline, and it is correct — including the subtle detail of popping
the path in *both* the base case and the general case. That is the part people forget; the neighbour
check is the part they fix in ten seconds.

**`DSU.cpp:7` — path compression in one line, and it is right:**

```cpp
return par[x]=(par[x]==x)?x:find(par,par[x]);
```

Assigning the recursive result back into `par[x]` on the way up is the entire optimisation, and
writing it as one expression is the idiomatic form. Your comment
`// return the group/cluster x belongs to` is the correct mental model — `find` returns an
*identifier*, and the identifier happens to be a node.

**Your `readme.md` list is unusually well chosen.** 733, 200, 994, 542 and 1034 are five grid
problems that share one template and differ in exactly one decision each — that is a better learning
sequence than five unrelated problems. And the note *"542 01 matrix take 0 as a source"* is the
actual insight of the problem, written down before solving it.
