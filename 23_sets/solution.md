# 23 — Sets: Solutions

Approach and pseudocode first. Full C++ only for **fundamentals**, **implementations**, **hard**
problems and **tricks**. Everything shown has been compiled and run.

---

## Section 1 — Must Do

### 1. The operations drill *(fundamental)*

Not a judge problem — run it and read the output.

```cpp
unordered_set<int> u;  set<int> o;
int a[] = {5, 1, 3, 1, 9, 3};
for (int i = 0; i < 6; ++i) { u.insert(a[i]); o.insert(a[i]); }

u.size();                       // 4, not 6 -- duplicates vanished
for (int x : o) cout << x;      // 1 3 5 9  -- ALWAYS ascending
for (int x : u) cout << x;      // some order; do not depend on it
u.erase(42);                    // absent -> no-op, no throw
u.count(3);                     // 1
u.insert(3).second;             // false -- it was already there
o.lower_bound(4);               // iterator to 5; unordered_set has no such method
```

**The `.second` of `insert` is the reusable part.** It tells you whether the value was new, in the
same lookup that inserts it — that is #2 in one line.

---

### 2. Contains Duplicate — LC 217 *(fundamental)*

```cpp
bool containsDuplicate(vector<int>& a) {
    unordered_set<int> seen;
    for (size_t i = 0; i < a.size(); ++i)
        if (!seen.insert(a[i]).second) return true;
    return false;
}
```

**O(n) time, O(n) space.** Sorting first and checking neighbours is O(n log n) and O(1) space —
state both and say which you would pick given the constraints.

---

### 3. Valid Anagram — LC 242 *(fundamental — and a set is the wrong tool)*

A set discards counts, and this problem *is* counts: `"aab"` and `"abb"` produce the same set.
Count instead, with a fixed array since the alphabet is 26 letters:

```cpp
bool isAnagram(string s, string t) {
    if (s.size() != t.size()) return false;
    int cnt[26] = {0};
    for (size_t i = 0; i < s.size(); ++i) { cnt[s[i]-'a']++; cnt[t[i]-'a']--; }
    for (int i = 0; i < 26; ++i) if (cnt[i]) return false;
    return true;
}
```

**One pass over both strings, incrementing for `s` and decrementing for `t`** — a second loop is
unnecessary. O(n) time, O(1) space.

Follow-ups worth having an answer for: *unicode?* → `unordered_map<char,int>`. *Many strings
against one?* → build the count vector once and reuse it. Sorting both strings also works and is
O(n log n).

---

### 4. Intersection of Two Arrays — LC 349

```
put the SMALLER array into a set
scan the larger; every hit goes into a result set (which also dedups)
```

O(n + m) time, **O(min(n, m))** space. Hashing the smaller array is the only decision in the
problem. If both inputs are sorted, two pointers do it in O(1) extra space.

LC 350 (`[fwd]`) asks for the intersection **with multiplicities**, which a set cannot express —
that one needs `unordered_map<int,int>` counts, so it belongs in `../24_maps`.

---

### 5. Count Distinct After Reverse — LC 2442 *(trick, code given)*

```cpp
int countDistinctIntegers(vector<int>& nums) {
    unordered_set<int> s;
    for (size_t i = 0; i < nums.size(); ++i) {
        s.insert(nums[i]);
        int x = nums[i], r = 0;
        while (x) { r = r * 10 + x % 10; x /= 10; }   // reverse the digits
        s.insert(r);
    }
    return s.size();
}
```

The set does all the work — insert both forms of every number and read `size()`. Verified:
`[1,13,10,12,31]` → 6, `[2,2,2]` → 1.

**`r = r*10 + x%10` is the digit-reversal idiom** and it also appears in LC 7, LC 9 and LC 1814
(`../24_maps`). Note it can overflow `int` for large inputs — within this problem's constraints it
cannot, but say so if asked.

---

### 6. Find Maximum Number of String Pairs — LC 2744

