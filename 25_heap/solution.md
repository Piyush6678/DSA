# 25 — Heaps: Solutions

Approach and pseudocode first. Full C++ only for **fundamentals**, **implementations**, **hard**
problems and **tricks**. Everything shown has been compiled and run.

---

## Section 1 — Must Do

### 1. The `priority_queue` drill *(fundamental)*

```cpp
priority_queue<int> maxH;                                 // MAX by default
priority_queue<int, vector<int>, greater<int> > minH;      // MIN needs all three arguments

int a[] = {10, 2, -5, 15};
for (int i = 0; i < 4; ++i) { maxH.push(a[i]); minH.push(a[i]); }
maxH.top();          // 15
minH.top();          // -5
int x = maxH.top(); maxH.pop();      // pop() returns void -- read top() FIRST
maxH.top();          // 10
// maxH is now {10, 2, -5}; there is no way to ask "is 2 in here" faster than O(n)
```

Your `basic.cpp` already prints `15` then `-5`, which is exactly right. Add to it: pop everything
into a vector and confirm the order, then check `empty()` before `top()` — calling `top()` on an
empty queue is **undefined behaviour**, not an exception.

---

### 2. Last Stone Weight — LC 1046 *(fundamental)*

```cpp
int lastStoneWeight(vector<int>& stones) {
    priority_queue<int> pq(stones.begin(), stones.end());   // O(n) build, one line
    while (pq.size() > 1) {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        if (a != b) pq.push(a - b);          // equal stones both vanish
    }
    return pq.empty() ? 0 : pq.top();
}
```

The `if (a != b)` is the only subtlety — pushing a 0 would work too but leaves junk in the heap.
**The empty check at the end is required**: `[2,2]` destroys everything. Verified: `[2,7,4,1,8,1]`
→ 1, `[2,2]` → 0, `[1]` → 1.

The range constructor is O(n) heapify, not n pushes.

---

### 3. Kth Largest Element — LC 215 *(fundamental — and the inversion to memorise)*

```cpp
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int> > minH;   // MIN heap for k LARGEST
    for (size_t i = 0; i < nums.size(); ++i) {
        minH.push(nums[i]);
        if ((int)minH.size() > k) minH.pop();               // evict the weakest survivor
    }
    return minH.top();
}
```

**k largest → min heap. k smallest → max heap.** The heap holds the k best so far, and its top is
the *worst* of them — the one to throw away when something better arrives. That is why the type is
inverted, and it is the single fact this problem exists to teach.

**O(n log k), O(k) space.** Verified: `[3,2,1,5,6,4]` k=2 → 5, with duplicates k=4 → 4.

Three alternatives worth naming:

| Method | Time | When |
|---|---|---|
| sort, index `n-k` | O(n log n) | k close to n, or you need the rest anyway |
| **size-k heap** | O(n log k) | k ≪ n, or a **stream** |
| **quickselect** | **O(n) average**, O(n²) worst | fixed array in memory — the fastest |

Quickselect is the expected follow-up: partition as in quicksort, but recurse into **only** the
side containing the target index. It reuses `../12_sorting`'s partition exactly.

*(Your `ques.cpp` `main()` does both directions of this with a size-k heap and both answers are
correct — 10 for the 3rd smallest and 24 for the 3rd largest.)*

---

### 4. K Closest Points to Origin — LC 973

#3 with a computed key. **Do not call `sqrt`** — it is monotonic, so it cannot change the ordering,
and it costs a floating-point operation per point.

```cpp
priority_queue< pair<long long,int> > maxH;                 // (distSquared, index) -- MAX heap
for (int i = 0; i < (int)pts.size(); ++i) {
    long long d = (long long)pts[i][0]*pts[i][0] + (long long)pts[i][1]*pts[i][1];
    maxH.push(make_pair(d, i));
    if ((int)maxH.size() > k) maxH.pop();                   // drop the FARTHEST
}
```

