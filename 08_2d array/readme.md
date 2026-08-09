# 08 — 2D Arrays & Matrices

> **Striver A2Z mapping:** the three matrix problems from Step 3 — **Set Matrix Zeroes,
> Rotate Matrix by 90°, Spiral Traversal** — plus Pascal's Triangle, and the matrix search
> problems.
> Prerequisites: `../06_Pointer` (decay), `../07_Array` (vectors, two pointers).

A matrix is an array of arrays. Almost every difficulty is **index bookkeeping**, which is
why `../04_pattern` was worth doing — this is that skill with data instead of stars.

> **Note on the old file.** This folder already contained `readme .md` (with a space in the
> name). Its contents are preserved at the bottom of this file, so that file is now redundant
> — delete it whenever you like. I haven't touched it.

---

## 1. Declaration and memory layout

```cpp
int a[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};   // 3 rows, 4 columns
int b[4][3] = {1,2,3,4,5,6,7,8,9,10,11,12};         // flat init — filled row by row
int c[3][4] = {0};                                   // all zero
```

**A 2D array is stored row-major** — one contiguous block, rows laid end to end:

```
a[0][0] a[0][1] a[0][2] a[0][3] a[1][0] a[1][1] ... a[2][3]
```

So `a[i][j]` lives at offset `i * COLS + j` from the start. **This single fact explains
almost everything else in this section.**

**Why the column dimension is mandatory and the row dimension is optional:**

```cpp
int arr[][4] = {{1,2,3,4},{5,6,7,8}};    // fine — rows deduced as 2
int arr[3][]  = ...;                      // ERROR
void f(int a[][4]);                       // fine
void f(int a[][]);                        // ERROR
```

To compute `i * COLS + j` the compiler needs `COLS`. It never needs the row count, because
that's only used for bounds you're expected to track yourself. Your `01_2dArray.cpp:4` notes
this correctly in a comment.

---

## 2. `vector<vector<int>>` — what you'll actually use

Your `2dvectors.cpp` has these commented out; they're all correct:

```cpp
vector<vector<int>> v(4);                        // 4 empty rows
vector<vector<int>> v2(4, vector<int>(3));       // 4x3, all zero
vector<vector<int>> v3(4, vector<int>(2, 3));    // 4x2, every element = 3
```

Access is `v[i][j]`; `v.size()` is the row count, `v[0].size()` the column count.

**Rows are independent allocations.** Unlike a C array, `vector<vector<int>>` is a vector of
*pointers* to separate row buffers, so:

- Rows are **not contiguous** with each other — worse cache behaviour on large matrices.
- Rows can have **different lengths** (a jagged array), which is what makes Pascal's Triangle
  natural to represent.
- Always guard `v[0].size()` with an `v.empty()` check.

**For performance-critical numeric code**, one flat `vector<int> m(R*C)` indexed as
`m[i*C + j]` is measurably faster — one allocation, fully contiguous. Worth naming in an
interview; rarely worth writing in one.

---

## 3. Traversal orders

```cpp
for (int i = 0; i < R; ++i)              // ROW-MAJOR — matches memory layout, cache-friendly
    for (int j = 0; j < C; ++j) use(a[i][j]);

for (int j = 0; j < C; ++j)              // COLUMN-MAJOR — cache-hostile on big matrices
    for (int i = 0; i < R; ++i) use(a[i][j]);
```

**Prefer row-major.** On a large matrix, column-major traversal misses the cache on nearly
every access and can run several times slower for identical work — same complexity, very
different wall-clock.

**The index identities worth memorising:**

| Want | Condition / formula |
|---|---|
| Main diagonal | `i == j` |
| Anti-diagonal | `i + j == n - 1` |
| Border cell | `i == 0 \|\| i == R-1 \|\| j == 0 \|\| j == C-1` |
| Flatten `(i,j)` → index | `i * C + j` |
| Unflatten `idx` → `(i,j)` | `(idx / C, idx % C)` |

