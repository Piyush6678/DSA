# 15 — Bit Manipulation: Practice Questions

> **Why 20 ranked problems.** Bit manipulation is a small toolbox — maybe eight expressions —
> attached to four families that each need their own reps: **bit surgery** (test/set/clear/count,
> where the code is one line and the value is fluency), the **XOR-cancellation** family (single
> number and its three variants, which are genuinely different problems despite looking alike),
> **bitmask-as-set** (subset enumeration, the bridge to `../26_dp`), and **arithmetic without
> arithmetic** (divide, add, power). Twenty is four or five per family. Going further means
> collecting party tricks; going shorter means dropping a family, and the three-times variant in
> particular is asked far too often to skip.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search
the quoted title.

**Scope note.** Everything here needs only `01`–`14`. This folder has **no `readme.md` problem
list** of its own, so the set is drawn from Striver's A2Z Step 7 plus the standard LeetCode
questions, scoped to what earlier folders support.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Number of 1 Bits | Easy | **LeetCode 191** | `number-of-1-bits` — Brian Kernighan; `biwise.cpp:34` |
| 2 | Power of Two | Easy | **LeetCode 231** | `power-of-two` — `n & (n-1)` in one line |
| 3 | Single Number | Easy | **LeetCode 136** | `single-number` — XOR cancellation; `biwise.cpp:88` |
| 4 | Missing Number | Easy | **LeetCode 268** | `missing-number` — the XOR solution; cf. `../12_sorting` §1 #7 |
| 5 | Counting Bits | Easy | **LeetCode 338** | `counting-bits` — `dp[i] = dp[i>>1] + (i&1)` |
| 6 | Reverse Bits | Easy | **LeetCode 190** | `reverse-bits` — needs `unsigned` |
| 7 | Single Number II (three times) | **Medium** | **LeetCode 137** | `single-number-ii` — **XOR alone does not work** |
| 8 | Single Number III (two singles) | **Medium** | **LeetCode 260** | `single-number-iii` — `x & -x`; `biwise.cpp:94` |
| 9 | Subsets via bitmask | **Medium** | **LeetCode 78** | `subsets` — the iterative form; cf. `../10_Recursion` §1 #8 |

**Why these nine.** #1–#2 are the two expressions everything else is built from, and doing them
first means `n & (n-1)` is reflexive by the time it matters. #3 is the XOR identity in its purest
form and the answer to a question asked constantly. #4 is worth doing *here* even though you met
it in `../12_sorting`, because the XOR solution and the cycle-sort solution are completely
different and comparing them is the lesson — XOR cannot overflow, which the `n(n+1)/2` version
can. #5 is your first bit-DP recurrence and the bridge to `../26_dp`. #6 is where signedness
bites for the first time.

**#7 and #8 are the two that earn the folder its place.** #7 is the standard "you cannot just
XOR" problem — the three-copies case breaks pair cancellation, and both fixes (bit-count mod 3,
or the `ones`/`twos` state machine) are worth knowing. #8 is the `x & -x` partition, which is the
single most elegant bit trick in common use. **You have attempted #8 in `biwise.cpp:94` and it
hangs** — see `solution.md`. #9 turns the recursive power set into a loop and is the entry point
to bitmask DP.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 10 | XOR of numbers in a range `[L, R]` | Easy | GFG | *"Find XOR of numbers from L to R"* — the mod-4 pattern |
| 11 | Highest power of 2 ≤ N | Easy | GFG | *"Find the highest occurring digit"* → *"Smallest power of 2 greater than or equal to n"* — `biwise.cpp:53` |
| 12 | Flip bits of a number | Easy | GFG | *"Set bits"* → *"Flipping bits"* — `biwise.cpp:70` |
| 13 | Minimum bit flips to convert A to B | Easy | **LeetCode 2220** | `minimum-bit-flips-to-convert-number` — `popcount(a^b)`; `biwise.cpp:82` |
| 14 | Divide Two Integers | **Medium** | **LeetCode 29** | `divide-two-integers` — no `/`, and `INT_MIN` is the test |
| 15 | Sum of Two Integers | **Medium** | **LeetCode 371** | `sum-of-two-integers` — no `+` |
| 16 | Power of Four | Easy | **LeetCode 342** | `power-of-four` — #2 plus a position check |
| 17 | Bitwise AND of Numbers Range | **Medium** | **LeetCode 201** | `bitwise-and-of-numbers-range` — the common prefix |

