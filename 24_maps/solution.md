# 24 — Maps: Solutions

Approach and pseudocode first. Full C++ only for **fundamentals**, **implementations**, **hard**
problems and **tricks**. Everything shown has been compiled and run.

---

## Section 1 — Must Do

### 1. The `operator[]` drill *(fundamental — do this before anything else)*

```cpp
unordered_map<string,int> m;
m["a"] = 1;
cout << m.size();               // 1
if (m["b"] == 0) { }            // a pure "read"...
cout << m.size();               // 2  <-- "b" now EXISTS, with value 0
cout << m.count("c");           // 0, and size stays 2
m["a"]++;                       // 2
m.at("zzz");                    // throws std::out_of_range -- no silent insert
```

**`m[k]` is a write.** It default-constructs the value if the key is absent, which is exactly why
`freq[x]++` works and exactly why `if (m[k] == v)` is a bug. Use `count`, `find` or `at` to read.

Then the ordered-vs-unordered half: insert keys `10, 30, 20` into both and iterate. `map` gives
`10 20 30` (your `ordered.cpp` shows this); `unordered_map` gives whatever the hash table's layout
happens to be.

---

### 2. Two Sum — LC 1 *(fundamental)*

```cpp
vector<int> twoSum(vector<int>& a, int target) {
    unordered_map<int,int> pos;                      // value -> index
    for (int i = 0; i < (int)a.size(); ++i) {
        if (pos.count(target - a[i])) return {pos[target - a[i]], i};
        pos[a[i]] = i;                               // insert AFTER probing
    }
    return {};
}
```

**Probe before inserting.** Insert first and `[3,2,4]` with target 6 returns `[0,0]` — the element
pairs with itself. Verified: target 9 on `[2,7,11,15]` → `[0,1]`, `[3,3]` target 6 → `[0,1]`,
`[3,2,4]` target 6 → `[1,2]`.

O(n)/O(n), versus the O(n²) double loop. **This is the pattern the whole folder is about**: the
inner loop becomes a lookup of something the outer loop already recorded.

---

### 3. First Unique Character — LC 387

Two passes: count every character, then scan the string again and return the first index whose
count is 1. **The second pass must walk the string, not the map** — the map has no order, and even
an ordered `map` would give you alphabetical order rather than positional.

26 letters, so `int cnt[26]` beats a hash map here (readme Q10).

---

### 4. Majority Element — LC 169

Count with a map, return the key whose count exceeds `n/2`. O(n)/O(n).

**Then do it in O(1) space with Boyer–Moore:**

```
candidate = none, count = 0
for x in nums:
    if count == 0: candidate = x
    count += (x == candidate) ? 1 : -1
return candidate
```

Each non-candidate cancels one candidate; a strict majority cannot be fully cancelled. That
argument is the answer — the algorithm without it looks like magic. Note it is only valid because
the problem *guarantees* a majority exists; without that guarantee you need a verification pass.

---

### 5. Group Anagrams — LC 49

```
groups = map<string, vector<string>>
for each word: groups[ key(word) ].push_back(word)
return the values
```

The loop is trivial; **the key is the problem.** Two choices:

| Key | Cost | Note |
|---|---|---|
| the word, sorted | O(n · k log k) | two lines, easy to explain |
| the 26 counts, rendered as a string | O(n · k) | e.g. `"#1#0#2..."` — faster for long words |

Say both and pick based on `k`. "What is the key?" is the question to ask on every grouping
problem — LC 249, LC 288 and LC 1152 are all this with a different key.

---

### 6. Finding 3-Digit Even Numbers — LC 2094 *(implementation)*

Ten possible digits, so count into an array and test each candidate.

```cpp
vector<int> findEvenNumbers(vector<int>& digits) {
    int cnt[10] = {0};
    for (size_t i = 0; i < digits.size(); ++i) cnt[digits[i]]++;
    vector<int> out;
    for (int n = 100; n <= 998; n += 2) {            // even, no leading zero, ascending
        int d[3] = { n/100, (n/10)%10, n%10 };
        int need[10] = {0};
        need[d[0]]++; need[d[1]]++; need[d[2]]++;
        bool ok = true;
        for (int i = 0; i < 10; ++i) if (need[i] > cnt[i]) { ok = false; break; }
        if (ok) out.push_back(n);
    }
    return out;
}
```

