# 19 — Infix, Prefix, Postfix

A small, self-contained folder: six conversions and two evaluations, all built on `../18_stack`.
It earns its place because expression parsing is where a stack stops being a toy and starts being
the reason compilers work — and because the conversions are asked in vivas and written rounds far
more often than their difficulty suggests.

---

## 1. The three notations

| Notation | Operator position | `2 + 6 * 4` |
|---|---|---|
| **Infix** | between operands | `2 + 6 * 4` |
| **Prefix** (Polish) | before operands | `+ 2 * 6 4` |
| **Postfix** (Reverse Polish) | after operands | `2 6 4 * +` |

**Only infix needs precedence rules and brackets.** Prefix and postfix are unambiguous by
construction — the position of the operator already says which operands it takes, so
`2 6 4 * +` can only mean one thing. That is exactly why machines use postfix and humans use
infix.

A useful way to see it: all three are the same expression tree, read in a different traversal
order.

```
      +
     / \
    2   *          pre-order  -> + 2 * 6 4     (prefix)
       / \         in-order   -> 2 + 6 * 4     (infix)
      6   4        post-order -> 2 6 4 * +     (postfix)
```

If `../10_Recursion/PreInPost.cpp` felt abstract, this is what it was for.

---

## 2. Precedence and associativity

| Operator | Precedence | Associativity |
|---|---|---|
| `^` (power) | 3 | **right** — `a^b^c` is `a^(b^c)` |
| `*` `/` | 2 | left |
| `+` `-` | 1 | left |
| `(` | 0 (never popped by precedence) | — |

**Associativity decides what happens between operators of *equal* precedence**, and it is the
detail that separates a working converter from one that passes the easy cases:

- **Left associative** (`-`, `/`): pop the stacked operator before pushing the new one, so
  `a-b-c` becomes `ab-c-` — i.e. `(a-b)-c`.
- **Right associative** (`^`): do *not* pop, so `a^b^c` becomes `abc^^` — i.e. `a^(b^c)`.

Give `(` precedence 0 so it is never popped by a precedence comparison. It is only removed by a
matching `)`.

---

## 3. Infix → postfix (the shunting-yard algorithm)

```
for each token c:
    if operand:            append to output
    else if c == '(':      push
    else if c == ')':      pop to output until '('; discard the '('
    else (an operator):
        while stack not empty and top != '(' and
              (prec(top) > prec(c) or (prec(top) == prec(c) and c is LEFT associative)):
            pop to output
        push c
flush the remaining stack to output
```

**Three things to get right:**

1. **`top != '('` in the while condition.** Without it, a `(` gets popped into the output.
2. **The equal-precedence clause is where associativity lives** (§2).
3. **Discard both brackets.** Neither appears in postfix — they exist only to override precedence,
   and postfix has no precedence to override.

**Operands go straight to the output; operators wait on the stack** until something of equal or
higher precedence forces them out. That is the whole algorithm in one sentence.

---

## 4. Infix → prefix: reverse, convert, reverse

The clean method is three steps, not a separate algorithm:

```
1. reverse the infix string
2. swap every '(' with ')' and vice versa
3. run infix->postfix -- but with associativity INVERTED
4. reverse the result
```

**Step 3's inversion is the part everyone misses.** Reversing the string reverses the order the
operators are encountered, which flips their effective associativity: a left-associative `-`
behaves right-associatively on the reversed string. So in the reversed pass you must **not** pop
on equal precedence (and `^`, being right-associative, becomes the exception that *does*).

Concretely, `a-b-c` must give `--abc`. Without the inversion you get `-a-bc`, which parses as
`a-(b-c)` — a different value. This is a real bug that only shows up with two same-precedence
operators in a row, so `a+b*c` will pass while `a-b-c` silently fails.

**Direct conversion is also possible** — build prefix strings on a value stack, emitting
`operator + left + right` — and that is what your two files do. It works and is arguably more
elegant; §Solutions covers both.

---

## 5. Evaluating postfix and prefix

```
POSTFIX: scan LEFT to right
    operand -> push
    operator -> b = pop; a = pop; push(a OP b)        # a is the FIRST operand

PREFIX: scan RIGHT to left
    operand -> push
    operator -> a = pop; b = pop; push(a OP b)        # a is the FIRST operand
```

**The operand order is the trap, and it is inverted between the two.** For postfix, the *second*
popped value is the left operand; for prefix, the *first* popped is. Get it backwards and `+` and
`*` still work — they are commutative — while `-` and `/` silently produce wrong answers. Test
with subtraction: `92-` must be `7`, not `-7`.

No precedence logic is needed at all, which is the point of these notations.

---

## Interview Q&A

**Q1. Why do computers use postfix?**
It needs no brackets and no precedence rules, so evaluation is a single left-to-right pass with
one stack — no lookahead, no backtracking. Infix requires precedence resolution before you can
evaluate anything.

**Q2. Convert `(A+B)*C-D` to postfix.**
`AB+C*D-`. Walk it: `A` out, `+` pushed, `B` out, `)` flushes `+` → `AB+`; `*` pushed; `C` out;
`-` has lower precedence than `*` so `*` pops → `AB+C*`; `-` pushed; `D` out; flush → `AB+C*D-`.

**Q3. How do you convert infix to prefix?**
Reverse the string, swap the brackets, convert to postfix **with associativity inverted**, then
reverse the result. The inversion is essential — without it, `a-b-c` comes out as `a-(b-c)`.

**Q4. What is the time and space complexity?**
Both conversion and evaluation are **O(n) time and O(n) space** — each token is pushed and popped
at most once, and the stack can hold all of them in the worst case (`((((a`).

**Q5. Where does associativity actually matter?**
Only between operators of equal precedence. `a-b-c` and `a^b^c` are the test cases; `a+b*c` passes
regardless because the precedences differ. A converter that only handles mixed precedence looks
correct and is not.

**Q6. How would you extend this to multi-digit numbers and unary minus?**
Multi-digit: accumulate consecutive digit characters into one token rather than treating each
character as an operand. Unary minus: detect it by context — an operator appearing at the start of
an expression or immediately after `(` or another operator — and give it high precedence with
right associativity, or rewrite it as `0 - x`.

---

## No `readme.md` was present

This folder had no problem list of its own, so nothing needed preserving. `questions.md` is the
standard conversion/evaluation set plus the calculator problems that build on it, scoped to
`01`–`18`.
