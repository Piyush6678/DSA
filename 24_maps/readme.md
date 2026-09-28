# 24 — Maps

A set remembers *whether*. A map remembers *what*.

```
set:  "have I seen x?"                  ->  yes / no
map:  "what is associated with x?"      ->  a count, an index, a list, an object
```

That extra slot is why maps show up in far more problems than sets. **Almost every "do it in O(n)
instead of O(n²)" solution is a map**: the inner loop becomes a lookup of something the outer loop
already recorded.

---

## 1. The two maps

| | `unordered_map` | `map` |
|---|---|---|
| Built on | **hash table** | **red-black tree** (`../22_bst`) |
| `insert` / `find` / `[]` / `erase` | **O(1) average**, O(n) worst | **O(log n) guaranteed** |
| Iteration order | arbitrary | **sorted by key, ascending** |
| `lower_bound` / `upper_bound` | ✗ | ✓ |
| Header | `<unordered_map>` | `<map>` |

Your `ordered.cpp` demonstrates the difference exactly: keys inserted `10, 30, 20` iterate as
`10 20 30`. An `unordered_map` gives no such promise.

**Default to `unordered_map`.** Reach for `map` when you need sorted keys, `lower_bound`, or a
worst-case guarantee. In this folder, tree column-views (§4) and time-based lookups both need
`map`, and they are the two problems that justify it.

---

## 2. `operator[]` — the one thing to get right

```cpp
unordered_map<string,int> m;
m["Piyush"] = 23;               // insert
m["Raghav"]++;                  // Raghav did not exist. It exists NOW, with value 0, then 1.
```

**`m[key]` inserts a default-constructed value if the key is absent.** For `int` that default is
`0`, which is precisely why the counting idiom works:

```cpp
for (char c : s) freq[c]++;     // no "if absent, set to 0" needed -- [] already did it
```

But it also means **`m[key]` is not a read-only operation**:

```cpp
if (m["Piyush"] == 23) ...      // if "Piyush" was absent, it now EXISTS with value 0
cout << m.size();               // ...and size() just went up
```

Use `count`, `find`, or (C++11) `at` when you only want to look:

```cpp
if (m.count("Piyush")) ...                       // pure read
if (m.find("Piyush") != m.end()) ...             // same, gives you the iterator
m.at("Piyush");                                  // throws if absent -- no silent insert
```

**This is the single most common map bug**, and it is silent — the program does not crash, it just
accumulates phantom keys and reports the wrong `size()`.

---

## 3. Iterating

```cpp
for (auto p : m)                       cout << p.first << " " << p.second;   // copies each pair
for (const auto& p : m)                cout << p.first << " " << p.second;   // no copy
for (pair<const string,int>& p : m)    ...                                   // the honest type
```

**Note the `const` on the key.** A map element is `pair<const Key, Value>` — you may change the
value through an iterator, never the key. Writing `pair<string,int> p : m` (as the commented-out
line in `../23_sets/mapsBasics.cpp` does) compiles but silently **copies** every element, because
the types do not match exactly. `const auto&` is the habit to build.

Structured bindings (`for (auto& [k, v] : m)`) would be nicer and are **C++17** — this toolchain is
C++14, so they will not compile here.

---

## 4. The patterns

### (a) Frequency counting — the default use

```cpp
unordered_map<int,int> freq;
for (int x : nums) freq[x]++;
```

Everything downstream is a query on `freq`: the max, the values with count 1, the values with count
> n/2, top k. **When the keys are a small known set — 26 letters, digits, ASCII — use a plain
array instead.** `int cnt[26]` is faster, smaller and simpler than a hash map, and it is the right
answer for anagram problems.

### (b) Value → index, so the inner loop disappears

```
for i in 0..n-1:
    if (target - a[i]) is in seen: return {seen[target - a[i]], i}
    seen[a[i]] = i
```

Two Sum. **Probe before you insert**, or an element pairs with itself. This is the shape of LC 1,
LC 3, LC 219, LC 454 and LC 1814 — "has the partner I need already gone past?"

### (c) Group by a computed key

```cpp
unordered_map<string, vector<string> > groups;
for (string& w : words) groups[ signature(w) ].push_back(w);
```

Group anagrams: the signature is the sorted string, or the 26-count vector rendered as a string. The
map's value can be any container, and choosing the key *is* the problem — the rest is a loop.

### (d) Prefix sum + map

Already met in `../13_prefixSum`: `count[runningSum]` turns "how many subarrays sum to k" from
O(n²) into O(n). The same idea reappears in `../21_tree` §3 #24 (path sum III) with an undo step.
**Seed it with `count[0] = 1`** — the empty prefix — or every subarray starting at index 0 is
missed.

### (e) Map + doubly linked list = O(1) cache

