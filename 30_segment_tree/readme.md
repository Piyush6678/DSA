# 30 — Segment Trees

A segment tree is a highly versatile data structure that allows answering range queries over an array, while still permitting elements to be modified, both in logarithmic time. It is the definitive answer to "I need to query a range, but the array is changing," making it an indispensable tool for competitive programming and advanced algorithmic problem-solving. 

What makes segment trees uniquely powerful is not just their speed, but their flexibility: by changing a single `merge` function, a segment tree can be adapted to compute range sums, minimums, maximums, greatest common divisors (GCD), XORs, or even more complex associative properties over any segment of an array.

---

## 1. Motivation — Why Segment Trees Exist

Consider the fundamental problem: you are given an array of $n$ elements and you need to perform two types of operations:
1. **Range Query**: Find some aggregate (sum, min, max, XOR, GCD) over a subarray `[l, r]`.
2. **Point Update**: Update a single element at index `i`.

If we rely on a naive array, updates are fast ($O(1)$), but computing the sum over a range requires iterating over all elements in the worst case, making queries $O(n)$. 

If we optimize for queries using a **Prefix Sum Array** (see `../13_prefixSum`), any range sum can be found in $O(1)$ using `pref[r] - pref[l-1]`. However, updating a single element at index 0 requires recomputing the entire prefix sum array, making updates $O(n)$. 

We need a structure that balances both operations. A **Segment Tree** achieves $O(\log n)$ for *both* queries and updates, giving us the perfect middle ground.

### Comparison of Approaches

| Data Structure | Point Update | Range Query | Best Used For |
|---|---|---|---|
| **Naive Array** | $O(1)$ | $O(n)$ | When there are almost no queries, only updates. |
| **Prefix Sum** | $O(n)$ | $O(1)$ | Static array, no updates. (Sum/XOR only). |
| **Sparse Table** | Not supported | $O(1)$ | Static array, idempotent operations (min, max, GCD). |
| **BIT/Fenwick** | $O(\log n)$ | $O(\log n)$ | Point update + prefix query, simpler code. (Invertible ops like sum, XOR). |
| **Segment Tree** | $O(\log n)$ | $O(\log n)$ | The most general structure. Handles updates and non-invertible queries (min/max). |

---

## 2. The Structure — A Complete Binary Tree Stored Flat

A segment tree is fundamentally a binary tree where each node represents an interval (or "segment") of the array.
- The **root** represents the entire array `[0, n-1]`.
- The **leaves** represent single elements `[i, i]`.
- An **internal node** representing `[l, r]` will have two children representing `[l, mid]` and `[mid+1, r]`, where `mid = (l + r) / 2`.
- Each node stores the precomputed answer (e.g., sum) for its interval. 

### Why a flat 1-indexed array of size 4*n?
We do not build a tree of pointers. Instead, we use a flat array, exactly like a binary heap (see `../25_heap`), usually 1-indexed:
- For any node at index `i`, its **left child** is at `2 * i`.
- Its **right child** is at `2 * i + 1`.
- Its **parent** is at `i / 2`.

Why `4 * n` space? A segment tree is a full binary tree. If $n$ is a power of 2, the tree has exactly $2n - 1$ nodes. But if $n$ is *not* a power of 2, the tree is not perfectly balanced at the bottom. In the worst case, the structure mimics a full binary tree of size $2^{\lceil \log_2 n \rceil + 1}$, which requires at most $4n$ nodes to ensure every leaf and internal node has a valid index. Allocating `4 * n` guarantees we never go out of bounds.

### The `merge` Function
The true beauty of the segment tree lies in the relationship between a node and its children. The value of a node is always the combination of its left and right children.
`tree[i] = merge(tree[2*i], tree[2*i+1])`
By simply changing the `merge` logic from `+` to `min()`, `max()`, `^` (XOR), or `std::gcd`, the entire tree changes its behavior.

---

## 3. Build — O(n)

Building the tree involves recursively dividing the array segments until we reach a leaf (a single element), setting its value, and then merging results as we backtrack up the tree.

### C++ Code for Building a Sum Segment Tree

