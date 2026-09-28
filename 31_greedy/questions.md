# 31 — Greedy Algorithms: Practice Questions

> **Why 22 ranked problems.** Greedy has moderate breadth: four main families (interval scheduling, sorting-based, two-pass, and proof-by-contradiction), and 22 covers the families plus the key compositions. Unlike DP where each pattern is independent, greedy techniques transfer better — once you can prove exchange arguments, half the problems fall.

**Platform note.** LeetCode numbers are exact. **GeeksforGeeks has no numeric IDs** — search the quoted title.

**Scope note.** Everything needs only `01`–`30`. Striver's A2Z Step 12 (Greedy) is the coverage floor.

---

## Section 1 — Must Do

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 1 | Assign Cookies | Easy | **LeetCode 455** | |
| 2 | Lemonade Change | Easy | **LeetCode 860** | |
| 3 | Jump Game | **Medium** | **LeetCode 55** | |
| 4 | Jump Game II | **Medium** | **LeetCode 45** | |
| 5 | Non-overlapping Intervals | **Medium** | **LeetCode 435** | the Activity Selection equivalent |
| 6 | N Meetings in One Room | Easy | GFG | *"N meetings in one room"* |
| 7 | Minimum Number of Platforms | **Medium** | GFG | *"Minimum Platforms"* |
| 8 | Fractional Knapsack | **Medium** | GFG | *"Fractional Knapsack"* |

**Why these eight.** #1-#2 are warm-ups where greedy is obvious. #3-#4 are the Jump Game pair — #3 is decision (reachability), #4 is optimization (minimum jumps) — and doing them consecutively shows how the same greedy framework handles both. #5-#6 are the Activity Selection family — #5 is the LeetCode formulation (minimize removals = maximize non-overlapping), #6 is the classic scheduling formulation. #7 is the event-based variant using sorted events. #8 is the canonical greedy-vs-DP contrast problem.

---

## Section 2 — Important

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 9 | Gas Station | **Medium** | **LeetCode 134** | |
| 10 | Candy | **Hard** | **LeetCode 135** | |
| 11 | Job Sequencing Problem | **Medium** | GFG | *"Job Sequencing Problem"* |
| 12 | Valid Parenthesis String | **Medium** | **LeetCode 678** | |
| 13 | Largest Number | **Medium** | **LeetCode 179** | |
| 14 | Minimum Number of Arrows to Burst Balloons | **Medium** | **LeetCode 452** | |
| 15 | Insert Interval | **Medium** | **LeetCode 57** | |

**Why these seven.** #9 is the single-pass circular greedy. #10 is the two-pass greedy (left-to-right then right-to-left). #11 is sorting by profit + greedy slot assignment — the classic job scheduling problem. #12 is range-tracking greedy on parentheses. #13 is custom comparator greedy on strings. #14 is #5's twin (bursting balloons ≈ interval overlap). #15 is interval merging from a different angle.

---

## Section 3 — Good to Know

| # | Problem | Difficulty | Platform | Where |
|---|---|---|---|---|
| 16 | Remove K Digits | **Medium** | **LeetCode 402** | monotonic stack greedy |
| 17 | Reorganize String | **Medium** | **LeetCode 767** | greedy with max-heap |
| 18 | Task Scheduler | **Medium** | **LeetCode 621** | greedy frequency counting |
| 19 | Queue Reconstruction by Height | **Medium** | **LeetCode 406** | |
| 20 | Optimal Partition of String | **Medium** | **LeetCode 2405** | |
| 21 | Minimum Cost to Connect Sticks | **Medium** | **LeetCode 1167** | or GFG *"Connect N Ropes"* — greedy with min-heap |
| 22 | Boats to Save People | **Medium** | **LeetCode 881** | two-pointer greedy |

**Why these seven.** #16 is where greedy meets monotonic stack. #17-#18 are frequency-based greedy. #19 is a clever sorting + insertion problem. #20 is a simple greedy partition. #21 connects to Huffman coding (always merge the two smallest). #22 is two-pointer greedy (pair heaviest with lightest).

---

## Section 4 — Extra Practice

| Problem | Difficulty | Platform | Note |
|---|---|---|---|
| Minimum Coins | Easy | *Drill* | standard denominations |
| Activity Selection | Easy | GFG | *"Activity Selection"* — classic formulation |
| Shortest Job First scheduling | Easy | GFG | *"Shortest Job first"* |
| Page Faults in LRU | **Medium** | GFG | *"Page Faults in LRU"* |
| Maximize Toys | Easy | GFG | *"Buy Maximum Toys"* |
| Split Array Largest Sum | **Hard** | **LeetCode 410** | binary search + greedy check |
| IPO | **Hard** | **LeetCode 502** | greedy with two heaps |
| Minimum Number of Refueling Stops | **Hard** | **LeetCode 871** | |

---

## Progress tracker

```
Section 1   [ ] 1   [ ] 2   [ ] 3   [ ] 4   [ ] 5   [ ] 6   [ ] 7   [ ] 8
Section 2   [ ] 9   [ ] 10  [ ] 11  [ ] 12  [ ] 13  [ ] 14  [ ] 15
Section 3   [ ] 16  [ ] 17  [ ] 18  [ ] 19  [ ] 20  [ ] 21  [ ] 22
Section 4   [ ] ______ / 8
```
