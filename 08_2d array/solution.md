# 08 — 2D Arrays & Matrices: Solutions

Full C++14 for Sections 1–3, approach-only for Section 4.
**Every algorithm below was compiled and executed before publication**, including on the
1×N / N×1 / 1×1 edge cases that matrix bugs hide in.

Assume `#include <iostream> <vector> <algorithm> <climits>` and `using namespace std;`,
with `typedef vector<vector<int>> M;`.

---

# Section 1 — Must Do

## 1. Transpose of a Matrix — LeetCode 867

```cpp
M transpose(const M &a) {
    int R = a.size(), C = a[0].size();
    M t(C, vector<int>(R));                 // NOTE the swapped dimensions
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < C; ++j)
            t[j][i] = a[i][j];
    return t;
}
```

**For a non-square matrix you must allocate.** An R×C matrix transposes to C×R, so the shape
changes and in-place is impossible. Saying that unprompted is the point of this problem.

**For a square matrix, in place, O(1) space:**

```cpp
void transposeSquare(M &a) {
    int n = a.size();
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)     // j = i+1 : ONE triangle only
            swap(a[i][j], a[j][i]);
}
```

**`j = i + 1` is the graded detail.** Looping the full square swaps every pair **twice**,
which restores the original matrix — a silent no-op that looks like working code.

Your `01_2dArray.cpp:56-64` uses `if (i == j) break;` inside a full inner loop, which covers
the *lower* triangle (`j < i`) instead. **That is equally correct** — each off-diagonal pair
is still touched exactly once.

**Dry run** — `[[1,2,3],[4,5,6]]` (2×3) → `[[1,4],[2,5],[3,6]]` (3×2) ✓ *Verified.*

**Complexity:** O(R·C) time; O(R·C) space for the general case, O(1) for square in-place.

---

## 2. Rotate Image 90° clockwise — LeetCode 48

```cpp
void rotate(M &a) {
    int n = a.size();
    for (int i = 0; i < n; ++i)                       // 1. transpose
        for (int j = i + 1; j < n; ++j)
            swap(a[i][j], a[j][i]);
    for (int i = 0; i < n; ++i)                       // 2. reverse each row
        reverse(a[i].begin(), a[i].end());
}
```

**Why two reflections make a rotation.** Transposing reflects across the main diagonal;
reversing each row reflects across the vertical axis. Composing two reflections about axes
meeting at 45° yields a rotation of 90°.

**Dry run** — `[[1,2,3],[4,5,6],[7,8,9]]`:

| step | result |
|---|---|
| transpose | `1 4 7 / 2 5 8 / 3 6 9` |
| reverse rows | `7 4 1 / 8 5 2 / 9 6 3` ✓ |

*Verified.*

**Anticlockwise** = transpose, then reverse each **column** (equivalently: reverse the row
*order* first, then transpose). This is the standard follow-up — know both.

**Your `01_2dArray.cpp:66-86` uses exactly this method and the approach is correct** — you
transpose, then swap column `i` with column `n-1-i` across all rows, which is reversing each
row. The only problem in that file is the `int r` redeclaration that stops it compiling, and
the VLA. See the bugs section.

**Complexity:** O(n²) time, **O(1) space**.

---

## 3. Spiral Matrix — LeetCode 54

```cpp
vector<int> spiralOrder(const M &a) {
    vector<int> res;
    if (a.empty()) return res;
    int top = 0, bottom = a.size() - 1, left = 0, right = a[0].size() - 1;

    while (top <= bottom && left <= right) {
        for (int j = left; j <= right; ++j) res.push_back(a[top][j]);
        ++top;
        for (int i = top; i <= bottom; ++i) res.push_back(a[i][right]);
        --right;
        if (top <= bottom) {                                   // GUARD
            for (int j = right; j >= left; --j) res.push_back(a[bottom][j]);
            --bottom;
        }
        if (left <= right) {                                   // GUARD
            for (int i = bottom; i >= top; --i) res.push_back(a[i][left]);
            ++left;
        }
    }
    return res;
}
```

