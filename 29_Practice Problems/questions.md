# 29 — Revision Set: 150 Problems Before the Interview

**This is not a learning list. It is a revision list.** Every problem here assumes you have already
worked through `01`–`28`; nothing new is introduced. The job of this file is to make sure that when
a problem arrives with no folder name attached, you still recognise what it is.

---

## How to use it

| Stage | What to do | Time |
|---|---|---|
| **Pass 1 — recognise** | Read each problem. Write down *only the technique and the complexity*. Do not code. | ~4 hours total |
| **Pass 2 — solve** | Code the ones you could not name in Pass 1, plus every **★** | 4–6 weeks |
| **Pass 3 — speed** | Re-solve the **★** set against a clock: Easy 10 min, Medium 20, Hard 35 | the last 2 weeks |

**Pass 1 is the one people skip and the one that matters.** Interview failure is almost never "I
did not know Dijkstra"; it is "I did not notice this was Dijkstra". Twenty seconds per problem
naming the technique is worth more than one problem solved slowly.

**★ marks the 87 highest-frequency problems.** If you have a month, do those. If you have two
weeks, use the explicit **50-problem cut** at the bottom of this file.

---

## About the company tags

Tags come from **publicly reported interview experiences** — LeetCode's own company lists,
GeeksforGeeks archives, Glassdoor, and Blind. They mean *"this has been reported repeatedly at this
company"*, not that it is on any current question bank. Companies rotate their sets and none of
this is authoritative.

**Use the tags to prioritise, never to predict.** A problem tagged with four companies is worth
doing first because it is a *widely useful pattern*, which is also why four companies converged on
it. Treat the tag as a proxy for "this pattern is load-bearing".

Abbreviations: **A** Amazon · **G** Google · **M** Meta · **MS** Microsoft · **AP** Apple ·
**N** Netflix · **U** Uber · **B** Bloomberg · **L** LinkedIn · **AB** Adobe

`[prem]` marks LeetCode Premium; each is free on GeeksforGeeks under a similar title.

---

## 1. Arrays and Hashing — 18

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 1 | ★ | Two Sum | 1 | Easy | hash map, value→index | A G M MS AP |
| 2 | ★ | Best Time to Buy and Sell Stock | 121 | Easy | one pass, min so far | A M B |
| 3 | | Contains Duplicate | 217 | Easy | hash set | A AP |
| 4 | ★ | Product of Array Except Self | 238 | Med | prefix × suffix, **no division** | A M AP |
| 5 | ★ | Maximum Subarray | 53 | Med | Kadane | A MS L |
| 6 | ★ | Merge Intervals | 56 | Med | sort by start, then merge | M G A B |
| 7 | | Insert Interval | 57 | Med | three phases: before, overlap, after | G M |
| 8 | | Non-overlapping Intervals | 435 | Med | greedy, sort by **end** | A G |
| 9 | ★ | Group Anagrams | 49 | Med | map keyed on a signature | A U M |
| 10 | ★ | Top K Frequent Elements | 347 | Med | count, then **bucket** (O(n)) or heap | A M G |
| 11 | ★ | Longest Consecutive Sequence | 128 | Med | hash set; only start at run heads | G M A |
| 12 | | Valid Sudoku | 36 | Med | three sets of sets; the box index | A AP U |
| 13 | | Set Matrix Zeroes | 73 | Med | use row 0 / col 0 as the flags | A MS |
| 14 | ★ | Spiral Matrix | 54 | Med | four shrinking boundaries | MS A G |
| 15 | | Rotate Image | 48 | Med | transpose, then reverse each row | A MS AP |
| 16 | | Majority Element | 169 | Easy | Boyer–Moore vote | A AB |
| 17 | ★ | Sort Colors | 75 | Med | Dutch national flag, three pointers | MS A M |
| 18 | | Next Permutation | 31 | Med | find the pivot, swap, reverse the tail | G A |

---

