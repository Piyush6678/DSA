# 31 — Greedy Algorithms: Solutions

Approach, pseudocode, complexity and key insight. Real C++ appears only for the **fundamentals** (implementations), the **tricks**, and the **hard** ones.

**Everything shown as code was compiled with `g++ -std=gnu++14` and executed.**

---

# Section 1 — Must Do

## 1. Assign Cookies — LeetCode 455

**Approach.** Sort both the greed factor of children and the size of cookies. Use two pointers to iterate through both arrays. If the current cookie can satisfy the current child, move both pointers. Otherwise, just move the cookie pointer to find a larger cookie.

```text
sort(g)
sort(s)
i = 0, j = 0
while i < g.length and j < s.length:
    if s[j] >= g[i]:
        i++
        j++
    else:
        j++
return i
```

**Key insight.** Sorting lets us greedily match the smallest cookie to the least greedy child. By satisfying the least greedy children first with the smallest possible cookies, we save larger cookies for greedier children later.

**Complexity.** Time O(N log N + M log M) due to sorting, Space O(1) or O(log N) depending on the sort implementation.

---

## 2. Lemonade Change — LeetCode 860

**Approach.** Iterate through the customers. Track the number of $5 and $10 bills you currently have. For a $5 bill, just increment the count. For a $10 bill, give one $5 bill as change. For a $20 bill, prefer giving one $10 and one $5 as change. If not possible, give three $5 bills.

```text
five = 0, ten = 0
for bill in bills:
    if bill == 5:
        five++
    else if bill == 10:
        if five == 0: return false
        five--
        ten++
    else:
        if ten > 0 and five > 0:
            ten--
            five--
        else if five >= 3:
            five -= 3
        else:
            return false
return true
```

**Key insight.** Always use $10 before two $5s for $20 change — preserving $5 bills is critical. A $5 bill is more versatile because it can be used to make change for both $10 and $20, whereas a $10 bill can only be used for $20.

**Complexity.** Time O(N), Space O(1).

---

## 3. Jump Game — LeetCode 55

**Approach.** Keep track of the farthest index that can be reached so far. Iterate through the array, and if the current index is greater than the farthest reachable index, return false. Otherwise, update the farthest reachable index.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    bool canJump(vector<int>& nums) {
        int farthest = 0;
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (i > farthest) {
                return false;
            }
            farthest = max(farthest, i + nums[i]);
            if (farthest >= n - 1) {
                return true;
            }
        }
        return true;
    }
};
```

**Key insight.** If you can reach index `i`, you can reach everything before it. Thus, you only need to track the maximum reach. There is no need to explore all possible paths, making the greedy choice optimal.

**Complexity.** Time O(N), Space O(1).

---

## 4. Jump Game II — LeetCode 45

**Approach.** Use a BFS-like approach where you keep track of the current level's end (the maximum reach of the previous jump) and the farthest you can reach in the next level. When you iterate past the current level's end, it means you must have made another jump.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int jump(vector<int>& nums) {
        int n = nums.size();
        if (n <= 1) return 0;
        
        int jumps = 0;
        int current_level_end = 0;
        int farthest = 0;
        
        for (int i = 0; i < n - 1; ++i) {
            farthest = max(farthest, i + nums[i]);
            if (i == current_level_end) {
                jumps++;
                current_level_end = farthest;
                if (current_level_end >= n - 1) {
                    break;
                }
            }
        }
        
        return jumps;
    }
};
```

**Key insight.** Each "level" is the range reachable in `k` jumps; the level boundary is the answer. You only increment the jump count when you cross the boundary of what was reachable with the previous number of jumps.

**Complexity.** Time O(N), Space O(1).

---

## 5. Non-overlapping Intervals — LeetCode 435

**Approach.** Sort the intervals by their end times. Keep track of the end time of the last added interval. If the next interval overlaps (start time < last end time), increment a removal counter. Otherwise, update the last end time.

```text
sort(intervals by end time)
count = 0
last_end = INT_MIN

for interval in intervals:
    if interval.start >= last_end:
        last_end = interval.end
    else:
        count++
        
return count
```

**Key insight.** This IS Activity Selection — minimize removals = maximize non-overlapping. By always picking the interval that ends earliest, we leave as much room as possible for subsequent intervals.

