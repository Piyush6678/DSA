# 27 — Graphs

A tree with the restrictions removed. No root, no parent/child, cycles allowed, and a node can be
reached by many paths — which is why **every graph algorithm carries a `visited` set** and every
tree algorithm does not.

```
tree   = connected + acyclic + n-1 edges     (a special case)
graph  = a set of nodes V and a set of edges E
```

Everything from `../21_tree` transfers: BFS is level-order with a visited check, DFS is pre-order
with a visited check. What is new is that the same node arrives twice, and deciding what to do
about that is most of this folder.

---

## 1. Vocabulary you will be held to

| Term | Meaning |
|---|---|
| **Directed / undirected** | is `u→v` the same edge as `v→u`? |
| **Weighted** | edges carry a cost; changes which shortest-path algorithm applies |
| **Degree** | edges touching a node. Directed graphs have **in-degree** and **out-degree** separately |
| **Path** | a walk with no repeated node |
| **Cycle** | a path that returns to its start |
| **Connected component** | a maximal set of mutually reachable nodes |
| **DAG** | directed acyclic graph — the only kind that can be topologically sorted |
| **Dense / sparse** | `E ≈ V²` versus `E ≈ V`; decides matrix versus list |

**Complexities are quoted in terms of both V and E.** "O(n)" is meaningless for a graph. BFS and
DFS are **O(V + E)** — every node once, every edge once.

---

## 2. Three representations, and you built all three

### Adjacency matrix — `vector<vector<int>>`, V × V

`O(1)` edge lookup, **`O(V²)` space**, and listing a node's neighbours costs `O(V)` even if it has
one. Use it when the graph is dense or V is small (≤ ~1000), and for Floyd–Warshall, which needs
it.

### Adjacency list — `vector<list<int>>` (your `adjency_list.cpp`)

```cpp
vector< list<int> > graph;
void add_edge(int src, int dest, bool bi_dir = true) {
    graph[src].push_back(dest);
    if (bi_dir) graph[dest].push_back(src);
}
```

**`O(V + E)` space** and neighbours come out in `O(degree)`. This is the default — nearly every
real graph is sparse, and every algorithm below except Floyd–Warshall wants this shape.

The `bi_dir` default parameter is the right design: **one function handles directed and undirected
graphs**, and the call site says which. Passing `false` gives you a directed edge with no second
code path.

`vector<vector<int>>` is usually preferable to `vector<list<int>>` — contiguous memory, better
cache behaviour, and you never need `list`'s O(1) middle insertion.

### Weighted variants — your other two files

```cpp
vector< list< pair<int,int> > > graph;       // WeigtedAdjencyList.cpp -- (neighbour, weight)
vector< unordered_map<int,int> > graph;      // adjencyMap.cpp         -- neighbour -> weight
```

Both are correct and they trade differently: the **list of pairs** iterates fastest and is what
Dijkstra wants; the **map** gives `O(1)` "what is the weight of `u→v`" and automatically collapses
duplicate edges. Having written both, the thing to notice is that the map version is really an
adjacency matrix with the empty cells removed.

---

## 3. BFS — shortest path when every edge costs the same

```cpp
vector<int> bfsDist(vector<vector<int> >& adj, int src) {
    vector<int> dist(adj.size(), INT_MAX);
    queue<int> q;
    dist[src] = 0;
    q.push(src);
    while (!q.empty()) {
        int cur = q.front(); q.pop();
        for (int nei : adj[cur]) {
            if (dist[nei] == INT_MAX) {        // dist doubles as the visited marker
                dist[nei] = dist[cur] + 1;
                q.push(nei);
            }
        }
    }
    return dist;
}
```

Three things decide whether a BFS is correct:

- **Mark visited when you ENQUEUE, not when you dequeue.** Marking on dequeue lets a node be
  pushed several times before it is ever processed; on a dense graph the queue explodes.
- **`INT_MAX` for unreachable**, not a small sentinel. `INT8_MAX` is 127, which is a *legal
  distance* in any graph with a path longer than 127 — the "unreachable" marker becomes
  indistinguishable from a real answer.
- **`dist` can be the visited set.** One array instead of two, and they can never disagree.

**BFS gives the shortest path only when every edge has the same weight.** With weights it is simply
wrong — that is Dijkstra's job.

**Multi-source BFS** is the same code with several nodes pushed before the loop starts. Rotting
oranges, 01-matrix and "nearest exit" are all this, and it is strictly better than running BFS from
each source.

---

## 4. DFS — reachability, paths, and structure

```cpp
void dfs(int u, vector<vector<int> >& adj, vector<bool>& vis) {
    vis[u] = true;
    for (int v : adj[u]) if (!vis[v]) dfs(v, adj, vis);
}
```

**Check the NEIGHBOUR, not the current node.** `if (!vis[u])` inside the loop is always false —
you just set it — so the recursion never happens and the function silently visits nothing. This is
the single most common DFS typo and it produces no error, just wrong answers.

**Two different kinds of "visited":**

