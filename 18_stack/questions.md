# 18 — Stack: Practice Questions

> **Why 26 ranked problems.** A stack has four O(1) operations and no algorithmic depth of its
> own, so the count is driven entirely by what people *build* with it. Three families, and they
> do not substitute for each other: **implementation** (three backing stores, plus the design
> problems — min stack, queue-from-stacks — which are asked by name), **matching/parsing** (the
> bracket family, where the stack holds "what is still open"), and above all the **monotonic
> stack**, which is one loop that solves a dozen problems and needs roughly ten reps before you
> recognise the shape unprompted. The histogram problem alone justifies four of them, because
> three other Hard problems reduce to it.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search the
quoted title.

**Scope note.** Everything needs only `01`–`17`. Entries marked **`[impl]`** are hand-rolled data
structures — write them, do not use `std::stack`.

**Both problems from your `readme.md` are covered** — balanced brackets at §1 #4, largest
rectangle in histogram at §3 #21.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Implement a stack with an array | Easy | GFG `[impl]` | *"Implement Stack using Array"* — `userDefinedStackArray.cpp` |
| 2 | Implement a stack with a vector | Easy | GFG `[impl]` | *"Implement Stack using Array"* — `Vectorimplementation.cpp` |
| 3 | Implement a stack with a linked list | Easy | GFG `[impl]` | *"Implement Stack using Linked List"* — `linkedlistimplementation.cpp` |
| 4 | Valid Parentheses | Easy | **LeetCode 20** | `valid-parentheses` — **on your list**; `ques.cpp:6` |
| 5 | Min Stack | **Medium** | **LeetCode 155** | `min-stack` — the classic design question |
| 6 | Next Greater Element I | Easy | **LeetCode 496** | `next-greater-element-i` — the monotonic template |
| 7 | Next Greater Element II (circular) | **Medium** | **LeetCode 503** | `next-greater-element-ii` — index `% n` twice round |
| 8 | Daily Temperatures | **Medium** | **LeetCode 739** | `daily-temperatures` — indices, not values |
| 9 | Online Stock Span | **Medium** | **LeetCode 901** | `online-stock-span` — `stockSpan.cpp`, `ques.cpp:135` |
| 10 | Remove Consecutive Duplicates | Easy | GFG | *"Remove consecutive duplicates"* — `ques.cpp:24` |

**Why these ten.** #1–#3 are the implementations, and they are where the `=` vs `==` and
overflow/underflow discipline gets fixed — all three of your files have that bug. #4 is the
bracket problem in its real form: **three** bracket types, so a counter is not enough and you
genuinely need a stack. Your version handles only `(`, which is why it is Section 1 rather than a
warm-up.

**#6–#9 are the monotonic stack, four times.** Do them consecutively. #6 is the bare template;
#7 adds the circular wrap (walk `2n` steps, index with `% n`); #8 forces you to store **indices**
because the answer is a distance; #9 is the same as #8 looking backwards, and it is worth noticing
that yours already has a working stack version next to a brute-force one. #5 is the design
question asked most often, and the `<=` in its push condition is the whole trick.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 11 | Previous Smaller Element | Easy | GFG | *"Smaller on Left"* — the fourth monotonic variant |
| 12 | Implement Queue using Stacks | Easy | **LeetCode 232** | `implement-queue-using-stacks` `[impl]` — amortised O(1) |
| 13 | Implement Stack using Queues | Easy | **LeetCode 225** | `implement-stack-using-queues` `[impl]` |
| 14 | Reverse a stack recursively | **Medium** | GFG | *"Reverse a Stack"* — `rev_stack.cpp` |
| 15 | Sort a stack recursively | **Medium** | GFG | *"Sort a stack"* — cf. `../10_Recursion` §3 #28 |
| 16 | Insert an element at the bottom of a stack | Easy | GFG | *"Insert an Element at the Bottom"* — `basic.cpp:5` |
| 17 | Remove All Adjacent Duplicates In String | Easy | **LeetCode 1047** | `remove-all-adjacent-duplicates-in-string` |
| 18 | Remove All Adjacent Duplicates II (k copies) | **Medium** | **LeetCode 1209** | `remove-all-adjacent-duplicates-in-string-ii` — stack of (char, count) |
| 19 | Backspace String Compare | Easy | **LeetCode 844** | `backspace-string-compare` |
| 20 | Asteroid Collision | **Medium** | **LeetCode 735** | `asteroid-collision` — a stack simulation |

