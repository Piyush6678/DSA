# 22 — Binary Search Trees: Solutions

Approach and pseudocode first. Full C++ appears only where the problem is a **fundamental**, an
**implementation**, **hard**, or turns on a **trick** — everything here has been compiled and run.

Node type used throughout:

```cpp
struct TreeNode {
    int val; TreeNode *left, *right;
    TreeNode(int v) : val(v), left(NULL), right(NULL) {}
};
```

---

## Section 1 — Must Do

### 1. Search in a BST — LC 700 *(fundamental)*

Compare, discard half, repeat. This is `../11_linearAndBinarySearch` with pointers instead of
indices.

```cpp
TreeNode* searchBST(TreeNode* root, int val) {
    while (root && root->val != val)
        root = val < root->val ? root->left : root->right;
    return root;
}
```

**O(h) time, O(1) space.** The recursive version is equally short but costs O(h) stack for nothing.

---

### 2. Minimum and maximum in a BST *(fundamental)*

```
min = walk left  until left  is NULL
max = walk right until right is NULL
```

Two `while` loops. O(h). Both are used inside delete and inside predecessor/successor, so write
them as reusable helpers rather than inline.

---

### 3. Insert into a BST — LC 701 *(fundamental)*

Descend as if searching; where you fall off, that is where the new leaf belongs.

```cpp
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    if (val < root->val) root->left  = insertIntoBST(root->left,  val);
    else                 root->right = insertIntoBST(root->right, val);
    return root;
}
```

