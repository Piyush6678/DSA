# 18 — Stack: Solutions

Approach, pseudocode, complexity and key insight. Real C++ appears for the **implementations**
(the whole exercise is writing them), the **monotonic stack template** (fundamental), the
**trick** designs (min stack), and the **hard** ones (histogram, trapping water).

**Everything shown as code was compiled with `g++ -std=gnu++14` and executed — 101 assertions
across folders 18–20, all passing.** Measured outputs are quoted.

---

# Section 1 — Must Do

## 1–3. Implement a stack — **`[impl]`, code given**

### Array-backed

```cpp
class ArrayStack {
    static const int CAP = 100;
    int arr[CAP];
    int idx;                                   // index of the top; -1 when empty
public:
    ArrayStack() : idx(-1) {}
    bool empty() const { return idx == -1; }
    int  size()  const { return idx + 1; }

    bool push(int v) {
        if (idx == CAP - 1) return false;      // OVERflow  -- '==', not '='
        arr[++idx] = v;
        return true;
    }
    bool pop() {
        if (idx == -1) return false;           // UNDERflow
        --idx;
        return true;
    }
    bool top(int& out) const {                 // out-parameter: can report failure
        if (idx == -1) return false;
        out = arr[idx];
        return true;
    }
};
```

**Key insight — three things, and your files miss all three.**

1. **`==`, not `=`.** `if (idx = 99)` assigns and evaluates to true, so the guard fires on *every*
   call. Measured: with that bug, two pushes are both refused and `size()` returns **100**.
2. **Return a status.** Printing "underflow" and returning tells the caller nothing. A `bool` (or
   an exception) lets them react. `top()` uses an out-parameter for the same reason — there is no
   `int` value that safely means "empty".
3. **Overflow ≠ underflow.** Pushing onto a full stack is *overflow*; both messages in
   `userDefinedStackArray.cpp` say "underflow".

### Vector-backed

Drop `idx` entirely — `v.size()` and `v.back()` already track it, and two sources of truth is one
too many. `push_back` / `pop_back` / `back()` map exactly onto the stack interface, and there is
no capacity limit.

### Linked-list-backed

```
push(v):  n = new Node(v);  n->next = head;  head = n;  ++size
pop():    if (!head) return false
          old = head;  head = head->next;  delete old;  --size;  return true
top():    if (!head) return false;  out = head->val;  return true
```

**Key insight — push at the *head*.** Head insertion and head removal are both O(1). Using the
tail would make `pop` O(n) on a singly linked list, because you would need the predecessor. The
head is naturally both ends of a LIFO.

**Two structural notes** on `linkedlistimplementation.cpp`: it has no `public:` label, so
everything including the constructor is private and the class is unusable from outside; and there
is no `main`, so the file does not link. Both are in the bugs section.

**Complexity.** All operations O(1) for every backing store.

**Verified (array version):** empty at construction; push/pop/top round-trip; **pop and top on an
empty stack are refused rather than silently doing the wrong thing**; 100 pushes fit and the 101st
is refused.

---

## 4. Valid Parentheses — LeetCode 20 — **fundamental, code given**

```cpp
bool isValid(string s) {
    stack<char> st;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') st.push(c);
        else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) return false;                  // a closer with nothing open
            char o = st.top(); st.pop();
            if ((c == ')' && o != '(') || (c == ']' && o != '[') || (c == '}' && o != '{'))
                return false;                              // wrong TYPE of opener
        }
        // anything else is ignored
    }
    return st.empty();                                     // nothing may be left open
}
```

**Key insight — why a stack and not a counter.** With one bracket type, an `int` is enough
(`../09_Strings` §10–12). With three types you must know *which* opener is outstanding, and the
answer is always the **most recent** one — which is precisely LIFO. `([)]` is the string that
proves counters fail: the counts balance, the nesting does not.

**The `else if` matters.** Your version uses a bare `else`, so every non-`(` character — letters,
digits, `[` — is treated as a closing bracket.

**Three separate failure modes**, all needed: closer on an empty stack, mismatched type, and a
non-empty stack at the end.

**Complexity.** O(n) time, O(n) space.

**Verified:** `"()"`, `"()[]{}"`, `"{[]}"`, `""` and `"a(b)c"` all true; `"(]"`, `"([)]"`, `"]"`
and `"(("` all false.

