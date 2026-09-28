# 31 — Greedy Algorithms

Greedy algorithms are simultaneously the easiest algorithms to write and the hardest algorithms to justify. A greedy approach simply means making the locally optimal choice at every single step, in the hope that these local optimums will culminate in a globally optimal solution. 

When a greedy algorithm works, it is beautiful: usually $O(N \log N)$ due to an initial sort, with minimal memory overhead, and extremely short code. When it fails, it fails silently, returning a plausible but incorrect answer. The entire skill of mastering greedy algorithms lies not in the coding, but in the **proof**.

---

## 1. What is Greedy? — The Core Idea

A greedy algorithm makes the **locally optimal choice** at each step, hoping it leads to a **globally optimal** solution. It builds up a solution piece by piece, always choosing the next piece that offers the most obvious and immediate benefit.

The key question you must ask yourself in an interview is: *"Can I prove that being greedy NOW never ruins the future?"*
- If yes $\rightarrow$ greedy works.
- If no $\rightarrow$ you must use Dynamic Programming (DP) or Backtracking.

### The Two Conditions for Greedy

For a greedy algorithm to yield the correct answer, the problem must possess two properties:
1. **Greedy Choice Property**: A globally optimal solution can be arrived at by making locally optimal (greedy) choices. In other words, making the greedy choice never prevents you from reaching an overall optimal state.
2. **Optimal Substructure**: An optimal solution to the problem contains optimal solutions to subproblems. This is the same property required for DP.

### The Classic Example: Coin Change

Consider making change with standard US coins: {1, 5, 10, 25}. 
If you need 41 cents, a greedy approach (always taking the largest possible coin) gives:
- 41 >= 25 $\rightarrow$ take 25 (remainder 16)
- 16 >= 10 $\rightarrow$ take 10 (remainder 6)
- 6 >= 5 $\rightarrow$ take 5 (remainder 1)
- 1 >= 1 $\rightarrow$ take 1 (remainder 0)
Result: 4 coins (25, 10, 5, 1). This is optimal. Greedy works here because the coin denominations are special (each is a multiple of the previous or structured in a way that prevents anomalies).

Now consider coins {1, 3, 4} and a target of 6.
Greedy gives:
- 6 >= 4 $\rightarrow$ take 4 (remainder 2)
- 2 >= 1 $\rightarrow$ take 1 (remainder 1)
- 1 >= 1 $\rightarrow$ take 1 (remainder 0)
Result: 3 coins (4, 1, 1).
But the optimal solution is 2 coins (3, 3). 
This is exactly WHY you need DP for the general Coin Change problem (`../26_dp`). Greedy only works for specific, well-behaved systems like canonical coin systems.

---

## 2. Greedy vs DP — When to Choose Which

Both Greedy and Dynamic Programming solve optimization problems. The core difference is how they explore the search space.

| Feature | Greedy | Dynamic Programming |
|---|---|---|
| **Decision** | One single choice per step, never revisited. | All possible choices explored, best one kept. |
| **Proof of Correctness** | Greedy choice property (usually proved via exchange argument). | Bellman's principle of optimality (state transitions). |
| **Time Complexity** | Usually $O(N \log N)$ (sorting) or $O(N)$ (linear scan). | Usually $O(N^2)$ or $O(N \cdot W)$ (state space size). |
| **Space Complexity** | Usually $O(1)$ or $O(N)$ for sorting/storing. | Often $O(N^2)$ or $O(N \cdot W)$ for memoization tables. |
| **Classic Example** | Activity Selection / Fractional Knapsack. | 0/1 Knapsack / Longest Common Subsequence. |

### The Critical Distinction: The Knapsack Problem

The difference is perfectly illustrated by the Knapsack Problem.

**Fractional Knapsack (Greedy)**: You can take fractions of items.
- Strategy: Sort items by their value-to-weight ratio. Take as much of the highest-ratio item as possible. If the knapsack fills up, take a fraction of the current item.
- **Why it works**: Because items are continuously divisible, taking the highest ratio item *never wastes capacity*. You are mathematically extracting the maximum value per unit of weight.