**The two inner guards are the entire difficulty.** After the first two passes, `top` and
`right` have already moved inward. For a **single-row** matrix the top pass consumed the whole
row, so `top > bottom` — without the guard the bottom pass walks the same row backwards. Same
story for a **single column** and the left pass.

**Your `spiral.cpp` has the first guard (`if (minr > maxr) break;`) but not the second.** I
generalised your code and ran it: a **5×1 column outputs `1 2 3 4 5 4 3 2`** — three elements
re-printed. Your 4×5 test matrix can't expose it.

**Verified on four shapes:**

| input | output |
|---|---|
| 4×5 | `1 2 3 4 5 10 15 20 19 18 17 16 11 6 7 8 9 14 13 12` ✓ |
| 1×5 | `1 2 3 4 5` ✓ |
| 5×1 | `1 2 3 4 5` ✓ |
| 3×3 | `1 2 3 6 9 8 7 4 5` ✓ |

**Always test spiral on 1×N, N×1 and 1×1.** A square matrix passes with the buggy version.

**Complexity:** O(R·C) time, O(1) extra beyond the output.

---

## 4. Set Matrix Zeroes — LeetCode 73

> If any cell is 0, zero its entire row and column. **O(1) extra space.**

**Why you can't just zero as you go:** a newly written 0 is indistinguishable from an original
one, so it would cascade and blank the whole matrix. You must record first, write second.

### Approach 1 — two marker arrays, O(R+C) space

`vector<bool> row(R), col(C);` mark in pass 1, apply in pass 2. Correct, and the right thing
to say first.

### Approach 2 — Optimal: store the markers inside the matrix, O(1) space

```cpp
void setZeroes(M &a) {
    int R = a.size(), C = a[0].size();
    bool col0 = false;                          // separate flag for column 0

    for (int i = 0; i < R; ++i) {
        if (a[i][0] == 0) col0 = true;          // column-0 zeros tracked separately
        for (int j = 1; j < C; ++j)             // note: j starts at 1
            if (a[i][j] == 0) { a[i][0] = 0; a[0][j] = 0; }
    }

    for (int i = R - 1; i >= 0; --i) {          // BOTTOM-UP, RIGHT-TO-LEFT
        for (int j = C - 1; j >= 1; --j)
            if (a[i][0] == 0 || a[0][j] == 0) a[i][j] = 0;
        if (col0) a[i][0] = 0;                  // column 0 last, per row
    }
}
```

**The idea:** row 0 and column 0 are exactly R+C cells, and they're going to be zeroed anyway
if they contain a zero — so use them as the marker arrays. That's O(1) extra.

**Two details make it correct, and both come from one problem — `a[0][0]` is shared:**

1. **`col0` as a separate flag.** `a[0][0]` would otherwise have to mean both "row 0 has a
   zero" and "column 0 has a zero". We let it mean only the first, and track the second in a
   variable.
2. **The second pass runs backwards.** Going top-left to bottom-right would overwrite the
   markers in row 0 and column 0 before the later rows read them.

**Dry run** — `[[1,1,1],[1,0,1],[1,1,1]]`: the 0 at (1,1) sets `a[1][0] = 0` and
`a[0][1] = 0`. Backwards pass zeroes every cell whose row-marker or column-marker is 0 →
`1 0 1 / 0 0 0 / 1 0 1` ✓ *Verified, along with `[[0,1,2,0],[3,4,5,2],[1,3,1,5]]` →
`0 0 0 0 / 0 4 5 0 / 0 3 1 0`.*

**Complexity:** O(R·C) time, **O(1) space**.

---

## 5. Search a 2D Matrix — LeetCode 74

> Each row sorted, **and each row's first element exceeds the previous row's last.**

```cpp
bool searchMatrix(const M &a, int target) {
    int R = a.size(), C = a[0].size();
    int lo = 0, hi = R * C - 1;                 // treat it as ONE sorted array
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;           // not (lo+hi)/2 — overflow
        int val = a[mid / C][mid % C];          // unflatten: row = idx/C, col = idx%C
        if      (val == target) return true;
        else if (val <  target) lo = mid + 1;
        else                    hi = mid - 1;
    }
    return false;
}
```

