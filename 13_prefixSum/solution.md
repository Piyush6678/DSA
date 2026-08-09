# 13 — Prefix Sum: Solutions

Full solutions for Sections 1–3 of `questions.md`. **Every function below was compiled with
`g++ -std=gnu++14` and executed against the test cases shown — all passing, as part of a
138-assertion run covering this folder and `../12_sorting`.**

Assume `#include <vector>`, `#include <algorithm>`, `using namespace std;`.

---

# Section 1 — Must Do

## 1. Running Sum of 1d Array — LeetCode 1480

```cpp
vector<int> runningSum(vector<int> a) {
    for (int i = 1; i < (int)a.size(); ++i) a[i] += a[i - 1];   // the += is the whole line
    return a;
}
```

**Key insight.** In place, one pass, O(1) extra space. Start at `i = 1` — `a[0]` is already its
own prefix, and starting at 0 would read `a[-1]`.

**The `+=` is not optional.** `a[i] = a[i-1]` compiles cleanly and fills the array with copies
of `a[0]`. That is the bug in `prefixsumAlgo.cpp:17` — see the bugs section.

**Complexity.** O(n) time, O(1) extra space.

**Verified:** `{1,2,3,4}→{1,3,6,10}`, `{1,1,1,1,1}→{1,2,3,4,5}`, `{7}→{7}`, and your own array
`{1,4,5,2,5,2,6}→{1,5,10,12,17,19,25}`.

---

## 2. Range Sum Query — Immutable — LeetCode 303

```cpp
struct NumArray {
    vector<long long> pre;                              // long long, not int
    NumArray(vector<int>& a) {
        pre.assign(a.size() + 1, 0);                    // size n+1, pre[0] = 0
        for (size_t i = 0; i < a.size(); ++i) pre[i + 1] = pre[i] + a[i];
    }
    long long sumRange(int l, int r) { return pre[r + 1] - pre[l]; }
};
```

**Key insight — the `n+1` padding.** With `pre[0] = 0` the query is a single expression with no
branch. The size-`n` alternative forces `l == 0 ? pre[r] : pre[r] - pre[l-1]` at every call site,
and that conditional is where mistakes live.

**`long long`** because the total can exceed `int` even when every element fits comfortably.

**Complexity.** O(n) to build, **O(1)** per query.

**Verified:** `{-2,0,3,-5,2,-1}` — `sumRange(0,2)=1`, `sumRange(2,5)=-1`, `sumRange(0,5)=-3`,
`sumRange(0,0)=-2` (the `l == 0` edge that the unpadded version gets wrong).

---

## 3. Find Pivot Index — LeetCode 724

```cpp
int pivotIndex(vector<int>& a) {
    long long total = 0; for (int x : a) total += x;
    long long left = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        if (left == total - left - a[i]) return i;      // a[i] belongs to NEITHER side
        left += a[i];
    }
    return -1;
}
```

**Key insight.** No prefix array needed — one running total is enough. The right sum is
`total − left − a[i]`, and subtracting `a[i]` is what excludes the pivot from both sides.

**Complexity.** O(n) time, O(1) space.

**Verified:** `{1,7,3,6,5,6}→3`, `{1,2,3}→-1`, `{2,1,-1}→0` (pivot at index 0, where the left
sum is the empty sum 0).

---

## 4. Equilibrium Point / equal partition — **and how it differs from #3**

These two problems are constantly confused, and they give different answers on the same input.

```cpp
int equilibriumSplit(vector<int>& a) {          // left [0..i] == right [i+1..n-1]
    long long total = 0; for (int x : a) total += x;
    long long run = 0;
    for (int i = 0; i < (int)a.size() - 1; ++i) {
        run += a[i];
        if (2 * run == total) return i;         // run == total - run
    }
    return -1;
}
```