| Marker | Semantics | Used by |
|---|---|---|
| `visited[]` — set once, never cleared | "I have *ever* been here" | components, reachability, islands |
| `onPath[]` — set on entry, **cleared on exit** | "I am here *right now*" | all simple paths, directed cycle detection |

Mixing them up is the second most common bug. Enumerating all paths needs the second, because a
node not on the current path may legitimately appear on a different one.

**Recursion depth is V.** At around 60,000–80,000 frames this toolchain overflows the stack
(measured: `0xC00000FD`), and because `cout` is buffered the program prints **nothing at all**. For
a 10⁵-node graph, use an explicit stack or BFS.

---

## 5. Grids are graphs — the most useful realisation here

Most interview "graph" problems never mention graphs. A grid is a graph where:

- **node** = a cell `(r, c)`
- **edges** = the 4 (or 8) neighbours
- **the adjacency list is implicit** — you never build it, you compute neighbours on the fly

```cpp
int dr[] = {-1, 1, 0, 0};
int dc[] = { 0, 0,-1, 1};
for (int d = 0; d < 4; ++d) {
    int nr = r + dr[d], nc = c + dc[d];
    if (nr < 0 || nr >= m || nc < 0 || nc >= n) continue;    // bounds FIRST
    ...
}
```

**Test the bounds before touching the cell.** `grid[nr][nc]` evaluated before the bounds check is
an out-of-range read, and `vector` will not tell you.

Number of islands, rotten oranges, 01-matrix, flood fill, word ladder, knight moves, surrounded
regions — all BFS or DFS with this neighbour loop and nothing else. **Six of the nine problems on
your own list are grid problems.**

For a grid, use `m*n` visited cells, or mutate the grid itself if the problem allows it (islands
usually does). And **8-directional** grids just extend the two arrays to 8 entries.

---

## 6. Cycle detection — and why directed is different

**Undirected:** a neighbour that is already visited and is **not the node you came from** closes a
cycle.

```
if (!visited[nei]) { parent[nei] = cur; enqueue }
else if (nei != parent[cur]) return true;
```

The parent check is what stops the edge you just walked from looking like a cycle.

**Directed: the parent trick does not work.** A diamond `0→1, 0→2, 1→2` revisits node 2 with no
cycle present. You need to know whether the revisited node is **still on the current recursion
stack**:

```
colour 0 = untouched      1 = on the stack now      2 = fully finished
seeing colour 1  ->  back edge  ->  cycle
seeing colour 2  ->  already explored, no cycle
```

**A revisit is only a cycle if the node is an ancestor of the current one.** That is the whole
distinction, and it is the standard interview follow-up to the undirected version.

Undirected cycle detection also falls out of **DSU** in one line — see `../28_DSU`.

---

## 7. Topological sort — only for DAGs

A linear order in which every edge points forward. Exists **if and only if the graph is acyclic**,
which makes it a cycle detector as well.

**Kahn's algorithm (BFS):**

```
compute in-degree of every node
push every node with in-degree 0
pop u, append to order, decrement each neighbour's in-degree, push the ones that hit 0
if the order is shorter than V, there was a cycle
```

**DFS version:** post-order, then reverse. A node is appended after all its descendants, so
reversing puts it before them.

Kahn is the one to write: it detects cycles for free, needs no recursion, and extends to problems
that need *lexicographically smallest* order (swap the queue for a min heap) or *level* information
(process one in-degree wave at a time). Course Schedule I and II are Kahn verbatim.

---

## 8. Shortest paths — pick by the constraint

| Algorithm | Handles | Complexity | Use when |
|---|---|---|---|
| **BFS** | unweighted | O(V + E) | every edge costs 1 |
| **0-1 BFS** | weights 0 or 1 | O(V + E) | a deque; `push_front` for 0, `push_back` for 1 |
| **Dijkstra** | non-negative weights | O((V+E) log V) | the default weighted case |
| **Bellman–Ford** | **negative** weights | O(V·E) | negatives, or you must *detect* a negative cycle |
| **Floyd–Warshall** | all pairs, negatives | O(V³) | small V, and you want every pair |
| **Topo + relax** | any weights, **DAG only** | O(V + E) | a DAG — beats Dijkstra and handles negatives |

**Dijkstra breaks on negative edges.** Once a node is popped it is treated as final, and a negative
edge discovered later could have improved it. If a graph has negative weights, the answer is
Bellman–Ford — say so, it is the check.

```cpp
priority_queue<pii, vector<pii>, greater<pii> > pq;    // MIN heap, (distance, node)
...
if (d > dist[u]) continue;                             // lazy deletion of stale entries
```

**The `if (d > dist[u]) continue;` line matters.** `priority_queue` cannot decrease a key, so the
standard trick is to push a new entry and skip the outdated one when it surfaces. Without it the
algorithm is still correct but re-expands nodes.

**Bellman–Ford's `V-1` rounds** come from a simple bound: a shortest path has at most `V-1` edges,
and each round finalises one more edge of every path. **A `V`th round that still improves something
means a negative cycle** — that is the detection, not an afterthought.

