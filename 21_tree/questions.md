# 21 — Binary Trees: Practice Questions

> **Why 34 ranked problems — the largest set in the repo.** Trees are not one technique, they are
> a *substrate*: five folders after this one are trees with a constraint added. The set breaks
> into six groups that transfer independently — **traversals** (four orders, each in recursive and
> iterative form, because the iterative versions are separately asked), **measures** computed
> bottom-up (height, diameter, balance — where the lesson is "one pass, not two"), **structural
> comparison** (same/mirror/subtree), **root-to-leaf path** problems that carry state downward and
> must backtrack, **level-order** problems where the level boundary is the whole trick, and
> **construction/serialisation**, which is the hardest group and the one that proves you
> understand what a traversal actually encodes. Thirty-four is roughly five per group. Below that
> you drop a group; the iterative traversals alone are three problems people fail on sight.
>
> **Plus 16 in Section 5**, added later and held to a different standard: those are problems where
> the tree is only half the answer and the other half comes from `22`–`27`. They are not part of
> the 34 because you cannot solve them yet.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search the
quoted title.

**Scope note.** Sections 1–4 need only `01`–`20`. Entries marked **`[impl]`** are structural
exercises rather than judge problems. **`[fwd]`** marks a problem whose natural home is a later
folder.

**Section 5 is different** — it deliberately breaks the scope rule. Those problems need BSTs,
maps, heaps, DP or graphs, and they are listed here rather than in those folders because the
*tree* part is the hard part. Come back to them once `22`–`27` are done.

**All twelve problems from your `readme.md` are covered**, and the one you marked **#imp** (236)
is in Section 2.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Build a tree from a level-order array | **Medium** | *Drill* `[impl]` | `bianryTree.cpp:38` — **yours is correct** |
| 2 | Binary Tree Preorder Traversal | Easy | **LeetCode 144** | `binary-tree-preorder-traversal` — **on your list** |
| 3 | Binary Tree Inorder Traversal | Easy | **LeetCode 94** | `binary-tree-inorder-traversal` — **on your list** |
| 4 | Binary Tree Postorder Traversal | Easy | **LeetCode 145** | `binary-tree-postorder-traversal` — **on your list** |
| 5 | Binary Tree Level Order Traversal | **Medium** | **LeetCode 102** | `binary-tree-level-order-traversal` — **on your list** |
| 6 | Maximum Depth of Binary Tree | Easy | **LeetCode 104** | `maximum-depth-of-binary-tree` — `nodeTree.cpp:42` |
| 7 | Invert Binary Tree | Easy | **LeetCode 226** | `invert-binary-tree` — **on your list** |
| 8 | Same Tree | Easy | **LeetCode 100** | `same-tree` — **on your list** |
| 9 | Symmetric Tree | Easy | **LeetCode 101** | `symmetric-tree` — #8 with the sides crossed |
| 10 | Diameter of Binary Tree | Easy | **LeetCode 543** | `diameter-of-binary-tree` — **on your list**; one pass |
| 11 | Balanced Binary Tree | Easy | **LeetCode 110** | `balanced-binary-tree` — one pass, sentinel return |
| 12 | Size, sum and maximum of a tree | Easy | *Drill* | `nodeTree.cpp` — **`sumOfTree` and `sizeOfTree` are correct** |

**Why these twelve.** #1 first, because without a way to *build* a tree you cannot test anything
else — and your `constructATree` already works, so this is a matter of reusing it.

**#2–#4 must be done twice: recursively and iteratively.** The recursive versions differ by one
line and take five minutes; the iterative ones are genuinely separate problems. Iterative in-order
in particular has a shape nothing else shares, and iterative post-order has the
"pre-order mirrored, then reversed" trick. Interviewers ask for the iterative form specifically
because the recursive one proves nothing.

#5 is where `int sz = q.size()` enters, and it recurs in five later problems. #6 is the simplest
bottom-up recursion and the place to fix the `NULL` base case — **yours returns `INT16_MIN` and
produces negative heights**. #7–#9 are structural recursion; #9 is #8 with the arguments crossed,
which is worth noticing rather than writing from scratch.