**Why these eight.** #10's mod-4 pattern (`XOR(1..n)` cycles `n, 1, n+1, 0`) is the kind of
result you derive once and never re-derive — and it turns a range XOR into two O(1) lookups.
#11–#13 are the three functions you already wrote; #11 is correct, #12 and #13 are worth
revisiting for the reasons in `solution.md`.

**#14 is the most-asked problem in this section** and it is really an overflow question: the only
case that matters is `INT_MIN / -1`, which overflows because `|INT_MIN| > INT_MAX`. #15 is the
full-adder written out — carry is `(a & b) << 1`, sum is `a ^ b` — and it is where you see why
signed left-shift needs care. **#17 is the sneaky one**: the AND of every number in a range is
the *common binary prefix* of the endpoints, because any bit that changes anywhere in the range
gets zeroed. Shifting both ends right until they agree is a two-line solution to something that
looks like it needs a loop over the range.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 18 | Maximum XOR of Two Numbers in an Array | **Medium** | **LeetCode 421** | `maximum-xor-of-two-numbers-in-an-array` — trie or prefix-set |
| 19 | Longest subarray with maximum bitwise AND | **Medium** | GFG | *"Longest subarray with maximum bitwise AND"* — `biwise.cpp:117` |
| 20 | Longest Nice Subarray | **Medium** | **LeetCode 2401** | `longest-nice-subarray` — bitmask as sliding-window state |

**Why these three.** #18 is the standard hard bit problem: build the answer bit by bit from the
top, keeping the set of prefixes seen so far, and at each step ask "can I achieve a 1 here?" It
introduces the bit-trie idea used in a whole family of XOR problems. **#19 is the one you have
already attempted and is the most instructive of the three**, because the trap is not the bit
operation at all — the maximum AND of any subarray equals the maximum *element* (ANDing anything
else only clears bits), so the problem reduces to "longest run of the maximum value", and the word
**run** is what your version misses. #20 combines this folder with `../14_Sliding window`: the
window state is an OR-mask, and an element may join iff `(mask & a[r]) == 0`.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Complement of Base 10 Integer | Easy | **LeetCode 1009** | build the mask, then XOR — same as #12 |
| Hamming Distance | Easy | **LeetCode 461** | `popcount(x ^ y)`; identical to #13 |
| Binary Number with Alternating Bits | Easy | **LeetCode 693** | `n ^ (n>>1)` gives all ones — then #2 |
| Number Complement | Easy | **LeetCode 476** | #12 with a judge attached |
| Add Binary | Easy | **LeetCode 67** | string carry addition, not bit ops — good contrast |
| Convert decimal to binary and back | Easy | *Drill* | `biwise.cpp:6,17` — check `0` and negatives |
| Swap two numbers without a temp | Easy | *Drill* | `a^=b; b^=a; a^=b;` — **fails when `&a == &b`** |
| Check if two integers have opposite signs | Easy | *Drill* | `(a ^ b) < 0` |
| Count total set bits from 1 to N | **Medium** | GFG | *"Count total set bits"* — per-bit-position counting |
| Gray Code | **Medium** | **LeetCode 89** | `i ^ (i >> 1)` |
| Single Number with one missing and one repeating | **Medium** | GFG | *"Repeat and Missing Number Array"* — #8's partition |
| Modular exponentiation | **Medium** | GFG | *"Modular Exponentiation for large numbers"* — `modulo.cpp` + `../10_Recursion` §1 #7 |
| Modular inverse | **Medium** | GFG | *"Modular multiplicative inverse"* — Fermat; needs #12 above |
| Factorials mod 10⁹+7 | Easy | *Drill* | `modulo.cpp:6` — verify the identities hold |
| Bitmask DP: assign tasks to people | **Hard** | GFG | *"Number of ways to assign"* — **forward reference**, `../26_dp` |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8   [ ] 9
Section 2   [ ] 10  [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15  [ ] 16  [ ] 17
Section 3   [ ] 18  [ ] 19  [ ] 20
Section 4   [ ] ______ / 15
```
