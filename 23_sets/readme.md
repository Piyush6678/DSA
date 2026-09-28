# 23 — Sets

A container that holds **each value at most once** and answers one question fast:

> *"Have I seen this before?"*

That is the entire topic. What makes it worth a folder is that a surprising number of problems
collapse to that question, and that C++ gives you **two** sets with very different performance
characteristics — picking the wrong one is the mistake this folder exists to prevent.

*(`mapsBasics.cpp` lives in this folder too. Maps get their own treatment in `../24_maps`; the
`unordered_map` intro here is the natural pair to `unordered_set`, since a set is a map with the
values thrown away.)*

---

## 1. The two sets

| | `unordered_set` | `set` |
|---|---|---|
| Built on | **hash table** | **red-black tree** (`../22_bst`) |
| `insert` / `find` / `erase` | **O(1) average**, O(n) worst | **O(log n) guaranteed** |
| Iteration order | arbitrary — and it *changes* | **sorted, ascending** |
| `lower_bound` / `upper_bound` | ✗ not available | ✓ |
| Header | `<unordered_set>` | `<set>` |

**Default to `unordered_set`.** Reach for `set` only when you need one of the two things it has
that the other does not: **sorted iteration**, or **`lower_bound`** (the nearest stored value ≥ x).

Your `basics.cpp` prints `1 3 4 5` — not `1 3 5 4` or any other order — which happens to be
ascending, but that is the hash table's business and nothing to rely on. **Never write code whose
correctness depends on `unordered_set` iteration order.**

```cpp
unordered_set<int> s;
s.insert(4);              // O(1) avg; a second insert(4) does nothing
s.erase(2);               // no-op if absent -- does not throw
s.size();                 // number of DISTINCT values
s.count(x);               // 0 or 1  -- the idiomatic membership test
s.find(x) != s.end();     // identical meaning, more typing
for (int x : s) ...       // arbitrary order
```

`count(x)` is the cleaner membership test and is what you should be writing. `find(x) != s.end()`
matters only when you then want to *use* the iterator.

**`insert` returns a pair**, and the `.second` is a bool saying whether it was new:

```cpp
if (!seen.insert(a[i]).second) return true;      // "already present" == duplicate found
```

That one line is LeetCode 217. It replaces the insert-then-check-then-insert shape and does a
single lookup instead of two.

---

## 2. The four things sets actually do

| Pattern | Looks like | Example |
|---|---|---|
| **Deduplicate** | build the set, read `size()` | count distinct values |
| **Membership during a scan** | `if (seen.count(x)) ...; seen.insert(x);` | LC 217, LC 1, cycle detection |
| **Set algebra** | build one, probe with the other | intersection, union, difference |
| **Ordered queries** | `set::lower_bound` | nearest value within k — LC 220 |

The second is the one that matters. **"Have I seen this before, in this scan?"** is the shape of
duplicate detection, two-sum, cycle detection in `../17_linked_list`, visited-tracking in
`../27_graphs`, and the character-window in `../14_Sliding window`.

**Set algebra with the STL:**

```cpp
unordered_set<int> A(a.begin(), a.end());        // range constructor -- one line, O(n)
for (int x : b) if (A.count(x)) common.insert(x);
```

The range constructor is worth remembering; the manual insert loop is three lines for nothing.

---

## 3. What a set costs you

**A set forgets two things: duplicates and order.**

That is fine for "which values appear" and useless for "how many times". `{1,1,2}` and `{1,2,2}`
have identical sets. The moment the question is *how many*, you want a `map<int,int>` — which is
`../24_maps`, and is why these two folders belong next to each other.

Two more real costs:

- **Memory and constant factor.** A hash set stores buckets, pointers and hashes. For small
  integers in a known range, `vector<bool>` or a plain `int cnt[26]` beats it soundly — that is
  why the anagram solution below uses an array and not a set.
- **`unordered_set` is O(n) worst case.** Adversarial keys can collide into a single bucket.
  Competitive programmers hit this deliberately with anti-hash tests; `set` is the safe fallback
  because O(log n) is a *guarantee*.

---

## 4. Sets of things that are not `int`

`unordered_set` needs a hash function. It has one for `int`, `string`, and other built-ins — **not
for `pair`, `vector`, or your own structs.** `set` needs only `<`, which `pair` and `string` do
have, so:

```cpp
set< pair<int,int> > seen;         // works out of the box -- pair has operator<
unordered_set< pair<int,int> > s;  // COMPILE ERROR -- no std::hash for pair
```

Three ways out, easiest first: use `set` instead; encode the pair into one number
(`x * 1000000 + y`) or a string; or write a hash functor and pass it as the second template
argument. In an interview, say the first two — the third is a lot of typing for no insight.

For a `set` of a custom struct, provide `operator<` (or a comparator type). **The comparator also
defines equality**: `set` considers `a` and `b` the same element when neither `a < b` nor `b < a`.
A comparator that compares only one field silently deduplicates on that field, which is
occasionally exactly what you want and more often a bug.

---

## Interview Q&A

**Q1. `set` or `unordered_set` — how do you choose?**
`unordered_set` by default: O(1) average versus O(log n). Switch to `set` if you need sorted
iteration, `lower_bound`/`upper_bound`, or a worst-case guarantee. Say the reason, not just the
name.

**Q2. What is `unordered_set`'s worst case, and when does it happen?**
O(n) per operation, when every key hashes into the same bucket. It happens with adversarial input
or a bad custom hash. `set` is O(log n) always because it is a balanced tree.

**Q3. What does `set` use internally?**
A red-black tree — a self-balancing BST (`../22_bst/advanced_tree_readme.md` §2). That is exactly
why iteration is sorted and `lower_bound` exists: both are properties of the tree, not extras.

**Q4. Find duplicates in an array. Which structure?**
`unordered_set` and check-before-insert — O(n)/O(n). If the values are known to be in `1..n`, you
can do it in **O(1) extra space** with cycle sort or index-marking (`../12_sorting`), which is the
follow-up they are waiting for.

**Q5. Why can't you put a `pair` in an `unordered_set`?**
There is no `std::hash` specialisation for `pair`. Use `set<pair<...>>` (which needs only `<`),
encode the pair into a single key, or supply a custom hash functor.

**Q6. Difference between a set and a map, in one sentence?**
A set stores keys; a map stores keys with attached values. If the question is *"does this exist"*
use a set; if it is *"how many"* or *"which one"*, use a map.

**Q7. How do you find the intersection of two arrays?**
Put the smaller one in an `unordered_set`, scan the larger, and collect the hits — O(n + m) time,
O(min(n,m)) space. If both are already sorted, two pointers do it in O(1) extra space, and if they
are not, sorting first is O(n log n) and usually worse.

**Q8. `count()` or `find()`?**
Identical for a set, since `count` is 0 or 1. Use `count` when you only want to know, `find` when
you want the iterator afterwards. (For `multiset`, `count` is O(k) in the number of matches —
there the difference is real.)

---

## Your original notes (preserved)

**All three are covered** — see `questions.md`.

```
lc
2442
2744
242
```

| Your entry | Now at |
|---|---|
| 2442 Count Distinct Integers After Reverse Operations | §1 #5 |
| 2744 Find Maximum Number of String Pairs | §1 #6 |
| 242 Valid Anagram | §1 #3 |