---

## 5. Min Stack — LeetCode 155 — **trick, code given**

```cpp
class MinStack {
    stack<int> st, mins;
public:
    void push(int x) {
        st.push(x);
        if (mins.empty() || x <= mins.top()) mins.push(x);   // <= , not <
    }
    void pop() {
        if (st.empty()) return;
        if (st.top() == mins.top()) mins.pop();
        st.pop();
    }
    int top()    { return st.top(); }
    int getMin() { return mins.top(); }
};
```

**Key insight — the `<=` is the whole problem.** Push `1`, then `1`. With `<`, the second `1` is
not recorded. Pop once: `st.top() == mins.top()`, so the single recorded `1` is removed — and now
`getMin()` reports whatever was below, even though a `1` is still on the stack.

With `<=`, duplicate minima are stored once *per occurrence*, so each pop removes exactly one.

**The O(1)-space follow-up** (worth naming): store `2*x - min` instead of `x` when a new minimum
arrives, and recover the previous minimum on pop. It needs `long long` to avoid overflow and is
the answer when the interviewer asks for constant extra space.

**Complexity.** All operations O(1); O(n) extra space.

**Verified:** the LeetCode example (`-2, 0, -3` → `getMin() = -3`, pop, `top() = 0`,
`getMin() = -2`), and the duplicate-minima case that `<` gets wrong.

---

## 6–9. The monotonic stack — **fundamental, code given once**

```cpp
vector<int> nextGreater(vector<int>& a) {
    int n = (int)a.size();
    vector<int> res(n, -1);
    stack<int> st;                                            // INDICES
    for (int i = n - 1; i >= 0; --i) {                        // right to left
        while (!st.empty() && a[st.top()] <= a[i]) st.pop();  // <= : we want STRICTLY greater
        if (!st.empty()) res[i] = a[st.top()];
        st.push(i);
    }
    return res;
}
```

**Write this once and derive the other three** from the table in `readme.md` §3 — change the scan
direction and flip the comparison. Doing all four in one sitting is the point.

**Key insight — three parts.**

1. **Amortised O(n).** Each index is pushed once and popped at most once, so the inner `while`
   runs at most n times *in total*. The nested loop is not O(n²).
2. **`<=` not `<`.** Equal elements must be popped, or an equal value gets reported as "greater".
   Measured on `{2,2,3}`: with `<`, the first `2` reports `2` as its next greater.
3. **Store indices.** #8 and #9 want a *distance*, which only an index gives you.

### 7. Next Greater Element II — circular

Walk `2n` steps and index with `i % n`; only record answers on the first pass. Two laps is enough
because any element's answer is at most one full circle away.

### 8. Daily Temperatures — LeetCode 739

Same template, forward, popping when the current day is **warmer**, and the answer is the index
difference:

```
for i in 0 .. n-1:
    while stack not empty and a[st.top()] < a[i]:
        res[st.top()] = i - st.top()          # answer the POPPED day, not the current one
        st.pop()
    st.push(i)
```

**Key insight.** Here you fill in the answer for the element being *popped*, not the one being
pushed — the current day is the answer to everything it evicts. That inversion is worth noticing;
it is the same shape as #24's contribution counting.

**Verified:** `{73,74,75,71,69,72,76,73}` → `{1,1,4,2,1,1,0,0}`.

### 9. Online Stock Span — LeetCode 901

Previous-greater looking backwards, with the span as a distance:

```
while stack not empty and a[st.top()] <= a[i]:  st.pop()
span[i] = st.empty() ? (i + 1) : (i - st.top())
st.push(i)
```

**The `st.empty()` branch must set `span[i] = i + 1`** — everything before `i` is smaller, so the
span reaches the start of the array. `stockSpan.cpp` pushes a sentinel `-1` instead and leaves
`span[i]` unwritten; see the bugs section.

**Verified:** `{100,80,60,70,60,75,85}` → `{1,1,1,2,1,4,6}`; the ascending arrays `{10,20,30}` and
`{1,2,3,4}` (which exercise the empty-stack branch) → `{1,2,3}` and `{1,2,3,4}`.

**Complexity.** O(n) time, O(n) space, for all four.

---

## 10. Remove Consecutive Duplicates

