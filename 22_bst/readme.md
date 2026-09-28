# 22 — Binary Search Trees

A binary tree with **one extra invariant**, and that invariant buys you everything:

> For every node: **all values in the left subtree < node < all values in the right subtree.**

Not "left child < node < right child". The condition is about the whole **subtree**, and
confusing the two is the single most common BST bug — it is exactly what LeetCode 98 tests.

Everything in `../21_tree` still applies; a BST *is* a binary tree. What changes is that you no
longer have to search both children. At every node the comparison tells you which half to
discard — which is `../11_linearAndBinarySearch` running on pointers instead of indices.

---

## 1. The one property that generates all the others

```
        50
      /    \
    30      70
   /  \    /  \
  20  40  60  80
```

**In-order traversal of a BST gives the values in sorted order.** `20 30 40 50 60 70 80`.

That single fact is the answer to about half of this folder:

| Problem | Restated as an in-order fact |
|---|---|
| Validate a BST | is the in-order sequence strictly increasing? |
| kth smallest | stop the in-order walk after k pops |
| Two elements swapped by mistake | find the descents in the in-order sequence |
| BST → sorted list / array | just write the in-order output down |
| In-order successor | the next node the in-order walk would visit |

If a BST problem is stalling, ask **"what does this look like in the sorted sequence?"** first.

---

## 2. Search, insert, delete

**Search is binary search.** Compare, go left or right, never both:

```cpp
TreeNode* searchBST(TreeNode* root, int val) {
    while (root && root->val != val)
        root = val < root->val ? root->left : root->right;
    return root;
}
```

`O(h)`. Iterative uses O(1) space, which is why the loop form is worth preferring here.

**Insert** always lands at a leaf. Walk down as if searching; where you fall off the tree, attach:

```cpp
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    if (val < root->val) root->left  = insertIntoBST(root->left,  val);
    else                 root->right = insertIntoBST(root->right, val);
    return root;                             // reassignment is how the parent gets relinked
}
```

**`root->left = insert(root->left, ...)` is the pattern to internalise.** The function returns the
new subtree root, and the caller stores it. Delete uses the same shape, and so does every
self-balancing tree.

**Delete is the only hard primitive**, because of three cases:

| The node has | Do this |
|---|---|
| no children | return `NULL` — it simply disappears |
| one child | return that child — the parent adopts a grandchild |
| two children | **copy the in-order successor's value into this node, then delete the successor from the right subtree** |

The two-children case works because the in-order successor is the smallest value greater than this
node, so it is the only value that can sit here without breaking the invariant. And the successor
is the leftmost node of the right subtree, which by definition **has no left child** — so deleting
it recursively can only ever hit the easy cases. The recursion is bounded at depth two.

---

## 3. Successor and predecessor — the case people forget

Your `iinorderPrecedessor.cpp` handles this correctly:

```
predecessor(node) = rightmost node of node->left
successor(node)   = leftmost  node of node->right
```

**But that is only the case where the child exists.** If the node has no right child, its successor
is *an ancestor* — the lowest ancestor for which this node is in the **left** subtree. Standing at
`40` in the tree above, there is no right child, and the answer is `50`.

The version that handles both cases does not walk up at all — it walks **down from the root**,
remembering the last time it turned left:

```cpp
TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
    TreeNode* ans = NULL;
    while (root) {
        if (p->val < root->val) { ans = root; root = root->left; }  // candidate, then go smaller
        else                      root = root->right;
    }
    return ans;
}
```

O(h), no parent pointers, and it covers the leaf case for free. Predecessor is the mirror: record
when you turn **right**.

---

## 4. Validation — the range, not the neighbour

```cpp
bool validate(TreeNode* r, long long lo, long long hi) {
    if (!r) return true;
    if (r->val <= lo || r->val >= hi) return false;
    return validate(r->left, lo, r->val) && validate(r->right, r->val, hi);
}
```

Each node inherits a **window** and narrows it for its children. Checking only
`left->val < node->val < right->val` passes this tree, which is not a BST:

```
      5
    /   \
   1     4          <- 3 is in the RIGHT subtree of 5 but smaller than 5
        / \
       3   6
```

**Use `long long` for the bounds**, or `TreeNode*` bounds that start as `NULL`. A node whose value
is genuinely `INT_MIN` breaks the `INT_MIN` sentinel version — the same class of bug as the
`INT16_MIN` sentinels in `../21_tree/nodeTree.cpp`.

The alternative is one in-order walk checking `prev < cur`, which is shorter and is exactly the
machinery LeetCode 99 needs anyway.

---

## 5. Height is the whole story

Every operation here is **O(h)**, not O(log n). On a balanced tree `h = O(log n)`; on a BST built
by inserting `1,2,3,4,5` in order, `h = n` and the tree is a linked list:

```
1
 \
  2
   \
    3 ...
```

Search degrades to O(n) and you have paid for pointers to get an array's worst case.

