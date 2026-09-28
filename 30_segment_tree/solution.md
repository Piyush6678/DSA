# 30 — Segment Trees: Solutions

Approach, pseudocode, complexity and key insight. Real C++ appears only for the **fundamentals** (implementations), the **tricks**, and the **hard** ones.

**Everything shown as code was compiled with `g++ -std=gnu++14` and executed.**

Assume standard segment tree class structure where `vector<int> tree;` and `int n;` exist.

---

# Section 1 — Must Do

## 1. Implement a segment tree (sum) — build, update, query — **fundamental, code given**

**Approach.** Build recursively, point update, range sum query. Use a 1-indexed array of size 4*n.

```cpp
class SegTree {
    vector<int> tree;
    int n;
public:
    SegTree(vector<int>& arr) {
        n = arr.size();
        tree.assign(4 * n, 0);
        if (n > 0) build(arr, 1, 0, n - 1);
    }

    void build(vector<int>& arr, int node, int start, int end) {
        if (start == end) {
            tree[node] = arr[start];
            return;
        }
        int mid = start + (end - start) / 2;
        build(arr, 2 * node, start, mid);
        build(arr, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    void update(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = start + (end - start) / 2;
        if (start <= idx && idx <= mid) {
            update(2 * node, start, mid, idx, val);
        } else {
            update(2 * node + 1, mid + 1, end, idx, val);
        }
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0; // No overlap
        if (l <= start && end <= r) return tree[node]; // Total overlap
        // Partial overlap
        int mid = start + (end - start) / 2;
        int left_sum = query(2 * node, start, mid, l, r);
        int right_sum = query(2 * node + 1, mid + 1, end, l, r);
        return left_sum + right_sum;
    }
};
```

**Key insight.** The three-case logic in query (total overlap, no overlap, partial overlap) is the entire algorithm. It dictates when to return early and when to traverse further down.

**Complexity.** Build O(n), Update O(log n), Query O(log n). O(n) space.

**Verified:** Initialized with `{1, 3, 5, 7, 9, 11}`, query(1, 3) gives `15`. Update index 1 to `10`, query(1, 3) gives `22`.

---

## 2. Implement a segment tree (min/max)

**Approach.** Identical to the sum segment tree, but change the merge function.
```
build: tree[node] = min(tree[2*node], tree[2*node+1])
update: tree[node] = min(tree[2*node], tree[2*node+1])
query:
    if no overlap: return INT_MAX
    if total overlap: return tree[node]
    return min(left_query, right_query)
```

**Key insight.** The ONLY change from #1 is the merge function: `min()` instead of `+`. This highlights that segment trees can be abstracted over any associative operation.

**Complexity.** Build O(n), Update O(log n), Query O(log n).

---

## 3. Range Sum Query - Mutable — LeetCode 307 — **fundamental, code given**

**Approach.** Use the segment tree implementation to wrap the required operations into a class.

```cpp
class NumArray {
    vector<int> tree;
    int n;

    void build(vector<int>& nums, int node, int start, int end) {
        if (start == end) {
            tree[node] = nums[start];
            return;
        }
        int mid = start + (end - start) / 2;
        build(nums, 2 * node, start, mid);
        build(nums, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    void updateTree(int node, int start, int end, int idx, int val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = start + (end - start) / 2;
        if (idx <= mid) updateTree(2 * node, start, mid, idx, val);
        else updateTree(2 * node + 1, mid + 1, end, idx, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    int queryTree(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return tree[node];
        int mid = start + (end - start) / 2;
        return queryTree(2 * node, start, mid, l, r) + queryTree(2 * node + 1, mid + 1, end, l, r);
    }

public:
    NumArray(vector<int>& nums) {
        n = nums.size();
        if (n > 0) {
            tree.assign(4 * n, 0);
            build(nums, 1, 0, n - 1);
        }
    }

    void update(int index, int val) {
        if (n > 0) updateTree(1, 0, n - 1, index, val);
    }

    int sumRange(int left, int right) {
        if (n == 0) return 0;
        return queryTree(1, 0, n - 1, left, right);
    }
};
```