**#10 and #11 are the "one pass, not two" pair**, and they are the most valuable entries in the
section. Both are O(n²) if you call `height()` at every node and O(n) if you compute the height and
the answer in the same traversal. Doing them together makes the technique stick.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 13 | Binary Tree Paths | Easy | **LeetCode 257** | `binary-tree-paths` — **on your list**; carry state down |
| 14 | Path Sum | Easy | **LeetCode 112** | `path-sum` — subtract as you descend |
| 15 | Path Sum II | **Medium** | **LeetCode 113** | `path-sum-ii` — **on your list**; backtracking |
| 16 | Minimum Depth of Binary Tree | Easy | **LeetCode 111** | `minimum-depth-of-binary-tree` — **not** the mirror of #6 |
| 17 | Binary Tree Right Side View | **Medium** | **LeetCode 199** | `binary-tree-right-side-view` — **on your list** |
| 18 | Zigzag Level Order Traversal | **Medium** | **LeetCode 103** | `binary-tree-zigzag-level-order-traversal` |
| 19 | Level Order Traversal II (bottom-up) | **Medium** | **LeetCode 107** | `binary-tree-level-order-traversal-ii` — reverse at the end |
| 20 | Lowest Common Ancestor | **Medium** | **LeetCode 236** | `lowest-common-ancestor-of-a-binary-tree` — **on your list (imp)** |
| 21 | Boundary Traversal | **Medium** | GFG | *"Boundary Traversal of binary tree"* — `bianryTree.cpp:150` |
| 22 | Subtree of Another Tree | Easy | **LeetCode 572** | `subtree-of-another-tree` — #8 called at every node |

**Why these ten.** #13–#15 are the top-down family: state travels *downward* as an argument, and
#15 introduces the shared-buffer plus `pop_back` discipline from `../10_Recursion` §4. **#16 is
the trap in this section** — minimum depth is *not* `1 + min(left, right)`, because a `NULL` child
is not a leaf; a node with one child must take the non-null side. Getting #6 right and #16 wrong
is the standard outcome.

#17–#19 are all #5 with one modification each (take the last of the level, alternate direction,
reverse the result), which is the point of grouping them — the level-order skeleton is reusable
and these prove it.

**#20 is the problem you flagged as important, and it deserves it.** The whole algorithm is four
lines, and the insight — "if both subtrees return non-null, I am the split point" — is one of the
genuinely elegant results in this folder. #21 is where your existing code has a real bug on skewed
trees; the fix is structural, not a patch. #22 combines #8 with a traversal and is a good check
that you can compose two solutions.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 23 | Construct Tree from Preorder and Inorder | **Medium** | **LeetCode 105** | `construct-binary-tree-from-preorder-and-inorder-traversal` — **on your list** |
| 24 | Path Sum III | **Medium** | **LeetCode 437** | `path-sum-iii` — **on your list**; prefix sums on a path |
| 25 | Binary Tree Maximum Path Sum | **Hard** | **LeetCode 124** | `binary-tree-maximum-path-sum` |
| 26 | Serialize and Deserialize Binary Tree | **Hard** | **LeetCode 297** | `serialize-and-deserialize-binary-tree` |
| 27 | Construct Tree from Inorder and Postorder | **Medium** | **LeetCode 106** | `construct-binary-tree-from-inorder-and-postorder-traversal` |
| 28 | Morris Inorder Traversal | **Hard** | GFG | *"Inorder Traversal (Iterative)"* — O(1) space |
| 29 | Flatten Binary Tree to Linked List | **Medium** | **LeetCode 114** | `flatten-binary-tree-to-linked-list` |
| 30 | Vertical Order Traversal | **Hard** | **LeetCode 987** | `vertical-order-traversal-of-a-binary-tree` |
| 31 | All Nodes Distance K | **Medium** | **LeetCode 863** | `all-nodes-distance-k-in-binary-tree` — parent pointers + BFS |
| 32 | Count Complete Tree Nodes | **Medium** | **LeetCode 222** | `count-complete-tree-nodes` — better than O(n) |
| 33 | Maximum Width of Binary Tree | **Medium** | **LeetCode 662** | `maximum-width-of-binary-tree` — index arithmetic |
| 34 | Burning Tree / time to infect | **Medium** | GFG | *"Burning Tree"* `[fwd]` — the same idea as #31 |