**Approach.** No stack needed — the result string *is* the stack, and `res.back()` is its top.

```
res = ""
for c in s:
    if res is empty or res.back() != c:  res += c
```

**Key insight.** Building the answer in a `string` and comparing against `back()` avoids the
push-then-pop-then-reverse dance. Your version pushes to a `stack<char>`, pops everything into a
string, then reverses it — three passes where one does. Same complexity, more places to go wrong.

**Complexity.** O(n) time, O(n) output.

**Verified:** `"aaabbcaa"→"abca"`, `""→""`.

---

# Section 2 — Important

## 11. Previous Smaller Element

The fourth variant: scan left to right, pop while `a[st.top()] >= a[i]`.

---

## 12. Queue using Stacks — LeetCode 232 — **`[impl]`, code given**

```cpp
class MyQueue {
    stack<int> in, out;
    void shift() {                                  // only when `out` runs dry
        if (out.empty())
            while (!in.empty()) { out.push(in.top()); in.pop(); }
    }
public:
    void push(int x) { in.push(x); }
    int  pop()  { shift(); int v = out.top(); out.pop(); return v; }
    int  peek() { shift(); return out.top(); }
    bool empty(){ return in.empty() && out.empty(); }
};
```

**Key insight — the amortised argument, which is the actual interview question.** A single `pop`
can cost O(n) when it triggers a full transfer. But **each element moves from `in` to `out` at
most once in its lifetime**, so across n operations the total transfer work is O(n) and the
amortised cost per operation is **O(1)**.

**The `if (out.empty())` guard is essential.** Transferring while `out` still holds elements would
put newer items in front of older ones and break FIFO.

**Complexity.** Amortised O(1) per operation, O(n) space.

**Verified:** interleaved pushes and pops — push 1, push 2, peek → 1, pop → 1, push 3, pop → 2,
pop → 3, empty → true.

---

## 13. Stack using Queues — LeetCode 225

**Approach — one queue, expensive push:**

```
push(x):  q.push(x)
          rotate the queue size-1 times      # bring x to the FRONT
pop():    q.pop()                            # now trivially the newest
```

**Key insight.** A queue hands you the *oldest* element, so make the newest one oldest at push
time by rotating. Push becomes O(n), pop and top O(1). The two-queue version is symmetric.

Unlike #12 there is **no amortisation** — every push really is O(n). That asymmetry is the
comparison worth making.

**Verified:** push 1, push 2, top → 2, pop → 2, pop → 1, empty → true.

---

## 14–16. Recursion on a stack — **code given (they are `../10_Recursion` applied)**

```cpp
void pushAtBottom(stack<int>& st, int val) {
    if (st.empty()) { st.push(val); return; }
    int x = st.top(); st.pop();
    pushAtBottom(st, val);          // dig to the bottom
    st.push(x);                     // rebuild on the way up
}
void reverseStack(stack<int>& st) {
    if (st.size() <= 1) return;     // <= 1, NOT == 1
    int x = st.top(); st.pop();
    reverseStack(st);
    pushAtBottom(st, x);            // the old top belongs at the bottom
}
void sortStack(stack<int>& st) {
    if (st.size() <= 1) return;
    int x = st.top(); st.pop();
    sortStack(st);
    insertSorted(st, x);            // same shape, different helper
}
```

**Key insight — `<= 1`, not `== 1`.** An empty stack never equals 1, so with `==` the recursion
pops from an empty stack. **Measured: `reverse_rec` on an empty stack gives an ACCESS_VIOLATION.**
This is the same barrier-versus-exact-hit rule as `../10_Recursion` §Q2.

The pop-recurse-push shape is exactly recursive insertion sort from `../10_Recursion` §3 #28 —
`reverseStack` and `sortStack` differ only in the helper they call on the way back up.

**Complexity.** `pushAtBottom` O(n); `reverseStack` and `sortStack` O(n²) time, O(n) stack.

**Verified:** reversing `{10,20,30}` (top-first `30,20,10`) gives top-first `10,20,30`; the empty
and single-element cases return cleanly; `sortStack` on `{3,1,4,2}` leaves the largest on top.

---

## 17–19. Collapsing adjacent elements

**#17 Remove All Adjacent Duplicates (1047).** The output string is the stack:
`if (!res.empty() && res.back() == c) res.pop_back(); else res += c;`

