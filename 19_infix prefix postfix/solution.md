# 19 — Infix / Prefix / Postfix: Solutions

Approach, pseudocode, complexity and key insight. Real C++ appears for the **shunting-yard
converter** and the **two evaluators** — this folder is an implementation topic, and the operand
order and associativity clause are exactly the details prose loses.

**Everything shown as code was compiled with `g++ -std=gnu++14` and executed** — part of a
101-assertion run across folders 18–20, all passing. Measured outputs are quoted.

---

# Section 1 — Must Do

## 1 & 4. Infix → Postfix and Infix → Prefix — **implementation, code given**

One function serves both. The only difference is a flag controlling what happens between
operators of **equal** precedence.

```cpp
int prec(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    if (c == '^')             return 3;
    return 0;                                  // '(' and anything else
}

// popOnEqual decides the equal-precedence case:
//   forward pass  (postfix): pop  -> LEFT associative
//   reversed pass (prefix):  keep -> associativity inverts when the string is reversed
string toPostfixCore(const string& s, bool popOnEqual) {
    string out; stack<char> op;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (isalnum((unsigned char)c)) out += c;              // operand -> straight to output
        else if (c == '(') op.push(c);
        else if (c == ')') {
            while (!op.empty() && op.top() != '(') { out += op.top(); op.pop(); }
            if (!op.empty()) op.pop();                        // discard the '('
        } else if (prec(c) > 0) {
            bool eq = (c == '^') ? !popOnEqual : popOnEqual;  // '^' is right associative
            while (!op.empty() && op.top() != '(' &&
                   (prec(op.top()) > prec(c) ||
                    (prec(op.top()) == prec(c) && eq)))
            { out += op.top(); op.pop(); }
            op.push(c);
        }
    }
    while (!op.empty()) { out += op.top(); op.pop(); }
    return out;
}

string infixToPostfix(const string& s) { return toPostfixCore(s, true); }

string infixToPrefix(string s) {
    reverse(s.begin(), s.end());
    for (size_t i = 0; i < s.size(); ++i) {                   // swap the brackets
        if (s[i] == '(') s[i] = ')';
        else if (s[i] == ')') s[i] = '(';
    }
    string post = toPostfixCore(s, false);                    // <-- inverted associativity
    reverse(post.begin(), post.end());
    return post;
}
```

**Key insight — three things, and the third is the one people miss.**

1. **`op.top() != '('` in the while condition.** A `(` must never be popped by a precedence
   comparison; it leaves only when its matching `)` arrives. Giving it precedence 0 is a second
   line of defence.
2. **Operands go straight out; operators wait.** An operator sits on the stack until something of
   equal-or-higher precedence forces it off. That is the whole algorithm.
3. **The reversed pass must invert associativity.** Reversing the string reverses the order
   operators are met, so a left-associative `-` behaves right-associatively on the reversed text.
   Without the inversion, `a-b-c` converts to `-a-bc`, which parses as `a-(b-c)` — a *different
   value*.

> **I got this wrong first.** My initial `infixToPrefix` reused the postfix converter unchanged
> and produced `-a-bc` for `a-b-c` and `++79-/483` for your test expression. The round-trip check
> (`eval(postfix) == eval(prefix)`) is what caught it. `a+b*c` passes either way, which is exactly
> why the bug is easy to ship.

**Complexity.** O(n) time, O(n) space.

**Verified:**

| Input | Postfix | Prefix |
|---|---|---|
| `(7+9)+4/8-3` | `79+48/+3-` | `-++79/483` |
| `a+b*c` | `abc*+` | `+a*bc` |
| `(a+b)*c` | `ab+c*` | `*+abc` |
| `a-b-c` (left assoc) | `ab-c-` | `--abc` |
| `a^b^c` (right assoc) | `abc^^` | `^a^bc` |
| `a+b*c-d/e` | `abc*+de/-` | — |

Plus a round-trip check on four expressions: `evalPostfix(toPostfix(e)) == evalPrefix(toPrefix(e))`.

---

## 2, 3 & 5. Evaluating postfix and prefix — **implementation, code given**