**Self-balancing trees exist precisely to stop this** — AVL, red-black, and the B-trees under every
database index. `std::set` and `std::map` in `../23_sets` and `../24_maps` are red-black trees, and
their guaranteed `O(log n)` is the reason to reach for them instead of hand-rolling. See
`advanced_tree_readme.md` in this folder for what those structures actually do.

---

## 6. Morris traversal, done right

Morris walks in-order in **O(1) space** by temporarily pointing each node's in-order predecessor at
it, then removing the thread on the way back:

```cpp
TreeNode* cur = root;
while (cur) {
    if (!cur->left) {                       // nothing to the left: visit, go right
        visit(cur);
        cur = cur->right;
    } else {
        TreeNode* pre = cur->left;          // rightmost node of the left subtree
        while (pre->right && pre->right != cur) pre = pre->right;
        if (!pre->right) { pre->right = cur; cur = cur->left; }   // build the thread, descend
        else             { pre->right = NULL; visit(cur); cur = cur->right; }  // remove, visit
    }
}
```

Three things decide whether it works:

1. **`while (cur)`**, not `while (!cur)`.
2. **`if (!cur->left)` means "no left subtree, so visit now"** — the visit happens in that branch,
   not the other one.
3. The `pre->right != cur` in the inner condition is what stops the thread you built from sending
   you round in a circle.

The tree is **left exactly as it was found** — every thread that gets built also gets removed. That
non-destructiveness is the point of the algorithm; a version that forgets to clear `pre->right`
leaves a cycle behind.

---

## Interview Q&A

**Q1. Define a BST precisely.**
For every node, *every* value in its left subtree is smaller and *every* value in its right subtree
is larger. State it about subtrees, not children — the child-only version is wrong and it is what
the question is checking.

**Q2. What is the time complexity of BST search?**
**O(h)** — O(log n) if balanced, O(n) if degenerate. Never say "O(log n)" without the balance
caveat; inserting sorted data produces the degenerate case, and that is the follow-up question.

**Q3. How do you delete a node with two children?**
Replace its value with its in-order successor (leftmost node of the right subtree), then delete
that successor from the right subtree. The predecessor works equally well. The successor is
guaranteed to have no left child, so the recursive delete only ever hits the easy cases.

**Q4. How do you validate a BST?**
Carry a `(lo, hi)` window down; each node must lie strictly inside it and passes a narrowed window
to each child. Or do one in-order walk and check that it is strictly increasing. Use `long long`
bounds so a node holding `INT_MIN` does not false-negative.

**Q5. Why is LCA easier on a BST than on a general binary tree?**
Because the values tell you which way to go. If both targets are smaller than the node, the answer
is in the left subtree; if both are larger, the right; otherwise the paths split **here** and this
node is the LCA. O(h) with no recursion and no bookkeeping, versus the O(n) post-order search a
general binary tree needs (`../21_tree` Q5).

**Q6. Find the kth smallest element.**
In-order walk, stop after k nodes — O(h + k). If the tree is queried repeatedly, store a subtree
size in each node; then each step compares k against `size(left) + 1` and descends, making it O(h)
per query. That augmentation is the expected follow-up.

**Q7. Two nodes of a BST were swapped by mistake. Find them.**
In-order the values are sorted except for the swap. Walk in-order tracking `prev`; each place where
`prev > cur` is a descent. **Two** descents means a non-adjacent swap — take the first `prev` and
the second `cur`. **One** descent means the swapped nodes were adjacent — take that `prev` and
`cur`. Swap the values back.

**Q8. What happens if you insert sorted data into a BST?**
You get a right-skewed degenerate tree of height n, and every operation becomes O(n). This is why
self-balancing variants exist and why `std::set` is a red-black tree rather than a plain BST.

**Q9. Can a BST hold duplicates?**
Not under the strict definition, and LeetCode 98 rejects them. Real implementations pick a
convention — send equals consistently to one side, or store a count per node. `std::multiset` takes
the second approach in spirit. Say which convention you are using before you write code.

**Q10. Convert a BST to a sorted doubly linked list in place.**
In-order traversal, keeping a `prev` pointer: set `prev->right = cur` and `cur->left = prev` at
each visit. `left` becomes `prev` and `right` becomes `next`. O(n) time, O(h) stack — or O(1) with
Morris.

---

## Your original notes (preserved)

**All eight are covered** — see `questions.md`.

```
701 700 235
98 1038 108 
450 114
```

| Your entry | Now at | | Your entry | Now at |
|---|---|---|---|---|
| 700 Search in a BST | §1 #2 | | 701 Insert into a BST | §1 #3 |
| 450 Delete node in a BST | §1 #4 | | 98 Validate BST | §1 #5 |
| 235 LCA of a BST | §1 #7 | | 108 Sorted array → BST | §2 #12 |
| 1038 BST to greater sum tree | §2 #14 | | 114 Flatten to linked list | §2 #16 |

`114` is a general binary-tree problem rather than a BST one — it is also
`../21_tree/questions.md` §3 #29. It is kept here because the in-order/pre-order framing you
meet in this folder makes it much clearer the second time.