**k *closest* → max heap** — the same inversion as #3, in the other direction. `pair` needs no
comparator because it already orders by `.first`. **`long long`**, because coordinates up to 10⁴
square to 10⁸ and two of them overflow nothing yet — but the habit is what saves you when they do.

Quickselect on distance is O(n) average and is the follow-up here too. Verified against both
LeetCode samples.

---

### 5. Top K Frequent Elements — LC 347 *(the problem where the heap loses)*

Count with a map, then select. The heap version is #3 over `(count, value)` pairs — O(n log k).

**The bucket version is O(n)** and is what they want:

```cpp
vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int,int> freq;
    for (size_t i = 0; i < nums.size(); ++i) freq[nums[i]]++;
    vector<vector<int> > bucket(nums.size() + 1);            // a count cannot exceed n
    for (unordered_map<int,int>::iterator it = freq.begin(); it != freq.end(); ++it)
        bucket[it->second].push_back(it->first);
    vector<int> out;
    for (int f = (int)bucket.size() - 1; f >= 1 && (int)out.size() < k; --f)
        for (size_t j = 0; j < bucket[f].size() && (int)out.size() < k; ++j)
            out.push_back(bucket[f][j]);
    return out;
}
```

**The counts are bounded by n, so they can index an array** — no comparison-based selection needed
at all. That is the whole idea, and it is the same reason counting sort beats comparison sorts on
bounded input (`../12_sorting`). Verified: `[1,1,1,2,2,3]` k=2 → `{1,2}`.

---

### 6. Heapify in O(n) — *(implementation; `heapify.cpp` does not work)*

```cpp
void siftDownMax(vector<int>& a, int i, int n) {      // n = logical size
    while (true) {
        int l = 2*i + 1, r = 2*i + 2, big = i;        // 0-INDEXED children
        if (l < n && a[l] > a[big]) big = l;
        if (r < n && a[r] > a[big]) big = r;
        if (big == i) return;                          // already in place
        swap(a[i], a[big]);
        i = big;                                       // local i, NOT the caller's loop variable
    }
}
void buildMaxHeap(vector<int>& a) {
    for (int i = (int)a.size()/2 - 1; i >= 0; --i) siftDownMax(a, i, a.size());
}
```

Three decisions:

- **`n/2 - 1` is the last node with a child** (0-indexed). Leaves need no work.
- **Bottom-up sift *down* is O(n).** Pushing elements one at a time is O(n log n). Most nodes are
  near the bottom, so `Σ (n/2^h)·h ≤ 2n`.
- **Compare against the *smaller* (here larger) child**, not the first one that qualifies.
  Otherwise the swap leaves the sibling violated.

Verified: `{9,4,7,1,-2,6,5}` and an ascending `{1,2,3,4,5}` both become valid max heaps with the
maximum at index 0; single-element and empty inputs are handled.

---

### 7. Heap sort, in place — *(implementation; `heap_sort.cpp` throws its answer away)*

```cpp
void heapSort(vector<int>& a) {                       // NOTE the reference
    buildMaxHeap(a);
    for (int end = (int)a.size() - 1; end > 0; --end) {
        swap(a[0], a[end]);                            // the max goes to its final position
        siftDownMax(a, 0, end);                        // heap now covers only [0, end)
    }
}
```

**O(n log n) time, O(1) extra space, not stable.** The array is split into "heap prefix" and
"sorted suffix", and the suffix grows by one each iteration. A max heap produces **ascending**
order — which surprises people; the largest element is placed last.

Verified: `{5,1,4,2,8}` → `1 2 4 5 8`, duplicates, negatives, single and empty all correct.

Your version uses a `priority_queue` and copies back, which is a valid *sorting* algorithm but not
*heap sort* — it costs O(n) extra space and hides the mechanism that makes heap sort interesting.
Write both and compare.

---

## Section 2 — Important

### 8. Implement a min heap — *(implementation; `implementationWuthArray.cpp` does not compile)*

