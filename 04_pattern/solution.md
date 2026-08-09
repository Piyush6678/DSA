# 04 — Patterns: Solutions

Full C++14 for the 15 patterns in Sections 1–3, each with the derivation table that
produced the formulas. Derivations only for Section 4.

Every function below assumes:

```cpp
#include <iostream>
using namespace std;
```

and is called from a driver like:

```cpp
int main() { int n; cin >> n; pattern7(n); return 0; }
```

**Read the derivation table before the code in each one.** The code is the easy part; the
table is the skill.

---

# Section 1 — Must Do

## 1. Rectangular star pattern — Striver P1

```
****
****
****
****
```

```cpp
void pattern1(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j)
            cout << "*";
        cout << "\n";              // ONCE per row, in the OUTER loop's body
    }
}
```

**Derivation:** `n` rows, `n` symbols each — no dependence on `i` at all. This is the
baseline the other 21 patterns vary.

**The one thing to get right** is the `cout << "\n"` placement. Inside the inner loop it
prints one `*` per line. `lec1.cpp:31` and `:44` make exactly this mistake in the plus and
cross patterns.

**Complexity:** O(n²) time, O(1) space. *(All 22 patterns are O(n²)/O(1) — the output is
Θ(n²) characters, so nothing can be faster. I'll note complexity only where it differs.)*

---

## 2. Right-angled star triangle — Striver P2

```
*
**
***
****
```

| row `i` | stars |
|---|---|
| 1 | 1 |
| 2 | 2 |
| 3 | 3 |
| 4 | 4 |

Stars = `i`. That's the entire derivation.

```cpp
void pattern2(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j)      // bound depends on i
            cout << "*";
        cout << "\n";
    }
}
```

**The idea being introduced:** the inner loop's bound is a *function of the outer index*.
Everything after this is a different function.

`lec1.cpp:11-15` has this correct.

---

## 3. Inverted right-angled triangle — Striver P5

```
****
***
**
*
```

| row `i` | stars |
|---|---|
| 1 | 4 |
| 2 | 3 |
| 3 | 2 |
| 4 | 1 |

Stars = `n - i + 1`.

```cpp
void pattern5(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n - i + 1; ++j)
            cout << "*";
        cout << "\n";
    }
}
```

**Equivalent form — count the outer loop down instead:**

```cpp
for (int i = n; i >= 1; --i) {
    for (int j = 1; j <= i; ++j) cout << "*";
    cout << "\n";
}
```

Both are correct. Prefer whichever makes the formula simpler — here the second, since the
inner bound reverts to plain `i`.

`lec1.cpp:17-21` uses a third variant, `for (j = 4; j >= i; j--)`, which is also correct
(it runs `4 - i + 1` times) but hardcodes `4` instead of `n`.

**The `+1` is where people slip.** Check `i = n`: `n - n + 1 = 1` star ✓. Without the `+1`
the last row prints nothing. **Always evaluate your formula at `i = 1` and `i = n`.**

---

## 4. Star pyramid — Striver P7

```
   *
  ***
 *****
*******
```

**This is the most important pattern in the folder.** Everything centred reuses its
formulas.

| row `i` | spaces | stars | width |
|---|---|---|---|
| 1 | 3 | 1 | 4 |
| 2 | 2 | 3 | 5 |
| 3 | 1 | 5 | 6 |
| 4 | 0 | 7 | 7 |

- spaces: `3, 2, 1, 0` → decreasing to 0 at `i = n` → **`n - i`**
- stars: `1, 3, 5, 7` → the odd numbers → **`2i - 1`**

```cpp
void pattern7(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n - i; ++j)        // segment A: leading spaces
            cout << " ";
        for (int j = 1; j <= 2 * i - 1; ++j)    // segment B: stars
            cout << "*";
        cout << "\n";
    }
}
```

**Why `2i - 1`, structurally:** a centred shape grows by one star on **each** side per row,
so it grows by 2 per row. Starting at 1 and stepping by 2 gives the odd numbers. That
reasoning — not the memorised formula — is what lets you handle the next centred pattern
you've never seen.

**Sanity check:** widest row is `i = n` → `2n - 1` stars ✓, and total width `(n-i) + (2i-1)`
= `n + i - 1`, which grows by one per row — matching the ragged right edge of a
left-padded pyramid.

**Trailing spaces are not needed.** Some sources pad the right side too; judges almost
never require it, and it makes the output harder to eyeball.

---

## 5. Diamond — Striver P9

```
   *
  ***
 *****
*******
*******
 *****
  ***
   *
```

**Do not invent a new loop.** A diamond is pattern 7 followed by pattern 8.

```cpp
void pattern9(int n) {
    // Top half: the pyramid from #4
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n - i; ++j)     cout << " ";
        for (int j = 1; j <= 2 * i - 1; ++j) cout << "*";
        cout << "\n";
    }
    // Bottom half: the inverted pyramid from #6
    for (int i = n; i >= 1; --i) {
        for (int j = 1; j <= n - i; ++j)     cout << " ";
        for (int j = 1; j <= 2 * i - 1; ++j) cout << "*";
        cout << "\n";
    }
}
```

Note the bottom loop is character-for-character the top loop with `i` counting **down**.
That's the cleanest possible expression of "mirror".

**Row count: `2n`.** Striver's P9 repeats the widest row (both halves include `i = n`), so a
diamond of `n = 4` is 8 rows. If you want the widest row printed once — the `2n - 1` variant
— start the second loop at `i = n - 1`.

**Decide which you want and check it.** This is where `lec2.cpp:57` goes wrong: the loop
bound `i < 2*n - 1` produces `2n - 2` rows, silently dropping the final single star. See
the bugs section.

**Why two loops beat one.** `lec2.cpp` uses a single loop with `nsp`/`nst` counters that
reverse direction at the midpoint. It works in principle, but it needs three pieces of
mutable state and two direction tests, and the row-count bug is a direct consequence.
Two independent loops have nothing to get wrong. **Compose simple patterns; don't fuse
them.**

---

# Section 2 — Important

## 6. Inverted star pyramid — Striver P8

```
*******
 *****
  ***
   *
```

| row `i` | spaces | stars |
|---|---|---|
| 1 | 0 | 7 |
| 2 | 1 | 5 |
| 3 | 2 | 3 |
| 4 | 3 | 1 |

- spaces: `0, 1, 2, 3` → **`i - 1`**
- stars: `7, 5, 3, 1` → **`2(n - i) + 1`**

```cpp
void pattern8(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i - 1; ++j)             cout << " ";
        for (int j = 1; j <= 2 * (n - i) + 1; ++j)   cout << "*";
        cout << "\n";
    }
}
```

**Check both ends:** `i = 1` → 0 spaces, `2(n-1)+1 = 2n-1` stars ✓.
`i = n` → `n-1` spaces, `2(0)+1 = 1` star ✓.

**Don't try to reuse #4's formulas by "flipping a sign".** Rebuild the table. It takes
fifteen seconds and it's how you avoid the class of bug where a pattern is right at one end
and wrong at the other.

---

## 7. Half diamond (arrow) — Striver P10

```
*
**
***
****
***
**
*
```

Two triangles, `2n - 1` rows total.

```cpp
void pattern10(int n) {
    for (int i = 1; i <= n; ++i) {          // ascending: 1..n stars
        for (int j = 1; j <= i; ++j) cout << "*";
        cout << "\n";
    }
    for (int i = n - 1; i >= 1; --i) {      // descending: n-1..1 stars
        for (int j = 1; j <= i; ++j) cout << "*";
        cout << "\n";
    }
}
```

**The `n - 1` is the whole question.** Start the second loop at `n` and the peak row prints
twice:

```
***
****
****      <- duplicated
***
```

Total rows: `n + (n-1) = 2n - 1` ✓ — an odd number, which is what a single-peaked shape
must have.

**The single-loop alternative**, worth seeing once:

```cpp
for (int i = 1; i <= 2 * n - 1; ++i) {
    int stars = (i <= n) ? i : 2 * n - i;   // rises to n, then falls
    for (int j = 1; j <= stars; ++j) cout << "*";
    cout << "\n";
}
```

`2n - i` at `i = n+1` gives `n-1` ✓, and at `i = 2n-1` gives `1` ✓. Compact, but the
two-loop version is easier to verify — prefer it under interview pressure.

---

## 8. Binary number triangle — Striver P11

```
1
01
101
0101
```

Row `i` has `i` characters. Rows starting with `1`: 1, 3 (odd `i`). Rows starting with `0`:
2, 4 (even `i`). Within a row the digits alternate.

Both facts collapse into one condition: **print `1` when `i + j` is even.**

```cpp
void pattern11(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j)
            cout << ((i + j) % 2 == 0 ? 1 : 0);
        cout << "\n";
    }
}
```

**Verify:** `i=1, j=1` → `2` even → `1` ✓. `i=2, j=1` → `3` odd → `0`; `j=2` → `4` even →
`1` → row is `01` ✓. `i=3` → `101` ✓.

**The alternative** — a toggling variable — is equally valid and generalises to non-binary
alternations:

```cpp
for (int i = 1; i <= n; ++i) {
    int start = (i % 2 == 1) ? 1 : 0;
    for (int j = 1; j <= i; ++j) { cout << start; start = 1 - start; }
    cout << "\n";
}
```

**The lesson:** the loop structure here is *identical* to pattern 2. Only the printed
character changed. Counts and characters are independent — recognising that collapses six
of the seven Section 4 patterns into nothing.

`lec1.cpp:82-88` has this pattern correct.

---

## 9. Hollow rectangle — Striver P21

```
****
*  *
*  *
****
```

**Switch to the grid-condition model.** Decomposing a hollow row into segments requires
different logic for border rows and interior rows; as a condition it's one line.

```cpp
void pattern21(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            bool onBorder = (i == 1 || i == n || j == 1 || j == n);
            cout << (onBorder ? '*' : ' ');
        }
        cout << "\n";
    }
}
```

**The condition reads exactly as the shape:** first row, last row, first column, or last
column. Four `||` clauses, one per edge.

**This is the highest-value pattern in the folder for interviews**, because "iterate the
grid, test a predicate on `(i, j)`" is precisely how you'll write boundary traversal, matrix
border sums, and spiral order in `../08_2d array`.

The same model gives you, for free:

```cpp
i == j                       // main diagonal
i + j == n + 1               // anti-diagonal
i == j || i + j == n + 1     // cross / X
i == (n+1)/2 || j == (n+1)/2 // plus / +
```

`lec1.cpp:24-46` already uses this model for the plus and cross, and **the conditions are
correct** — only the `endl` placement is wrong. See the bugs section.

**Edge case:** at `n = 1` and `n = 2` every cell is on a border, so the rectangle is solid.
That's correct, and it's a good check that your condition has no interior special-casing.

---

## 10. Number crown — Striver P12

```
1      1
12    21
123  321
12344321
```

Three segments per row. Build the table for `n = 4` (total width is a constant `2n = 8`):

| row `i` | ascending | spaces | descending |
|---|---|---|---|
| 1 | 1 | 6 | 1 |
| 2 | 2 | 4 | 2 |
| 3 | 3 | 2 | 3 |
| 4 | 4 | 0 | 4 |

- ascending count: **`i`**, printing `1..i`
- spaces: `6, 4, 2, 0` → **`2(n - i)`**
- descending count: **`i`**, printing `i..1`

```cpp
void pattern12(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j)             cout << j;   // 1, 2, ..., i
        for (int j = 1; j <= 2 * (n - i); ++j)   cout << " ";
        for (int j = i; j >= 1; --j)             cout << j;   // i, ..., 2, 1
        cout << "\n";
    }
}
```

**The check that catches errors instantly:** every row must be exactly `2n` characters wide.
`i + 2(n-i) + i = 2n` ✓ — constant, independent of `i`. If your three formulas don't sum to
a constant, one is wrong, and you know that *before* running the code.

**Note the third loop counts down**, both in its bound and in what it prints. Writing
`cout << i - j + 1` in an ascending loop works too, but counting down says what it means.

---

# Section 3 — Good to Know

## 11. Increasing number triangle — Striver P13

```
1
2 3
4 5 6
7 8 9 10
```

The printed value depends on **neither `i` nor `j`** — it's a counter that persists across
rows. That's the new idea.

```cpp
void pattern13(int n) {
    int num = 1;                       // declared OUTSIDE both loops
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j)
            cout << num++ << " ";
        cout << "\n";
    }
}
```

**Declaring `num` outside is the whole problem.** Inside the outer loop it resets to 1 every
row and you get pattern P3 with spaces.

`lec1.cpp:89-95` implements Floyd's triangle with exactly this structure — a `cnt` declared
before the loops and incremented in the inner update clause. Correct.

**Total numbers printed:** `1+2+…+n = n(n+1)/2`, so the last value is `n(n+1)/2`. For
`n = 4` that's 10 ✓ — another instant sanity check.

---

## 12. Alpha-hill pattern — Striver P17

```
   A
  ABA
 ABCBA
ABCDCBA
```

This is pattern 7's skeleton with the star loop replaced by a **palindromic letter run**.

| row `i` | spaces | up-letters | down-letters | total chars |
|---|---|---|---|---|
| 1 | 3 | A (1) | — (0) | 1 |
| 2 | 2 | AB (2) | A (1) | 3 |
| 3 | 1 | ABC (3) | BA (2) | 5 |
| 4 | 0 | ABCD (4) | CBA (3) | 7 |

Same `n - i` spaces and same `2i - 1` total as the pyramid — split as `i` up and `i - 1`
down.

```cpp
void pattern17(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n - i; ++j)  cout << " ";

        for (int j = 0; j < i; ++j)       cout << char('A' + j);       // A .. A+i-1
        for (int j = i - 2; j >= 0; --j)  cout << char('A' + j);       // A+i-2 .. A

        cout << "\n";
    }
}
```

**`i - 2` is the tricky index.** The up-run ends at `'A' + i - 1`; the down-run must not
repeat the peak, so it starts one below, at `'A' + i - 2`. At `i = 1` that's `-1`, so the
loop runs zero times ✓ — exactly right, since row 1 is just `A`.

**`char('A' + j)`** is the character-arithmetic idiom from `../01_basics` §4. `'A' + j` is
an `int`; the cast makes `cout` print it as a letter rather than a number. Forget the cast
and you get `65 66 67`.

---

## 13. Symmetric void pattern — Striver P19

For `n = 5` — 10 rows, each exactly `2n = 10` characters wide:

```
**********
****  ****
***    ***
**      **
*        *
*        *
**      **
***    ***
****  ****
**********
```

| row `i` (top half) | stars | spaces | stars |
|---|---|---|---|
| 1 | 5 | 0 | 5 |
| 2 | 4 | 2 | 4 |
| 3 | 3 | 4 | 3 |
| 4 | 2 | 6 | 2 |
| 5 | 1 | 8 | 1 |

- stars: `5, 4, 3, 2, 1` → **`n - i + 1`**
- spaces: `0, 2, 4, 6, 8` → **`2(i - 1)`**

```cpp
void pattern19(int n) {
    // Top half: wide -> narrow
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n - i + 1; ++j)   cout << "*";
        for (int j = 1; j <= 2 * (i - 1); ++j) cout << " ";
        for (int j = 1; j <= n - i + 1; ++j)   cout << "*";
        cout << "\n";
    }
    // Bottom half: narrow -> wide (the same loop, i counting down)
    for (int i = n; i >= 1; --i) {
        for (int j = 1; j <= n - i + 1; ++j)   cout << "*";
        for (int j = 1; j <= 2 * (i - 1); ++j) cout << " ";
        for (int j = 1; j <= n - i + 1; ++j)   cout << "*";
        cout << "\n";
    }
}
```

**Width check:** `(n-i+1) + 2(i-1) + (n-i+1) = 2n` ✓ — constant, as it must be for a shape
with straight vertical edges.

As with the diamond, the bottom half is the top half with `i` reversed. **Write the top,
copy it, reverse the loop.**

---

## 14. Symmetric butterfly — Striver P20

The mirror image of #13. For `n = 5`:

```
*        *
**      **
***    ***
****  ****
**********
**********
****  ****
***    ***
**      **
*        *
```

| row `i` (top half) | stars | spaces | stars |
|---|---|---|---|
| 1 | 1 | 8 | 1 |
| 2 | 2 | 6 | 2 |
| 3 | 3 | 4 | 3 |
| 4 | 4 | 2 | 4 |
| 5 | 5 | 0 | 5 |

- stars: **`i`**
- spaces: `8, 6, 4, 2, 0` → **`2(n - i)`**

```cpp
void pattern20(int n) {
    for (int i = 1; i <= n; ++i) {           // narrow -> wide
        for (int j = 1; j <= i; ++j)           cout << "*";
        for (int j = 1; j <= 2 * (n - i); ++j) cout << " ";
        for (int j = 1; j <= i; ++j)           cout << "*";
        cout << "\n";
    }
    for (int i = n; i >= 1; --i) {           // wide -> narrow
        for (int j = 1; j <= i; ++j)           cout << "*";
        for (int j = 1; j <= 2 * (n - i); ++j) cout << " ";
        for (int j = 1; j <= i; ++j)           cout << "*";
        cout << "\n";
    }
}
```

**Do #13 and #14 back to back, and derive both tables from scratch.** They look nearly
identical and their formulas are opposites (`n-i+1` vs `i`, `2(i-1)` vs `2(n-i)`). If you
pattern-match from memory instead of tabulating, this pair is where it fails — which is
exactly why they're both in the list.

---

## 15. The number pattern / concentric squares — Striver P22

The hardest one. For `n = 4`, a `(2n-1) × (2n-1)` = 7×7 grid:

```
4444444
4333334
4322234
4321234
4322234
4333334
4444444
```

**Segment decomposition is hopeless here.** The right model is:

> **The value at a cell is `n` minus its distance to the nearest edge of the grid.**

With 0-indexed `i, j` in `[0, 2n-2]`, the distances to the four edges are `i` (top), `j`
(left), `2n-2-i` (bottom), `2n-2-j` (right). So:

```cpp
#include <algorithm>

void pattern22(int n) {
    int size = 2 * n - 1;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int top = i, left = j, bottom = size - 1 - i, right = size - 1 - j;
            int distanceToEdge = min(min(top, bottom), min(left, right));
            cout << n - distanceToEdge;
        }
        cout << "\n";
    }
}
```

**Verify the corners and centre:**
- `(0,0)`: distances `0, 0, 6, 6` → min `0` → prints `4 - 0 = 4` ✓
- `(3,3)` (centre): distances `3, 3, 3, 3` → min `3` → prints `4 - 3 = 1` ✓
- `(1,2)`: distances `1, 2, 5, 4` → min `1` → prints `3` ✓

**Why this is worth the effort.** "Distance to the nearest boundary" is a real primitive —
it's how you compute ring indices in matrix rotation, how you peel layers in spiral
traversal (`../08_2d array/spiral.cpp`), and the seed of multi-source BFS in
`../27_graphs`. The pattern is a toy; the `min` over four distances is not.

**Your `lec2.cpp:88-102` implementation is correct.** It maintains `A = min(i, size-1-i)`
and `B = min(j, size-1-j)` incrementally via the `if (j < N-1) B++ else B--` trick, then
prints `N - min(A, B)`. That's the same formula computed by running counters instead of
`min` calls — a legitimately clever version. The explicit `min` above is easier to read and
to prove correct; both are right.

---

# Section 4 — Extra Practice (derivations only)

Six of these seven are Section 1's triangles with the **character rule** changed and the
**loop structure untouched**. Write the loop from #2 or #3, then change what `cout` prints.

### P3 — Right-angled number pyramid  ·  `1` / `12` / `123` / `1234`
Pattern #2 exactly; print `j` instead of `*`. The column index *is* the number.
`lec1.cpp:100-104` has this.

### P4 — Number pyramid, same digit per row  ·  `1` / `22` / `333` / `4444`
Pattern #2 exactly; print `i` instead of `*`. Contrast with P3: `j` varies within a row,
`i` is constant within a row. Understanding which index to print is the entire exercise.

### P6 — Inverted numbered right triangle  ·  `1234` / `123` / `12` / `1`
Pattern #3's structure (`n - i + 1` symbols), printing `j`.

### P14 — Increasing letter triangle  ·  `A` / `AB` / `ABC` / `ABCD`
Pattern #2, printing `char('A' + j - 1)`. The `- 1` is because `j` is 1-indexed and `'A'` is
offset 0. `lec1.cpp:117-121` prints `char(j + 64)` — arithmetically identical, since
`'A' == 65`, but `char('A' + j - 1)` doesn't hardcode the ASCII table and is what you should
write.

### P15 — Reverse letter triangle  ·  `ABCD` / `ABC` / `AB` / `A`
Pattern #3's bound (`n - i + 1`), printing `char('A' + j - 1)`.

### P16 — Alpha-ray triangle  ·  `A` / `BB` / `CCC` / `DDDD`
Pattern #2's bound, printing `char('A' + i - 1)` — the **row** index, so the letter is
constant within a row and advances between rows. This is P4's idea in letters.

### P18 — Alpha-triangle  ·  `D` / `CD` / `BCD` / `ABCD`  (the only Medium here)
The genuine exception: **both ends move**. Row `i` has `i` letters, ending at `'A' + n - 1`
and therefore starting at `'A' + (n - i)`.

| row `i` | starts at | ends at | letters |
|---|---|---|---|
| 1 | `'A'+3` = D | D | `D` |
| 2 | `'A'+2` = C | D | `CD` |
| 3 | `'A'+1` = B | D | `BCD` |
| 4 | `'A'+0` = A | D | `ABCD` |

```cpp
for (int i = 1; i <= n; ++i) {
    for (int j = n - i; j <= n - 1; ++j) cout << char('A' + j);
    cout << "\n";
}
```

Check `i = 1`: `j` runs `n-1..n-1`, one letter, `'A'+n-1` ✓. Check `i = n`: `j` runs
`0..n-1`, all `n` letters starting at `'A'` ✓.

---

# Bugs in your current code

Per the repo's convention I have not edited your `.cpp` files. Both compile cleanly
(verified with `g++ -fsyntax-only`) — these are logic bugs, and the two `endl` ones produce
dramatically wrong output.

### `04_pattern/lec1.cpp`

| Line | Problem | Fix |
|---|---|---|
| 31 | **Plus pattern: `cout << endl;` is inside the inner `j` loop.** It prints one character per line, producing 25 lines of a single char instead of a 5×5 plus | move `cout << endl;` outside the `j` loop, into the `i` loop's body |
| 44 | **Cross pattern: same bug**, same fix | as above |
| 106-115 | The section labelled *"number triangle flipped"* prints `"X"`, not numbers — it's a right-aligned **star** triangle | rename the comment, or print `k` |
| 17-21 | Inverted triangle hardcodes `4` in `for (j = 4; j >= i; j--)` | use `n` so the pattern is parameterised |

**The conditions themselves are correct.** `j == ((n+1)/2) or i == ((n+1)/2)` for the plus,
and `j == i or (i+j) == (n+1)` for the cross, are exactly the right grid predicates — see
solution #9. Only the newline placement is wrong. Fix the two `endl` lines and both patterns
render correctly.

**What's right in this file:** the binary triangle at lines 82-88 (`(i+j)%2` — correct),
Floyd's triangle at 89-95 (counter declared outside both loops — correct), and the odd-number
triangle at 76-81. Those are three of the trickier ones.

### `04_pattern/lec2.cpp`

| Line | Problem | Fix |
|---|---|---|
| 24 | **Star pyramid: `for (int j = 1; j <= r2 - 1; j++)` prints a constant 3 spaces on every row.** It should be `r2 - i`. The output is a left-aligned triangle shifted right by 3, not a pyramid | `j <= r2 - i` |
| 57 | **Diamond prints `2n - 2` rows instead of `2n - 1`.** `for (i = 1; i < 2*n - 1; i++)` with `n = 4` runs `i = 1..6`, giving star counts `1,3,5,7,5,3` — the final single-star row is missing | `i <= 2*n - 1`, or use the two-loop form in solution #5 |
| 66 | `for (int i = 1; i <= nst; i++)` **shadows the outer `i`**. It happens to work because the inner `i` is only a counter, but any reference to the outer `i` inside would silently read the wrong variable | rename to `k` |

**What's right in this file:** the rhombus (lines 8-21) is correct, the number-pyramid
palindrome (lines 39-55) is correct including the `k = i-1` down-count that avoids repeating
the peak, and — the impressive one — **the concentric-square pattern at lines 88-102 is
correct**. Maintaining `A` and `B` as incremental distances to the nearest edge and printing
`N - min(A, B)` is a genuinely good solution to the hardest pattern in the set. See
solution #15 for the explicit-`min` version of the same idea.
