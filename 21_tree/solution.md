# 21 — Binary Trees: Solutions

Approach, pseudocode, complexity and key insight. Real C++ appears for the **traversals**
(fundamental — the code *is* the lesson, and the iterative forms cannot be conveyed in prose), the
**one-pass** measures, the **trick** problems (LCA, Morris, path-sum-III), and the **hard**
construction problems.

**Everything shown as code was compiled with `g++ -std=gnu++14` and executed — 67 assertions, all
passing.** Measured outputs are quoted.

Assume `struct TreeNode { int val; TreeNode *left, *right; };`.

---

# Section 1 — Must Do

## 1. Build a tree from a level-order array — **`[impl]`**

**Approach.** BFS with a queue: pop a node, take the next two array slots as its children, and
enqueue only the non-null ones.

```
root = new Node(a[0]);  q = [root];  i = 1
while q not empty and i < n:
    cur = q.pop()
    if i < n:  if a[i] != -1: cur->left  = new Node(a[i]); q.push(left);   i++
    if i < n:  if a[i] != -1: cur->right = new Node(a[i]); q.push(right);  i++
```

**Key insight.** Only **non-null** nodes are enqueued, so only non-null nodes consume child slots.
That is the LeetCode convention, and it is what makes `{1,2,3,4,5,-1,6,...}` unambiguous — the `-1`
at index 5 means node 3 has no left child, and no slots are reserved for that missing child's
children.

**Your `constructATree` at `bianryTree.cpp:38` is correct.** Verified against a structural
fingerprint (pre-order with explicit nulls), not just level order:

```
built:    1,2,4,#,#,5,7,#,#,8,#,#,3,#,6,9,#,#,#,
expected: 1,2,4,#,#,5,7,#,#,8,#,#,3,#,6,9,#,#,#,
```

One nit: `if (j != n && arr[j] != -1)` should be `j < n`. It happens to be safe here because `i`
and `j` always differ by one, but `<` states the intent and cannot be defeated by a stride change.

**Complexity.** O(n) time, O(n) space.

---

## 2–4. The three depth-first traversals — **fundamental, code given**

### Recursive — one line moves

```cpp
void inorder(TreeNode* r, vector<int>& out) {
    if (!r) return;                    // base case FIRST
    inorder(r->left, out);
    out.push_back(r->val);             // <-- move this line for pre / post
    inorder(r->right, out);
}
```

Pre-order puts the `push_back` **before** both calls, post-order **after** both. Nothing else
changes. This is `../10_Recursion` §3's before/after-the-call distinction on a branching structure.

**Complexity.** O(n) time, **O(h) stack** — O(log n) balanced, O(n) degenerate.

**Verified on the perfect 7-node tree:** pre `1,2,4,5,3,6,7`; in `4,2,5,1,6,3,7`;
post `4,5,2,6,7,3,1`; and all three return empty for a `NULL` root.

### Iterative pre-order

```cpp
vector<int> preIter(TreeNode* root) {
    vector<int> out; if (!root) return out;
    stack<TreeNode*> st; st.push(root);
    while (!st.empty()) {
        TreeNode* n = st.top(); st.pop();
        out.push_back(n->val);
        if (n->right) st.push(n->right);     // RIGHT first...
        if (n->left)  st.push(n->left);      // ...so LEFT pops first
    }
    return out;
}
```

**Key insight.** Push right before left. A stack reverses, so the last pushed is visited first.

**Your `peroderIterative` at `bianryTree.cpp:74` is correct**, including the `if(root)` guard.

### Iterative in-order — the different shape

```cpp
vector<int> inIter(TreeNode* root) {
    vector<int> out; stack<TreeNode*> st; TreeNode* cur = root;
    while (cur || !st.empty()) {
        while (cur) { st.push(cur); cur = cur->left; }   // dive left, remembering the path
        cur = st.top(); st.pop();
        out.push_back(cur->val);
        cur = cur->right;                                 // then handle the right subtree
    }
    return out;
}
```

**Key insight.** You cannot visit a node until its entire left subtree is done, so the stack holds
the *path* down to the leftmost node rather than a set of pending nodes. The loop condition needs
**both** `cur` and the stack — `cur` non-null means there is more to descend, a non-empty stack
means there is more to come back to.

