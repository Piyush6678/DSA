# 10 — Recursion & Backtracking: Practice Questions

> **Why 28 ranked problems — the largest set in the repo so far.** Recursion is not one
> technique, it is the substrate for four later folders (`../21_tree`, `../22_bst`, `../26_dp`,
> `../27_graphs`). The set breaks into seven families that transfer independently: basic
> linear recursion, divide & conquer, pick/not-pick on subsequences, combinations with the
> three separate duplicate-handling rules, permutations, constraint backtracking, and
> string/grid recursion. Each family needs 3–5 problems before the pattern is yours rather
> than memorised, and the duplicate-handling trio in particular is worth doing back to back
> because the difference between them *is* the lesson. Below 28 you drop a family; above it
> you are re-solving Subsets with new labels.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title.

**Scope note.** Everything here needs only constructs from `01`–`09`. Memoisation is mentioned
where it applies but belongs to `../26_dp`; those notes are marked **`[dp]`** and are *not*
required to solve the problem.

**All six problems from your `readme.md` are in Sections 1–2** (#8, #9, #10, #12, #15, #16).

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Print 1 to N and N to 1 (both forms) | Easy | *Drill* | Striver 4.1 — `01_recursion.cpp:12,20,27` |
| 2 | Factorial / Sum of first N | Easy | *Drill* | Striver 4.1 — `01_recursion.cpp:3,34` |
| 3 | Fibonacci Number | Easy | **LeetCode 509** | `fibonacci-number` — `01_recursion.cpp:56` |
| 4 | Climbing Stairs | Easy | **LeetCode 70** | `climbing-stairs` — `01_recursion.cpp:76` |
| 5 | Reverse an array / string recursively | Easy | **LeetCode 344** | `reverse-string` |
| 6 | Valid Palindrome (recursive) | Easy | **LeetCode 125** | `valid-palindrome` — `palindrome.cpp` |
| 7 | Pow(x, n) — binary exponentiation | **Medium** | **LeetCode 50** | `powx-n` — `01_recursion.cpp:62` |
| 8 | Subsets | **Medium** | **LeetCode 78** | `subsets` — **on your list**; `subsets.cpp` |
| 9 | Subsets II | **Medium** | **LeetCode 90** | `subsets-ii` — **on your list**; `subsets.cpp:34` |
| 10 | Combination Sum | **Medium** | **LeetCode 39** | `combination-sum` — **on your list**; `combinationSum.cpp` |
| 11 | Combination Sum II | **Medium** | **LeetCode 40** | `combination-sum-ii` |
| 12 | Generate Parentheses | **Medium** | **LeetCode 22** | `generate-parentheses` — **on your list**; **your version is correct** |
| 13 | Permutations | **Medium** | **LeetCode 46** | `permutations` — `Permutationl.cpp` |

**Why these thirteen.** #1–#2 are where you prove you can find a base case; do them in *both*
the parameterised and functional style, because #1's "print before vs. after the call" is the
same idea as pre-order vs. post-order traversal in `../21_tree`. #3 and #4 are the same
recurrence wearing different clothes — noticing that is the point, and #3 is the standard
motivating example for memoisation. #5–#6 are two-pointer recursion and the place to fix the
`i == j` base case for good. **#7 is the first genuinely important one**: halving instead of
decrementing turns O(n) into O(log n), and the negative-exponent and `INT_MIN` cases are what
the interviewer is actually testing.

**#8–#13 are the heart of the folder.** #8 is pick/not-pick in its purest form. Then #9, #10,
#11 are three *different* duplicate rules and must be done consecutively — #9 skips duplicates
at the same depth, #10 reuses the same index to allow unlimited repeats, #11 does both at once.
Doing them apart teaches you three tricks; doing them together teaches you the principle. #12
is constraint pruning (`close > open`) and is the cleanest small backtracking problem there is.
#13 introduces the swap-and-undo idiom.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 14 | Tower of Hanoi | **Medium** | GFG | *"Tower Of Hanoi"* — `problems.cpp:44`, **yours is correct** |
| 15 | K-th Symbol in Grammar | **Medium** | **LeetCode 779** | `k-th-symbol-in-grammar` — **on your list**; **yours is correct** |
| 16 | Count and Say | **Medium** | **LeetCode 38** | `count-and-say` — **on your list**; `countAndSay.cpp` |
| 17 | Subsequences whose sum equals K (print all) | **Medium** | GFG | *"Subsets with sum K"* — Striver 4.3 |
| 18 | Count subsequences with sum K | **Medium** | GFG | *"Perfect Sum Problem"* — the count/any/print trio |
| 19 | Subsequences of a string | Easy | GFG | *"Print all subsequences of a string"* — `susequence.cpp` |
| 20 | Unique Paths in a grid | **Medium** | **LeetCode 62** | `unique-paths` — `maize.cpp`, **yours is correct** |
| 21 | Letter Combinations of a Phone Number | **Medium** | **LeetCode 17** | `letter-combinations-of-a-phone-number` |
| 22 | Binary strings with no consecutive 1s | **Medium** | GFG | *"Binary Strings"* — `binarystrings.cpp`, **yours is correct** |
| 23 | Palindrome Partitioning | **Medium** | **LeetCode 131** | `palindrome-partitioning` |

**Why these ten.** #14 is the canonical "trust the recursion" problem — you cannot trace it in
your head past n=3, and learning to stop trying is a real skill. **#15 is the best problem in
this section**: the naive approach builds a string of length 2ⁿ⁻¹ and blows up, while the
insight that position `k`'s value depends only on its parent makes it O(n) time and O(1)
extra. #16 is recursion over the *previous answer* rather than over a smaller input.

**#17–#19 are the pick/not-pick trio and deserve to be done together**: print-all, count, and
return-on-first-success are the same recursion with three different return types, and the
"return `true` to stop early" version is the one that transfers to real backtracking. #20–#22
are grid and string recursion, all of which you already have working code for. #23 combines
recursion with a palindrome check and is the first problem where pruning genuinely matters.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 24 | N-Queens | **Hard** | **LeetCode 51** | `n-queens` — the canonical backtracking problem |
| 25 | Sudoku Solver | **Hard** | **LeetCode 37** | `sudoku-solver` — backtracking returning `bool` |
| 26 | Rat in a Maze | **Medium** | GFG | *"Rat in a Maze Problem - I"* — grid backtracking with a visited matrix |
| 27 | Word Search | **Medium** | **LeetCode 79** | `word-search` — DFS with undo on a grid |
| 28 | Sort an array / stack recursively | **Medium** | GFG | *"Sort an array using recursion"* |

**Why these five.** **#24 is the problem to be able to write from memory** — it is the reference
implementation of "place, recurse, remove", and the O(1) diagonal check
(`abs(col[i] - c) == r - i`) is a genuinely elegant observation worth carrying. #25 is #24 with
a `bool` return, which is the important variation: as soon as one branch succeeds you must
propagate `true` all the way up rather than continuing to search. #26 and #27 are the same
skeleton on a grid, and they are direct preparation for the DFS in `../27_graphs` — do them and
graph traversal will feel familiar rather than new. #28 is deliberately strange (recursion where
a loop is obviously better) and exists to prove you can express *any* iteration recursively.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Merge Sort | **Medium** | GFG | *"Merge Sort"* — divide & conquer; you write it in `../12_sorting` |
| Quick Sort | **Medium** | GFG | *"Quick Sort"* — partition + two recursive calls |
| Binary Search (recursive) | Easy | **LeetCode 704** | `binary-search` — 1 branch, not 2; O(log n) depth |
| Combination Sum III | **Medium** | **LeetCode 216** | fixed count *and* fixed sum — two constraints at once |
| Permutations II (duplicates) | **Medium** | **LeetCode 47** | the §1 #9 skip rule applied to permutations |
| Subsets sum — return any one | **Medium** | GFG | *"Subset Sum Problem"* — `bool` return, stop at the first hit |
| Generate all balanced BSTs / Catalan count | **Medium** | GFG | *"Unique BSTs"* — same Catalan numbers as #12 |
| Josephus Problem | **Medium** | GFG | *"Josephus problem"* — `(josephus(n-1,k) + k) % n` |
| Print all paths in a grid | **Medium** | GFG | *"Print all possible paths from top left to bottom right"* — `maize.cpp:9` |
| Word Break | **Medium** | **LeetCode 139** | plain recursion first, then memoise `[dp]` |
| Count paths with obstacles | **Medium** | **LeetCode 63** | #20 plus a blocked-cell base case |
| Generate all k-length strings from an alphabet | Easy | *Drill* | `problems.cpp:54` — your `kstrings` |
| Remove all occurrences of a character | Easy | *Drill* | `problems.cpp:14` — index version, not `substr` |
| Find max/min of an array recursively | Easy | *Drill* | `problems.cpp:33` — watch the initial `max` |
| Check if an array is sorted, recursively | Easy | *Drill* | one comparison plus one call |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7
            [ ] 8   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13
Section 2   [ ] 14  [ ] 15  [ ] 16  [ ] 17  [ ] 18  [ ] 19  [ ] 20  [ ] 21  [ ] 22  [ ] 23
Section 3   [ ] 24  [ ] 25  [ ] 26  [ ] 27  [ ] 28
Section 4   [ ] ______ / 15
```