```
seen = empty set
for each word w:
    if reverse(w) is in seen: answer++          # w pairs with an earlier word
    else: insert w
```

Same shape as #5 and as two-sum: **probe for the partner before inserting yourself**, so a word
never pairs with itself. O(n) with strings of length 2. All strings being distinct is what makes
the greedy count correct.

---

## Section 2 — Important

### 7. Longest Consecutive Sequence — LC 128 *(trick — the one that changes the complexity)*

```cpp
int longestConsecutive(vector<int>& nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int best = 0;
    for (unordered_set<int>::iterator it = s.begin(); it != s.end(); ++it) {
        int x = *it;
        if (s.count(x - 1)) continue;          // <-- THE LINE. not a run start, skip.
        int len = 1;
        while (s.count(x + len)) ++len;
        best = max(best, len);
    }
    return best;
}
```

**Why this is O(n) and not O(n²).** The inner `while` only ever runs for a value that has no
predecessor in the set — the *start* of a run. Each run therefore gets walked exactly once, and the
total inner work across all runs is n. Delete the `continue` line and every member of a run walks
the whole run: O(n²).

Verified: `[100,4,200,1,3,2]` → 4, `[0,3,7,2,5,8,4,6,0,1]` → 9 (duplicates handled by the set
itself), empty → 0, negatives → 3.

The sorting solution is three lines and O(n log n); this one exists to show that the right
structure can move a problem into a lower complexity class.

---

### 8. Happy Number — LC 202

```
seen = empty set
while n != 1:
    if n already in seen: return false        # a cycle, so it never reaches 1
    insert n
    n = sum of squares of digits of n
return true
```

O(log n) space. **The follow-up is Floyd's cycle detection** — run a slow pointer at one step and a
fast one at two, and they meet inside the cycle. O(1) space, and it is the same algorithm as
`../17_linked_list`'s cycle detection. Being able to move from "set of visited states" to "two
pointers" is a reusable upgrade — it also applies to LC 141 and LC 287.

---

### 9. Unique Number of Occurrences — LC 1207

```
count each value in a map            -> map<int,int>
put every COUNT into a set
return set.size() == map.size()
```

Two structures, each doing what it is good at: the map counts, the set checks distinctness. This is
the cleanest example of the division of labour between the two folders.

---

### 10. Contains Duplicate II — LC 219

```
window = empty set
for i in 0..n-1:
    if i > k: window.erase(a[i - k - 1])      # the element that just left the window
    if a[i] in window: return true
    window.insert(a[i])
return false
```

The set holds exactly the last `k` elements. **The erase line is the sliding window**
(`../14_Sliding window`) — without it the set grows to n and the answer is wrong, not just slow.

An `unordered_map<int,int>` of last-seen indices also works and is arguably clearer: on a hit,
check `i - last[a[i]] <= k`.

---

### 11. Intersection of Two Arrays II — LC 350

Counts, so a set cannot do it. `unordered_map<int,int>` over the smaller array; for each element of
the larger, if the count is positive, take it and decrement. O(n + m).

**If both arrays are sorted** — two pointers, O(1) extra space. **If one array is huge and on
disk** — hash the small one and stream the big one, which is the real follow-up and the answer they
are fishing for.

---

## Section 3 — Good to Know

### 12. Longest Substring Without Repeating Characters — LC 3

Set version: expand `right`; while `s[right]` is already in the set, erase `s[left]` and advance
`left`; record `right - left + 1`. O(n), each character inserted and erased once.

**The better version uses a map of last positions** and jumps `left` straight to
`max(left, last[c] + 1)` instead of erasing one character at a time. Same complexity, no inner
loop. Compare the two — it is the clearest case of a map beating a set by remembering *where*
rather than merely *whether*.

---

### 13. Set Mismatch — LC 645

Set solution: scan, the value already in the set is the duplicate; the value in `1..n` missing from
the set is the missing one. O(n)/O(n).

