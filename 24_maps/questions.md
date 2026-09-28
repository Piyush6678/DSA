# 24 — Maps: Practice Questions

> **Why 24 ranked problems — nearly twice `../23_sets`.** A set does one thing; a map does six, and
> they do not transfer to each other. **Frequency counting** (5 problems — the default use, and the
> place to learn when an array beats a hash map), **value → index so an inner loop disappears** (4 —
> the O(n²)→O(n) move that shows up in more interviews than any other single idea), **group by a
> computed key** (3 — where choosing the key *is* the problem), **prefix sum + map** (2 — revisiting
> `../13_prefixSum` now that you have the structure it needed), **ordered-map queries** (4 —
> `lower_bound` and column-indexed tree views, which `unordered_map` cannot do), and **design
> problems** (6 — map plus a second structure, which is the whole genre of LRU/LFU/GetRandom).
> Twenty-four is four per pattern. Dropping below that means dropping a pattern, and the design
> group is the one interviews weight most heavily.

**Platform note.** LeetCode numbers are exact. GFG has no numeric IDs — search the quoted title.
**`[prem]`** marks LeetCode Premium.

**Scope note.** Everything needs only `01`–`24`. **`[fwd]`** marks a problem whose best-known
solution wants a later folder — solve it with a map first.

**All four entries from your `readme.md` are covered**: 2094 → §1 #6, 1814 → §1 #7, top view →
§2 #10, 138 → §2 #9.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | `map` vs `unordered_map`, and what `[]` really does | Easy | *Drill* | extend `ordered.cpp` |
| 2 | Two Sum | Easy | **LeetCode 1** | `two-sum` — value → index |
| 3 | First Unique Character in a String | Easy | **LeetCode 387** | `first-unique-character-in-a-string` |
| 4 | Majority Element | Easy | **LeetCode 169** | `majority-element` — then Boyer–Moore |
| 5 | Group Anagrams | **Medium** | **LeetCode 49** | `group-anagrams` — choosing the key |
| 6 | Finding 3-Digit Even Numbers | Easy | **LeetCode 2094** | `finding-3-digit-even-numbers` — **on your list** |
| 7 | Count Nice Pairs in an Array | **Medium** | **LeetCode 1814** | `count-nice-pairs-in-an-array` — **on your list** |
| 8 | Subarray Sum Equals K | **Medium** | **LeetCode 560** | `subarray-sum-equals-k` — from `../13_prefixSum` |

**Why these eight.** #1 is the drill that prevents the folder's signature bug: print `m.size()`
before and after `if (m["missing"] == 0)` and watch it grow. Five minutes, and it stops you writing
`m[k]` in a read-only context for the rest of your life.

#2 is the whole "value → index" pattern in six lines, and **probing before inserting** is the
detail — insert first and an element pairs with itself. #3 and #4 are counting in its plainest
form; #4 is here because the **Boyer–Moore majority vote** follow-up does it in O(1) space and is
worth seeing right after you have solved it the obvious way.

**#5 is the most instructive problem in the section.** The algorithm is one loop; the entire
difficulty is deciding what the key should be — the sorted string (O(n·k log k)) or the 26-count
vector rendered as a string (O(n·k)). "What is the key" is the question to ask on every grouping
problem.

#6 and #7 are your own picks and they are a good pair: #6 counts digits and checks candidates
against the counts (and the array-instead-of-map lesson from Q10 applies directly — ten digits, so
`int cnt[10]`), and **#7 is the "value → index" pattern with a derived key**. Rearranged,
`a+rev(b) == b+rev(a)` becomes `a-rev(a) == b-rev(b)`, so counting equal values of `x - rev(x)`
solves it in one pass. That rearrangement is the problem.

#8 you solved in `../13_prefixSum`; redo it now that you know what structure it was reaching for,
and make sure you can say why `count[0] = 1` is needed.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 9 | Copy List with Random Pointer | **Medium** | **LeetCode 138** | `copy-list-with-random-pointer` — **on your list** |
| 10 | Top View of a Binary Tree | **Medium** | GFG | *"Top View of Binary Tree"* — **on your list**; `topviewBt.cpp` |
| 11 | Bottom View of a Binary Tree | **Medium** | GFG | *"Bottom View of Binary Tree"* — #10 with one line changed |
| 12 | Vertical Order Traversal of a Binary Tree | **Hard** | **LeetCode 987** | `vertical-order-traversal-of-a-binary-tree` |
| 13 | 4Sum II | **Medium** | **LeetCode 454** | `4sum-ii` — split into halves |
| 14 | Longest Substring Without Repeating Characters | **Medium** | **LeetCode 3** | `longest-substring-without-repeating-characters` |
| 15 | Isomorphic Strings | Easy | **LeetCode 205** | `isomorphic-strings` — **two** maps |
| 16 | Word Pattern | Easy | **LeetCode 290** | `word-pattern` — #15 with words |
| 17 | Time Based Key-Value Store | **Medium** | **LeetCode 981** | `time-based-key-value-store` — ordered map |