**0/1 Knapsack (DP)**: You must take an item whole or leave it. 
- Strategy: You cannot use greedy. You must use DP (covered in `../26_dp`).
- **Why greedy fails**: With indivisible items, taking a high-ratio item might leave you with awkward leftover capacity that cannot be filled. For example, knapsack capacity = 50. Item 1: weight 10, value 60 (ratio 6). Item 2: weight 20, value 100 (ratio 5). Item 3: weight 30, value 120 (ratio 4). 
Greedy picks Item 1 (ratio 6). Remaining capacity 40. Then picks Item 2 (ratio 5). Remaining capacity 20. Total value = 160.
Optimal takes Item 2 and Item 3. Total value = 220. 
Because Item 1 prevented the combination of Item 2 + 3, the local optimum ruined the global optimum.

---

## 3. The Exchange Argument — How to Prove Greedy Works

In an interview, if you claim a greedy algorithm works, a strong interviewer will ask: *"Can you prove that?"* You don't need a formal mathematical proof, but you do need the **Exchange Argument**.

The Exchange Argument is the standard way to prove greedy correctness.

### The Idea:
1. Assume, for the sake of contradiction, that there exists an optimal solution $O$ that **does not** use the greedy choice at a certain step.
2. Show that you can **exchange** elements in $O$ to force it to use the greedy choice, yielding a new solution $O'$.
3. Prove that the value of $O'$ is $\ge$ the value of $O$ (i.e., making the swap doesn't make the solution worse).
4. By induction, you can keep swapping until the solution perfectly matches the greedy algorithm's output, proving the greedy strategy is optimal.

### Concrete Example: Activity Selection
Problem: Given $N$ meetings with start and end times, find the maximum number of non-overlapping meetings.
Greedy Strategy: Always pick the meeting that finishes earliest.

**Proof via Exchange**:
Suppose the optimal schedule $O$ picks meeting $M_k$ as its first meeting, but the greedy choice would be meeting $M_1$ (which finishes strictly earlier than $M_k$). 
- Because $M_1$ finishes before $M_k$, if we swap $M_k$ out of $O$ and put $M_1$ in its place, $M_1$ will end even sooner than $M_k$ did. 
- Since $M_1$ ends sooner, it cannot possibly overlap with whatever meeting $O$ had scheduled *after* $M_k$. 
- The number of meetings remains exactly the same, but the ending time is now earlier or equal. The solution is just as good, if not better.
- Therefore, the greedy choice is always safe.

Being able to verbalize this concept separates a candidate who is just guessing from an engineer who understands algorithm design.

---

## 4. Pattern 1 — Interval/Scheduling Problems

Interval problems are the most common manifestation of greedy algorithms. 
The core trick is deciding whether to sort by **start time** or **end time**.

### Sorting by End Time: Activity Selection
Whenever the goal is to maximize the count of non-overlapping intervals, you want to free up the resource as early as possible. Thus, sort by end time.

```cpp
// g++ -std=gnu++14
// Non-overlapping Intervals (LC 435) / Activity Selection
#include <vector>
#include <algorithm>

using namespace std;

int eraseOverlapIntervals(vector<vector<int>>& intervals) {
    if (intervals.empty()) return 0;
    
    // Sort by end time
    sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];
    });
    
    int count = 1;
    int current_end = intervals[0][1];
    
    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] >= current_end) {
            count++;
            current_end = intervals[i][1];
        }
    }
    // Return minimum removals to make non-overlapping
    return intervals.size() - count;
}
```

### Sorting by Start Time: Merge Intervals & Meeting Rooms
When the goal is to group, merge, or find the maximum overlap (how many rooms do I need?), sorting by start time processes events in chronological order.

- **Merge Intervals (LC 56)**: Covered heavily in `../07_Array`. Sort by start, merge if `current[0] <= prev[1]`.
- **Minimum Platforms / Meeting Rooms II**: Sort arrival/start times and departure/end times separately, or use a min-heap to track when rooms free up.

```cpp
// g++ -std=gnu++14
// Minimum Platforms (GFG)
#include <algorithm>

using namespace std;

int findPlatform(int arr[], int dep[], int n) {
    sort(arr, arr + n);
    sort(dep, dep + n);
    
    int platforms_needed = 1, result = 1;
    int i = 1, j = 0;
    
    while (i < n && j < n) {
        // If next train arrives before or when the previous train departs
        if (arr[i] <= dep[j]) {
            platforms_needed++;
            i++;
        } else {
            // Train departs, platform frees up
            platforms_needed--;
            j++;
        }
        result = max(result, platforms_needed);
    }
    return result;
}
```

