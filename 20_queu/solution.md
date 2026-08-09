# 20 — Queue: Solutions

Approach, pseudocode, complexity and key insight. Real C++ appears for the **implementations**
(circular buffer, the two conversions) — this is an implementation topic and the modulo
arithmetic is exactly what prose loses.

**Everything shown as code was compiled with `g++ -std=gnu++14` and executed** — part of a
101-assertion run across folders 18–20, all passing. Measured outputs are quoted.

---

# Section 1 — Must Do

## 1 & 3. Circular array queue — **`[impl]`, code given**

```cpp
class CircularQueue {
    static const int CAP = 5;
    int arr[CAP];
    int front, count;                          // head index + element count
public:
    CircularQueue() : front(0), count(0) {}
    bool empty() const { return count == 0; }
    bool full()  const { return count == CAP; }
    int  size()  const { return count; }

    bool push(int v) {
        if (full()) return false;
        arr[(front + count) % CAP] = v;        // rear is DERIVED
        ++count;
        return true;
    }
    bool pop() {
        if (empty()) return false;
        front = (front + 1) % CAP;             // move the head; shift NOTHING
        --count;
        return true;
    }
    bool getFront(int& out) const { if (empty()) return false; out = arr[front]; return true; }
    bool getBack(int& out)  const { if (empty()) return false; out = arr[(front+count-1)%CAP]; return true; }
};
```

**Key insight — three decisions, all load-bearing.**

1. **Wrap with `%`, do not shift.** Shifting on `pop` makes it O(n); advancing `front` without
   wrapping makes the queue report itself full while the front half sits empty. The modulo is what
   buys O(1) *and* reuse.
2. **Store `front` + `count`, not `front` + `rear`.** With two indices, `front == rear` means both
   *empty* and *full* and you have to waste a slot or carry a flag. A count makes both tests
   trivial and lets you derive the rear as `(front + count - 1) % CAP`.
3. **Return a status.** As with `../18_stack` §1, printing "queue is empty" and returning tells the
   caller nothing.