**Why these nine. #9 is the cleanest map problem there is**: the difficulty is that you cannot set
a `random` pointer until the node it points to exists, and a map from old node to new node makes
that a non-problem. Two passes, done. **Then do the O(1)-space version** — interleave the copies
into the original list, fix the randoms, then unweave — because that is the follow-up and it is a
genuinely clever `../17_linked_list` trick.

**#10 is where your existing file is.** It has three separate bugs (see `solution.md`), and the
fix is not a patch — using an *ordered* `map` removes the min/max tracking that the bugs are in.
#11 is the same function with `if (!seen.count(col))` changed to an unconditional assignment, and
#12 is the hard version where ties within a cell must be broken by value.

#13 is the pattern that makes 4Sum tractable: **map the sums of the first two arrays, then look up
their negatives from the last two** — O(n²) instead of O(n⁴). #14 you met in
`../14_Sliding window`; the map version stores last positions and jumps the left pointer instead of
erasing one character at a time.

**#15 and #16 are a matched pair with a single trap:** one map is not enough. `"ab" → "aa"` passes
a one-way check and is not isomorphic. **#17 is the ordered-map showcase** — store each key's
values against timestamps and answer a query with `upper_bound` then step back, which is binary
search on a `map`.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 18 | LRU Cache | **Medium** | **LeetCode 146** | `lru-cache` — map + doubly linked list |
| 19 | Insert Delete GetRandom O(1) | **Medium** | **LeetCode 380** | `insert-delete-getrandom-o1` — map + vector |
| 20 | LFU Cache | **Hard** | **LeetCode 460** | `lfu-cache` — two maps and a list per frequency |
| 21 | Maximum Frequency Stack | **Hard** | **LeetCode 895** | `maximum-frequency-stack` — map of stacks |
| 22 | Minimum Window Substring | **Hard** | **LeetCode 76** | `minimum-window-substring` — window + counts |
| 23 | Number of Atoms | **Hard** | **LeetCode 726** | `number-of-atoms` — map + stack + parsing |
| 24 | Design Twitter | **Medium** | **LeetCode 355** | `design-twitter` `[fwd]` — maps + a heap |

**Why these seven.** They are all the same idea — **a map plus one more structure, because the map
alone cannot do it** — and each pairing teaches something different:

| Problem | Map + | Because the map cannot… |
|---|---|---|
| #18 LRU | doubly linked list | maintain recency order |
| #19 GetRandom | vector | pick a uniformly random element |
| #20 LFU | list per frequency | find the least-frequent in O(1) |
| #21 FreqStack | stack per frequency | remember insertion order within a frequency |
| #23 Atoms | stack | handle nesting |
| #24 Twitter | heap | merge k feeds by recency |

**#19 has the neatest trick in the folder**: to erase in O(1) from a vector you swap the doomed
element with the last one, `pop_back`, and fix that one moved element's index in the map. Random
access needs contiguous storage, and that is how you get both.

#22 is the hardest sliding window there is and its state *is* a map. #20 is #18 with an extra
dimension and is worth attempting only after #18 is comfortable.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Ransom Note | Easy | **LeetCode 383** | counts; an array beats a map here |
| Contains Duplicate II | Easy | **LeetCode 219** | last-seen index in a map |
| Find All Anagrams in a String | **Medium** | **LeetCode 438** | counts + sliding window |
| Sort Characters by Frequency | **Medium** | **LeetCode 451** | count, then bucket by frequency |
| Top K Frequent Elements | **Medium** | **LeetCode 347** `[fwd]` | count, then heap or bucket — `../25_heap` |
| Longest Palindrome | Easy | **LeetCode 409** | counts; odd counts contribute one |
| Intersection of Two Arrays II | Easy | **LeetCode 350** | counts, which is why a set fails |
| Find the Difference | Easy | **LeetCode 389** | counts, or XOR (`../15_bitwise`) |
| Continuous Subarray Sum | **Medium** | **LeetCode 523** | prefix sum **mod k** in a map |
| Subarray Sums Divisible by K | **Medium** | **LeetCode 974** | same, and mind negative remainders |
| Longest Substring with At Most K Distinct | **Medium** | **LeetCode 340** `[prem]` | window whose state is a map |
| Encode and Decode TinyURL | **Medium** | **LeetCode 535** | two maps, both directions |
| Design HashMap | Easy | **LeetCode 706** | build the bucket array yourself |
| Word Frequency in a paragraph | Easy | **LeetCode 819** | parse, count, filter a banned set |
| Vertical order — top/bottom/left/right views together | **Medium** | *Drill* | one BFS, four different reducers |
| `map<pair<int,int>, int>` vs `unordered_map` | Easy | *Drill* | the second does not compile — know why |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8
Section 2   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13
            [ ] 14  [ ] 15  [ ] 16  [ ] 17
Section 3   [ ] 18  [ ] 19  [ ] 20  [ ] 21  [ ] 22  [ ] 23  [ ] 24
Section 4   [ ] ______ / 16
```