```cpp
long long evalPostfix(const string& s) {
    stack<long long> st;
    for (size_t i = 0; i < s.size(); ++i) {          // LEFT to right
        char c = s[i];
        if (isdigit((unsigned char)c)) { st.push(c - '0'); continue; }
        long long b = st.top(); st.pop();            // SECOND operand comes off FIRST
        long long a = st.top(); st.pop();
        switch (c) { case '+': st.push(a+b); break;  case '-': st.push(a-b); break;
                     case '*': st.push(a*b); break;  case '/': st.push(a/b); break; }
    }
    return st.top();
}

long long evalPrefix(const string& s) {
    stack<long long> st;
    for (int i = (int)s.size() - 1; i >= 0; --i) {   // RIGHT to left
        char c = s[i];
        if (isdigit((unsigned char)c)) { st.push(c - '0'); continue; }
        long long a = st.top(); st.pop();            // FIRST operand comes off FIRST
        long long b = st.top(); st.pop();
        switch (c) { case '+': st.push(a+b); break;  case '-': st.push(a-b); break;
                     case '*': st.push(a*b); break;  case '/': st.push(a/b); break; }
    }
    return st.top();
}
```

**Key insight — the operand order inverts between the two, and that is the whole trap.**

- **Postfix**, scanning left to right: the right operand was pushed most recently, so it pops
  first. `b = pop(); a = pop();`
- **Prefix**, scanning right to left: the *left* operand was pushed most recently, so it pops
  first. `a = pop(); b = pop();`

Get it backwards and `+` and `*` still give the right answer — they commute — while `-` and `/`
silently do not. **Always test with subtraction.** `"92-"` must be `7`, and `"-92"` must also be
`7`.

No precedence handling appears anywhere, which is the entire point of these notations.

**For LeetCode 150** the tokens are strings, not characters: split on whitespace, and treat a
token as an operand if it is longer than one character or is a digit (so `"-11"` is a number, not
an operator).

**Complexity.** O(n) time, O(n) space.

**Verified:** `evalPostfix("79+48/+3-") = 13`, `evalPostfix("231*+9-") = -4`,
`evalPostfix("92-") = 7`; `evalPrefix("-++79/483") = 13`, `evalPrefix("-92") = 7`.

---

# Section 2 — Important

## 6–9. The other four conversions — **one algorithm, four edits**

All four have the same shape: scan, push operands, and on an operator pop two, combine, push back.

```
scan direction:   postfix input -> LEFT to right      prefix input -> RIGHT to left
on an operand:    push it
on an operator:   pop two operands (order per the table below)
                  combine into the TARGET notation
                  push the combined string
answer:           the single string left on the stack
```

| Conversion | Scan | First pop is | Combine as |
|---|---|---|---|
| Postfix → Infix | left → right | `b` (right operand) | `"(" + a + op + b + ")"` |
| Postfix → Prefix | left → right | `b` | `op + a + b` |
| Prefix → Infix | right → left | `a` (left operand) | `"(" + a + op + b + ")"` |
| Prefix → Postfix | right → left | `a` | `a + b + op` |

**Key insight — two points.**

1. **The scan direction is set by the *input* notation; the combine order by the *output*.** Those
   are independent choices, which is why one function with two parameters covers all four.
2. **The infix targets must add brackets.** Infix is the only ambiguous notation — reconstructing
   `a+b*c` without brackets would lose the grouping that the postfix form encoded. `(a+(b*c))` is
   correct even though the brackets look redundant; removing them safely is a separate problem
   (§4).

This is also the **direct** method for infix→prefix that your `prefix.cpp` uses — combining
`op + val1 + val2` on a value stack instead of reverse-convert-reverse. Both are valid; see the
notes on your files below.

**Complexity.** O(n) time, O(n·length) space for the intermediate strings.

---

## 10. Basic Calculator II — LeetCode 227

**Approach.** Precedence without brackets, in one pass, using a stack of *terms*.

```
stack of long long;  num = 0;  op = '+'          # the operator BEFORE the current number
for each character c (and one extra iteration at the end):
    if digit:  num = num*10 + (c - '0')          # multi-digit accumulation
    if c is an operator or we are at the end:
        if op == '+':  push(num)
        if op == '-':  push(-num)                # store the sign IN the value
        if op == '*':  x = pop(); push(x * num)
        if op == '/':  x = pop(); push(x / num)  # C++ truncates toward zero -- what LC wants
        op = c;  num = 0
answer = sum of the stack
```

**Key insight — defer `+`/`-`, apply `*`/`/` immediately.** Higher-precedence operators can be
resolved on the spot because nothing later can outrank them; lower-precedence ones must wait, and
"waiting" means sitting on the stack as a signed term. Summing at the end applies them all at
once. That is the shunting-yard idea with the general machinery stripped out.

**Two details that are always tested:** skip spaces, and process the pending operator once more
after the loop ends (or append a sentinel `+` to the input) so the final number is not dropped.