```cpp
class MinHeap {
    vector<int> a;                                    // a[0] unused; root at index 1
public:
    MinHeap() { a.push_back(0); }
    bool empty() const { return a.size() == 1; }
    int  size()  const { return (int)a.size() - 1; }
    int  top()   const { return a[1]; }               // caller must check empty()
    void push(int v) {
        a.push_back(v);
        int i = a.size() - 1;
        while (i > 1 && a[i/2] > a[i]) { swap(a[i/2], a[i]); i /= 2; }        // sift UP
    }
    void pop() {
        if (empty()) return;
        a[1] = a.back(); a.pop_back();                                        // last -> root
        int i = 1, n = a.size();
        while (true) {                                                        // sift DOWN
            int l = 2*i, r = 2*i + 1, small = i;
            if (l < n && a[l] < a[small]) small = l;
            if (r < n && a[r] < a[small]) small = r;
            if (small == i) return;
            swap(a[i], a[small]);
            i = small;
        }
    }
};
```

**A `vector` instead of `int arr[101]`** removes the fixed capacity, which your version silently
overflows past 100 elements. Keeping index 0 unused preserves the clean `i/2`, `2i`, `2i+1`
arithmetic your file's opening comment describes.

Verified: pushing `7,3,9,1,8,2` pops as `1 2 3 7 8 9`; `pop()` on an empty heap is a safe no-op.

---

### 9. Minimum cost to connect ropes *(trick — the answer is the accumulator)*

```cpp
int minCostConnectRopes(vector<int>& v) {
    if (v.size() < 2) return 0;
    priority_queue<int, vector<int>, greater<int> > pq(v.begin(), v.end());
    int cost = 0;
    while (pq.size() > 1) {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        cost += a + b;                 // <-- THE ANSWER. the push is just bookkeeping.
        pq.push(a + b);
    }
    return cost;
}
```

**The greedy argument:** every merge adds its combined length to the total, and a rope's length is
counted once per merge it takes part in. Short ropes should therefore participate most, which means
merging the two shortest at every step. That justification is what an interviewer is listening for
— the code is six lines.

Verified: `{4,3,2,6}` → **29** (2+3=5, 4+5=9, 9+6=15; total 29), `{1,2,3,4,5}` → 33, single rope →
0, `{1,2}` → 3.

**Note the objective matters.** "Minimise the *maximum* rope at any point" is a different problem
with a different greedy — worth thinking about for five minutes.

---

### 10. Sort a nearly sorted (k-sorted) array

Each element is at most `k` positions from its sorted place, so **once you hold `k+1` candidates,
the smallest of them is definitely the next output.**

```cpp
void sortKSorted(vector<int>& v, int k) {
    priority_queue<int, vector<int>, greater<int> > minH;
    int idx = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        minH.push(v[i]);
        if ((int)minH.size() > k + 1) { v[idx++] = minH.top(); minH.pop(); }
    }
    while (!minH.empty()) { v[idx++] = minH.top(); minH.pop(); }              // drain
}
```

**O(n log k)** instead of O(n log n), and O(k) space. The heap is a sliding window of size `k+1`
(`../14_Sliding window`). **The drain loop at the end is not optional** — the last `k+1` elements
are still inside.

Verified: `{6,5,3,2,8,10,9}` with k=3 → `2 3 5 6 8 9 10`; `{10,9,8,7,4,70,60,50}` with k=4 →
`4 7 8 9 10 50 60 70`.

---

### 11. Find K Closest Elements — LC 658 *(where the heap is the worse answer)*

Heap version: a max heap of size k keyed on `(|x - target|, value)` — O(n log k).

**But the array is sorted**, so binary search for the left end of the window:

```
lo = 0, hi = n - k
while lo < hi:
    mid = lo + (hi - lo) / 2
    if x - a[mid] > a[mid + k] - x:  lo = mid + 1      # window is too far left
    else:                            hi = mid
return a[lo .. lo+k-1]
```

