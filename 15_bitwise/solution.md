# 15 — Bit Manipulation: Solutions

Approach, pseudocode, complexity and key insight. Real C++ appears only for the **fundamental
one-liners** (where the expression *is* the answer and prose would be longer than the code) and
the **trick** problems — Single Number II/III, divide-without-division.

**Everything shown as code was compiled and executed — part of a 77-assertion run across folders
14–17, all passing.** Measured outputs are quoted.

---

# Section 1 — Must Do

## 1. Number of 1 Bits — LeetCode 191 — **fundamental**

```cpp
int countSetBits(unsigned n) {
    int c = 0;
    while (n) { ++c; n &= n - 1; }      // clear the lowest set bit each time
    return c;
}
```

**Key insight.** `n - 1` flips the lowest set bit to 0 and turns every bit below it into 1;
ANDing therefore clears exactly that one bit. The loop runs **once per set bit**, not 32 times —
Brian Kernighan's algorithm. `__builtin_popcount(n)` is one instruction and is the right answer
in production.

**Take `unsigned`.** With a signed `int` and `while (n > 0)`, a negative input skips the loop
entirely and returns 0.

**Complexity.** O(set bits), O(1) space.

**Verified:** `13→3`, `0→0`, `(unsigned)-1→32`.

---

## 2. Power of Two — LeetCode 231 — **fundamental**

```cpp
bool isPowerOfTwo(int n) { return n > 0 && (n & (n - 1)) == 0; }
```

**Key insight.** Exactly one set bit ⟺ clearing the lowest set bit leaves zero. **`n > 0` is
required** — 0 passes `(n & (n-1)) == 0` but is not a power of two, and negatives in two's
complement can pass too.

**Complexity.** O(1).

---

## 3. Single Number — LeetCode 136

**Approach.** XOR every element. Pairs cancel; the survivor is the answer.

```
ans = 0
for x in a:  ans ^= x
return ans
```

**Key insight.** `x ^ x == 0` and `x ^ 0 == x`, and XOR is commutative and associative — so the
order does not matter and every duplicated value vanishes regardless of position. O(1) space,
which is what the problem is actually testing; a hash map is the obvious solution and the wrong
answer.

**Your `unique1` at `biwise.cpp:88` is exactly this and is correct.**

**Complexity.** O(n) time, O(1) space.

---

## 4. Missing Number — LeetCode 268

**Approach.** XOR all the indices `0..n` together with all the values. Everything present cancels
and the missing number survives.

```
ans = n
for i in 0 .. n-1:  ans ^= i ^ a[i]
return ans
```

**Key insight.** Preferable to `n(n+1)/2 − sum` because **XOR cannot overflow**, while the sum
can for large `n`. Both are O(n)/O(1); mention both and say why you picked this one. The
cycle-sort placement version is in `../12_sorting/solution.md` §7.

---

## 5. Counting Bits — LeetCode 338

**Approach.** `dp[i] = dp[i >> 1] + (i & 1)`.

**Key insight.** `i >> 1` is `i` with its last bit removed, and that value is strictly smaller so
it is already computed. Set bits of `i` = set bits of everything above the last bit, plus the last
bit itself. O(n) total instead of O(n · 32).

`dp[i] = dp[i & (i-1)] + 1` also works and is arguably prettier — it says "one more than the
number with the lowest set bit removed".

---

## 6. Reverse Bits — LeetCode 190

```
res = 0
for i in 0 .. 31:
    res = (res << 1) | (n & 1)
    n >>= 1
return res
```

**Key insight.** Build the result by shifting it left while shifting the input right — the two
move in opposite directions. **`n` must be `unsigned`**: `>>` on a negative signed int copies the
sign bit here, so the loop feeds in 1s forever.

Always exactly 32 iterations — you cannot stop early, because leading zeros in the input become
trailing zeros in the output and still have to be shifted through.

---

## 7. Single Number II — LeetCode 137 — **trick, code given**

**Why XOR fails.** XOR cancels in *pairs*. Three copies leave one behind, so the result is
polluted by every element.

**Approach A — count each bit position mod 3.** For each of the 32 positions, sum that bit across
all numbers and take mod 3; what remains belongs to the unique element. O(32n), easy to explain,
and handles negatives if you assemble into an `unsigned`.

**Approach B — the `ones`/`twos` state machine:**

```cpp
int singleNumberII(vector<int>& a) {
    int ones = 0, twos = 0;
    for (int x : a) {
        ones = (ones ^ x) & ~twos;
        twos = (twos ^ x) & ~ones;
    }
    return ones;
}
```