**The whole problem is the index mapping.** Because of the stronger sortedness guarantee, the
matrix read row by row *is* a sorted sequence. So binary search the range `[0, R*C)` and
convert each index to a cell with `(idx / C, idx % C)` — the inverse of the row-major
`i*C + j` from `readme.md` §1.

**`R * C` can overflow** for very large matrices; use `long long` if the constraints allow it.

**Complexity:** **O(log(R·C))** time, O(1) space.
*Verified: target 3 → true, target 13 → false on `[[1,3,5,7],[10,11,16,20],[23,30,34,60]]`.*

---

# Section 2 — Important

## 6. Search a 2D Matrix II — LeetCode 240

> Rows sorted left-to-right, columns sorted top-to-bottom. **No relation between rows.**

```cpp
bool searchMatrixII(const M &a, int target) {
    int i = 0, j = (int)a[0].size() - 1;        // START at the TOP-RIGHT corner
    while (i < (int)a.size() && j >= 0) {
        if      (a[i][j] == target) return true;
        else if (a[i][j] >  target) --j;        // too big -> whole column below is bigger
        else                        ++i;        // too small -> whole row left is smaller
    }
    return false;
}
```

**Binary search over the flattened array is INVALID here.** Without the "next row starts
above the previous row's end" guarantee, the row-major sequence is not sorted. Applying #5's
solution to this problem is the single most common mistake, and it passes small tests.

**Why top-right is special.** That cell is the **maximum of its row** and the **minimum of its
column**. So:
- If it's larger than the target, nothing in that column can be the target → drop the column.
- If it's smaller, nothing in that row can be the target → drop the row.

Each comparison eliminates an entire row or column. Bottom-left has the mirror property and
works identically. **Top-left and bottom-right have neither** — from the top-left, a larger
value tells you nothing about which direction to go, which is exactly why starting there
fails.

**Complexity:** **O(R + C)** time, O(1) space.
*Verified: target 5 → true, target 20 → false.*

---

## 7. Matrix multiplication

```cpp
M multiply(const M &A, const M &B) {
    int R = A.size(), N = A[0].size(), C = B[0].size();
    // A is R x N, B must be N x C, result is R x C
    M res(R, vector<int>(C, 0));
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < C; ++j) {
            long long sum = 0;                          // long long: N terms can overflow
            for (int k = 0; k < N; ++k)
                sum += (long long)A[i][k] * B[k][j];    // i,k and k,j — NOT R,k and k,C
            res[i][j] = (int)sum;
        }
    return res;
}
```

**`res[i][j] = Σₖ A[i][k] · B[k][j]`** — row `i` of A dotted with column `j` of B. Note `k` is
the *shared* dimension and appears as the **second** index of A and the **first** of B. If
your inner expression doesn't have that shape, it's wrong.

**Dimensions must agree:** A is R×N, B is N×C. Multiplication is undefined otherwise, so check
`A[0].size() == B.size()` in real code.

**Your `multiplication.cpp` has three separate indexing errors** — it uses the *dimensions*
`r` and `c` where the *loop counters* `i` and `j` belong. I ran it side by side with the
correct version:

| | row 0 | row 1 | row 2 | row 3 |
|---|---|---|---|---|
| **yours** | 45 45 45 45 | 45 45 45 45 | 45 45 45 45 | 45 45 45 45 |
| **correct** | 14 14 14 14 | 35 35 35 35 | 66 66 66 66 | 42 42 42 42 |

Details in the bugs section. *Verified.*

**Complexity:** **O(R·N·C)** time, O(R·C) space. (Strassen's algorithm is
O(n^2.807) — worth naming, never worth writing.)

---

## 8. Pascal's Triangle — LeetCode 118

```cpp
M generate(int n) {
    M res;
    for (int i = 0; i < n; ++i) {
        vector<int> row(i + 1, 1);                       // ends are always 1
        for (int j = 1; j < i; ++j)
            row[j] = res[i-1][j-1] + res[i-1][j];        // sum of the two above
        res.push_back(row);
    }
    return res;
}
```

**The additive recurrence** `C(i,j) = C(i-1,j-1) + C(i-1,j)`. Initialising the row to all `1`s
handles both endpoints for free, and the inner loop runs `1 .. i-1`, so rows 0 and 1 skip it
entirely.

