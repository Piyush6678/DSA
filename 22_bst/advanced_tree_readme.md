# Advanced Trees — the ones hard interviews reach for

A companion to `readme.md`. Nothing here is needed to solve the problems in `questions.md` — it is
the layer above, and it is where senior interviews go once the plain-BST answers land.

Read it in three passes:

| Pass | What to take | Why |
|---|---|---|
| **Now** | §1 (why balance), §2 (AVL rotations), §4 (Trie) | asked directly, and a Trie is a plain interview problem |
| **After `../26_dp`** | §5 (segment tree), §6 (Fenwick) | range queries, and both are contest staples |
| **Later** | §3 (B-trees), §7–§10 | you need these to *discuss*, rarely to code |

Everything with C++ in it has been compiled and run.

---

## 1. Why a plain BST is not enough

Insert `1, 2, 3, 4, 5, 6, 7` in that order and you get a right-skewed chain of height 7. Every
operation is O(n), and you have paid for pointers to obtain an array's worst case.

**Sorted input is not an adversarial edge case — it is the common case.** Loading records by
timestamp, by ID, by name: all sorted. So a BST that does not rebalance is not usable as a general
container, and *every* production ordered map is a balanced variant.

The fix, in every scheme, is the same primitive: a **rotation**.

```
      y                                x
     / \       rotateRight(y)         / \
    x   C     ------------------>    A   y
   / \        <------------------       / \
  A   B         rotateLeft(x)          B   C
```

`A < x < B < y < C` before and after. **A rotation changes the height but never the in-order
sequence** — that is the entire reason it is legal. Everything in §2 and §3 is a policy for
*when* to rotate.

---

## 2. Self-balancing BSTs

### AVL — strict balance

**Invariant:** for every node, `|height(left) − height(right)| ≤ 1`.

Each node stores its height. After an insert, walk back up; the first node that violates the
invariant gets fixed by one of four cases:

| Case | Shape | Fix |
|---|---|---|
| **LL** | left child, inserted into *its* left | one `rotateRight` |
| **RR** | right child, inserted into *its* right | one `rotateLeft` |
| **LR** | left child, inserted into *its* right | `rotateLeft(left)` then `rotateRight` |
| **RL** | right child, inserted into *its* left | `rotateRight(right)` then `rotateLeft` |

```cpp
struct AvlNode {
    int val, ht; AvlNode *left, *right;
    AvlNode(int v) : val(v), ht(1), left(NULL), right(NULL) {}
};
int  H(AvlNode* n)         { return n ? n->ht : 0; }
int  balanceOf(AvlNode* n) { return n ? H(n->left) - H(n->right) : 0; }
void fix(AvlNode* n)       { n->ht = 1 + max(H(n->left), H(n->right)); }

AvlNode* rotateRight(AvlNode* y) {
    AvlNode* x = y->left;
    y->left  = x->right;
    x->right = y;
    fix(y); fix(x);            // ORDER MATTERS: the lower node first
    return x;                  // new subtree root -- caller must store it
}
AvlNode* rotateLeft(AvlNode* x) {
    AvlNode* y = x->right;
    x->right = y->left;
    y->left  = x;
    fix(x); fix(y);
    return y;
}
AvlNode* avlInsert(AvlNode* n, int v) {
    if (!n) return new AvlNode(v);
    if (v < n->val)      n->left  = avlInsert(n->left,  v);
    else if (v > n->val) n->right = avlInsert(n->right, v);
    else return n;                                   // no duplicates
    fix(n);
    int b = balanceOf(n);
    if (b >  1 && v < n->left->val)  return rotateRight(n);                            // LL
    if (b < -1 && v > n->right->val) return rotateLeft(n);                             // RR
    if (b >  1 && v > n->left->val)  { n->left  = rotateLeft(n->left);   return rotateRight(n); } // LR
    if (b < -1 && v < n->right->val) { n->right = rotateRight(n->right); return rotateLeft(n);  } // RL
    return n;
}
```

**`fix(y)` before `fix(x)` in the rotation.** `x` becomes the parent, so its height depends on the
already-updated `y`. Reversing the two lines gives heights that are silently one too small, and the
tree slowly unbalances without ever failing an assertion.