**O(log(n−k) + k).** The comparison `x - a[mid] > a[mid+k] - x` asks "would sliding the window one
step right be better?" — searching over *window positions*, not values. That reframing is the
lesson, and it is `../11_linearAndBinarySearch` §binary-search-on-answers.

---

### 12. Sort Array by Increasing Frequency — LC 1636

Count with a map, then sort with a comparator: **frequency ascending, and for equal frequency,
value descending.**

```
sort(v.begin(), v.end(), [&](int a, int b){
    if (freq[a] != freq[b]) return freq[a] < freq[b];
    return a > b;                                     // note: DESCENDING on ties
});
```

The tie rule is the whole problem and it is easy to get backwards — read the statement twice.
(Lambdas are C++11 and fine on this toolchain; a functor struct works identically.)

---

### 13. Kth Smallest in a Sorted Matrix — LC 378

Three solutions, increasing in quality:

| Method | Time |
|---|---|
| size-k max heap over all n² entries | O(n² log k) |
| k-way merge — min heap over the n rows, pop k times | O(k log n) |
| **binary search on the answer value** | **O(n log(max − min))** |

The third: guess a value `mid`, count how many matrix entries are `≤ mid` by walking the staircase
from the bottom-left in O(n), and binary search on the guess. **It never stores a heap at all**, and
it is the same "binary search on the answer" technique as `../11_linearAndBinarySearch`'s ship /
Koko problems.

---

### 14. Merge k Sorted Lists — LC 23 *(implementation — the k-way merge)*

```cpp
struct Item { int val, list, idx; };
struct ItemGreater { bool operator()(const Item& a, const Item& b) const { return a.val > b.val; } };

vector<int> mergeKSorted(vector<vector<int> >& lists) {
    priority_queue<Item, vector<Item>, ItemGreater> pq;
    for (int i = 0; i < (int)lists.size(); ++i)
        if (!lists[i].empty()) { Item it = {lists[i][0], i, 0}; pq.push(it); }
    vector<int> out;
    while (!pq.empty()) {
        Item cur = pq.top(); pq.pop();
        out.push_back(cur.val);
        if (cur.idx + 1 < (int)lists[cur.list].size()) {
            Item nxt = {lists[cur.list][cur.idx+1], cur.list, cur.idx+1};
            pq.push(nxt);
        }
    }
    return out;
}
```

**O(N log k)**, and the heap never exceeds k entries — which is the point. Note `operator>` in the
comparator to get a *min* heap: the comparator is inverted relative to `sort`.

**Divide and conquer is the alternative** — merge lists pairwise in rounds, `log k` rounds of O(N)
work each. Same complexity, no heap, and it is what `../12_sorting`'s merge sort already taught you.
Merging one at a time into an accumulator is O(N·k) and is the trap.

Verified on three lists and on a single empty list.

---

### 15. Kth Largest in a Stream — LC 703

#3 as a class: keep a min heap of size k forever; `add(v)` pushes, pops if the size exceeds k, and
returns `top()`. O(log k) per call.

The design point is that **the heap never grows beyond k**, so the memory is bounded no matter how
long the stream runs. That is the answer to "what if there are a billion numbers".

---

## Section 3 — Good to Know

### 16. Median from a Data Stream — LC 295 *(hard — the two-heap pattern)*

```cpp
class MedianFinder {
    priority_queue<int> lo;                                  // MAX heap -- smaller half
    priority_queue<int, vector<int>, greater<int> > hi;       // MIN heap -- larger half
public:
    void addNum(int num) {
        lo.push(num);
        hi.push(lo.top()); lo.pop();                         // funnel: guarantees lo.top() <= hi.top()
        if (hi.size() > lo.size()) { lo.push(hi.top()); hi.pop(); }
    }
    double findMedian() {
        if (lo.size() > hi.size()) return lo.top();
        return (lo.top() + hi.top()) / 2.0;
    }
};
```

**The funnelling insert is what makes it correct.** Pushing straight into whichever heap "looks
right" by comparing against a top is a common variant and gets the boundary wrong; pushing into
`lo`, moving `lo`'s top into `hi`, then rebalancing back is unconditionally correct in three lines.