**Complexity.** O(n) time, O(n) space.

---

# Section 3 — Good to Know

## 11. Basic Calculator — LeetCode 224

**Approach.** Brackets with only `+` and `-`. No conversion needed — keep a running result and a
current sign, and stack the *context* when a bracket opens.

```
result = 0;  sign = +1;  num = 0;  stack empty
for each character c:
    digit          -> num = num*10 + digit
    '+' or '-'     -> result += sign * num;  num = 0;  sign = (c=='+' ? +1 : -1)
    '('            -> push(result);  push(sign);  result = 0;  sign = +1
    ')'            -> result += sign * num;  num = 0
                      result *= pop();          # the sign that preceded the '('
                      result += pop();          # the result accumulated before it
result += sign * num
```

**Key insight — push the *state*, not the expression.** On `(` you save what you have so far and
start fresh; on `)` you fold the sub-result back in, multiplied by the sign that preceded the
bracket. That handles `1-(2+3)` correctly, which is where naive approaches fail.

**Unary minus is the real content.** `-(3+4)` and `1-(-2)` must both work. Initialising `sign = +1`
and `result = 0` handles a leading `-` for free, because the first term is added to zero.

**Complexity.** O(n) time, O(n) space.

---

## 12. Basic Calculator III — LeetCode 772

Brackets **and** precedence — the union of #10 and #11.