**Key insight.** `ones` holds the bits seen exactly once so far, `twos` the bits seen exactly
twice. A bit appearing a third time is present in `twos`, so `& ~twos` prevents it re-entering
`ones` — and the updated `ones` then clears it from `twos`. After three appearances a bit is in
neither, so only the unique element survives in `ones`.

**The order of the two lines matters.** `twos` is computed using the *already-updated* `ones`;
swapping them breaks it. This is a case where the code must be seen exactly, which is why it is
here rather than in pseudocode.

**Complexity.** O(n) time, O(1) space.

**Verified:** `{2,2,3,2}→3`, `{0,1,0,1,0,1,99}→99`, `{-2,-2,1,-2}→1` (negatives work — the
bit-level reasoning is sign-agnostic).

---

## 8. Single Number III — LeetCode 260 — **trick, code given**

```cpp
vector<int> singleNumberIII(vector<int>& a) {
    long long x = 0;
    for (int v : a) x ^= v;                    // x == p ^ q
    int diff = (int)(x & -x);                  // lowest bit where p and q DIFFER
    int p = 0, q = 0;
    for (int v : a) { if (v & diff) p ^= v; else q ^= v; }
    return {min(p,q), max(p,q)};
}
```

**Key insight.** XOR-ing everything gives `p ^ q`. Since `p ≠ q` that is non-zero, so it has at
least one set bit — and **any** set bit is a position where `p` and `q` differ. Partition the
whole array on that bit: `p` and `q` land in different groups, every duplicate pair lands together
in one group, and XOR-ing each group separately recovers one answer each.

`x & -x` isolates the lowest set bit because in two's complement `-x == ~x + 1`.

`x` is accumulated in `long long` so that `x & -x` is safe even when `x` is `INT_MIN` (negating
`INT_MIN` as an `int` overflows).

**Complexity.** O(n) time, O(1) space, two passes.

**Verified:** `{1,2,1,3,2,5}→{3,5}`; `{2,4,2,6,4,8}→{6,8}` — the second case has bit 0 clear in
the XOR, which is exactly what breaks the version in `biwise.cpp`.

---

## 9. Subsets via bitmask — LeetCode 78 — **fundamental**

```cpp
for (int mask = 0; mask < (1 << n); ++mask) {
    vector<int> cur;
    for (int i = 0; i < n; ++i)
        if (mask & (1 << i)) cur.push_back(a[i]);
    res.push_back(cur);
}
```

**Key insight.** Each of the 2ⁿ integers *is* a subset: bit `i` set means element `i` is included.
No recursion, no backtracking, no undo step. This is the iterative counterpart to
`../10_Recursion` §1 #8, and the mental model — an int is a set — is what `../26_dp`'s bitmask DP
runs on.

`n` must be < 31, or `1 << n` overflows.

**Complexity.** O(2ⁿ · n) time.

**Verified:** `{1,2,3}` → 8 subsets.

---

# Section 2 — Important

## 10. XOR of a range `[L, R]`

**Approach.** `XOR(L..R) = XOR(1..R) ^ XOR(1..L-1)` — prefix XOR, since XOR is its own inverse.
And `XOR(1..n)` has a period-4 closed form:

```
n % 4 == 0  ->  n
n % 4 == 1  ->  1
n % 4 == 2  ->  n + 1
n % 4 == 3  ->  0
```

**Key insight.** Consecutive groups of four cancel to zero, so only `n mod 4` matters. Two O(1)
lookups replace a loop over the range.

**Verified against brute force for every n from 1 to 200.**

---

## 11. Highest power of 2 ≤ N — **fundamental**

```cpp
unsigned highestPowerOf2(unsigned n) {
    if (n == 0) return 0;
    n |= n>>1; n |= n>>2; n |= n>>4; n |= n>>8; n |= n>>16;   // smear: all bits below MSB set
    return (n + 1) >> 1;
}
```

**Key insight.** The cascade of shift-ORs turns any number into `2^k − 1` (all ones up to its
highest set bit) — each step doubles the number of leading ones, so five steps cover 32 bits.
Adding 1 gives `2^k`, and halving gives the highest power of two ≤ n.

**Your `maxPower` at `biwise.cpp:53` is this exactly, and it is correct.** Verified:
`100→64`, `1→1`, `2→2`, `7→4`, `8→8`, `0→0`, `63→32`, `64→64`.

**Complexity.** O(1).

---

## 12. Flip the bits of a number