**Complexity.** Time O(N log N), Space O(1) or O(log N).

---

## 6. N Meetings in One Room — GFG

**Approach.** Exact same logic as Non-overlapping Intervals, but instead of counting removals, we count the number of meetings we can hold.

```text
sort(meetings by end time)
count = 0
last_end = -1

for meeting in meetings:
    if meeting.start > last_end:
        count++
        last_end = meeting.end
        
return count
```

**Key insight.** The earliest ending meeting leaves maximum free time for other meetings. Note that if start times are equal, sorting by end time still naturally prioritizes the shorter meeting.

**Complexity.** Time O(N log N), Space O(N) to store meeting structures if not already provided.

---

## 7. Minimum Platforms — GFG

**Approach.** Put all arrival and departure times into separate arrays. Sort both arrays. Use two pointers to traverse them. If an arrival happens before or at the same time as the current departure, a new platform is needed. Otherwise, a platform is freed. Keep track of the maximum platforms needed at any point.

```text
sort(arr)
sort(dep)
platforms = 1
max_platforms = 1
i = 1, j = 0
n = arr.length

while i < n and j < n:
    if arr[i] <= dep[j]:
        platforms++
        i++
    else:
        platforms--
        j++
    max_platforms = max(max_platforms, platforms)
    
return max_platforms
```

**Key insight.** The answer is the maximum overlap at any point. Sorting the arrays separates the events (arrivals and departures) but preserves the chronological sequence of overlapping intervals.

**Complexity.** Time O(N log N), Space O(1) if sorting in-place.

---

## 8. Fractional Knapsack — GFG

**Approach.** Calculate the value-to-weight ratio for each item. Sort items in descending order of this ratio. Greedily take as much of the item with the highest ratio as possible. If an item cannot fit entirely, take the fractional part that fits.

```cpp
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int value;
    int weight;
};

class Solution {
public:
    static bool cmp(struct Item a, struct Item b) {
        double r1 = (double)a.value / (double)a.weight;
        double r2 = (double)b.value / (double)b.weight;
        return r1 > r2;
    }
    
    double fractionalKnapsack(int W, Item arr[], int n) {
        sort(arr, arr + n, cmp);
        
        double final_value = 0.0;
        int current_weight = 0;
        
        for (int i = 0; i < n; i++) {
            if (current_weight + arr[i].weight <= W) {
                current_weight += arr[i].weight;
                final_value += arr[i].value;
            } else {
                int remain = W - current_weight;
                final_value += arr[i].value * ((double)remain / (double)arr[i].weight);
                break;
            }
        }
        
        return final_value;
    }
};
```

**Key insight.** Fractions allow continuous division, making greedy optimal — contrast with 0/1 knapsack in `../26_dp` where you cannot break items, thus making greedy fail because a high-ratio item might leave unused capacity.

**Complexity.** Time O(N log N), Space O(1).

---

# Section 2 — Important

## 9. Gas Station — LeetCode 134

**Approach.** Track the total gas minus total cost. If this total is negative, there is no solution. Otherwise, keep a running sum of `gas[i] - cost[i]`. If the running sum drops below 0, it means we cannot reach the next station from our current starting point, so we reset the starting point to the next station and reset the running sum to 0.

```cpp
#include <vector>

using namespace std;

class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int total_surplus = 0;
        int current_surplus = 0;
        int start_index = 0;
        
        for (int i = 0; i < gas.size(); ++i) {
            total_surplus += gas[i] - cost[i];
            current_surplus += gas[i] - cost[i];
            
            if (current_surplus < 0) {
                current_surplus = 0;
                start_index = i + 1;
            }
        }
        
        return total_surplus < 0 ? -1 : start_index;
    }
};
```

**Key insight.** If total gas ≥ total cost, a solution exists. Start from the point where the cumulative surplus stops being negative. Any station before the failing point also couldn't have reached the failing point, so we can safely jump the start index to `i + 1`.

**Complexity.** Time O(N), Space O(1).

---

## 10. Candy — LeetCode 135

