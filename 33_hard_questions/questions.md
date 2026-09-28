# 33 — Hard Questions: Practice Problems

> **Why 50 problems.** This folder exists to bridge the gap between topic-level practice and real interview performance. Each problem here combines two or more concepts from folders `01`–`32`. Fifty is the minimum needed to cover the ten most common concept combinations — DP+Trees, DP+Graphs, DP+Bitmask, DP+Strings, Segment Tree+DP, Graphs+Advanced, Stack+DP, Trie+Backtracking, Greedy+Heap, and Math+Algorithms — with enough depth to pattern-match in interviews.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search the quoted title.

**Scope note.** All are Hard or tricky Medium. Organised by CONCEPT COMBINATION, not by difficulty.

---

## Section 1 — DP + Trees (5)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 1 | Binary Tree Maximum Path Sum | DP + Trees | **LeetCode 124** | max path through any node |
| 2 | Binary Tree Cameras | DP + Trees | **LeetCode 968** | 3-state DP on trees |
| 3 | Sum of Distances in Tree | DP + Trees | **LeetCode 834** | Rerooting DP |
| 4 | House Robber III | DP + Trees | **LeetCode 337** | Pick/skip DP on tree (Medium) |
| 5 | Distribute Coins in Binary Tree | DP + Trees | **LeetCode 979** | DFS + greedy flow (Medium) |

**Why these.** These five teach you how to propagate DP states up and down a tree. #1 is the single most important tree DP problem — compute local max at each node, maintain global max. #2 introduces 3-state DP (covered, needs cover, has camera). #3 is rerooting DP — compute answer for ALL roots in O(n) by reusing parent's computation. #4 and #5 are simpler warmups.

---

## Section 2 — DP + Graphs (5)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 6 | Shortest Path Visiting All Nodes | DP + Graphs | **LeetCode 847** | Bitmask DP + BFS |
| 7 | Longest Increasing Path in a Matrix | DP + Graphs | **LeetCode 329** | DFS + memoization on implicit DAG |
| 8 | Parallel Courses III | DP + Graphs | **LeetCode 2050** | Topological sort + DP |
| 9 | Number of Restricted Paths | DP + Graphs | **LeetCode 1786** | Dijkstra + DP counting (Medium) |
| 10 | Frog Jump | DP + Graphs | **LeetCode 403** | DP with set-based transitions |

**Why these.** #6 is the TSP-lite problem that teaches bitmask state with BFS. #7 is the canonical 'DFS memo on grid as DAG' problem. #8 combines topological ordering with DP — critical path computation. #9 is Dijkstra followed by counting paths in a DAG. #10 is unusual — the state space isn't a grid or graph but stone positions with varying jump lengths.

---

## Section 3 — DP + Strings (5)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 11 | Regular Expression Matching | DP + Strings | **LeetCode 10** | |
| 12 | Wildcard Matching | DP + Strings | **LeetCode 44** | |
| 13 | Distinct Subsequences | DP + Strings | **LeetCode 115** | |
| 14 | Shortest Common Supersequence | DP + Strings | **LeetCode 1092** | LCS + reconstruction |
| 15 | Count Different Palindromic Subsequences | DP + Strings | **LeetCode 730** | |

**Why these.** #11 and #12 are the two pattern-matching DPs that EVERY FAANG candidate must know. The '*' semantics are different in each — #11's '*' depends on the preceding character, #12's doesn't. #13 is counting DP on two strings. #14 requires PRINTING the LCS-based answer, not just computing its length. #15 is the hardest palindrome DP.

---

## Section 4 — DP + Binary Search / Math (5)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 16 | Super Egg Drop | DP + Binary Search | **LeetCode 887** | DP + binary search optimisation |
| 17 | Russian Doll Envelopes | DP + Math | **LeetCode 354** | LIS + sorting trick |
| 18 | Split Array Largest Sum | Binary Search | **LeetCode 410** | Binary search on answer + greedy check |
| 19 | Dungeon Game | DP | **LeetCode 174** | Backward DP on grid |
| 20 | Burst Balloons | DP + Math | **LeetCode 312** | Interval DP with 'last to burst' trick |

**Why these.** #16 is the classic quant interview problem — the naive DP is O(kn²), the optimised version uses binary search on the DP transition to get O(kn log n). #17 combines sorting (by width ascending, height descending) with LIS. #18 is the binary search + greedy verification pattern. #19 forces backward thinking — start from the end. #20 is the hardest interval DP — the trick is thinking about which balloon to burst LAST.

---

## Section 5 — DP + Bitmask (4)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 21 | Shortest Superstring | DP + Bitmask | **LeetCode 943** | TSP variant |
| 22 | Maximum Students Taking Exam | DP + Bitmask | **LeetCode 1349** | Bitmask DP on rows |
| 23 | Number of Ways to Wear Different Hats | DP + Bitmask | **LeetCode 1434** | |
| 24 | Minimum Cost to Connect Two Groups of Points | DP + Bitmask | **LeetCode 1595** | |

**Why these.** #21 is TSP with string overlap — the classic NP-hard problem solved exactly with bitmask DP for small n. #22 is bitmask DP on grid rows with adjacency constraints. #23 and #24 are assignment problems solvable with bitmask DP because one dimension is small (≤ 10-15).

---

## Section 6 — Segment Tree + Other (4)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 25 | Count of Smaller Numbers After Self | Segment Tree | **LeetCode 315** | Seg tree + coordinate compression |
| 26 | Longest Increasing Subsequence II | Seg tree + DP | **LeetCode 2407** | Seg tree + DP |
| 27 | Rectangle Area II | Seg tree + Math | **LeetCode 850** | Line sweep + seg tree |
| 28 | Falling Squares | Segment Tree | **LeetCode 699** | Coordinate compression + range update |

