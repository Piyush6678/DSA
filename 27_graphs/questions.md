# 27 — Graphs: Practice Questions

> **Why 46 ranked problems — second only to `../26_dp`.** Graphs are not one technique. They are
> **one data structure with eight algorithms bolted on**, and the algorithms share almost nothing:
> knowing Dijkstra tells you nothing about topological sort. The breakdown is
> **representation and traversal** 6 (the foundation, and where the visited-set discipline is
> built), **grids and implicit graphs** 10 — the largest group on purpose, because most interview
> graph problems never use the word *graph* and six of your own nine listed problems are grids —
> **cycle detection and topological sort** 8 (where directed and undirected genuinely diverge),
> **shortest paths** 8 (five algorithms, and choosing between them is the examined skill),
> **MST** 4, **bipartite and colouring** 4, and **advanced structure** 6 (SCC, bridges,
> articulation points — the ones that separate a hard interview from a medium one).
>
> Forty-six is roughly six per group. Below that the shortest-path group loses either Bellman–Ford
> or the DAG relaxation, and "which algorithm and why" is the single most reliably asked graph
> question there is.

**Platform note.** LeetCode numbers are exact. **`[prem]`** marks LeetCode Premium — the same
problem is free on GFG under the quoted title. GFG has no numeric IDs.

**Scope note.** Everything needs only `01`–`27`. **`[impl]`** marks a structural drill rather than
a judge problem. **`[fwd]`** marks a problem whose cleanest solution wants `../28_DSU`.

**All nine problems from your `readme.md` are covered** — the mapping table is at the bottom of
`readme.md`.

---

## Section 1 — Representation and traversal

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Build all three representations from an edge list | Easy | *Drill* `[impl]` | your four files — **all correct** |
| 2 | BFS of a graph | Easy | GFG | *"BFS of graph"* — `bfs.cpp`, **INT8_MAX sentinel** |
| 3 | DFS of a graph | Easy | GFG | *"DFS of Graph"* — `dfs.cpp`, **the neighbour check is wrong** |
| 4 | Find Centre of Star Graph | Easy | **LeetCode 1791** | `find-center-of-star-graph` — **on your list** |
| 5 | Number of Connected Components | **Medium** | **LeetCode 323** `[prem]` | GFG *"Number of Provinces"* — `connectedComponent.cpp` **works** |
| 6 | Keys and Rooms | **Medium** | **LeetCode 841** | `keys-and-rooms` — **on your list** |

**Why these six.** #1 is not a judge problem and is the most valuable half-hour here: you have
already written the unweighted list, the weighted list and the weighted map. Add the **matrix**,
then write one function that converts between any two. After that, "which representation" stops
being a guess.

**#2 and #3 are your own files and both have bugs** — #2 uses a sentinel of 127 for "unreachable"
and #3 checks `visited.count(src)` where it means `nei`, which stops the recursion happening at
all. Fixing them yourself from the descriptions in `solution.md` is the exercise.

**#4 is here because it is a trap dressed as an easy problem.** A star graph's centre is the node
that appears in *both* of the first two edges — you never need to build the graph, count degrees,
or traverse anything. Anyone who builds an adjacency list has already lost the point.

#5 you have working (verified on four cases). #6 is reachability from node 0, and it is worth
noticing that it is `connectedComponent.cpp` asking a different question about the same walk.

---

## Section 2 — Grids and implicit graphs

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 7 | Flood Fill | Easy | **LeetCode 733** | `flood-fill` — **on your list** |
| 8 | Number of Islands | **Medium** | **LeetCode 200** | `number-of-islands` — **on your list** |
| 9 | Max Area of Island | **Medium** | **LeetCode 695** | `max-area-of-island` — #8 returning a size |
| 10 | Rotting Oranges | **Medium** | **LeetCode 994** | `rotting-oranges` — **on your list**; multi-source |
| 11 | 01 Matrix | **Medium** | **LeetCode 542** | `01-matrix` — **on your list**; *"take 0 as a source"* |
| 12 | Surrounded Regions | **Medium** | **LeetCode 130** | `surrounded-regions` — start from the **border** |
| 13 | Coloring A Border | **Medium** | **LeetCode 1034** | `coloring-a-border` — **on your list** |
| 14 | Minimum Knight Moves | **Medium** | **LeetCode 1197** `[prem]` | GFG *"Knight Walk"* — **on your list** |
| 15 | Word Ladder | **Hard** | **LeetCode 127** | `word-ladder` — the graph is **not given** |
| 16 | Number of Enclaves | **Medium** | **LeetCode 1020** | `number-of-enclaves` — #12 counting instead |