**#18 Remove All Adjacent Duplicates II (1209)** — the generalisation worth learning:

```
stack of (char, count)
for c in s:
    if stack not empty and top.char == c:
        top.count++
        if top.count == k:  pop
    else:
        push (c, 1)
```

**Key insight.** Pushing a **pair** instead of a character turns "remove k in a row" into the same
one-pass algorithm. Storing k copies individually also works but re-scans; the counter does not.

**#19 Backspace String Compare (844).** Build both strings with a stack and compare. The O(1)-space
follow-up walks both from the right, skipping characters as backspaces accumulate.

---

## 20. Asteroid Collision — LeetCode 735

```
for each asteroid a:
    alive = true
    while alive and stack not empty and stack.top() > 0 and a < 0:      # only these collide
        if stack.top() < -a:      stack.pop();  continue                # top destroyed, keep going
        if stack.top() == -a:     stack.pop()                           # both destroyed
        alive = false                                                   # a is destroyed (or tied)
    if alive: push a
```

**Key insight.** A collision happens only when the stack top moves **right** (`> 0`) and the new
asteroid moves **left** (`< 0`). Everything else coexists. The `while` is needed because one large
left-mover can destroy several stacked right-movers in sequence.

**Complexity.** O(n) time, O(n) space.

---

# Section 3 — Good to Know

## 21. Largest Rectangle in Histogram — LeetCode 84 — **hard, code given**

**Your `largestHistogram.cpp` is a 0-byte file** and this problem is on your readme list, so this
is the gap to fill first.

```cpp
long long largestRectangleArea(vector<int> h) {
    h.push_back(0);                                        // sentinel: flushes the stack
    stack<int> st;                                         // indices, heights INCREASING
    long long best = 0;
    for (int i = 0; i < (int)h.size(); ++i) {
        while (!st.empty() && h[st.top()] >= h[i]) {
            int height = h[st.top()]; st.pop();
            int left = st.empty() ? -1 : st.top();         // first shorter bar to the LEFT
            long long width = i - left - 1;                // (i is the first shorter bar RIGHT)
            best = max(best, (long long)height * width);
        }
        st.push(i);
    }
    return best;
}
```

**Key insight — both boundaries come free from one stack.** For a bar of height `h`, the widest
rectangle of that height runs until the first strictly shorter bar on each side. When you pop a
bar:

- the **right** boundary is `i` — that is *why* it is being popped;
- the **left** boundary is whatever is now on top of the stack, because everything between them
  was already popped and so was taller.

Hence `width = i - left - 1`. Getting that `-1` right is most of the problem.

**The `0` sentinel** is what forces every remaining bar to be popped and measured at the end,
removing a duplicated flush loop after the main one.

**Complexity.** O(n) time, O(n) space.

**Verified:** `{2,1,5,6,2,3}→10` (the classic), `{2,4}→4`, `{5}→5`, `{1,1,1,1}→4`,
`{6,5,4,3,2,1}→12`, `{1,2,3,4,5}→9`.

---

## 22. Maximal Rectangle — LeetCode 85

**Approach.** Treat each row as the base of a histogram: `h[c]` is the count of consecutive 1s
ending at this row in column `c`. Run #21 on every row.

```
h[c] = 0 for all c
for each row r:
    for each column c:  h[c] = m[r][c] ? h[c] + 1 : 0        # reset on a 0
    best = max(best, largestRectangleArea(h))
```

**Key insight.** The reduction *is* the solution — there is no new algorithm, only the observation
that a rectangle in a binary matrix is a histogram rectangle whose base sits on some row. Do #21
first and this is a ten-minute problem.

**Complexity.** O(R·C) time, O(C) space.

**Verified:** the standard 4×5 LeetCode matrix → **6**.

---

## 23. Trapping Rain Water — LeetCode 42 — **hard, code given**

```cpp
long long trapStack(vector<int>& h) {
    stack<int> st; long long total = 0;
    for (int i = 0; i < (int)h.size(); ++i) {
        while (!st.empty() && h[st.top()] < h[i]) {
            int bottom = h[st.top()]; st.pop();
            if (st.empty()) break;                       // no left wall -> water escapes
            int width   = i - st.top() - 1;
            int bounded = min(h[st.top()], h[i]) - bottom;
            total += (long long)width * bounded;
        }
        st.push(i);
    }
    return total;
}
```