## 2. Two Pointers and Sliding Window — 14

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 19 | | Valid Palindrome | 125 | Easy | two pointers, skip non-alphanumeric | M A |
| 20 | | Two Sum II — Sorted | 167 | Med | two pointers inward | A |
| 21 | ★ | 3Sum | 15 | Med | sort, fix one, two-pointer; **skip duplicates** | M A AB |
| 22 | ★ | Container With Most Water | 11 | Med | two pointers, move the shorter side | A G B |
| 23 | ★ | Trapping Rain Water | 42 | **Hard** | two pointers, or a monotonic stack | A G M AP |
| 24 | ★ | Longest Substring Without Repeating | 3 | Med | window + last-index map | A M G B |
| 25 | ★ | Longest Repeating Character Replacement | 424 | Med | window; `len − maxFreq ≤ k` | G A |
| 26 | | Permutation in String | 567 | Med | fixed window + count comparison | M MS |
| 27 | ★ | Minimum Window Substring | 76 | **Hard** | window + a `missing` counter | M A U L |
| 28 | ★ | Sliding Window Maximum | 239 | **Hard** | monotonic **deque**, O(n) | A G M |
| 29 | ★ | Subarray Sum Equals K | 560 | Med | prefix sum + map; seed `count[0]=1` | M A G |
| 30 | | Find All Anagrams in a String | 438 | Med | fixed window of counts | A U |
| 31 | | Fruit Into Baskets | 904 | Med | longest window with ≤ 2 distinct | G A |
| 32 | | Max Consecutive Ones III | 1004 | Med | longest window with ≤ k zeros | A G |

---

## 3. Binary Search — 10

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 33 | | Binary Search | 704 | Easy | the invariant, and `lo+(hi-lo)/2` | — |
| 34 | ★ | Search in Rotated Sorted Array | 33 | Med | one half is always sorted | A M MS |
| 35 | ★ | Find Minimum in Rotated Sorted Array | 153 | Med | compare mid against `hi` | A M MS |
| 36 | | Search a 2D Matrix | 74 | Med | treat it as one flat array | A MS |
| 37 | ★ | Koko Eating Bananas | 875 | Med | **binary search on the answer** | G M A |
| 38 | ★ | Median of Two Sorted Arrays | 4 | **Hard** | partition the smaller array | A G M AP |
| 39 | | Find First and Last Position | 34 | Med | lower_bound and upper_bound | M A L |
| 40 | | Capacity To Ship Packages in D Days | 1011 | Med | search on the answer — same as #37 | A G |
| 41 | | Split Array Largest Sum | 410 | **Hard** | search on the answer, or DP | G A |
| 42 | | Find Peak Element | 162 | Med | binary search with no sorted array | M G MS |

---

## 4. Linked List — 12

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 43 | ★ | Reverse Linked List | 206 | Easy | three pointers; know it cold | **all** |
| 44 | ★ | Merge Two Sorted Lists | 21 | Easy | dummy head | A AP M |
| 45 | ★ | Linked List Cycle | 141 | Easy | Floyd's tortoise and hare | A M MS |
| 46 | | Reorder List | 143 | Med | mid + reverse + merge — three sub-skills | M A |
| 47 | | Remove Nth Node From End | 19 | Med | two pointers `n` apart | A M |
| 48 | ★ | Copy List with Random Pointer | 138 | Med | old→new map, or interleave for O(1) | A M MS B |
| 49 | ★ | Add Two Numbers | 2 | Med | carry propagation, dummy head | A MS AB B |
| 50 | ★ | Merge k Sorted Lists | 23 | **Hard** | min heap of size k, or divide and conquer | A G M AP |
| 51 | ★ | Reverse Nodes in k-Group | 25 | **Hard** | count first, then reverse a block | M MS G |
| 52 | ★ | LRU Cache | 146 | Med | hash map + doubly linked list | A M G MS B |
| 53 | ★ | Find the Duplicate Number | 287 | Med | Floyd's on the *index* graph | A G M |
| 54 | | Intersection of Two Linked Lists | 160 | Easy | swap heads at the end; lengths equalise | A M B |

---

## 5. Stack and Queue — 9

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 55 | ★ | Valid Parentheses | 20 | Easy | stack; the canonical warm-up | A G M B |
| 56 | ★ | Min Stack | 155 | Med | a second stack, or store the delta | A M B |
| 57 | | Evaluate Reverse Polish Notation | 150 | Med | stack of operands | A L |
| 58 | ★ | Generate Parentheses | 22 | Med | backtracking with two counters | A G U |
| 59 | ★ | Daily Temperatures | 739 | Med | monotonic decreasing stack | A M G |
| 60 | ★ | Largest Rectangle in Histogram | 84 | **Hard** | monotonic increasing stack | A G M |
| 61 | | Implement Queue using Stacks | 232 | Easy | two stacks; **amortised** O(1) | M B |
| 62 | | Asteroid Collision | 735 | Med | stack with a collision rule | A U |
| 63 | | Basic Calculator II | 227 | Med | stack; hold the previous number | A G M |

