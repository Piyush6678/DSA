# 15 — Bit Manipulation

Bit tricks are the smallest topic with the worst effort-to-reward ratio *if you memorise them*
and one of the best *if you learn the four or five that recur*. This folder is mostly the
latter set, plus the modular-arithmetic identities that share a page with them.

---

## 1. The operators

| Op | Name | Rule | Common use |
|---|---|---|---|
| `&` | AND | 1 only if both are 1 | **masking** — keep selected bits |
| `\|` | OR | 1 if either is 1 | **setting** bits |
| `^` | XOR | 1 if they **differ** | **toggling**, and cancelling pairs |
| `~` | NOT | flips every bit | building masks |
| `<<` | left shift | `x << k` = `x · 2^k` | |
| `>>` | right shift | `x >> k` = `x / 2^k` (for `x ≥ 0`) | |

### The four XOR identities that solve most problems

```
x ^ x = 0          x ^ 0 = x          XOR is commutative and associative
```

Together these mean **XOR-ing a list cancels every value that appears an even number of times**.
That single fact solves "find the single number", "find the missing number", and most of §5.

---

## 2. The tricks worth memorising

Everything else you can derive. These five you should know cold:

| Goal | Expression | Why |
|---|---|---|
| Test bit `i` | `(n >> i) & 1` | shift it down, mask off the rest |
| Set bit `i` | `n \| (1 << i)` | |
| Clear bit `i` | `n & ~(1 << i)` | |
| Toggle bit `i` | `n ^ (1 << i)` | |
| **Clear the lowest set bit** | `n & (n - 1)` | subtracting 1 flips that bit and everything below |
| **Isolate the lowest set bit** | `n & -n` | two's complement makes `-n` the complement plus 1 |
| Is a power of two? | `n > 0 && (n & (n-1)) == 0` | exactly one bit set |
| Count set bits | loop `n &= n-1` | Brian Kernighan — runs once per **set** bit |

`n & (n-1)` and `n & -n` are the two to really understand rather than memorise — they show up in
Fenwick trees, subset enumeration and the Single Number problems.

**Brian Kernighan's loop is O(number of set bits)**, not O(32). For sparse numbers that is a real
win, and it is the answer interviewers want over a 32-iteration loop.

---

## 3. Bit-width, signedness, and the traps

**`char` is signed on this toolchain and `int` is 32-bit.** Three consequences:

1. **`>>` on a negative int is implementation-defined** (arithmetic shift here — it copies the
   sign bit). `-8 >> 1` is `-4`, not a logical shift. For bit manipulation, use `unsigned`.
2. **`while (n > 0)` never runs for a negative `n`.** Any "count the bits" loop written that way
   returns 0 for negatives. Use `unsigned n` and `while (n)`.
3. **`1 << 31` overflows a signed `int`** — undefined behaviour. Write `1u << 31`, or `1LL << 31`
   when you need more than 32 bits.

### Operator precedence — the trap that looks like it works

```cpp
if (temp & 1 == 1)          // parses as  temp & (1 == 1)  ==  temp & 1
```

`==` binds **tighter** than `&`. Here it happens to give the right answer because `1 == 1` is `1`,
but `if (x & 3 == 3)` becomes `x & 1` and is simply wrong. **Always parenthesise:**
`if ((x & 3) == 3)`.

### Shifts do not modify their operand

`n >> 1;` on its own is a computed-and-discarded expression, exactly like `ans + to_string(c)` in
`../10_Recursion`. You need `n = n >> 1;` or `n >>= 1;`. This is the bug in `biwise.cpp:103` and
it produces an **infinite loop**, not a wrong number.

---

## 4. Bitmasks as sets

A 32-bit int is a subset of a 32-element universe. This is how subset enumeration, `../26_dp`
bitmask DP, and "longest nice subarray" all work:

```
for mask in 0 .. (1<<n) - 1:            # every subset
    for i in 0 .. n-1:
        if mask & (1 << i):  element i is IN this subset
```

