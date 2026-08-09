# 13 — Prefix Sum

One idea: **precompute cumulative totals so any range query becomes a subtraction.** Ten
minutes to learn, and it turns a whole class of O(n²) problems into O(n). It also generalises
further than most people expect — to products, XOR, counts, 2D grids, and (run backwards) to
difference arrays.

---

## 1. The core

```cpp
// 1-indexed prefix: pre[i] = sum of the first i elements, pre[0] = 0
vector<long long> buildPrefix(vector<int>& a) {
    vector<long long> pre(a.size() + 1, 0);
    for (int i = 0; i < (int)a.size(); ++i) pre[i + 1] = pre[i] + a[i];
    return pre;
}
long long rangeSum(vector<long long>& pre, int l, int r) {   // inclusive [l, r]
    return pre[r + 1] - pre[l];
}
```

Build once in O(n); every subsequent range query is **O(1)**. For `q` queries that is
O(n + q) instead of O(n·q).

**Use the size-`n+1`, 1-indexed form.** With `pre[0] = 0`, `rangeSum(0, r)` works with no special
case. The size-`n` version forces `l == 0 ? pre[r] : pre[r] - pre[l-1]` at every call site, and
that branch is where the bugs go.

**Use `long long`.** With n = 10⁵ elements up to 10⁹, the total reaches 10¹⁴ — an `int` prefix
array silently overflows. The individual elements fitting in `int` says nothing about their sum.

### The in-place variant

When you don't need the original array, overwrite it:

```cpp
for (int i = 1; i < n; ++i) a[i] += a[i - 1];
```

Now `a[i]` is the sum of `a[0..i]`. This is what LeetCode 1480 asks for literally, and what
`ques.cpp` does correctly. It is O(1) extra space, at the cost of destroying the input.

> **The `+=` is the whole line.** `a[i] = a[i-1]` — dropping the `+` — compiles fine and fills
> the array with copies of `a[0]`. That is exactly the bug in `prefixsumAlgo.cpp:17`, and
> running it prints `1 1 1 1 1 1 1`.

---

## 2. Prefix + hash map — subarrays with a given sum

The single most important application, and the reason "prefix sum" shows up in hard interviews.

**The identity.** `sum(l..r) = pre[r] - pre[l-1]`. So asking "does a subarray ending at `r` sum
to `k`?" is asking "have I seen the running sum `pre[r] - k` before?" That is a lookup, not a
scan.

```cpp
int subarraySum(vector<int>& a, int k) {                 // LeetCode 560
    unordered_map<long long,int> seen;
    seen[0] = 1;                                         // the empty prefix
    long long run = 0; int cnt = 0;
    for (int x : a) {
        run += x;
        cnt += seen[run - k];                            // subarrays ending here
        seen[run]++;
    }
    return cnt;
}
```

**`seen[0] = 1` is the line people forget.** Without it, a subarray starting at index 0 is never
counted — there is no earlier prefix to subtract.

For the **longest** such subarray rather than the count, store the *first* index each running sum
appeared at and never overwrite it — the earliest start gives the longest span.

> `unordered_map` arrives in `../24_maps`. When all values are **positive**, a sliding window
> solves the same problem in O(1) space with no map at all — the running sum is then monotonic,
> which is what makes the window valid. That is `../14_Sliding window`.

---

## 3. Prefix works for any invertible operation

Sum is just the common case. The requirement is an operation with an **inverse**, so the prefix
of a range can be recovered from two endpoints:

| Operation | Combine | Recover range `[l,r]` |
|---|---|---|
| Sum | `pre[i] = pre[i-1] + a[i]` | `pre[r] - pre[l-1]` |
| XOR | `pre[i] = pre[i-1] ^ a[i]` | `pre[r] ^ pre[l-1]` (XOR is its own inverse) |
| Product | `pre[i] = pre[i-1] * a[i]` | `pre[r] / pre[l-1]` — **breaks on a zero** |
| Count of X | `pre[i] = pre[i-1] + (a[i]==X)` | `pre[r] - pre[l-1]` |

**Min and max have no inverse**, so prefix arrays do not work for them — you need a sparse table
or a segment tree. Knowing *why* is a good interview answer.

The product row is why **Product of Array Except Self** (LC 238) forbids division: a single zero
makes the prefix-quotient approach collapse. The fix is a prefix pass and a suffix pass, never
dividing:

```cpp
vector<int> productExceptSelf(vector<int>& a) {
    int n = (int)a.size();
    vector<int> res(n, 1);
    int run = 1;
    for (int i = 0; i < n; ++i)      { res[i] = run;  run *= a[i]; }   // product of everything LEFT
    run = 1;
    for (int i = n - 1; i >= 0; --i) { res[i] *= run; run *= a[i]; }   // times everything RIGHT
    return res;
}
```

