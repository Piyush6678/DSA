# 04 — Patterns: Nested Loop Mastery

> **Striver A2Z mapping:** Step 1.2 *"Build-up Logic"* — all **22 patterns**.
> Prerequisites: `../01_basics`, `../02_Conditional`, `../03_Loops`.

Patterns are not a topic. They are a **drill for nested-loop control and index
arithmetic**, and they are the last easy place to build that skill before it becomes
load-bearing in 2D arrays, matrix problems, DP tables, and grid traversal.

**Nobody will ask you to print a diamond at a FAANG onsite.** But if you cannot derive
`n - i` spaces and `2i - 1` stars in ten seconds, you will lose time on every matrix
problem you ever see. That is why this folder exists, and it's why the goal is
**derivation, not memorisation** — 22 memorised patterns are worth nothing; one method that
generates all 22 is worth a lot.

---

## 1. The universal method

Every pattern reduces to the same four questions:

1. **How many rows?** → that's your outer loop.
2. **For row `i`, what segments does the line contain?** → usually
   `spaces`, then `symbols`, sometimes `spaces` and `symbols` again.
3. **How many of each, as a function of `i`?** → that's each inner loop's bound.
4. **What character does each position print?** → usually a constant, a counter, or
   `char('A' + k)`.

Then the skeleton is always:

```cpp
for (int i = 1; i <= rows; ++i) {        // 1. rows
    for (int j = 1; j <= <spaces(i)>; ++j) cout << " ";     // 2+3. segment A
    for (int j = 1; j <= <symbols(i)>; ++j) cout << "*";    // 2+3. segment B
    cout << "\n";                        // ONCE per row — outside the inner loops
}
```

**The single most common bug in this folder is putting `cout << endl` inside the inner
loop.** That prints one character per line. `04_pattern/lec1.cpp:31` and `:44` both do
this. The newline belongs to the *row*, so it goes in the outer loop's body.

---

## 2. Derive, don't memorise — build the table

Before writing code, tabulate. For a pyramid with `n = 4`:

```
   *      row 1: 3 spaces, 1 star
  ***     row 2: 2 spaces, 3 stars
 *****    row 3: 1 space,  5 stars
*******   row 4: 0 spaces, 7 stars
```

| row `i` | spaces | stars |
|---|---|---|
| 1 | 3 | 1 |
| 2 | 2 | 3 |
| 3 | 1 | 5 |
| 4 | 0 | 7 |

Now read the columns as sequences:
- spaces: `3, 2, 1, 0` — decreasing by 1, hits 0 at `i = n` → **`n - i`**
- stars: `1, 3, 5, 7` — odd numbers → **`2i - 1`**

That's the whole derivation, and it takes fifteen seconds. **Write the table for any
pattern you don't immediately see.** Two or three rows are enough to identify a linear
sequence.

### The formulas that cover most patterns

| Sequence in the table | Formula |
|---|---|
| `1, 2, 3, …, n` | `i` |
| `n, n-1, …, 1` | `n - i + 1` |
| `0, 1, 2, …, n-1` | `i - 1` |
| `n-1, n-2, …, 0` | `n - i` |
| `1, 3, 5, 7, …` (odd) | `2i - 1` |
| `2, 4, 6, 8, …` (even) | `2i` |
| `0, 2, 4, 6, …` | `2(i - 1)` |

**Sanity-check the total width.** A pyramid row is `(n-i)` spaces + `(2i-1)` stars =
`n + i - 1` characters, which for `i = n` is `2n - 1` — the widest row. If your formulas
don't produce a constant or a smoothly changing width, one of them is wrong.

---

## 3. The second model: full-grid conditions

Segment decomposition handles left-aligned and centred shapes. For **symmetric or hollow**
shapes — plus, cross, hollow rectangle, hollow diamond — a different model is far easier:

> **Imagine the full `n × n` grid. Visit every cell `(i, j)`. Print the symbol when a
> condition on `(i, j)` holds, and a space otherwise.**

```cpp
for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j)
        cout << (CONDITION ? '*' : ' ');
    cout << "\n";
}
```

Now each shape is just a condition:

| Shape | Condition |
|---|---|
| Filled rectangle | `true` |
| Right triangle | `j <= i` |
| Inverted right triangle | `j >= i` |
| **Hollow rectangle** | `i == 1 \|\| i == n \|\| j == 1 \|\| j == n` |
| **Main diagonal** | `i == j` |
| **Anti-diagonal** | `i + j == n + 1` |
| **Cross (X)** | `i == j \|\| i + j == n + 1` |
| **Plus (+)** | `i == (n+1)/2 \|\| j == (n+1)/2` |