| | LeetCode 724 (#3) | Equal partition (#4) |
|---|---|---|
| Element `a[i]` | belongs to **neither** side | belongs to the **left** side |
| Condition | `left == total − left − a[i]` | `2 * left == total` |
| On `{1,2,3,4,5,15}` | **−1** | **4** |
| On `{1,7,3,6,5,6}` | **3** | **−1** |

**Your `ques.cpp` implements #4, and implements it correctly.** `2*arr[i] - arr[n-1] == 0` is
exactly `2 * prefix[i] == total` once the array has been turned into its own prefix array.
**Verified: it prints `4` for `{1,2,3,4,5,15}`, which is right.** If you had run it against
LeetCode 724's tests it would have failed, and the reason would have been the problem statement,
not the code.

**Complexity.** O(n) time, O(1) extra space.

---

## 5. Product of Array Except Self — LeetCode 238

```cpp
vector<int> productExceptSelf(vector<int>& a) {
    int n = (int)a.size();
    vector<int> res(n, 1);
    int run = 1;
    for (int i = 0; i < n; ++i)      { res[i] = run;  run *= a[i]; }   // everything to the LEFT
    run = 1;
    for (int i = n - 1; i >= 0; --i) { res[i] *= run; run *= a[i]; }   // times everything RIGHT
    return res;
}
```

**Key insight — the order within each loop.** Assign `res[i]` **before** folding `a[i]` into
`run`. That is what excludes the element itself. Swap the two statements and every entry includes
its own value.

**Why no division.** `total / a[i]` fails on a zero, and zeros are the whole point of the test
cases. The prefix/suffix pair sidesteps it: with one zero, every other position gets 0 (its
product spans the zero) and the zero's own position gets the product of everything else.

**O(1) extra space** — the output array does not count, and `run` replaces both auxiliary arrays.

**Complexity.** O(n) time, O(1) extra space.

**Verified:** `{1,2,3,4}→{24,12,8,6}`, `{-1,1,0,-3,3}→{0,0,9,0,0}` (one zero), `{0,0}→{0,0}`
(two zeros — every entry is 0).

---

## 6. Subarray Sum Equals K — LeetCode 560 `[hash]`

**Brute force.** Every `(l,r)` pair with a running sum: O(n²).

**Optimal — prefix + hash map:**

```cpp
int subarraySum(vector<int>& a, int k) {
    unordered_map<long long,int> seen;
    seen[0] = 1;                                   // the empty prefix — DO NOT omit
    long long run = 0; int cnt = 0;
    for (int x : a) {
        run += x;
        cnt += seen[run - k];                      // every earlier prefix equal to run-k
        seen[run]++;
    }
    return cnt;
}
```

**Key insight.** `sum(l..r) = pre[r] − pre[l−1]`, so a subarray ending at `r` sums to `k`
precisely when some earlier prefix equals `run − k`. Counting those is a map lookup, not a scan.

**`seen[0] = 1` is the line people forget.** Without it, a qualifying subarray that starts at
index 0 has no earlier prefix to match and is never counted.

**Increment `seen[run]` *after* the lookup**, or a `k == 0` query counts the current prefix
against itself.

> **Why not a sliding window?** Because values may be negative, so the running sum is not
> monotonic — shrinking the window can *increase* the sum. Sliding windows need positivity. That
> restriction is exactly the boundary between this folder and `../14_Sliding window`.

**Complexity.** O(n) time, O(n) space.

**Verified:** `{1,1,1}` k=2 → 2, `{1,2,3}` k=3 → 2, `{1,-1,0}` k=0 → 3 (negatives and a zero).

---

## 7. Longest Subarray with Sum K `[hash]`

```cpp
int longestSubarraySumK(vector<int>& a, int k) {
    unordered_map<long long,int> firstIdx;
    long long run = 0; int best = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        run += a[i];
        if (run == k) best = i + 1;                                   // from index 0
        if (firstIdx.count(run - k)) best = max(best, i - firstIdx[run - k]);
        if (!firstIdx.count(run)) firstIdx[run] = i;                  // keep the EARLIEST only
    }
    return best;
}
```

**Key insight — never overwrite an existing entry.** For the *longest* span you want the
earliest occurrence of each prefix value; overwriting with a later index shortens every future
answer. That single `if (!firstIdx.count(run))` is the difference between #6 and #7.

**Complexity.** O(n) time, O(n) space.

**Verified:** `{10,5,2,7,1,9}` k=15 → 4, `{-1,1,1,-1,1,1}` k=2 → 6 (the whole array),
`{1,2,3}` k=100 → 0.

---

## 8. Contiguous Array — LeetCode 525 `[hash]`

```cpp
int findMaxLength(vector<int>& a) {
    unordered_map<int,int> first;
    first[0] = -1;                                 // prefix 0 sits "before" index 0
    int run = 0, best = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        run += (a[i] == 1) ? 1 : -1;               // relabel 0 as -1
        if (first.count(run)) best = max(best, i - first[run]);
        else                  first[run] = i;
    }
    return best;
}
```

**Key insight — the relabelling.** Counting 0s and 1s separately is awkward; treating 0 as −1
turns "equal counts" into "**sum zero**", which is #7 with `k = 0`. Two indices sharing a running
sum bracket a balanced stretch.

`first[0] = -1` makes a balanced prefix starting at index 0 come out with the right length.

**Complexity.** O(n) time, O(n) space.

**Verified:** `{0,1}→2`, `{0,1,0}→2`, `{0,0,1,0,0,0,1,1}→6`, `{1,1,1}→0`.

---

# Section 2 — Important

## 9. Subarray Sums Divisible by K — LeetCode 974 — **the modulo trap**

```cpp
int subarraysDivByK(vector<int>& a, int k) {
    vector<int> cnt(k, 0);                         // only k possible remainders — no map needed
    cnt[0] = 1;
    int run = 0, res = 0;
    for (int x : a) {
        run = ((run + x) % k + k) % k;             // normalise into [0, k)
        res += cnt[run];
        cnt[run]++;
    }
    return res;
}
```

**Key insight.** Two prefixes with the same remainder mod `k` bracket a subarray divisible by
`k`. Since there are only `k` remainders, an array replaces the map — so this one is **fully in
scope**, no `unordered_map` needed.

**The trap: C++ `%` can return a negative.** `(-7) % 5` is `-2`, not `3`. A negative index into
`cnt` is an out-of-bounds write, and even with a map it splits what should be one bucket into
two. `((r % k) + k) % k` folds it back.

**Verified, both versions on `{4,5,0,-2,-3,1}` with k=5: the corrected version gives 7 (the
right answer); the naive `run = (run + x) % k` gives 4.** Three matches silently lost.

**Complexity.** O(n) time, O(k) space.

---

## 10. Minimum Penalty for a Shop — LeetCode 2483

```cpp
int bestClosingTime(string customers) {
    int penalty = 0, best = 0, bestIdx = 0;
    for (int i = 0; i < (int)customers.size(); ++i) {
        if (customers[i] == 'Y') --penalty;         // open & customers came: penalty drops
        else                     ++penalty;         // open & nobody came: penalty rises
        if (penalty < best) { best = penalty; bestIdx = i + 1; }
    }
    return bestIdx;
}
```

**Key insight.** The absolute penalty is never needed — only *changes* matter. Starting from
"close at hour 0", each additional open hour either helps (a `'Y'`, −1) or hurts (an `'N'`, +1).
So the answer is the index of the **minimum running total**, which is one pass and O(1) space.

Strict `<` on `penalty < best` gives the **earliest** minimum, which is what the problem asks for
on ties.

**Complexity.** O(n) time, O(1) space.

**Verified:** `"YYNY"→2`, `"NNNNN"→0` (close immediately), `"YYYY"→4` (never close).

---

## 11. Reducing Dishes — LeetCode 1402 `[sort]`

```cpp
int maxSatisfaction(vector<int> a) {
    sort(a.begin(), a.end());
    int n = (int)a.size(), best = 0, suffix = 0, total = 0;
    for (int i = n - 1; i >= 0; --i) {
        suffix += a[i];        // sum of dishes from i to the end
        total  += suffix;      // prepending one dish re-counts EVERY dish after it
        if (total > best) best = total;
    }
    return best;
}
```

**Key insight — why the suffix sum appears.** The score is `Σ (position × satisfaction)`.
Prepending a dish at the front shifts every later dish's position up by one, adding their entire
sum to the total. So walking right to left, `total += suffix` computes the new score in O(1) per
step.

Sorting first is what makes greedy valid: higher-satisfaction dishes belong at higher positions.
`best` starts at 0 because cooking nothing is allowed — which is the answer when every dish is
negative.

**Complexity.** O(n log n) time, O(1) extra space.

**Verified:** `{-1,-8,0,5,-9}→14`, `{4,3,2}→20`, `{-1,-4,-5}→0` (cook nothing).

---

## 12. Maximum Subarray — LeetCode 53 — **two views of one algorithm**

```cpp
int maxSubArrayKadane(vector<int>& a) {                    // the familiar form
    int best = a[0], cur = a[0];
    for (size_t i = 1; i < a.size(); ++i) {
        cur = max(a[i], cur + a[i]);
        best = max(best, cur);
    }
    return best;
}
int maxSubArrayPrefix(vector<int>& a) {                    // the prefix-sum reading
    long long run = 0, minPre = 0; int best = INT_MIN;
    for (int x : a) {
        run += x;
        best = max(best, (int)(run - minPre));             // best subarray ending here
        minPre = min(minPre, run);                         // cheapest place to have started
    }
    return best;
}
```

**Key insight.** `sum(l..r) = pre[r] − pre[l−1]`, so maximising the subarray sum means
maximising `pre[r] − min(pre[l−1] for l ≤ r)`. Tracking the running minimum prefix gives Kadane's
algorithm from the prefix-sum identity — the two are the same computation. Seeing that connects
`../07_Array` to this folder.

`best` must start at `INT_MIN` (or `a[0]`), never 0, or an all-negative array returns 0.

**Complexity.** O(n) time, O(1) space, both forms.

**Verified:** `{-2,1,-3,4,-1,2,1,-5,4}→6` from **both** functions; all-negative `{-3,-1,-2}→-1`
from both; single element → itself.

---

## 13. Range Sum Query 2D — LeetCode 304

```cpp
struct NumMatrix {
    vector<vector<long long> > p;
    NumMatrix(vector<vector<int> >& m) {
        int R = (int)m.size(), C = (int)m[0].size();
        p.assign(R + 1, vector<long long>(C + 1, 0));
        for (int i = 0; i < R; ++i)
            for (int j = 0; j < C; ++j)
                p[i+1][j+1] = m[i][j] + p[i][j+1] + p[i+1][j] - p[i][j];
    }
    long long sumRegion(int r1, int c1, int r2, int c2) {
        return p[r2+1][c2+1] - p[r1][c2+1] - p[r2+1][c1] + p[r1][c1];
    }
};
```

**Key insight — inclusion–exclusion, twice.** Building: the region above and the region to the
left overlap in the top-left block, counted twice, so subtract it once. Querying: removing the
strip above and the strip to the left removes their intersection twice, so add it back. The
padding row and column make every boundary case disappear.

**Complexity.** O(R·C) to build, **O(1)** per query.

**Verified:** on the standard 5×5 matrix — `sumRegion(2,1,4,3)=8`, `sumRegion(1,1,2,2)=11`,
`sumRegion(1,2,2,4)=12`, `sumRegion(0,0,0,0)=3` (single cell at the origin).

---

## 14–15. Corporate Flight Bookings (1109) / Range Addition — **the difference array**

```cpp
vector<int> corpFlightBookings(vector<vector<int> >& b, int n) {
    vector<int> diff(n + 1, 0);                    // n+1 so diff[r+1] is always writable
    for (size_t i = 0; i < b.size(); ++i) {
        diff[b[i][0] - 1] += b[i][2];              // flights are 1-indexed
        diff[b[i][1]]     -= b[i][2];
    }
    vector<int> res(n); int run = 0;
    for (int i = 0; i < n; ++i) { run += diff[i]; res[i] = run; }   // one prefix pass
    return res;
}
```

**Key insight.** Prefix sum run backwards. Adding `v` to every index in `[l, r]` is two writes
— `+v` where the effect starts and `−v` where it stops — and a single prefix pass at the end
replays all of them. Each update is O(1) instead of O(r−l).

Use it whenever there are **many range updates and one final read**. If reads are interleaved
with updates, you want a Fenwick tree instead.

**Complexity.** O(n + q) time, O(n) space.

**Verified:** `{{1,2,10},{2,3,20},{2,5,25}}` with n=5 → `{10,55,45,25,25}`; `{{1,2,10},{2,2,15}}`
with n=2 → `{10,25}`. A generic version with 0-indexed ops on n=5 → `{-2,0,3,5,3}`.

---

# Section 3 — Good to Know

## 16. Count Number of Nice Subarrays — LeetCode 1248

```cpp
int numberOfSubarrays(vector<int>& a, int k) {
    unordered_map<int,int> seen; seen[0] = 1;
    int run = 0, res = 0;
    for (int x : a) {
        run += (x & 1);                            // count ODDS instead of summing
        res += seen.count(run - k) ? seen[run - k] : 0;
        seen[run]++;
    }
    return res;
}
```

**Key insight.** #6 with the running quantity changed from "sum" to "count of odd numbers".
Nothing else moves. Recognising that a problem is #6 in disguise is the actual skill here — and
since the count only ranges over `0..n`, a plain `vector<int>` works instead of a map, putting
this in scope.

**Verified:** `{1,1,2,1,1}` k=3 → 2, `{2,4,6}` k=1 → 0, `{2,2,2,1,2,2,1,2,2,2}` k=2 → 16.

---

## 17. Binary Subarrays With Sum — LeetCode 930 — **the map-free alternative**

```cpp
int atMost(vector<int>& a, int goal) {             // subarrays with sum <= goal
    if (goal < 0) return 0;
    int l = 0, run = 0, res = 0;
    for (int r = 0; r < (int)a.size(); ++r) {
        run += a[r];
        while (run > goal) run -= a[l++];
        res += (r - l + 1);                        // all subarrays ending at r
    }
    return res;
}
int numSubarraysWithSum(vector<int>& a, int goal) {
    return atMost(a, goal) - atMost(a, goal - 1);  // exactly == at most k minus at most k-1
}
```

**Key insight.** `exactly(k) = atMost(k) − atMost(k−1)` — a genuinely reusable trick. It needs no
map at all, only a sliding window, which works here because all values are non-negative (0 or 1)
so the running sum is monotonic. This is the direct bridge into `../14_Sliding window`.

The `res += (r - l + 1)` line counts every subarray ending at `r` at once, which is the part
worth internalising.

**Complexity.** O(n) time, O(1) space — better than the prefix-map solution on space.

**Verified:** `{1,0,1,0,1}` goal=2 → 4, `{0,0,0,0,0}` goal=0 → 15 (= 5·6/2, every subarray).

---

## 18. Maximum Size Subarray Sum Equals k — LeetCode 325

Identical to #7, with a judge attached. Same rule: store the **first** index of each prefix value
and never overwrite it.

---

# Section 4 — approach only

- **Left/Right Sum Differences (2574)** — prefix and suffix arrays, then `abs(left[i]-right[i])`.
- **Find the Highest Altitude (1732)** — running sum, track the maximum; start at 0.
- **Number of Ways to Split Array (2270)** — one pass; count indices where `left >= total-left`.
- **Ways to Make a Fair Array (1664)** — **four** prefix sums (odd-index and even-index, left and
  right). Removing element `i` swaps the parity of everything after it, which is the insight.
- **Trapping Rain Water (42)** — prefix-max and suffix-max per position, then
  `min(leftMax, rightMax) - height[i]`. Two pointers reduce it to O(1) space.
- **Candy (135)** — left-to-right pass, then right-to-left, taking the max at each index.
- **Subarray Product Less Than K (713)** — do **not** use prefix products; they overflow fast.
  A sliding window with division-out works because values are positive.
- **XOR Queries (1310)** — prefix XOR; recover a range with `pre[r+1] ^ pre[l]`, since XOR is its
  own inverse.
- **Matrix Block Sum (1314)** — #13 with the query bounds clamped to the matrix edges.
- **Maximum Sum Rectangle** — fix a pair of columns, collapse the rows between them into a 1D
  array of sums, run Kadane. O(C²·R).
- **Number of Submatrices That Sum to Target (1074)** — same column-pair collapse, then #6 on
  each resulting 1D array.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `13_prefixSum/` was edited.**

## `prefixsumAlgo.cpp:17` — the second loop destroys the prefix sums

```cpp
int pre[n];
int sum=0;
for  (int i =0;i<n;i++){        // :9-12  this loop is CORRECT
    sum+=arr[i];
    pre[i]=sum;
};
//or 
pre[0]=arr[0];
for  (int i =1;i<n;i++){
    pre[i]=pre[i-1];            // :17  missing  + arr[i]
};
```

The first loop computes the prefix sums correctly. The second — labelled `//or` as an alternative
— then **overwrites them**, and because it assigns `pre[i-1]` without adding `arr[i]`, every
entry becomes a copy of `pre[0]`.

**Verified: the program prints `1 1 1 1 1 1 1`. The correct output is `1 5 10 12 17 19 25`.**

The fix is one character:

```cpp
pre[i] = pre[i-1] + arr[i];
```

Two smaller points:

1. **`int pre[n]` is a variable-length array**, which is a GCC extension and not standard C++.
   `n` is a compile-time constant here so it happens to be fine, but `vector<int> pre(n)` is the
   portable form. Same note as `../07_Array`.
2. **Use `long long`** for a prefix array in any real problem — see `readme.md` §1.

Only one of the two loops should survive. The first one is the better of the two, because it
does not need the separate `pre[0] = arr[0]` initialisation.

---

## What you got right

- **`ques.cpp` is correct.** Verified: it prints the prefix sums `3 6 10 15 30` and then `4`, and
  4 **is** the right answer for the equal-partition question on `{1,2,3,4,5,15}` — the left half
  `1+2+3+4+5 = 15` equals the right half `15`.

  Two things worth naming explicitly:

  - **Building the prefix sums in place** with `arr[i] += arr[i-1]` is the O(1)-space form, and
    it is what LeetCode 1480 asks for. This is the exact line that `prefixsumAlgo.cpp:17` gets
    wrong — you wrote it correctly here.
  - **`2*arr[i] - arr[n-1] == 0`** is a neat way to write "prefix equals half the total" without
    dividing, which avoids the odd-total rounding problem entirely. Comparing `arr[i] == total/2`
    would wrongly match when the total is odd. Whether or not that was deliberate, it is the
    right form.

  The loop bound `i < n-1` is also correct — the split point cannot be the last index, since the
  right half would be empty.

- **The first loop in `prefixsumAlgo.cpp` (`:9-12`) is correct**, including using a separate
  `sum` accumulator rather than reading back from `pre`. It is only the "alternative" second loop
  that is broken — and having written the working version immediately above it makes the fix
  obvious once you see the output.

- **Your `readme.md` problem list is well chosen.** 1480, 238, 2483 and 1402 cover the in-place
  running sum, the prefix/suffix pair, the running-minimum sweep and the suffix-sum greedy — four
  genuinely different applications rather than four variations of the same range query. That is a
  better-balanced selection than most prefix-sum lists.