Invariant: `lo.size()` is equal to `hi.size()` or one greater. O(log n) insert, **O(1) query**.

Verified against LeetCode's sample and against the running medians of
`6,10,2,6,5,0,6,3` → `6, 8, 6, 6, 6, 5.5, 6, 5.5`.

**Follow-ups:** all values in `[0,100]` → a 101-bucket counting array, O(1) insert; 99% in that
range → buckets plus two heaps for the outliers.

---

### 17. Task Scheduler — LC 621

Heap version: repeatedly take the `n+1` most frequent remaining tasks, decrement them, push back
what is left. Correct, and O(total · log 26).

**The formula version is O(n) and is the real answer:**

```
maxFreq = the largest count
countOfMax = how many tasks share that count
answer = max(totalTasks, (maxFreq - 1) * (n + 1) + countOfMax)
```

The picture: lay out `maxFreq - 1` full blocks of width `n+1` and put the `countOfMax` most frequent
tasks in the last row. The `max` with `totalTasks` covers the case where there are so many distinct
tasks that no idling is ever needed. **Deriving that picture is the problem**; the heap is the
fallback.

---

### 18. Meeting Rooms II — LC 253

```
sort meetings by start time
minHeap of END times
for each meeting:
    if heap is non-empty and heap.top() <= start:  heap.pop()     # a room freed up
    heap.push(end)
answer = the maximum heap size reached
```

**The heap size is the number of rooms in use.** O(n log n).

The alternative is the **sweep line**: `+1` at each start, `−1` at each end, sort all events, track
the running maximum. Same complexity, no heap, and it generalises better — GFG's "Minimum
Platforms" is the same problem.

---

### 19. Reorganize String — LC 767

**Feasibility first:** if any character's count exceeds `(n+1)/2`, it is impossible. Then greedily
place the most frequent character that is not the previous one, using a max heap on counts and
holding the just-used character back for one step.

The alternative fills even indices then odd indices with characters in descending frequency order —
O(n log 26) and no per-step heap. Worth knowing both.

---

### 20. Furthest Building — LC 1642 *(the heap of regrets)*

```
minHeap of the climbs where a LADDER was used
for each gap d > 0:
    use a ladder: push d
    if heap size > ladders:               # one ladder too many
        bricks -= heap.pop()              # convert the SMALLEST ladder use into bricks
        if bricks < 0: return here
```

**Use ladders greedily, then retroactively downgrade the cheapest one.** You cannot know in advance
which climbs deserve ladders, so you commit and undo — the heap holds the decisions you might
regret, and its top is the cheapest to reverse.

O(n log L). **This idea transfers**: LC 871 (refuelling stops) is the same shape with a max heap of
fuel you could have taken.

---

### 21. IPO — LC 502

Sort projects by capital ascending. Walk a pointer forward pushing every project whose capital
requirement you can now afford into a **max heap by profit**; take the top; repeat k times.

**Two orderings at once** — one static (sort by capital) and one dynamic (heap by profit) — because
affordability is monotonic in your capital and profitability is not. Recognising when a problem
needs both is the skill.

---

### 22. Convert a BST to a max heap — *(yours works)*

```
values = reverse in-order of the BST        # right, node, left -> DESCENDING
i = 0
pre-order the tree, writing values[i++] into each node
```

**Why it is correct:** pre-order visits every node before all of its descendants, and the values
arrive in descending order — so every node receives a value larger than everything its descendants
will receive. That is exactly the max-heap property.

The BST property is destroyed, which is expected. **The child order within the pre-order does not
matter** — root-left-right and root-right-left both work, which is why your `preOrder(root, r, l)`
is fine.

For a **min** heap: in-order (ascending) instead of reverse in-order. Two characters changed.

---

## Bugs in your files — with evidence

Compiled and executed. **Nothing in `25_heap/` was edited.**

Of the six `.cpp` files, **five compile** and one does not.