The last two are the basis of treating a sorted matrix as one sorted array — see LeetCode 74
in `solution.md` #5.

---

## 4. The four techniques that cover most matrix problems

### Transpose + reverse = rotate

**Rotate 90° clockwise** = transpose, then reverse each **row**.
**Rotate 90° anticlockwise** = transpose, then reverse each **column**.

Both are O(1) space for a square matrix. Your `01_2dArray.cpp:66-86` uses exactly this
approach and the structure is correct.

### Boundary shrinking = spiral

Maintain `top`, `bottom`, `left`, `right`. Walk one edge, move that boundary inward, repeat.
**The guards between passes are where the bugs live** — see §6.

### Using the matrix itself as storage = O(1) space

Set Matrix Zeroes needs to remember which rows and columns contain a zero. The naive way is
two boolean arrays, O(R+C) space. The trick is to store those flags **in row 0 and column 0
of the matrix itself**, reducing it to O(1) — at the cost of one extra variable for the
overlap cell.

### Staircase search = sorted-matrix search

Start at the **top-right** corner. If the value is too big, move left; too small, move down.
Each step eliminates an entire row or column, giving O(R+C).

---

## 5. Complexity

Any algorithm that must examine every cell is **Θ(R·C)** and cannot be beaten — the input
size is R·C. So for matrix problems the interesting question is usually **space**, not time:
can you do it in-place?

The exceptions are search problems, where structure lets you skip cells: LeetCode 74 is
O(log(R·C)) by binary search, LeetCode 240 is O(R+C) by staircase.

---

## 6. Bugs this folder exists to prevent

**Redeclaring a variable.** `01_2dArray.cpp` declares `int r=5,c=2;` at line 11 and `int r=4;`
at line 52 — **the file does not compile**. Verified.

**Using the wrong loop variable in the index.** `multiplication.cpp` writes
`arr[r][k] * brr[k][c]` where `r` and `c` are the *dimensions*, not the loop counters. Both
reads are out of bounds. Verified: it outputs `45` everywhere instead of the correct product.

**Missing guards in spiral traversal.** After shrinking `top`/`right`, you must re-check
before the bottom and left passes, or a single-row or single-column matrix gets traversed
twice. `spiral.cpp` guards the bottom pass but **not the left pass** — verified: a 5×1 column
prints `1 2 3 4 5 4 3 2`.

**`int max = -999`** as a running maximum (`01_2dArray.cpp:28`) — wrong for any matrix whose
elements are all below −999. Use `INT_MIN` or `a[0][0]`.

**Too few initialisers.** `int trr[4][4] = {1,2,3,45,6,7,8,9,10,12,13,14,15,16};` supplies
only 14 values for 16 slots; the last two are silently zero. (`45` looks like a typo for
`4,5`.)

**VLA for matrices.** `int rotate[n][n]` with runtime `n` is a GCC extension, not standard
C++ — and here it also shadows `std::rotate` under `using namespace std`.

---

## Interview Q&A

### Q1. Why must you specify the column dimension when passing a 2D array to a function?

Because a 2D array is stored **row-major** in one contiguous block, and the compiler
translates `a[i][j]` into `*(base + i * COLS + j)`. Without `COLS` it cannot generate that
address arithmetic at all.

The row count is never needed for indexing — it only bounds the loop, which is your
responsibility. So `void f(int a[][4])` compiles and `void f(int a[][])` does not.

This is the same **array decay** from `../06_Pointer` §5, one dimension up: `int a[3][4]`
decays to `int (*)[4]` — a pointer to an array of 4 ints — not to `int**`. That distinction
matters, because it means you **cannot** pass a `int a[3][4]` to a function expecting
`int**`; the memory layouts genuinely differ (one contiguous block versus an array of row
pointers).