---

## 6. Trees and BST — 16

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 64 | ★ | Invert Binary Tree | 226 | Easy | swap children, recurse | G A |
| 65 | | Maximum Depth of Binary Tree | 104 | Easy | the bottom-up template | A L |
| 66 | ★ | Diameter of Binary Tree | 543 | Easy | **one pass**, height + reference answer | M A G |
| 67 | | Balanced Binary Tree | 110 | Easy | one pass with a sentinel return | A M |
| 68 | | Same Tree | 100 | Easy | recurse on both at once | A M |
| 69 | | Subtree of Another Tree | 572 | Easy | #68 called at every node | A M |
| 70 | | Lowest Common Ancestor of a BST | 235 | Med | descend by comparison — O(h), no recursion | M A MS |
| 71 | ★ | Binary Tree Level Order Traversal | 102 | Med | queue + `int sz = q.size()` | A MS M |
| 72 | ★ | Binary Tree Right Side View | 199 | Med | level order, keep the last | M A |
| 73 | | Count Good Nodes in Binary Tree | 1448 | Med | carry the max-so-far downward | M A |
| 74 | ★ | Validate Binary Search Tree | 98 | Med | `(lo, hi)` window, `long long` bounds | A M MS |
| 75 | ★ | Kth Smallest Element in a BST | 230 | Med | in-order, stop after k | A M G |
| 76 | ★ | Construct Tree from Preorder and Inorder | 105 | Med | index map + a shared forward pointer | A M B |
| 77 | ★ | Binary Tree Maximum Path Sum | 124 | **Hard** | one pass; a negative subtree contributes 0 | M A G |
| 78 | ★ | Serialize and Deserialize Binary Tree | 297 | **Hard** | preorder **with explicit nulls** | A M G L |
| 79 | ★ | Lowest Common Ancestor of a Binary Tree | 236 | Med | both sides non-null ⇒ I am the split | M A MS |

---

## 7. Tries — 3

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 80 | ★ | Implement Trie | 208 | Med | `next[26]` + `isEnd` | A G M MS |
| 81 | | Design Add and Search Words | 211 | Med | DFS over all children at a `.` | A M G |
| 82 | ★ | Word Search II | 212 | **Hard** | Trie **+** grid DFS — the classic pairing | A G M U |

---

## 8. Heap and Priority Queue — 8

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 83 | ★ | Kth Largest Element in an Array | 215 | Med | size-k **min** heap, or quickselect | M A G |
| 84 | | Last Stone Weight | 1046 | Easy | max heap, pop two | A |
| 85 | ★ | K Closest Points to Origin | 973 | Med | size-k **max** heap; no `sqrt` | M A G |
| 86 | ★ | Task Scheduler | 621 | Med | greedy heap, or the closed-form | M A AP |
| 87 | | Design Twitter | 355 | Med | maps + a k-way merge | A M |
| 88 | ★ | Find Median from Data Stream | 295 | **Hard** | **two heaps**, balanced within one | A G M |
| 89 | | Reorganize String | 767 | Med | most frequent first, hold one back | G A |
| 90 | ★ | Meeting Rooms II | 253 `[prem]` | Med | min heap of end times, or sweep line | M G A B |

---

## 9. Backtracking — 9

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 91 | ★ | Subsets | 78 | Med | take / skip at each index | M A G |
| 92 | ★ | Combination Sum | 39 | Med | reuse allowed ⇒ do not advance `i` | A U M |
| 93 | ★ | Permutations | 46 | Med | used[] or swap in place | A MS M |
| 94 | | Subsets II | 90 | Med | sort, then skip equal siblings | M A |
| 95 | ★ | Word Search | 79 | Med | grid DFS + **undo** the mark | A MS M |
| 96 | | Palindrome Partitioning | 131 | Med | front partition + a palindrome check | A G |
| 97 | ★ | Letter Combinations of a Phone Number | 17 | Med | the plainest backtracking there is | A M G U |
| 98 | | Combination Sum II | 40 | Med | #92 with duplicates and no reuse | A |
| 99 | ★ | N-Queens | 51 | **Hard** | three conflict sets: col, `r+c`, `r−c` | A G AP |