```cpp
// g++ -std=gnu++14
#include <vector>

using namespace std;

const int MAXN = 100005;
int tree[4 * MAXN];
int arr[MAXN];

// node is the current index in the tree array.
// [start, end] is the array range that this node is responsible for.
void build(int node, int start, int end) {
    if (start == end) {
        // Base case: Leaf node. It represents a single element.
        tree[node] = arr[start];
    } else {
        int mid = start + (end - start) / 2;
        // Recurse on the left child
        build(2 * node, start, mid);
        // Recurse on the right child
        build(2 * node + 1, mid + 1, end);
        // Merge step
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
}
```

### Why is Build O(n) and not O(n log n)?
At a glance, we visit levels of height $O(\log n)$, leading some to guess $O(n \log n)$. However, we visit each node in the tree exactly once. The total number of nodes in a segment tree is strictly less than $4n$. The recurrence relation is $T(n) = 2T(n/2) + O(1)$, which by the Master Theorem resolves to exactly $O(n)$. It is equivalent to a post-order traversal of a binary tree of size $4n$.

---

## 4. Point Update — O(log n)

If we change an element in the original array, we don't need to rebuild the whole tree. A single element `arr[i]` only belongs to one leaf node. That leaf node contributes to its parent, which contributes to its parent, all the way to the root.

We can update the tree by walking from the root to the target leaf, updating the leaf, and then re-merging values as the recursion unwinds.

### C++ Code for Point Update

```cpp
// Updates arr[idx] to 'val'
void update(int node, int start, int end, int idx, int val) {
    if (start == end) {
        // Leaf node reached
        arr[idx] = val;
        tree[node] = val;
    } else {
        int mid = start + (end - start) / 2;
        if (start <= idx && idx <= mid) {
            // idx is in the left child
            update(2 * node, start, mid, idx, val);
        } else {
            // idx is in the right child
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        // Re-merge after the child is updated
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
}
```

### Why is Update O(log n)?
The recursion strictly follows a single path from the root down to one specific leaf. Since the height of the segment tree is bounded by $\lceil \log_2 n \rceil + 1$, we visit at most $O(\log n)$ nodes.

---

## 5. Range Query — O(log n)

To answer a query for the range `[l, r]`, we start at the root and descend. At any node representing the range `[start, end]`, there are exactly three possibilities:

1. **Completely Outside**: The node's range `[start, end]` does not overlap with the query range `[l, r]`. We return a "null" value (e.g., 0 for sum, $\infty$ for min) that doesn't affect the merge.
2. **Completely Inside**: The node's range `[start, end]` is fully contained within `[l, r]`. We immediately return `tree[node]`. **This is where the magic happens**—we skip visiting all descendants, achieving logarithmic time.
3. **Partial Overlap**: The node's range partially overlaps with `[l, r]`. We recursively query both children and merge their results.

### C++ Code for Range Sum Query

```cpp
// Returns the sum in range [l, r]
int query(int node, int start, int end, int l, int r) {
    // 1. Completely outside
    if (r < start || end < l) {
        return 0; // The identity for sum is 0
    }
    
    // 2. Completely inside
    if (l <= start && end <= r) {
        return tree[node];
    }
    
    // 3. Partial overlap
    int mid = start + (end - start) / 2;
    int p1 = query(2 * node, start, mid, l, r);
    int p2 = query(2 * node + 1, mid + 1, end, l, r);
    
    return p1 + p2;
}
```

### Why is Query O(log n)?
It isn't immediately obvious why partial overlaps don't branch out and ruin the complexity. The invariant is this: **At any level of the segment tree, we visit at most 4 nodes.** If a range covers a large middle section of the array, the higher-level nodes will be "completely inside" and terminate instantly without branching. Only the boundary nodes (at the far left and right of the query range) will experience partial overlaps and continue branching downward. Thus, the work is strictly bounded by the height of the tree, giving $O(\log n)$.

---

## 6. Lazy Propagation — Range Updates in O(log n)

What if we need to update an entire range, e.g., add $v$ to all elements in `[l, r]`? 
If we do this via point updates, we do $O(r - l + 1) \times O(\log n)$ work. In the worst case, this is $O(n \log n)$ per update, completely defeating the purpose.

**The Solution: Lazy Propagation**
Instead of updating every single leaf in the range immediately, we find the largest nodes that fit entirely inside `[l, r]` (just like a range query), update them, and **leave a note** (a "lazy" tag) saying: *"Hey, my children also need to be updated by $v$, but I'll only tell them if someone actually needs to visit them."*

We introduce a `lazy[]` array. Before we do anything at a node (update or query), we first **push down** any pending lazy updates to its children.