**For `push_front` (the deque, §2 #8):** `front = (front - 1 + CAP) % CAP`. **The `+ CAP` is
mandatory** — C++ `%` on a negative left operand returns a negative, so `(0 - 1) % 5` is `-1`, not
`4`. Same trap as LeetCode 974 in `../13_prefixSum` §9.

**Complexity.** `push`, `pop`, `front`, `back`, `size` — all **O(1)**.

**Verified:** fill to capacity (5), 6th push refused; front `10`, back `50`; **after two pops a new
push reuses the freed slots and the back wraps correctly** (front `30`, back `60`, size 4).

---

## 2. Linked-list queue — **`[impl]`**

```
push(v):  n = new Node(v)
          if size == 0:  front = back = n
          else:          back->next = n;  back = n
          ++size

pop():    if size == 0: return false
          old = front
          front = front->next
          if (front == NULL) back = NULL          # <-- the queue is now empty
          delete old
          --size;  return true
```

**Key insight.** `back` must be reset when the last element leaves, or it dangles at a freed node
and the next `push` writes through it. Identical to the `deleteAtHead` trap in
`../17_linked_list/solution.md` §1.

Push at the **tail**, pop from the **head** — the opposite of a stack, which is why a queue needs a
`back` pointer at all.

**Complexity.** O(1) for every operation, O(n) space with one pointer of overhead per element.

> **The naming collision that stops all three of your files compiling:** a class cannot have a
> member variable `front` *and* a member function `front()`. Rename the data members (`frontIdx`,
> `head`) or the accessors (`getFront()`). Details in the bugs section.

---

## 4. Queue using two stacks — LeetCode 232 — **`[impl]`, code given**

```cpp
class MyQueue {
    stack<int> in, out;
    void shift() {                                     // only when `out` is exhausted
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

**Key insight — the amortised argument is the question.** A single `pop` can cost O(n) when it
triggers a full transfer. But **each element crosses from `in` to `out` exactly once in its
lifetime**, so across n operations the total transfer work is O(n) — **amortised O(1) per
operation**. Saying "O(n) worst case" without the amortised bound is the incomplete answer.

**The `if (out.empty())` guard is essential.** Draining while `out` still holds elements would
stack newer items on top of older ones and break FIFO.

**Complexity.** Amortised O(1); O(n) space.

**Verified:** push 1, push 2, peek → 1, pop → 1, push 3 (interleaved), pop → 2, pop → 3,
empty → true.

---

## 5. Stack using one queue — LeetCode 225 — **`[impl]`**

```
push(x):  q.push(x)
          repeat (q.size() - 1) times:  q.push(q.front()); q.pop()      # rotate x to the front
pop():    v = q.front(); q.pop(); return v
```

**Key insight.** A queue always hands you the *oldest* element, so make the newest one oldest at
push time by rotating everything else behind it. Push becomes O(n); pop and top are O(1).

**Note the asymmetry with #4** — this is the comparison worth making explicitly. Two stacks
simulate a queue at amortised O(1); a queue simulating a stack pays a genuine O(n) on **every**
push, with no amortisation to recover it. Stacks are the more powerful primitive here.

**Complexity.** Push O(n), pop/top O(1), O(n) space.

**Verified:** push 1, push 2, top → 2, pop → 2, pop → 1, empty → true.

---

## 6 & 7. Reversing a queue — **code given for #7**

**#6, whole queue:** pour every element into a stack, then pour it back. The stack reverses the
order for free.

**#7, first K only:**

```cpp
void reverseFirstK(queue<int>& q, int k) {
    int n = (int)q.size();                                    // capture BEFORE mutating
    stack<int> st;
    for (int i = 0; i < k; ++i) { st.push(q.front()); q.pop(); }      // 1. first k -> stack
    while (!st.empty()) { q.push(st.top()); st.pop(); }               // 2. k pushes back
    for (int i = 0; i < n - k; ++i) { q.push(q.front()); q.pop(); }   // 3. n-k rotations
}
```

**Key insight — count the two loops separately.** Step 2 runs exactly `k` times and step 3 exactly
`n - k` times, for `n` total operations after the initial drain. Fusing them into one loop of `n`
iterations with an `if` inside (what `ques.cpp` does) invites an off-by-one, and that is exactly
what happened — its loop runs `n - 1` times, so one element is left un-rotated.

Capture `n` **before** popping, or `q.size()` changes underneath you.

**Complexity.** O(n) time, O(k) space.

**Verified:** `{1,2,3,4,5}` with k=3 → `{3,2,1,4,5}`; `{1,2,3,4}` k=2 → `{2,1,3,4}`; `k == n` →
fully reversed; `k = 0` → unchanged.

---

# Section 2 — Important

## 8. Design Circular Deque — LeetCode 641

#1 plus two operations:

```
push_front(v):  front = (front - 1 + CAP) % CAP;  arr[front] = v;  ++count
pop_back():     --count                                    # nothing else to do
```

**Key insight — `+ CAP` before the modulo.** `(0 - 1) % CAP` is `-1` in C++, not `CAP - 1`, and a
negative index is an out-of-bounds write. `pop_back` needs no index arithmetic at all, because the
rear is derived from `front + count`.

**Do not shift.** `deque.cpp`'s `push_front` uses a shift loop — and the loop copies in the wrong
direction anyway; see the bugs section.

---

## 9. Students Unable to Eat Lunch — LeetCode 1700 — **the trap**

**The naive reading** says to simulate: rotate the student queue until someone takes the top
sandwich. But that loops forever once nobody wants it, and detecting the deadlock is fiddly.

**Key insight — order does not matter, only counts.** A student who does not want the current
sandwich goes to the back and the queue is otherwise unchanged, so the only question is whether
*anyone at all* wants sandwich type `s`. That collapses to two counters:

```
cnt[0], cnt[1] = number of students preferring each type
for each sandwich s in order:
    if cnt[s] == 0:  return cnt[0] + cnt[1]      # nobody wants it -> everyone left is stuck
    cnt[s]--
return 0
```

**O(n) time, O(1) space, and no queue at all.** Recognising that the described data structure is
not the one to use is the actual skill being tested — the same lesson as *Crawler Log Folder* in
`../18_stack` §4.

**Verified:** `students={1,1,0,0}, sandwiches={0,1,0,1}` → 0; the deadlock case
`students={1,1,1,0,0,1}, sandwiches={1,0,0,0,1,1}` → 3.

---

## 10. Time Needed to Buy Tickets — LeetCode 2073

Same lesson as #9. The simulation is O(total tickets); the direct O(n) answer is:

```
total = 0
for i, t in tickets:
    total += (i <= k) ? min(t, tickets[k]) : min(t, tickets[k] - 1)
```

**Key insight.** Person `i` buys `min(tickets[i], tickets[k])` tickets before person `k` finishes
— minus one if they stand *behind* `k`, because they do not get a final turn.

---

## 11. Dota2 Senate — LeetCode 649 — **the one that genuinely needs queues**

```cpp
string predictPartyVictory(string senate) {
    queue<int> R, D;
    int n = (int)senate.size();
    for (int i = 0; i < n; ++i) (senate[i] == 'R' ? R : D).push(i);   // store INDICES
    while (!R.empty() && !D.empty()) {
        int r = R.front(); R.pop();
        int d = D.front(); D.pop();
        if (r < d) R.push(r + n);            // earlier index acts first and bans the other
        else       D.push(d + n);
    }
    return R.empty() ? "Dire" : "Radiant";
}
```

**Key insight — two things.**

1. **Store indices, not characters.** Turn order is decided by position, so the index *is* the
   state.
2. **Re-queue the survivor at `index + n`.** That places them correctly in the *next* round
   relative to everyone else, without ever tracking round numbers. Adding `n` each time keeps the
   ordering consistent across arbitrarily many rounds.

The loop ends when one side is empty; that side lost.

**Complexity.** O(n) time, O(n) space.

**Verified:** `"RD"→"Radiant"`, `"RDD"→"Dire"`, `"DDRRR"→"Dire"`.

---

## 12. First negative in every window of size K

Queue of the *indices* of negatives; the front is the answer, or 0 if the queue is empty. Full
solution and verification in `../14_Sliding window/solution.md` §1 #3 — including the two cases
your `ques.cpp` version gets wrong.

---

## 13. First non-repeating character in a stream

```
freq[26] = 0;  queue<char> q
for each incoming char c:
    freq[c]++;  q.push(c)
    while q not empty and freq[q.front()] > 1:  q.pop()      # front is no longer unique
    emit  q.empty() ? '#' : q.front()
```

**Key insight.** The queue holds *candidates in arrival order*; once a character repeats it can
never be the answer again, so popping it is permanent. Each character is pushed once and popped at
most once — **O(1) amortised per query**.

---

# Section 3 — Good to Know

## 14. Sliding Window Maximum — LeetCode 239

The monotonic **deque**: pop the front when it expires, pop the back while the incoming element
dominates, and the front is always the window maximum. O(n) time, O(k) space.

Full solution, code and verification in `../14_Sliding window/solution.md` §3 #17.

**Why it belongs here too:** it is the problem that justifies `deque` existing. A `queue` cannot do
it (no back removal) and a `stack` cannot (no front removal); you need both ends in O(1).

---

## 15 & 16. BFS — Rotting Oranges (994), Number of Islands (200) `[fwd]`

**The BFS skeleton:**

```
queue of positions;  visited grid
push every starting cell;  mark visited
level = 0
while queue not empty:
    sz = queue.size()                       # freeze the level boundary
    for i in 0 .. sz-1:
        cell = pop
        for each of the 4 neighbours:
            if in bounds and unvisited and passable:
                mark visited;  push
    level++
```

**Key insight — two.**

1. **`sz = queue.size()` before the inner loop** is what separates levels. Without it you cannot
   tell distance `k` from `k+1`, and #15 needs exactly that (the answer is the number of levels).
2. **#15 is *multi-source* BFS**: seed the queue with **every** rotten orange before starting.
   Levels then measure simultaneous spread, which is the whole problem. Running BFS once per source
   would be O(n²).

**Mark visited at push time, not at pop time** — otherwise the same cell is queued several times
before it is first processed.

These are forward references to `../27_graphs`, but doing them while the queue is fresh makes graph
traversal a variation rather than a new topic.

---

## 17. Design Hit Counter — LeetCode 362

A queue of timestamps; on every call, pop from the front while `front <= now - 300`. The size is
the answer. **Amortised O(1)** — each timestamp is pushed and popped once.

For high-frequency hits, a 300-slot circular buffer of (timestamp, count) pairs is the O(1)-space
follow-up.

---

## 18. Design Front Middle Back Queue — LeetCode 1670

Two deques, `left` and `right`, with the invariant `left.size() <= right.size() <= left.size()+1`.
Rebalance after every operation. The middle is then always at a known end of one of them.

**Key insight.** Same idea as the LRU cache in `../16_oops` §3 #15 — compose two structures and
make the class responsible for keeping an invariant between them. The invariant is the design.

---

# Section 4 — approach only

- **k queues in one array** — a free-list of unused slots plus per-queue front/rear pointers. Far
  better than fixed `n/k` partitions, which overflow one queue while others sit empty.
- **Interleave the halves of a queue** — push the first half into a stack, pour it back (reversing
  it), rotate, then alternate. Or split into two queues and zip them.
- **Generate binary numbers 1..N** — seed the queue with `"1"`; pop `s`, emit it, push `s+"0"` and
  `s+"1"`. This is BFS over a binary tree of strings — the same skeleton as #15.
- **Circular Tour / Gas Station (134)** — often framed with a queue; the O(n) greedy (track a
  running deficit, restart when it goes negative) is strictly better. Another "is a queue actually
  the tool?" case.
- **Sum of min and max of all subarrays of size K** — two monotonic deques at once, one increasing
  and one decreasing; add `front` of each per window.
- **Moving Average (346) / Recent Counter (933)** — fixed-size queue with a running sum, and a
  timestamp queue with expiry. Both are #17's shape.
- **Level-order traversal (102)** — the `sz = queue.size()` level trick from #15, on a tree. This is
  what `../21_tree` opens with.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `20_queu/` was edited.**

## All three implementation files fail to compile — the same root cause

```
arrayImplementation.cpp:35: error: 'int Queue::front()' conflicts with a previous declaration
arrayImplementation.cpp:39: error: 'int Queue::back()'  conflicts with a previous declaration
arrayImplementation.cpp:44: error: 'int Queue::size()'  conflicts with a previous declaration
```

| File | Result |
|---|---|
| `arrayImplementation.cpp` | **does not compile** |
| `deque.cpp` | **does not compile** |
| `linkedListIMplementation.cpp` | **does not compile** |

**In C++ a class cannot have a member variable and a member function with the same name.** You
have `int front; int back; int size;` as data members *and* `int front()`, `int back()`,
`int size()` as accessors. This is a hard error, not a shadowing warning — the name `front` inside
the class can only mean one thing.

It cascades: the constructor line `front=0;` now names a function, giving
`error: invalid use of member 'int Queue::front()' (did you forget the '&' ?)`, and in
`linkedListIMplementation.cpp` `back->next=temp` becomes
`error: base operand of '->' is not a pointer`.

**Fix — rename one side.** Either the data (`frontIdx` / `rearIdx` / `count`, or `head` / `tail`)
or the accessors (`getFront()` / `getBack()`). The standard library sidesteps this by having no
public data members at all.

## `arrayImplementation.cpp:16` and `deque.cpp:18, 28` — `=` instead of `==`

```cpp
if(size=100){ cout<<"queue overflow";return; }
```

Assigns 100 and evaluates to true, so **push always refuses** — and sets `size` to 100 on the way,
corrupting the object. This is the same bug as `userDefinedStackArray.cpp` and
`Vectorimplementation.cpp` in `../18_stack`, giving **five occurrences across the two folders**.
It is worth making a habit of writing the constant on the left (`if (100 == size)`), which turns
the typo into a compile error.

Also `size=100` should be `size == CAP - 1` given that `size` is being used as a top-index starting
at −1, and the empty-check message says "stack is empty" in a queue class.

## `arrayImplementation.cpp:26` — `pop` is O(n), and the loop bound is short

```cpp
for (int i =0;i<size-1;i++){ arr[i]=arr[i+1]; }
back--;size--;
```

Two problems. **Shifting every element makes `pop` O(n)** — the whole reason circular buffers
exist. And with `size` used as the *top index* (initialised to −1), the last element is at
`arr[size]`, so the loop should run to `i < size`, not `size - 1`; as written the final element is
never shifted down.

The circular version in §1 does no shifting at all.

## `deque.cpp:31` — `push_front` shifts in the wrong direction

```cpp
for (int i =1;i<=size;i++){ arr[i]=arr[i-1]; }
arr[0]=val;
```

Copying **left to right** propagates `arr[0]` across the whole array: `arr[1] = arr[0]`, then
`arr[2] = arr[1]` — which is the value just written — and so on. Every element becomes a copy of
the original `arr[0]`.

To shift right you must iterate **downward**: `for (int i = size; i >= 1; --i) arr[i] = arr[i-1];`

Better still, do not shift: `front = (front - 1 + CAP) % CAP;` is O(1).

## `ques.cpp:8` — `reverse_k_ele` leaves one element un-rotated

```cpp
for(int i =1;i<n;i++){        // n-1 iterations; needs n
    if(i<=k){ q.push(st.top()); st.pop(); }
    else    { q.push(q.front()); q.pop(); }
}
```

The loop must perform `k` pushes from the stack **plus** `n - k` rotations — `n` iterations total.
Running from `1` to `n - 1` gives one too few, so the last element never rotates into place.

**Verified:**

| Input | k | Your output | Correct |
|---|---|---|---|
| `{1,2,3,4,5}` | 3 | `5,3,2,1,4` | `3,2,1,4,5` |
| `{1,2,3,4}` | 2 | `4,2,1,3` | `2,1,3,4` |

Corrected version in §1 #7 — two separate loops rather than one loop with an `if`, which removes
the chance of an off-by-one.

## `ques.cpp:28` — `firstnegative` computes `n` from a decayed pointer

```cpp
void firstnegative(int arr[],int k){
    int n =sizeof(arr)/sizeof(arr[0]);      // arr is int*, so this is 4/4
```

**Verified: `n` evaluates to 1** for a 9-element array. Identical to the `bubblesort.cpp` bug in
`../12_sorting` — an array parameter decays to a pointer, so `sizeof` gives the pointer size.
Pass the length explicitly.

Two more faults in the same function:

- **The expiry condition is wrong.** `while(help.size() && i+k<n) help.pop();` drains the entire
  queue whenever `i+k < n`, rather than popping only indices that have left the window. It should
  be `while (!help.empty() && help.front() < i) help.pop();`.
- **`ans` is never returned or printed** — the function is `void` and the vector is discarded.

Working version in `../14_Sliding window/solution.md` §1 #3.

---

## What you got right

- **`basics.cpp` is correct** — verified, it prints `10`, `50`, `20`, matching your comments
  (`front`, `back`, `front` after a pop).

- **`display()` at `basics.cpp:6` is the right technique.** A queue cannot be iterated, so you
  rotate: pop the front, print it, push it back, exactly `n` times. Capturing `n = q.size()`
  **before** the loop is the essential detail — using `q.size()` in the condition would loop
  forever, since every pop is matched by a push. You got that right.

- **`display_rev()` at `:14` uses a stack to reverse**, which is the correct composition and the
  same idea as §1 #6.

- **`removeEven()` at `:32`** is correct: rotate `n` times and re-push only the odd indices. The
  `n` is captured up front here too.

- **`linkedListIMplementation.cpp`'s `pop()` calls `delete(temp)`** — it is the only container in
  the last three folders that frees what it removes. The stack implementations in `../18_stack` all
  leak. Keep that habit.

- **`linkedListIMplementation.cpp` returns `-1` on an empty queue** rather than reading through a
  null pointer. That is a real guard, and better than the stack files which print a message and
  then dereference anyway.

- **Making the data members `private`** in all three queue classes is the right call — it is the
  encapsulation point from `../16_oops`, and it is what would have let you rename them freely to
  fix the collision without touching any caller.

- **`ques.cpp`'s `reverse_k_ele` has the right structure** — drain `k` into a stack, push them back,
  rotate the rest. Only the loop bound is wrong. And using a *stack* to reverse a *queue* segment is
  exactly the right instinct for a problem that mixes both.