**Key insight.** This is the motivating problem for a segment tree — it perfectly demonstrates why we need it. This IS #1 wrapped in a LeetCode interface.

**Complexity.** Build O(n), Update O(log n), Query O(log n).

**Verified:** Initialized with `{1, 3, 5}`, sumRange(0, 2) is 9. Update(1, 2) makes array `{1, 2, 5}`, sumRange(0, 2) is 8.

---

## 4. Range Minimum Query — GFG

**Approach.** If there are updates, use a min-segment tree. If static, build a Sparse Table.
```
For Sparse Table:
st[i][j] stores minimum in range [i, i + 2^j - 1]
st[i][j] = min(st[i][j-1], st[i + 2^(j-1)][j-1])
Query:
k = log2(R - L + 1)
return min(st[L][k], st[R - 2^k + 1][k])
```

**Key insight.** Sparse Table gives O(1) query for STATIC arrays; segment tree is needed only when updates exist. Use the right tool for the job.

**Complexity.** ST Build O(n log n), ST Query O(1).

---

## 5. Range Sum Query - Immutable — LeetCode 303

**Approach.** Prefix sums array. `pref[i]` stores sum of `nums[0..i]`.
```
sumRange(left, right):
    return pref[right] - (left > 0 ? pref[left-1] : 0)
```

**Key insight.** Prefix sum, NOT segment tree. Contrast — this is what you use when there are NO updates. O(1) query vs O(log n).

**Complexity.** Build O(n), Query O(1).

---

## 6. Range Sum Query 2D - Immutable — LeetCode 304

**Approach.** 2D prefix sums array.
```
pref[i][j] = matrix[i-1][j-1] + pref[i-1][j] + pref[i][j-1] - pref[i-1][j-1]
sumRegion(r1, c1, r2, c2):
    return pref[r2+1][c2+1] - pref[r1][c2+1] - pref[r2+1][c1] + pref[r1][c1]
```

**Key insight.** Inclusion-exclusion formula. Again contrast — a 2D segment tree is overkill here since there are no updates.

**Complexity.** Build O(n*m), Query O(1).

---

## 7. Lazy Propagation — range add + range sum query — **fundamental, code given**

**Approach.** Add a `lazy` array. When a range is updated, we update the node and delay updating its children by storing the pending updates in `lazy`.

```cpp
class LazySegTree {
    vector<int> tree, lazy;
    int n;

    void pushDown(int node, int start, int end) {
        if (lazy[node] != 0) {
            int mid = start + (end - start) / 2;
            // Update children
            tree[2 * node] += lazy[node] * (mid - start + 1);
            lazy[2 * node] += lazy[node];
            
            tree[2 * node + 1] += lazy[node] * (end - mid);
            lazy[2 * node + 1] += lazy[node];
            
            // Clear current node lazy value
            lazy[node] = 0;
        }
    }

    void updateRange(int node, int start, int end, int l, int r, int val) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node] * (end - start + 1);
            if (start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }

        if (r < start || end < l) return;

        if (l <= start && end <= r) {
            tree[node] += val * (end - start + 1);
            if (start != end) {
                lazy[2 * node] += val;
                lazy[2 * node + 1] += val;
            }
            return;
        }

        pushDown(node, start, end); // CRITICAL: push down before recurring
        int mid = start + (end - start) / 2;
        updateRange(2 * node, start, mid, l, r, val);
        updateRange(2 * node + 1, mid + 1, end, l, r, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    int queryRange(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;

        if (lazy[node] != 0) {
            tree[node] += lazy[node] * (end - start + 1);
            if (start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }

        if (l <= start && end <= r) return tree[node];

        pushDown(node, start, end); // CRITICAL: push down before recurring
        int mid = start + (end - start) / 2;
        return queryRange(2 * node, start, mid, l, r) + queryRange(2 * node + 1, mid + 1, end, l, r);
    }
public:
    LazySegTree(int size) {
        n = size;
        tree.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
    }
    void update(int l, int r, int val) { updateRange(1, 0, n - 1, l, r, val); }
    int query(int l, int r) { return queryRange(1, 0, n - 1, l, r); }
};
```