**Approach.** Initialize an array `candies` with 1s. Do a left-to-right pass: if the current child has a higher rating than the left neighbor, they get one more candy than the left neighbor. Then do a right-to-left pass: if the current child has a higher rating than the right neighbor, they get `max(current, right_neighbor + 1)` candies.

```cpp
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

class Solution {
public:
    int candy(vector<int>& ratings) {
        int n = ratings.size();
        vector<int> candies(n, 1);
        
        for (int i = 1; i < n; ++i) {
            if (ratings[i] > ratings[i - 1]) {
                candies[i] = candies[i - 1] + 1;
            }
        }
        
        for (int i = n - 2; i >= 0; --i) {
            if (ratings[i] > ratings[i + 1]) {
                candies[i] = max(candies[i], candies[i + 1] + 1);
            }
        }
        
        int total = 0;
        for (int c : candies) {
            total += c;
        }
        return total;
    }
};
```

**Key insight.** Left pass handles rising neighbors, right pass handles falling. Take max of both passes. This cleanly resolves peaks without needing complex local maximum tracking.

**Complexity.** Time O(N), Space O(N).

---

## 11. Job Sequencing — GFG

**Approach.** Sort the jobs in descending order of their profit. Find the maximum deadline to size an array of time slots. For each job, iterate backwards from its deadline to 1. Assign it to the first empty slot found.

```text
sort(jobs by profit descending)
max_deadline = max(job.deadline for job in jobs)
slots = array of size max_deadline + 1 initialized to empty

total_profit = 0
for job in jobs:
    for i from job.deadline down to 1:
        if slots[i] is empty:
            slots[i] = job
            total_profit += job.profit
            break
            
return total_profit
```

**Key insight.** Latest available slot maximizes future flexibility. By pushing the job as far back as possible without missing its deadline, we reserve earlier slots for jobs that have tighter deadlines.

**Complexity.** Time O(N * max_deadline), Space O(max_deadline). Can be optimized to O(N log N) using Disjoint Set Union.

---

## 12. Valid Parenthesis String — LeetCode 678

**Approach.** Keep track of the minimum and maximum possible number of open parentheses. When encountering `(`, both min and max increase. For `)`, both decrease. For `*`, min decreases and max increases. If max drops below 0, it's invalid. If min drops below 0, reset it to 0. At the end, min must be 0.

```text
minOpen = 0
maxOpen = 0

for char in string:
    if char == '(':
        minOpen++
        maxOpen++
    else if char == ')':
        minOpen--
        maxOpen--
    else: // '*'
        minOpen--
        maxOpen++
        
    if maxOpen < 0: return false
    if minOpen < 0: minOpen = 0
    
return minOpen == 0
```

**Key insight.** The range of possible open-paren counts is what you track, not a single count. The asterisks create branching possibilities, but because the changes are contiguous by 1, tracking the bounds is sufficient.

**Complexity.** Time O(N), Space O(1).

---

## 13. Largest Number — LeetCode 179

**Approach.** Convert all integers to strings. Sort them using a custom comparator that compares `a + b` and `b + a`. If `a + b > b + a`, then `a` should come before `b`. Edge case: if the highest number after sorting is "0", return "0".

```text
strings = map(to_string, nums)
sort(strings, lambda a, b: a + b > b + a)

if strings[0] == "0":
    return "0"
    
return "".join(strings)
```

**Key insight.** This comparator defines a total order, and sorting by it gives the lexicographically largest concatenation. Direct string comparison fails (e.g., "3" and "30").

**Complexity.** Time O(N log N * K) where K is the average string length. Space O(N * K).

---

## 14. Minimum Arrows — LeetCode 452

**Approach.** Sort balloons by their end coordinate. If the next balloon starts after the current arrow's coordinate (which is the end of the previous tracked balloon), we need a new arrow.

```text
sort(points by end coordinate)
arrows = 1
arrow_pos = points[0].end

for point in points:
    if point.start > arrow_pos:
        arrows++
        arrow_pos = point.end
        
return arrows
```

**Key insight.** Identical to #5 — burst = overlap, arrow = activity. The greedy choice is to always shoot the arrow at the very end of the current balloon's interval to pop as many subsequent balloons as possible.

**Complexity.** Time O(N log N), Space O(1) or O(log N).

---

## 15. Insert Interval — LeetCode 57