The map gives O(1) lookup, the list gives O(1) reordering. Neither alone is enough. That is LRU
(LC 146), and it is the design problem of this folder.

### (f) Ordered map for tree column views

**Your `topviewBt.cpp` is this pattern.** BFS carrying a column index — `col-1` going left, `col+1`
going right — and a `map<int,int>` keyed on the column. Because the map is ordered, iterating it
gives the columns left to right with no min/max tracking at all:

```cpp
map<int,int> firstAtCol;
queue< pair<TreeNode*,int> > q;
q.push(make_pair(root, 0));
while (!q.empty()) {
    TreeNode* n = q.front().first; int col = q.front().second; q.pop();
    if (!firstAtCol.count(col)) firstAtCol[col] = n->val;   // BFS: first seen at a column is topmost
    if (n->left)  q.push(make_pair(n->left,  col - 1));
    if (n->right) q.push(make_pair(n->right, col + 1));
}
```

**Top view records the first value at each column; bottom view overwrites and keeps the last.** One
line apart. With an `unordered_map` you would have to track the minimum and maximum column and loop
between them — which is what your file attempts, and where its bug is.

---

## 5. Keys that are not `int` or `string`

Same rule as `../23_sets`: `unordered_map` needs `std::hash`, which does not exist for `pair`,
`vector` or your own structs. `map` needs only `<`.

```cpp
map< pair<int,int>, int > grid;             // fine
unordered_map< pair<int,int>, int > bad;    // COMPILE ERROR
```

Encode the pair into one key (`(long long)x * 1000000 + y`, or `to_string(x) + "," + to_string(y)`),
or use `map`. Writing a custom hash functor is the third option and rarely worth the time in an
interview.

---

## Interview Q&A

**Q1. `map` or `unordered_map`?**
`unordered_map` by default — O(1) average versus O(log n). Use `map` for sorted iteration,
`lower_bound`/`upper_bound`, or when you need a worst-case guarantee. Naming the reason is the
answer; naming the container is not.

**Q2. What does `m[key]` do when the key is missing?**
Inserts it with a default-constructed value and returns a reference to it. So `m[k]++` works for
counting, and `if (m[k] == x)` silently grows the map. Use `count`/`find`/`at` for pure reads.

**Q3. Why is `unordered_map` O(n) in the worst case?**
All keys can collide into one bucket, degrading it to a linked list. With adversarial input this is
a real attack (hash flooding). `map` is O(log n) always because it is a balanced tree.

**Q4. What is the underlying structure of each?**
`unordered_map` — hash table with bucket chaining. `map` — red-black tree. That single fact
explains every row of the comparison table, including why only one of them has `lower_bound`.

**Q5. Design an LRU cache with O(1) get and put.**
Hash map from key to list node, plus a doubly linked list ordered most-recent to least-recent.
`get` moves the node to the front; `put` inserts at the front and evicts from the back. The map
gives O(1) lookup, the list gives O(1) splicing — neither can do the job alone, which is the
insight being tested.

**Q6. Count subarrays summing to k.**
Running prefix sum plus a map from prefix value to how many times it has occurred; at each index
add `count[sum - k]`. Seed with `count[0] = 1`. O(n). The two-pointer alternative fails as soon as
negatives are allowed — say so, it is the follow-up.

**Q7. Can I use a `vector` as a map key?**
With `map`, yes — `vector` has `operator<`. With `unordered_map`, no, there is no `std::hash` for
it. Convert it to a string, or supply a hash functor.

**Q8. How would you find the top k frequent elements?**
Count with a map, then select. A heap of size k gives O(n log k); **bucket sort by frequency gives
O(n)**, because a frequency cannot exceed n so it can index an array of buckets directly. The
bucket answer is the one they are waiting for. (LC 347 — it is in `../25_heap`.)

**Q9. Two strings — are they isomorphic?**
Two maps, one in each direction. A single map passes `"ab" → "aa"` incorrectly, because it only
checks that each source character maps consistently, not that no two map to the same target.

**Q10. When is a plain array better than a map?**
Whenever the keys are small integers in a known range — letters, digits, ASCII, values `1..n`. An
array is faster, uses less memory, has no hashing cost, and cannot degrade. Reaching for a hash map
to count 26 letters is the reflex to unlearn.

---

## Your original notes (preserved)

**All four are covered** — see `questions.md`.

```
lc 
2094
1814 nice pair 
top view of binary tree
138
```

| Your entry | Now at |
|---|---|
| 2094 Finding 3-Digit Even Numbers | §1 #6 |
| 1814 Count Nice Pairs in an Array | §1 #7 |
| top view of binary tree | §2 #10 — `topviewBt.cpp`, and it has bugs |
| 138 Copy List with Random Pointer | §2 #9 |