**Key insight.** `pushDown()` MUST be called before accessing children, in both update and query. It ensures that the children have the correct state before we traverse down to them.

**Complexity.** Range Update O(log n), Range Query O(log n).

**Verified:** Update [0, 3] by adding 5. Query [1, 2] returns 10. Update [2, 4] by adding 2. Query [1, 4] returns 5+7+7+2 = 21.

---

## 8. Range XOR Queries

**Approach.** Use a prefix XOR array.
```
pref[i] = pref[i-1] ^ arr[i]
query(l, r) = pref[r] ^ pref[l-1]
```

**Key insight.** XOR is its own inverse, so range XOR from l to r is `prefix[r] XOR prefix[l-1]`. A segment tree works too, but prefix XOR is O(1) for static arrays.

**Complexity.** Build O(n), Query O(1).

---

## 9. Kth Smallest Element in a Sorted Matrix — LeetCode 378

**Approach.** Binary search on the answer range `[matrix[0][0], matrix[n-1][n-1]]`. Count elements `<= mid`.

**Key insight.** Min-heap or binary search is simpler; segment tree approach exists but is not the intended solution. Don't force a complex structure when a simple binary search does it.

**Complexity.** O(n log(max - min)).

---

## 10. Count of Smaller Numbers After Self — LeetCode 315 — **hard, code given**

**Approach.** Coordinate compression + Segment tree. Traverse right to left.

```cpp
class Solution {
public:
    vector<int> tree;
    void update(int node, int start, int end, int idx) {
        if (start == end) { tree[node]++; return; }
        int mid = start + (end - start) / 2;
        if (idx <= mid) update(2 * node, start, mid, idx);
        else update(2 * node + 1, mid + 1, end, idx);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
    int query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return tree[node];
        int mid = start + (end - start) / 2;
        return query(2 * node, start, mid, l, r) + query(2 * node + 1, mid + 1, end, l, r);
    }
    vector<int> countSmaller(vector<int>& nums) {
        int n = nums.size();
        vector<int> res(n, 0);
        // Coordinate Compression
        set<int> s(nums.begin(), nums.end());
        unordered_map<int, int> rank;
        int r = 0;
        for (int x : s) rank[x] = r++;
        
        tree.assign(4 * r, 0);
        for (int i = n - 1; i >= 0; --i) {
            int comp_val = rank[nums[i]];
            if (comp_val > 0) {
                res[i] = query(1, 0, r - 1, 0, comp_val - 1);
            }
            update(1, 0, r - 1, comp_val);
        }
        return res;
    }
};
```

**Key insight.** Coordinate compression is necessary because values can be huge but the count of distinct elements is at most n. The segment tree acts as a frequency array — traversing from right to left allows us to only query numbers that appeared *after* the current number.

**Complexity.** O(n log n) time, O(n) space.

**Verified:** `[5, 2, 6, 1]` returns `[2, 1, 1, 0]`.

---

# Section 2 — Important

## 11. My Calendar I — LeetCode 729

**Approach.** Keep a collection of sorted intervals. For each booking, check if it overlaps with the previous or next interval.
```
set of intervals S
book(start, end):
    it = S.lower_bound({start, end})
    if it != end and it->start < end: return false
    if it != begin and prev(it)->end > start: return false
    S.insert({start, end})
    return true
```

**Key insight.** A `set` or `map` of intervals works beautifully; segment tree is overkill here but educational if you want to model a boolean array.

**Complexity.** O(log n) per booking.

