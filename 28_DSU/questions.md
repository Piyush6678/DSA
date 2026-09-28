# 28 — Disjoint Set Union: Practice Questions

> **Why 18 ranked problems.** DSU is one structure with two operations, so this cannot be a large
> list without padding — but it is not as small as `../23_sets` either, because the *applications*
> genuinely do not transfer to each other. The breakdown: **implementation** 4 (path compression,
> union by size and by rank, the iterative `find` you need past 60k nodes, and the component
> counter — all four are drills, and three of them are where your own file is wrong),
> **connectivity and counting** 5 (the "how many groups" family), **cycle detection and MST** 3
> (where the boolean return of `unite` becomes the whole algorithm), **transitive grouping** 4 (the
> phrase to listen for — accounts, equations, strings), and **DSU where a traversal cannot go** 2
> (edges arriving one at a time, which is the only thing DSU can do that BFS cannot).
>
> Eighteen is roughly three or four per group. The last group is two problems and cannot be larger —
> there are not many famous ones — but it is the group that justifies learning DSU at all, so it
> stays.

**Platform note.** LeetCode numbers are exact. **`[prem]`** is LeetCode Premium — the same problem
is free on GFG under the quoted title. GFG has no numeric IDs.

**Scope note.** Everything needs only `01`–`28`. **`[impl]`** marks a structural drill.

**This folder has no `readme.md` problem list**, so the set below is sourced from the standard DSU
canon rather than from your notes.

---

## Section 1 — Implementation

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | DSU with path compression and union by size | **Medium** | *Drill* `[impl]` | `DSU.cpp` — **`find` is right, `Union` is not** |
| 2 | Union by rank, and why rank rises only on a tie | **Medium** | *Drill* `[impl]` | `DSU.cpp:13` — **increments unconditionally** |
| 3 | Iterative `find` for very deep chains | **Medium** | *Drill* `[impl]` | recursion dies past ~60k — measured |
| 4 | Component count maintained incrementally | Easy | *Drill* `[impl]` | start at `n`, decrement on a real merge |

**Why these four.** #1 is the folder. Write the class once, with the boolean return from `unite`,
and everything below is four lines of glue. **Your `find` is already correct** — one-line path
compression, the idiomatic form. The exercise is `unite`, and specifically the two things
`solution.md` measures: linking the **root** rather than the node, and returning `false` when the
two are already together.

**#2 is worth doing separately from #1** because rank is the version most tutorials teach and the
easier one to get subtly wrong. Build it, then print the root's rank after seven unions against the
same root: it must be **1**, not 7.

**#3 is not academic.** Measured on this toolchain, a 60,000-deep chain survives and 80,000 aborts
with `0xC00000FD` — printing nothing, because `cout` is buffered. Constraints of 10⁵ are routine, so
the iterative form is the one to have written at least once.

#4 is one integer and it converts half of Section 2 into a one-liner.

---

## Section 2 — Connectivity and counting

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 5 | Number of Provinces | **Medium** | **LeetCode 547** | `number-of-provinces` — the plainest DSU there is |
| 6 | Number of Connected Components | **Medium** | **LeetCode 323** `[prem]` | GFG *"Number of Provinces"* — #5 from an edge list |
| 7 | Number of Islands | **Medium** | **LeetCode 200** | `number-of-islands` — the DSU version of `../27_graphs` §2 #8 |
| 8 | Graph Valid Tree | **Medium** | **LeetCode 261** `[prem]` | GFG *"Check if a given graph is tree"* |
| 9 | Number of Operations to Make Network Connected | **Medium** | **LeetCode 1319** | `number-of-operations-to-make-network-connected` |

**Why these five.** #5 and #6 are the same problem from a matrix and from an edge list, and doing
both makes the input format stop mattering. #7 is worth doing **after** the BFS version, so you can
compare: BFS is more natural here, DSU is the one that generalises to #17.

**#8 is the best cheap problem in the folder.** A graph is a tree exactly when it has **`n-1` edges
AND is connected** — and DSU gives you both in one pass, because a `unite` that returns false means
a cycle. Checking only the edge count passes a graph that is a triangle plus an isolated node.

**#9 is #8's arithmetic sibling.** With `c` components you need `c-1` cables to join them, and you
have a spare cable for every edge that failed to unite. So the answer is `c-1`, provided
`edges >= n-1` — and that guard is the entire difficulty. Verified: 4 computers with 3 connections
→ 1; 6 computers with 2 connections → −1.

---

## Section 3 — Cycle detection and MST

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 10 | Detect Cycle in an Undirected Graph | **Medium** | GFG | *"Detect cycle using DSU"* |
| 11 | Redundant Connection | **Medium** | **LeetCode 684** | `redundant-connection` — the **last** such edge |
| 12 | Kruskal's Minimum Spanning Tree | **Medium** | GFG | *"Minimum Spanning Tree"* |

**Why these three.** They are one idea seen three times: **`unite` returning `false` means the edge
closes a cycle.** #10 asks whether any edge does; #11 asks which one; #12 skips exactly those and
keeps the rest.

**#11's subtlety is in the question, not the code** — it asks for the *last* edge that could be
removed, and because you process edges in input order the first failing `unite` is already that
edge. Verified: `[[1,2],[2,3],[3,4],[1,4],[1,5]]` → `1 4`.