**The reassignment is the lesson.** `root->left = f(root->left)` lets the child function decide
what the subtree becomes — new node, unchanged node, or a different node entirely. Delete (#4),
trim (#20) and delete-and-forest all use exactly this.

---

### 4. Delete Node in a BST — LC 450 *(implementation, three cases)*

```
delete(root, key):
    if root is NULL:        return NULL
    if key < root->val:     root->left  = delete(root->left,  key)
    elif key > root->val:   root->right = delete(root->right, key)
    else:                                        # found it
        if no left  child:  return root->right   # covers "no children" too
        if no right child:  return root->left
        succ = leftmost node of root->right      # smallest value greater than root
        root->val = succ->val
        root->right = delete(root->right, succ->val)
    return root
```

```cpp
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) return NULL;
    if (key < root->val)      root->left  = deleteNode(root->left,  key);
    else if (key > root->val) root->right = deleteNode(root->right, key);
    else {
        if (!root->left)  return root->right;
        if (!root->right) return root->left;
        TreeNode* succ = root->right;
        while (succ->left) succ = succ->left;
        root->val = succ->val;
        root->right = deleteNode(root->right, succ->val);
    }
    return root;
}
```

**Why the two-children case terminates fast.** The successor is the leftmost node of the right
subtree, so it has **no left child** — the recursive delete on it can only hit the one-child or
no-child branch. The recursion never nests more than one extra level.

Verified: deleting a leaf, a one-child node, the root, an absent key, and the only node in a
one-node tree all behave, and the tree still validates afterwards.

---

### 5. Validate BST — LC 98 *(fundamental, and the definitional trap)*

**Window version** — each node inherits `(lo, hi)` and narrows it:

```cpp
bool validate(TreeNode* r, long long lo, long long hi) {
    if (!r) return true;
    if (r->val <= lo || r->val >= hi) return false;
    return validate(r->left, lo, r->val) && validate(r->right, r->val, hi);
}
bool isValidBST(TreeNode* root) { return validate(root, LLONG_MIN, LLONG_MAX); }
```

**In-order version** — walk in-order and check `prev < cur` at every step. Shorter, and it is the
same machinery #10, #21 and #24 need, so it is the one to internalise.

Two things that catch people:

- Use **`long long`** bounds. A node genuinely holding `INT_MIN` fails the `INT_MIN`-sentinel
  version. (This is the same class of bug as the `INT16_MIN` sentinels in `../21_tree`.)
- Comparisons are **strict**. A duplicate value is not a valid BST under the LeetCode definition.

Verified against the LC 98 sample `[5,1,4,null,null,3,6]` (rejected), a tree holding both `INT_MIN`
and `INT_MAX` (accepted), and a duplicate (rejected).

---

### 6. Kth Smallest — LC 230 *(fundamental)*

In-order gives sorted order, so the answer is "stop after k pops". The iterative form is preferred
because you can actually stop.

```cpp
int kthSmallest(TreeNode* root, int k) {
    stack<TreeNode*> st; TreeNode* cur = root;
    while (cur || !st.empty()) {
        while (cur) { st.push(cur); cur = cur->left; }
        cur = st.top(); st.pop();
        if (--k == 0) return cur->val;
        cur = cur->right;
    }
    return -1;
}
```

**O(h + k)**, not O(n) — the walk stops early. Recursively you need a by-reference counter and an
early return, which works but does not stop the traversal, only the work.

**Follow-up they will ask:** if the tree is modified and queried often, store `leftCount` in each
node. Then each step compares `k` with `leftCount + 1` and descends — **O(h) per query**.

---

### 7. LCA of a BST — LC 235 *(fundamental)*

```
while root:
    if both p,q < root:  go left
    elif both p,q > root: go right
    else: root is the LCA          # the paths split here, or root IS p or q
```

```cpp
TreeNode* lcaBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root) {
        if (p->val < root->val && q->val < root->val)      root = root->left;
        else if (p->val > root->val && q->val > root->val) root = root->right;
        else return root;
    }
    return NULL;
}
```

**O(h), O(1) space, no recursion.** Compare with `../21_tree` §2 #20, where a general binary tree
needs a full O(n) post-order search. The `else` branch also handles "one of them *is* the current
node", so no special case is needed.

---

### 8. In-order predecessor and successor — LC 285 *(trick — the case your file misses)*

The version everyone writes first only covers the child case:

```
predecessor = rightmost node of node->left      # only if a left child exists
successor   = leftmost  node of node->right     # only if a right child exists
```

When the child does **not** exist, the answer is an ancestor. Rather than walking up, descend from
the root and remember the last useful turn:

```cpp
TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
    TreeNode* ans = NULL;
    while (root) {
        if (p->val < root->val) { ans = root; root = root->left; }   // candidate; try smaller
        else                      root = root->right;
    }
    return ans;
}
```

Predecessor is the mirror — record the node when you turn **right**.

**Why it works.** The successor is the smallest value strictly greater than `p`. Every time you
turn left you have found *a* value greater than `p`; each later left turn finds a smaller one. The
last candidate recorded is the smallest such value.

Verified on the tree `50/30,70/20,40,60,80`: `succ(20)=30`, `succ(40)=50` (climbs two ancestors),
`succ(80)=NULL`, `pred(60)=50`, `pred(20)=NULL`.

---

### 9. Range Sum of BST — LC 938

```
sum(node, lo, hi):
    if node is NULL: return 0
    if node->val < lo: return sum(node->right, lo, hi)   # whole left subtree is too small
    if node->val > hi: return sum(node->left,  lo, hi)   # whole right subtree is too large
    return node->val + sum(node->left) + sum(node->right)
```

The two early returns are **pruning** — an entire subtree is skipped, not visited-and-ignored. That
is the difference between using the invariant and merely traversing. Same idea in #20.

---

### 10. Minimum Absolute Difference — LC 530

In sorted order the closest pair is **adjacent**, so only consecutive in-order values need
comparing.

```
prev = NULL; best = INF
inorder(node):
    recurse left
    if prev exists: best = min(best, node->val - prev)
    prev = node->val
    recurse right
```

O(n) time, O(h) space. Sorting an array of all values would also work and is strictly worse — it
throws away the ordering the tree already has. LC 783 is the same problem.

---

## Section 2 — Important

### 11 & 19. Ceil, Floor, Closest value

All three are #1 plus a remembered candidate:

```
ceil(root, x):        # smallest value >= x
    ans = -1
    while root:
        if root->val == x: return x
        if root->val <  x: root = root->right
        else: ans = root->val; root = root->left
    return ans
```

Floor mirrors it (record when going right). Closest keeps `best` by `abs(root->val - x)` and
descends by comparison, updating at every node. **Note this is the same shape as #8** — descend,
record a candidate on one branch, and the last candidate wins.

---

### 12. Sorted Array → balanced BST — LC 108 *(fundamental)*

The middle element must be the root, or the halves are unbalanced.

```cpp
TreeNode* build(vector<int>& a, int lo, int hi) {
    if (lo > hi) return NULL;
    int mid = lo + (hi - lo) / 2;
    TreeNode* r = new TreeNode(a[mid]);
    r->left  = build(a, lo, mid - 1);
    r->right = build(a, mid + 1, hi);
    return r;
}
```

O(n) time, O(log n) stack. `lo + (hi-lo)/2` for the same overflow reason as binary search. Verified:
for `1..9` the in-order recovers the array and the height is 4 — the minimum possible.

**LC 109** is this on a linked list, where there is no random access. Two options: copy to an array
first (O(n) extra), or find the middle with slow/fast pointers each time (O(n log n)). The elegant
answer is the in-order simulation — build left, then consume the current list node, then build
right.

---

### 13. Two Sum IV — LC 653 *(trick — the O(h)-space version)*

Three approaches, and the third is the point:

| Approach | Time | Space |
|---|---|---|
| in-order into an array, then two pointers | O(n) | O(n) |
| hash set during any traversal | O(n) | O(n) |
| **two iterators walking inward** | O(n) | **O(h)** |

The third keeps two stacks — one on the left spine (yielding ascending values) and one on the
right spine (descending) — then runs the classic two-pointer step:

```cpp
bool findTarget(TreeNode* root, int k) {
    stack<TreeNode*> lo, hi;
    TreeNode* n = root; while (n) { lo.push(n); n = n->left;  }
    n = root;           while (n) { hi.push(n); n = n->right; }
    while (!lo.empty() && !hi.empty() && lo.top() != hi.top()) {
        int s = lo.top()->val + hi.top()->val;
        if (s == k) return true;
        if (s < k)  { n = lo.top()->right; lo.pop(); while (n) { lo.push(n); n = n->left;  } }
        else        { n = hi.top()->left;  hi.pop(); while (n) { hi.push(n); n = n->right; } }
    }
    return false;
}
```

**`lo.top() != hi.top()` is the guard that stops the same node being used twice** — that is what
makes target 40 fail on a tree whose smallest value is 20, correctly. Verified.

---

### 14. BST to Greater Sum Tree — LC 1038 *(trick — reversed in-order)*

Each node becomes itself plus every value greater than it. In **reverse** in-order (right, node,
left) the values arrive in descending order, so a single running total does the whole job.

```cpp
void gst(TreeNode* r, int& running) {
    if (!r) return;
    gst(r->right, running);          // RIGHT first
    running += r->val;
    r->val = running;
    gst(r->left, running);
}
```

O(n) time, O(h) space. **`../25_heap/bsttoheap.cpp` uses the same reverse-in-order idea** to pull
values out in descending order — noticing that they are the same trick is worth more than either
problem. LC 538 is identical.

---

### 15. Construct BST from Preorder — LC 1008 *(trick — the `bound` parameter)*

Preorder alone determines a BST, because the invariant supplies what the in-order array supplies
for a general tree. Walk the array once, carrying an **upper bound**:

```cpp
TreeNode* buildPre(vector<int>& pre, int& i, int bound) {
    if (i == (int)pre.size() || pre[i] > bound) return NULL;
    TreeNode* r = new TreeNode(pre[i++]);
    r->left  = buildPre(pre, i, r->val);      // left subtree: everything below r
    r->right = buildPre(pre, i, bound);       // right subtree: r's own bound applies
    return r;
}
```

**O(n)** — `i` only moves forward, exactly as in `../21_tree` §3 #23. The naive version rescans for
the split point at each node and is O(n²).

Verified on `{8,5,1,7,10,12}`: in-order comes out sorted and the preorder round-trips exactly.

---

### 16. Flatten to a Linked List — LC 114

Already in `../21_tree` §3 #29. The reverse-pre-order form is worth redoing here because after #14
it is the same shape:

```
prev = NULL
flatten(node):        # visit order right, left, node
    if node NULL: return
    flatten(node->right); flatten(node->left)
    node->right = prev; node->left = NULL; prev = node
```

The other O(1)-space option is Morris-flavoured: for each node with a left child, find that
subtree's rightmost node, attach the current right subtree to it, then move the left subtree over.

---

### 17. BST Iterator — LC 173 *(implementation)*

The stack holds exactly the **left spine** of the not-yet-visited part of the tree.

```cpp
class BSTIterator {
    stack<TreeNode*> st;
    void pushLeft(TreeNode* n) { while (n) { st.push(n); n = n->left; } }
public:
    BSTIterator(TreeNode* root) { pushLeft(root); }
    bool hasNext() { return !st.empty(); }
    int  next() {
        TreeNode* n = st.top(); st.pop();
        pushLeft(n->right);
        return n->val;
    }
};
```

**O(h) space and O(1) amortised per `next()`.** A single `next()` can push h nodes, but each node is
pushed and popped exactly once over the whole iteration — the same amortised argument as the
monotonic stack in `../18_stack`. Flattening into an array first would be O(n) space and is what
the problem is asking you *not* to do.

---

### 18. Balance a BST — LC 1382

In-order into a sorted vector, then run #12 on it. O(n) time, O(n) space. Nothing new — the value
is in seeing that #12 was already the answer.

*(The real-world answer is a self-balancing tree that never gets unbalanced in the first place; see
`advanced_tree_readme.md`.)*

---

### 20. Trim a BST — LC 669

```
trim(node, lo, hi):
    if node NULL: return NULL
    if node->val < lo: return trim(node->right, lo, hi)   # node and its left subtree are gone
    if node->val > hi: return trim(node->left,  lo, hi)
    node->left  = trim(node->left,  lo, hi)
    node->right = trim(node->right, lo, hi)
    return node
```

The two early returns are the same pruning as #9, and the reassignments are #3's pattern. O(n).

---

## Section 3 — Good to Know

### 21. Recover BST — LC 99 *(hard)*

In-order, the values are sorted **except** for the swap. Track `prev` and look at the descents
(places where `prev > cur`):

- **Two descents** → the swapped nodes are non-adjacent. Take the **first descent's `prev`** and
  the **second descent's `cur`**.
- **One descent** → they were adjacent. Take that descent's `prev` and `cur`.

Recording `a` only on the first descent and `b` on every descent handles both cases in one pass:

```cpp
void recoverScan(TreeNode* r, TreeNode*& prev, TreeNode*& a, TreeNode*& b) {
    if (!r) return;
    recoverScan(r->left, prev, a, b);
    if (prev && prev->val > r->val) { if (!a) a = prev; b = r; }
    prev = r;
    recoverScan(r->right, prev, a, b);
}
void recoverTree(TreeNode* root) {
    TreeNode *prev = NULL, *a = NULL, *b = NULL;
    recoverScan(root, prev, a, b);
    if (a && b) swap(a->val, b->val);
}
```

**`if (!a) a = prev;` is the whole trick** — `a` latches on the first descent, `b` keeps
overwriting, so the adjacent case ends with `a` and `b` from the *same* descent. Verified on both:
swapping `30`↔`70` (non-adjacent) and `60`↔`70` (adjacent) both recover.

O(n) time, O(h) space — O(1) with Morris, which is the follow-up.

---

### 22. Largest BST Subtree — LC 333 *(hard — the Info-struct shape)*

You cannot decide "is this a BST" looking downward, so return everything the parent needs:

```cpp
struct Info { bool isBST; int size, mn, mx; };

Info largest(TreeNode* r, int& best) {
    if (!r) { Info i = {true, 0, INT_MAX, INT_MIN}; return i; }   // note the INVERTED sentinels
    Info L = largest(r->left, best), R = largest(r->right, best);
    Info cur;
    if (L.isBST && R.isBST && L.mx < r->val && r->val < R.mn) {
        cur.isBST = true;
        cur.size  = L.size + R.size + 1;
        cur.mn    = min(r->val, L.mn);
        cur.mx    = max(r->val, R.mx);
        best      = max(best, cur.size);
    } else {
        cur.isBST = false;
        cur.size  = max(L.size, R.size);
        cur.mn = INT_MIN; cur.mx = INT_MAX;      // so no ancestor can ever accept this subtree
    }
    return cur;
}
```

**The empty tree returns `mn = INT_MAX, mx = INT_MIN` — deliberately backwards.** That makes
`L.mx < r->val` and `r->val < R.mn` automatically true for a missing child, which is exactly what
you want, and it is why the identity value matters as much as the combine step
(`../21_tree` §5(a)).

Verified: the LC 333 sample gives 3, a genuine 7-node BST gives 7, and the empty tree gives 0.

---

### 23. Morris in-order — *(implementation; `morrisTraversal.cpp` does not run)*

```
cur = root
while cur:
    if cur has no left child:
        VISIT cur; cur = cur->right
    else:
        pre = rightmost node of cur's left subtree, stopping if it already points back to cur
        if pre->right is NULL:  pre->right = cur;  cur = cur->left    # build thread, descend
        else:                   pre->right = NULL; VISIT cur; cur = cur->right   # unthread, visit
```

```cpp
vector<int> morrisInorder(TreeNode* root) {
    vector<int> out;
    TreeNode* cur = root;
    while (cur) {
        if (!cur->left) {
            out.push_back(cur->val);
            cur = cur->right;
        } else {
            TreeNode* pre = cur->left;
            while (pre->right && pre->right != cur) pre = pre->right;
            if (!pre->right) { pre->right = cur;  cur = cur->left; }
            else             { pre->right = NULL; out.push_back(cur->val); cur = cur->right; }
        }
    }
    return out;
}
```

**O(n) time, O(1) space.** Each edge is walked at most three times, so the inner `while` does not
make it quadratic. Verified: output is sorted **and the tree is byte-for-byte restored afterwards**
— the second check is the one that matters, because an implementation that forgets
`pre->right = NULL` leaves a cycle in the tree and every later traversal hangs.

---

### 24. Binary Tree to DLL

Morris's easier cousin. In-order with a `prev` pointer; at each visit rewire `left` as *prev* and
`right` as *next*:

```
prev = NULL, head = NULL
inorder(node):
    recurse left
    if prev is NULL: head = node          # first node visited is the smallest
    else: prev->right = node; node->left = prev
    prev = node
    recurse right
```

O(n) time, O(h) stack. The list comes out sorted for free, because in-order on a BST is sorted.

---

### 25. Serialize / Deserialize a BST — LC 449

**Preorder values, no null markers.** Deserialising is #15 with the `bound` parameter — the
invariant tells you where each subtree ends, so the nulls are redundant.

That is the whole point of the problem: LC 297 on a general binary tree *must* emit nulls (see
`../21_tree` Q7), and a BST does not. The output is roughly half the size.

---

### 26. Merge two BSTs

```
it1 = iterator over BST A, it2 = iterator over BST B
repeatedly take the smaller of it1.peek(), it2.peek()   -> one sorted stream
then run LC 108 on that stream to build a balanced BST
```

O(m + n) time, **O(h1 + h2)** space using #17's iterators — flattening both trees into arrays first
is O(m + n) space and is the version the follow-up rules out. The merge step is the merge from
`../12_sorting`.

---

## Bugs in your files — with evidence

Compiled and executed; none of your `.cpp` files were edited.

### `morrisTraversal.cpp:12` — the loop never runs

```cpp
while(!curr){      // enters only when curr is NULL
```

Should be `while(curr)`. **Measured:** on `50/30,70/20,40,60,80` the function prints **nothing at
all** — the reference in-order is `20 30 40 50 60 70 80`.

### `morrisTraversal.cpp:13` — the branches are inverted

```cpp
if(!curr->left){          // "no left child" -- but the body then does curr->left
    TreeNode* pre=curr->left;
```

The `if` body assumes a left subtree exists and immediately dereferences `pre->right` on line 15,
so even with line 12 fixed this is a null dereference on any leaf. The `!` belongs on the *other*
branch: `if (!cur->left) { visit; go right; } else { thread work }`.

### `morrisTraversal.cpp:17-25` — the thread logic is inside the wrong loop

Both `if(!pre->right)` and `if(pre->right==curr)` sit **inside** `while(pre->right && ...)`, which
only runs while `pre->right` is non-null and not `curr`. `if(!pre->right)` can therefore only be
reached after `pre` has already advanced past its own guard. Those two `if`s belong *after* the
inner `while`, as siblings of each other, not nested inside it. See the corrected version above.

### `morrisTraversal.cpp:3-9` / `iinorderPrecedessor.cpp:3-8` — no constructor

```cpp
class TreeNode { public: int val; TreeNode* left; TreeNode* right; };
```

`left` and `right` are uninitialised. **This is the same class of bug measured in
`../25_heap/bsttoheap.cpp`, where a node built this way had `l = 0x1169d18` and `r = 0x1169e98` —
non-null garbage — and traversing it produced `0xC00000FD` (STACK_OVERFLOW).** Your `Node` classes
in `../21_tree` all set both to `NULL` in the constructor; these two files dropped it.

### `iinorderPrecedessor.cpp:10, 20` — only half the problem

```cpp
int pred(TreeNode* root){ if (!root->left) return -1; ... }
```

**Measured on `50/30,70/20,40,60,80`:** `pred(50)=40` ✓, `succ(50)=60` ✓, `pred(70)=60` ✓ — the
child cases are all right. But `succ(20)` returns **−1** when the true in-order successor is **30**,
and `pred(60)` returns **−1** when the true predecessor is **50**. The ancestor case is missing;
§1 #8 above has the version that covers both.

Also `pred(NULL)` dereferences a null pointer — the guard checks `root->left` before checking
`root`.

---

## What you got right

**Predecessor and successor via the child are exactly correct** — `rightmost of left` and
`leftmost of right`, with the right loop conditions, verified on four cases. That is the half of
the problem most people get wrong; what is missing is a case, not a correction.

**You picked Morris.** It is a genuinely hard algorithm and nothing forced you to attempt it — the
folder would have been complete with recursive in-order. The three pieces are all *present* in your
code (find the rightmost predecessor, build the thread, remove it and visit); they are assembled
wrongly, which is a very different failure from not knowing the algorithm.

**`readme.md`'s eight problems are well chosen.** 700 → 701 → 450 is precisely the right order for
the primitives, and 98 next is correct because delete is what makes you care whether the result is
still a BST. Adding 1038 is unusual and good — it is the one problem that forces *reverse*
in-order, and you will meet the same trick again in `../25_heap/bsttoheap.cpp`.

**You kept the BST files separate from `../21_tree`.** Given that a BST *is* a binary tree, folding
them together would have been the easy choice and would have buried the invariant. The folder split
matches how the material is actually taught.
