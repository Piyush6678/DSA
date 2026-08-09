# 20 — Queue: Practice Questions

> **Why 18 ranked problems.** A queue has fewer native problems than a stack — there is no
> "monotonic queue" family to mine — so the count reflects three narrower groups: the
> **implementations** (circular array, linked list, deque, and the two conversion designs, which
> are asked by name and where the complexity *argument* is the answer), the **simulation**
> problems where the rules describe a queue literally, and the **deque** problems that bridge into
> `../14_Sliding window` and `../27_graphs`. Eighteen covers all three without padding; a queue is
> mostly a means to an end, and that end is BFS two folders from now.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search the
quoted title.

**Scope note.** Everything needs only `01`–`19`. Entries marked **`[impl]`** are hand-rolled — do
not use `std::queue`. **`[fwd]`** marks problems whose natural home is a later folder.

**All five problems from your `readme.md` are covered** — 622 at §1 #3, 232 at §1 #4, 1700 at
§2 #9, 649 at §2 #11, 239 at §3 #14.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Implement a queue with an array (circular) | **Medium** | GFG `[impl]` | *"Implement Queue using array"* — `arrayImplementation.cpp` |
| 2 | Implement a queue with a linked list | Easy | GFG `[impl]` | *"Implement Queue using Linked List"* — `linkedListIMplementation.cpp` |
| 3 | Design Circular Queue | **Medium** | **LeetCode 622** | `design-circular-queue` — **on your list** |
| 4 | Implement Queue using Stacks | Easy | **LeetCode 232** | `implement-queue-using-stacks` `[impl]` — **on your list** |
| 5 | Implement Stack using Queues | Easy | **LeetCode 225** | `implement-stack-using-queues` `[impl]` |
| 6 | Reverse a queue | Easy | GFG | *"Queue Reversal"* — pour into a stack and back |
| 7 | Reverse the first K elements of a queue | **Medium** | GFG | *"Reverse First K elements of Queue"* — `ques.cpp:8` |

**Why these seven.** #1 is the one that matters most: writing it correctly means understanding
**why the modulo is there**, and your current version shifts every element on `pop` (O(n)) *and*
does not compile. #3 is the same thing with a judge attached, so do #1 first and #3 becomes a
transcription.

#2 is short but has the trap that `back` must be reset to `NULL` when the queue empties — the same
dangling-tail bug as `../17_linked_list`.

**#4 and #5 are the design pair, and they should be done together** because the interesting result
is the *asymmetry*: a queue from two stacks is amortised O(1), while a stack from a queue is
genuinely O(n) per push with no amortisation to rescue it. Being able to explain why is the
question. #6 and #7 are the "queue plus auxiliary stack" pattern; #7 is where the loop bounds
matter (`k` pushes back, then `n-k` rotations) and yours is off by one.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 8 | Design Circular Deque | **Medium** | **LeetCode 641** | `design-circular-deque` — `deque.cpp` |
| 9 | Number of Students Unable to Eat Lunch | Easy | **LeetCode 1700** | `number-of-students-unable-to-eat-lunch` — **on your list** |
| 10 | Time Needed to Buy Tickets | Easy | **LeetCode 2073** | `time-needed-to-buy-tickets` — simulation, or O(n) directly |
| 11 | Dota2 Senate | **Medium** | **LeetCode 649** | `dota2-senate` — **on your list**; two queues |
| 12 | First negative in every window of size K | Easy | GFG | *"First negative integer in every window of size k"* — `ques.cpp:28` |
| 13 | First non-repeating character in a stream | **Medium** | GFG | *"First non-repeating character in a stream"* — queue + frequency array |

**Why these six.** #8 completes the implementation set and is where you need `push_front` — which
in a circular buffer is `front = (front - 1 + CAP) % CAP`, **not** a shift loop. The `+ CAP` is
mandatory because C++ `%` on a negative gives a negative.

