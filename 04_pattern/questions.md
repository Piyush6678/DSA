# 04 — Patterns: Practice Questions

**All 22 patterns from Striver A2Z Step 1.2**, ranked by how much technique each one
teaches. Sections 1–3 are the 15 that carry the ideas; Section 4 is the remaining 7, which
are variations you should be able to derive once you've done the first 15.

**Platform note — read this one.** Pattern printing is **not on LeetCode**. LeetCode
problems return values; patterns print to stdout, so they live on the practice portals
instead:

- **takeuforward** — [takeuforward.org/strivers-a2z-dsa-course/](https://takeuforward.org/strivers-a2z-dsa-course/) → Step 1.2. All 22 on one page with the reference output.
- **Naukri Code360** (formerly Coding Ninjas) — Striver's pattern problems are hosted here as judge submissions; search the pattern name.
- **GeeksforGeeks** — search the pattern name on [practice.geeksforgeeks.org](https://practice.geeksforgeeks.org). No numeric IDs.

**Do these by hand.** The value is in deriving the formula, not in getting a green tick.
For each one: write the output for `n = 4` on paper, build the spaces/symbols table, *then*
write the loops.

---

## Section 1 — Must Do (the shapes every other pattern is built from)

| # | Striver # | Pattern | Difficulty | Output for `n = 4` |
|---|---|---|---|---|
| 1 | P1 | Rectangular star pattern | Easy | 4×4 block of `*` |
| 2 | P2 | Right-angled star triangle | Easy | `*` / `**` / `***` / `****` |
| 3 | P5 | Inverted right-angled triangle | Easy | `****` / `***` / `**` / `*` |
| 4 | P7 | Star pyramid (centred) | Easy | `   *` / `  ***` / ` *****` / `*******` |
| 5 | P9 | Diamond | Easy | pyramid stacked on inverted pyramid |

**Why these five.** #1 establishes the two-loop skeleton. #2 is the first bound that depends
on `i`. #3 is the same idea reversed, which is where off-by-one errors first appear. **#4 is
the most important pattern in the folder** — it introduces the space segment and the
`n-i` / `2i-1` pair that every centred pattern reuses. #5 teaches **composition**: a diamond
is not a new problem, it's #4 followed by its mirror.

---

## Section 2 — Important

| # | Striver # | Pattern | Difficulty | Output for `n = 4` |
|---|---|---|---|---|
| 6 | P8 | Inverted star pyramid | Easy | `*******` / ` *****` / `  ***` / `   *` |
| 7 | P10 | Half diamond / arrow | Easy | `*` `**` `***` `****` `***` `**` `*` |
| 8 | P11 | Binary number triangle | Easy | `1` / `01` / `101` / `0101` |
| 9 | P21 | Hollow rectangle | **Medium** | border `*`, hollow interior |
| 10 | P12 | Number crown | **Medium** | `1____1` / `12__21` / `123321` |

**Why these five.** #6 inverts the space/star relationship, so you must rederive rather
than reuse. #7 is composition again, with the subtlety that the peak row must not print
twice. #8 swaps the *character* rule instead of the *count* rule — the loop shape is
unchanged, which is the lesson. **#9 is the best pattern here for interviews**: it's the
grid-condition model in its purest form, and boundary conditions on a grid is a real skill.
#10 is the first three-segment row (`up`, `spaces`, `down`), which is where careful
tabulation stops being optional.

---

## Section 3 — Good to Know

| # | Striver # | Pattern | Difficulty | Output for `n = 4` |
|---|---|---|---|---|
| 11 | P13 | Increasing number triangle | Easy | `1` / `2 3` / `4 5 6` / `7 8 9 10` |
| 12 | P17 | Alpha-hill pattern | **Medium** | `   A` / `  ABA` / ` ABCBA` / `ABCDCBA` |
| 13 | P19 | Symmetric void pattern | **Medium** | wide → narrow → wide |
| 14 | P20 | Symmetric butterfly | **Medium** | narrow → wide → narrow |
| 15 | P22 | The number pattern (concentric squares) | **Hard** | `4444444` / `4333334` / `4322234` / `4321234` … |

**Why these five.** #11 introduces a **running counter** that persists across rows — the
value no longer depends on `i` and `j` alone. #12 combines the pyramid's spacing with a
palindromic character sequence. #13 and #14 are mirror images of each other and are the
best test of whether you actually derive formulas or pattern-match from memory — they look
almost identical and the arithmetic is opposite. **#15 is the hardest pattern in the
folder** and the most interesting: it's `value = n - min(distance to each of the four
edges)`, which is a genuinely different way of thinking and is exactly how you'd write a
distance-from-boundary function on a grid.

---

## Section 4 — Extra Practice (the remaining 7 Striver patterns)

Derive these yourself. Each is a variation on something in Sections 1–3; the note says
which.

| Striver # | Pattern | Difficulty | Output for `n = 4` | Variation of |
|---|---|---|---|---|
| P3 | Right-angled number pyramid | Easy | `1` / `12` / `123` / `1234` | #2, print `j` instead of `*` |
| P4 | Number pyramid, same digit per row | Easy | `1` / `22` / `333` / `4444` | #2, print `i` instead of `*` |
| P6 | Inverted numbered right triangle | Easy | `1234` / `123` / `12` / `1` | #3, print `j` |
| P14 | Increasing letter triangle | Easy | `A` / `AB` / `ABC` / `ABCD` | #2, print `char('A' + j - 1)` |
| P15 | Reverse letter triangle | Easy | `ABCD` / `ABC` / `AB` / `A` | #3, print `char('A' + j - 1)` |
| P16 | Alpha-ray triangle | Easy | `A` / `BB` / `CCC` / `DDDD` | #2, print `char('A' + i - 1)` |
| P18 | Alpha-triangle | **Medium** | `D` / `CD` / `BCD` / `ABCD` | #2, but the starting letter moves each row |

**Notice the pattern in the "variation of" column.** Six of these seven are Section 1's
triangles with the *character rule* changed and the *loop structure* untouched. That is the
whole point of ranking them last — once you've internalised that counts and characters are
independent, these cost about thirty seconds each.

**P18 is the exception** and is worth real attention: the row doesn't start at `'A'`, it
starts at `'A' + (n-i)` and counts up to `'A' + n - 1`. Both ends move, which is why it's
the only Medium in this section.

---

## Progress tracker

```
Section 1   [ ] P1   [ ] P2   [ ] P5   [ ] P7   [ ] P9
Section 2   [ ] P8   [ ] P10  [ ] P11  [ ] P21  [ ] P12
Section 3   [ ] P13  [ ] P17  [ ] P19  [ ] P20  [ ] P22
Section 4   [ ] P3   [ ] P4   [ ] P6   [ ] P14  [ ] P15  [ ] P16  [ ] P18
```

**Target:** all 22, from a blank file, deriving each formula rather than recalling it.
When you can do that, you are done with patterns forever — move to `../05_Function`.