**Key insight — stop special-casing.** At this point the clean answers are either (a) convert to
postfix with the shunting-yard algorithm (#1) and evaluate it (#2), or (b) write a small
recursive-descent parser: `expr → term (('+'|'-') term)*`, `term → factor (('*'|'/') factor)*`,
`factor → number | '(' expr ')'`. Reaching that conclusion — that the general algorithm is now
*simpler* than another layer of special cases — is what the problem is for.

Option (a) reuses everything in Section 1 and is the answer to give when this folder is the
context.

---

# Section 4 — approach only

- **Balanced brackets (20)** — prerequisite; `../18_stack/solution.md` §1 #4.
- **Remove redundant brackets** — a bracket pair is redundant if no operator appears at its top
  level. Track, per open bracket, whether an operator was seen before the matching close.
- **Expression tree from postfix** — same stack shape as #6–#9, but push *nodes* instead of
  strings: pop two, make them children of an operator node, push it back. Evaluating the tree is a
  post-order traversal. Needs `../21_tree`.
- **Multi-digit operands** — accumulate consecutive digit characters into one token instead of
  treating each character as an operand. Every real converter needs this; single-digit versions are
  teaching code.
- **Unary minus** — detect by context: a `-` at the start of the expression, or immediately after
  `(` or another operator. Give it high precedence and right associativity, or rewrite it as
  `0 - x`.
- **`^` right associativity** — `a^b^c` must give `abc^^`. Test it; it is the case a `>=` pop
  condition gets wrong.
- **Left associativity** — `a-b-c` must give `ab-c-`. This is the case the reversed pass gets
  wrong without the inversion in #1.
- **Round trip** — `evalPostfix(toPostfix(e))` must equal `evalPrefix(toPrefix(e))` for every `e`.
  A four-line test that catches the associativity bug immediately.
- **Different Ways to Add Parentheses (241)** — divide and conquer: split at each operator,
  recursively evaluate both sides, combine every pair. Not a stack problem — included as a contrast.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `19_infix prefix postfix/` was edited.**

Both files **compile and both produce the correct answer for their test expression** — verified:

| File | Output for `(7+9)+4/8-3` | Correct |
|---|---|---|
| `postfix.cpp` | `79+48/+3-` | ✓ |
| `prefix.cpp` | `-++79/483` | ✓ |

The problems below are all latent — inputs your `main` does not use.

## `prio()` returns 2 for brackets — both files, line 7

```cpp
int prio(char ch){
    if(ch=='+'||ch=='-')return 1;
    else return 2;                      // '(' and ')' also get 2
}
```

Everything that is not `+` or `-` is given precedence 2, including `(`. That matters at line 42
and 44, where `prio(op.top())` is compared — if `(` is on top with precedence 2, a `*` will not be
pushed above it correctly and a `+` will try to pop it.

In practice the `op.top()=='('` guard at line 27 catches most of these before the comparison
happens, which is why the test expression works. Give `(` precedence **0** and the guard becomes a
belt-and-braces check rather than the only defence.

`^` is also unhandled — it would get precedence 2, the same as `*`.

## The branch order puts `)` after two conditions that can swallow it — both files, line 27

```cpp
if(!op.size() || s[i]=='(' || op.top()=='(' ) op.push(s[i]);
else if(s[i]==')') { ... }
```

A `)` reaches its own branch only if the stack is non-empty *and* its top is not `(`. Two cases
slip through:

- **A bracket pair containing no operator** — at the `)`, `op.top()` is `(`, so the **first**
  condition fires and `)` is pushed onto the operator stack. The final flush then treats it as a
  binary operator and pops two operands that are not there.
- **A `)` when the operator stack is empty** — malformed input, pushed rather than rejected.

**Verified: `run("(1)")` segfaults.** With an underflow guard added, the trace is explicit:

```
*** UNDERFLOW at final flush: operator ')', operands=1
```

`(1)` is not a contrived input — any bracketed single term (`(a)*b`, or a redundant pair a user
typed) hits it.

**Fix:** test `s[i]==')'` **first**, before any other case.

## Equal precedence and associativity — `prefix.cpp` line 44

```cpp
while(op.size()>0 && prio(op.top())>=prio(s[i])){ ... }
```

`>=` means "pop on equal precedence", which is correct **left**-associative behaviour and right
for `postfix.cpp`.

But `prefix.cpp` uses the **direct** method — building `op + val1 + val2` on a value stack rather
than reverse-convert-reverse — and for that method the `>=` is also correct, because the string is
*not* reversed. So both files are right here. The associativity trap I describe in §1 applies to
the reverse-convert-reverse method, which is the more common way to write infix→prefix and the one
your files do not use.

**Verified — the direct method is correct on every left-associative case I could construct:**

| Infix | `prefix.cpp` output | Evaluates to | Correct |
|---|---|---|---|
| `9-2-3` | `--923` | 4 | ✓ (not `-9-23` = 10) |
| `8/4/2` | `//842` | 1 | ✓ (not `/8/42` = 4) |
| `1-2+3` | `+-123` | 2 | ✓ |
| `(7+9)+4/8-3` | `-++79/483` | 13 | ✓ |
| `2+3*4` | `+2*34` | 14 | ✓ |
| `(1+2)*(3-1)` | `*+12-31` | 6 | ✓ |

`9-2-3` and `8/4/2` are precisely the inputs that break the reverse-convert-reverse method when
its associativity is not inverted. Your approach never reverses the string, so the question does
not arise. See "What you got right".

## Single digits only — both files, line 23

```cpp
if(s[i]>=48 && s[i]<=57){ val.push(to_string(s[i]-48)); }
```

`"12+3"` is read as three operands `1`, `2`, `3` and produces nonsense. Accumulate consecutive
digits into one token. Spaces are also not skipped — `"7 + 9"` treats each space as an operator and
falls into the `else` branch.

`s[i]>=48 && s[i]<=57` is correct but `isdigit((unsigned char)s[i])` says what it means.

## No guards on `val.top()` / `op.top()`

Every pop sequence assumes at least two operands are present. On malformed input
(`"+"`, `"(("`, `"1+"`) these are undefined behaviour rather than an error. Fine for a learning
file; worth a sentence in an interview.

---

## What you got right

- **Both converters produce correct output**, verified against hand-computed answers for
  `(7+9)+4/8-3`. That expression exercises brackets, two precedence levels, and a trailing
  lower-precedence operator — it is a better test case than most textbook examples.

- **`prefix.cpp` uses the direct method rather than reverse-convert-reverse**, and this is worth
  calling out: it builds prefix strings on the value stack by putting the operator first
  (`solve()` does `s.push_back(ch)` then appends the operands). That sidesteps the associativity
  inversion entirely — the trap I hit when writing the reverse-based version for §1. Whether or
  not it was deliberate, it is the more robust of the two approaches.

- **The two files differ by exactly three lines** — the body of `solve()`. Keeping them otherwise
  identical makes the difference between the notations visible at a glance, which is good
  pedagogy.

- **Using a `stack<string>` for values rather than a `stack<char>`** is what allows partial results
  to be combined and re-pushed. That is the key structural decision in the direct method and you
  got it right.

- **The final flush loop** (lines 59–69) correctly drains any operators left after the scan. It is
  duplicated from the body rather than factored out, but it is present and correct — omitting it is
  a common bug.

- **`op.top()=='('` in the push condition** is what stops operators being popped past an opening
  bracket. It is doing the work that a `prio('(') == 0` would do more cleanly, but it does work.