### C++ Code: Range Update + Range Query with Lazy Propagation

```cpp
int tree[4 * MAXN];
int lazy[4 * MAXN]; // Initialize with 0

void pushDown(int node, int start, int end) {
    if (lazy[node] != 0) {
        // Apply the pending update to this node
        // For a sum tree, adding 'v' to all elements in a range of length L adds L * v to the sum
        tree[node] += (end - start + 1) * lazy[node];
        
        // If it's not a leaf, push the tag to children
        if (start != end) {
            lazy[2 * node] += lazy[node];
            lazy[2 * node + 1] += lazy[node];
        }
        
        // Clear the tag for the current node
        lazy[node] = 0;
    }
}

void rangeUpdate(int node, int start, int end, int l, int r, int val) {
    pushDown(node, start, end); // Always resolve pending updates first
    
    // Completely outside
    if (r < start || end < l) return;
    
    // Completely inside
    if (l <= start && end <= r) {
        lazy[node] += val; // Leave a lazy tag
        pushDown(node, start, end); // Apply it immediately
        return;
    }
    
    // Partial overlap
    int mid = start + (end - start) / 2;
    rangeUpdate(2 * node, start, mid, l, r, val);
    rangeUpdate(2 * node + 1, mid + 1, end, l, r, val);
    
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

int rangeQueryLazy(int node, int start, int end, int l, int r) {
    pushDown(node, start, end); // Must resolve pending updates before querying
    
    if (r < start || end < l) return 0;
    
    if (l <= start && end <= r) return tree[node];
    
    int mid = start + (end - start) / 2;
    int p1 = rangeQueryLazy(2 * node, start, mid, l, r);
    int p2 = rangeQueryLazy(2 * node + 1, mid + 1, end, l, r);
    
    return p1 + p2;
}
```

### When is lazy needed vs not?
If you only have point updates, lazy propagation is useless overhead. If you have range updates, you **must** use lazy propagation to maintain $O(\log n)$ performance.

**Common Bugs**:
- Forgetting to call `pushDown` at the very beginning of both `rangeUpdate` and `rangeQueryLazy`.
- Applying the lazy value incorrectly to `tree[node]`. For a sum tree, adding $v$ to $k$ elements means the sum increases by $k \times v$. For a min tree, the min just increases by $v$. The application logic changes based on the operation!

---

## 7. Variants

Because the architecture separates the tree logic from the `merge` logic, you can swap out operations effortlessly.

1. **Range Min/Max Query**: 
   - `merge`: `min(p1, p2)` or `max(p1, p2)`.
   - `lazy application`: `tree[node] += lazy[node]`.
2. **Range XOR Query**:
   - `merge`: `p1 ^ p2`.
   - `lazy application`: Adding a value isn't easily compatible with XOR sums, but XORing a range with $v$ works. If the range length is odd, the XOR sum is XORed with $v$. If even, it remains unchanged ($v \oplus v = 0$).
3. **Range GCD Query**:
   - `merge`: `std::gcd(p1, p2)`.
4. **Range Assignment (Set all in [l,r] to v)**:
   - Instead of `lazy[node] += val`, we do `lazy[node] = val`.
   - `pushDown` replaces the children's tags instead of adding to them.
   - Note: you need a special "empty" value for the lazy array to distinguish between "set to 0" and "no pending update".
5. **Count of elements in range satisfying a condition**:
   - Commonly known as a Merge Sort Tree. This is an advanced variant where each node stores a sorted vector of the elements in its range, allowing for binary search at each node.

---

## 8. Segment Tree vs Other Structures

| | Build | Point Update | Range Query | Range Update | Space | When to use |
|---|---|---|---|---|---|---|
| **Prefix Sum** | $O(n)$ | $O(n)$ rebuild | $O(1)$ | $O(n)$ | $O(n)$ | Static array, sum only |
| **Sparse Table** | $O(n \log n)$ | Not supported | $O(1)$ | Not supported | $O(n \log n)$ | Static, idempotent ops (min/max/GCD) |
| **BIT/Fenwick** | $O(n)$ | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ with trick | $O(n)$ | Point update + prefix query, simpler code |
| **Segment Tree** | $O(n)$ | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ with lazy | $O(4n)$ | Most general, handles everything |