**Your `inOrderIterative`'s traversal logic is correct**; only the printing loop is wrong — see the
bugs section.

### Iterative post-order — the trick

```cpp
vector<int> postIter(TreeNode* root) {
    vector<int> out; if (!root) return out;
    stack<TreeNode*> st; st.push(root);
    while (!st.empty()) {
        TreeNode* n = st.top(); st.pop();
        out.push_back(n->val);
        if (n->left)  st.push(n->left);      // LEFT first -> visits root, right, left
        if (n->right) st.push(n->right);
    }
    reverse(out.begin(), out.end());          // -> left, right, root
    return out;
}
```

**Key insight.** Run pre-order with the children swapped, then reverse. `root,right,left` reversed
is `left,right,root`, which is post-order. Genuine one-stack post-order needs a "last visited"
pointer and is much fiddlier; this is the version to write under pressure.

**Your `postIterative` is exactly this and is correct** — it even uses the same collect-then-reverse
structure.

**Verified:** all three iterative versions match their recursive counterparts on the perfect tree,
on a `NULL` root, and on a right-skewed tree.

---

## 5. Level Order Traversal — LeetCode 102 — **fundamental, code given**

```cpp
vector<vector<int> > levelOrder(TreeNode* root) {
    vector<vector<int> > res; if (!root) return res;          // guard the NULL root
    queue<TreeNode*> q; q.push(root);
    while (!q.empty()) {
        int sz = (int)q.size();                                // FREEZE the level boundary
        vector<int> level;
        for (int i = 0; i < sz; ++i) {
            TreeNode* n = q.front(); q.pop();
            level.push_back(n->val);
            if (n->left)  q.push(n->left);
            if (n->right) q.push(n->right);
        }
        res.push_back(level);
    }
    return res;
}
```

**Key insight — `int sz = q.size();` before the inner loop.** Capturing the size *first* means the
inner loop processes exactly the current level, while pushes for the next level accumulate behind
it. Without it you get a flat BFS with no level boundaries, and five problems in Section 2 depend
on those boundaries.

**Guard the root.** `q.push(root)` on a `NULL` root then dereferences it immediately. **Your
`bfslOrderQueue` (in all three files) omits this — verified, `bfslOrderQueue(NULL)` gives an
ACCESS_VIOLATION.**

**Complexity.** O(n) time, O(w) space where `w` is the maximum level width — up to n/2 for a
perfect tree.

**Verified:** `[1][2,3][4,5,6,7]` on the perfect tree; `[1][2,3][4,5,6][7,8,9]` on the tree built
from your array; empty for a `NULL` root.

---

## 6. Maximum Depth — LeetCode 104 — **fundamental**

```cpp
int height(TreeNode* r) {
    if (!r) return 0;                                  // <-- 0, not a sentinel
    return 1 + max(height(r->left), height(r->right));
}
```

**Key insight — the base case must be the *identity* for the operation, and for height that is
0.** An empty tree has height 0; a leaf is `1 + max(0,0) = 1`. Returning `INT16_MIN` for `NULL`
makes a leaf `1 + (-32768) = -32767` and poisons every value above it — which is exactly what
happens in your version.

**Complexity.** O(n) time, O(h) stack.

**Verified:** height 3 on the perfect tree, 1 on a leaf, 0 on `NULL`.

---

## 7–9. Structural recursion — Invert (226), Same Tree (100), Symmetric (101)

```
invert(r):     if !r: return NULL
               swap(r->left, r->right), recursing into both
               return r

isSame(p,q):   if both NULL: true
               if one NULL:  false                       # order matters: this check comes second
               return p->val==q->val and isSame(p->left,q->left) and isSame(p->right,q->right)

isSymmetric(r): return !r or mirror(r->left, r->right)
mirror(a,b):    if both NULL: true
                if one NULL:  false
                return a->val==b->val and mirror(a->left, b->right)     # CROSSED
                                       and mirror(a->right, b->left)
```

**Key insight — #9 is #8 with the arguments crossed.** `isSame` compares left-with-left; `mirror`
compares left-with-right. One character of difference, two problems. Noticing that is worth more
than writing the second from scratch.

**The two null checks must be in that order.** `if (!p && !q) return true;` before
`if (!p || !q) return false;` — reversed, two empty trees report as different.