`lec1.cpp` already uses this model for the plus and the cross, and **the conditions are
correct** — `j==((n+1)/2) or i==((n+1)/2)` and `j==i or (i+j)==(n+1)` are exactly right.
Only the `endl` placement is wrong.

**Which model to use.** Segment decomposition when the row is a few contiguous runs
(triangles, pyramids). Grid conditions when the shape is defined by *which cells are on*
(hollow shapes, diagonals, anything symmetric). Some patterns are natural in both.

---

## 4. Composition: build big patterns from small ones

Most of the "hard" patterns are two easy patterns stacked:

| Pattern | = |
|---|---|
| Diamond | pyramid + inverted pyramid |
| Half diamond (arrow) | increasing triangle + decreasing triangle |
| Butterfly | two mirrored halves |
| Alpha-hill | pyramid, with the star loop replaced by up-then-down letters |

So the correct move on seeing a diamond is **not** to invent one clever loop. It's to write
the pyramid, write the inverted pyramid, and put them one after the other. Two simple loops
beat one loop with three counters — and the second is where off-by-one bugs breed.

`lec2.cpp` writes the diamond as a single loop with `nsp`/`nst` counters that toggle
direction. That approach works, but it's exactly why the loop bound came out wrong (see
`solution.md`) — the two-loop version has nothing to get wrong.

---

## 5. Complexity

A pattern with `n` rows prints roughly `n²/2` to `n²` characters, so it is **Θ(n²) time**
and you cannot do better — **the output itself is Θ(n²)**. That's the answer if an
interviewer asks whether you can optimise: *"No. The time complexity is bounded below by
the size of the output."*

**Space is O(1)** if you print as you go. If you build strings first it becomes O(n²), so
print directly.

One real optimisation exists: use `'\n'` rather than `endl`. `endl` flushes the stream on
every row, and for large `n` that dominates the runtime. See `../01_basics` §2.

---

## 6. Bugs this folder exists to prevent

**`cout << endl` inside the inner loop.** The newline belongs to the row.
(`lec1.cpp:31`, `lec1.cpp:44`)

**A constant where `i` belongs.** `for (j = 1; j <= r2-1; j++)` prints the same number of
spaces on every row, so the pyramid comes out as a left-aligned triangle shifted right.
(`lec2.cpp:24`)

**Shadowing the outer loop variable.** `for (int i = 1; i <= nst; i++)` inside a loop that
already uses `i` compiles and — here — happens to work, but any use of the outer `i` inside
would silently read the inner one. (`lec2.cpp:66`)

**Off-by-one in the row count.** A diamond has `2n - 1` rows (or `2n` if the widest row is
repeated). `i < 2*n - 1` gives you `2n - 2`. (`lec2.cpp:57`)

**Forgetting the space characters entirely.** Centred patterns are *spaces plus symbols*.
If you only print symbols you get a left-aligned triangle, which is a different pattern.

---

## Top 5 Interview Q&A

### Q1. Give me a general method for solving any pattern problem.

Four steps, always the same:

1. **Count the rows** — that's the outer loop bound.
2. **Split one row into segments** — typically `spaces`, then `symbols`, sometimes
   symbols-spaces-symbols for symmetric shapes.
3. **Tabulate segment lengths against the row index** and read off the formula. Write two
   or three rows out by hand; a linear sequence is obvious immediately.
4. **Decide what each position prints** — a constant, a running counter, or `char('A'+k)`.

Then the code writes itself: one inner loop per segment, and `cout << "\n"` once per row in
the **outer** loop's body.

The reason to state this as a method rather than solve the specific pattern is that
interviewers are testing whether you have a *procedure* or a memorised catalogue. Say the
method out loud, build the table on the whiteboard, and the code follows without guesswork.

---

### Q2. How do you handle hollow or symmetric patterns like a plus, a cross, or a hollow rectangle?

Switch models. Instead of decomposing each row into runs, **iterate the full `n × n` grid
and print the symbol only where a condition on `(i, j)` holds**:

```cpp
for (i = 1; i <= n; ++i) {
    for (j = 1; j <= n; ++j)
        cout << (CONDITION ? '*' : ' ');
    cout << "\n";
}
```

Then every shape is one boolean expression:

- Hollow rectangle: `i == 1 || i == n || j == 1 || j == n` — "on any border"
- Main diagonal: `i == j`; anti-diagonal: `i + j == n + 1`
- Cross: the **or** of the two diagonals
- Plus: `i == (n+1)/2 || j == (n+1)/2` — "on the middle row or middle column"