**Enumerating the 450 candidates instead of the O(n³) index triples is the whole idea** — the
answer space is tiny and fixed, so iterate over *it*. Starting at 100 excludes leading zeros and
stepping by 2 gives evenness for free; the output is sorted with no sort call.

**`need[i] > cnt[i]` and not `!=`** is the detail: `220` needs two 2s, and the digit multiset must
supply them. Verified: `[2,1,3,0]` → `102 120 130 132 210 230 302 310 312 320`, `[2,2,8,8,2]` →
`222 228 282 288 822 828 882`.

`int cnt[10]` rather than a map — readme Q10 in practice.

---

### 7. Count Nice Pairs — LC 1814 *(trick — the rearrangement is the problem)*

The condition is `a[i] + rev(a[j]) == a[j] + rev(a[i])`. Move the `i` terms to one side:

```
a[i] - rev(a[i])  ==  a[j] - rev(a[j])
```

Now each element has a **key that depends only on itself**, and the question is "how many earlier
elements share my key" — one pass with a counting map.

```cpp
int countNicePairs(vector<int>& nums) {
    const long long MOD = 1000000007LL;
    unordered_map<int,long long> freq;
    long long ans = 0;
    for (size_t i = 0; i < nums.size(); ++i) {
        int k = nums[i] - rev(nums[i]);
        ans = (ans + freq[k]) % MOD;                 // pairs with every earlier equal key
        freq[k]++;
    }
    return (int)ans;
}
```

O(n) instead of O(n²). **Add `freq[k]` before incrementing it** — that counts the pairs `(earlier,
current)` exactly once each and avoids the `c*(c-1)/2` pass at the end. Verified: `[42,11,1,97]` →
2, `[13,10,35,24,76]` → 4.

The take-away generalises: **when a condition mixes `i` and `j`, try to separate them.** Once each
side depends on one index only, a map finishes the job.

---

### 8. Subarray Sum Equals K — LC 560

```
count = { 0 : 1 }          # the empty prefix -- REQUIRED
sum = 0, ans = 0
for x in nums:
    sum += x
    ans += count[sum - k]
    count[sum]++
```

O(n). `count[0] = 1` is what lets a subarray starting at index 0 be counted; without it every such
subarray is missed. See `../13_prefixSum`.

**Why not two pointers?** They need the running sum to be monotonic, which requires all values
non-negative. With negatives allowed the window cannot decide whether to grow or shrink. That is
the follow-up question, every time.

---

## Section 2 — Important

### 9. Copy List with Random Pointer — LC 138 *(trick, code given)*

The obstacle: you cannot set `random` until the target node exists. A map from old node to new node
removes it entirely.

```cpp
RNode* copyRandomList(RNode* head) {
    unordered_map<RNode*, RNode*> clone;
    for (RNode* c = head; c; c = c->next) clone[c] = new RNode(c->val);   // pass 1: all nodes
    for (RNode* c = head; c; c = c->next) {                               // pass 2: all pointers
        clone[c]->next   = c->next   ? clone[c->next]   : NULL;
        clone[c]->random = c->random ? clone[c->random] : NULL;
    }
    return head ? clone[head] : NULL;
}
```

**The keys are pointers**, which is fine — `unordered_map` hashes pointers happily. O(n)/O(n).
Verified including a null `random` and a null head.

**The O(1)-space follow-up** is a genuinely nice `../17_linked_list` trick, and it is what the
question is really for:

1. Interleave — insert each copy directly after its original: `A → A' → B → B' → …`
2. Now `cur->random->next` **is** the copy of `cur->random`, so randoms can be set with no map.
3. Unweave the two lists.

---

### 10. Top View of a Binary Tree *(implementation — your file's version)*

Assign each node a **column**: root is 0, left is `col-1`, right is `col+1`. BFS, and record the
first value seen at each column — BFS order guarantees the first is the topmost.

```cpp
vector<int> topView(TreeNode* root) {
    vector<int> out;
    if (!root) return out;                       // the guard the original lacks
    map<int,int> firstAtCol;                     // ORDERED -- iterating gives left to right
    queue< pair<TreeNode*,int> > q;
    q.push(make_pair(root, 0));
    while (!q.empty()) {
        TreeNode* n = q.front().first;
        int col     = q.front().second;
        q.pop();
        if (!firstAtCol.count(col)) firstAtCol[col] = n->val;
        if (n->left)  q.push(make_pair(n->left,  col - 1));
        if (n->right) q.push(make_pair(n->right, col + 1));
    }
    for (map<int,int>::iterator it = firstAtCol.begin(); it != firstAtCol.end(); ++it)
        out.push_back(it->second);
    return out;
}
```