**Approach.** Iterate through the intervals. Add all intervals ending before the new interval starts. Merge the new interval with all overlapping intervals by taking the min of starts and max of ends. Add all remaining intervals.

```text
result = []
i = 0
n = intervals.length

// Phase 1: before
while i < n and intervals[i].end < newInterval.start:
    result.push(intervals[i])
    i++
    
// Phase 2: overlapping (merge)
while i < n and intervals[i].start <= newInterval.end:
    newInterval.start = min(newInterval.start, intervals[i].start)
    newInterval.end = max(newInterval.end, intervals[i].end)
    i++
result.push(newInterval)

// Phase 3: after
while i < n:
    result.push(intervals[i])
    i++
    
return result
```

**Key insight.** Three phases: before, overlapping (merge), after. The intervals are already sorted, so we can do this in a single pass without resorting.

**Complexity.** Time O(N), Space O(N) for the result.

---

# Section 3 — Good to Know

## 16. Remove K Digits — LeetCode 402

**Approach.** Use a monotonic increasing stack. Iterate over the digits, and while `k > 0` and the stack top is greater than the current digit, pop the stack and decrement `k`. After the loop, pop remaining `k` digits from the end. Remove leading zeros.

```text
stack = []
for digit in num:
    while k > 0 and stack is not empty and stack.top() > digit:
        stack.pop()
        k--
    stack.push(digit)

while k > 0:
    stack.pop()
    k--
    
result = join(stack)
remove leading zeros from result
if result is empty: return "0"
return result
```

**Key insight.** Remove peaks (digit larger than the next) first — leftmost peaks matter most. This naturally builds a monotonically increasing sequence which represents the smallest number.

**Complexity.** Time O(N), Space O(N).

---

## 17. Reorganize String — LeetCode 767

**Approach.** Count character frequencies and put them in a max-heap (priority queue). Pop the top two most frequent characters, append them to the result, decrement their frequencies, and push them back if frequency > 0.

```text
counts = hash map of char frequencies
max_heap = max priority queue of (count, char)

result = ""
while max_heap.size >= 2:
    (cnt1, char1) = max_heap.pop()
    (cnt2, char2) = max_heap.pop()
    
    result += char1
    result += char2
    
    if cnt1 - 1 > 0: max_heap.push((cnt1 - 1, char1))
    if cnt2 - 1 > 0: max_heap.push((cnt2 - 1, char2))

if not max_heap.empty():
    (cnt, char) = max_heap.pop()
    if cnt > 1: return ""
    result += char
    
return result
```

**Key insight.** Always place the most frequent character, then the second most, alternating. This prevents adjacent duplicates while burning down the highest counts as fast as possible.

**Complexity.** Time O(N log A) where A is the alphabet size, Space O(A).

---

## 18. Task Scheduler — LeetCode 621

**Approach.** Find the maximum frequency among all tasks. Calculate the minimum intervals needed based on the most frequent task. 

```text
counts = array of 26 zeros
for task in tasks: counts[task - 'A']++
sort(counts)

maxFreq = counts[25]
countOfMaxFreq = number of tasks with maxFreq

result = (maxFreq - 1) * (n + 1) + countOfMaxFreq
return max(result, tasks.length)
```

**Key insight.** The answer is `max((maxFreq-1)*(n+1) + countOfMaxFreq, totalTasks)`. The max frequency dictates the number of "blocks" of time. All other tasks can simply fill the idle slots.

**Complexity.** Time O(N), Space O(1).

---

## 19. Queue Reconstruction — LeetCode 406

**Approach.** Sort the people in descending order of height. If heights are equal, sort in ascending order of `k` value. Then, iterate and insert each person into the result array at index `k`.

```text
sort(people, lambda a, b: a.h != b.h ? a.h > b.h : a.k < b.k)
result = []

for person in people:
    result.insert_at(person.k, person)
    
return result
```

**Key insight.** Sort by height descending, then by `k` ascending. Insert at position `k`. Since we process taller people first, inserting shorter people later won't disrupt the `k` counts of the taller people.

**Complexity.** Time O(N^2) due to insertions, Space O(N).

---

## 20. Optimal Partition — LeetCode 2405