**Complexity.** O(n) time, O(h) stack.

**Verified:** `invertTree` on `{4,2,7,1,3,6,9}` gives pre-order `4,7,9,6,2,3,1`; `isSameTree` true
for identical trees, false for a single differing leaf, true for two `NULL`s, false for one `NULL`;
`isSymmetric` true on `{1,2,2,3,4,4,3}` and false on `{1,2,2,-1,3,-1,3}`.

---

## 10. Diameter — LeetCode 543 — **the one-pass trick, code given**

```cpp
int helper(TreeNode* r, int& best) {
    if (!r) return 0;
    int L = helper(r->left,  best);
    int R = helper(r->right, best);
    best = max(best, L + R);            // the path THROUGH r, in edges
    return 1 + max(L, R);               // the height r reports to its parent
}
int diameterOfBinaryTree(TreeNode* r) { int best = 0; helper(r, best); return best; }
```

**Key insight — return one thing, record another.** The function *returns* the height (which the
parent needs) while *updating* `best` by reference (which is the answer). That separation is what
makes it one pass.

The naive version calls a separate `height()` at every node, which is **O(n²)** — and on a
degenerate tree that is 10⁵ nodes squared. This is the single most reused shape in the folder:
#11, #25 and most tree DP are the same skeleton.

`L + R` counts **edges**. For the node-count convention it is `L + R + 1`; know which the question
wants.

**Complexity.** O(n) time, O(h) stack.

**Verified:** 4 on the perfect 7-node tree, 0 on a single node, 3 on `{1,2,3,4,5}`, 2 on a
right-skewed 3-node tree.

---

## 11. Balanced Binary Tree — LeetCode 110 — **one pass, sentinel return**

```cpp
int balHelper(TreeNode* r) {
    if (!r) return 0;
    int L = balHelper(r->left);  if (L == -1) return -1;    // propagate failure immediately
    int R = balHelper(r->right); if (R == -1) return -1;
    if (abs(L - R) > 1) return -1;
    return 1 + max(L, R);
}
bool isBalanced(TreeNode* r) { return balHelper(r) != -1; }
```

**Key insight — overload the return value.** The function returns a height, *or* `-1` meaning
"already unbalanced somewhere below". Since a real height is never negative, `-1` is an
unambiguous sentinel, and checking it right after each recursive call prunes the rest of the work.

**Complexity.** O(n) time, O(h) stack — versus O(n²) for the recompute-height version.

**Verified:** true on the perfect tree and on `NULL`, false on a right-skewed tree, true on the
LeetCode example `{3,9,20,null,null,15,7}`.

---

## 12. Size, sum, maximum

```
size(r) = r ? 1 + size(left) + size(right) : 0
sum(r)  = r ? val + sum(left) + sum(right) : 0
max(r)  = r ? max(val, max(max(left), max(right))) : INT_MIN
```

**Key insight — the `NULL` return is the identity of the operation**: 0 for count and sum,
`INT_MIN` for maximum, `true` for "all satisfy". Use **`INT_MIN`, not `INT16_MIN`** — the latter
is −32768, and any tree containing a value below that reports the sentinel as its maximum.

**Your `sumOfTree` and `sizeOfTree` are correct** — verified, 28 and 7 on the 7-node tree.
`max_value_node` is correct for values above −32768 and wrong below it.

Use `long long` for the sum if values can be large; `n` values near `INT_MAX` overflow an `int`.

---

# Section 2 — Important

## 13–15. Root-to-leaf paths — the top-down family

**#13 Binary Tree Paths (257)** — carry the path down as a value:

```
paths(r, cur):
    if !r: return
    cur += (cur empty ? "" : "->") + r->val
    if leaf: record cur; return
    paths(r->left, cur);  paths(r->right, cur)
```

Passing `cur` **by value** gives each branch its own copy, so no undo is needed. Simple, and O(h)
copying per node.

**#14 Path Sum (112)** — subtract on the way down:

```
hasPathSum(r, target):
    if !r: return false
    if leaf: return target == r->val
    return hasPathSum(left, target - r->val) or hasPathSum(right, target - r->val)
```