O(n) time, O(1) extra space (the output does not count). **The prefix/suffix pair is the
transferable idea** — it also solves "trapping rain water", "candy", and LC 2483.

---

## 4. Difference array — prefix sum run backwards

For **many range updates followed by one read**, invert the relationship: store *differences*,
then prefix-sum at the end.

```cpp
// apply "add v to every index in [l, r]" many times, in O(1) each
vector<int> diff(n + 1, 0);
for (each update l, r, v) { diff[l] += v; diff[r + 1] -= v; }

int run = 0;                                  // then ONE prefix pass reconstructs the array
for (int i = 0; i < n; ++i) { run += diff[i]; res[i] = run; }
```

Each update is O(1) instead of O(r−l). For `q` updates that is O(n + q) rather than O(n·q).
Size the array `n+1` so `diff[r+1]` is always writable when `r == n-1`.

Prefix sum answers range queries fast; difference arrays apply range updates fast. They are the
same identity read in opposite directions.

---

## 5. 2D prefix sums

```cpp
p[i+1][j+1] = m[i][j] + p[i][j+1] + p[i+1][j] - p[i][j];   // subtract the double-counted corner
```

Then the sum of the rectangle from `(r1,c1)` to `(r2,c2)` inclusive is:

```cpp
p[r2+1][c2+1] - p[r1][c2+1] - p[r2+1][c1] + p[r1][c1];     // inclusion-exclusion
```

**Both formulas are inclusion–exclusion.** Building: adding the row-above and column-left
prefixes counts their overlap twice, so subtract it once. Querying: subtracting the strip above
and the strip to the left removes their intersection twice, so add it back. The `+1` padding row
and column remove every boundary special case.

---

## Interview Q&A

**Q1. When does a prefix sum actually pay off?**
When you make **multiple** range queries. Building costs O(n), so a single query is no better
than just summing that range. With `q` queries it is O(n+q) versus O(n·q). If the array is being
*modified* between queries, a plain prefix array is the wrong structure — rebuilding is O(n) per
update, and you want a Fenwick or segment tree.

**Q2. Why `long long` for the prefix array?**
Because element values fitting in `int` says nothing about their sum. 10⁵ elements of 10⁹ gives
10¹⁴, far past `int`. Signed overflow is undefined behaviour, not merely a wrong number.

**Q3. Explain the prefix + hash map trick.**
`sum(l..r) = pre[r] − pre[l−1]`, so "is there a subarray ending at `r` with sum `k`?" becomes
"have I seen the prefix value `pre[r] − k`?" — an O(1) lookup. Store running sums in a map as
you go. Seed it with `{0: 1}` so subarrays starting at index 0 are counted. O(n) instead of
O(n²).

**Q4. Why does Product of Array Except Self forbid division?**
Because a zero in the array makes the total product zero, and you cannot divide it back out.
Even with the zero-count workaround, division on integers is a needless correctness risk. The
prefix-product/suffix-product pair avoids it entirely in O(n) time and O(1) extra space.

**Q5. What is a difference array and when do you use it?**
The inverse of a prefix sum. To add `v` over `[l,r]` you write `diff[l] += v` and
`diff[r+1] -= v` in O(1); after all updates, one prefix pass reconstructs the array. Use it for
many range updates and a single final read — booking systems, flight-seat problems, interval
counting.

**Q6. Which operations support a prefix array, and which don't?**
Any operation with an inverse: sum (subtract), XOR (XOR), product (divide, if no zeros), and
counts. **Min and max do not** — knowing the min of `[0..r]` and `[0..l-1]` tells you nothing
about the min of `[l..r]`. Those need a sparse table (O(1) query, immutable) or a segment tree
(O(log n) query, updatable).

**Q7. How do you find the equilibrium index of an array?**
The index where the sum to its left equals the sum to its right. Compute the total, then walk
left to right maintaining a running left sum; at index `i` the right sum is
`total − left − a[i]`. One pass, O(1) space. Your `ques.cpp` solves the closely related
*partition* version — see `solution.md` for how the two differ.

---

## Your original notes (preserved)

The problem list from this file is kept verbatim. **All four are covered** — see `questions.md`.

```
1480 Running sum of !d array 
238 Product of array except self 
2483 minimum penalty for a shop
1402 Reducing Dishes
```

| Your entry | Now at |
|---|---|
| 1480 Running Sum of 1d Array | `questions.md` §1 #1 — the in-place form; cf. `prefixsumAlgo.cpp` |
| 238 Product of Array Except Self | `questions.md` §1 #5 — prefix × suffix, no division |
| 2483 Minimum Penalty for a Shop | `questions.md` §2 #10 — running penalty, one pass |
| 1402 Reducing Dishes | `questions.md` §2 #11 — sort, then suffix sums |
