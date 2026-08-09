# 19 — Infix / Prefix / Postfix: Practice Questions

> **Why only 12 ranked problems — the smallest set in the repo.** This topic is genuinely finite:
> there are three notations, so there are six conversions and three evaluations, and that is
> nearly all of it. The six conversions are not six problems either — four of them reduce to
> "reverse, convert, reverse" once you have infix→postfix. What is left is the shunting-yard
> algorithm, the two evaluators, and the calculator problems that apply them to real input.
> Twelve is honest; padding it would mean listing the same conversion with different operators.
> As with `../13_prefixSum`, a small count is a fact about the topic, not a shortcut.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search the
quoted title. Several entries are **Drills** because the conversions are viva and written-round
questions rather than judge problems.

**Scope note.** Everything needs only `01`–`18`. This folder has **no `readme.md` problem list**
of its own.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Infix to Postfix | **Medium** | GFG | *"Infix to Postfix"* — the shunting-yard algorithm; `postfix.cpp` |
| 2 | Evaluate Postfix expression | Easy | GFG | *"Evaluation of Postfix Expression"* |
| 3 | Evaluate Reverse Polish Notation | **Medium** | **LeetCode 150** | `evaluate-reverse-polish-notation` — #2 with a judge |
| 4 | Infix to Prefix | **Medium** | GFG | *"Infix to Prefix"* — reverse, convert, reverse; `prefix.cpp` |
| 5 | Evaluate Prefix expression | Easy | GFG | *"Evaluation of Prefix Expression"* — **operand order inverts** |

**Why these five.** #1 is the algorithm the whole folder rests on — everything else is either an
application of it or a transformation into it. Do it with `^` included, because right
associativity is what separates a converter that works from one that only looks like it does.

**#2 and #3 are the same problem** and worth doing in that order: #2 on paper with single digits to
fix the operand order, then #3 where the judge feeds you multi-character tokens and negative
numbers. **The operand order is the entire trap** — `b = pop(); a = pop();` for postfix, and
`a = pop(); b = pop();` for prefix. Commutative operators hide the mistake, so test with `-`.

#4 is the reverse-swap-reverse method, and **#5's inverted pop order** is the detail that makes it
worth a separate entry rather than a footnote to #2.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 6 | Postfix to Infix | Easy | GFG | *"Postfix to Infix Conversion"* — build bracketed strings |
| 7 | Prefix to Infix | Easy | GFG | *"Prefix to Infix Conversion"* |
| 8 | Postfix to Prefix | **Medium** | GFG | *"Postfix to Prefix Conversion"* |
| 9 | Prefix to Postfix | **Medium** | GFG | *"Prefix to Postfix Conversion"* |
| 10 | Basic Calculator II | **Medium** | **LeetCode 227** | `basic-calculator-ii` — precedence, no brackets |

**Why these five.** #6–#9 complete the six conversions, and the reason they are Section 2 rather
than Section 1 is that **they are all one algorithm**: pop two operands, combine them with the
operator in the target notation, push the result back. The only things that change are the scan
direction (left-to-right for postfix input, right-to-left for prefix) and where the operator goes
in the combined string. Write one and the other three are edits, which is exactly the observation
worth having.

Note #6 and #7 must **add brackets** around every combination — infix is the only ambiguous
notation, so `a+b*c` reconstructed without brackets would lose the original grouping.

**#10 is where this becomes a real interview question.** No brackets, but precedence must still be
honoured: keep a stack of terms, push `+x` or `-x` directly, and for `*` or `/` pop the top and
combine immediately. The answer is the sum of the stack. It is the shunting-yard idea reduced to
its minimum.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Basic Calculator | **Hard** | **LeetCode 224** | `basic-calculator` — brackets and unary minus |
| 12 | Basic Calculator III | **Hard** | **LeetCode 772** | `basic-calculator-iii` — brackets **and** precedence |

**Why these two.** #11 has brackets and `+`/`-` only; the elegant solution keeps a stack of
`(result, sign)` pairs and pushes the current state on `(`, restoring it on `)` — no full
conversion needed. **Unary minus is the real content**: `-(3+4)` and `1-(-2)` both have to work,
and detecting a minus that follows an operator or an opening bracket is the fix.

**#12 is the union of #10 and #11** — brackets *and* precedence — and it is the point where writing
a proper recursive-descent parser, or converting to postfix first and evaluating that, becomes
cleaner than any amount of special-casing. Reaching that conclusion yourself is the value of the
problem.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Check for balanced brackets in an expression | Easy | **LeetCode 20** | prerequisite; `../18_stack` §1 #4 |
| Remove redundant brackets | **Medium** | GFG | *"Removing brackets"* — detect a bracket pair with no operator inside |
| Build an expression tree from postfix | **Medium** | GFG | *"Expression Tree"* — **forward reference**, `../21_tree` |
| Evaluate an expression tree | **Medium** | **LeetCode 1628** | **forward reference** — post-order traversal |
| Infix to postfix with multi-digit operands | **Medium** | *Drill* | accumulate digits into one token |
| Infix to postfix with unary minus | **Medium** | *Drill* | detect by context; high precedence, right associative |
| Handle `^` right associativity | Easy | *Drill* | `a^b^c` must give `abc^^`, not `ab^c^` |
| Handle left associativity | Easy | *Drill* | `a-b-c` must give `ab-c-`; the case `prefix.cpp` gets wrong |
| Convert and evaluate — round trip | Easy | *Drill* | `eval(toPostfix(e))` must equal `eval(toPrefix(e))` |
| Number of ways to bracket an expression | **Hard** | **LeetCode 241** | `different-ways-to-add-parentheses` — divide and conquer |
| Valid expression checker | **Medium** | *Drill* | operand/operator alternation plus balanced brackets |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5
Section 2   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 3   [ ] 11  [ ] 12
Section 4   [ ] ______ / 11
```