**Key insight — the leaf test is `!left && !right`, not `!r`.** Returning `target == 0` at a
`NULL` would accept a path that ends in mid-air, and a node with one child would count as a leaf on
its null side. Verified: `hasPathSum(NULL, 0)` must be **false**.

**#15 Path Sum II (113)** — the same, but collecting, so the buffer is shared and must be undone:

```cpp
void ps2(TreeNode* r, int target, vector<int>& cur, vector<vector<int> >& out) {
    if (!r) return;
    cur.push_back(r->val);
    if (!r->left && !r->right && target == r->val) out.push_back(cur);
    else { ps2(r->left, target - r->val, cur, out); ps2(r->right, target - r->val, cur, out); }
    cur.pop_back();                          // BACKTRACK -- restore for the sibling branch
}
```

**Key insight.** `cur` is by reference so it is not copied at every node; the price is that you
must `pop_back()` before returning. Exactly the discipline from `../10_Recursion` §4 —
do → recurse → undo.

**Complexity.** O(n·h) worst case for the output, O(h) stack.

**Verified:** `binaryTreePaths` on `{1,2,3,null,5}` gives `1->2->5` and `1->3`; `hasPathSum` true
for 22 and false for 100 on the LeetCode tree; `pathSumII` returns `[5,4,11,2]` and `[5,8,4,5]`.

---

## 16. Minimum Depth — LeetCode 111 — **the trap**

```
minDepth(r):
    if !r: return 0
    if !r->left:  return 1 + minDepth(r->right)      # only one real side
    if !r->right: return 1 + minDepth(r->left)
    return 1 + min(minDepth(left), minDepth(right))
```

**Key insight — it is NOT the mirror of maximum depth.** `1 + min(L, R)` is wrong, because a
`NULL` child returns 0 and would report a depth of 1 for a node that is not a leaf. Minimum depth
means *shortest root-to-leaf* path, and a node with one child is not a leaf. The two explicit
one-child cases are the fix.

BFS is arguably the better answer here — the first leaf you dequeue is at the minimum depth, so you
can stop early rather than exploring the whole tree.

---

## 17–19. Level-order variations — all of them are #5 plus one line

| Problem | Change to #5 |
|---|---|
| **199 Right Side View** | record the node when `i == sz - 1` |
| **103 Zigzag** | reverse `level` on alternate rows (or push front/back) |
| **107 Bottom-up** | build normally, then `reverse(res)` at the end |

**Key insight.** The level-order skeleton is reusable, and grouping these makes that obvious. For
#199, note the answer is the **last node of each level**, which is not always a right child — a
left child on a level with no right sibling still counts. Verified: `{1,2,null,3}` gives
`1,2,3`.

**Complexity.** O(n) time, O(w) space for all three.

---

## 20. Lowest Common Ancestor — LeetCode 236 — **trick, code given**

```cpp
TreeNode* lowestCommonAncestor(TreeNode* r, TreeNode* p, TreeNode* q) {
    if (!r || r == p || r == q) return r;
    TreeNode* L = lowestCommonAncestor(r->left,  p, q);
    TreeNode* R = lowestCommonAncestor(r->right, p, q);
    if (L && R) return r;              // found on OPPOSITE sides -> r is the split point
    return L ? L : R;                  // otherwise propagate whichever was found
}
```

**Key insight — four lines, and the whole thing turns on `if (L && R)`.** Each call returns
"something interesting found in this subtree, or `NULL`". If both children report something, `p`
and `q` are on opposite sides and the current node is the meeting point. If only one does, the
answer is somewhere in that subtree — pass it up unchanged.

**The `r == p || r == q` base case handles the ancestor case for free.** If `p` is an ancestor of
`q`, the recursion stops at `p` and returns it, which is correct — verified,
`LCA(5, 4)` where 4 is a descendant of 5 returns **5**.

Note this returns a node even if only one of `p`/`q` is present; LeetCode guarantees both exist. If
that is not guaranteed, verify afterwards.

**Complexity.** O(n) time, O(h) stack. On a **BST** the ordering gives O(h) directly — see
`../22_bst`.

**Verified** on the standard LeetCode tree: `LCA(5,1)=3`, `LCA(5,4)=5` (ancestor case),
`LCA(6,4)=5`.

---

## 21. Boundary Traversal

**Approach.** Three parts, in order, with no node counted twice:

```
1. root                       (unless the root is itself a leaf)
2. left boundary, top-down    -- excluding leaves
3. all leaves, left to right
4. right boundary, bottom-up  -- excluding leaves
```

```
leftBoundary(node):                      # ITERATIVE, and it must not descend into leaves
    while node:
        if node has a child: emit node->val
        node = node->left ? node->left : node->right
```

**Key insight — every node must appear exactly once, and the three parts must not overlap.** Both
boundary walks **skip leaves**, because the leaf pass already covers them. The right boundary is
collected then emitted in reverse.

Following `left ? left : right` means a boundary continues down the only available child, which is
what makes skewed trees work — and it is where the recursive version in `bianryTree.cpp` produces
duplicates. See the bugs section.

**Complexity.** O(n) time, O(h) space.

**Verified:** perfect tree → `1,2,4,5,6,7,3`; single node → `1`; right-skewed `1→2→3` → `1,3,2`
(no duplicate); left-skewed → `1,2,3`.

---

## 22. Subtree of Another Tree — LeetCode 572

At every node of the big tree, call #8's `isSameTree`. O(n·m) and perfectly acceptable. The O(n+m)
answer serialises both trees with explicit null markers and runs a substring search — mention it as
the follow-up.

---

# Section 3 — Good to Know

## 23. Construct from Preorder and Inorder — LeetCode 105 — **hard, code given**

```cpp
TreeNode* build(vector<int>& pre, int& pi, unordered_map<int,int>& pos, int lo, int hi) {
    if (lo > hi) return NULL;
    int rootVal = pre[pi++];                       // pre-order gives the root, IN ORDER
    TreeNode* root = new TreeNode(rootVal);
    int mid = pos[rootVal];                        // where it splits the inorder range
    root->left  = build(pre, pi, pos, lo, mid - 1);   // LEFT first -- pre-order demands it
    root->right = build(pre, pi, pos, mid + 1, hi);
    return root;
}
```

**Key insight — three things.**

1. **Pre-order hands you roots in exactly the order you need them.** A single shared index `pi`
   (passed by reference) walks forward and never rewinds.
2. **In-order tells you the split.** Everything left of the root's in-order position is the left
   subtree, everything right is the right subtree.
3. **Recurse left before right.** The shared `pi` means order is load-bearing — swapping the two
   lines silently builds a different tree.

The hash map from value to in-order index turns an O(n) linear search per node into O(1), taking
the whole algorithm from O(n²) to **O(n)**. It requires distinct values, which LeetCode guarantees.

**#27 (inorder + postorder)** is the same with post-order read **backwards** and `right` built
before `left`.

**Complexity.** O(n) time, O(n) space.

**Verified:** `pre={3,9,20,15,7}, in={9,3,15,20,7}` gives the structure
`3,9,#,#,20,15,#,#,7,#,#`; a full round trip on the 7-node tree (take its own pre-order and
in-order, rebuild, compare fingerprints) matches exactly; single node works.

---

## 24. Path Sum III — LeetCode 437 — **trick, code given**

```cpp
int ps3(TreeNode* r, long long running, int target, unordered_map<long long,int>& seen) {
    if (!r) return 0;
    running += r->val;
    int cnt = seen.count(running - target) ? seen[running - target] : 0;   // paths ending HERE
    seen[running]++;
    cnt += ps3(r->left,  running, target, seen);
    cnt += ps3(r->right, running, target, seen);
    seen[running]--;                          // UNDO -- this prefix belongs to THIS branch only
    return cnt;
}
int pathSumIII(TreeNode* r, int target) {
    unordered_map<long long,int> seen; seen[0] = 1;     // the empty prefix
    return ps3(r, 0, target, seen);
}
```

**Key insight — this is `../13_prefixSum` §6 on a tree.** `sum(a..b) = running(b) - running(a-1)`,
so a downward path summing to `target` exists whenever some ancestor's running sum equals
`running - target`. The map counts those ancestors.

**The `seen[running]--` on the way back up is the whole difference from the array version.** A
prefix on the left branch is *not* a prefix for anything on the right branch — the map must
describe only the current root-to-node path. Forget the undo and you count paths that do not
exist.

`seen[0] = 1` seeds the empty prefix, exactly as in LeetCode 560, so paths starting at the root
are counted.