**Why these ten — the largest group, deliberately.** Nearly every graph question in a real
interview is one of these: a grid, a word list, a set of states. **The skill being tested is
noticing that it is a graph at all**, and after ten of them you stop missing it.

#7 and #8 are the two templates. #7 has exactly one trap — **if the new colour equals the old
colour the fill never terminates**, and it is a one-line guard. #8 is the four-direction sweep, and
the detail is marking a cell **when you enqueue it**, not when you dequeue it.

**#10, #11 and #14 are the multi-source group** and they are why your note *"take 0 as a source"*
was the right observation. Seeding the queue with every source before the loop starts is strictly
better than running BFS once per source, and the level count is the answer.

**#12 is the inversion worth learning**: instead of finding regions that *are* surrounded, start
from the border and mark everything that is *not*. That reframing — "solve the complement" — shows
up again in #16 and in `../28_DSU`.

**#15 is the hardest idea in the section**: there is no grid and no edge list, only a word list. Two
words are neighbours when they differ in one letter, and you generate that adjacency on demand. Once
you see it, it is plain BFS; before you see it, it is impossible. The optimisation (bucket words by
wildcard patterns like `h*t`) is the follow-up.

---

## Section 3 — Cycle detection and topological sort

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 17 | Clone Graph | **Medium** | **LeetCode 133** | `clone-graph` — **on your list**; a map old→new |
| 18 | Detect Cycle in an Undirected Graph | **Medium** | GFG | *"Detect cycle in an undirected graph"* `[fwd]` |
| 19 | Detect Cycle in a Directed Graph | **Medium** | GFG | *"Detect cycle in a directed graph"* — 3 colours |
| 20 | Course Schedule | **Medium** | **LeetCode 207** | `course-schedule` — is it a DAG? |
| 21 | Course Schedule II | **Medium** | **LeetCode 210** | `course-schedule-ii` — the order itself |
| 22 | Topological Sort | **Medium** | GFG | *"Topological sort"* — both Kahn and DFS |
| 23 | Alien Dictionary | **Hard** | **LeetCode 269** `[prem]` | GFG *"Alien Dictionary"* — build the DAG first |
| 24 | Find Eventual Safe States | **Medium** | **LeetCode 802** | `find-eventual-safe-states` — reverse-graph Kahn |

**Why these eight. #17 is the cleanest "map old to new" problem there is** — you cannot set a
cloned node's neighbours until they exist, and an `unordered_map<Node*, Node*>` makes that a
non-issue. Exactly the same shape as LC 138 in `../24_maps`; solving both makes the pattern
portable.

**#18 and #19 must be done together.** The whole lesson is that the undirected parent trick fails
on a directed diamond, and you only feel that by writing the undirected version first and watching
it give the wrong answer.

#20 and #21 are Kahn's algorithm with two different return values — a boolean and the order — which
is why they are adjacent. **#23 is the best problem in the section**: the hard part is not the
topological sort, it is **deriving the edges** from adjacent words (first differing character) and
handling the invalid case where a word is a prefix of a shorter one that precedes it. Most failed
submissions never get to the sort.

**#24 is Kahn run on the reversed graph**, and seeing that "a node is safe if all its out-edges
lead to safe nodes" is the same statement as "peel nodes with out-degree 0" is the trick.

---

