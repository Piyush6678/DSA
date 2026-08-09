# 14 — Sliding Window

A sliding window is two pointers that only ever move **forward**. That single property is what
makes it O(n): each index enters the window once and leaves once, so even though the inner
`while` looks like a nested loop, the total work is 2n.

It is the natural successor to `../13_prefixSum`. Prefix sums answer "what is the sum of this
range?" for arbitrary ranges; sliding windows answer "what is the best range?" — but only when
the window can be *shrunk* safely, which is a real precondition, not a formality.

---

## 1. The precondition nobody states

> A sliding window is valid only when **extending the window moves the quantity one way and
> shrinking it moves it the other.**

For sums, that means **all values must be non-negative**. With a negative in the array,
shrinking from the left can *increase* the sum, so "the window is too big, shrink it" is not
sound and the algorithm silently returns the wrong answer.

This is exactly the boundary with the previous folder:

| Situation | Tool |
|---|---|
| Subarray sum, **all values ≥ 0** | sliding window, O(1) space |
| Subarray sum, **negatives present** | prefix sum + hash map, `../13_prefixSum` §6 |
| Longest/shortest window under a *countable* constraint | sliding window |
| Min or max over the window | monotonic deque (§4) |

If an interviewer gives you an array that may contain negatives and you reach for a window,
that is the mistake they were testing for.

---

## 2. Fixed-size window

The window never changes length, so there is no `while` at all — one element in, one out.

```cpp
long long run = 0;
for (int i = 0; i < k; ++i) run += a[i];      // build the first window
long long best = run;
for (int r = k; r < n; ++r) {
    run += a[r] - a[r - k];                   // add entering, drop leaving
    best = max(best, run);
}
```

**The one-line update is the whole idea.** Recomputing each window from scratch is O(n·k); the
add-one/drop-one update makes it O(n).

**If you need the window's *index*, initialise it.** `int idx;` left unset before the loop
holds garbage whenever the first window is already the best — that is the bug in `ques.cpp:6`.

---

## 3. Variable-size window — the template

```
l = 0
for r in 0 .. n-1:
    add a[r] to the window state
    while (window is INVALID):
        remove a[l] from the window state
        l++
    // the window [l, r] is now valid
    record the answer
```

Everything hinges on where you record the answer, and the two variants are mirror images:

| Goal | Loop condition | Record |
|---|---|---|
| **Longest** valid window | `while (INVALID) shrink` | after shrinking — window is valid |
| **Shortest** window that *reaches* a target | `while (VALID) { record; shrink }` | **inside** the loop, before shrinking |

Getting these backwards is the standard error. For "shortest", you must record *while the
window still satisfies the condition*, because the next shrink is what might break it.

**Complexity.** O(n) — `l` and `r` each advance at most n times in total. Say that out loud in
an interview; the nested `while` makes people guess O(n²).

---

## 4. Monotonic deque — max/min over a window

Sum can be maintained incrementally, but **max cannot**: when the maximum leaves the window you
have no idea what the new maximum is without rescanning.

The fix is a deque of **indices** whose values are kept decreasing:

```
for r in 0 .. n-1:
    if front index is <= r - k:  pop_front        # expired
    while back value <= a[r]:    pop_back         # dominated -- can never win again
    push_back r
    if r >= k-1:  answer is a[front]
```

**The insight is "dominated".** If `a[r]` is bigger than something already in the deque *and*
arrives later, that older element can never be the maximum of any future window — it is beaten
now and expires sooner. Discard it permanently.

Every index is pushed once and popped once, so despite the inner `while` this is **O(n)**, not
O(n·k).

---

## 5. The `atMost` trick

Many problems ask for *exactly* k of something. A window handles "at most k" naturally, because
the constraint is monotone; "exactly k" is not, because shrinking below k breaks it. So:

```
exactly(k) = atMost(k) - atMost(k-1)
```

You have already used this in `../13_prefixSum` §17 for LeetCode 930. It converts an awkward
problem into two calls to an easy one, and it costs O(1) space where the hash-map alternative
costs O(n).

**`res += (r - l + 1)`** inside an `atMost` window counts every valid subarray *ending* at `r`
in one statement — worth internalising separately.

---

## Interview Q&A

**Q1. Why is a sliding window O(n) when it contains a nested loop?**
Amortised analysis. `l` never decreases and is bounded by n, so across the whole run the inner
`while` executes at most n times *in total*, not n times per outer step. Total work is ≤ 2n.

**Q2. When can you NOT use a sliding window?**
When shrinking the window does not move the quantity monotonically. The classic case is subarray
sums with negative numbers — removing a negative element *increases* the sum, so "too big,
shrink" is invalid. Use prefix sums with a hash map instead.

**Q3. Fixed vs. variable window — how do you tell which you need?**
If the problem states the window length (`of size k`), it is fixed and there is no `while` loop.
If it asks for the longest or shortest window satisfying a property, it is variable and the
`while` does the shrinking.

**Q4. How do you get the maximum of every window of size k in O(n)?**
A monotonic deque holding indices with decreasing values. Pop the front when it expires, pop the
back while the incoming element is greater or equal, then push. The front is always the current
maximum. Each index enters and leaves once → O(n), O(k) space. A heap gives O(n log k) and is
the acceptable second answer.

**Q5. Why store indices in the deque rather than values?**
Because you must know *when* an element expires, and only the index tells you that. With values
alone you cannot distinguish the copy that just left the window from an equal one still inside.

**Q6. What is the window "state" and why does it matter?**
Whatever you maintain incrementally — a running sum, a frequency array, a distinct-element count.
The rule is that adding and removing one element must both be O(1); if removal is not O(1), the
window is not O(n). That is why "count of distinct characters" is tracked with a map plus a
counter rather than recomputed.

---

## Your original notes (preserved)

The problem list from this file is kept verbatim. **Both are covered** — see `questions.md`.

```
1052 Grumpy Bookstore Owner 
209 Minimum Size Subarray Sum 
```

| Your entry | Now at |
|---|---|
| 209 Minimum Size Subarray Sum | `questions.md` §1 #4 — the shortest-window template |
| 1052 Grumpy Bookstore Owner | `questions.md` §2 #12 — fixed window over the *gain*, not the total |