**`long long` for the running sum** — values can be ±10⁹ and a path can be 1000 nodes long.

**Complexity.** O(n) time, O(h) space for the map plus O(h) stack. The naive
"start a fresh sum at every node" is O(n²).

**Verified:** the LeetCode tree with target 8 → **3**; `{1,-2,-3}` with target −1 → 1 (negatives);
`{0,1,1}` with target 1 → 4 (zeros, where the same node participates in several counts).

---

## 25. Binary Tree Maximum Path Sum — LeetCode 124 — **hard, code given**

```cpp
int helper(TreeNode* r, int& best) {
    if (!r) return 0;
    int L = max(0, helper(r->left,  best));    // a negative branch is worth NOTHING -- clamp to 0
    int R = max(0, helper(r->right, best));
    best = max(best, r->val + L + R);          // path BENDING at r: both sides usable
    return r->val + max(L, R);                 // path CONTINUING upward: one side only
}
int maxPathSum(TreeNode* r) { int best = INT_MIN; helper(r, best); return best; }
```

**Key insight — the returned value and the recorded value are different, and that is the problem.**
A path that *bends* at `r` can use both subtrees; a path that continues up through `r` to its
parent can only use one. Returning `val + max(L,R)` while recording `val + L + R` captures both.

**`max(0, ...)` is the second half.** If a subtree's best contribution is negative, do not take it
— zero is always available by stopping at `r`.

**`best` starts at `INT_MIN`, not 0.** An all-negative tree has a negative answer; starting at 0
would wrongly return 0.

**Complexity.** O(n) time, O(h) stack.

**Verified:** `{1,2,3}` → 6; the LeetCode example `{-10,9,20,null,null,15,7}` → 42;
single node `{-3}` → −3 (all-negative case); `{2,-1}` → 2 (the negative child is clamped away).

---

## 26. Serialize and Deserialize — LeetCode 297

**Approach.** Pre-order with an explicit marker for null.

```
serialize(r):    if !r: return "#,"
                 return val + "," + serialize(left) + serialize(right)

deserialize:     read tokens in order; "#" -> NULL; otherwise make a node and
                 recursively fill left then right (same order as serialisation)
```

**Key insight — nulls are what make one traversal sufficient.** Pre-order alone is ambiguous;
pre-order *with nulls* is not, because every node's arity becomes explicit. This is the concrete
answer to "which single traversal determines a tree?".

The two functions must agree on order exactly — serialise left-then-right, deserialise
left-then-right.

**Complexity.** O(n) both ways.

---

## 28. Morris Inorder Traversal — **trick, code given, O(1) space**

```cpp
vector<int> morrisInorder(TreeNode* root) {
    vector<int> out; TreeNode* cur = root;
    while (cur) {
        if (!cur->left) { out.push_back(cur->val); cur = cur->right; }
        else {
            TreeNode* pred = cur->left;                              // in-order predecessor
            while (pred->right && pred->right != cur) pred = pred->right;
            if (!pred->right) { pred->right = cur; cur = cur->left; }        // create the thread
            else { pred->right = NULL; out.push_back(cur->val); cur = cur->right; }  // remove it
        }
    }
    return out;
}
```

**Key insight — borrow the null right pointers as temporary parent links.** Every node with a left
subtree has an in-order predecessor whose `right` is null; point it at the current node so you can
find your way back without a stack. On the second visit, the thread is already there — that is how
you know the left subtree is finished, so you remove it and emit.

**`pred->right != cur` in the inner loop is what stops it looping** on a thread you already made.

**The tree is restored.** Every thread is removed on the second visit, so the structure is
unchanged afterwards — verified by running a normal recursive in-order after Morris and getting the
same result.

**Complexity.** O(n) time (each edge is traversed at most three times), **O(1) space**.

**Verified:** `4,2,5,1,6,3,7` on the perfect tree, matching the recursive version; empty for
`NULL`; **and the tree is intact afterwards**.

---

## 29–34 — approach only

- **114 Flatten to a linked list** — reverse post-order (right, left, root) with a `prev` pointer:
  set `r->right = prev; r->left = NULL; prev = r;`. O(n)/O(h). The Morris-style version is O(1)
  space.