**Verified:** inserting `1..7` in sorted order — the case that destroys a plain BST — gives height
**3**, root **4**, in-order still `1 2 3 4 5 6 7`, and every node balanced. All four rotation cases
produce root `20` from their respective three-node inputs.

Note the shape is `../22_bst` §1 #3's reassignment pattern with two extra lines. **An AVL insert is
a BST insert plus `fix` plus four `if`s** — that framing is what makes it writable in an interview.

### Red-black — looser balance, fewer rotations

**Invariants:** every node is red or black; the root and all null leaves are black; a red node has
no red child; **every root-to-null path contains the same number of black nodes**.

Those give `height ≤ 2·log₂(n+1)` — looser than AVL, but restored with **at most 2 rotations on
insert and 3 on delete**, versus AVL's O(log n) rotations on delete.

| | AVL | Red-black |
|---|---|---|
| Height bound | ~1.44 log n | ~2 log n |
| Lookups | **faster** (shorter tree) | slower |
| Insert/delete | more rotations | **fewer rotations** |
| Used by | read-heavy indexes | `std::map`, `std::set`, Java `TreeMap`, Linux CFS |

**Why `std::map` chose red-black:** mixed workloads. AVL wins when you read far more than you
write. Nobody hand-writes red-black insert in an interview; know the invariants and the trade-off.

This is the direct answer to "why does `std::map` give O(log n) *guaranteed* while a hand-rolled
BST does not" — a question `../24_maps` will make concrete.

### Treap and Splay — the two you should be able to name

- **Treap** = BST on keys, **heap on random priorities**. Each node gets a random priority; rotate
  to keep the heap property. The randomness makes the expected height O(log n) with no balance
  bookkeeping at all, and the code is dramatically shorter than red-black. Ties `../25_heap` to
  this folder neatly.
- **Splay tree** — after every access, rotate the touched node to the root. No balance invariant
  whatsoever; the bound is **amortised** O(log n). Recently-used keys sit near the root, so it is
  self-optimising for skewed access patterns. The same amortised-analysis style as the monotonic
  stack in `../18_stack`.

---

## 3. B-trees and B+ trees — when the tree does not fit in RAM

A binary node holds one key. Reading it from disk costs one page fetch — **and a page is 4–16 KB,
so you fetched thousands of bytes to learn one comparison.**

A **B-tree** node holds hundreds of keys and hundreds of children, sized to fill exactly one page.
Height for a million records drops from ~20 (binary) to **~3**. Every database index and every
filesystem is built on this.

**B+ tree** — the variant actually used: *all values live in the leaves*, internal nodes hold only
keys for routing, and **the leaves are chained in a linked list**. That chain is why
`SELECT ... WHERE id BETWEEN 100 AND 200` is fast: descend once, then walk the leaf list.

**The interview point is not the algorithm, it is the reason.** Balanced-binary vs B-tree is a
question about the *memory hierarchy*, not about comparisons. If asked "why don't databases use
red-black trees", the answer is node fan-out versus page size.

---

## 4. Tries — trees keyed by prefix

Not a BST. Each edge is a character; each path from the root is a prefix.

```cpp
struct TrieNode {
    TrieNode* next[26];
    bool isEnd;
    TrieNode() { isEnd = false; for (int i = 0; i < 26; ++i) next[i] = NULL; }
};
class Trie {
    TrieNode* root;
    TrieNode* walk(const string& w) {
        TrieNode* cur = root;
        for (size_t i = 0; i < w.size(); ++i) {
            int c = w[i] - 'a';
            if (!cur->next[c]) return NULL;
            cur = cur->next[c];
        }
        return cur;
    }
public:
    Trie() { root = new TrieNode(); }
    void insert(const string& w) {
        TrieNode* cur = root;
        for (size_t i = 0; i < w.size(); ++i) {
            int c = w[i] - 'a';
            if (!cur->next[c]) cur->next[c] = new TrieNode();
            cur = cur->next[c];
        }
        cur->isEnd = true;                 // "apple" inserted does NOT make "app" a word
    }
    bool search(const string& w)     { TrieNode* n = walk(w); return n && n->isEnd; }
    bool startsWith(const string& p) { return walk(p) != NULL; }
};
```

**`isEnd` is the whole design.** Without it you cannot distinguish a stored word from a prefix of
one. Verified: after inserting only `"apple"`, `search("app")` is **false** while
`startsWith("app")` is **true**.

