# 22 — Binary Search Trees: Practice Questions

> **Why 26 ranked problems.** A BST adds exactly one invariant to `../21_tree`, so this folder is
> narrower than that one — but the invariant gets *exploited* in four distinct ways, and each way
> is a separate skill. **(a) Descend by comparison** — search, insert, LCA, ceil/floor, closest;
> five problems that all reduce to "which half do I discard". **(b) In-order is sorted** — kth
> smallest, validate, recover, min-difference, BST→DLL; this is the largest group and the one
> people never connect to each other. **(c) Rebuild under the invariant** — sorted array→BST,
> preorder→BST, balance, trim; construction is where the invariant becomes a *constraint* rather
> than a shortcut. **(d) Prove a subtree is a BST** — validate and largest-BST-subtree, the two
> problems that force the bottom-up "return an Info struct" shape. Twenty-six is roughly six per
> group plus the two design problems (iterator, serialise). Fewer and group (b) collapses into
> "do an in-order traversal", which misses that the *interesting* part is where you stop.

**Platform note.** LeetCode numbers are exact. Entries marked **`[prem]`** are LeetCode Premium —
the same problem is free on GFG under the quoted title. **GeeksforGeeks has no numeric IDs.**

**Scope note.** Everything needs only `01`–`22`. **`[impl]`** marks a structural exercise rather
than a judge problem. **`[fwd]`** marks a problem whose full solution wants a later folder.

**All eight problems from your `readme.md` are covered** — see the mapping table at the bottom of
`readme.md`.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Search in a Binary Search Tree | Easy | **LeetCode 700** | `search-in-a-binary-search-tree` — **on your list** |
| 2 | Minimum and maximum in a BST | Easy | *Drill* | leftmost and rightmost node — one loop each |
| 3 | Insert into a Binary Search Tree | **Medium** | **LeetCode 701** | `insert-into-a-binary-search-tree` — **on your list** |
| 4 | Delete Node in a BST | **Medium** | **LeetCode 450** | `delete-node-in-a-bst` — **on your list**; three cases |
| 5 | Validate Binary Search Tree | **Medium** | **LeetCode 98** | `validate-binary-search-tree` — **on your list** |
| 6 | Kth Smallest Element in a BST | **Medium** | **LeetCode 230** | `kth-smallest-element-in-a-bst` |
| 7 | Lowest Common Ancestor of a BST | **Medium** | **LeetCode 235** | `lowest-common-ancestor-of-a-binary-search-tree` — **on your list** |
| 8 | In-order predecessor and successor | **Medium** | **LeetCode 285** `[prem]` | GFG *"Predecessor and Successor"* — `iinorderPrecedessor.cpp` |
| 9 | Range Sum of BST | Easy | **LeetCode 938** | `range-sum-of-bst` — prune whole subtrees |
| 10 | Minimum Absolute Difference in BST | Easy | **LeetCode 530** | `minimum-absolute-difference-in-bst` |

**Why these ten.** #1 and #2 are five-line problems whose only job is to make the descent
automatic — after them you should never write a BST search that recurses into both children. #3
introduces `root->left = insert(root->left, val)`, the reassignment pattern that #4, #25 and #26
all reuse; it is worth more than the problem itself.

**#4 is the hardest primitive in the folder** and the one interviewers actually ask. Three cases,
and the two-children case is the whole point: you cannot delete the node, so you overwrite it with
the only value allowed to sit there.

**#5 is the definitional trap.** Comparing each node only to its children passes trees that are not
BSTs. Do it with a `(lo, hi)` window *and* with the in-order `prev` check, because #10, #20 and #23
are all the second version wearing a hat.

#6 is the first "stop the traversal early" problem — the answer is not the traversal, it is *where
you stop*. **#7 is the payoff problem of the folder**: LCA on a general tree (`../21_tree` §2 #20)
needs an O(n) post-order search; here the values tell you which way to walk and it collapses to a
loop.

**#8 is where your existing file is half done.** `pred`/`succ` via the child are correct; the
ancestor case — a node with no right child — returns `-1`. Write the descend-from-the-root version
that handles both. #9 and #10 are easy, but #9 teaches *pruning* (skip a subtree entirely when its
whole range is out of bounds), which #25 needs.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Ceil and Floor in a BST | **Medium** | GFG | *"Ceil in BST"* / *"Floor in BST"* — #1 with a candidate |
| 12 | Convert Sorted Array to BST | Easy | **LeetCode 108** | `convert-sorted-array-to-binary-search-tree` — **on your list** |
| 13 | Two Sum IV — Input is a BST | Easy | **LeetCode 653** | `two-sum-iv-input-is-a-bst` |
| 14 | BST to Greater Sum Tree | **Medium** | **LeetCode 1038** | `binary-search-tree-to-greater-sum-tree` — **on your list** |
| 15 | Construct BST from Preorder Traversal | **Medium** | **LeetCode 1008** | `construct-binary-search-tree-from-preorder-traversal` |
| 16 | Flatten Binary Tree to Linked List | **Medium** | **LeetCode 114** | `flatten-binary-tree-to-linked-list` — **on your list** |
| 17 | Binary Search Tree Iterator | **Medium** | **LeetCode 173** | `binary-search-tree-iterator` — design |
| 18 | Balance a Binary Search Tree | **Medium** | **LeetCode 1382** | `balance-a-binary-search-tree` — #12 after an in-order |
| 19 | Closest Value in a BST | Easy | **LeetCode 270** `[prem]` | GFG *"Find the closest element in BST"* |
| 20 | Trim a Binary Search Tree | **Medium** | **LeetCode 669** | `trim-a-binary-search-tree` — #4's return pattern |