**Using `map` instead of `unordered_map` deletes the entire min/max-column problem** — and that is
where your file's bugs are. Ordered iteration *is* left-to-right.

**It must be BFS, not DFS.** A DFS can reach a column at a deeper level before a shallower one and
record the wrong value.

Verified: the 7-node perfect tree gives `4 2 1 3 7`, a right skew gives `1 2 3`, and `NULL` gives
an empty result instead of a crash.

---

### 11. Bottom View — one line different

```cpp
lastAtCol[col] = n->val;        // unconditional: the last write at each column wins
```

That is the entire change from #10 — no `if (!count(col))`. BFS means later writes are deeper.
Verified: `4 2 6 3 7` on the same tree.

**Left view and right view are the same BFS keyed on *level* instead of column** — first and last
node of each level. Four views, one traversal, four different reducers. Worth writing once as a
single function with a flag.

---

### 12. Vertical Order Traversal — LC 987

Same column BFS, but **ties matter**: nodes at the same `(row, col)` must be output in increasing
*value* order. So the accumulator is `map<int, vector<pair<int,int> > >` keyed on column holding
`(row, value)`, sorted at the end.

**The tie rule is the whole difficulty** and it is what separates this from the GFG "vertical
order" problem, which does not require it. Read the statement carefully — this is a problem people
fail on a technicality, not on the algorithm.

---

### 13. 4Sum II — LC 454

```
sums = map: for every (a,b): sums[A[a] + B[b]]++          # n^2
ans = 0
for every (c,d): ans += sums[ -(C[c] + D[d]) ]            # n^2
```

**O(n²) instead of O(n⁴)** by splitting four arrays into two halves and letting the map join them.
"Meet in the middle" is the name; it also solves subset-sum for n ≈ 40.

---

### 14. Longest Substring Without Repeating — LC 3

```
last = map char -> most recent index
left = 0, best = 0
for right in 0..n-1:
    if s[right] in last: left = max(left, last[s[right]] + 1)
    last[s[right]] = right
    best = max(best, right - left + 1)
```

**`max(left, ...)` is essential** — `last` can hold an index from before the current window, and
without the `max` the left pointer moves backwards and the window becomes invalid.

The `../23_sets` version erases one character at a time; this one jumps. Same complexity, and the
map version is the one to keep, because it remembers *where* rather than merely *whether*.

---

### 15 & 16. Isomorphic Strings / Word Pattern — LC 205, 290

**Two maps, one per direction.** With a single map, `"ab" → "aa"` passes: `a→a` and `b→a` are each
individually consistent, but two source characters share one target, which is not a bijection.

```
for each position i:
    if fwd has s[i] and fwd[s[i]] != t[i]: return false
    if bwd has t[i] and bwd[t[i]] != s[i]: return false
    fwd[s[i]] = t[i];  bwd[t[i]] = s[i]
```

LC 290 is identical after splitting the sentence into words. Check the lengths match first —
`"aaa"` against `"dog cat"` must fail on count, not on mapping.

---

### 17. Time Based Key-Value Store — LC 981 *(the ordered-map showcase)*

```
store: unordered_map< string, map<int, string> >     # key -> (timestamp -> value)

set(k, v, t):  store[k][t] = v                        # timestamps arrive increasing
get(k, t):     it = store[k].upper_bound(t)           # first timestamp STRICTLY greater
               if it == begin(): return ""            # nothing at or before t
               return (--it)->second
```

**`upper_bound` then step back** is the idiom for "the largest key ≤ t", and there is no `≤`
version in the STL — you have to build it from `upper_bound` and a decrement. O(log n) per query.

Since timestamps arrive strictly increasing, a `vector<pair<int,string> >` plus `std::upper_bound`
is equally correct and faster; say that as the optimisation.

---

## Section 3 — Good to Know

### 18. LRU Cache — LC 146 *(implementation — the design problem of the folder)*

Two structures, because neither alone suffices: a hash map for **O(1) lookup**, a doubly linked
list for **O(1) reordering**.