## Section 4 — Shortest paths

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 25 | Dijkstra's Algorithm | **Medium** | GFG | *"Implementing Dijkstra Algorithm"* `[impl]` |
| 26 | Network Delay Time | **Medium** | **LeetCode 743** | `network-delay-time` — Dijkstra, plainly |
| 27 | Path With Minimum Effort | **Medium** | **LeetCode 1631** | `path-with-minimum-effort` — Dijkstra on a *max* |
| 28 | Cheapest Flights Within K Stops | **Medium** | **LeetCode 787** | `cheapest-flights-within-k-stops` — **Bellman–Ford** |
| 29 | Bellman–Ford | **Medium** | GFG | *"Distance from the Source (Bellman-Ford)"* `[impl]` |
| 30 | Floyd–Warshall | **Medium** | GFG | *"Floyd Warshall"* `[impl]` |
| 31 | Shortest Path in a DAG | **Medium** | GFG | *"Shortest path in Directed Acyclic Graph"* |
| 32 | Swim in Rising Water | **Hard** | **LeetCode 778** | `swim-in-rising-water` `[fwd]` — Dijkstra, or DSU + sort |

**Why these eight.** The examined skill is **choosing**, and you cannot choose between algorithms
you have not written. #25, #29 and #30 are implementation drills for exactly that reason.

**#28 is the most instructive problem here and it is a deliberate trap.** It looks like Dijkstra and
Dijkstra gives the wrong answer, because the cheapest route to a node is not necessarily the
cheapest route *within k stops* — a node can be finalised via a cheap-but-long path and block a
dearer-but-shorter one. Bellman–Ford relaxing exactly `k+1` rounds is the fix, and understanding
*why* is worth more than the other seven combined.

**#27 and #32 both replace "sum of weights" with "maximum weight on the path"**, which is still a
valid Dijkstra as long as the relaxation is monotone. Recognising that Dijkstra generalises beyond
addition is a real jump.

**#31 is the one people forget exists.** On a DAG, topological order plus a single relaxation pass
is O(V+E) — beating Dijkstra, and it handles negative weights. If a problem says "directed acyclic",
this is the answer.

---

## Section 5 — MST, bipartite, and advanced structure

### 5a — Minimum spanning tree

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 33 | Prim's Algorithm | **Medium** | GFG | *"Minimum Spanning Tree"* `[impl]` |
| 34 | Kruskal's Algorithm | **Medium** | GFG | *"Minimum Spanning Tree"* `[impl]` `[fwd]` |
| 35 | Min Cost to Connect All Points | **Medium** | **LeetCode 1584** | `min-cost-to-connect-all-points` — a **complete** graph |
| 36 | Connecting Cities With Minimum Cost | **Medium** | **LeetCode 1135** `[prem]` | GFG *"Minimum Spanning Tree"* variant |

**#35 is the one worth doing** — the graph is never given, it is implied: every pair of points is an
edge with Manhattan distance as its weight, so E = V². That makes it a **dense** graph, which is
exactly the case where Prim beats Kruskal, and noticing that is the point.

### 5b — Bipartite and colouring

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 37 | Is Graph Bipartite? | **Medium** | **LeetCode 785** | `is-graph-bipartite` |
| 38 | Possible Bipartition | **Medium** | **LeetCode 886** | `possible-bipartition` `[fwd]` — #37 with edges to build |
| 39 | Flower Planting With No Adjacent | **Medium** | **LeetCode 1042** | `flower-planting-with-no-adjacent` — greedy 4-colouring |
| 40 | M-Coloring Problem | **Medium** | GFG | *"M-Coloring Problem"* — backtracking, not BFS |

**#37 and #40 are a contrast worth drawing.** Two-colouring is a linear BFS. **Three-colouring is
NP-complete**, so #40 is backtracking with pruning and no polynomial algorithm exists. Knowing where
that cliff is stops you looking for an efficient algorithm that cannot exist.

### 5c — Advanced structure

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 41 | Bridges in a Graph | **Hard** | **LeetCode 1192** | `critical-connections-in-a-network` — Tarjan |
| 42 | Articulation Points | **Hard** | GFG | *"Articulation Point"* — #41 with a different test |
| 43 | Strongly Connected Components | **Hard** | GFG | *"Strongly Connected Components (Kosaraju)"* |
| 44 | Eventual Safe Nodes via SCC | **Hard** | *Drill* | #24 re-derived through condensation |
| 45 | Mother Vertex | **Medium** | GFG | *"Mother Vertex"* — one DFS, then verify |
| 46 | Euler Path / Circuit existence | **Medium** | GFG | *"Euler circuit and Path"* — a degree argument |

