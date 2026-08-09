# 14 — Sliding Window: Solutions

**New format from this folder on:** approach, pseudocode, complexity and the key insight.
Real C++ appears only for the two window **templates** (fundamental — the code *is* the lesson),
the **monotonic deque** (a different data structure), and the two **trick** problems. Everything
else you write yourself from the pseudocode.

**Everything that does appear as code was compiled with `g++ -std=gnu++14` and executed — 77
assertions across folders 14–17, all passing.** Measured outputs are quoted.

---

# Section 1 — Must Do

## 1–2. Maximum sum / average subarray of size K — **fundamental, code given**

```cpp
long long maxSumWindow(vector<int>& a, int k) {
    long long run = 0;
    for (int i = 0; i < k; ++i) run += a[i];      // seed the first window
    long long best = run;
    for (int r = k; r < (int)a.size(); ++r) {
        run += a[r] - a[r - k];                   // add entering, drop leaving
        best = max(best, run);
    }
    return best;
}
```

**Key insight.** One statement replaces a rescan: `a[r] - a[r-k]`. That is the entire fixed-window
idea, and every other fixed-window problem is this with a different "state".

**If you also want the starting index**, initialise it to 0 before the loop — not left unset. See
the bugs section; `ques.cpp` returns garbage whenever the first window is already the best.

For **LC 643** (maximum average) return `best / (double)k`; do the division once at the end, never
inside the loop.

**Complexity.** O(n) time, O(1) space.

**Verified:** `{7,1,2,5,8,4,9,3,6}` k=3 → 21; `{9,9,1,1,1}` k=2 → 18 (first window is the best).

---

## 3. First negative in every window of size K — **`[deque]`**

**Approach.** Keep a queue of the *indices* of negative numbers currently in the window. The
front is the first negative; if the queue is empty the answer is 0.

```
dq = empty deque of indices
for r in 0 .. n-1:
    if a[r] < 0:                       dq.push_back(r)
    if dq not empty and dq.front() <= r - k:  dq.pop_front()      # expired
    if r >= k-1:
        answer[r-k+1] = dq.empty() ? 0 : a[dq.front()]
```

**Key insight.** Only negatives ever go in the deque, so it is never longer than the count of
negatives in the window, and each index enters and leaves once → **O(n)**.

Both failure modes of the naive approach are handled here: a window containing **no** negative
(the deque empties, answer 0) and a stale index from an earlier window (popped at the front).

**Complexity.** O(n) time, O(k) space.

**Verified:** `{2,-3,4,4,-7,-1,4,-2,6}` k=4 → `-3 -3 -7 -7 -7 -1`; `{-1,2,3,4,5,6}` k=3 →
`-1 0 0 0`; `{1,2,3,4}` k=2 → `0 0 0`.

---

## 4. Minimum Size Subarray Sum — LeetCode 209 — **fundamental, code given**

This is the **shortest-window** template. Learn the shape, not the problem.

```cpp
int minSubArrayLen(int target, vector<int>& a) {
    int l = 0, best = INT_MAX;
    long long run = 0;
    for (int r = 0; r < (int)a.size(); ++r) {
        run += a[r];
        while (run >= target) {              // while still VALID
            best = min(best, r - l + 1);     // record BEFORE shrinking
            run -= a[l++];
        }
    }
    return best == INT_MAX ? 0 : best;
}
```