**Key insight — the stack version fills water in horizontal layers**, not columns. When a bar
taller than the top arrives, the popped bar becomes the *floor* of a puddle whose walls are the
new bar and whatever is left on the stack. The `if (st.empty()) break;` is the "no left wall"
case — water runs off the edge.

**Three solutions, worth comparing deliberately:**

| Method | Time | Space |
|---|---|---|
| Prefix-max + suffix-max arrays | O(n) | **O(n)** |
| Two pointers | O(n) | **O(1)** ← best |
| Monotonic stack (above) | O(n) | O(n) |

Two pointers wins on space; the stack version is here because it is the same machinery as #21 and
seeing the connection is the point.

**Complexity.** O(n) time, O(n) space.

**Verified:** `{0,1,0,2,1,0,1,3,2,1,2,1}→6`, `{4,2,0,3,2,5}→9`, `{1,2,3}→0`.

---

## 24. Sum of Subarray Minimums — LeetCode 907

**Approach — contribution counting.** Instead of enumerating subarrays, ask of each element:
*how many subarrays am I the minimum of?* If `left` is the distance to the previous smaller
element and `right` the distance to the next smaller, the answer is `left × right`.

```
for each i:
    contribution = a[i] * (i - prevSmaller[i]) * (nextSmaller[i] - i)
sum all contributions, mod 1e9+7
```

**Key insight — break ties asymmetrically.** Use *previous smaller-or-equal* on one side and
*strictly smaller* on the other. Otherwise a subarray with repeated minima is counted twice, and
that is the entire difficulty of the problem.

**This technique is the most transferable thing in Section 3** — "sum over all subarrays of some
extremum" is always contribution counting with two monotonic stacks.

**Complexity.** O(n) time, O(n) space.

---

## 25. Remove K Digits — LeetCode 402

```
for each digit d:
    while k > 0 and stack not empty and stack.top() > d:  pop;  k--
    push d
while k > 0:  pop;  k--            # still have removals left -> drop from the END
strip leading zeros;  return "0" if empty
```

**Key insight.** Greedy: removing a digit that is larger than its successor always lowers the
number, and doing it as far left as possible helps most. The two trailing steps — spending leftover
`k` from the back (for an ascending input) and stripping leading zeros — are where the test cases
live.

---

## 26. Longest Valid Parentheses — LeetCode 32

**Approach.** Stack of **indices**, seeded with `-1` as a base marker.

```
st = [-1]
for i, c in s:
    if c == '(':  push i
    else:
        pop
        if st empty:  push i               # this ')' is a new base
        else:         best = max(best, i - st.top())
```

**Key insight.** The stack bottom holds "the index just before the current valid run", so
`i - st.top()` is a length without any counting. Seeding with `-1` makes a run starting at index 0
come out right — the same sentinel idea as `pre[0] = 0` in `../13_prefixSum`.

---

# Section 4 — approach only

- **Baseball Game (682) / Make The String Great (1544)** — direct simulation; the string is the
  stack.
- **Crawler Log Folder (1598)** — a counter is enough. Worth doing to practise *not* reaching for
  a stack.
- **Simplify Path (71)** — split on `/`; `..` pops, `.` and empty segments are skipped.
- **Decode String (394)** — two stacks (repeat counts and partial strings), or one stack of pairs.
- **Score of Parentheses (856)** — push 0 on `(`; on `)` pop and combine as `max(2*v, 1)`.
- **Next Greater Node in Linked List (1019)** — #6 over `../17_linked_list`; convert to a vector or
  push nodes directly.
- **Validate Stack Sequences (946)** — simulate: push from `pushed`, pop whenever the top matches
  the next expected value.
- **Basic Calculator (224/227)** — 224 keeps a stack of (result, sign) across brackets; 227 has no
  brackets so a running stack of terms suffices. Both are `../19` in miniature.
- **Celebrity Problem** — push everyone; repeatedly pop two and eliminate one. O(n), then verify
  the survivor.
- **Two stacks in one array** — grow from both ends toward the middle; full when the tops meet.
- **N stacks in one array** — a free-list of slots plus per-stack top pointers, which is far better
  than the fixed `n/m` partition your `ques.cpp:108` comment describes: fixed partitioning wastes
  space and overflows a stack while others sit empty.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `18_stack/` was edited.**