```cpp
class LRUCache {
    struct Node { int k, v; Node *prev, *next; Node(int k_, int v_) : k(k_), v(v_), prev(NULL), next(NULL) {} };
    int cap;
    unordered_map<int, Node*> mp;
    Node *head, *tail;                                 // head side = most recent
    void unlink(Node* n) { n->prev->next = n->next; n->next->prev = n->prev; }
    void pushFront(Node* n) {
        n->next = head->next; n->prev = head;
        head->next->prev = n; head->next = n;
    }
public:
    LRUCache(int capacity) : cap(capacity) {
        head = new Node(0,0); tail = new Node(0,0);    // SENTINELS
        head->next = tail; tail->prev = head;
    }
    int get(int key) {
        if (!mp.count(key)) return -1;
        Node* n = mp[key];
        unlink(n); pushFront(n);
        return n->v;
    }
    void put(int key, int value) {
        if (mp.count(key)) { Node* n = mp[key]; n->v = value; unlink(n); pushFront(n); return; }
        if ((int)mp.size() == cap) {
            Node* lru = tail->prev;
            unlink(lru); mp.erase(lru->k); delete lru;
        }
        Node* n = new Node(key, value);
        mp[key] = n; pushFront(n);
    }
};
```

Three things make this work:

- **The sentinel head and tail.** With them, `unlink` and `pushFront` never test for `NULL` — the
  list is never empty. This is the same technique as a dummy head in `../17_linked_list`, and it
  removes about half the code you would otherwise write.
- **The map stores the node pointer, not the value.** Storing values would leave you searching the
  list for the node to move, which is O(n) and defeats the design.
- **`mp.erase(lru->k)` before `delete`.** Deleting first leaves you reading freed memory to find
  the key.

Verified against LeetCode's own trace (`1, -1, -1, 3, 4`), capacity 1, and overwriting an existing
key.

**`std::list` plus `splice` gives the same thing in a third of the lines** — worth mentioning, but
hand-rolling it once is what the question is for.

---

### 19. Insert Delete GetRandom O(1) — LC 380

`vector<int> vals` for O(1) random access, `unordered_map<int,int> idx` from value to its position.

**The trick is erase:** swap the doomed element with the **last** element, `pop_back`, and update
that one moved element's index in the map. O(1), and it is the only way to delete from the middle
of a vector in constant time.

Random selection needs contiguous storage; lookup needs a map; deletion needs the swap. All three
constraints at once is the design.

---

### 20. LFU Cache — LC 460

`key → (value, freq)`, plus `freq → list of keys at that frequency` in insertion order, plus
`minFreq`. On access, move the key from list `f` to list `f+1`; if list `minFreq` becomes empty,
`minFreq++`. Evict from the front of `list[minFreq]`.

**`minFreq` only ever increases by one on an access**, which is why the eviction is O(1) rather
than a search. Do #18 first.

---

### 21. Maximum Frequency Stack — LC 895

`freq[x]` and `group[f]` = a stack of the values that have *reached* frequency `f`. Push `x`:
increment `freq[x]`, push `x` onto `group[freq[x]]`, update `maxFreq`. Pop: take from
`group[maxFreq]`.

**A value appears in several groups at once**, which is the counter-intuitive part and also exactly
what makes pop correct — popping from `group[maxFreq]` leaves the earlier copies in the lower
groups untouched.

---

### 22. Minimum Window Substring — LC 76

Sliding window whose state is a count map, plus a single integer `missing` counting how many
required characters are still unsatisfied.

```
expand right; if s[right] was needed, missing--
while missing == 0:  record the window, then shrink from the left
```

**Track `missing` as a number, not by comparing two maps.** Comparing maps at every step is what
turns this into O(n · Σ). O(n) with the counter.

---

### 23. Number of Atoms — LC 726

Parse with a **stack of count maps**. `(` pushes a fresh map, `)` pops it, multiplies every count
by the following number, and merges it into the map below. Output needs sorted element names, so
the final container is an ordered `map`.

Map + stack + string parsing at once. The parsing is the tedious part; the stack is the idea.

---

### 24. Design Twitter — LC 355

`unordered_map<int, set<int> > following` and `unordered_map<int, vector<pair<int,int> > > tweets`
with a global timestamp. `getNewsFeed` merges the followed users' most recent tweets — **a
size-10 heap over k sorted lists**, which is `../25_heap`.

Do it after that folder. The map part is trivial; the merge is the question.

---

## Bugs in your files — with evidence

Both files were compiled and run. **Nothing in `24_maps/` was edited.**

`ordered.cpp` compiles and runs correctly: it prints `1 2 3 10 20 30` — the set values ascending,
then the map keys ascending even though they were inserted `10, 30, 20`. That is the correct
demonstration of what `map` and `set` guarantee.