#12 is `../27_graphs` §5a #34 from the other side. Sort, unite, stop at `n-1` edges — verified at
weight 16 on the same graph Prim gives 16 for.

---

## Section 4 — Transitive grouping

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 13 | Accounts Merge | **Medium** | **LeetCode 721** | `accounts-merge` — email → owner |
| 14 | Satisfiability of Equality Equations | **Medium** | **LeetCode 990** | `satisfiability-of-equality-equations` — **order matters** |
| 15 | Most Stones Removed with Same Row or Column | **Medium** | **LeetCode 947** | `most-stones-removed-with-same-row-or-column` |
| 16 | Smallest String With Swaps | **Medium** | **LeetCode 1202** | `smallest-string-with-swaps` — group, then sort inside |

**Why these four. This is the group that teaches you to recognise DSU** in a problem that never
mentions graphs. All four say the same thing in different words: *"these two belong together, and
therefore so does everything they already belong with."*

**#13 is the template.** Two accounts merge when they share **any** email, so keep a
`map<string,int>` from email to the first account that claimed it, and union on every collision.
Verified against the LeetCode sample: 4 accounts collapse to 3, with the emails sorted inside each.

**#14 has a trap that costs most people a submission: process every `==` before any `!=`.** An
inequality checked before a later equality merges the two is checked against an incomplete
partition. Verified: `a==b, b==c, a!=c` is correctly rejected only when the equalities go first.

**#15 is the one that needs a real idea.** The answer is `stones − components`, because every
component can be reduced to exactly one stone. Then the trick: union a stone's **row index with its
column index** (offset the columns by a constant so they cannot collide with row ids) rather than
unioning stones pairwise — O(n) instead of O(n²).

#16 is "sort within each component": group the indices, collect their characters, sort, write back.

---

## Section 5 — Where a traversal cannot go

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 17 | Number of Islands II | **Hard** | **LeetCode 305** `[prem]` | GFG *"Number of Islands"* (online) — the reason DSU exists |
| 18 | Swim in Rising Water | **Hard** | **LeetCode 778** | `swim-in-rising-water` — sort cells, union, watch for connection |

**Why these two, and why the section is small.** There are not many famous "dynamic connectivity"
problems — but this is the only thing DSU does that a traversal cannot, so the section stays.

**#17 is the canonical case.** Land appears one cell at a time and you must report the island count
after each. BFS would be O(k·mn); DSU is O(k·α). Each new cell starts as its own island, then merges
with each already-present neighbour — one decrement per successful merge. Verified:
`(0,0),(0,1),(1,2),(2,1)` on a 3×3 grid → `1 1 2 3`, and a **repeated position must not
double-count** (verified separately).

**#18 is DSU used as a sweep.** Sort the cells by height and add them one at a time, unioning with
already-added neighbours; the answer is the height at which the start and the end first become
connected. Dijkstra also solves it (`../27_graphs` §4 #32) — comparing the two is the exercise, and
the DSU version is the one that generalises to "offline queries sorted by threshold".

---

## Section 6 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Find if Path Exists in Graph | Easy | **LeetCode 1971** | one `find` comparison |
| Longest Consecutive Sequence | **Medium** | **LeetCode 128** | DSU works; the set version in `../23_sets` is better |
| Count Unreachable Pairs of Nodes | **Medium** | **LeetCode 2316** | component sizes, then `n*(n-1)/2` minus the parts |
| Redundant Connection II | **Hard** | **LeetCode 685** | **directed** — two failure modes at once |
| Regions Cut By Slashes | **Medium** | **LeetCode 959** | split each cell into 4 triangles — the trick |
| Similar String Groups | **Hard** | **LeetCode 839** | O(n²) pair checks, then union |
| Evaluate Division | **Medium** | **LeetCode 399** | **weighted DSU**, or a graph walk |
| Making a Large Island | **Hard** | **LeetCode 827** | component sizes, then try flipping each 0 |
| Possible Bipartition | **Medium** | **LeetCode 886** | DSU with a complement set, or 2-colouring |
| Rank Transform of a Matrix | **Hard** | **LeetCode 1632** | union equal values in a row/column |
| Minimize Malware Spread | **Hard** | **LeetCode 924** | component sizes + infected counts |
| Lexicographically Smallest Equivalent String | **Medium** | **LeetCode 1061** | union, keeping the **smallest** letter as root |
| Checking Existence of Edge Length Limited Paths | **Hard** | **LeetCode 1697** | offline: sort queries **and** edges |
| The Earliest Moment When Everyone Become Friends | **Medium** | **LeetCode 1101** `[prem]` | sort by time, union until one component |
| Rollback DSU (union with undo) | **Hard** | *Drill* | no path compression; a stack of changes |
| DSU over strings via a `map<string,int>` | Easy | *Drill* | the id-mapping layer #13 needs |

---

## Progress tracker

```
S1 implementation  [ ] 1   [ ] 2   [ ] 3   [ ] 4
S2 connectivity    [ ] 5   [ ] 6   [ ] 7   [ ] 8   [ ] 9
S3 cycle / MST     [ ] 10  [ ] 11  [ ] 12
S4 grouping        [ ] 13  [ ] 14  [ ] 15  [ ] 16
S5 dynamic         [ ] 17  [ ] 18
S6 extra           [ ] ______ / 16
```