## Two files do not build

| File | Result |
|---|---|
| `largestHistogram.cpp` | **0 bytes** — empty. `undefined reference to WinMain` |
| `linkedlistimplementation.cpp` | **no `main`** — `undefined reference to WinMain` |

`largestHistogram.cpp` is LeetCode 84, which is on your own readme list — solution in §3 #21.

## `userDefinedStackArray.cpp:11` and `:19` — `=` instead of `==`

```cpp
void push (int val){
    if(idx=99){                      // assigns 99, evaluates TRUE
        cout<<"stack underflow";
        return;
    }
    ...
}
void pop(){
    if(idx=-1){ ... return; }        // assigns -1, evaluates TRUE
```

Both guards fire unconditionally, so **neither operation ever executes**. Worse, the assignment
corrupts `idx`.

**Verified: after `push(10); push(20);` the stack refuses both and `size()` returns 100** —
because `idx` was set to 99 by the guard itself.

GCC warns:
`warning: suggest parentheses around assignment used as truth value [-Wparentheses]` at both lines.

Three fixes: `==`, `idx == 99` should be a named `CAP - 1`, and the push message should say
**overflow**. `top()` also has no empty check — `arr[-1]` on an empty stack.

## `Vectorimplementation.cpp:16` — the same `=` bug in `pop`

**Verified: after `push(10); push(20); pop();` the size is 0**, not 1 — the guard assigns `idx = -1`
and returns, so the vector still holds two elements while `idx` says empty. The object's two
"sizes" now disagree permanently.

The deeper fix is to **delete `idx` entirely**: `v.size()` and `v.back()` already are the top and
the size, and keeping a parallel counter is what allows them to diverge.

## `ques.cpp:6` — `balancedBrackets` treats every non-`(` character as a closer

```cpp
if(s[i]=='('){ stk.push('('); }
else{ if(stk.size()==0)return false; else stk.pop(); }   // <-- bare else
```

**Verified:**

| Input | Your output | Correct |
|---|---|---|
| `"(())"` | true | true ✓ |
| `"ab"` | **false** | true — no brackets at all |
| `"a(b)c"` | **false** | true |
| `"[]"` | **false** | true |

The `if (s.length() % 2 != 0) return false;` shortcut is also only valid for a string of pure
brackets — `"a(b)"` has odd length and is balanced. Corrected version in §1 #4.

## `ques.cpp:39` and `:52` — `nxtgrtest` / `prevgrtest` mishandle equal values

```cpp
while( help.size() && help.top() < arr[i] ) help.pop();     // should be <=
```

With `<`, an equal element survives the pop and is then reported as the "next greater" — but equal
is not greater.

**Verified:**

| Input | Your output | Correct |
|---|---|---|
| `{2,2,3}` | `2,3,-1` | `3,3,-1` |
| `{3,3,3}` | `3,3,-1` | `-1,-1,-1` |
| `{3,2,3}` (prev) | `-1,3,3` | `-1,3,-1` |
| `{4,5,2,25}` | `5,25,25,-1` | ✓ correct — no duplicates |