**Why these ten.** #11 completes the four-variant table in `readme.md` §3 — write all four in one
sitting and they stop being four separate things. **#12 is the more instructive of the two
conversions**: the amortised-O(1) argument (each element crosses between the stacks at most once,
so a single O(n) `pop` is paid for by n cheap pushes) is a real complexity insight, not a puzzle.

#14–#16 are the recursion-on-a-stack trio and connect straight back to `../10_Recursion` — #16 is
the helper both of the others need, so write it first. #17–#19 are the "collapse adjacent things"
family, where the stack holds what has survived so far; **#18 generalises it by pushing pairs**,
which is the step that makes the pattern reusable. #20 is the best pure *simulation* problem here
— the rules are given, and the whole difficulty is deciding when to push, when to pop, and when to
do neither.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 21 | Largest Rectangle in Histogram | **Hard** | **LeetCode 84** | `largest-rectangle-in-histogram` — **on your list**; `largestHistogram.cpp` is empty |
| 22 | Maximal Rectangle | **Hard** | **LeetCode 85** | `maximal-rectangle` — #21 once per row |
| 23 | Trapping Rain Water | **Hard** | **LeetCode 42** | `trapping-rain-water` — also solvable two-pointer |
| 24 | Sum of Subarray Minimums | **Medium** | **LeetCode 907** | `sum-of-subarray-minimums` — contribution counting |
| 25 | Remove K Digits | **Medium** | **LeetCode 402** | `remove-k-digits` — greedy with a monotonic stack |
| 26 | Longest Valid Parentheses | **Hard** | **LeetCode 32** | `longest-valid-parentheses` — stack of indices |

**Why these six.** **#21 is the problem this folder exists for.** Your `largestHistogram.cpp` is a
0-byte file and your readme lists it, so it is the obvious gap. Once you have it, #22 falls out
immediately — build a histogram per row and call #21 — and that reduction is worth as much as the
original.

#23 has three solutions (prefix arrays, two pointers, monotonic stack) and comparing them is the
exercise; the stack version computes water *horizontally* in layers, which is a genuinely
different way to see the problem. **#24 is the most transferable idea in this section**:
"contribution counting" — for each element, count how many subarrays it is the minimum of, using
previous-smaller and next-smaller boundaries. That technique reappears constantly. #25 is greedy
made concrete: pop while the top is bigger and you still have removals left. #26 is the bracket
problem again, but you push *indices* and the answer is a distance between them.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Baseball Game | Easy | **LeetCode 682** | pure stack simulation; a good warm-up |
| Make The String Great | Easy | **LeetCode 1544** | #17 with a case-difference predicate |
| Crawler Log Folder | Easy | **LeetCode 1598** | a counter suffices — a good "do I need a stack?" test |
| Simplify Path | **Medium** | **LeetCode 71** | split on `/`, stack of directory names |
| Decode String | **Medium** | **LeetCode 394** | two stacks: counts and partial strings |
| Score of Parentheses | **Medium** | **LeetCode 856** | stack of running scores |
| Minimum Remove to Make Valid Parentheses | **Medium** | **LeetCode 1249** | mark then filter |
| Next Greater Node in Linked List | **Medium** | **LeetCode 1019** | #6 on `../17_linked_list` |
| Validate Stack Sequences | **Medium** | **LeetCode 946** | simulate and check |
| Basic Calculator | **Hard** | **LeetCode 224** | stack of signs; cf. `../19_infix prefix postfix` |
| Basic Calculator II | **Medium** | **LeetCode 227** | precedence without brackets |
| Celebrity Problem | **Medium** | GFG | *"The Celebrity Problem"* — stack elimination, O(n) |
| Design a stack with `getMax` in O(1) | **Medium** | *Drill* | #5 with the comparison flipped |
| Two stacks in one array | **Medium** | GFG | *"Implement two stacks in an array"* — grow from both ends |
| N stacks in one array | **Hard** | GFG | *"Implement N stacks in an Array"* — `ques.cpp:108` note |
| Check if a string is a palindrome using a stack | Easy | *Drill* | `ques.cpp:92` — the `X`-marks-the-middle version |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8   [ ] 9   [ ] 10
Section 2   [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15  [ ] 16  [ ] 17  [ ] 18  [ ] 19  [ ] 20
Section 3   [ ] 21  [ ] 22  [ ] 23  [ ] 24  [ ] 25  [ ] 26
Section 4   [ ] ______ / 16
```