**Floyd–Warshall's `k` loop must be outermost.** `k` is "intermediate nodes 0..k are now allowed";
putting it inside gives an algorithm that is not Floyd–Warshall and quietly returns wrong answers.

---

## 9. Minimum spanning tree

The cheapest set of edges connecting every node. Two greedy algorithms, both correct:

| | **Prim** | **Kruskal** |
|---|---|---|
| Grows | one tree, outward from a node | a forest, merging |
| Needs | a **heap** (`../25_heap`) | **sorting** + **DSU** (`../28_DSU`) |
| Complexity | O(E log V) | O(E log E) |
| Better for | dense graphs | sparse graphs, and edge lists |

Both were verified to give **16** on the same 5-node graph. Kruskal is easier to write when the
input is already an edge list, and its DSU is the reason `../28_DSU` comes next.

**MST is not shortest path.** The MST minimises *total* edge weight; it does not give the shortest
route between two particular nodes. Confusing them is a standard interview trap.

---

## 10. Bipartite — 2-colouring

Colour a node, colour every neighbour the opposite, and fail if a neighbour already has your
colour. BFS or DFS, O(V + E).

**A graph is bipartite exactly when it has no odd-length cycle.** A triangle fails; a 4-cycle
passes; **every tree is bipartite** because it has no cycles at all. That equivalence is the answer
they want, not the algorithm.

---

## Interview Q&A

**Q1. Adjacency list or adjacency matrix?**
List for sparse graphs — O(V+E) space, and neighbours in O(degree). Matrix for dense graphs or when
you need O(1) "is there an edge `u→v`" — O(V²) space regardless of edge count. Most real graphs are
sparse, so list is the default.

**Q2. BFS or DFS?**
BFS for shortest path in an unweighted graph, level-by-level processing, or when the answer is
likely near the source. DFS for reachability, path enumeration, cycle detection, topological order
and anything about structure. Both are O(V+E); BFS uses O(width) memory, DFS O(depth).

**Q3. How do you detect a cycle, and why do directed and undirected differ?**
Undirected: a visited neighbour that is not your parent. Directed: the parent trick fails on
diamonds, so track whether the revisited node is still on the recursion stack (three colours), or
run Kahn's algorithm and check whether the order is shorter than V.

**Q4. Why does Dijkstra fail on negative edges?**
It finalises a node when it is popped, assuming no cheaper route can appear later. A negative edge
can make one appear. Bellman–Ford relaxes every edge V-1 times instead and never finalises early —
and a V-th improving round proves a negative cycle.

**Q5. Complexity of Dijkstra?**
O((V + E) log V) with a binary heap. O(V²) with a plain array, which is actually **better on dense
graphs** where E ≈ V². With a Fibonacci heap it is O(E + V log V), which is theory rather than
practice.

**Q6. Prim or Kruskal?**
Kruskal for sparse graphs and edge-list input — sort the edges, add any that joins two different
components, using DSU. Prim for dense graphs — grow one tree with a heap. Same answer, different
data structure.

**Q7. Topological sort — when does it exist, and how?**
Only for a DAG. Kahn: repeatedly remove in-degree-zero nodes; if fewer than V come out, there is a
cycle. Or DFS post-order reversed. Kahn is preferable because the cycle check is free.

**Q8. How do you find the shortest path in a grid?**
BFS from the source with a 4-direction neighbour loop — a grid is a graph with implicit edges.
Multi-source BFS if there are several starting points. Dijkstra only if entering a cell has a cost.

**Q9. What is a strongly connected component?**
A maximal set of nodes in a **directed** graph where every node reaches every other. Kosaraju:
DFS to get a finish-time order, reverse all edges, then DFS in that order — each tree is one SCC.
Tarjan does it in a single pass with low-link values.

**Q10. Given a graph with 10⁵ nodes, what breaks?**
Recursive DFS — the stack overflows around 60–80k frames on a typical setup (measured here at
`0xC00000FD`). Use an explicit stack or BFS. Also an O(V²) adjacency matrix is 10¹⁰ cells, so the
representation must be a list.

---

## Your original notes (preserved)

**All nine are covered** — see `questions.md`.

```
733 flood fill
1791 center ofstar graph
841 keys and row
133 clone graph
1034
200 no. of island
994 rotten orange
542 01 matrix take 0 as a source
1197  minimum knight moces
```

| Your entry | Now at | | Your entry | Now at |
|---|---|---|---|---|
| 733 Flood Fill | §2 #7 | | 200 Number of Islands | §2 #8 |
| 994 Rotting Oranges | §2 #10 | | 542 01 Matrix | §2 #11 |
| 1034 Coloring A Border | §2 #13 | | 1791 Centre of Star Graph | §1 #4 |
| 841 Keys and Rooms | §1 #6 | | 133 Clone Graph | §3 #17 |
| 1197 Minimum Knight Moves | §2 #14 | | | |

Your note on 542 — *"take 0 as a source"* — is exactly the insight the problem turns on, and it is
the same idea as 994. Both are multi-source BFS; writing that down before solving it is the right
instinct.