**Why these twelve.** **#23 is the most instructive problem in the folder.** Pre-order gives you
the root; finding that root in the in-order array splits it into the left and right subtrees.
Building it recursively — with a hash map for O(1) index lookup and a *shared* pre-order pointer
that only ever moves forward — is the moment the traversals stop being outputs and start being
*encodings*. #27 is the same idea with post-order read backwards, so do it second and it takes ten
minutes.

**#24 is `../13_prefixSum` applied to a tree.** The count of downward paths summing to `k` uses a
running-sum map exactly as LeetCode 560 does — the only addition is **undoing the map entry on the
way back up**, because a prefix on one branch is not a prefix on another. That undo is the whole
problem. #25 is the same "one pass with a reference answer" shape as #10, with the extra wrinkle
that a negative subtree contributes 0 rather than its value.

#26 makes the point from Q7 concrete — a single traversal *with explicit nulls* does determine the
tree. #28 is the O(1)-space answer worth having. **#31 is where trees meet graphs**: add parent
pointers and a tree becomes an undirected graph, then BFS. That reframing is direct preparation for
`../27_graphs`, and #34 is the same technique with a different question.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Sum of Left Leaves | Easy | **LeetCode 404** | pass a "am I a left child" flag downward |
| Average of Levels | Easy | **LeetCode 637** | #5 with a running sum per level |
| Merge Two Binary Trees | Easy | **LeetCode 617** | recurse on both at once, like #8 |
| Count Good Nodes | **Medium** | **LeetCode 1448** | carry the max-so-far downward |
| Cousins in Binary Tree | Easy | **LeetCode 993** | same depth, different parent |
| Sum Root to Leaf Numbers | **Medium** | **LeetCode 129** | `n = n*10 + val` carried down |
| Delete Leaves With a Given Value | **Medium** | **LeetCode 1325** | post-order — children first |
| Diameter in **nodes** rather than edges | Easy | *Drill* | `L + R + 1`; know which convention is asked |
| Top view / Bottom view of a tree | **Medium** | GFG | *"Top View of Binary Tree"* — horizontal distance + a map |
| Left view of a tree | Easy | GFG | *"Left View of Binary Tree"* — #17 mirrored |
| Print all ancestors of a node | **Medium** | GFG | *"Ancestors in Binary Tree"* — post-order with a bool return |
| Check if two trees are mirrors | Easy | *Drill* | #9's helper, exposed |
| Level with the maximum sum | Easy | GFG | *"Maximum sum of nodes in a level"* — #5 plus a max |
| Iterative postorder with **one** stack | **Hard** | GFG | *"Postorder traversal using one stack"* — harder than the two-stack version |
| Convert a tree to its mirror in place | Easy | *Drill* | #7 without returning a new tree |
| Children Sum Property | **Medium** | GFG | *"Children Sum Parent"* — post-order validation |

---

## Section 5 — Advanced / multi-concept

**These are not harder tree problems. They are tree problems that stop being tree problems**
partway through — the traversal is the easy half and the answer lives in a technique from another
folder. That is what makes them interview questions rather than exercises.

Every entry below needs at least one topic this folder does not cover. The **Needs** column says
which, so you can defer a problem until you have the prerequisite rather than bouncing off it.

`solution.md` §5 explains what each one is asking and which ideas it combines — **it does not give
the answers**, deliberately. The value of this section is entirely in solving them yourself.

### 5a — DP on trees

| # | Problem | Difficulty | Platform | Needs |
|---|---|---|---|---|
| 35 | House Robber III | **Medium** | **LeetCode 337** | `../26_dp` — return a *pair* per node |
| 36 | Binary Tree Cameras | **Hard** | **LeetCode 968** | `../26_dp` — three-state post-order |
| 37 | Longest Path With Different Adjacent Characters | **Hard** | **LeetCode 2246** | `../26_dp` + `../09_Strings` |
| 38 | Sum of Distances in Tree | **Hard** | **LeetCode 834** | `../26_dp` + `../27_graphs` — rerooting |
| 39 | Kth Ancestor of a Tree Node | **Hard** | **LeetCode 1483** | `../26_dp` + `../15_bitwise` — binary lifting |

