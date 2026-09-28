# 23 — Sets: Practice Questions

> **Why only 14 ranked problems — the smallest set in the repo so far, deliberately.** A set
> answers exactly one question, *"have I seen this?"*, and padding it out would mean importing
> problems that are really about maps, sliding windows or sorting. Fourteen covers the four things
> a set genuinely does and nothing else: **deduplicate** (3 problems), **membership during a scan**
> (5 — the pattern that recurs in every later folder), **set algebra** (2), and **ordered-set
> queries** (2 — `lower_bound`, the one thing `unordered_set` cannot do), plus 2 that exist to show
> you where a set is the *wrong* tool. Compressing further would drop the ordered-set pair, and
> those are the only reason `set` exists alongside `unordered_set`.
>
> The genuinely large hashing set lives in `../24_maps`, because counting is a map problem.

**Platform note.** LeetCode numbers are exact. GFG has no numeric IDs — search the quoted title.

**Scope note.** Everything needs only `01`–`23`. **`[fwd]`** marks a problem whose best solution
belongs to a later folder; solve it with a set first, then look at the note.

**All three problems from your `readme.md` are covered** — they are §1 #3, #5 and #6.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | `set` vs `unordered_set` — the operations drill | Easy | *Drill* | extend `basics.cpp` |
| 2 | Contains Duplicate | Easy | **LeetCode 217** | `contains-duplicate` |
| 3 | Valid Anagram | Easy | **LeetCode 242** | `valid-anagram` — **on your list** |
| 4 | Intersection of Two Arrays | Easy | **LeetCode 349** | `intersection-of-two-arrays` |
| 5 | Count Distinct Integers After Reverse Operations | **Medium** | **LeetCode 2442** | `count-number-of-distinct-integers-after-reverse-operations` — **on your list** |
| 6 | Find Maximum Number of String Pairs | Easy | **LeetCode 2744** | `find-maximum-number-of-string-pairs` — **on your list** |

**Why these six.** #1 is not a judge problem and is the most useful thing in the section: insert
the same value twice and watch `size()` not move; iterate an `unordered_set` and a `set` holding
the same numbers and compare the order; call `erase` on something that is not there; check what
`insert` actually returns. Fifteen minutes, and it removes an entire class of later confusion.

#2 is the pattern in its shortest form, and it is worth writing as
`if (!seen.insert(x).second) return true;` — one lookup instead of two.

**#3 is here to show you when a set is the wrong answer.** "Valid anagram" is about *counts*, and a
set throws counts away — `"aab"` and `"abb"` have the same set. The right tool is a 26-element
array (or a map). Doing this problem inside the sets folder is the point, not an oversight.

#4 is set algebra, and the trick is which array to hash: **the smaller one**, so the space is
O(min(n,m)). #5 and #6 are your own picks and both are one-liners around a set — #5 inserts each
number and its reversal, #6 inserts each string and probes for its reverse. Do them back to back;
they are the same problem.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 7 | Longest Consecutive Sequence | **Medium** | **LeetCode 128** | `longest-consecutive-sequence` — the O(n) trick |
| 8 | Happy Number | Easy | **LeetCode 202** | `happy-number` — cycle detection with a set |
| 9 | Unique Number of Occurrences | Easy | **LeetCode 1207** | `unique-number-of-occurrences` — a map *then* a set |
| 10 | Contains Duplicate II | Easy | **LeetCode 219** | `contains-duplicate-ii` — a set with a window |
| 11 | Intersection of Two Arrays II | Easy | **LeetCode 350** | `intersection-of-two-arrays-ii` `[fwd]` — counts, so a map |

**Why these five. #7 is the problem of this folder.** Sorting gives O(n log n) in two lines; the
set solution is O(n) and turns on one observation — **only start counting a run from a number
whose predecessor is absent.** Without that guard the inner `while` re-walks every run from every
member and the whole thing is O(n²). It is the best "a set changed the complexity class" example
there is.

#8 is your first cycle detection: keep the numbers you have seen; a repeat means a loop and the
answer is false. **The follow-up is Floyd's tortoise-and-hare** (`../17_linked_list`), which does
it in O(1) space — the same upgrade as #2's.

#9 and #11 are both "the set alone is not enough" problems: count with a map first, *then* use a
set on the counts. #10 is a set with a size limit, which is a sliding window
(`../14_Sliding window`) wearing a set costume — erase the element leaving the window as you go.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 12 | Longest Substring Without Repeating Characters | **Medium** | **LeetCode 3** | `longest-substring-without-repeating-characters` |
| 13 | Set Mismatch | Easy | **LeetCode 645** | `set-mismatch` `[fwd]` — O(1) space with cycle sort |
| 14 | Contains Duplicate III | **Hard** | **LeetCode 220** | `contains-duplicate-iii` — **`set::lower_bound`** |

**Why these three. #14 is the reason `set` exists.** The question is "is there an earlier value
within `valueDiff` of this one, within `indexDiff` positions" — you need the *nearest stored value*
to `x`, which is `lower_bound`, which `unordered_set` does not have at any price. Maintain a `set`
holding the current window, `lower_bound(x - t)`, and check whether what comes back is within `t`.
This single problem justifies the whole `set`-vs-`unordered_set` distinction.

#12 you have already met in `../14_Sliding window`; redo it here with a set and compare against the
"jump the left pointer using a map of last positions" version — the map version is strictly better
and seeing why is the lesson. #13 is a set problem with an O(1)-space answer hiding behind it,
which is the same conversation as #2 and #8.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Jewels and Stones | Easy | **LeetCode 771** | the shortest possible membership problem |
| Single Number | Easy | **LeetCode 136** `[fwd]` | a set works; XOR is O(1) space (`../15_bitwise`) |
| Missing Number | Easy | **LeetCode 268** `[fwd]` | set, or sum formula, or XOR |
| Uncommon Words from Two Sentences | Easy | **LeetCode 884** | split, count, then filter |
| Destination City | Easy | **LeetCode 1436** | set difference — sources minus destinations |
| Check if the Sentence Is Pangram | Easy | **LeetCode 1832** | `set.size() == 26` |
| Number of Good Pairs | Easy | **LeetCode 1512** `[fwd]` | counts, so really `../24_maps` |
| Linked List Cycle | Easy | **LeetCode 141** `[fwd]` | set of visited nodes, then Floyd's |
| Distribute Candies | Easy | **LeetCode 575** | `min(distinct, n/2)` |
| Unique Email Addresses | Easy | **LeetCode 929** | string normalisation into a set |
| Custom comparator on an ordered `set` | **Medium** | *Drill* | a comparator that ignores a field silently dedups on it |
| Set of `pair<int,int>` | Easy | *Drill* | works with `set`, does **not** compile with `unordered_set` |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6
Section 2   [ ] 7   [ ] 8   [ ] 9   [ ] 10  [ ] 11
Section 3   [ ] 12  [ ] 13  [ ] 14
Section 4   [ ] ______ / 12
```