Both also push **values** rather than indices, which works for this question but blocks the
distance-based variants (#8, #9, #21).

## `ques.cpp:92` — `isPalindrome` has two fatal bugs

```cpp
while(s[i]){
    if(s[i]=='X')a=1;
    else if(a=0) st.push(s[i]);      // ASSIGNMENT: a=0 is always false
    else{ if(s[i]==st.top()) ... }   // so we always land here, on an EMPTY stack
}                                    // and `i` is NEVER incremented
```

1. **`a=0` should be `a==0`.** The assignment evaluates to 0, so the push branch is unreachable and
   nothing is ever pushed.
2. **`i` is never incremented**, so the `while` cannot terminate.

In practice the first bug masks the second: the very first character falls through to
`st.top()` on an empty stack, which is undefined behaviour. **Verified: with an added empty-stack
guard it returns `false` for `"abXba"`, which is a palindrome.** Without that guard it is UB before
it can hang.

GCC flags the first: `ques.cpp:98:12: warning: suggest parentheses around assignment used as truth
value`.

## `ques.cpp:65` — `pushHelper` pushes to the min-stack twice

```cpp
//O(n) space complexity
if(m.empty() || m.top()>n){ m.push(n); } else m.push(m.top());
//O(1) space complexity
if(m.empty() || m.top()>=n){ m.push(n); };
```

These are two **alternative** designs, and both run. Every call pushes two entries onto `m` while
pushing one onto `o`, so the two stacks desynchronise immediately and `get_min` is meaningless.
Keep one — the first (which mirrors every push) or the second (which needs matching pop logic),
never both. Correct version in §1 #5.

The function also hard-codes `for(i=1;i<=5;i++)` and reads from `cin`, so it cannot be tested or
reused. `get_min()` returns after reading exactly five numbers from standard input.

## `stockSpan.cpp` — uninitialised read and an out-of-bounds index

```cpp
if(st.size()==0) st.push(-1);      // pushes -1 as an INDEX; pgi[i] never assigned
else pgi[i]=st.top();
pgi[i]=i-pgi[i];                   // reads pgi[i] on the empty-stack path
```

Two faults on adjacent lines:

1. When the stack empties, `pgi[i]` is **never written** before being read on the next line.
2. Pushing `-1` as an index means a later `arr[st.top()]` evaluates `arr[-1]` — out of bounds.

**Verified** with the array pre-filled with a `-12345` sentinel so uninitialised reads are visible:

| Input | Your output | Correct |
|---|---|---|
| `{100,80,60,70,60,75,85}` | `1,1,1,2,1,4,6` ✓ | `1,1,1,2,1,4,6` |
| `{10,20,30}` | `1,`**`12346`**`,3` | `1,2,3` |
| `{1,2,3,4}` | `1,`**`12346`**`,3,4` | `1,2,3,4` |

`12346` is `1 - (-12345)` — the sentinel leaking through. Your own descending test array never
empties the stack, which is exactly why the bug survived. **Ascending input is the case to test.**

The program also never prints `pgi`, and `int pgi[n]` is a variable-length array (a GCC extension,
not standard C++).

## `rev_stack.cpp:38` — `reverse_rec` crashes on an empty stack

```cpp
if(st.size()==1){ return; }        // 0 never equals 1
```

**Verified: ACCESS_VIOLATION (`0xC0000005`) on an empty stack.** `st.top()` is called on it.
Fix: `if (st.size() <= 1) return;`.

`displayRev_rec` and `display_rec` both take `stack<int> st` **by value**, so the `st.push(x)` at
the end of each restores a copy that is immediately discarded. Harmless, but it suggests the intent
was to preserve the caller's stack — which requires `stack<int>&`.

---

## What you got right

- **`ques.cpp:135` `calculateSpan_using_stack` is correct** — verified against the brute-force
  version on your own array. It stores **indices**, pops with `<=`, and handles the empty-stack
  case as `i + 1`. That is the textbook monotonic stack, and it is the version `stockSpan.cpp`
  should have been. Your comments on it ("Stores INDICES, not values", "We use a WHILE loop because
  we might need to pop multiple") name the two things that matter.

- **`ques.cpp:111` `calculateSpan` (brute force) is also correct**, and having both side by side
  with the same signature is exactly how to check an optimisation. Keep that habit.

- **`basic.cpp`'s `reverse()` is correct** — verified, `{10,20,30}` comes back reversed. Popping
  into a `vector` and pushing back in the same order does reverse a stack, and it is O(n) rather
  than the three-stack version you left commented out above it. Choosing the simpler one was right.

- **`basic.cpp`'s `pushAtBottom` and `pushAtIdx` are both correct**, and `pushAtIdx` generalises
  the other cleanly (`while (st.size() > idx)` — bottom is just `idx = 0`).

- **`rev_stack.cpp`'s `reverse_rec` works on non-empty stacks** — verified, `{10,20,30}` reverses
  correctly. `pushAtBottom_rec` is right, and using it as the helper is the correct decomposition;
  only the base case is too narrow.

- **`main` in `basic.cpp` restores the stack after printing it** (drains into `temp`, then pushes
  back). That is a deliberate, correct touch — printing a stack destroys it otherwise.

- **The comment at `ques.cpp:108`** — "m stack using an array: divide array in m parts" — is a real
  technique and the right thing to have noted. See §4 for why the free-list variant is better.
