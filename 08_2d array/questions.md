# 08 — 2D Arrays & Matrices: Practice Questions

> **Why 14 ranked problems.** Matrices are a narrower topic than 1D arrays — the algorithms
> reduce to four techniques (transpose+reverse, boundary shrinking, in-matrix flag storage,
> staircase search) plus careful index bookkeeping. Fourteen is what covers each technique
> with one problem to learn it and one to confirm it transferred, plus the traversal-order
> problems that build index fluency. Padding beyond that would just be more nested loops
> teaching the same thing. All six problems from your own `readme .md` list are included.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title.

**Test every solution on 1×N, N×1, and 1×1.** Square matrices hide almost every matrix bug —
that's how the missing guard in your `spiral.cpp` survived.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Transpose of a Matrix | Easy | **LeetCode 867** | `transpose-matrix` — on your list; also `01_2dArray.cpp:44` |
| 2 | Rotate Image (90° clockwise, in place) | **Medium** | **LeetCode 48** | `rotate-image` — on your list; also `01_2dArray.cpp:66` |
| 3 | Spiral Matrix | **Medium** | **LeetCode 54** | `spiral-matrix` — on your list; also `spiral.cpp` |
| 4 | Set Matrix Zeroes | **Medium** | **LeetCode 73** | `set-matrix-zeroes` — Striver Step 3 |
| 5 | Search a 2D Matrix | **Medium** | **LeetCode 74** | `search-a-2d-matrix` |

**Why these five.** #1 is the foundation — rotation, symmetry checks and column-major access
are all transpose in disguise, and doing the **non-square** case forces you to notice the
dimensions swap. **#2 is the highest-value matrix problem there is**: transpose + reverse, in
place, with the `j = i+1` triangle detail that catches everyone. #3 is boundary shrinking,
and its guard conditions are the most common matrix bug in existence — yours has one. **#4 is
the best O(1)-space problem in the folder**: the trick of storing your bookkeeping *inside*
the input is a genuinely transferable idea. #5 teaches the flatten/unflatten index mapping
(`idx/C`, `idx%C`) that turns a matrix into a 1D array.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 6 | Search a 2D Matrix II | **Medium** | **LeetCode 240** | `search-a-2d-matrix-ii` — on your list |
| 7 | Matrix multiplication | **Medium** | GFG | *"Multiply Matrices"* — fixes `multiplication.cpp` |
| 8 | Pascal's Triangle | Easy | **LeetCode 118** | `pascals-triangle` — on your list; cf. `../05_Function` #8 |
| 9 | Flipping an Image | Easy | **LeetCode 861** | `flipping-an-image` — on your list |
| 10 | Row with maximum number of 1s | **Medium** | GFG | *"Row with max 1s"* |

**Why these five.** **#6 paired with #5 is the point** — they look like the same problem and
need completely different algorithms, because #6's matrix is only row/column-sorted, not
globally sorted. Getting the distinction is worth more than either solution alone. #7 is
three nested loops where every index must be right, and **your current version has three
separate indexing errors** — it's the best debugging exercise here. #8 connects to the
recurrence work in `../05_Function` and is the simplest jagged (non-rectangular) matrix. #9
is two-pointer reverse fused with an in-place invert — one pass instead of two. **#10 is a
staircase search in disguise**: the O(R+C) solution starts top-right and never revisits a
column, which is #6's technique applied to a different question.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Toeplitz Matrix | Easy | **LeetCode 766** | `toeplitz-matrix` |
| 12 | Print matrix diagonals | **Medium** | GFG | *"Print Matrix Diagonally"* |
| 13 | Boundary traversal of a matrix | Easy | GFG | *"Boundary traversal of matrix"* |
| 14 | Print matrix in wave / snake form | Easy | GFG | *"Print Matrix in snake pattern"* — `01_2dArray.cpp:100` |

**Why these four.** #11 is a one-line condition (`a[i][j] == a[i-1][j-1]`) once you see that a
diagonal is defined by constant `i - j`, and that observation drives every diagonal problem.
#12 makes the `i + j` grouping explicit — cells on the same anti-diagonal share `i + j`, which
is the indexing insight behind diagonal DP later. #13 is spiral traversal's first iteration
only, and it has the same duplicate-edge trap for single rows and columns — good reinforcement
of #3. #14 is the simplest possible direction-alternating traversal and you've already written
it correctly.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Spiral Matrix II | **Medium** | **LeetCode 59** | #3 inverted — *write* 1..n² in spiral order instead of reading |
| Rotate image 90° **anticlockwise** | **Medium** | GFG | transpose then reverse **columns** — the standard follow-up to #2 |
| Matrix Diagonal Sum | Easy | **LeetCode 1572** | both diagonals in one pass; the odd-size centre is double-counted |
| Game of Life | **Medium** | **LeetCode 289** | in-place with 2-bit encoding — #4's "store extra state in the input" idea |
| Number of Islands | **Medium** | **LeetCode 200** | **forward reference** — grid DFS/BFS, needs `../27_graphs` |
| Rotting Oranges | **Medium** | **LeetCode 994** | **forward reference** — multi-source BFS, `../27_graphs` |
| Transpose a non-square matrix | Easy | *Drill* | R×C becomes C×R, so in-place is impossible — say why |
| Check if a matrix is symmetric / identity | Easy | *Drill* | one-line conditions on `a[i][j]` vs `a[j][i]` |
| Print matrix in Z form | Easy | GFG | first row, anti-diagonal, last row |
| Sum of matrix border elements | Easy | *Drill* | the border condition from `readme.md` §3 — watch the double-counted corners |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5
Section 2   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 3   [ ] 11  [ ] 12  [ ] 13  [ ] 14
Section 4   [ ] ______ / 10
```