---

## 5. Pattern 2 — Sorting-Based Greedy

These problems become trivial once you sort the input based on some clever criterion. The greedy part is simply taking the best available option from the sorted array.

### Assign Cookies (LC 455)
Problem: You have children with greed factors and cookies with sizes. Maximize happy children.
Sort children, sort cookies. Give the smallest sufficient cookie to the least greedy child.

```cpp
// g++ -std=gnu++14
#include <vector>
#include <algorithm>

using namespace std;

int findContentChildren(vector<int>& g, vector<int>& s) {
    sort(g.begin(), g.end());
    sort(s.begin(), s.end());
    int i = 0, j = 0;
    while (i < g.size() && j < s.size()) {
        if (s[j] >= g[i]) {
            i++; // Child is happy, move to next child
        }
        j++; // Always move to next cookie
    }
    return i;
}
```

### Job Sequencing with Deadlines
You are given jobs with a deadline and profit. Maximize profit.
Greedy strategy: Sort by profit descending. Try to schedule each job as late as possible (closest to its deadline). This leaves earlier slots open for other jobs.

---

## 6. Pattern 3 — Two-Pass / Multi-Pass Greedy

Sometimes, a single left-to-right pass cannot capture all constraints. But two passes (one left-to-right, one right-to-left) can beautifully enforce a dual constraint.

### Candy (LC 135)
Problem: Give candies to children based on ratings. A child must have more candies than their neighbors if their rating is higher.
- Pass 1 (Left to Right): Ensure right child gets more if rating > left child.
- Pass 2 (Right to Left): Ensure left child gets more if rating > right child.

```cpp
// g++ -std=gnu++14
#include <vector>
#include <algorithm>

using namespace std;

int candy(vector<int>& ratings) {
    int n = ratings.size();
    vector<int> candies(n, 1);
    
    // Left to right pass
    for (int i = 1; i < n; i++) {
        if (ratings[i] > ratings[i-1]) {
            candies[i] = candies[i-1] + 1;
        }
    }
    
    // Right to left pass
    for (int i = n - 2; i >= 0; i--) {
        if (ratings[i] > ratings[i+1]) {
            candies[i] = max(candies[i], candies[i+1] + 1);
        }
    }
    
    int total = 0;
    for (int c : candies) total += c;
    return total;
}
```

### Gas Station (LC 134)
A brilliant single-pass greedy trick. 
Property 1: If total gas < total cost, it's impossible.
Property 2: If you start at $A$ and get stuck at $B$, **no station between $A$ and $B$ can be a valid starting point**.
Therefore, just greedily start at the next station after $B$.

---

## 7. Pattern 4 — Greedy with Proof by Contradiction

These problems require you to greedily stretch a boundary or satisfy constraints under the assumption that "if I fail doing this, it must be completely impossible."

### Jump Game I (LC 55)
Track the farthest index you can reach. If you are currently at index $i$ and $i > \text{farthest}$, you are stuck.

```cpp
// g++ -std=gnu++14
#include <vector>
#include <algorithm>

using namespace std;

bool canJump(vector<int>& nums) {
    int farthest = 0;
    for (int i = 0; i < nums.size(); i++) {
        if (i > farthest) return false; // We can't even reach index i
        farthest = max(farthest, i + nums[i]);
        if (farthest >= nums.size() - 1) return true;
    }
    return true;
}
```

### Valid Parenthesis String (LC 678)
With `*` acting as `(`, `)`, or empty, tracking exact balances is hard. Instead, greedily track the **minimum** and **maximum** possible number of open brackets. If max becomes negative, it's invalid. If min is zero at the end, it's valid.

---

## 8. Pattern 5 — Greedy on Strings/Arrays

These often involve monotonic stacks, custom comparators, or priority queues to greedily construct the lexicographically smallest/largest sequence.

### Largest Number (LC 179)
You want to arrange numbers to form the largest string. 
Greedy strategy: Sort them using a custom comparator.
Compare `"a" + "b"` vs `"b" + "a"`. 

```cpp
// g++ -std=gnu++14
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

string largestNumber(vector<int>& nums) {
    vector<string> strs;
    for (int num : nums) strs.push_back(to_string(num));
    
    sort(strs.begin(), strs.end(), [](string& a, string& b) {
        return a + b > b + a;
    });
    
    if (strs[0] == "0") return "0";
    
    string res = "";
    for (string& s : strs) res += s;
    return res;
}
```