---

## 10. Graphs — 16

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 100 | ★ | Number of Islands | 200 | Med | grid flood fill; the archetype | A G M MS |
| 101 | ★ | Clone Graph | 133 | Med | map old→new, register **before** recursing | M G A |
| 102 | | Max Area of Island | 695 | Med | #100 returning a size | A G M |
| 103 | ★ | Pacific Atlantic Water Flow | 417 | Med | BFS **inward** from both edges, intersect | G A M |
| 104 | | Surrounded Regions | 130 | Med | start from the border — solve the complement | A G |
| 105 | ★ | Rotting Oranges | 994 | Med | **multi-source** BFS; levels are minutes | A M G |
| 106 | ★ | Course Schedule | 207 | Med | Kahn — is it a DAG? | A M G AP |
| 107 | ★ | Course Schedule II | 210 | Med | #106 returning the order | A M G |
| 108 | | Redundant Connection | 684 | Med | DSU; the first `unite` that fails | A G |
| 109 | | Number of Connected Components | 323 `[prem]` | Med | DSU or DFS sweep | A G |
| 110 | ★ | Word Ladder | 127 | **Hard** | BFS on an **implicit** graph | A G M L |
| 111 | ★ | Network Delay Time | 743 | Med | Dijkstra, plainly | A G |
| 112 | ★ | Cheapest Flights Within K Stops | 787 | Med | **Bellman–Ford** — Dijkstra is wrong here | A G M |
| 113 | ★ | Alien Dictionary | 269 `[prem]` | **Hard** | build the DAG, then topo sort | G A AB |
| 114 | ★ | Accounts Merge | 721 | Med | DSU over accounts, keyed by email | M A G |
| 115 | | Critical Connections in a Network | 1192 | **Hard** | Tarjan bridges, `low[v] > tin[u]` | A G |

---

## 11. Dynamic Programming — 22

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 116 | | Climbing Stairs | 70 | Easy | fib; the conversion drill | A AP AB |
| 117 | | Min Cost Climbing Stairs | 746 | Easy | #116 with a cost | A |
| 118 | ★ | House Robber | 198 | Med | take / skip — the archetype | A G M |
| 119 | | House Robber II | 213 | Med | run #118 twice on a circle | A G |
| 120 | ★ | Longest Palindromic Substring | 5 | Med | expand around **2n−1** centres | A MS AB |
| 121 | | Palindromic Substrings | 647 | Med | #120 counting instead | M A |
| 122 | ★ | Decode Ways | 91 | Med | 1-D DP; the base cases are the problem | M A G U |
| 123 | ★ | Coin Change | 322 | Med | unbounded knapsack, minimising | A G M AP |
| 124 | ★ | Maximum Product Subarray | 152 | Med | carry **both** max and min | A L MS |
| 125 | ★ | Word Break | 139 | Med | `dp[j]` + a prefix check; or a Trie | A G M U |
| 126 | ★ | Longest Increasing Subsequence | 300 | Med | O(n²), then `lower_bound` for O(n log n) | MS A G |
| 127 | ★ | Partition Equal Subset Sum | 416 | Med | subset sum to `total/2` | A M G |
| 128 | | Unique Paths | 62 | Med | grid DP, one row of space | A B |
| 129 | ★ | Longest Common Subsequence | 1143 | Med | **the** two-sequence template | A G M |
| 130 | | Buy and Sell Stock with Cooldown | 309 | Med | state machine | G A |
| 131 | | Coin Change II | 518 | Med | counting; **loop order decides the answer** | A G |
| 132 | | Target Sum | 494 | Med | subset sum after the algebra | M G |
| 133 | ★ | Edit Distance | 72 | **Hard** | three neighbours, three operations | G A M |
| 134 | | Burst Balloons | 312 | **Hard** | interval DP; think **last**, not first | G A |
| 135 | | Regular Expression Matching | 10 | **Hard** | two-sequence DP with `*` | M G U |
| 136 | ★ | Jump Game | 55 | Med | greedy furthest-reach beats the DP | A M MS |
| 137 | ★ | Maximal Square | 221 | Med | `1 + min` of three neighbours | A M AP |

---