Three ways around the rigidity, in increasing preference: pass a flat `int*` with both
dimensions and index manually as `p[i*C + j]`; use a template to deduce the extent; or use
`vector<vector<int>>`, which carries its own dimensions and is what you'd actually write.

---

### Q2. How do you rotate an N×N matrix 90° clockwise in place?

Two steps, each O(1) space:

1. **Transpose** — swap `a[i][j]` with `a[j][i]` for all `j > i`.
2. **Reverse each row.**

```cpp
for (int i = 0; i < n; ++i)
    for (int j = i + 1; j < n; ++j) swap(a[i][j], a[j][i]);
for (int i = 0; i < n; ++i) reverse(a[i].begin(), a[i].end());
```

**Why `j = i + 1`** — iterating the full square would swap every pair *twice*, returning the
matrix to its original state. You must touch only one triangle. (Your `01_2dArray.cpp` uses
`if (i == j) break;` inside a full inner loop, which covers the *lower* triangle instead —
different triangle, equally correct.)

**Why transpose-then-reverse gives a clockwise rotation:** transposing reflects across the
main diagonal, and reversing each row reflects across the vertical axis. Two reflections
compose into a rotation, and these two produce +90°.

**Anticlockwise is transpose then reverse each *column*** — or equivalently reverse the rows
first, then transpose. Being asked for the other direction is the standard follow-up.

For a **non-square** matrix, in-place is impossible — the dimensions change from R×C to C×R —
so you allocate a new matrix. Worth stating, because interviewers sometimes ask precisely to
see whether you notice.

---

### Q3. How do you solve Set Matrix Zeroes in O(1) extra space?

The naive approach records which rows and columns must be zeroed in two arrays: O(R+C) space.
You cannot zero cells as you find them, because a freshly written 0 is indistinguishable from
an original one and would cascade.

**The O(1) trick is to store those flags inside the matrix itself** — in row 0 and column 0,
which are exactly R+C cells and are themselves going to be zeroed if they contain a zero.

The complication is the shared cell `a[0][0]`, which would have to represent both "row 0 has
a zero" and "column 0 has a zero". So you keep **one extra boolean** for column 0 and let
`a[0][0]` stand only for row 0.

Then the second pass must go **bottom-right to top-left**, so that the marker row and column
are read before they get overwritten.

```cpp
bool col0 = false;
for (int i = 0; i < R; ++i) {
    if (a[i][0] == 0) col0 = true;
    for (int j = 1; j < C; ++j) if (a[i][j] == 0) { a[i][0] = 0; a[0][j] = 0; }
}
for (int i = R-1; i >= 0; --i) {
    for (int j = C-1; j >= 1; --j) if (a[i][0] == 0 || a[0][j] == 0) a[i][j] = 0;
    if (col0) a[i][0] = 0;
}
```

The two details that make it correct are the **reverse iteration** and the **separate
`col0` flag**. Both come directly from the aliasing problem, which is the real content of the
question.

---

### Q4. Search in a sorted matrix — what's the difference between LeetCode 74 and 240?

**They have different sortedness guarantees, so they need different algorithms.**

**LeetCode 74** — each row is sorted, **and the first element of each row exceeds the last of
the previous row.** The whole matrix is therefore one sorted sequence, just wrapped. So treat
it as a flat array of length `R*C` and binary search it, mapping index → cell with
`(mid / C, mid % C)`. **O(log(R·C)).**

**LeetCode 240** — rows are sorted left-to-right and columns top-to-bottom, but **there is no
relationship between the end of one row and the start of the next.** The flattened sequence
is not sorted, so binary search over it is invalid.

Instead use the **staircase search**, starting at the **top-right** corner:

```cpp
int i = 0, j = C - 1;
while (i < R && j >= 0) {
    if      (a[i][j] == t) return true;
    else if (a[i][j] >  t) --j;      // too big: the whole COLUMN below is bigger
    else                   ++i;      // too small: the whole ROW to the left is smaller
}
```