**This is a jagged matrix** — row `i` has `i+1` elements. `vector<vector<int>>` represents that
naturally; a fixed `int[n][n]` would waste half its space.

**Three ways to get a Pascal entry, and they're all in this repo:**
- **Additive recurrence** (here) — O(n²), no overflow of intermediates, the DP view.
- **Multiplicative** `C(i,j+1) = C(i,j)·(i-j)/(j+1)` — `../05_Function` #8, and what your
  `lec9.cpp` uses. O(n) per row.
- **Factorials** — `../05_Function` #7, and the one that overflows.

**LeetCode 119** asks for a single row in O(k) space — use the multiplicative form.

**Complexity:** O(n²) time and space, which is optimal since the output is n²/2 numbers.
*Verified for n=5.*

---

## 9. Flipping an Image — LeetCode 861

> Reverse each row, then invert every bit.

```cpp
M flipAndInvertImage(M a) {
    for (auto &row : a) {                    // note: auto& — modifies in place
        int l = 0, r = (int)row.size() - 1;
        while (l < r) {
            swap(row[l], row[r]);
            row[l] ^= 1; row[r] ^= 1;        // invert BOTH while we're here
            ++l; --r;
        }
        if (l == r) row[l] ^= 1;             // odd length: the middle element
    }
    return a;
}
```

**One pass instead of two.** The naive solution reverses each row, then loops again to invert.
Since the two-pointer reverse already visits every element exactly once, invert during the
swap.

**`if (l == r)` is the graded line.** For an odd-length row the two pointers land on the same
middle cell and the `while` exits without processing it — so it never gets inverted. This is
the same middle-element subtlety as the two-pointer reverse in `../06_Pointer` #13.

**`x ^= 1` flips a 0/1 bit** — cleaner than `x = 1 - x`, and idiomatic.

**`auto &row`** — without the `&` you'd modify a copy and the function would return the input
unchanged.

**Dry run** — `[[1,1,0],[1,0,1],[0,0,0]]` → `[[1,0,0],[0,1,0],[1,1,1]]` ✓ *Verified.*

**Complexity:** O(R·C) time, O(1) extra.

---

## 10. Row with maximum number of 1s

> Each row is sorted (all 0s then all 1s). Find the row with the most 1s.

### Approach 1 — Brute force
Count 1s in every row: **O(R·C)**.

### Approach 2 — Binary search per row
Find the first 1 in each row: **O(R log C)**.

### Approach 3 — Optimal: staircase, O(R + C)

```cpp
int rowWithMaxOnes(const M &a) {
    int R = a.size(), C = a[0].size();
    int i = 0, j = C - 1, best = -1;
    while (i < R && j >= 0) {
        if (a[i][j] == 1) { best = i; --j; }   // this row reaches further left -> new best
        else              ++i;                 // no 1 here -> this row can't beat best
    }
    return best;
}
```

**This is #6's staircase applied to a different question.** Start top-right. If the current
cell is a 1, this row extends at least as far left as any previous best, so record it and move
left. If it's a 0, this row cannot beat the current best (its 1s start further right), so drop
down.

`j` only ever decreases and `i` only ever increases, so the total work is O(R + C).

**`best = -1`** handles an all-zero matrix — a case the brute force gets right by accident and
this version needs to state.

**Complexity:** **O(R + C)** time, O(1) space.
*Verified: `[[0,1,1,1],[0,0,1,1],[1,1,1,1],[0,0,0,0]]` → row 2; all-zeros → −1.*

---

# Section 3 — Good to Know

## 11. Toeplitz Matrix — LeetCode 766

> Every diagonal from top-left to bottom-right has the same value.

```cpp
bool isToeplitzMatrix(const M &a) {
    for (size_t i = 1; i < a.size(); ++i)
        for (size_t j = 1; j < a[0].size(); ++j)
            if (a[i][j] != a[i-1][j-1]) return false;
    return true;
}
```

**One comparison per cell.** You don't need to walk diagonals at all — if every cell equals
its up-left neighbour, then by transitivity every diagonal is constant.