**When to use a Binary Indexed Tree (BIT/Fenwick)?**
If you only need to compute prefix sums (or prefix XORs) and perform point updates, BIT is strictly superior. It uses exactly $O(n)$ space, has microscopic constant factors, and is 10 lines of code.

**When MUST you use a Segment Tree?**
1. When you need **range updates** (lazy propagation).
2. When your operation is **non-invertible**. BIT requires an inverse operation to compute a range (e.g., `sum[l,r] = sum[0,r] - sum[0,l-1]`). You cannot "subtract" a minimum, so BIT cannot do range minimum queries efficiently. Segment Trees naturally handle min/max.

---

## 9. Persistent Segment Trees (Advanced)

Imagine a scenario where, after making $M$ updates, you need to answer a query on what the array looked like at version $K$ ($K < M$). Storing $M$ copies of the tree requires $O(M \times N)$ memory, which will OOM.

A **Persistent Segment Tree** solves this. Since every point update in a segment tree modifies exactly $O(\log n)$ nodes, we can simply create new copies of *only those nodes*, while retaining pointers to the old, unmodified nodes. Every update creates a new root node for the new version, requiring only $O(\log n)$ extra space per update. 

This is firmly in advanced competitive programming territory, often used to solve the classic "find the $k$-th smallest element in a range `[l, r]`" online. (For those curious, see President's Tree).

---

## 10. Coordinate Compression with Segment Trees

Sometimes you have an array where the values or indices are massive (e.g., up to $10^9$), but there are only a few of them (e.g., $10^5$ operations). You cannot allocate `tree[4 * 10^9]`. 

Instead, you use **Coordinate Compression**. You gather all the unique indices that will ever be queried or updated, sort them, and map them to $0, 1, 2, \dots, K$. The segment tree is then built over this compressed range of size $K$, fitting easily into memory.

---

## 11. Interview Q&A

**Q1: When would you use a segment tree vs a BIT (Fenwick Tree)?**
A BIT is simpler to implement, takes exactly $O(n)$ space, and has a smaller constant factor, making it faster in practice for point updates and prefix sums. However, BIT requires invertible operations (like sum or XOR) to answer range queries. If the operation is non-invertible (like min/max), or if you need range updates, a Segment Tree is necessary. 

**Q2: Why is the tree array size 4*n?**
A segment tree is a full binary tree. If $n$ is a power of 2, the tree has $2n - 1$ nodes, fitting in $2n$ space. If $n$ is not a power of 2, the tree's depth is determined by the next highest power of 2. In the absolute worst case, the size of the array required to hold all nodes without out-of-bounds indexing is bounded by $4n$.

**Q3: Explain lazy propagation and when it's needed.**
Lazy propagation is used to handle range updates (updating all elements in an interval `[l, r]`) in $O(\log n)$ time. Instead of updating every leaf node immediately ($O(n)$), we update the highest covering nodes and leave a "lazy" tag on them. This tag defers the update to the children until those children are actually accessed by a future query or update. It is strictly needed when range updates are present.

**Q4: What's the time complexity of build and why isn't it O(n log n)?**
The time complexity is $O(n)$. Although the tree has height $O(\log n)$, the `build` function visits every node in the tree exactly once. Since there are at most $4n$ nodes, the total work is proportional to $n$. Mathematically, the recurrence $T(n) = 2T(n/2) + O(1)$ resolves to $O(n)$.

**Q5: Can a segment tree handle non-commutative operations?**
Yes. Since the segment tree strictly maintains the order of intervals (left child is strictly before right child), the `merge` function just needs to be non-commutative aware. For instance, matrix multiplication is associative but non-commutative, and a segment tree handles it perfectly. BITs, however, struggle with this.

**Q6: How would you handle range assignment vs range addition?**
For range addition, the lazy tag accumulates: `lazy[node] += val`. For range assignment (set all elements to $v$), the lazy tag overrides: `lazy[node] = val`. Additionally, you must designate a special flag value for the lazy array (e.g., `-1` or a boolean `hasPendingUpdate`) to differentiate between a pending assignment of `0` and "no pending update".

**Q7: What is a persistent segment tree and when would you use it?**
A persistent segment tree maintains the history of all previous versions of the tree after updates. Since a point update only modifies a single path of $O(\log n)$ nodes from root to leaf, a persistent tree creates new nodes just for that path and points to the unchanged children from previous versions. It is used for complex online queries, such as retrieving historical states or solving the "k-th smallest element in range [l,r]" problem.