### `implementationWuthArray.cpp:9` and `:14` — a member and a method both called `top`

```cpp
int top;                     // line  9 -- the "next free index" counter
int top(){ ... }             // line 14 -- the accessor
```

**Measured:**

```
25_heap/implementationWuthArray.cpp:17:5: error: 'int minHeap::top()' conflicts with a previous
declaration
25_heap/implementationWuthArray.cpp:11:9: error: invalid use of member 'int minHeap::top()'
```

...and eleven more, all cascading from the same collision — `arr[top]`, `top++`, `top--` and
`l > top-1` each fail because the compiler resolves `top` to the function.

**The algorithm is correct.** Renaming the member to `tp` and changing nothing else, the class was
compiled and tested: pushing `7,3,9,1,8,2` pops as `1 2 3 7 8 9`, and `top()` on an empty heap
returns the `-100` sentinel as designed. **Both sift loops are right**, including the
compare-against-the-smaller-child logic. This is a naming bug, not a logic bug.

Two smaller issues once it compiles: `int arr[101]` silently overflows past 100 elements (no bound
check in `push`), and returning `-100` from `top()` is a sentinel — the same class of decision that
went wrong in `../21_tree`'s `INT16_MIN` heights. Prefer an `empty()` the caller must check.

### `heapify.cpp:7-27` — 1-indexed arithmetic on a 0-indexed vector

```cpp
for (int i =n/2;i>=1;i--){          // starts at n/2, stops at 1 -- index 0 never touched
    int l=2*i;                       // 1-INDEXED children...
    int r=2*i +1;
```

`v` is a `vector`, so it is 0-indexed and the children of `i` are at `2i+1` and `2i+2`. As written,
`v[2i]` is the *wrong node*, and `i >= 1` means **index 0 is never heapified at all**.

**Measured** on `{9,4,7,1,-2,6,5}`:

```
before : 9 4 7 1 -2 6 5
after  : 9 -2 4 1 7 6 5        <- v[0]=9 > v[1]=-2 : not a min heap
```

and on `{5,4,3,2,1}`:

```
after  : 5 1 3 2 4             <- the minimum, 1, is at index 1; v[0] is still 5
```

The minimum ends up at index **1** rather than index 0, which is the signature of exactly this bug.

### `heapify.cpp:7` — the loop variable is reused as the descent variable

```cpp
for (int i =n/2;i>=1;i--){
    while(1){ ... i=l; ... }         // the inner loop MOVES i
}
```