The top-right corner is special because it is the **maximum of its row and the minimum of its
column** — so each comparison definitively eliminates an entire row or column. Bottom-left
works identically. The other two corners give you no such property, which is why starting
there fails. **O(R+C).**

Confusing these two is the most common mistake on matrix search; the giveaway is whether the
matrix is globally sorted or only row/column-wise.

---

### Q5. What are the guard conditions in a spiral traversal, and why are they needed?

The loop walks four edges — right along the top, down the right, left along the bottom, up
the left — shrinking a boundary after each:

```cpp
while (top <= bottom && left <= right) {
    for (int j = left; j <= right; ++j)  res.push_back(a[top][j]);   top++;
    for (int i = top;  i <= bottom; ++i) res.push_back(a[i][right]); right--;
    if (top <= bottom) { for (int j = right; j >= left; --j) res.push_back(a[bottom][j]); bottom--; }
    if (left <= right) { for (int i = bottom; i >= top; --i) res.push_back(a[i][left]);  left++;  }
}
```

**The two inner `if`s are mandatory.** After the first two passes, `top` and `right` have
already moved. In a **single-row** matrix, the top pass consumes the entire row and `top` now
exceeds `bottom` — without the guard, the bottom pass would walk that same row again
backwards. Symmetrically, a **single-column** matrix would repeat the column via the left
pass.

This is not hypothetical: your `spiral.cpp` guards the bottom pass but not the left one, and
I verified that a 5×1 column outputs `1 2 3 4 5 4 3 2` — the last three re-printed.

**The test cases that expose it are 1×N, N×1, and 1×1.** Any spiral implementation should be
run against all three before you trust it; a square matrix will not reveal the bug.

---

### Q6. `vector<vector<int>>` or a flat array — does it matter?

Functionally, no. For performance on large matrices, yes.

`vector<vector<int>>` is a vector of *independent* row vectors. Each row is its own heap
allocation, so the rows are **scattered in memory**. That costs you:

- **An extra indirection per access** — read the row pointer, then the element.
- **Cache misses** — walking `a[i][j]` in row-major order is contiguous *within* a row but
  jumps arbitrarily between rows.
- **R+1 allocations** instead of one.

A flat `vector<int> m(R*C)` indexed `m[i*C + j]` is one contiguous block: one allocation,
perfect locality, and no indirection. On a large matrix multiply this can be several times
faster for the same asymptotic complexity.

**When to care:** numeric/graphics code, or anything where the matrix is large and traversed
repeatedly. **When not to:** interview solutions and anything under a few hundred rows —
`vector<vector<int>>` reads better, is what LeetCode signatures use, and the difference is
noise.

The general principle is worth stating: **asymptotic complexity is not the whole performance
story.** Row-major versus column-major traversal of the same matrix is the same O(R·C) and can
differ several-fold in wall-clock time, purely from cache behaviour.

---

## Common mistakes checklist

- [ ] Loop counters in the index (`a[i][k]`), never the dimensions (`a[R][k]`)
- [ ] Column dimension supplied in any 2D array parameter
- [ ] `INT_MIN` for a running maximum, not `-999`
- [ ] Spiral guarded before **both** the bottom and left passes
- [ ] Tested on 1×N, N×1, and 1×1
- [ ] `j = i+1` (or one triangle only) when transposing in place
- [ ] `v.empty()` checked before `v[0].size()`
- [ ] `vector<vector<int>>` instead of a VLA `int m[n][n]`
- [ ] Enough initialisers, or `{0}` and fill explicitly

---

## Your original notes (preserved)

*From `readme .md`. All six are covered: 867 is #1, 48 is #2, 54 is #3, 118 is #8, 861 is #9,
240 is #6.*

```
# LEETCODE
- 867 transpose of a matrix
- 48  rotate a matrix by 90 degree
- 54 spiral matrix
- 118
- 861
- 240
```

---

## Next

`questions.md` → `solution.md` → `../09_Strings`.