- **987 Vertical Order** — BFS carrying `(row, col)`; `col-1` going left, `col+1` going right.
  Group by column in a map, and sort within a cell by value for ties. The **sorting rule for ties
  is the whole difficulty**, not the traversal.
- **863 All Nodes Distance K** — build a `child → parent` map with one traversal, then BFS outward
  from the target treating the tree as an **undirected graph**, with a visited set. This reframing
  is the direct bridge to `../27_graphs`, and #34 (burning tree) is the same technique asking for
  the maximum distance instead of a specific one.
- **222 Count Complete Tree Nodes** — exploit completeness: compare the leftmost and rightmost
  depths; if equal, the subtree is perfect and holds `2^h - 1` nodes with no traversal. Otherwise
  recurse. **O(log²n)** instead of O(n).
- **662 Maximum Width** — index nodes as in a heap (`2i`, `2i+1`) and take
  `lastIndex - firstIndex + 1` per level. Use `unsigned long long` or re-base each level's indices
  at 0, or a deep tree overflows.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `21_tree/` was edited.**

All four files compile. Three of them (`bianryTree.cpp`, `creating_a_tree.cpp`,
`implementTree.cpp`) **produce no output at all** — their `main` builds a tree and stops without
calling any traversal. `nodeTree.cpp` prints `28`, `7`, `7`, all correct.

## `nodeTree.cpp:42` and `implementTree.cpp:46` — `levelTree` returns nonsense

```cpp
int levelTree(Node *root ){
    if(root==NULL)return INT16_MIN ;              // -32768
    int levels = 1 + max(levelTree(root->left), levelTree(root->right));
    return levels;
}
```

An empty subtree must contribute **0**, not a large negative number. With `INT16_MIN`, a leaf
computes `1 + (-32768) = -32767`, and the error propagates up unchanged.

**Verified on the 7-node perfect tree:**

| Call | Your result | Correct |
|---|---|---|
| `levelTree(root)` | **−32765** | 3 |
| `levelTree(leaf)` | **−32767** | 1 |
| `levelTree(NULL)` | **−32768** | 0 |

**Fix:** `if (root == NULL) return 0;`.

## `implementTree.cpp:62` — `bfs()` prints nothing, for two separate reasons

```cpp
void bfs(Node* root){
    for(int i =1;i<=levelTree(root);i++){       // bound is NEGATIVE -> loop never runs
        display_level(root->left,1,i);          // and this skips the root AND the right subtree
        cout<<endl;
    }
}
```

1. **The loop bound is `levelTree(root)`**, which is about −32765, so the loop body never executes.
   **Verified: `bfs()` on the 7-node tree prints nothing at all.**
2. **Even with the height fixed, it is wrong.** It passes `root->left`, so the root is never
   printed and the **entire right subtree is unreachable** — `display_level` only descends from the
   node it is given. It should be `display_level(root, 1, i)`.

`bfsR2L` has both faults identically.

