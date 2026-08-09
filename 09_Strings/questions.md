# 09 — Strings: Practice Questions

> **Why 26 ranked problems.** Strings look like one topic but are really six independent
> techniques sharing a data type: frequency counting, two pointers, expand-around-centre,
> parsing/state machines, in-place manipulation, and the prefix function. Each needs one
> problem to learn it and at least one more to confirm it transferred, and the parsing family
> (atoi, roman numerals) needs three because the edge cases *are* the problem. Twenty-six is
> where that stops adding new ideas. Below that you skip a technique; above it you are doing
> the same problem with different letters.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title.

**Scope note.** `unordered_map` arrives in `../24_maps`, so problems whose *optimal* solution
needs arbitrary-key hashing are marked **`[hash]`**. Every one of them has an in-scope
solution using a `freq[26]` or `freq[256]` array, given in `solution.md`. Problems marked
**`[sort]`** call `std::sort` — you already use it in `string.cpp:24`, so it is fair game as a
library call even though you write sorting yourself in `../12_sorting`.

**All three problems from your `readme.md` are in Section 1** (#1, #4, #5).

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Valid Anagram | Easy | **LeetCode 242** | `valid-anagram` — **on your list**; cf. `string.cpp:23` |
| 2 | Reverse String | Easy | **LeetCode 344** | `reverse-string` — two pointers, in place |
| 3 | Valid Palindrome | Easy | **LeetCode 125** | `valid-palindrome` — filtering + two pointers |
| 4 | Longest Common Prefix | Easy | **LeetCode 14** | `longest-common-prefix` — **on your list** |
| 5 | Isomorphic Strings | Easy | **LeetCode 205** | `isomorphic-strings` — **on your list** |
| 6 | Longest Substring Without Repeating Characters | **Medium** | **LeetCode 3** | `longest-substring-without-repeating-characters` |
| 7 | Longest Palindromic Substring | **Medium** | **LeetCode 5** | `longest-palindromic-substring` — expand around centre |
| 8 | Reverse Words in a String | **Medium** | **LeetCode 151** | `reverse-words-in-a-string` |

**Why these eight.** #1 is the frequency array in its purest form and the single most-asked
string question anywhere — do it with 26 ints, not with sorting. #2 and #3 are the two-pointer
pattern; #3 adds the filter-as-you-go wrinkle that makes it a real question rather than a
warm-up. #4 teaches you to compare *vertically* across strings rather than pairwise, and its
edge case (one string is a prefix of another) is the whole test. #5 is the first problem where
one map is not enough — you need the mapping to be a **bijection**, and `"badc" → "baba"` is the
counterexample that proves it. **#6 is the highest-frequency medium in this folder**; it is
sliding window before you have met sliding window, and the `"abba"` case separates people who
understand the technique from people who memorised it. #7 is the centre-expansion idea, which
you will reuse for palindromic substring counting. #8 forces you to handle multiple, leading
and trailing spaces — real parsing, no algorithm.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 9 | Largest Odd Number in String | Easy | **LeetCode 1903** | `largest-odd-number-in-string` — Striver Step 6 |
| 10 | Remove Outermost Parentheses | Easy | **LeetCode 1021** | `remove-outermost-parentheses` — depth counter |
| 11 | Maximum Nesting Depth of the Parentheses | Easy | **LeetCode 1614** | `maximum-nesting-depth-of-the-parentheses` |
| 12 | Minimum Add to Make Parentheses Valid | **Medium** | **LeetCode 921** | `minimum-add-to-make-parentheses-valid` |
| 13 | Roman to Integer | Easy | **LeetCode 13** | `roman-to-integer` |
| 14 | Integer to Roman | **Medium** | **LeetCode 12** | `integer-to-roman` — greedy |
| 15 | String to Integer (atoi) | **Medium** | **LeetCode 8** | `string-to-integer-atoi` — **overflow is the problem** |
| 16 | Implement strStr() | Easy | **LeetCode 28** | `find-the-index-of-the-first-occurrence-in-a-string` |
| 17 | Sort Characters By Frequency | **Medium** | **LeetCode 451** | `sort-characters-by-frequency` `[sort]` |
| 18 | Ransom Note | Easy | **LeetCode 383** | `ransom-note` |
| 19 | First Unique Character in a String | Easy | **LeetCode 387** | `first-unique-character-in-a-string` |

**Why these eleven.** #9 is three lines once you see that a number is odd iff its **last digit**
is odd, so you scan from the right for the first odd digit and cut — no big-integer arithmetic.
#10–#12 are the depth-counter family: all three are the same `int`, and doing them together
makes the pattern obvious in a way doing one never does. **#15 is the most important problem in
this section** — every interviewer who asks it is testing whether you check for overflow
*before* multiplying rather than after, because after is already undefined behaviour. #13/#14
are a matched pair (parse vs. generate) and #14 is your first greedy proof. #16 is the naive
O(n·m) search whose weakness motivates KMP in Section 3. #17–#19 are frequency arrays again,
which is the point: once you see it, you see it everywhere.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 20 | Rotate String | Easy | **LeetCode 796** | `rotate-string` — the `s+s` trick |
| 21 | Repeated Substring Pattern | Easy | **LeetCode 459** | `repeated-substring-pattern` — same trick, sharper |
| 22 | Longest Happy Prefix | **Hard** | **LeetCode 1392** | `longest-happy-prefix` — this *is* the LPS array |
| 23 | Shortest Palindrome | **Hard** | **LeetCode 214** | `shortest-palindrome` — LPS on `s + '#' + reverse(s)` |
| 24 | Group Anagrams | **Medium** | **LeetCode 49** | `group-anagrams` `[hash]` |
| 25 | Palindromic Substrings (count) | **Medium** | **LeetCode 647** | `palindromic-substrings` — #7's engine, reused |
| 26 | Check if Two Strings Are Anagram Permutations in a Window | **Medium** | **LeetCode 567** | `permutation-in-string` — sliding frequency array |

**Why these seven.** #20 and #21 are worth doing back to back: both hinge on the observation
that every rotation of `s` is a substring of `s + s`, but #21 needs the extra step of trimming
one character off each end so `s` cannot match itself. **#22 and #23 are where the prefix
function stops being theory** — #22 asks for the LPS value directly, and #23 is the trick of
gluing the string to its own reverse with a separator that cannot appear in either. Write
`buildLPS` once and both fall out. #24 is here rather than Section 1 only because the natural
solution wants a map; the in-scope version is in `solution.md`. #25 reuses #7's centre expansion
with a counter instead of a max — do it right after #7 while it's fresh. #26 is your first
*sliding* frequency array, which is the bridge to `../14_Sliding window`.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Valid Palindrome II (one deletion) | Easy | **LeetCode 680** | two pointers + one branch on mismatch |
| Longest Palindrome (buildable) | Easy | **LeetCode 409** | pairs of each char, plus one odd in the middle |
| Reverse Words in a String III | Easy | **LeetCode 557** | reverse each word in place — simpler than #8 |
| Reverse Vowels of a String | Easy | **LeetCode 345** | two pointers with a skip condition |
| Length of Last Word | Easy | **LeetCode 58** | scan from the right; trailing spaces are the test |
| Add Strings | Easy | **LeetCode 415** | schoolbook addition, no `stoi` allowed |
| Multiply Strings | **Medium** | **LeetCode 43** | the `i+j` / `i+j+1` index identity |
| Compare Version Numbers | **Medium** | **LeetCode 165** | parse and compare segment by segment |
| String Compression | **Medium** | **LeetCode 443** | in-place, two-pointer write index |
| Count and Say | **Medium** | **LeetCode 38** | also in `../10_Recursion` §2 #16 |
| Zigzag Conversion | **Medium** | **LeetCode 6** | simulate the row index bouncing between 0 and numRows-1 |
| Sum of Beauty of All Substrings | **Medium** | **LeetCode 1781** | Striver Step 6 — freq array rebuilt per start |
| Count vowels in a string | Easy | *Drill* | fixes the hanging loop at `string.cpp:66` |
| Most-frequent character(s) | Easy | *Drill* | fixes `mostOccuringCharacter` at `string.cpp:30` |
| Count characters differing from both neighbours | Easy | *Drill* | your `diff()` — already correct, prove it to yourself |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8
Section 2   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15  [ ] 16  [ ] 17  [ ] 18  [ ] 19
Section 3   [ ] 20  [ ] 21  [ ] 22  [ ] 23  [ ] 24  [ ] 25  [ ] 26
Section 4   [ ] ______ / 15
```