---

## 12. My Calendar II — LeetCode 731

**Approach.** Keep track of standard intervals and a separate set of overlapping intervals (double bookings).

**Key insight.** Count overlaps; reject if any point gets ≥ 3 bookings. Segment tree with lazy propagation is the clean approach. With lazy prop, `queryRange(start, end-1)` checks if the max >= 2. If it is, reject. Else, `updateRange(start, end-1, 1)`.

**Complexity.** O(log(MAX_TIME)) per booking with segment tree.

---

## 13. My Calendar III — LeetCode 732 — **code given**

**Approach.** Lazy propagation Segment Tree with range-add and range-max queries. Due to the large coordinate space (`10^9`), we need to use a dynamic segment tree.

```cpp
class MyCalendarThree {
    unordered_map<int, int> tree, lazy;
    
    void update(int node, int start, int end, int l, int r, int val) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node];
            if (start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
        
        if (r < start || end < l) return;
        
        if (l <= start && end <= r) {
            tree[node] += val;
            if (start != end) {
                lazy[2 * node] += val;
                lazy[2 * node + 1] += val;
            }
            return;
        }
        
        int mid = start + (end - start) / 2;
        update(2 * node, start, mid, l, r, val);
        update(2 * node + 1, mid + 1, end, l, r, val);
        tree[node] = max(tree[2 * node], tree[2 * node + 1]);
    }

public:
    MyCalendarThree() {}
    
    int book(int start, int end) {
        update(1, 0, 1e9, start, end - 1, 1);
        return tree[1]; // Global max is always at the root
    }
};
```

**Key insight.** This is a pure range-add + range-max query problem. The answer is the global max after each booking, which is just the value at the root of the tree (`tree[1]`). The dynamic segment tree handles the `10^9` range by lazily allocating nodes via `unordered_map`.

**Complexity.** O(log(MAX)) time per query, where MAX is 10^9.

**Verified:** `book(10, 20)` -> 1, `book(50, 60)` -> 1, `book(10, 40)` -> 2, `book(5, 15)` -> 3, `book(5, 10)` -> 3, `book(25, 55)` -> 3.

---

## 14. Reverse Pairs — LeetCode 493

**Approach.** Count `i < j` such that `nums[i] > 2 * nums[j]`. Either use Merge Sort or a Segment Tree.

**Key insight.** Merge sort is the cleaner O(n log n) approach, but segment tree with coordinate compression also works — process from left to right, for each element query `[2*nums[i]+1, MAX]`, then insert `nums[i]`. The difficulty lies in compressing `nums[i]` and `2*nums[i]` together into a tight index space.

**Complexity.** O(n log n).

---

## 15. Count of Range Sum — LeetCode 327

**Approach.** Prefix sums + segment tree or merge sort.
`lower <= prefix[j] - prefix[i] <= upper` translates to `prefix[j] - upper <= prefix[i] <= prefix[j] - lower`.

**Key insight.** For each prefix sum `prefix[j]`, count how many previous prefix sums fall in `[prefix[j]-upper, prefix[j]-lower]`. A segment tree with coordinate compression of all possible prefix boundaries handles this seamlessly.

**Complexity.** O(n log n).

---

## 16. Range Module — LeetCode 715

**Approach.** Maintain a dynamic Segment Tree with Lazy Propagation. Operations are range assign 1 (addRange), range assign 0 (removeRange), and query min (queryRange).

**Key insight.** Lazy propagation with range ASSIGNMENT (set to true/false), not range addition. `pushDown` *replaces* children's values instead of adding to them. Querying requires checking if the min value in the range is 1.

**Complexity.** O(log MAX) per operation.

---

## 17. Falling Squares — LeetCode 699

**Approach.** Segment tree over the x-axis. `update(L, R, height)` sets the range to the new height.

**Key insight.** Coordinate compression + segment tree with range max query and range assignment update. The new height of a square is `max(height in [left, right-1]) + square_size`.