**O(L) per operation** where L is the word length — independent of how many words are stored. That
is the property a hash map cannot match for prefix queries.

Problems that are Tries in disguise: **LC 208** (implement Trie), **LC 211** (add and search with
`.` wildcards — DFS over children at a `.`), **LC 212** (word search II — a Trie *plus* grid DFS,
the classic hard combination), **LC 648**, **LC 720**. And the **binary Trie** over the 32 bits of
an integer is how **LC 421 maximum XOR of two numbers** goes from O(n²) to O(32n) — that one ties
straight back to `../15_bitwise`.

**Compressed trie / radix tree** — collapse chains of single-child nodes into one edge holding a
whole substring. Same queries, far less memory; this is what IP routing tables use.

**Suffix tree / suffix array** — insert every suffix of a string. Substring search becomes a
prefix walk. Suffix arrays give most of the power with a fraction of the memory and are the
practical choice.

---

## 5. Segment trees — range queries with updates

The problem: answer `sum(l, r)` **and** `set(i, v)`, many of each.

| Structure | Query | Update |
|---|---|---|
| plain array | O(n) | O(1) |
| prefix sums (`../13_prefixSum`) | **O(1)** | O(n) — rebuild |
| **segment tree** | **O(log n)** | **O(log n)** |

Prefix sums die the moment updates appear. A segment tree is the answer, and this is exactly the
motivation to give.

Each node owns a range and stores the aggregate over it. Root owns `[0, n-1]`; children split it.

```cpp
class SegTree {
    vector<long long> t; int n;
    void build(vector<int>& a, int node, int lo, int hi) {
        if (lo == hi) { t[node] = a[lo]; return; }
        int mid = lo + (hi - lo) / 2;
        build(a, 2*node+1, lo, mid);
        build(a, 2*node+2, mid+1, hi);
        t[node] = t[2*node+1] + t[2*node+2];
    }
    long long query(int node, int lo, int hi, int l, int r) {
        if (r < lo || hi < l)   return 0;          // no overlap    -> identity
        if (l <= lo && hi <= r) return t[node];    // total overlap -> stored answer
        int mid = lo + (hi - lo) / 2;              // partial       -> both children
        return query(2*node+1, lo, mid, l, r) + query(2*node+2, mid+1, hi, l, r);
    }
    void update(int node, int lo, int hi, int i, int v) {
        if (lo == hi) { t[node] = v; return; }
        int mid = lo + (hi - lo) / 2;
        if (i <= mid) update(2*node+1, lo, mid, i, v);
        else          update(2*node+2, mid+1, hi, i, v);
        t[node] = t[2*node+1] + t[2*node+2];
    }
public:
    SegTree(vector<int>& a) { n = a.size(); t.assign(4*n, 0); build(a, 0, 0, n-1); }
    long long query(int l, int r) { return query(0, 0, n-1, l, r); }
    void update(int i, int v)     { update(0, 0, n-1, i, v); }
};
```

Three things to remember:

- **`4*n` size.** The tree is not perfect, and `2*n` is not enough. `4*n` always is.
- **The three-case query** — no overlap / total overlap / partial — is the whole algorithm, and it
  is the same case split for min, max, gcd, or any associative operation. Swap `+` for `min` and
  change the no-overlap identity from `0` to `INT_MAX`, and nothing else moves.
- **Lazy propagation** extends this to *range* updates ("add 5 to everything in `[l,r]`") by
  storing a pending value at a node and pushing it down only when that subtree is next visited.
  That is the standard follow-up.

**Verified** on `{1,3,5,7,9,11}`: `sum(0,5)=36`, `sum(1,3)=15`, and after `update(1,10)` the same
queries give `43` and `22`.

---

## 6. Fenwick tree (Binary Indexed Tree)

Same prefix-sum-with-updates problem, **half the code and a quarter of the memory** — at the cost
of only supporting invertible operations (sums yes, min no).

```cpp
class BIT {
    vector<long long> t; int n;
public:
    BIT(int n_) : t(n_ + 1, 0), n(n_) {}
    void add(int i, int delta) {                  // 1-INDEXED
        for (; i <= n; i += i & (-i)) t[i] += delta;
    }
    long long prefix(int i) {
        long long s = 0;
        for (; i > 0; i -= i & (-i)) s += t[i];
        return s;
    }
    long long range(int l, int r) { return prefix(r) - prefix(l - 1); }
};
```

