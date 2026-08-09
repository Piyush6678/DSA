# 21 — Binary Trees

The first branching structure. Everything before this was linear — arrays, lists, stacks,
queues — and everything after it (`../22_bst`, `../25_heap`, `../27_graphs`) is a tree with a
constraint added.

Trees are where `../10_Recursion` pays off. Almost every tree problem is three lines: handle
`NULL`, recurse into both children, combine. The difficulty is never the recursion — it is
deciding **what to return upward** and **what to carry downward**.

---

## 1. The node, and the vocabulary

```cpp
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node(int val) { this->val = val; left = NULL; right = NULL; }
};
```

Two pointers instead of one, and that is the entire difference from `../17_linked_list`.
Initialising both to `NULL` in the constructor is what makes every `if (root->left)` check
meaningful — the same discipline as a list node.

| Term | Meaning |
|---|---|
| **Height** of a node | edges (or nodes) on the longest path down to a leaf |
| **Depth** of a node | edges from the root down to it |
| **Level** | all nodes at the same depth |
| **Leaf** | `!left && !right` |
| **Degree** | number of children (0, 1 or 2) |

**Height and depth are measured in opposite directions** and are constantly confused. Also agree
with yourself whether height counts *nodes* or *edges* — a single node has height 1 in nodes and
0 in edges. Both conventions are used; pick one and say which.

| Tree | Definition |
|---|---|
| **Full** | every node has 0 or 2 children |
| **Complete** | every level filled except possibly the last, which fills left to right |
| **Perfect** | full *and* all leaves at the same depth — exactly `2^h - 1` nodes |
| **Balanced** | height difference between the subtrees of every node is ≤ 1 |
| **Degenerate** | every node has one child — a linked list wearing a tree costume |

**A tree with `n` nodes has `n-1` edges**, always. And a *balanced* tree has height `O(log n)`
while a degenerate one has height `O(n)` — which is why every "O(log n)" claim about trees quietly
assumes balance.

---

## 2. The four traversals

```
PRE-order   root, left, right      work BEFORE the recursive calls
IN-order    left, root, right      work BETWEEN them
POST-order  left, right, root      work AFTER both
LEVEL-order breadth first          a queue, not recursion
```

The first three differ **only in where the `cout` line sits**. That is exactly the
before/after-the-call distinction from `../10_Recursion` §3 — your `PreInPost.cpp` in that folder
demonstrated all three positions in one function, and this is what it was preparing you for.

```cpp
void inorder(Node* root) {
    if (!root) return;              // 1. base case FIRST, always
    inorder(root->left);
    cout << root->val << " ";       // 2. move this line to change the traversal
    inorder(root->right);
}
```

**When to use which:**

| Traversal | Use it when |
|---|---|
| Pre-order | you need to process a node **before** its children — copying/serialising a tree |
| In-order | **sorted order on a BST** (`../22_bst`) — this is the one with a special property |
| Post-order | you need results **from both children first** — height, deleting a tree, most DP-on-trees |
| Level-order | anything about *levels* — right side view, minimum depth, zigzag |

**Post-order is the workhorse.** "Compute something for both subtrees, then combine" describes
height, diameter, balance, max path sum, and LCA. If you are unsure which traversal a problem
wants, it is usually post-order.

---

## 3. Level-order, and the one line that matters

```cpp
queue<Node*> q;
q.push(root);
while (!q.empty()) {
    int sz = q.size();              // <-- FREEZE the level boundary before consuming
    for (int i = 0; i < sz; ++i) {
        Node* n = q.front(); q.pop();
        // ... process n, knowing which level it is on
        if (n->left)  q.push(n->left);
        if (n->right) q.push(n->right);
    }
    // one level finished here
}
```

**`int sz = q.size();` before the inner loop is what separates the levels.** Without it you get a
flat BFS with no idea where one level ends and the next begins — and level boundaries are the
whole point of problems like right side view, zigzag, and minimum depth.

**Guard the root.** `q.push(root)` with a `NULL` root then dereferences it on the first
iteration — an immediate crash, and it is the most common omission in this pattern.

This is the same skeleton as BFS in `../20_queu` §3 and `../27_graphs`. Learning it here means
graph traversal is a variation, not a new topic.

---

## 4. Iterative traversals — what the stack is doing

Recursion uses the call stack; the iterative versions make it explicit.

**Pre-order** is the easy one — push right before left, so left pops first:

```cpp
st.push(root);
while (!st.empty()) {
    Node* n = st.top(); st.pop();
    visit(n);
    if (n->right) st.push(n->right);    // RIGHT first
    if (n->left)  st.push(n->left);
}
```

**In-order** needs a different shape, because you must reach the leftmost node before visiting
anything:

```cpp
while (cur || !st.empty()) {
    while (cur) { st.push(cur); cur = cur->left; }   // dive left, remembering the path
    cur = st.top(); st.pop();
    visit(cur);
    cur = cur->right;
}
```