**Why these.** These are the segment tree problems where the tree is an OPTIMISATION LAYER on top of another algorithm. #25 uses seg tree as frequency array. #26 replaces O(n²) DP with O(n log n) via range max query. #27 is computational geometry + seg tree. #28 is coordinate compression + lazy propagation.

---

## Section 7 — Graphs + Advanced Techniques (6)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 29 | Critical Connections in a Network | Graphs | **LeetCode 1192** | Tarjan's bridges |
| 30 | Swim in Rising Water | Graphs | **LeetCode 778** | Binary search + BFS / Dijkstra / DSU |
| 31 | Word Ladder II | Graphs | **LeetCode 126** | BFS (shortest path) + DFS (all paths) |
| 32 | Minimum Cost to Make at Least One Valid Path | Graphs | **LeetCode 1368** | 0-1 BFS with deque |
| 33 | The Skyline Problem | Advanced | **LeetCode 218** | Line sweep + multiset/heap |
| 34 | Alien Dictionary | Graphs | **LeetCode 269** | Topological sort with edge cases |

**Why these.** #29 is Tarjan's bridge-finding — the `tin[]` and `low[]` arrays. #30 is solvable three different ways (binary search+BFS, Dijkstra, DSU) — knowing all three is interview gold. #31 combines BFS for shortest distance with DFS for path enumeration. #32 is 0-1 BFS where turning costs 1 and going straight costs 0. #33 is the line-sweep classic. #34 has brutal edge cases (invalid ordering, missing letters).

---

## Section 8 — Stack/Heap + Complex Logic (4)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 35 | Largest Rectangle in Histogram | Stack | **LeetCode 84** | Monotonic stack |
| 36 | Maximal Rectangle | DP + Stack | **LeetCode 85** | DP (histogram per row) + monotonic stack |
| 37 | Trapping Rain Water II | Heap + BFS | **LeetCode 407** | 3D BFS + min-heap |
| 38 | Sliding Window Median | Heap | **LeetCode 480** | Two heaps + sliding window |

**Why these.** #35 is THE monotonic stack problem — if you can't solve this, you can't solve #36. #36 layers DP on top (#35 applied to each row). #37 extends Trapping Rain Water to 2D — the boundary shrinks inward using a min-heap. #38 combines the two-heap median technique with a sliding window and lazy deletion.

---

## Section 9 — Trie/String + Other (4)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 39 | Word Search II | Trie | **LeetCode 212** | Trie + grid backtracking |
| 40 | Palindrome Pairs | Trie | **LeetCode 336** | Trie + palindrome detection |
| 41 | Stream of Characters | Trie | **LeetCode 1032** | Reverse trie |
| 42 | Concatenated Words | Trie + DP | **LeetCode 472** | Trie + DP |

**Why these.** #39 is the single hardest trie problem — without the trie-guided pruning trick and removing found words, it TLEs. #40 requires storing reversed words in a trie and checking palindrome remainders. #41 flips the trie for suffix matching. #42 combines trie-accelerated dictionary lookup with word-break DP.

---

## Section 10 — Multi-Concept Compositions / Quant Favorites (8)

| # | Problem | Concepts | Platform | Note |
|---|---|---|---|---|
| 43 | Median of Two Sorted Arrays | Math/Search | **LeetCode 4** | Binary search on partitions |
| 44 | Cherry Pickup | 3D DP | **LeetCode 741** | 3D DP (two simultaneous paths) |
| 45 | N-Queens | Backtracking | **LeetCode 51** | Backtracking + diagonal bitmask |
| 46 | Smallest Range Covering Elements from K Lists | Heap + Window | **LeetCode 632** | Min-heap + sliding window |
| 47 | Max Points on a Line | Math | **LeetCode 149** | Math (GCD/slope) + hash map |
| 48 | Poor Pigs | Math | **LeetCode 458** | Information theory / combinatorial math |
| 49 | Minimum Number of Refueling Stops | Greedy/Heap | **LeetCode 871** | Greedy + max-heap |
| 50 | Minimum Cost to Hire K Workers | Greedy/Heap | **LeetCode 857** | Sort by ratio + heap |

**Why these.** #43 is asked at every quant firm — the binary search on partition is the key insight. #44 is 3D DP with two agents moving simultaneously. #45 is the classic backtracking with bitmask optimisation. #46 is heap + window. #47 is a math+hashing problem that quant firms love — GCD for slope representation avoids floating point. #48 is pure math/information theory — more a puzzle than an algorithm, perfect for quant. #49-#50 are greedy+heap compositions that test whether you can identify the greedy invariant.

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5
Section 2   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 3   [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15
Section 4   [ ] 16  [ ] 17  [ ] 18  [ ] 19  [ ] 20
Section 5   [ ] 21  [ ] 22  [ ] 23  [ ] 24
Section 6   [ ] 25  [ ] 26  [ ] 27  [ ] 28
Section 7   [ ] 29  [ ] 30  [ ] 31  [ ] 32  [ ] 33  [ ] 34
Section 8   [ ] 35  [ ] 36  [ ] 37  [ ] 38
Section 9   [ ] 39  [ ] 40  [ ] 41  [ ] 42
Section 10  [ ] 43  [ ] 44  [ ] 45  [ ] 46  [ ] 47  [ ] 48  [ ] 49  [ ] 50
```