## 12. Greedy and Intervals — 6

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 138 | ★ | Jump Game II | 45 | Med | BFS-style level counting | A G |
| 139 | ★ | Gas Station | 134 | Med | total ≥ 0 ⇒ a start exists; reset on deficit | A MS G |
| 140 | | Hand of Straights | 846 | Med | ordered map, always start at the smallest | G |
| 141 | ★ | Partition Labels | 763 | Med | last-index map, then one sweep | A M |
| 142 | | Valid Parenthesis String | 678 | Med | track a **range** of open counts | M A |
| 143 | | Minimum Arrows to Burst Balloons | 452 | Med | sort by end, greedy — same as #8 | A G |

---

## 13. Bit Manipulation and Math — 7

| # | ★ | Problem | LC | Diff | Technique | Companies |
|---|---|---|---|---|---|---|
| 144 | ★ | Single Number | 136 | Easy | XOR cancels pairs | A AP |
| 145 | | Number of 1 Bits | 191 | Easy | `n & (n-1)` clears the lowest set bit | A AP MS |
| 146 | | Counting Bits | 338 | Easy | `dp[i] = dp[i>>1] + (i&1)` | A AP |
| 147 | | Reverse Bits | 190 | Easy | shift out, shift in | A AP |
| 148 | | Missing Number | 268 | Easy | XOR, or the sum formula | A MS |
| 149 | | Sum of Two Integers | 371 | Med | XOR for the sum, AND<<1 for the carry | A MS |
| 150 | ★ | Pow(x, n) | 50 | Med | fast exponentiation; **mind `n = INT_MIN`** | M A G B |

---

## The ★ set — 87 problems, the one-month list

```
Arrays      1   2   4   5   6   9   10  11  14  17
Two ptr     21  22  23  24  25  27  28  29
Bin search  34  35  37  38
Linked list 43  44  45  48  49  50  51  52  53
Stack       55  56  58  59  60
Trees       64  66  71  72  74  75  76  77  78  79
Trie        80  82
Heap        83  85  86  88  90
Backtrack   91  92  93  95  97  99
Graphs      100 101 103 105 106 107 110 111 112 113 114
DP          118 120 122 123 124 125 126 127 129 133 136 137
Greedy      138 139 141
Bits        144 150
```

## The hard cut — exactly 50, for two weeks

One representative per pattern, chosen so that no technique is left unrepresented. If you can only
do fifty, do these fifty.

```
Arrays      1   4   5   6   10  11                      (6)
Two ptr     21  23  24  27  29                          (5)
Bin search  34  37  38                                  (3)
Linked list 43  45  48  50  52                          (5)
Stack       55  56  59                                  (3)
Trees       66  71  74  76  77  78  79                  (7)
Trie        80  82                                      (2)
Heap        83  88  90                                  (3)
Backtrack   91  93  97                                  (3)
Graphs      100 101 105 106 110 112 114                 (7)
DP          118 123 125 129 133                         (5)
Greedy      139                                         (1)
```

**Why the cut falls where it does.** Trees and graphs keep the most slots because they carry the
most distinct sub-patterns — a tree traversal does not teach you construction, and a grid BFS does
not teach you topological sort. Arrays and DP lose the most, because within those the problems
genuinely are variations: solve #123 Coin Change and #131 Coin Change II costs you twenty minutes,
not a slot.

---

## Progress tracker

```
Pass 1 (recognise)   [ ] 1-25   [ ] 26-50   [ ] 51-75   [ ] 76-100  [ ] 101-125  [ ] 126-150
Pass 2 (solve)       [ ] ______ / 150
Pass 3 (timed ★)     [ ] ______ /  87
Hard cut (2 weeks)   [ ] ______ /  50
```

---

## What is deliberately *not* here

- **Segment trees, Fenwick trees, heavy-light decomposition, suffix automata.** Competitive
  programming, not interviews. They are described in `../22_bst/advanced_tree_readme.md` so you can
  discuss them; you will not be asked to code one.
- **Anything needing a library you cannot write from memory.**
- **Puzzle questions with no algorithmic content.** Largely retired at the companies tagged above.
- **System design.** A separate discipline, and it is not what this repo is for.

Thirty **theoretical** questions — the conceptual ones asked alongside the coding, and the ones a
problem list cannot cover — are in `solution.md`, with answers.