**O(1) space follow-up:** the values are `1..n`, so use the array itself as the record — for each
value `v`, negate `a[|v|-1]`; a slot already negative identifies the duplicate. Or use sum and
sum-of-squares and solve two equations. Same family as `../12_sorting`'s cycle-sort answers to LC
268/287/448/41.

---

### 14. Contains Duplicate III — LC 220 *(hard — the problem `set` exists for)*

```
window = empty ORDERED set
for i in 0..n-1:
    if i > indexDiff: window.erase(nums[i - indexDiff - 1])
    it = window.lower_bound( nums[i] - valueDiff )       # smallest stored value >= nums[i]-t
    if it != window.end() and *it - nums[i] <= valueDiff: return true
    window.insert(nums[i])
return false
```

**`lower_bound` is the whole problem.** You need the nearest stored value to `nums[i]`, and a hash
set cannot answer that at any cost — it has no notion of nearness. A `set` can, in O(log k), because
it is a balanced BST.

O(n log k) time, O(k) space. Use `long long` in the comparison, or `nums[i] - valueDiff` overflows
on extreme inputs.

The bucketing alternative — put values into buckets of width `valueDiff + 1` and check the
neighbouring buckets — is O(n) and worth knowing, but the ordered-set version is the one to write
first because it explains *why the folder has two set types*.

---

## Notes on your files

Both `.cpp` files in this folder were compiled and run.

### `basics.cpp` — correct, and the output is worth reading

```
55134target exist
```

That is `size()` printing `5`, then the four remaining values `5 1 3 4` after `erase(2)`, then the
membership message — all correct. Two observations:

- `s.size()` is 5 because all five inserted values were distinct. Insert `1` twice and it stays 5;
  that is the experiment to run.
- The iteration came out as `5 1 3 4` — **not sorted**, and not insertion order either. That is the
  hash table's internal layout showing through, and it is exactly why `set` exists. Swap
  `unordered_set` for `set` in this file and the same loop prints `1 3 4 5`.

Adding `cout << endl` between the outputs would make it much easier to read; right now the numbers
run into the message.

### `mapsBasics.cpp:31` — does not compile

```cpp
if(student.find("Piyush")!=student.end()){//code}
```

The `}` is **inside the `//` comment**, so it never closes the `if`. Measured:

```
23_sets/mapsBasics.cpp:34:1: error: expected '}' at end of input
```

Put the brace on its own line:

```cpp
if (student.find("Piyush") != student.end()) {
    // code
}
```

**A `//` comment swallows everything to the end of the line, including closing braces**, and the
error surfaces at the *end of the file* rather than at line 31 — which is why it looks mystifying.
Worth recognising the shape: "expected `}` at end of input" almost always means a brace was eaten
somewhere above.

Everything else in the file is right — `pair` construction and `insert`, `operator[]` insertion,
the range-for with `auto` (the commented-out `pair<string,int> p` version also works but copies),
and `erase` by key.

---

## What you got right

**You wrote `unordered_set` and `unordered_map` first**, before the ordered versions in
`../24_maps/ordered.cpp`. That is the correct order to learn them in — the hash versions are the
default and the ordered ones are the specialisation you reach for when you need order. Many
courses do it backwards.

**`basics.cpp` covers exactly the right five operations** — `insert`, `size`, `erase`, iterate,
`find` against `end()` — with nothing extraneous. That is the complete working set for a set, and
the `find(...) != s.end()` idiom is the one that generalises to maps and to every other STL
container.

**`mapsBasics.cpp` shows both insertion styles.** `insert(pair)` and `student["Raghav"] = 56` are
not the same operation — `operator[]` **default-constructs a value if the key is absent**, which
matters enormously for counting (`m[x]++` works precisely because of it) and is a real source of
bugs when you only meant to look something up. Having both in one file is the right way to have met
them.

**Keeping the map intro in the sets folder was a reasonable call**, even though it splits the topic
across two folders. A set is a map with the values removed; meeting them together makes the
relationship visible rather than academic.