**Complexity.** O(n log n).

---

## 18. Number of Longest Increasing Subsequence — LeetCode 673

**Approach.** LIS using a Segment Tree where nodes store `{max_length, count}`.

**Key insight.** DP optimization — segment tree stores `{length, count}` pairs and merges them. When merging two children, if lengths are equal, sum the counts; otherwise, take the count of the larger length.

**Complexity.** O(n log n).

---

# Section 3 — Good to Know

## 19. Create Sorted Array through Instructions — LeetCode 1649

**Approach.** For each element inserted, we need the count of elements strictly less than it, and strictly greater than it.

**Key insight.** Same technique as #10. Use a segment tree as a frequency array. The cost is `min(query(0, val-1), query(val+1, MAX))`. Add `val` to the tree after.

**Complexity.** O(n log MAX).

---

## 20. Longest Increasing Subsequence II — LeetCode 2407

**Approach.** Range max segment tree over the values.

**Key insight.** Segment tree stores max LIS length for each value. For each element `val`, query `[val-k, val-1]` for the best predecessor length. Update `val` with `best + 1`.

**Complexity.** O(n log(MAX_VAL)).

---

## 21. Rectangle Area II — LeetCode 850

**Approach.** Line sweep.

**Key insight.** Line sweep on x-coordinates + segment tree on y-coordinates. The segment tree counts the total covered length of y-intervals at the current x. As the sweep line moves right, add the covered y-length multiplied by the delta x.

**Complexity.** O(n log n).

---

## 22. The Skyline Problem — LeetCode 218

**Approach.** Line sweep storing heights.

**Key insight.** Event-based line sweep with a max-heap of active heights. Segment tree approach exists (range max update, point query) but `multiset` or priority queue is cleaner and less error-prone to write.

**Complexity.** O(n log n).

---

## 23. Minimum Number of Increments on Subarrays to Form a Target Array — LeetCode 1526

**Approach.** Iterate through the array and accumulate positive differences.

**Key insight.** Greedy is O(n) and much simpler; just count increases in adjacent differences (`target[i] - target[i-1]`). A segment tree doing divide-and-conquer range-minimums works but is O(n log n) and overkill.

**Complexity.** O(n).

---

## 24. Count Good Meals — LeetCode 1711

**Approach.** Hash map storing frequencies of seen numbers.

**Key insight.** Hash map checking powers of 2 (up to 2^21) is the intended O(n) approach. Don't force a tree structure.

**Complexity.** O(n).

---

## 25. Online Majority Element In Subarray — LeetCode 1157

**Approach.** Segment Tree returning candidate majorities.

**Key insight.** Segment tree stores a Boyer-Moore candidate per node. Merge picks the surviving candidate. Then verify with binary search on sorted index lists (using `lower_bound` and `upper_bound`) to confirm if the frequency is `>= threshold`.

**Complexity.** Query O(log n).

---

# Section 4 — approach only

- **Range GCD Query** — Just like range min/sum. Segment tree storing `gcd(left, right)`. Update/Query logic is exactly the same, replacing `+` with `__gcd()`.
- **Merge Sort Tree** — Each node in the segment tree stores a sorted `vector` of all elements in its range. Built similarly to merge sort. Querying involves `lower_bound` inside the nodes. Used for counting elements `>= k` in a range.
- **Persistent Segment Tree** — Whenever an update happens, instead of modifying the node, create a new node and link to the unchanged children. This gives access to the segment tree's state at any historical update.
- **Inversion Count using Segment Tree** — Coordinate compress the array, then iterate. For element `x`, query `[x+1, MAX]` for inversions, then update point `x` with `+1`. Same as #10.
- **Range Maximum Subarray Sum** — Each node stores 4 values: total sum, prefix max sum, suffix max sum, and max subarray sum. Merge function carefully combines these to form the optimal subarray sum that might cross the boundary between left and right children.