**Why these five.** #35 is the gateway: the moment a node needs to return *two* numbers (best-if-I
-take-this-node, best-if-I-skip-it) instead of one, `../21_tree` §5(a) becomes DP. #36 is the same
shape with three states and a greedy tie-break, and it is one of the hardest tree problems on
LeetCode. #38 and #39 are the two that change your model of what a tree is — #38 computes the
answer for one root and then *slides* it to every other root in O(1) each, and #39 stores 2ʲ-th
ancestors so a query jumps by powers of two.

### 5b — the tree is a BST, or contains one

| # | Problem | Difficulty | Platform | Needs |
|---|---|---|---|---|
| 40 | Maximum Sum BST in Binary Tree | **Hard** | **LeetCode 1373** | `../22_bst` — Info struct upward |
| 41 | Closest BST Values II (k closest) | **Hard** | **LeetCode 272** `[prem]` | `../22_bst` + `../25_heap` |
| 42 | Merge BSTs to Create Single BST | **Hard** | **LeetCode 1932** | `../22_bst` + `../24_maps` |
| 43 | Unique Binary Search Trees II | **Medium** | **LeetCode 95** | `../22_bst` + `../26_dp` — Catalan |

**Why these four.** #40 is `../22_bst` §3 #22 with a sum carried alongside, and it is the cleanest
example of the return-a-struct-upward pattern. #41 has two good answers — a size-k heap, or an
in-order walk with two pointers — and comparing them is the exercise. **#42 is the nastiest problem
in this section**: it is a graph-assembly problem disguised as a BST problem, and getting the
"exactly one root survives" bookkeeping right is most of the work.

### 5c — maps, sets and hashing on trees

| # | Problem | Difficulty | Platform | Needs |
|---|---|---|---|---|
| 44 | Find Duplicate Subtrees | **Medium** | **LeetCode 652** | `../24_maps` — serialise, then count |
| 45 | Most Frequent Subtree Sum | **Medium** | **LeetCode 508** | `../24_maps` + `../25_heap` |
| 46 | Create Binary Tree From Descriptions | **Medium** | **LeetCode 2196** | `../23_sets` + `../24_maps` |
| 47 | Smallest Missing Genetic Value in Each Subtree | **Hard** | **LeetCode 2003** | `../23_sets` — small-to-large merging |
| 48 | Throne Inheritance | **Medium** | **LeetCode 1600** | `../24_maps` — n-ary tree, design |

**Why these five.** #44 makes the point from Q7 operational — you *hash the serialisation* of each
subtree, so "are these two subtrees identical" becomes a string comparison. #46 is the reverse of
every problem so far: you are given edges and must find which node is the root, which is a
set-difference. **#47 is the one worth the most** — the naive solution is O(n²) and the fix is
"always merge the smaller set into the larger", the same amortised argument that makes union-by-size
work in `../28_DSU`.

### 5d — the tree is really a graph

| # | Problem | Difficulty | Platform | Needs |
|---|---|---|---|---|
| 49 | Minimum Height Trees | **Medium** | **LeetCode 310** | `../27_graphs` — peel leaves inward |
| 50 | Linked List in Binary Tree | **Medium** | **LeetCode 1367** | `../17_linked_list` |

**Why these two.** #49 is given as an edge list, not a root pointer, and there is no root — which
is the moment you realise "tree" and "rooted tree" are different things. #50 is the odd one out on
purpose: two different pointer structures at once, and the bug everyone hits is restarting the list
match at the wrong place.

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6
            [ ] 7   [ ] 8   [ ] 9   [ ] 10  [ ] 11  [ ] 12
Section 2   [ ] 13  [ ] 14  [ ] 15  [ ] 16  [ ] 17
            [ ] 18  [ ] 19  [ ] 20  [ ] 21  [ ] 22
Section 3   [ ] 23  [ ] 24  [ ] 25  [ ] 26  [ ] 27  [ ] 28
            [ ] 29  [ ] 30  [ ] 31  [ ] 32  [ ] 33  [ ] 34
Section 4   [ ] ______ / 16
Section 5   [ ] 35  [ ] 36  [ ] 37  [ ] 38  [ ] 39   (dp on trees)
            [ ] 40  [ ] 41  [ ] 42  [ ] 43           (bst)
            [ ] 44  [ ] 45  [ ] 46  [ ] 47  [ ] 48   (maps / sets)
            [ ] 49  [ ] 50                           (graphs / lists)
```