**`i & (-i)` is the lowest set bit** — straight out of `../15_bitwise`. Index `i` stores the sum of
the `lowbit(i)` elements ending at `i`, so `prefix` walks up by clearing low bits and `add` walks
down by adding them. Both loops run at most `log n` times because each step changes one bit.

**It must be 1-indexed** — `lowbit(0) == 0`, so index 0 would loop forever.

**Verified** against the same `{1,3,5,7,9,11}`: `prefix(6)=36`, `range(2,4)=15`, and after
`add(2, +7)` they become `43` and `22` — matching the segment tree exactly.

Classic uses: **counting inversions** (`../12_sorting`'s merge-sort answer, done differently), and
**LC 315 count of smaller numbers after self**.

---

## 7. Augmenting a BST — the order-statistic tree

The general technique: **store one extra field per node that can be maintained in O(1) from the
children.** Subtree size is the useful one.

```cpp
struct OSNode {
    int val, cnt;                 // cnt = number of nodes in this subtree
    OSNode *left, *right;
    OSNode(int v) : val(v), cnt(1), left(NULL), right(NULL) {}
};
int SZ(OSNode* n) { return n ? n->cnt : 0; }

int osKth(OSNode* n, int k) {                     // k is 1-indexed
    while (n) {
        int leftSize = SZ(n->left);
        if (k == leftSize + 1) return n->val;     // exactly this node
        if (k <= leftSize)     n = n->left;
        else { k -= leftSize + 1; n = n->right; } // discount everything to the left, and n itself
    }
    return -1;
}
```

This turns `questions.md` §1 #6 from **O(h + k)** into **O(h)**, and it is the expected follow-up
to that problem. `k -= leftSize + 1` is where people slip — the `+1` is the current node.

Verified on `50/30,70/20,40,60,80`: 1st = 20, 4th = 50, 7th = 80, 8th = −1.

The same idea gives **rank of a value** (how many are smaller), which is what an
"order-statistic tree" means in the literature. Other augmentations worth knowing: max-in-subtree
(→ interval trees), and count-of-a-value (→ multiset).

---

## 8. Trees over other domains

| Structure | Keyed on | Answers |
|---|---|---|
| **Interval tree** | interval start, augmented with subtree max end | "which stored intervals overlap `[a,b]`?" |
| **k-d tree** | cycles through k dimensions, one per level | nearest neighbour in k-d space |
| **Quadtree / Octree** | recursive spatial subdivision | collision detection, image compression, LOD |
| **Cartesian tree** | BST on index, heap on value | built in O(n) from an array; range-minimum queries |
| **Merkle tree** | hashes of children | "did any of these blocks change?" — git, blockchains, rsync |
| **B+ tree** | keys, leaves chained | database and filesystem indexes (§3) |
| **Trie / radix** | string prefixes | autocomplete, IP routing (§4) |

**Merkle trees are worth 60 seconds of thought** because they are the one on this list you have
already used — git commits are a Merkle DAG, and that is why `git status` can detect a change
without reading every file.

---

## 9. LCA, done properly

`../21_tree` §2 #20 solves LCA in O(n) per query. With **q** queries that is O(nq), and the
follow-up is always "now do a million queries".

| Technique | Preprocess | Per query |
|---|---|---|
| naive recursion | — | O(n) |
| **binary lifting** | O(n log n) | **O(log n)** |
| Euler tour + sparse table (RMQ) | O(n log n) | **O(1)** |
| Tarjan's offline LCA (DSU) | O(n α(n)) | amortised O(α(n)) |

**Binary lifting** is the one to learn. Store `up[v][j]` = the 2ʲ-th ancestor of `v`, computed by
`up[v][j] = up[ up[v][j-1] ][j-1]`. To find the LCA: lift the deeper node to the same depth, then
lift both together by descending powers of two while their ancestors differ. It is the same
"jump by powers of two" idea as binary search.

**Tarjan's offline version uses DSU** — which is `../28_DSU`. Worth returning to after that folder.

---

## 10. Decompositions — named things to recognise

Not to implement now; to *recognise* when someone says them.

- **Heavy-light decomposition** — split the tree into chains so any root-to-node path crosses
  O(log n) chains, then put a segment tree on each chain. Turns "sum/max on the path from u to v,
  with updates" into O(log² n).
- **Centroid decomposition** — recursively remove the centroid (the node whose removal leaves no
  component bigger than n/2). Gives an O(log n)-deep recursion over *paths*, which is how
  "count paths of length k" problems get solved.
- **Euler tour / flattening** — write the tree out as an array by entry and exit time. Every
  subtree becomes a **contiguous range**, so subtree queries become range queries and §5 applies
  directly. This is the single most useful idea in the section and the easiest to implement.
- **Small-to-large merging** — when merging sets up a tree, always merge the smaller into the
  larger. Total cost O(n log n), for the same reason as union by size in `../28_DSU`.

---

## Interview Q&A — the hard layer

**Q1. Why does `std::map` guarantee O(log n) when a hand-written BST does not?**
It is a red-black tree, which rebalances on every insert and delete and keeps height ≤ 2·log₂(n+1).
A plain BST has no such invariant; sorted insertions give height n. If you need guaranteed
ordered-map behaviour, do not hand-roll it.

**Q2. AVL or red-black — which and why?**
AVL is more strictly balanced, so lookups are faster, but rebalancing after deletion can cost
O(log n) rotations. Red-black rebalances in at most 2–3 rotations. **Read-heavy → AVL;
write-heavy or mixed → red-black.** That is why libraries default to red-black.

**Q3. Why do databases use B+ trees rather than balanced binary trees?**
Because the cost is disk pages, not comparisons. A B+ tree node fills one page and has a fan-out
in the hundreds, so a million-row index is 3 levels deep instead of 20. Leaves are chained, so
range scans are one descent plus a linked-list walk.

**Q4. Prefix search over ten million words — what structure?**
A trie, or a compressed/radix trie for memory. O(L) per query independent of the dictionary size,
and the prefix walk *is* the query. A hash map gives O(1) exact lookup but cannot enumerate by
prefix at all.

**Q5. Range sum with updates — array, prefix sums, or something else?**
Prefix sums are O(1) query but O(n) per update, so they lose as soon as writes exist. Segment tree
or Fenwick: both O(log n) for each. **Fenwick if the operation is invertible (sums), segment tree
if it is not (min, max, gcd) or if you need lazy range updates.**

**Q6. What does augmenting a tree mean?**
Storing extra data per node that is computable in O(1) from its children, and maintaining it on
every rotation and update. Subtree size gives O(h) kth-smallest and rank; subtree max gives
interval trees. The constraint is the O(1)-from-children part — anything else cannot survive a
rotation.

**Q7. How would you answer a million LCA queries on a fixed tree?**
Preprocess. Binary lifting: O(n log n) to build `up[v][j]`, O(log n) per query. If O(1) per query
is required, Euler tour plus a sparse table for range minimum.

**Q8. What is a splay tree good for?**
Access locality. It has no balance invariant — every accessed node is rotated to the root, giving
amortised O(log n) with the recently-used keys near the top. Good for caches and skewed access;
bad when you need a worst-case guarantee for a single operation.

**Q9. Give a tree you have used without realising it.**
Git. Commits form a Merkle DAG; trees and blobs are addressed by the hash of their contents, so a
changed file changes every hash on the path to the root — which is how git detects change without
comparing file contents.

**Q10. A trie for lowercase words wastes memory. Fix it.**
Three options, in increasing order of effort: replace `next[26]` with a hash map or a sorted small
vector (sparse nodes); compress single-child chains into one edge (radix tree); or, if the set is
static, use a **DAWG** — merge identical suffix subtrees so the structure becomes a DAG.

---

## What to actually learn, in order

1. **Trie** — appears as a plain interview problem (LC 208, 211, 212, 421). Write it today.
2. **AVL rotations** — the four cases, and why a rotation preserves in-order. Asked directly.
3. **Segment tree** — the three-case query. The most reusable structure here.
4. **Fenwick** — twenty lines, and it makes `../15_bitwise` pay off.
5. **Red-black invariants and the B+ tree argument** — to *discuss*, not to code.
6. Everything else — recognise the name and say what problem it solves.