### Remove K Digits (LC 402)
To make a number as small as possible by removing $K$ digits, you must remove "peaks" (a digit that is greater than the one immediately following it). 
Use a monotonic increasing stack: pop while `stack.top() > current_digit` and $K > 0$.

### Reorganize String (LC 767)
To prevent adjacent duplicate characters, greedily place the most frequent character. Use a max-heap of character frequencies. Always pop the top two, place them, and push them back with decremented counts.

---

## 9. When Greedy FAILS — Common Traps

The biggest danger in an interview is confidently writing a greedy solution for a DP problem. If you cannot mathematically convince yourself (using the exchange argument) that greedy works, do not use it.

Common traps where greedy fails:
1. **0/1 Knapsack**: Cannot take fractions, taking a high-value item might block a better combination. DP needed.
2. **Coin Change (arbitrary denominations)**: `{1, 3, 4}` to make 6. Greedy takes 4, 1, 1. Optimal is 3, 3. DP needed.
3. **Longest Increasing Subsequence**: Greedily picking the next largest element fails. E.g., `[10, 20, 11, 12, 13]`. Greedy from 10 might pick 20 and stop. Optimal is 10, 11, 12, 13. DP or Binary Search needed.
4. **Pathfinding with Negative Weights**: Dijkstra is a greedy algorithm. It fails with negative weights because its greedy assumption (once a node is visited, its shortest path is finalized) is violated. Bellman-Ford (DP) is needed.

The Trap: Just because a greedy strategy yields a plausible, sometimes optimal answer on sample test cases does NOT mean it is globally correct. You must verify the greedy choice property.

---

## 10. Interview Q&A

**Q1: When does greedy work vs when do you need DP?**
Greedy works when a locally optimal choice is guaranteed to lead to a globally optimal solution (Greedy Choice Property). It never needs to revisit past decisions. If a decision depends on future consequences or overlapping subproblems where a local choice might ruin the global outcome, you must use DP to explore all possibilities.

**Q2: What is the exchange argument?**
It is a mathematical proof technique used to prove greedy algorithms. You assume an optimal solution exists that doesn't use the greedy choice. You then show that you can swap out one of its choices for the greedy choice without making the overall solution worse. This proves the greedy choice is always safe.

**Q3: Why does greedy work for Activity Selection but not 0/1 Knapsack?**
In Activity Selection, finishing a meeting as early as possible strictly maximizes the remaining time for other meetings; it imposes no other constraint. In 0/1 Knapsack, taking an item consumes capacity. A locally optimal item (high value/weight) might consume an awkward amount of capacity, preventing the inclusion of two smaller items whose combined value is greater. The indivisibility breaks the greedy choice.

**Q4: How do you prove a greedy algorithm is correct in an interview?**
You don't need formal math. Use the exchange argument conceptually: "If I choose something else, can I swap the greedy choice back in and get an equal or better result?" If the answer is yes, explain that swap to the interviewer. Also, verify that there are no hidden constraints that the greedy choice violates.

**Q5: Fractional Knapsack vs 0/1 Knapsack — what's the difference?**
Fractional knapsack allows you to take parts of an item (like sand or liquid). Because you can break items apart, you can always perfectly fill the knapsack with the absolute highest value-to-weight ratio material. This is solvable greedily. 0/1 Knapsack forces an all-or-nothing choice, requiring DP.

**Q6: Why sort by finish time in Activity Selection, not by start time or duration?**
- If you sort by start time: A meeting starting at 0 but lasting 100 hours blocks everything else.
- If you sort by duration: A short 1-hour meeting from 11:30 to 12:30 could overlap and block two other meetings (10:00-12:00 and 12:00-14:00).
- Sorting by finish time ensures you free up the resource as early as possible, strictly maximizing the remaining unreserved time.

**Q7: Can greedy algorithms produce non-unique optimal solutions?**
Yes. If there are multiple choices that are equally optimal locally (e.g., two meetings finish at exactly the same earliest time), picking either one might lead to a different but equally valid globally optimal solution. The greedy choice guarantees *an* optimal value, not necessarily a unique optimal sequence.