**Key insight — where the recording goes.** For a *shortest* window you record **inside** the
`while`, because the current window is valid and the next shrink is what might break it. For a
*longest* window (#5) you record **after** the `while`, because that is when the window has just
become valid again. Getting this backwards is the single most common sliding-window bug.

`best == INT_MAX ? 0 : best` handles "no such subarray" — the problem wants 0, not `INT_MAX`.

**This only works because all values are positive** (the constraints guarantee it). With
negatives allowed the problem becomes LC 862 (§3 #22) and needs a deque over prefix sums.

**Complexity.** O(n) time, O(1) space.

**Verified:** `({2,3,1,2,4,3}, 7) → 2`; `({1,1,1,1}, 11) → 0`; `({1,4,4}, 4) → 1`.

---

## 5. Longest Substring Without Repeating Characters — LeetCode 3

**Approach.** The **longest-window** mirror of #4. Track the last index at which each character
was seen; when the incoming character was last seen *inside* the current window, jump `l` past it.

```
last[c] = -1 for all c
l = 0, best = 0
for r in 0 .. n-1:
    c = s[r]
    if last[c] >= l:   l = last[c] + 1        # jump, never step
    last[c] = r
    best = max(best, r - l + 1)               # record AFTER the fix-up
```

**Key insight.** The guard must be `last[c] >= l`, not `last[c] != -1`. On `"abba"`, the final
`'a'` was last seen at index 0 but `l` has already moved to 2 — without the `>= l` test you would
drag `l` *backwards* and report 3 instead of 2. **The left edge must never move backwards.**

Full dry run in `../09_Strings/solution.md` #6.

**Complexity.** O(n) time, O(1) space (a 256-slot array).

**Verified:** `"abcabcbb"→3`, `"abba"→2`.

---

## 6. Max Consecutive Ones III — LeetCode 1004

**Approach.** Longest window containing at most `k` zeros.

```
l = 0, zeros = 0, best = 0
for r in 0 .. n-1:
    if a[r] == 0: zeros++
    while zeros > k:                          # INVALID
        if a[l] == 0: zeros--
        l++
    best = max(best, r - l + 1)
```

**Key insight.** "At most k violations" is the workhorse variable-window shape. The state is a
single counter, and the constraint is monotone — one more element can only increase `zeros` — so
the window is valid.

**Complexity.** O(n) time, O(1) space.

---

## 7. Longest Repeating Character Replacement — LeetCode 424

**Approach.** A window is valid when `(window length) − (count of the most frequent character) ≤ k`,
since that difference is how many characters you would have to replace.

```
freq[26] = 0, l = 0, maxFreq = 0, best = 0
for r in 0 .. n-1:
    freq[s[r]]++
    maxFreq = max(maxFreq, freq[s[r]])        # NEVER recomputed on shrink
    while (r - l + 1) - maxFreq > k:
        freq[s[l]]--
        l++
    best = max(best, r - l + 1)
```

**Key insight — the part that looks wrong and isn't.** `maxFreq` is never decreased when the
window shrinks, so it can be **stale**, i.e. larger than the true maximum frequency inside the
current window. That is fine: a stale-high `maxFreq` only ever makes the window look *more*
valid, so the window can never shrink when it should have. And since `best` only grows, a window
that was accepted on a stale value cannot beat the genuine best window that produced that value
in the first place.

Being able to explain *why* the stale value is safe is what this problem is testing.

**Complexity.** O(n) time, O(1) space.

---

## 8. Fruit Into Baskets — LeetCode 904 `[hash]`

**Approach.** Longest window with **at most 2 distinct** values. State is a `map value → count`;
the window is invalid when the map has more than 2 keys.

```
for r in 0 .. n-1:
    cnt[a[r]]++
    while cnt.size() > 2:
        if --cnt[a[l]] == 0:  cnt.erase(a[l])     # erase, or size() stays wrong
        l++
    best = max(best, r - l + 1)
```

**Key insight.** **Erase the key when its count hits zero.** `cnt.size()` is the validity test, so
leaving a zero-count entry behind makes the window look invalid forever. This one line is the
whole problem.

Generalises directly to #16 (at most **k** distinct) by changing `2` to `k`.

> **In scope without a map:** if the values are bounded (LeetCode guarantees them, but not
> small), use `int cnt[N]` plus a separate `distinct` counter incremented when a count goes
> 0→1 and decremented when it goes 1→0. That version is O(1) per step and is worth writing.

**Complexity.** O(n) time, O(1) space (at most 3 keys).

**Verified:** `{1,2,1}→3`, `{1,2,3,2,2}→4`.

---

# Section 2 — Important

## 9–10. Permutation in String (567) / Find All Anagrams (438)

**Approach.** Fixed window of length `|p|`; state is a 26-slot frequency array. Slide, and compare
against the pattern's frequency array.

```
need[26] from p;  have[26] = 0
for r in 0 .. n-1:
    have[s[r]]++
    if r >= |p|:  have[s[r - |p|]]--          # drop the element leaving
    if r >= |p| - 1 and have == need:  record
```

**Key insight.** The 26-way array comparison looks like a nested loop but is **O(1)** — 26 is a
constant — so the whole algorithm is O(n). To make it genuinely O(1) per step, maintain a
`matches` counter of how many of the 26 letters currently agree, updating it as counts change.

#10 is #9 with "record every position" instead of "return on the first".

**Complexity.** O(n · 26) = O(n) time, O(1) space.

Full code in `../09_Strings/solution.md` #26 — verified there, including the match-at-the-very-end
case that the `r >= |p| - 1` guard exists for.

---

## 11. Binary Subarrays With Sum — LeetCode 930

**Approach.** `exactly(goal) = atMost(goal) − atMost(goal − 1)`.

```
atMost(g):
    if g < 0: return 0
    l = 0, run = 0, res = 0
    for r in 0 .. n-1:
        run += a[r]
        while run > g:  run -= a[l++]
        res += (r - l + 1)                    # every subarray ENDING at r
    return res
```

**Key insight, two parts.** "Exactly k" is not a monotone constraint, so no single window can
express it — but "at most k" is, so subtract two of them. And `res += (r - l + 1)` counts all
valid subarrays ending at `r` in one statement rather than a loop.

The `if (g < 0) return 0` guard matters for `goal = 0`, where `atMost(-1)` is called.

**Complexity.** O(n) time, **O(1) space** — better than the prefix+map solution in
`../13_prefixSum` §17.

---

## 12. Grumpy Bookstore Owner — LeetCode 1052 — **trick, code given**

```cpp
int maxSatisfied(vector<int>& c, vector<int>& g, int m) {
    int base = 0;
    for (size_t i = 0; i < c.size(); ++i) if (!g[i]) base += c[i];   // kept regardless
    int gain = 0, best = 0;
    for (int r = 0; r < (int)c.size(); ++r) {
        if (g[r]) gain += c[r];                                      // only grumpy minutes
        if (r >= m && g[r - m]) gain -= c[r - m];
        best = max(best, gain);
    }
    return base + best;
}
```

**Key insight — decompose before you slide.** The answer splits into two independent pieces:

1. **`base`** — customers served in non-grumpy minutes. You get these no matter where the window
   goes, so they are not part of the optimisation at all.
2. **`best`** — the best fixed window of size `m` over the *grumpy* minutes only. That is what the
   technique actually buys you.

Sliding a window over the raw totals double-counts the non-grumpy minutes inside the window and
gets the wrong answer. Recognising that the objective decomposes is the transferable move; the
window itself is then the trivial #1 template.

**Complexity.** O(n) time, O(1) space.

**Verified:** `c={1,0,1,2,1,1,7,5}, g={0,1,0,1,0,1,0,1}, m=3 → 16`; a never-grumpy case → the
plain total.

---

## 13. Subarrays with K Different Integers — LeetCode 992 `[hash]`

**Approach.** `exactly(k) = atMost(k) − atMost(k−1)`, where `atMost` is #8 generalised.

**Key insight.** This is rated Hard and becomes routine once #8 and #11 are done — which is
exactly why they are ordered that way. The direct "exactly k" window does not exist; do not spend
time looking for it.

**Complexity.** O(n) time, O(k) space.

---

## 14. Substrings Containing All Three Characters — LeetCode 1358

**Approach.** For each right end, find the **furthest left** start that still contains all of
`a`, `b`, `c`; every start at or before it also works, so add `last + 1` to the count.

```
last[3] = {-1,-1,-1}                          # last index of each character
for r in 0 .. n-1:
    last[s[r]] = r
    res += 1 + min(last[a], last[b], last[c]) # 0 if any is still -1
```

**Key insight.** No explicit window is needed — the minimum of the three last-seen positions
*is* the window's left boundary. When any character has not appeared, `min` is −1 and the term
contributes 0, which is exactly right.

**Complexity.** O(n) time, O(1) space.

---

## 15. Count Number of Nice Subarrays — LeetCode 1248

**Approach.** Replace "sum" with "count of odd numbers" and apply either the prefix+count method
or `atMost(k) − atMost(k−1)`.

Solved with prefix counts in `../13_prefixSum/solution.md` §16 — verified there. Worth doing
**both ways** here specifically to feel where the two techniques overlap: prefix works because
the count is a running total; the window works because the count is monotone.

---

## 16. Longest Substring with At Most K Distinct — LeetCode 340 `[hash]`

#8 with `2` replaced by `k`. Same erase-on-zero rule.

---

# Section 3 — Good to Know

## 17. Sliding Window Maximum — LeetCode 239 — **fundamental structure, code given**

```cpp
vector<int> maxSlidingWindow(vector<int>& a, int k) {
    deque<int> dq;                                                   // INDICES, values decreasing
    vector<int> res;
    for (int r = 0; r < (int)a.size(); ++r) {
        if (!dq.empty() && dq.front() <= r - k) dq.pop_front();      // expired
        while (!dq.empty() && a[dq.back()] <= a[r]) dq.pop_back();   // dominated
        dq.push_back(r);
        if (r >= k - 1) res.push_back(a[dq.front()]);
    }
    return res;
}
```

**Key insight — "dominated".** If `a[r] >= a[j]` for some `j` already in the deque, then `j` can
never be the maximum of any future window: it is beaten now, and it expires *earlier* than `r`.
So discard it permanently. That is why the total work is O(n) despite the inner `while` — each
index is pushed once and popped once.

**Store indices, not values.** Expiry is `dq.front() <= r - k`, which needs the position; with
values alone you cannot tell which copy just left the window.

`<=` rather than `<` in the pop condition keeps the deque smaller on ties and is still correct.

**Complexity.** O(n) time, O(k) space. A max-heap gives O(n log k) — a fine second answer.

**Verified:** `{1,3,-1,-3,5,3,6,7}` k=3 → `3 3 5 5 6 7`; k=1 → the array itself; k = n → the
single maximum.

---

## 18. Minimum Window Substring — LeetCode 76

**Approach.** Shortest-window template (#4) with a frequency map as state and a `formed` counter
so validity is O(1).

```
need[c] = counts from t;  required = number of distinct chars in t
have = empty map;  formed = 0
l = 0;  best = (INF, 0)
for r in 0 .. n-1:
    have[s[r]]++
    if have[s[r]] == need[s[r]]:  formed++          # == , not >=
    while formed == required:                        # window is VALID
        record (r - l + 1, l) if shorter             # record BEFORE shrinking
        have[s[l]]--
        if have[s[l]] < need[s[l]]:  formed--
        l++
return the recorded substring
```

**Key insight — the `formed` counter.** Comparing the whole frequency map on every step would be
O(n · alphabet). Instead, `formed` counts how many *distinct* characters have reached their
required count, and it changes only on the exact transition — `have == need` when adding,
`have < need` when removing. Using `>=` instead of `==` when incrementing would over-count on
repeated characters and is the standard bug.

Note this is the shortest-window form, so the recording is **inside** the `while`.

**Complexity.** O(n + m) time, O(alphabet) space.

---

## 19. Longest Subarray with Absolute Diff ≤ Limit — LeetCode 1438 `[deque]`

**Approach.** Longest-window template with **two** monotonic deques: one decreasing (window
maximum, exactly #17) and one increasing (window minimum). The window is invalid when
`maxDq.front() - minDq.front() > limit`.

```
while a[maxDq.front()] - a[minDq.front()] > limit:
    if maxDq.front() == l: maxDq.pop_front()
    if minDq.front() == l: minDq.pop_front()
    l++
```

**Key insight.** You cannot maintain the max of a *shrinking-from-the-left* window with a plain
variable, for the same reason as #17. Two deques give both extremes in O(1), and the shrink step
must pop from whichever deque was pointing at the departing index.

**Complexity.** O(n) time, O(n) space.

---

## 20. Maximum Points From Cards — LeetCode 1423 — **trick**

**Approach.** You take `k` cards from the two ends, so the taken cards are *not* contiguous — but
the `n − k` cards you leave behind **are**. Invert the problem.

```
total = sum(all)
w = n - k
if w == 0: return total
minWindow = minimum sum over all fixed windows of size w      # the #1 template
return total - minWindow
```

**Key insight.** Maximising a non-contiguous selection becomes minimising a contiguous one.
Reframing so the sliding window applies at all is the entire difficulty; once reframed it is
problem #1.

Guard `w == 0` (you take every card) — otherwise the window loop never runs and `minWindow`
stays at its sentinel.

**Complexity.** O(n) time, O(1) space.

---

## 21. Minimum K Consecutive Bit Flips — LeetCode 995

**Approach.** Greedy left to right: whenever the current position reads 0 after accounting for
earlier flips, you *must* flip the window starting here. Track the parity of active flips with a
difference array (`../13_prefixSum` §14) so each step is O(1).

```
flipped[n+1] = 0;  active = 0;  count = 0
for i in 0 .. n-1:
    active ^= flipped[i]                       # flips whose effect starts here
    if (a[i] ^ active) == 0:                   # still a 0 -> must flip
        if i + k > n: return -1                # no room
        count++
        active ^= 1
        flipped[i + k] ^= 1                    # schedule the expiry
```

**Key insight.** The greedy choice is forced — the leftmost 0 can only be fixed by the window
starting at it — so no search is needed. The difference array turns "which flips still cover me?"
from an O(k) scan into an XOR.

**Complexity.** O(n) time, O(n) space.

---

## 22. Shortest Subarray with Sum at Least K — LeetCode 862 — **why #4 breaks**

**Approach.** Build prefix sums, then keep a deque of prefix **indices with increasing values**.

```
pre[0..n] = prefix sums
dq = empty
for r in 0 .. n:
    while dq not empty and pre[r] - pre[dq.front()] >= k:
        best = min(best, r - dq.pop_front())        # this start can never do better later
    while dq not empty and pre[dq.back()] >= pre[r]:
        dq.pop_back()                                # dominated: later AND larger
    dq.push_back(r)
```

**Key insight — this is #4 with negatives allowed, and the plain window is simply invalid.**
With a negative element, shrinking from the left can *increase* the sum, so "the window is big
enough, shrink it" is unsound.

Two deque rules replace it:

- **Pop the front when the condition is met.** That start index has found its shortest valid end;
  any later end gives a longer subarray, so it is finished forever.
- **Pop the back while `pre[back] >= pre[r]`.** A later index with a smaller-or-equal prefix
  dominates: it produces a larger sum *and* a shorter subarray for every future `r`.

Do this immediately after #4. The pair is the clearest demonstration of the precondition in
`readme.md` §1.

**Complexity.** O(n) time, O(n) space.

---

# Section 4 — approach only

- **Contains Duplicate II (219)** — fixed window of size k holding a set; add on the right, erase
  on the left.
- **Max Consecutive Ones (485)** — not a window at all: a run counter reset on 0. Included as a
  contrast — reaching for a window here is over-engineering.
- **Defuse the Bomb (1652)** — circular fixed window; index with `(i + j) % n` and handle `k < 0`
  by walking backwards.
- **Maximum Erasure Value (1695)** — #5's longest-distinct window, carrying a running sum
  alongside the last-seen array.
- **Replace the Substring for Balanced String (1234)** — like #20, the window is what you
  *replace*; the constraint is on everything **outside** it.
- **Frequency of the Most Frequent Element (1838)** — sort first; the window is valid while
  `a[r] * len - windowSum <= k`. The sort is what makes it a window problem.
- **Longest Nice Subarray (2401)** — window state is a **bitmask** of the OR of the window; a new
  element is admissible iff `(mask & a[r]) == 0`. Ties to `../15_bitwise`.
- **Sliding Window Median (480)** — two heaps with lazy deletion; needs `../25_heap`.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `14_Sliding window/` was edited.**

## `ques.cpp:6` — `idx` is read uninitialised

```cpp
int maxSum=INT16_MIN,idx;          // idx never initialised
...
maxSum=prevSum;                    // first window recorded, but idx is NOT set
while(j<n){
    ...
    if(maxSum<prevSum){ maxSum=prevSum; idx=i; }
}
cout<<idx<<"  "<<maxSum;
```

`idx` is only assigned when a *later* window beats the first one. If the first window is already
the best, it is printed without ever having been written.

**Verified: `ques1({9,9,1,1,1}, n=5, k=2)` prints `idx=73`.** The correct answer is 0. Your own
test array happens to peak in the middle, which is why it looked fine — `ques1({7,1,2,5,8,4,9,3,6}, k=3)`
correctly prints `idx=4  maxSum=21`.

**Fix:** `int idx = 0;` — the first window is the incumbent, so index 0 is the right seed.

Two smaller notes on the same function:

- `INT16_MIN` is −32768. It is immediately overwritten by `maxSum = prevSum`, so it does no harm
  here, but as a sentinel for a sum of `int`s it is far too small. Use `LLONG_MIN`, or better,
  seed with the first window as you already do and drop the sentinel.
- `prevSum` should be `long long`. With `n = 10⁵` elements near `INT_MAX` the window sum
  overflows.

## `ques.cpp:30` — `ques2` reads a stale or invalid index when a window has no negative

```cpp
int prvIdx=-1;
for (int i =0;i<k;i++){
    if(arr[i]<0){ prvIdx=i; ans[0]=arr[prvIdx]; break; }   // ans[0] set ONLY inside the if
}
...
ans[i]=arr[prvIdx];                                        // prvIdx may be -1, or expired
```

Two related faults:

1. **`ans[0]` is never assigned** when the first window contains no negative.
2. When a *later* window contains no negative, the inner rescan finds nothing, `prvIdx` keeps
   pointing at an index that has already left the window, and `arr[prvIdx]` is written anyway.
   With `prvIdx == -1` that is an out-of-bounds read.

**Verified.** On your own input it is correct — `{2,-3,4,4,-7,-1,4,-2,6}` with k=4 gives
`-3 -3 -7 -7 -7 -1`, all six right. But:

| Input | k | Your output | Correct |
|---|---|---|---|
| `{-1,2,3,4,5,6}` | 3 | `-1 -1 -1 -1` | `-1 0 0 0` |
| `{1,2,3,4}` | 2 | `16 4199136 4199136` | `0 0 0` |

The second row is uninitialised memory being printed.

**Fix:** the deque version in #3 — it handles both cases structurally rather than by patching.

Also: the trailing comment `// -3,-3,-7,-7,-7,-1,-2` lists **seven** values. With n=9 and k=4
there are only `9-4+1 = 6` windows, and the code prints six. The code is right and the comment
has one value too many.

`int ans[n-k+1]` is a variable-length array — a GCC extension, not standard C++. `vector<int>`
is the portable form. Same note as `../07_Array`.

---

## What you got right

- **`ques2`'s core idea is the right one** and it is not obvious: rather than rescanning every
  window, remember the index of the last negative found and only rescan when that index falls
  out of the window (`if(prvIdx<i)`). That is the deque optimisation in embryo — you reached for
  amortisation rather than brute force. The deque version in #3 is the same idea made total, by
  keeping *all* the candidate indices instead of just one.

- **`ques1`'s rolling update is exactly right.** `prevSum -= arr[i-1]; prevSum += arr[j];` is the
  add-one/drop-one step that makes a fixed window O(n), and building the first window separately
  before the loop is the correct structure. The only defect is the uninitialised `idx`.

- **Seeding `maxSum = prevSum` after the priming loop** rather than trusting the sentinel is the
  more robust choice, and it is why the `INT16_MIN` mistake is harmless.

- **Your `readme.md` pair is well chosen.** 209 and 1052 are the variable-window template and the
  decompose-then-window trick — two genuinely different lessons rather than two variations of one.