**Approach.** Use a set to track characters seen in the current substring. Iterate through the string; if a character is already in the set, we must partition the string here. Increment the partition count and clear the set.

```text
seen = empty set
partitions = 1

for char in string:
    if char in seen:
        partitions++
        seen.clear()
    seen.add(char)
    
return partitions
```

**Key insight.** Track seen characters with a set, partition when repeat found. The greedy choice is to extend each partition as long as possible until a duplicate is encountered.

**Complexity.** Time O(N), Space O(1) (since alphabet size is bounded to 26).

---

## 21. Minimum Cost to Connect Sticks — LeetCode 1167 / GFG

**Approach.** Put all stick lengths into a min-heap. Extract the two smallest sticks, add their sum to the total cost, and push the merged stick back into the min-heap. Repeat until one stick remains.

```text
min_heap = new MinHeap(sticks)
total_cost = 0

while min_heap.size > 1:
    first = min_heap.pop()
    second = min_heap.pop()
    cost = first + second
    total_cost += cost
    min_heap.push(cost)
    
return total_cost
```

**Key insight.** This IS Huffman coding. By repeatedly merging the two smallest elements, their weights contribute the fewest number of times to the total cost.

**Complexity.** Time O(N log N), Space O(N).

---

## 22. Boats to Save People — LeetCode 881

**Approach.** Sort the weights. Use a two-pointer approach with one pointer at the heaviest person and one at the lightest. If the sum of their weights is <= limit, both can share a boat. Otherwise, the heaviest person must go alone.

```text
sort(people)
left = 0, right = people.length - 1
boats = 0

while left <= right:
    if people[left] + people[right] <= limit:
        left++
    right--
    boats++
    
return boats
```

**Key insight.** If heaviest can't pair with lightest, heaviest goes alone. The greedy pairing maximizes the number of 2-person boats.

**Complexity.** Time O(N log N), Space O(1).

---

# Section 4 — Extra Practice

## 23. Maximum Length of Pair Chain — LeetCode 646

**Approach.** This is identical to the Non-overlapping Intervals and Activity Selection problem. Sort the pairs by their second coordinate (the end value). Always greedily select the pair that ends the earliest, provided it starts after the previous selected pair ends. 

## 24. Minimum Number of Taps to Open to Water a Garden — LeetCode 1326

**Approach.** This is a variation of the Jump Game II problem. Convert each tap's range into an interval `[max(0, i - ranges[i]), min(n, i + ranges[i])]`. From here, you want to find the minimum number of intervals to cover `[0, n]`. Keep track of the farthest reach achievable from the current covered range, similar to BFS levels.

## 25. Partition Labels — LeetCode 763

**Approach.** First pass: record the last occurrence index of each character in a hash map or array. Second pass: iterate through the string and keep expanding the current partition's end to the maximum last occurrence of any character within it. When the current index matches the partition's end, the partition is complete.

## 26. Earliest Possible Day of Full Bloom — LeetCode 2136

**Approach.** The plant time must be done sequentially, but grow times happen in parallel. To minimize the overall time, you should plant the seeds that take the longest to grow first. Sort the indices by `growTime` in descending order, then keep a running sum of `plantTime` and update the maximum bloom day as `current_plant_time + growTime`.

## 27. Maximum Number of Events That Can Be Attended — LeetCode 1353

**Approach.** Sort events by their start day. Use a min-heap to keep track of the end days of all available events on a given day. For each day, remove events from the heap that have already ended. Then, attend the event that ends the earliest (the top of the min-heap) and remove it. Add new events to the heap as their start days arrive.

## 28. Advantage Shuffle — LeetCode 870

**Approach.** Sort both arrays. Use two pointers on the sorted `nums1`. For each element in `nums2`, if the largest available element in `nums1` is greater than it, pair them up. If it's not greater, pair the `nums2` element with the smallest available element in `nums1` (since it's going to lose anyway, sacrifice the worst card).

## 29. Two City Scheduling — LeetCode 1029

**Approach.** The greedy choice relies on opportunity cost. Sort the people based on the difference in cost between sending them to city A versus city B (`costA - costB`). The first `N` people in the sorted list (where the difference is most negative, meaning A is much cheaper than B) go to city A, and the remaining `N` people go to city B.