`topviewBt.cpp` compiles. It has four bugs, and the first one alone stops it dead.

### `topviewBt.cpp:27` and `:33` — the wrong node is pushed, so it never terminates

```cpp
if(temp->left){
    pair<TreeNode*,int> p;
p.first=root;              // <-- should be temp->left
p.second=level-1;
q.push(p);
}
if(temp->right){
    pair<TreeNode*,int> p2;
p2.first=root;             // <-- should be temp->right
```

The **root** is re-enqueued instead of the child. `root` always has children, so it re-enqueues
itself forever. **Measured** on a 7-node tree, with a counter added to the loop: the queue was still
being popped after **200,000 iterations**, at which point the harness aborted it. The correct output
is `4 2 1 3 7`.

The variable `temp` was created two lines earlier precisely for this — it is `level` that is read
from `temp`, and `first` that is not.

### `topviewBt.cpp:39-40` — the sentinels are the wrong way round

```cpp
int minLevel=INT16_MIN;      // looking for a MINIMUM, starting at the smallest possible value
int maxLevel=INT16_MAX;      // looking for a MAXIMUM, starting at the largest
```

`min(minLevel, level)` can never go below `INT16_MIN`, and `max(maxLevel, level)` can never go
above `INT16_MAX`, so **the loop cannot change either variable** and the columns actually present
are ignored.

**Measured**, with only the queue bug repaired so execution could reach this point:

```
[minLevel=-32768  maxLevel=32767  => 65536 print iterations]
0 0 0 0 0 0 0 0 0 0 0 0 ... (65536 values printed in total)
```

Sixty-five thousand zeros instead of `4 2 1 3 7`. The initialisation must be **inverted** —
`minLevel = INT_MAX`, `maxLevel = INT_MIN` — so that the first real value replaces it. The same
inverted-identity idea appears deliberately in `../22_bst/solution.md` §3 #22, where it is the
correct thing to do; here it is accidental.

Also prefer `INT_MAX`/`INT_MIN` from `<climits>` over `INT16_*`. Column indices cannot exceed
±32767 in practice, but the 16-bit sentinels are the same family of bug as
`../21_tree/nodeTree.cpp`'s `INT16_MIN` heights.

### `topviewBt.cpp:47` — `m[i]` inserts while you are reading

```cpp
for(int i =minLevel;i<=maxLevel;i++){
    cout<<m[i]<<" ";
}
```

`operator[]` on an absent key **creates it** with value 0. So this loop does not merely print the
wrong thing — it grows `m` from 5 entries to 65,536 as it runs. That is the readme §2 trap in its
purest form. `m.count(i)` first, or `find`, or an ordered `map` iterated directly.

### `topviewBt.cpp:19` — no `NULL` guard

`m[0]=root->val;` runs before anything checks `root`. `topView(NULL)` dereferences a null pointer.
(That line is also redundant — the loop assigns column 0 on the first pop anyway.)

### The structural fix

Switch `unordered_map` to `map` and **all of the min/max machinery disappears** — no sentinels, no
range loop, no `m[i]`, just iterate the map. See §2 #10 above. That is the difference between
patching four bugs and removing the code they live in.

---

## What you got right

**The core idea of `topviewBt.cpp` is correct and it is the right idea.** Column index carried
through a BFS queue as `pair<TreeNode*, int>`, `level-1` going left and `level+1` going right, and
`if(m.find(level)==m.end())m[level]=temp->val;` to keep only the first value at each column — that
line is exactly right, including using `find` against `end()` rather than `[]`. Everything wrong
with the file is downstream of it.

**You chose BFS, not DFS.** Top view is one of the problems where that choice is load-bearing: a
DFS can reach a column at a deeper level first and record the wrong node. Getting that right
without being told is the substantive part of the problem.

**`ordered.cpp` makes exactly the right point in nine lines.** Inserting `10, 30, 20` and printing
`10 20 30` is the demonstration — out-of-order input, in-order output. Putting the `set` and the
`map` side by side in one file shows they share the same underlying tree, which is the fact that
explains both.

**Your `readme.md` list is well chosen and unusually varied.** 2094 (counting into a small array),
1814 (a derived key), top view (an ordered map on a tree) and 138 (pointers as keys) are four
genuinely different uses of a map — that is better coverage than a list of four frequency-counting
problems would have been.