**#9 is the best problem in this section** because the queue is a trap: the naive simulation
rotates students indefinitely and you must detect the deadlock. The insight is that **order does
not matter** — only how many students want each sandwich type — so a two-element count array
solves it in O(n) with no queue at all. Recognising when the described data structure is not the
one to use is a real skill. #10 is the same lesson in miniature.

**#11 genuinely needs two queues** and is the cleanest use of the structure here: store indices,
compare fronts, and re-queue the winner at `index + n` to represent the next round. #12 you have
already met in `../14_Sliding window` §1 #3; the queue version is the natural one. #13 is the
canonical "stream" problem — a queue of candidates plus a frequency array, popping from the front
while the front has been seen more than once.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 14 | Sliding Window Maximum | **Hard** | **LeetCode 239** | `sliding-window-maximum` — **on your list**; monotonic **deque** |
| 15 | Rotting Oranges | **Medium** | **LeetCode 994** | `rotting-oranges` `[fwd]` — multi-source BFS |
| 16 | Number of Islands | **Medium** | **LeetCode 200** | `number-of-islands` `[fwd]` — BFS on a grid |
| 17 | Design Hit Counter | **Medium** | **LeetCode 362** | `design-hit-counter` — a queue with expiry |
| 18 | Design Front Middle Back Queue | **Medium** | **LeetCode 1670** | `design-front-middle-back-queue` — two deques |

**Why these five.** **#14 is the payoff for learning `deque`** — you need to remove from the front
(expiry) *and* the back (domination) in the same loop, which no other container gives you in O(1).
Full solution in `../14_Sliding window/solution.md` §3 #17.

**#15 and #16 are BFS, and they are why this folder exists.** Both are forward references to
`../27_graphs`, but doing them now — while the queue is fresh — makes graph traversal feel like a
variation rather than a new topic. #15 is *multi-source* BFS (seed the queue with every rotten
orange at once), which is the more instructive of the two and a genuinely useful pattern.

#17 is a queue used as a sliding time window: push timestamps, pop from the front while they are
more than 300 seconds old. #18 is two deques kept balanced around the middle — the same
"maintain an invariant between two containers" idea as the LRU cache in `../16_oops`.

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Implement a queue using a single array | Easy | *Drill* | prove to yourself why the shift version is O(n) |
| Implement `k` queues in one array | **Hard** | GFG | *"Efficiently implement k Queues"* — free-list of slots |
| Interleave the first and second half of a queue | **Medium** | GFG | *"Interleave the first half of the queue"* — half into a stack |
| Generate binary numbers 1..N using a queue | Easy | GFG | *"Generate Binary Numbers"* — push `s+"0"` and `s+"1"` |
| Generate numbers with digits 5 and 6 | Easy | GFG | same shape as above |
| Circular Tour / Gas Station | **Medium** | **LeetCode 134** | queue framing, but O(n) greedy is better |
| Sum of min and max of all subarrays of size K | **Hard** | GFG | *"Sum of minimum and maximum elements"* — two deques |
| Longest Subarray with Absolute Diff ≤ Limit | **Medium** | **LeetCode 1438** | two deques; `../14_Sliding window` §3 #19 |
| Shortest Subarray with Sum at Least K | **Hard** | **LeetCode 862** | monotonic deque over prefix sums |
| Moving Average from Data Stream | Easy | **LeetCode 346** | fixed-size queue with a running sum |
| Design Recent Counter | Easy | **LeetCode 933** | #17 with a 3000 ms window |
| Task Scheduler | **Medium** | **LeetCode 621** | `[fwd]` — greedy + counting; heap version in `../25_heap` |
| Level-order traversal of a tree | **Medium** | **LeetCode 102** | `[fwd]` — `../21_tree`; the queue's other main use |
| Remove elements at even indices | Easy | *Drill* | `basics.cpp:32` — rotate and conditionally re-push |
| Print a queue in reverse | Easy | *Drill* | `basics.cpp:14` — via a stack |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7
Section 2   [ ] 8   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13
Section 3   [ ] 14  [ ] 15  [ ] 16  [ ] 17  [ ] 18
Section 4   [ ] ______ / 15
```