Starting both loops at 1 avoids the out-of-bounds check entirely; the first row and column
have no up-left neighbour and need no test.

**The underlying identity:** cells on the same top-left→bottom-right diagonal share a constant
**`i - j`**. (Cells on an *anti*-diagonal share `i + j` — used in #12.) Those two facts drive
every diagonal problem, including diagonal DP.

**The follow-up** — "what if the matrix is too large to fit in memory and you can only load a
few rows?" — is answered by exactly this formulation: you only ever need two adjacent rows at
a time.

**Complexity:** O(R·C) time, O(1) space. *Verified true and false cases.*

---

## 12. Print matrix diagonals

```cpp
void printDiagonals(const M &a) {
    int R = a.size(), C = a[0].size();
    for (int s = 0; s <= R + C - 2; ++s) {        // s = i + j, the anti-diagonal id
        for (int i = 0; i < R; ++i) {
            int j = s - i;
            if (j >= 0 && j < C) cout << a[i][j] << " ";
        }
        cout << "| ";
    }
}
```

**Cells on the same anti-diagonal share `i + j`.** So iterate `s` from 0 to `R+C-2` and, for
each, emit every in-bounds `(i, s-i)`. The bounds check is what handles the diagonals that
don't span the full matrix.

**Dry run** — 3×3 `[[1,2,3],[4,5,6],[7,8,9]]` → `1 | 2 4 | 3 5 7 | 6 8 | 9` ✓ *Verified.*

**For top-left→bottom-right diagonals**, group by `i - j` instead, which ranges over
`-(C-1) .. (R-1)` — offset it by `C-1` to index an array.

**LeetCode 498 (Diagonal Traverse)** adds alternating direction: reverse the emission order
when `s` is odd. That's the wave idea from #14 applied to diagonals.

**Complexity:** O(R·C) work spread over O(R+C) diagonals; the naive `i` loop per diagonal
makes it O((R+C)·R), so index the range directly if R is large.

---

## 13. Boundary traversal

```cpp
vector<int> boundaryTraversal(const M &a) {
    int R = a.size(), C = a[0].size();
    vector<int> res;
    if (R == 1) { for (int j = 0; j < C; ++j) res.push_back(a[0][j]); return res; }
    if (C == 1) { for (int i = 0; i < R; ++i) res.push_back(a[i][0]); return res; }

    for (int j = 0;   j <  C; ++j) res.push_back(a[0][j]);        // top, left to right
    for (int i = 1;   i <  R; ++i) res.push_back(a[i][C-1]);      // right, top to bottom
    for (int j = C-2; j >= 0; --j) res.push_back(a[R-1][j]);      // bottom, right to left
    for (int i = R-2; i >= 1; --i) res.push_back(a[i][0]);        // left, bottom to top
    return res;
}
```

**This is one iteration of the spiral (#3), and it has the same trap.** Without the `R == 1`
and `C == 1` early returns, a single-row matrix gets traversed by both the top and bottom
loops — the identical duplicate-edge bug that's in your `spiral.cpp`.

**The starting offsets prevent corner double-counting:** the right pass starts at `i = 1`
(the top-right corner was already emitted), the bottom pass at `j = C-2`, the left pass at
`i = R-2` and stops at `i >= 1`.

**Dry run** — 3×4 → `1 2 3 4 8 12 11 10 9 5` ✓ · 1×3 → `1 2 3` ✓ · 3×1 → `1 2 3` ✓
*All verified.*

**Complexity:** O(R + C) time, O(R+C) output.

---

## 14. Wave / snake traversal

```cpp
void wavePrint(const M &a) {
    int R = a.size(), C = a[0].size();
    for (int i = 0; i < R; ++i) {
        if (i % 2 == 0) for (int j = 0;   j <  C; ++j) cout << a[i][j] << " ";
        else            for (int j = C-1; j >= 0; --j) cout << a[i][j] << " ";
    }
}
```

**The simplest direction-alternating traversal**: even rows left-to-right, odd rows
right-to-left. The `i % 2` test is the entire algorithm.

**Your `01_2dArray.cpp:100-118` implements this correctly.**

**Dry run** — 3×3 → `1 2 3 6 5 4 7 8 9` ✓ *Verified.*

**The column-wise variant** (used by some "wave print" problems) alternates *columns* instead
of rows — swap the roles of `i` and `j`. Read which is being asked.

**Complexity:** O(R·C) time, O(1) extra.

---

# Section 4 — Extra Practice (approach only)

### Spiral Matrix II — LeetCode 59 (Medium)
Generate an n×n matrix filled with 1..n² in spiral order. **The identical boundary-shrinking
loop as #3**, writing `counter++` instead of reading. Because it's square and you fill exactly
n² cells, you can loop `while (num <= n*n)` instead of on the boundaries — but keep the guards
if you generalise to rectangles. **O(n²) time, O(1) extra.**

### Rotate 90° anticlockwise — GFG (Medium)
Transpose, then reverse each **column** (i.e. reverse the *order of the rows*). Equivalently,
reverse the rows first, then transpose. Deriving it from #2 rather than memorising it is the
exercise — it's the same two reflections composed in the other order. **O(n²) time, O(1) space.**

### Matrix Diagonal Sum — LeetCode 1572 (Easy)
One pass: add `a[i][i]` and `a[i][n-1-i]`. **For odd `n` the centre cell belongs to both
diagonals** and gets counted twice — subtract `a[n/2][n/2]` once at the end. That single
subtraction is the whole problem. **O(n) time, O(1) space.**

### Game of Life — LeetCode 289 (Medium)
Every cell must update based on the *original* neighbour states, so naive in-place updating
corrupts later cells. The O(1)-space trick is **2-bit encoding**: keep the current state in
bit 0 and write the next state into bit 1, then shift everything right in a second pass.
Same "store extra state inside the input" idea as #4. **O(R·C) time, O(1) space.**

### Number of Islands — LeetCode 200 · **forward reference**
Scan for an unvisited `'1'`, then flood-fill its whole component with DFS or BFS, counting one
island per fill. Needs `../27_graphs`. The grid *is* the graph — each cell has up to four
neighbours — which is the key reframing. **O(R·C) time, O(R·C) space worst case.**

### Rotting Oranges — LeetCode 994 · **forward reference**
Multi-source BFS: seed the queue with **every** rotten orange at once and expand level by
level, counting levels as minutes. Needs `../27_graphs`. It's on the list in
`../27_graphs/readme.md`. **O(R·C) time and space.**

### Transpose a non-square matrix — *Drill*
R×C becomes C×R, so the result cannot share storage with the input — **in-place is
impossible**. Articulating *why* matters more than the code. (In-place transposition of a
non-square matrix *is* possible via cycle-following permutation, but it's O(R·C) extra time
and genuinely intricate — worth knowing it exists.) **O(R·C) time and space.**

### Symmetric / identity matrix check — *Drill*
Symmetric: `a[i][j] == a[j][i]` for all `j > i` — one triangle, same as #1. Identity:
`a[i][j] == (i == j ? 1 : 0)`. Both are one-line conditions inside a double loop, and both let
you **return early** on the first violation. **O(n²) worst case, often much faster.**

### Print matrix in Z form — GFG (Easy)
Entire first row, then the anti-diagonal (`a[i][n-1-i]` for the middle rows), then the entire
last row. The only trap is not double-printing the two corners the diagonal shares with the
first and last rows. **O(n) time.**

### Sum of border elements — *Drill*
Use the border condition from `readme.md` §3, or sum row 0, row R−1, column 0 and column C−1
and subtract the **four corners**, which are each counted twice. For R or C equal to 1 the
formula degenerates — handle it separately, exactly as in #13. **O(R+C) time.**

---

# Bugs in your current code

Per the repo's convention I have not edited your `.cpp` files. All confirmed by compiling
and running.

### `08_2d array/01_2dArray.cpp` — does not compile

| Line | Problem | Fix |
|---|---|---|
| 52 | **`int r = 4;` redeclares `int r = 5;` from line 11.** `g++ -fsyntax-only` reports *"redeclaration of 'int r'"* — the file has never compiled | rename to `int sq = 4;`, or scope each experiment in its own `{ }` block |
| 28 | `int max = -999` fails for any matrix whose elements are all below −999 | `INT_MIN`, or `arr[0][0]` |
| 28 | The variable `max` shadows `std::max` under `using namespace std` | rename to `mx` |
| 68 | `int rotate[n][n]` with runtime `n` is a **VLA** (a GCC extension, not standard C++), and `rotate` also shadows `std::rotate` | `vector<vector<int>> rot(n, vector<int>(n));` |
| 53 | `int trr[4][4] = {1,2,3,45,6,7,8,9,10,12,13,14,15,16};` supplies **14 initialisers for 16 slots** — the last two are silently zero. `45` looks like a typo for `4,5` | supply all 16 |
| 11 | `c` is declared and never used | drop it |

**What's right in this file, and it's a lot:**
- **The transpose at lines 56-64 is correct.** `if (i == j) break;` inside a full inner loop
  restricts you to the lower triangle, so each off-diagonal pair is swapped exactly once —
  the trap this problem is famous for, and you avoided it.
- **The rotation at lines 66-86 uses the right method** — transpose, then swap column `i` with
  column `n-1-i`, which is reversing each row. That's solution #2.
- **The non-square transpose at lines 44-50** correctly writes `brr[i][j] = arr[j][i]` with
  `brr` being 4×3 and `arr` 3×4 — you got the dimension swap right.
- **The wave print at lines 100-118 is correct** (solution #14).
- Your comment at line 4 about the column dimension being required is exactly right.

### `08_2d array/multiplication.cpp` — completely wrong output

Three separate indexing errors on one line:

```cpp
res[j][i] += (arr[r][k] * brr[k][c]);
//   ^  ^        ^              ^
//   |  |        |              +-- c is the COLUMN COUNT (4), not the loop index j
//   |  |        +----------------- r is the ROW COUNT (4), not the loop index i
//   +--+-------------------------- indices transposed; should be res[i][j]
```

`arr` is declared `int arr[r][n]` = 4×3, so **`arr[4][k]` is out of bounds** (valid rows are
0–3). Likewise `brr` is 3×4, so **`brr[k][4]` is out of bounds**. Both are undefined
behaviour, reading whatever is adjacent in memory.

**Measured** — every cell comes out as `45`:

```
yours:    45 45 45 45   |   correct:  14 14 14 14
          45 45 45 45   |             35 35 35 35
          45 45 45 45   |             66 66 66 66
          45 45 45 45   |             42 42 42 42
```

The fix is `res[i][j] += arr[i][k] * brr[k][j];` — see solution #7. Also note `res` is
declared `int res[r][c]` and never initialised before `+=`; the inner `res[j][i] = 0;` does
initialise, but only because `r == c == 4` here. With non-square dimensions it would index out
of bounds too.

### `08_2d array/spiral.cpp` — missing one guard

| Line | Problem | Fix |
|---|---|---|
| 24-28 | **No guard before the "up" pass.** You correctly guard the left pass with `if (minr > maxr) break;` at line 19, but the final upward loop has no `if (minc > maxc)` check. On a single-**column** matrix the column is traversed twice | add `if (minc > maxc) break;` before the up pass — see solution #3 |

**Measured** on a generalised version of your code: a **5×1 column prints
`1 2 3 4 5 4 3 2`** instead of `1 2 3 4 5`.

**Your 4×5 case is correct** — I ran the file and it outputs the right spiral. That's exactly
why the bug survived: **rectangular and square matrices both pass**. Test 1×N, N×1 and 1×1.

The overall structure — four directional passes with `minr`/`maxr`/`minc`/`maxc` shrinking
after each — is the standard approach and is right.

### `08_2d array/2dvectors.cpp`

Everything is commented out and `main()` is empty, so nothing runs.

**The three commented declarations are correct C++** and are the ones worth knowing:

```cpp
vector<vector<int>> v(4);                      // 4 empty rows
vector<vector<int>> v2(4, vector<int>(3));     // 4x3, zero-filled
vector<vector<int>> v3(4, vector<int>(2, 3));  // 4x2, every element = 3
```

One thing to fix if you uncomment them: the first and third are both named `v`, which would be
a redeclaration error — the same issue as `01_2dArray.cpp:52`. Give them distinct names.