**Post-order** has a trick: run pre-order with the children swapped (root, right, left) and
**reverse the output**. That gives left, right, root. Writing genuine one-stack post-order is
possible but fiddly, and this is the version to know.

**Morris traversal** does in-order in **O(1) space** by temporarily threading each node's
in-order predecessor's right pointer to it, then undoing the thread on the way back. It is the
answer to "can you traverse without recursion *and* without a stack?" — and the important
property is that it **restores the tree**, so it is non-destructive despite mutating during the
walk.

---

## 5. The two questions every tree recursion answers

Almost every problem here is one of these shapes:

**(a) Bottom-up — return a value, combine at each node.**

```
solve(node):
    if node is NULL: return <identity>
    L = solve(node->left)
    R = solve(node->right)
    return combine(L, R, node->val)
```

Height, size, sum, balance, diameter, max path sum. The identity for `NULL` is the detail — 0 for
height/size/sum, `INT_MIN` for maximum, `true` for "all children satisfy X".

**(b) Top-down — carry state into the recursion.**

```
solve(node, stateSoFar):
    if node is NULL: return
    stateSoFar = update(stateSoFar, node)
    if leaf: record(stateSoFar)
    solve(node->left, stateSoFar); solve(node->right, stateSoFar)
```

Root-to-leaf paths, path sums, depth tracking. If the state is a shared mutable buffer, you must
**backtrack** (`pop_back` after both calls) — the same discipline as `../10_Recursion` §4.

**The "one pass instead of two" trick** shows up repeatedly: computing height *and* the answer in
the same recursion, using a reference parameter for the answer. Diameter, balance and max path sum
are all O(n) this way and O(n²) if you recompute height at every node.

---

## Interview Q&A

**Q1. What is the difference between height and depth?**
Depth is measured from the root down to the node; height from the node down to its deepest leaf.
The root's depth is 0; a leaf's height is 0 (edge convention). Always state whether you are
counting edges or nodes — a single node is height 0 or 1 depending on that choice.

**Q2. Why does in-order matter more than the others?**
Because on a **BST** it produces the values in sorted order. That single property is the basis of
validate-BST, kth-smallest, and BST-to-sorted-list — all of `../22_bst`. On a plain binary tree
in-order has no special meaning.

**Q3. How do you traverse level by level?**
A queue, and **capture `q.size()` before processing each level**. That count is exactly the number
of nodes on the current level, so the inner loop handles one level and the outer loop counts them.

**Q4. Compute the diameter in one pass.**
Write a helper that returns the height and updates a by-reference `best` with `L + R` at every
node — the longest path *through* that node. One O(n) traversal. Calling a separate `height()` at
every node is O(n²), which is the answer they are checking you avoid.

**Q5. Explain the LCA algorithm for a general binary tree.**
Recurse. If the node is `NULL` or matches `p` or `q`, return it. Otherwise take the results from
both subtrees: if **both** are non-null, `p` and `q` lie on opposite sides, so this node is the
LCA; if only one is non-null, propagate it upward. O(n) time, O(h) stack. On a **BST** you can do
better using the ordering (`../22_bst`).

**Q6. Can you traverse a tree without recursion and without a stack?**
Morris traversal — O(1) space. Thread each node's in-order predecessor's right pointer to the node,
follow it back up, then remove the thread. It mutates the tree during the walk but **restores it**,
so the tree is unchanged afterwards.

**Q7. Which single traversal uniquely determines a tree?**
None. Pre-order plus in-order does (pre-order gives the root, in-order splits into subtrees); so
does post-order plus in-order. **Pre-order plus post-order does not** — it is ambiguous for nodes
with one child. A single traversal *with explicit nulls* also works, which is how serialisation
is done.

**Q8. What is the space complexity of a recursive tree traversal?**
O(h) for the call stack, where `h` is the height. That is **O(log n) for a balanced tree and O(n)
for a degenerate one** — worth stating as a range, because the worst case is what breaks in
production.

---

## Your original notes (preserved)

The problem list from this file is kept verbatim. **All twelve are covered** — see `questions.md`.

```
lc 
543 Diameter of binary tree
100 same tree
226 invert binary tree
257 Binary tree paths
236 lowest common ancestor #imp
144 preorder 94 in order  145 post order  
102 level order traversal
199 right side view
113 path sum 2 
437 path sum 3 
105 cionsutruct a tree 
```

| Your entry | Now at | | Your entry | Now at |
|---|---|---|---|---|
| 144 / 94 / 145 traversals | §1 #2–#4 | | 102 Level order | §1 #5 |
| 226 Invert tree | §1 #7 | | 100 Same tree | §1 #8 |
| 543 Diameter | §1 #10 | | 257 Binary tree paths | §2 #13 |
| 113 Path Sum II | §2 #15 | | 199 Right side view | §2 #17 |
| 236 LCA **(imp)** | §2 #20 | | 105 Construct from pre+in | §3 #23 |
| 437 Path Sum III | §3 #24 | | | |

The one you marked **#imp** — 236 — is in Section 2 with the two problems it depends on
immediately before it.