This model is dramatically easier for hollow shapes, because a hollow row is
`symbol, spaces…, symbol` — three segments whose lengths depend on whether the row is a
border row, which is a mess to decompose but trivial as a condition.

The transferable point: this is exactly how you'll think about **2D grids** in
`../08_2d array` and in matrix problems — spiral traversal, rotating a matrix, boundary
traversal are all "which cells, under what condition".

---

### Q3. Why is a pyramid row `n - i` spaces and `2i - 1` stars? Derive it, don't recite it.

Build the table for `n = 4`:

| row `i` | spaces | stars | width |
|---|---|---|---|
| 1 | 3 | 1 | 4 |
| 2 | 2 | 3 | 5 |
| 3 | 1 | 5 | 6 |
| 4 | 0 | 7 | 7 |

**Spaces** go `3, 2, 1, 0` — a decreasing arithmetic sequence reaching 0 at `i = n`. Any
sequence of that shape is `n - i`.

**Stars** go `1, 3, 5, 7` — the odd numbers, which are `2i - 1`. The structural reason is
that a centred shape grows by **one star on each side** per row, so it grows by 2 — and
starting from 1 gives the odds. That "+2 per row, so `2i - 1`" reasoning generalises to
every centred pattern.

**Then sanity-check:** the widest row is `i = n`, giving `2n - 1` stars — which is correct
for a pyramid of base width `2n-1`. If the arithmetic hadn't come out to a sensible total
width, one formula would be wrong.

The point of the question is method. A candidate who says "it's `2i-1`, I remember" and a
candidate who builds the table are distinguishable the moment the interviewer changes the
pattern slightly.

---

### Q4. What's the time complexity of printing a pattern, and can it be improved?

**Θ(n²) time**, and **no, it cannot be improved** — because the pattern *is* Θ(n²)
characters. The output size is a lower bound on the runtime; you cannot print n² characters
in fewer than n² operations.

**Space is O(1)** when you print directly to the stream. Building the rows into a string or
a 2D array first would make it O(n²) space for no benefit.

The one real optimisation is I/O, not algorithmic: **use `'\n'` instead of `endl`.** `endl`
flushes the output buffer on every row, forcing a system call. For `n` in the thousands
that flushing dominates the entire runtime. (In competitive programming you'd also add
`ios_base::sync_with_stdio(false); cin.tie(NULL);`.)

Recognising that output size bounds the complexity is a genuinely useful habit — it's the
same reasoning that tells you an algorithm listing all subsets can't beat O(2ⁿ), or that
printing all permutations can't beat O(n!·n).

---

### Q5. What are the classic bugs in pattern code, and how do you catch them fast?

**Five bugs account for nearly all failures:**

1. **`endl` inside the inner loop** → one character per line. The newline belongs to the
   *row*, so it goes in the outer loop's body.
2. **A constant where the loop index belongs** — `j <= n-1` instead of `j <= n-i` in the
   space loop. Every row gets identical indentation and the pyramid comes out slanted.
3. **Off-by-one in the row count** — a diamond is `2n-1` rows, and `i < 2*n-1` gives you
   `2n-2`, silently dropping the last row.
4. **Omitting the space segment** — you get a left-aligned triangle instead of a centred
   pyramid.
5. **Shadowing the outer loop variable** — declaring `int i` in an inner loop that sits
   inside another `i` loop. It compiles, and it breaks the moment you reference the outer
   one.

**How to catch them in seconds:** run with `n = 1`, `n = 2`, and `n = 3`. Almost every
off-by-one shows up at `n = 1` (does the pyramid print exactly one star?) or `n = 2`. And
**count the characters in the first and last rows against your table** — if the widest row
isn't `2n-1` wide, your formula is wrong before you even look at the shape.

Testing the smallest input first is a habit that pays off well beyond patterns — it's the
same instinct that catches empty-array and single-node cases in real problems.

---

## Common mistakes checklist

- [ ] `cout << "\n"` in the **outer** loop body, once per row
- [ ] Every inner-loop bound mentions `i` (unless the segment is genuinely constant)
- [ ] Space segments actually printed for centred patterns
- [ ] Row count checked: `n` for a triangle, `2n-1` or `2n` for a diamond
- [ ] Tested at `n = 1`, `n = 2`, `n = 3`
- [ ] Widest row is exactly `2n-1` characters for centred patterns
- [ ] No inner loop redeclares the outer loop's variable
- [ ] `'\n'` rather than `endl`

---

## Next

`questions.md` — all 22 Striver patterns, ranked.
`solution.md` — full code for 15, derivations for the rest.
Then move to `../05_Function`.