**Why these ten.** #11 and #19 are the same shape as #1 with one addition — **remember the best
candidate seen so far while descending** — and that shape is also #8's answer. Doing the three
together makes the connection obvious; doing them a month apart does not.

**#12 is the inverse of "in-order is sorted"**, and picking the middle element as root is what
makes the result balanced. #18 is literally #12 run on the output of an in-order traversal, which
is worth noticing rather than solving from scratch.

**#13 is worth doing three ways** and comparing: in-order into an array then two pointers (O(n)
space), a hash set during traversal (O(n) space, one pass), or **two BST iterators walking inward
from the smallest and the largest** (O(h) space). The third is the answer they want and it needs
#17 first.

**#14 and #15 are the two "reversed" problems.** #14 needs a *reverse* in-order — right, node,
left — so values arrive in descending order and a running sum is all you need. #15 needs the
observation that a preorder sequence plus an upper bound is enough to know where each subtree
ends; the `bound` parameter *is* the algorithm.

#16 you have already met in `../21_tree`; redo it here, because after #14 the "reverse pre-order
with a `prev` pointer" version writes itself. **#17 is the design problem of the folder** — the
stack holds exactly the left spine, `next()` is O(1) amortised, and the space is O(h) rather than
O(n). #20 pairs with #9: both prune by range, one sums and one rebuilds.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 21 | Recover Binary Search Tree | **Hard** | **LeetCode 99** | `recover-binary-search-tree` — two swapped nodes |
| 22 | Largest BST Subtree | **Medium** | **LeetCode 333** `[prem]` | GFG *"Largest BST"* — bottom-up Info struct |
| 23 | Morris in-order traversal | **Hard** | *Drill* `[impl]` | `morrisTraversal.cpp` — **yours does not run** |
| 24 | Binary Tree to Doubly Linked List | **Hard** | GFG | *"Binary Tree to DLL"* — in-order with a `prev` |
| 25 | Serialize and Deserialize BST | **Medium** | **LeetCode 449** | `serialize-and-deserialize-bst` — shorter than 297, and why |
| 26 | Merge two BSTs | **Hard** | GFG | *"Merge two BST's"* — two iterators, O(h1+h2) space |

**Why these six.** **#21 is the best in-order problem there is.** The sorted sequence has either
one descent (adjacent nodes swapped) or two (non-adjacent), and getting the one-descent case right
is what separates a working solution from one that passes the sample. Do it after #10, which is the
same walk with a different question.

**#22 is the structural lesson of the folder.** You cannot answer "is this subtree a BST" by
looking down — you have to return `{isBST, size, min, max}` upward and combine. That
**return-a-struct-upward** shape is the general form of the one-pass trick from `../21_tree` §5,
and it recurs in `../26_dp` on trees.

**#23 is in your folder and does not work.** `while(!curr)` exits immediately, `if(!curr->left)`
is inverted, and the thread-building block is nested inside the wrong loop. Rewriting it from the
invariant rather than patching it is the exercise. #24 is Morris's easier cousin — same in-order
walk, but you keep a `prev` pointer and rewire as you go.

**#25 makes a point 297 cannot.** Serialising a *general* binary tree needs explicit nulls;
serialising a **BST** needs only the preorder values, because the invariant supplies the structure
— which is #15 read backwards. #26 is the capstone: two BSTs, merge without flattening both into
arrays, using two iterators from #17.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Convert Sorted List to BST | **Medium** | **LeetCode 109** | #12 on a linked list — no random access |
| Increasing Order Search Tree | Easy | **LeetCode 897** | in-order, relink right-only |
| Find Mode in Binary Search Tree | Easy | **LeetCode 501** | duplicates are adjacent in-order |
| Minimum Distance Between BST Nodes | Easy | **LeetCode 783** | #10 under a different name |
| All Elements in Two BSTs | **Medium** | **LeetCode 1305** | two iterators, merge step of merge sort |
| Convert BST to Greater Tree | **Medium** | **LeetCode 538** | #14, identical |
| Insert into a sorted circular linked list | **Medium** | **LeetCode 708** `[prem]` | the same "where does it go" question, no tree |
| Second minimum in a BST | Easy | *Drill* | in-order, stop at the second value |
| Count nodes in a value range | Easy | *Drill* | #9 counting instead of summing |
| Check if an array is the preorder of some BST | **Medium** | GFG | *"Preorder to BST"* — a stack, not a tree |
| Kth largest in a BST | **Medium** | GFG | reverse in-order — #6 mirrored |
| Sum of k smallest elements | Easy | *Drill* | #6 accumulating instead of returning |
| Unique Binary Search Trees | **Medium** | **LeetCode 96** `[fwd]` | Catalan numbers — DP, `../26_dp` |
| Unique Binary Search Trees II | **Medium** | **LeetCode 95** `[fwd]` | build all of them; recursion + memo |
| Maximum Sum BST in Binary Tree | **Hard** | **LeetCode 1373** `[fwd]` | #22 carrying a sum too |
| Closest Nodes Queries in a BST | **Medium** | **LeetCode 2476** `[fwd]` | flatten, then binary search per query |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5
            [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 2   [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15
            [ ] 16  [ ] 17  [ ] 18  [ ] 19  [ ] 20
Section 3   [ ] 21  [ ] 22  [ ] 23  [ ] 24  [ ] 25  [ ] 26
Section 4   [ ] ______ / 16
```