| Set operation | Bit expression |
|---|---|
| Union | `a \| b` |
| Intersection | `a & b` |
| Difference | `a & ~b` |
| Add element `i` | `a \| (1 << i)` |
| Is `i` present? | `a & (1 << i)` |
| Size | `__builtin_popcount(a)` |

`__builtin_popcount` is a GCC builtin and compiles to a single instruction on modern CPUs. It
exists; use it, but be able to write the loop when asked.

---

## 5. Modular arithmetic

`modulo.cpp` covers this, and it belongs with bit manipulation because both are about keeping
numbers inside a fixed width.

```
(a + b) % m == ((a % m) + (b % m)) % m
(a - b) % m == ((a % m) - (b % m) + m) % m        <-- the +m matters
(a * b) % m == ((a % m) * (b % m)) % m
```

**The `+ m` in the subtraction rule is not optional.** In C++, `%` on a negative left operand
returns a **negative** result: `(-7) % 5` is `-2`, not `3`. Adding `m` before the final `%` folds
it back into `[0, m)`. This is the same trap as LeetCode 974 in `../13_prefixSum` §9.

**Division has no such rule** — you need the modular inverse (Fermat's little theorem:
`a⁻¹ ≡ a^(m-2) mod m` when `m` is prime), which is binary exponentiation from `../10_Recursion`.

**Multiplication overflows before the modulo saves you.** With `m = 10⁹+7`, two reduced values
still reach ~10¹⁸ when multiplied — that fits `long long` but not `int`. Every intermediate in a
modular product must be `long long`.

---

## Interview Q&A

**Q1. How do you check whether a number is a power of two?**
`n > 0 && (n & (n - 1)) == 0`. A power of two has exactly one set bit, and `n & (n-1)` clears the
lowest set bit — so the result is 0 only when there was exactly one. **The `n > 0` guard is
required**: 0 passes the second test but is not a power of two, and negatives misbehave.

**Q2. Count the set bits in an integer. What's the best you can do?**
`while (n) { count++; n &= n-1; }` — Brian Kernighan, O(set bits) rather than O(32). Or
`__builtin_popcount(n)`, one instruction. Take `unsigned`, or negatives fail the loop guard.

**Q3. Every element appears twice except one. Find it.**
XOR everything. Pairs cancel (`x ^ x == 0`) and the survivor is the answer. O(n) time, **O(1)
space** — no map, no sorting.

**Q4. Same, but exactly two elements appear once.**
XOR everything to get `x = a ^ b`. Since `a ≠ b`, `x` has at least one set bit; isolate the
lowest with `x & -x`. That bit differs between `a` and `b`, so partition the array on it and XOR
each half separately. Two passes, O(1) space.

**Q5. Every element appears three times except one — why doesn't XOR work?**
Because XOR cancels in *pairs*, and three copies leave one behind. Two fixes: count each bit
position mod 3 (O(32n)); or the two-variable `ones`/`twos` state machine that tracks "seen once"
and "seen twice" simultaneously.

**Q6. Why use `unsigned` for bit manipulation?**
Right-shifting a negative signed int is implementation-defined (arithmetic shift on GCC — it
propagates the sign bit, so the loop never reaches 0), left-shifting into the sign bit is
undefined behaviour, and `while (n > 0)` skips negatives entirely. `unsigned` has none of these
problems and wraps predictably.

**Q7. What does `n & -n` give you, and why?**
The lowest set bit, isolated. In two's complement `-n == ~n + 1`, which flips every bit above the
lowest set bit and leaves that bit and the zeros below it unchanged — so ANDing keeps exactly
that one bit. It is the core of Fenwick tree indexing.

---

## No `readme.md` was present

This folder had no problem list of its own, so nothing needed preserving. `questions.md` is
sourced from Striver's A2Z Step 7 (bit manipulation) plus the LeetCode set, scoped to what
`01`–`14` support.