**Approach.** Build the full smear mask (as in #11) and XOR — but **do not** halve it.

```cpp
unsigned flipBits(unsigned a) {
    if (a == 0) return 1;
    unsigned m = a;
    m |= m>>1; m |= m>>2; m |= m>>4; m |= m>>8; m |= m>>16;   // full mask
    return m ^ a;                                              // NOT ((m+1)>>1) ^ a
}
```

**Key insight.** You want to flip every bit *within the number's own width*, so the mask must be
all the ones — `1010` needs mask `1111` to become `0101`. `((m+1)>>1)` is the single highest bit,
which is #11's answer, not a mask. **This is the one-line difference between your `maxPower`
(correct) and your `flip` (wrong)** — see the bugs section.

**Complexity.** O(1).

**Verified:** `10→5`, `1→0`, `7→0`, `0→1`.

---

## 13. Minimum Bit Flips to Convert A to B — LeetCode 2220

**Approach.** `popcount(a ^ b)`.

**Key insight.** XOR is 1 exactly where the two differ, and each differing position costs one
flip. **Your `flipConvert` at `biwise.cpp:82` is correct** — one line, and it is the right one.

---

## 14. Divide Two Integers — LeetCode 29 — **trick, code given**

```cpp
int divide(int dividend, int divisor) {
    if (dividend == INT_MIN && divisor == -1) return INT_MAX;   // the ONLY overflow case
    long long a = llabs((long long)dividend), b = llabs((long long)divisor), res = 0;
    while (a >= b) {
        long long t = b, m = 1;
        while (a >= (t << 1)) { t <<= 1; m <<= 1; }   // largest doubling that still fits
        a -= t; res += m;
    }
    bool neg = (dividend < 0) ^ (divisor < 0);
    return (int)(neg ? -res : res);
}
```

**Key insight — this is long division in binary.** Repeatedly subtract the largest
`divisor × 2^k` that still fits, accumulating `2^k` into the quotient. Each outer iteration
removes at least the top remaining bit, so it is O(log² n), not O(quotient).

**The `INT_MIN / -1` guard is the entire interview.** `|INT_MIN| = 2147483648 > INT_MAX`, so the
true answer is not representable — the problem specifies clamping to `INT_MAX`. Taking absolute
values in `long long` avoids a second overflow at `llabs(INT_MIN)`.

**Complexity.** O(log² n) time, O(1) space.

**Verified:** `10/3→3`, `7/-3→-2` (truncation toward zero, not floor), `INT_MIN/-1→INT_MAX`,
`INT_MIN/1→INT_MIN`.

---

## 15. Sum of Two Integers — LeetCode 371

**Approach.** A full adder. `a ^ b` is the sum ignoring carries; `(a & b) << 1` is the carry.
Repeat until there is no carry left.

```
while b != 0:
    carry = (a & b) << 1
    a = a ^ b
    b = carry
return a
```

**Key insight.** Compute the carry **before** overwriting `a`. The loop terminates because each
carry is strictly further left, so after at most 32 rounds it shifts out.

**Do the shift on `unsigned`.** `(a & b) << 1` on negative signed ints is undefined behaviour;
cast to `unsigned` for the shift and back afterwards.

**Verified:** `3+5→8`, `-3+5→2`, `0+0→0`.

---

## 16. Power of Four — LeetCode 342

**Approach.** A power of two (#2) whose single set bit sits at an **even** position.

```
n > 0 && (n & (n-1)) == 0 && (n & 0x55555555) != 0
```

**Key insight.** `0x55555555` is `0101…0101` — the even bit positions. Powers of four are
`1, 4, 16, 64…` = bits 0, 2, 4, 6. `(n - 1) % 3 == 0` is an equivalent test and worth knowing as
the arithmetic alternative.

---

## 17. Bitwise AND of Numbers Range — LeetCode 201

**Approach.** The answer is the **common binary prefix** of `left` and `right`.

```
shift = 0
while left != right:  left >>= 1; right >>= 1; shift++
return left << shift
```

**Key insight.** Any bit position that changes anywhere in `[left, right]` contributes a 0 to the
AND — and if two numbers in the range differ at position `i`, then somewhere in between that bit
is 0. So only the leading bits where `left` and `right` already agree survive. Shifting both
right until they are equal finds that prefix in O(log n) without touching the range.

---

# Section 3 — Good to Know

## 18. Maximum XOR of Two Numbers — LeetCode 421

**Approach.** Build the answer from the top bit down. At each bit, assume you can achieve a 1 and
check whether any pair of prefixes seen so far realises it.

```
ans = 0
for bit from 30 down to 0:
    ans |= (1 << bit)                                  # optimistically claim this bit
    prefixes = { x >> bit for x in a }                 # all prefixes at this width
    if no p in prefixes with (p ^ ans) in prefixes:
        ans ^= (1 << bit)                              # retract the claim
return ans
```

**Key insight.** `a ^ b == target` ⟺ `a ^ target == b`, so testing "is some pair achievable" is a
set lookup, not a nested loop. The bit-trie formulation is the same algorithm with the prefix set
made explicit as a tree, and it generalises to the whole family of XOR-query problems.

**Complexity.** O(32n) time, O(n) space.

---

## 19. Longest subarray with maximum bitwise AND

**Approach.** Two observations collapse this to a trivial scan:

1. **The maximum achievable AND is the maximum element.** ANDing any other element in can only
   clear bits, never set them — so the best AND value is `max(a)`, achieved by a subarray of
   maximum elements alone.
2. So the answer is the **longest run of consecutive occurrences** of the maximum.

```
best_val = max(a)
run = 0, ans = 0
for x in a:
    if x == best_val:  run++;  ans = max(ans, run)
    else:              run = 0            # <-- the reset is the whole problem
return ans
```

**Key insight — the word is *run*, not *count*.** Occurrences of the maximum that are not
adjacent do not form a subarray. **Your `lenSubarray` at `biwise.cpp:117` never resets `run`**,
so it counts total occurrences instead — see the bugs section.

**Complexity.** O(n) time, O(1) space.

---

## 20. Longest Nice Subarray — LeetCode 2401

**Approach.** Sliding window (`../14_Sliding window`) whose **state is a bitmask** — the OR of
everything currently in the window.

```
mask = 0, l = 0, best = 0
for r in 0 .. n-1:
    while (mask & a[r]) != 0:        # a[r] shares a bit with something in the window
        mask ^= a[l];  l++           # XOR removes it, because it is definitely present
    mask |= a[r]
    best = max(best, r - l + 1)
```

**Key insight.** "Every pair in the window has AND zero" is equivalent to "no two elements share
a set bit", which a single OR-mask tests in O(1). Removal uses `^=` rather than `&= ~`, which is
safe precisely because the bit is known to be present.

**Complexity.** O(n) time, O(1) space.

---

# Section 4 — approach only

- **Complement of Base 10 Integer (1009) / Number Complement (476)** — #12 exactly. Build the
  smear mask, XOR. Both need the `n == 0 → 1` special case.
- **Hamming Distance (461)** — `popcount(x ^ y)`, identical to #13.
- **Alternating Bits (693)** — `n ^ (n >> 1)` is all ones iff the bits alternate; then apply #2 to
  `result + 1`.
- **Add Binary (67)** — string carry addition. Included as a contrast: it looks like a bit problem
  and is not.
- **Swap without a temp** — `a^=b; b^=a; a^=b;`. **Broken when both operands are the same object**
  (`swap(x, x)` zeroes it) — the same self-aliasing bug as the additive swap in `../07_Array`.
- **Opposite signs** — `(a ^ b) < 0`. The sign bits differ iff the XOR is negative.
- **Count total set bits 1..N** — count per bit position: position `i` cycles with period `2^(i+1)`,
  giving `(n+1)/2^(i+1) × 2^i` full cycles plus a partial remainder.
- **Gray Code (89)** — `i ^ (i >> 1)`. Consecutive values differ in exactly one bit; proving that
  is the question.
- **Modular exponentiation** — binary exponentiation from `../10_Recursion` §1 #7 with a `% m`
  after every multiply, all intermediates in `long long`.
- **Modular inverse** — Fermat: `a⁻¹ ≡ a^(m-2) mod m` for prime `m`. Uses the line above.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `15_bitwise/` was edited.**

## `biwise.cpp:94` — `unique2` never terminates

```cpp
while(true){
    if(temp&1==1){break;}
    temp>>1;                        // <-- computed and DISCARDED
    k++;
}
```

`temp >> 1` is an expression with no side effect — it does not modify `temp`. When the XOR's
lowest bit is 0, the loop condition never changes.

**Verified: hangs on both inputs tested.**

| Input | XOR of the two singles | Result |
|---|---|---|
| `{1,2,1,3,2,5}` | `3 ^ 5 = 6` (bit 0 clear) | **no termination after 5,000,000 iterations** |
| `{2,4,2,6,4,8}` | `6 ^ 8 = 14` (bit 0 clear) | **no termination after 5,000,000 iterations** |

**Fix:** `temp >>= 1;`. Same class of bug as `ans+to_string(count)` in `../10_Recursion` — an
expression whose result is thrown away, which the compiler does not warn about.

**Two more faults in the same function:**

1. **`int retval;` is uninitialised** (`:107`) and then used with `retval ^= number`. Reading an
   uninitialised variable is undefined behaviour. It must start at 0.
2. **`if(temp&1==1)` and `if ((number>>k)&1 ==1)` parse as `temp & (1==1)`** — `==` binds tighter
   than `&`. Both happen to give the right answer because `1==1` is `1`, but the same shape with
   any other constant (`x & 3 == 3`) is silently wrong. Parenthesise: `if ((temp & 1) == 1)`.

The whole function is replaced by #8, which finds the differing bit with `x & -x` in one step
instead of looping to locate it.

## `biwise.cpp:70` — `flip` returns the wrong thing

```cpp
n=((n+1)>>1);          // <-- this line turns the MASK into the single highest bit
return n^a;
```

The smear cascade above it correctly builds `1111…` up to `a`'s highest set bit. Then
`((n+1)>>1)` reduces it to just the top bit — which is what `maxPower` wants, but not what a bit
flip wants.

**Verified: `flip(10)` returns 2. Flipping `1010` within its own width gives `0101` = 5.**

**Fix:** delete that line and return `n ^ a` with the full mask. Also add the `a == 0` case, which
should return 1.

## `biwise.cpp:117` — `lenSubarray` counts occurrences, not runs

```cpp
if(arr[i]>max_el){ max_el=arr[i]; count=1; }
else if (arr[i]==max_el){ count++; }
// nothing resets count when arr[i] < max_el
```

A non-maximum element should end the current run. Without the reset, occurrences separated by
other values are counted as though contiguous.

**Verified:**

| Input | Your output | Correct |
|---|---|---|
| `{1,2,3,3,2,2}` | 2 | 2 ✓ (adjacent, so it happens to agree) |
| `{3,1,3}` | **2** | 1 — the two 3s are not adjacent |
| `{2,2,1,2}` | **3** | 2 — the run is broken by the 1 |

**Fix:** add `else { count = 0; }`. Full version in #19.

## `biwise.cpp:17` — `decimalToBinary(0)` returns an empty string

`while(num > 0)` never executes for 0. **Verified: returns `""`; should return `"0"`.** Negative
inputs also return `""`.

**Fix:** `if (num == 0) return "0";` up front, and take `unsigned` (or handle two's complement
explicitly) if negatives matter.

## `biwise.cpp:34` — `SetBits` returns 0 for negative input

`while (n > 0)` skips negatives entirely. **Verified: `SetBits(-1)` returns 0; the answer is 32.**
Take `unsigned n` and write `while (n)`.

Line 36, `__builtin_popcount(13);`, computes a value nothing reads — harmless, but it looks like a
line that was meant to do something.

## `modulo.cpp:10` — a correctness note, not a bug

```cpp
fact[i]=((i%mod) *(fact[i-1]%mod))%mod;
```

This is correct as written — `i` and `fact[i-1]` are already `long long` in the expression because
`fact` is `vector<long long>`, so the product is evaluated in 64 bits. Worth knowing **why** it is
safe rather than assuming: with `mod = 10⁹+7`, two reduced operands multiply to just under 10¹⁸,
which fits `long long` (max ≈ 9.2 × 10¹⁸) but would overflow `int` many times over. If `fact` were
`vector<int>`, the same line would be broken.

`int mod` should be `const long long mod` for the same reason — it works here only because the
other operand forces the promotion.

`main` is empty, so `fact()` has never been run.

---

## What you got right

- **`maxPower` (`:53`) is correct.** Verified on 0, 1, 2, 7, 8, 63, 64 and 100 — all right. The
  shift-OR smear is a genuinely non-obvious technique and you implemented it exactly, including
  the `>>16` step that most people forget. You also left your slower `n & (n-1)` version in a
  comment above it, which is a good habit: the two agree, and one is O(set bits) while the other
  is O(1).

- **`unique1` (`:88`) is correct** — the XOR-cancellation solution to LeetCode 136, in four lines
  and O(1) space.

- **`flipConvert` (`:82`) is correct** — `__builtin_popcount(x^y)` is exactly right, and knowing
  the builtin exists is worth as much as the insight.

- **`binary_to_decimal` (`:6`) is correct** — verified `"100"→4`. Using `1 << (n-i-1)` for the
  place value rather than a running multiplication is the better form.

- **Brian Kernighan is the method you chose** for `SetBits`, with the naive shift loop kept in a
  comment as method 1. That is the right preference and the comment names the algorithm — only
  the signed-input guard is missing.

- **`modulo.cpp`'s three identities in the comments are all stated correctly**, including
  `(a-b)%c == (a%c - b%c + c)%c` with the `+c`. That `+c` is the exact thing that breaks LeetCode
  974, and you had written it down before meeting the problem.