Both are also O(n·h) — they re-walk the tree once per level. The queue version (#5) is O(n) and is
already in the same file at `:79`.

## `bianryTree.cpp:121` — `inOrderIterative` prints an out-of-bounds element

```cpp
for(int i =0-1;i<ans.size();i++){     // starts at -1
    cout<<ans[i]<<"->";
}
```

`ans[-1]` reads before the start of the vector's buffer.

**Verified: prints `134231769->4->2->5->1->6->3->7->`.** The leading number is garbage from the
out-of-bounds read; everything after it (`4 2 5 1 6 3 7`) is the correct in-order traversal.

**The traversal logic above it is correct** — only the printing loop is wrong. Fix: `int i = 0`.

GCC also warns on this line: `comparison between signed and unsigned integer expressions`. Compare
against `(int)ans.size()`.

## `bianryTree.cpp:150` — `boundary()` duplicates nodes on skewed trees, and crashes on NULL

```cpp
void boundary(Node* root){
    leftBoundary(root);
    bottomBoundary(root);
    rightBoundary(root->right);      // <-- dereferences root without checking
}
```

**Two faults.**

1. **`root->right` on a `NULL` root** is an immediate crash.
2. **Nodes are printed twice on a skewed tree.** `leftBoundary` falls back to
   `leftBoundary(root->right)` when there is no left child — so on a right-skewed tree it walks
   down the right spine. Then `rightBoundary(root->right)` walks the same spine again.

**Verified on the right-skewed tree `1 → 2 → 3`:**

```
got:  1->2->3->2->        (2 appears twice)
want: 1->2->3
```

**It is correct on the perfect 7-node tree** — verified, `1->2->4->5->6->7->3->`, which is exactly
right — and correct on a single node. Only the skewed cases fail, which is why it looked fine.

The fix is structural (#21): walk each boundary **iteratively** with
`node = node->left ? node->left : node->right`, skip leaves in both boundary passes, and collect
the right boundary before emitting it reversed.

## `bfslOrderQueue` — no `NULL` guard, in all three files

```cpp
queue<Node*>q;
q.push(root);                        // root may be NULL
while(q.size()>0){
    Node* temp= q.front();
    cout<<temp->val<<"->";           // dereferences NULL on the first iteration
```

**Verified: ACCESS_VIOLATION (`0xC0000005`) on `bfslOrderQueue(NULL)`.**

**Fix:** `if (!root) return;` before the push.

## `implementTree.cpp:95` — `constructATree` is an empty stub

```cpp
void constructATree (int arr[],int n ){
    queue<Node*>tree;
}
```

Declares a queue and returns. The working version is in `bianryTree.cpp` and
`creating_a_tree.cpp` — this one appears to be an abandoned first attempt.

## `nodeTree.cpp:34` — `max_value_node` uses the wrong sentinel

`INT16_MIN` is −32768. Any tree whose values are all below that reports the sentinel as its
maximum.

**Verified:** on a tree containing −40000 and −50000, it returns **−32768** — a value that is not
in the tree at all. Correct answer: −40000.

**Fix:** `INT_MIN` from `<climits>`. It is correct for the 1–7 test tree, which is why it passed.

## Smaller notes

- **`bianryTree.cpp:55`** — `if (j != n && arr[j] != -1)` should be `j < n`. Safe as written
  because `j = i + 1` always, but `<` states the intent.
- **Memory** — every `new Node` in all four files leaks. Harmless in a scratch file; mention it in
  an interview.
- **`creating_a_tree.cpp` and `bianryTree.cpp` contain the same `constructATree` and
  `bfslOrderQueue`.** Duplicated code drifts; `creating_a_tree.cpp` looks like the earlier copy.

---

## What you got right

- **`constructATree` is correct** — verified against a structural fingerprint, not just level
  order. Building a tree from a level-order array with `-1` for null is genuinely fiddly, and the
  key decision — **enqueue only non-null children, so nulls consume no child slots** — is exactly
  the LeetCode convention. Getting that right without a reference is not trivial.

- **`peroderIterative` (`:74`) is correct**, including pushing **right before left** so that left
  pops first, and the `if(root)` guard that the BFS function is missing.

- **`postIterative` (`:86`) is correct**, and it uses the good trick: traverse root-right-left into
  a vector, then print it in reverse. That is the version worth knowing, and you also correctly
  push **left before right** to get the mirrored order.

- **`inOrderIterative`'s traversal is correct** (`:104-120`) — the `while (st.size() || temp)`
  condition with the dive-left inner branch is exactly the right shape, and it is the hardest of
  the three iterative traversals. Only the printing loop after it is wrong.

- **`sumOfTree` and `sizeOfTree` are correct** — verified, 28 and 7. Both use the right identity
  (`0`) for the `NULL` case, which is precisely what `levelTree` and `max_value_node` get wrong in
  the same file. You had the pattern; two functions drifted from it.

- **`display_level` (`implementTree.cpp:39`) is correct in isolation** — it prints exactly the
  nodes at a given depth, and the default arguments (`level=0, n=3`) are a reasonable convenience.
  Only its *callers* pass the wrong root.

- **`display_level_R2L` visiting right before left** is the right way to get a level printed in
  reverse without reversing anything afterwards.

- **All three DFS traversals in `implementTree.cpp:18-35` are correct**, and having them adjacent
  and differing by one line each is the clearest possible presentation of what pre/in/post actually
  means. That file is the best demonstration in the folder.

- **The `Node` constructor sets both `left` and `right` to `NULL`** in all four files. That is what
  makes every `if (root->left)` check meaningful, and it is the single most common omission in a
  first tree implementation.
