# 33 — Hard Questions: Multi-Concept Problems

*Note: Code in this repository strictly adheres to a C++14 ceiling (`g++ -std=gnu++14`). No C++17 features are used.*

This is a **strategy guide** and practice folder, not a theory document. There are no new data structures to learn here. Instead, the focus is entirely on combining what you already know.

---

## 1. Why This Folder Exists

Real FAANG and quantitative finance interviews rarely ask you to just "implement a trie" or "write a BFS." Instead, they present scenarios that **combine** concepts.

The primary gap between solving individual topic-based problems and successfully solving complex interview problems is the ability to **recognize** which concepts apply. When a problem stops being a textbook "DP problem" and becomes a "DP with Segment Tree optimization," many candidates freeze.

This folder contains 50 Hard problems where the difficulty comes from combining 2 to 3 distinct concepts. The prerequisites for solving these problems span folders `01` through `32` in this repository.

---

## 2. The Concept Map — What Combines With What

In advanced interview problems, certain data structures and algorithms naturally pair together. Here are the most common combinations:

| Combination | What It Looks Like | Example |
|---|---|---|
| **DP + Trees** | Compute optimal value at each node using children's results | Binary Tree Cameras |
| **DP + Graphs** | Shortest/longest path with state (BFS + memo or topo sort + DP) | Shortest Path Visiting All Nodes |
| **DP + Bitmask** | Subset enumeration where state is a bitmask of visited/used items | Shortest Superstring (TSP variant) |
| **DP + Binary Search** | Optimise a DP transition using binary search on monotonic property | Super Egg Drop, Russian Doll Envelopes |
| **DP + Strings** | Two-sequence matching/alignment problems | Regular Expression Matching |
| **Segment Tree + DP** | Use segment tree to accelerate a DP transition from $O(n^2)$ to $O(n \log n)$ | LIS II |
| **Graphs + DSU** | Connectivity queries mixed with edge additions/deletions | Swim in Rising Water |
| **Trie + Backtracking**| Multi-word search on a grid using trie-guided DFS | Word Search II |
| **Stack + DP** | Histogram-style problems where monotonic stack finds boundaries | Maximal Rectangle |
| **Greedy + Heap** | Process items in sorted order, maintaining a priority queue of candidates | Min Cost to Hire K Workers |
| **Binary Search + Graph**| Binary search on answer, verify with BFS/DFS | Swim in Rising Water |
| **Math + Hash Map** | Geometric or arithmetic properties + efficient lookup | Max Points on a Line |

---

## 3. How to Approach a Multi-Concept Problem in an Interview

When confronted with a multi-concept problem, follow this structured approach to break it down:

1. **Read the problem. Classify the INPUT**: Is the input an Array? Graph? Tree? String? Grid? This quickly narrows down the *primary concept*.
2. **Classify the QUESTION**: What are you asked to do? Optimise (min/max)? Count? Check existence? Find the shortest path? This narrows down the *primary technique* (e.g., DP for counting, BFS for shortest path).
3. **Check for constraints**: 
    - $n \le 20$ heavily implies bitmask DP or backtracking.
    - $n \le 10^5$ with range queries implies a segment tree or Fenwick tree.
    - A weighted graph implies Dijkstra's or Bellman-Ford algorithms.
4. **Look for the SECOND concept**: After identifying the primary structure and technique, ask yourself: *"What makes this harder than the textbook version of this problem?"* The answer to that question is almost always the second concept.
5. **Verify time complexity**: Map out your approach. If your proposed solution is $O(n^3)$ and $n = 10^5$, you immediately know you need an optimization layer—like a segment tree, binary search, or a monotonic stack—to bring the complexity down to a feasible $O(n \log n)$.

---

## 4. Constraint-Based Heuristics

Constraints are the biggest hints in competitive programming and technical interviews. Use this quick-reference table to guess the expected time complexity and approach:

| Constraint | Likely Approach |
|---|---|
| **$n \le 10-15$** | Bitmask DP, brute-force with pruning (backtracking) |
| **$n \le 20-25$** | Bitmask DP, meet in the middle |
| **$n \le 100-500$** | $O(n^3)$ interval DP, $O(n^2)$ DP (e.g., Floyd-Warshall) |
| **$n \le 10^3-10^4$** | $O(n^2)$ DP, $O(n \sqrt{n})$ (Mo's algorithm), segment tree |
| **$n \le 10^5$** | $O(n \log n)$ — sort, binary search, segment tree, heap |
| **$n \le 10^6-10^7$** | $O(n)$ — linear scan, prefix sum, two pointers, sliding window |
| **$n \le 10^9$** | $O(\log n)$ — binary search, math, matrix exponentiation |

---

## 5. Interview Q&A

### Q1: How do you decide between DP and greedy in an interview?
**A:** If you can prove that a local optimum always leads to a global optimum, use greedy. If making a choice now affects the viability or optimality of future choices, and you must consider multiple paths, it's DP. When in doubt during an interview, try to break a greedy approach with a small counter-example; if you can break it, fall back to DP.

### Q2: When should you suspect bitmask DP?
**A:** The biggest giveaway is $n \le 20$. If a problem asks for permutations, subsets, or asks you to visit a small set of items and you need to keep track of *which* specific items have been visited as part of your state, it is almost certainly a bitmask DP. Examples include the Traveling Salesperson Problem or assigning $N$ jobs to $N$ workers.

### Q3: How do you recognise that a segment tree can optimise a DP?
**A:** You will notice this when you write the DP transition. If your state looks like `dp[i] = max(dp[j]) + cost` for some range `l <= j <= r`, and finding that maximum natively takes $O(n)$, your overall DP will be $O(n^2)$. If $n = 10^5$, $O(n^2)$ is too slow. A segment tree can compute `max(dp[j])` over that range in $O(\log n)$, reducing the total time to $O(n \log n)$.

### Q4: What's the difference between "DP on trees" and "DFS + memoisation on a graph"?
**A:** DP on trees (often called "in-out DP" or "tree DP") leverages the fact that a tree is a Directed Acyclic Graph (DAG) when rooted, meaning there are no cycles and natural subproblems exist at each subtree. DFS + memoisation on a graph requires you to track visited nodes or state carefully to handle or avoid cycles. Tree DP builds the answer bottom-up (or top-down) using the strict parent-child hierarchy.

### Q5: How do you handle problems where you don't immediately see the approach?
**A:** Do not panic. Start by working through a small, concrete example by hand. Pay attention to the constraints—they are massive hints. Classify the input structure. If you are truly stuck, write out a brute-force solution. The inefficiencies in the brute-force solution (e.g., recalculating the same states, searching linearly instead of logarithmically) will often reveal the data structure or algorithm needed to optimize it.