The sift-down writes into the same `i` the `for` loop is counting with, so after one descent the
outer loop resumes from wherever the element landed instead of from the next node up. Nodes get
skipped. Use a separate variable, or extract `siftDown` as its own function (§1 #6 above) — which
is the better fix, since `heap_sort` needs it too.

### `heap_sort.cpp:4` — the vector is passed by value

```cpp
void heapSort(vector<int>v){          // a COPY
```

Everything the function does is thrown away when it returns. **Measured:** `{5,1,4,2,8}` is still
`5 1 4 2 8` after the call.

Change to `vector<int>& v`. The logic inside is otherwise correct — filling a max heap and writing
back from `v.size()-1` downward does produce ascending order. (`<vector>` is also not included; it
arrives transitively via `<queue>` here, which is luck rather than correctness.)

### `ques.cpp:16` and `:22` — `==` where `=` was meant

```cpp
v[idx++]==(minH.top());        // comparison, discarded. idx still advances.
```

`sortKnearest` therefore **does nothing to the array** while behaving as though it worked.
**Measured** — this is the actual output of running your `ques.cpp`:

```
6
5
3
2
8
10
9
```

The input `{6,5,3,2,8,10,9}` unchanged; the correct k-sorted output is `2 3 5 6 8 9 10`.

**GCC catches this with `-Wall`.** Verified:

```
25_heap/ques.cpp:16:17: warning: value computed is not used [-Wunused-value]
         v[idx++]==(minH.top());
25_heap/ques.cpp:22:21: warning: value computed is not used [-Wunused-value]
```

`g++ -Wall ques.cpp` would have found it in a second. Worth making `-Wall` your default — the same
run also flags `minH.size()<k+1` as a signed/unsigned comparison (harmless here; cast `k`, or
compare against `(size_t)(k+1)`).

### `ques.cpp:28-45` — `MinimumcostToconnectAllRopes` returns the total length, not the cost

```cpp
while(pq.size()>1 ){
    int a =pq.top(); pq.pop();
    int b =pq.top(); pq.pop();
    pq.push(a+b);              // merges correctly...
}
return pq.top();               // ...but this is just the SUM of all the ropes
```

There is no accumulator. **Measured:**

| Input | Yours | Correct |
|---|---|---|
| `{4,3,2,6}` | **15** | **29** |
| `{1,2,3,4,5}` | **15** | **33** |
| `{5}` | **5** | **0** |

15 is `4+3+2+6` — the total length, which is what a single remaining rope must weigh regardless of
merge order. The cost is the sum of every intermediate merge: `cost += a + b` inside the loop.

Two more: the `length` variable computed at the top is never used (it is the value the function
accidentally returns), and it uses a **max** heap — the greedy needs the two **shortest** ropes, so
it must be `greater<int>`. With a max heap the merge order is wrong even once the accumulator is
added.

### `ques.cpp:47` — `bstToMaxHeap` is an empty stub

The working implementation is in `bsttoheap.cpp`; this one is a placeholder.

### `bsttoheap.cpp:10-12` — the constructor leaves `l` and `r` uninitialised

```cpp
Node(int val){ this->val=val; }        // l and r are never set
```

**Measured:** a freshly constructed `Node(50)` had `l = 0x1169d18` and `r = 0x1169e98` — non-null
garbage. Running `inOrder` on it produced:

```
exit code = 0xC00000FD          (STACK_OVERFLOW)
```

It recursed into arbitrary memory until the stack ran out. Add `l = NULL; r = NULL;` to the
constructor — your `Node` classes in `../21_tree` do this and these two files dropped it.

---

## What you got right

**`bsttoheap.cpp` is correct, and it is a genuinely clever piece of code.** Once the nodes are
built with null children, it was tested and passes: the max-heap property holds at every node and
the root holds the maximum.

What makes it good is that the two halves fit together for a reason. Reverse in-order (`right`,
node, `left`) yields the values in **descending** order. A pre-order overwrite visits every node
**before** all of its descendants. Put those together and every node necessarily receives a larger
value than anything below it — which is the max-heap property, obtained without a single
comparison or swap. Most people solve this by heapifying.

**You will meet the same reverse-in-order trick in `../22_bst` LC 1038** (greater sum tree), and
recognising that they are the same idea is worth more than either problem.

**The size-k heap logic in `ques.cpp`'s `main()` is correct in both directions**, including the
inversion that catches most people:

```cpp
priority_queue<int> pq;                                   // MAX heap for the k SMALLEST
if(pq.size()<k) pq.push(v[i]);
else if(v[i]<pq.top()){ pq.push(v[i]); pq.pop(); }
```

Verified — it prints `10` (3rd smallest) and `24` (3rd largest), matching your comments exactly.
Writing both directions next to each other is the right way to learn that inversion.

**`implementationWuthArray.cpp`'s algorithm is right**, and the opening comment
(`parent at i, children at 2i and 2i+1`) shows you chose 1-based indexing deliberately and stayed
in it throughout that file. Both sift loops handle the "only a left child" case explicitly, which
is the case people forget. The problem is the identifier `top`, not the heap.

**`basic.cpp` makes the two things that matter concrete** — that `priority_queue<int>` is a max
heap, and the exact `priority_queue<int, vector<int>, greater<int> >` incantation for a min heap.
The comment `// problem identification kth smallest /largest top k closest k` is the right note to
have written: those four phrases really are the signal that a heap is wanted, and three of the four
are in Section 1.
