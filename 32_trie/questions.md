# 32 — Tries: Practice Questions

> **Why 14 ranked problems.** Tries are a single data structure with two variants (character trie and bit-trie), and 14 is what it takes to implement the structure, cover prefix-query applications, drill the bit-trie for XOR problems, and reach the hard compositions like Word Search II where tries combine with backtracking. The topic is narrow — unlike DP's ten patterns, a trie IS the pattern — so padding beyond 14 would mean importing string or tree problems that happen to have a trie-based alternative.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search the
quoted title.

**Scope note.** Everything needs only `01`–`31`.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Implement Trie (Prefix Tree) | **Medium** | **LeetCode 208** | the data structure itself |
| 2 | Implement Trie-II (with counts) | **Medium** | GFG | *"Implement Trie (Prefix Tree) - II"* or *Drill* — countEndsWith, countPrefix |
| 3 | Longest Common Prefix | Easy | **LeetCode 14** | Trie approach vs vertical scan |
| 4 | Design Add and Search Words Data Structure | **Medium** | **LeetCode 211** | Trie + DFS for wildcard '.' |
| 5 | Maximum XOR of Two Numbers in an Array | **Medium** | **LeetCode 421** | Bit-Trie |

**Why these five.** #1 is the structure itself — without it nothing else makes sense. #2 is Striver's Trie-II with counting support, a common follow-up in interviews. #3 is the simplest APPLICATION — it can be solved without a trie (vertical scan), but solving it WITH a trie teaches you what tries are naturally good at. #4 is the first genuine composition — Trie + DFS backtracking for the wildcard. #5 is the Bit-Trie, an entirely different variant where each "character" is a bit — it connects back to `../15_bitwise` and is the reason bit-tries exist.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 6 | Word Search II | **Hard** | **LeetCode 212** | Trie + backtracking on a grid |
| 7 | Search Suggestions System | **Medium** | **LeetCode 1268** | autocomplete with trie + DFS |
| 8 | Replace Words | **Medium** | **LeetCode 648** | prefix matching in a sentence |
| 9 | Map Sum Pairs | **Medium** | **LeetCode 677** | trie with value accumulation |
| 10 | Count Distinct Substrings | **Medium** | GFG | *"Count of distinct substrings"* |

**Why these five.** #6 is THE hard trie problem — it combines trie-based pruning with grid backtracking, and it's asked more than any other trie problem in FAANG interviews. #7 is the practical autocomplete application. #8 is trie as a prefix dictionary. #9 tests value storage in trie nodes. #10 is the counting application — insert all suffixes of a string into a trie, count nodes.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Word Break (Trie approach) | **Medium** | **LeetCode 139** | trie + DP (already in `../26_dp`) |
| 12 | Palindrome Pairs | **Hard** | **LeetCode 336** | trie + palindrome checking |
| 13 | Maximum XOR With an Element From Array | **Hard** | **LeetCode 1707** | offline + sorted bit-trie |
| 14 | Stream of Characters | **Hard** | **LeetCode 1032** | reverse trie for suffix matching |

**Why these four.** #11 shows how a trie can accelerate DP — instead of checking every prefix with string slicing, walk the trie. #12 is the hardest pure trie problem. #13 extends #5 with an element constraint, requiring offline processing. #14 flips the trie on its head — store reversed words and match suffixes.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Longest Word in Dictionary | **Medium** | **LeetCode 720** | |
| Extra Characters in a String | **Medium** | **LeetCode 2707** | trie + DP |
| Implement Magic Dictionary | **Medium** | **LeetCode 676** | |
| Concatenated Words | **Hard** | **LeetCode 472** | trie + DP |
| Prefix and Suffix Search | **Hard** | **LeetCode 745** | |
| Short Encoding of Words | **Medium** | **LeetCode 820** | reverse trie |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5
Section 2   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 3   [ ] 11  [ ] 12  [ ] 13  [ ] 14
Section 4   [ ] ______ / 6
```