**Why these six.** **#41 is the highest-value hard graph problem** — `low[v] > tin[u]` means "the
subtree below `v` has no back edge above `u`", so removing the edge disconnects it. Both bridges and
articulation points come from the same DFS with **one comparison changed** (`>` versus `>=`, plus a
root special case), and doing them back to back makes the difference visible.

**#43 is the canonical hard-graph question.** Kosaraju is two DFS passes and a reversal, and the
reason it works — the finishing-time order of the original graph is a topological order of the
condensation — is the explanation you should be able to give.

**#46 is not an algorithm, it is a counting argument**: an Euler circuit exists iff every vertex has
even degree and the graph is connected; an Euler *path* iff exactly two vertices have odd degree.
Worth five minutes because it is pure reasoning, and interviewers like that.

---

## Section 6 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Find if Path Exists in Graph | Easy | **LeetCode 1971** | the simplest possible traversal |
| All Paths From Source to Target | **Medium** | **LeetCode 797** | `dfs.cpp`'s `allPath`, done right |
| Pacific Atlantic Water Flow | **Medium** | **LeetCode 417** | two reverse BFS, then intersect |
| Rotting Oranges with 8 directions | **Medium** | *Drill* | change two arrays; nothing else |
| Shortest Bridge | **Medium** | **LeetCode 934** | DFS to find one island, then BFS outward |
| Open the Lock | **Medium** | **LeetCode 752** | BFS over a **state space**, not a grid |
| Minimum Genetic Mutation | **Medium** | **LeetCode 433** | LC 127 with a 4-letter alphabet |
| Snakes and Ladders | **Medium** | **LeetCode 909** | BFS with an awkward index mapping |
| Bus Routes | **Hard** | **LeetCode 815** | BFS where the *routes* are the nodes |
| Reconstruct Itinerary | **Hard** | **LeetCode 332** | Hierholzer — Euler path, built |
| The Maze II | **Medium** | **LeetCode 505** `[prem]` | rolling changes the edge cost — Dijkstra |
| Number of Ways to Arrive at Destination | **Medium** | **LeetCode 1976** | Dijkstra counting paths |
| Word Ladder II | **Hard** | **LeetCode 126** | BFS for layers, DFS to rebuild paths |
| Minimum Height Trees | **Medium** | **LeetCode 310** | peel leaves — also `../21_tree` §5 #49 |
| Graph Valid Tree | **Medium** | **LeetCode 261** `[prem]` | `n-1` edges **and** connected `[fwd]` |
| Count Unreachable Pairs | **Medium** | **LeetCode 2316** | component sizes, then combinatorics `[fwd]` |
| Convert an adjacency list to a matrix and back | Easy | *Drill* | the §1 #1 conversion, written out |
| Iterative DFS with an explicit stack | **Medium** | *Drill* | what you need past ~60k nodes |

---

## Progress tracker

```
S1 traversal   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6
S2 grids       [ ] 7   [ ] 8   [ ] 9   [ ] 10  [ ] 11
               [ ] 12  [ ] 13  [ ] 14  [ ] 15  [ ] 16
S3 cycle/topo  [ ] 17  [ ] 18  [ ] 19  [ ] 20  [ ] 21  [ ] 22  [ ] 23  [ ] 24
S4 shortest    [ ] 25  [ ] 26  [ ] 27  [ ] 28  [ ] 29  [ ] 30  [ ] 31  [ ] 32
S5a MST        [ ] 33  [ ] 34  [ ] 35  [ ] 36
S5b bipartite  [ ] 37  [ ] 38  [ ] 39  [ ] 40
S5c advanced   [ ] 41  [ ] 42  [ ] 43  [ ] 44  [ ] 45  [ ] 46
S6 extra       [ ] ______ / 18
```
