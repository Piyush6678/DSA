# 33 — Hard Questions: Solutions

Multi-concept problems. For each problem: the concept combination, the approach, the key insight, and complexity. Full C++ for the 10 hardest/most unique. Pseudocode for 15 more. Brief approach for the rest. Problems that already have full solutions in earlier folders get a cross-reference instead of duplicated code.

---

### Section 1 — DP + Trees

## 1. Binary Tree Maximum Path Sum — LeetCode 124
**Concepts:** DP + Trees
**Approach.** For each node, compute maxGain = node.val + max(0, leftGain) + max(0, rightGain). Update global max. Return node.val + max(0, max(leftGain, rightGain)) for the parent.
```cpp
#include <iostream>
#include <algorithm>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
    int maxSum = -1e9;
public:
    int maxPathSum(TreeNode* root) {
        maxGain(root);
        return maxSum;
    }
    
    int maxGain(TreeNode* node) {
        if (!node) return 0;
        
        int leftGain = max(maxGain(node->left), 0);
        int rightGain = max(maxGain(node->right), 0);
        
        int priceNewpath = node->val + leftGain + rightGain;
        maxSum = max(maxSum, priceNewpath);
        
        return node->val + max(leftGain, rightGain);
    }
};
```
**Key insight.** the return value (single path for parent) is different from the candidate answer (path through this node).
**Complexity.** Time O(N), Space O(H) for call stack.
**Verified:** Initialized with test cases, returns expected max path sum.

## 2. Binary Tree Cameras — LeetCode 968
**Concepts:** DP + Trees
**Approach.** 3-state DFS: 0=needs cover, 1=has camera, 2=covered. If any child needs cover, place camera. If any child has camera, this node is covered. Otherwise, this node needs cover.
```text
function dfs(node):
    if not node: return 2
    left = dfs(node.left)
    right = dfs(node.right)
    if left == 0 or right == 0:
        cameras++
        return 1
    if left == 1 or right == 1:
        return 2
    return 0

if dfs(root) == 0:
    cameras++
return cameras
```
**Key insight.** greedy bottom-up — place cameras at parents of leaves, never at leaves themselves.
**Complexity.** Time O(N), Space O(H).

## 3. Sum of Distances in Tree — LeetCode 834
**Concepts:** DP + Trees
**Approach.** Two DFS passes. First DFS (rooted at 0): compute count[node] (subtree size) and dist[0] (sum of distances from root 0). Second DFS: for each edge parent→child, ans[child] = ans[parent] - count[child] + (n - count[child]).
```text
function dfs1(node, parent):
    for child in graph[node]:
        if child != parent:
            dfs1(child, node)
            count[node] += count[child]
            ans[node] += ans[child] + count[child]

function dfs2(node, parent):
    for child in graph[node]:
        if child != parent:
            ans[child] = ans[node] - count[child] + n - count[child]
            dfs2(child, node)
```
**Key insight.** moving the root from parent to child makes count[child] nodes 1 closer and (n - count[child]) nodes 1 farther.
**Complexity.** Time O(N), Space O(N).

## 4. House Robber III — LeetCode 337
**Concepts:** DP + Trees
**Approach.** DFS returns (rob_this, skip_this). See `../26_dp/solution.md`.
**Key insight.** each node makes one binary choice, and the tree structure means children's choices are independent.
**Complexity.** Time O(N), Space O(H).

## 5. Distribute Coins — LeetCode 979
**Concepts:** DP + Trees
**Approach.** DFS returns excess coins at each subtree.
**Key insight.** |excess| at each edge = moves needed across that edge. Total moves = sum of |excess| over all edges.
**Complexity.** Time O(N), Space O(H).

---

### Section 2 — DP + Graphs

## 6. Shortest Path Visiting All Nodes — LeetCode 847
**Concepts:** BFS + Bitmask
**Approach.** BFS with state = (current_node, visited_bitmask). Start: all (i, 1<<i) with distance 0. End: any state with visited = (1<<n)-1.
```cpp
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

class Solution {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        int n = graph.size();
        if (n == 1) return 0;
        
        queue<tuple<int, int, int>> q;
        vector<vector<bool>> visited(n, vector<bool>(1 << n, false));
        
        for (int i = 0; i < n; i++) {
            q.push({i, 1 << i, 0});
            visited[i][1 << i] = true;
        }
        
        int target = (1 << n) - 1;
        
        while (!q.empty()) {
            auto [node, mask, dist] = q.front();
            q.pop();
            
            if (mask == target) return dist;
            
            for (int neighbor : graph[node]) {
                int nextMask = mask | (1 << neighbor);
                if (!visited[neighbor][nextMask]) {
                    visited[neighbor][nextMask] = true;
                    q.push({neighbor, nextMask, dist + 1});
                }
            }
        }
        return -1;
    }
};
```
**Key insight.** this is BFS on a state-space graph, not the original graph. Each state encodes position AND history.
**Complexity.** Time O(N * 2^N), Space O(N * 2^N).
**Verified:** Graph with cycle returns shortest distance to touch every node.

## 7. Longest Increasing Path in Matrix — LeetCode 329
**Concepts:** DP + Graphs
**Approach.** DFS with memoization. For each cell, recurse on neighbors with strictly greater value. memo[i][j] = 1 + max(valid neighbors).
```text
function dfs(r, c):
    if memo[r][c] != 0: return memo[r][c]
    ans = 1
    for nr, nc in neighbors(r, c):
        if in_bounds(nr, nc) and matrix[nr][nc] > matrix[r][c]:
            ans = max(ans, 1 + dfs(nr, nc))
    memo[r][c] = ans
    return ans

return max(dfs(r, c) for r, c in matrix)
```
**Key insight.** the strictly increasing constraint makes the implicit graph a DAG — no cycles possible, so simple memoization works without 'visited' tracking.
**Complexity.** Time O(M*N), Space O(M*N).

## 8. Parallel Courses III — LeetCode 2050
**Concepts:** Topological Sort
**Approach.** Topological sort (Kahn's). dp[node] = time[node] + max(dp[predecessor] for all predecessors). Process in topological order.
```text
indegree = array(n, 0)
graph = build_graph(relations)
dp = array(n, 0)
queue = []

for node in 0..n-1:
    if indegree[node] == 0:
        queue.push(node)
        dp[node] = time[node]

while not queue.empty():
    node = queue.pop()
    for neighbor in graph[node]:
        dp[neighbor] = max(dp[neighbor], dp[node] + time[neighbor])
        indegree[neighbor]--
        if indegree[neighbor] == 0:
            queue.push(neighbor)

return max(dp)
```
**Key insight.** this is the critical path method — each task starts after ALL prerequisites finish, so the longest path determines total time.
**Complexity.** Time O(V + E), Space O(V + E).

## 9. Number of Restricted Paths — LeetCode 1786
**Concepts:** DP + Graphs
**Approach.** Dijkstra from node n to compute dist[]. Then count paths from 1 to n on the DAG defined by dist[u] > dist[v].
**Key insight.** Dijkstra gives distances, then the restricted path condition defines a DAG — count paths with DP.
**Complexity.** Time O(E log V), Space O(V + E).

## 10. Frog Jump — LeetCode 403
**Concepts:** DP + Graphs
**Approach.** DP with state (stone_position, last_jump). For each stone, try jumps of size k-1, k, k+1. Use hash set for O(1) stone lookup.
**Key insight.** the state space is sparse — use a map from position to set of reachable jump sizes.
**Complexity.** Time O(N^2), Space O(N^2).

---

### Section 3 — DP + Strings

## 11. Regular Expression Matching — LeetCode 10
**Concepts:** DP + Strings
**Approach.** dp[i][j] = does s[0..i-1] match p[0..j-1]. If p[j-1] is '.': dp[i][j] = dp[i-1][j-1]. If p[j-1] is '*': either use zero occurrences (dp[i][j-2]) or use one more if p[j-2] matches s[i-1] (dp[i-1][j]).
```cpp
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    bool isMatch(string s, string p) {
        int m = s.length(), n = p.length();
        vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
        dp[0][0] = true;
        
        for (int j = 2; j <= n; j++) {
            if (p[j - 1] == '*') {
                dp[0][j] = dp[0][j - 2];
            }
        }
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (p[j - 1] == '.' || p[j - 1] == s[i - 1]) {
                    dp[i][j] = dp[i - 1][j - 1];
                } else if (p[j - 1] == '*') {
                    dp[i][j] = dp[i][j - 2]; 
                    if (p[j - 2] == '.' || p[j - 2] == s[i - 1]) {
                        dp[i][j] = dp[i][j] || dp[i - 1][j]; 
                    }
                }
            }
        }
        
        return dp[m][n];
    }
};
```
**Key insight.** '*' means 'zero or more of the PRECEDING element' — it's p[j-2] that matters, not p[j-1].
**Complexity.** Time O(M*N), Space O(M*N).
**Verified:** "aa" vs "a*" returns true.

## 12. Wildcard Matching — LeetCode 44
**Concepts:** DP + Strings
**Approach.** dp[i][j] = s[0..i-1] matches p[0..j-1]. '*' matches ANY sequence (unlike regex where '*' depends on preceding char). dp[i][j] = dp[i][j-1] (empty match) OR dp[i-1][j] (extend match).
```text
dp = matrix(m+1, n+1, false)
dp[0][0] = true
for j in 1..n:
    if p[j-1] == '*': dp[0][j] = dp[0][j-1]

for i in 1..m:
    for j in 1..n:
        if p[j-1] == s[i-1] or p[j-1] == '?':
            dp[i][j] = dp[i-1][j-1]
        else if p[j-1] == '*':
            dp[i][j] = dp[i][j-1] or dp[i-1][j]
return dp[m][n]
```
**Key insight.** in wildcard, '*' is independent — it matches any substring. In regex, '*' is dependent on p[j-2].
**Complexity.** Time O(M*N), Space O(M*N).

## 13. Distinct Subsequences — LeetCode 115
**Concepts:** DP + Strings
**Approach.** dp[i][j] = number of subsequences of s[0..i-1] equal to t[0..j-1]. If s[i-1]==t[j-1]: dp[i][j] = dp[i-1][j-1] + dp[i-1][j]. Else: dp[i][j] = dp[i-1][j].
**Key insight.** when characters match, you either USE this character (dp[i-1][j-1]) or SKIP it (dp[i-1][j]).
**Complexity.** Time O(M*N), Space O(M*N).

## 14. Shortest Common Supersequence — LeetCode 1092
**Concepts:** DP + Strings
**Approach.** Compute LCS, then reconstruct by interleaving characters not in LCS.
**Key insight.** SCS length = |s| + |t| - |LCS|. The hard part is PRINTING the actual string, not computing the length.
**Complexity.** Time O(M*N), Space O(M*N).

## 15. Count Different Palindromic Subsequences — LeetCode 730
**Concepts:** DP + Strings
**Approach.** Interval DP with tracking of first/last occurrence of each character.
**Key insight.** for each interval, consider palindromic subsequences starting/ending with each character a-d separately to avoid double counting.
**Complexity.** Time O(N^2), Space O(N^2).

---

### Section 4 — DP + Binary Search / Math

## 16. Super Egg Drop — LeetCode 887
**Concepts:** DP + Math
**Approach.** Reframe: dp[m][k] = max floors testable with m moves and k eggs. dp[m][k] = dp[m-1][k-1] + dp[m-1][k] + 1. Answer: smallest m such that dp[m][k] >= n.
```cpp
#include <vector>

using namespace std;

class Solution {
public:
    int superEggDrop(int k, int n) {
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        int m = 0;
        while (dp[m][k] < n) {
            m++;
            for (int j = 1; j <= k; j++) {
                dp[m][j] = dp[m - 1][j - 1] + dp[m - 1][j] + 1;
            }
        }
        return m;
    }
};
```
**Key insight.** the reframing from 'minimum moves for n floors' to 'maximum floors for m moves' turns O(kn²) into O(kn) or even O(k log n).
**Complexity.** Time O(K log N), Space O(K log N).
**Verified:** k=1, n=2 returns 2.

## 17. Russian Doll Envelopes — LeetCode 354
**Concepts:** Binary Search + LIS
**Approach.** Sort by width ascending, then by height DESCENDING (for same width). Apply LIS on heights using O(n log n) binary search.
```text
sort envelopes: 
    if a.w == b.w: return a.h > b.h
    return a.w < b.w

dp = empty list
for e in envelopes:
    idx = binary_search_lower_bound(dp, e.h)
    if idx == dp.size():
        dp.append(e.h)
    else:
        dp[idx] = e.h

return dp.size()
```
**Key insight.** sorting by height descending for same width prevents two envelopes of equal width from being nested.
**Complexity.** Time O(N log N), Space O(N).

## 18. Split Array Largest Sum — LeetCode 410
**Concepts:** Binary Search
**Approach.** Binary search on the answer (the maximum subarray sum). For each candidate, greedily check if you can split into ≤ k subarrays with each ≤ candidate.
```text
left = max(nums), right = sum(nums)
while left < right:
    mid = left + (right - left) / 2
    splits = 1, current_sum = 0
    for x in nums:
        if current_sum + x > mid:
            splits++
            current_sum = x
        else:
            current_sum += x
    if splits > k:
        left = mid + 1
    else:
        right = mid

return left
```
**Key insight.** the answer is monotonic — if you can split with max=X, you can split with max=X+1.
**Complexity.** Time O(N log(Sum - Max)), Space O(1).

## 19. Dungeon Game — LeetCode 174
**Concepts:** DP
**Approach.** dp[i][j] = minimum HP needed to reach bottom-right from (i,j). Fill from bottom-right to top-left. dp[i][j] = max(1, min(dp[i+1][j], dp[i][j+1]) - dungeon[i][j]).
**Key insight.** you MUST process backward because the constraint is 'HP > 0 at every step', which future cells affect.
**Complexity.** Time O(M*N), Space O(M*N).

## 20. Burst Balloons — LeetCode 312
**Concepts:** Interval DP
**Approach.** Interval DP. dp[l][r] = max coins from bursting all balloons in (l, r) exclusive. For each k in (l+1, r-1), dp[l][r] = max(dp[l][k] + dp[k][r] + nums[l]*nums[k]*nums[r]).
```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxCoins(vector<int>& nums) {
        int n = nums.size();
        vector<int> A(n + 2, 1);
        for (int i = 0; i < n; i++) A[i + 1] = nums[i];
        
        vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
        
        for (int len = 2; len <= n + 1; len++) {
            for (int l = 0; l + len <= n + 1; l++) {
                int r = l + len;
                for (int k = l + 1; k < r; k++) {
                    dp[l][r] = max(dp[l][r], dp[l][k] + dp[k][r] + A[l] * A[k] * A[r]);
                }
            }
        }
        
        return dp[0][n + 1];
    }
};
```
**Key insight.** think about which balloon to burst LAST, not first.
**Complexity.** Time O(N^3), Space O(N^2).
**Verified:** [3, 1, 5, 8] returns 167.

---

### Section 5 — DP + Bitmask

## 21. Shortest Superstring — LeetCode 943
**Concepts:** TSP + Bitmask
**Approach.** Precompute overlap[i][j] = max overlap when word i is followed by word j. TSP with bitmask DP: dp[mask][i] = min length superstring using words in mask, ending with word i. Reconstruct path.
```cpp
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    string shortestSuperstring(vector<string>& words) {
        int n = words.size();
        vector<vector<int>> overlap(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    for (int k = min(words[i].size(), words[j].size()); k > 0; k--) {
                        if (words[i].substr(words[i].size() - k) == words[j].substr(0, k)) {
                            overlap[i][j] = k;
                            break;
                        }
                    }
                }
            }
        }
        
        vector<vector<int>> dp(1 << n, vector<int>(n, 1e9));
        vector<vector<int>> parent(1 << n, vector<int>(n, -1));
        
        for (int i = 0; i < n; i++) dp[1 << i][i] = words[i].size();
        
        for (int mask = 1; mask < (1 << n); mask++) {
            for (int bit = 0; bit < n; bit++) {
                if ((mask & (1 << bit)) == 0) continue;
                int pmask = mask ^ (1 << bit);
                for (int pbit = 0; pbit < n; pbit++) {
                    if ((pmask & (1 << pbit)) == 0) continue;
                    int val = dp[pmask][pbit] + words[bit].size() - overlap[pbit][bit];
                    if (val < dp[mask][bit]) {
                        dp[mask][bit] = val;
                        parent[mask][bit] = pbit;
                    }
                }
            }
        }
        
        int min_len = 1e9, last = -1;
        for (int i = 0; i < n; i++) {
            if (dp[(1 << n) - 1][i] < min_len) {
                min_len = dp[(1 << n) - 1][i];
                last = i;
            }
        }
        
        int curr_mask = (1 << n) - 1;
        vector<int> path;
        while (last != -1) {
            path.push_back(last);
            int temp = parent[curr_mask][last];
            curr_mask ^= (1 << last);
            last = temp;
        }
        reverse(path.begin(), path.end());
        
        string res = words[path[0]];
        for (int i = 1; i < path.size(); i++) {
            res += words[path[i]].substr(overlap[path[i-1]][path[i]]);
        }
        return res;
    }
};
```
**Key insight.** this IS the Travelling Salesman Problem on a complete graph where edge weight = word_j.length - overlap[i][j].
**Complexity.** Time O(N^2 * 2^N + N^2 * L), Space O(N * 2^N).
**Verified:** Path reconstructed successfully.

## 22. Maximum Students Taking Exam — LeetCode 1349
**Concepts:** DP + Bitmask
**Approach.** For each row, enumerate valid seatings as bitmasks (no adjacent 1s, no broken seats). DP: dp[row][mask] = max students for rows 0..row with current row having seating mask. Check compatibility with previous row's mask (no diagonal cheating).
```text
for row in 0..m-1:
    valid_masks = []
    for mask in 0..(1<<n)-1:
        if (mask & (mask >> 1)) == 0 and (mask & broken_seats[row]) == 0:
            valid_masks.append(mask)
    
    for mask in valid_masks:
        for prev_mask in valid_masks_prev:
            if (mask & (prev_mask >> 1)) == 0 and (mask & (prev_mask << 1)) == 0:
                dp[row][mask] = max(dp[row][mask], dp[row-1][prev_mask] + count_bits(mask))
```
**Key insight.** small column count (≤ 8) makes bitmask enumeration feasible.
**Complexity.** Time O(M * 3^N), Space O(M * 2^N).

## 23. Ways to Wear Different Hats — LeetCode 1434
**Concepts:** DP + Bitmask
**Approach.** n people (≤ 10), 40 hats. Bitmask on PEOPLE (not hats — 2^10 << 2^40). For each hat, either assign it to an eligible unassigned person or skip it.
**Key insight.** mask the smaller dimension. n ≤ 10 → bitmask people. If hats ≤ 10 → bitmask hats.
**Complexity.** Time O(H * 2^N), Space O(2^N).

## 24. Min Cost Connect Two Groups — LeetCode 1595
**Concepts:** DP + Bitmask
**Approach.** dp[i][mask] where i is index in group1, mask is subset of group2 already connected.
**Key insight.** after processing all group1 points, any unconnected group2 points must be connected to their cheapest group1 option.
**Complexity.** Time O(N * 2^M), Space O(N * 2^M).

---

### Section 6 — Segment Tree + Other

## 25. Count of Smaller Numbers After Self — LeetCode 315
**Concepts:** Segment Tree
**Approach.** See `../30_segment_tree/solution.md` #10.
**Key insight.** Seg tree as frequency array.
**Complexity.** Time O(N log N), Space O(N).

## 26. LIS II — LeetCode 2407
**Concepts:** Segment Tree
**Approach.** See `../30_segment_tree/solution.md` #20.
**Key insight.** Range max query for DP acceleration.
**Complexity.** Time O(N log M), Space O(M).

## 27. Rectangle Area II — LeetCode 850
**Concepts:** Segment Tree + Line Sweep
**Approach.** Line sweep on x-coordinates + segment tree on y-intervals.
**Key insight.** the segment tree maintains total covered y-length; multiply by delta-x as sweep advances.
**Complexity.** Time O(N log N), Space O(N).

## 28. Falling Squares — LeetCode 699
**Concepts:** Segment Tree
**Approach.** Coordinate compress x-intervals. Segment tree with range max query + range assignment.
**Key insight.** each square lands on top of the tallest existing structure in its x-range.
**Complexity.** Time O(N log N), Space O(N).

---

### Section 7 — Graphs + Advanced

## 29. Critical Connections — LeetCode 1192
**Concepts:** Graphs + DFS
**Approach.** Tarjan's bridge algorithm. DFS with tin[] (discovery time) and low[] (lowest reachable time). Edge (u,v) is bridge if low[v] > tin[u].
```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    int timer = 0;
    void dfs(int u, int p, vector<vector<int>>& adj, vector<int>& tin, vector<int>& low, vector<vector<int>>& bridges) {
        tin[u] = low[u] = ++timer;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                dfs(v, u, adj, tin, low, bridges);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) {
                    bridges.push_back({u, v});
                }
            }
        }
    }
public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        vector<vector<int>> adj(n);
        for (auto& edge : connections) {
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }
        vector<int> tin(n, 0), low(n, 0);
        vector<vector<int>> bridges;
        for (int i = 0; i < n; i++) {
            if (!tin[i]) dfs(i, -1, adj, tin, low, bridges);
        }
        return bridges;
    }
};
```
**Key insight.** low[v] > tin[u] means v cannot reach any ancestor of u without using edge (u,v) — removing it disconnects the graph.
**Complexity.** Time O(V + E), Space O(V + E).
**Verified:** Valid bridge successfully found.

## 30. Swim in Rising Water — LeetCode 778
**Concepts:** Graphs + Dijkstra
**Approach.** Dijkstra on grid. State = (max elevation on path to (i,j)). Priority queue ordered by this max.
```text
pq = min_heap()
pq.push((grid[0][0], 0, 0))
visited[0][0] = true

while not pq.empty():
    max_elev, r, c = pq.pop()
    if r == n-1 and c == n-1: return max_elev
    
    for nr, nc in neighbors(r, c):
        if not visited[nr][nc]:
            visited[nr][nc] = true
            pq.push((max(max_elev, grid[nr][nc]), nr, nc))
```
**Key insight.** this is Dijkstra where the 'distance' is the MAXIMUM edge weight on the path, not the sum.
**Complexity.** Time O(N^2 log N), Space O(N^2).

## 31. Word Ladder II — LeetCode 126
**Concepts:** Graphs
**Approach.** BFS to compute shortest distances from beginWord. DFS backward from endWord following decreasing distances.
**Key insight.** BFS forward for distances, DFS backward for all shortest paths. Don't enumerate all paths during BFS — it explodes.
**Complexity.** Time O(V + E + Paths), Space O(V + E).

## 32. Min Cost Valid Path — LeetCode 1368
**Concepts:** Graphs + 0-1 BFS
**Approach.** 0-1 BFS. Moving in the arrow's direction costs 0, other directions cost 1. Use deque: push_front for cost 0, push_back for cost 1.
**Key insight.** 0-1 BFS is Dijkstra's special case for binary weights — deque replaces priority queue.
**Complexity.** Time O(V + E), Space O(V + E).

## 33. Skyline Problem — LeetCode 218
**Concepts:** Line Sweep
**Approach.** Events: building start (add height), building end (remove height). Sweep left to right. Maintain max-heap or multiset of active heights. When the max height changes, emit a key point.
```text
events = []
for l, r, h in buildings:
    events.append((l, -h)) // start: negative height
    events.append((r, h))  // end: positive height

sort(events)
active_heights = multiset()
active_heights.insert(0)
prev_max = 0

for x, h in events:
    if h < 0:
        active_heights.insert(-h)
    else:
        active_heights.erase(active_heights.find(h))
    
    curr_max = *active_heights.rbegin()
    if curr_max != prev_max:
        result.append([x, curr_max])
        prev_max = curr_max
```
**Key insight.** the output is the set of points where the maximum active height changes.
**Complexity.** Time O(N log N), Space O(N).

## 34. Alien Dictionary — LeetCode 269
**Concepts:** Topological Sort
**Approach.** Build directed graph from adjacent word comparisons. Topological sort.
**Key insight.** compare adjacent words char by char — the first difference gives one edge. But if a longer word appears before its prefix, the input is INVALID.
**Complexity.** Time O(C), Space O(1).

---

### Section 8 — Stack/Heap + Complex

## 35. Largest Rectangle in Histogram — LeetCode 84
**Concepts:** Stack
**Approach.** Monotonic stack. For each bar, find the nearest shorter bar on left and right. Width = right_boundary - left_boundary - 1. Area = height * width.
```cpp
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        stack<int> st;
        int max_area = 0;
        int n = heights.size();
        
        for (int i = 0; i <= n; i++) {
            int h = (i == n) ? 0 : heights[i];
            while (!st.empty() && h < heights[st.top()]) {
                int height = heights[st.top()];
                st.pop();
                int width = st.empty() ? i : i - st.top() - 1;
                max_area = max(max_area, height * width);
            }
            st.push(i);
        }
        
        return max_area;
    }
};
```
**Key insight.** the stack maintains bars in increasing order. When a bar is popped, the current bar is its right boundary and the new stack top is its left boundary.
**Complexity.** Time O(N), Space O(N).
**Verified:** [2,1,5,6,2,3] gives 10.

## 36. Maximal Rectangle — LeetCode 85
**Concepts:** Stack + DP
**Approach.** For each row, compute histogram heights (dp[j] = heights of consecutive 1s ending at row i). Apply #35 to each row's histogram.
```text
heights = array(cols, 0)
max_area = 0
for row in matrix:
    for j in 0..cols-1:
        if row[j] == '1': heights[j]++
        else: heights[j] = 0
    max_area = max(max_area, largestRectangleArea(heights))
return max_area
```
**Key insight.** convert 2D problem to n applications of the 1D histogram problem.
**Complexity.** Time O(M*N), Space O(N).

## 37. Trapping Rain Water II — LeetCode 407
**Concepts:** Heap + BFS
**Approach.** BFS from all boundary cells using min-heap. Process cells in order of height. For each unvisited neighbor, water trapped = max(0, current_boundary_height - neighbor_height).
```text
pq = min_heap()
push all boundary cells to pq, mark visited
max_height = 0
water = 0

while not pq.empty():
    h, r, c = pq.pop()
    max_height = max(max_height, h)
    for nr, nc in neighbors(r, c):
        if not visited[nr][nc]:
            visited[nr][nc] = true
            if grid[nr][nc] < max_height:
                water += max_height - grid[nr][nc]
            pq.push((grid[nr][nc], nr, nc))
return water
```
**Key insight.** the boundary shrinks inward — the min-heap always processes the lowest boundary cell first, just like water flows over the lowest point.
**Complexity.** Time O(M*N log(M*N)), Space O(M*N).

## 38. Sliding Window Median — LeetCode 480
**Concepts:** Heap
**Approach.** Two multisets: small (max-heap) and large (min-heap). Slide window: add new element, remove outgoing, rebalance.
```text
small = multiset(max_heap_style)
large = multiset(min_heap_style)

function insert(val):
    small.insert(val)
    large.insert(*small.rbegin())
    small.erase(prev(small.end()))
    if small.size() < large.size():
        small.insert(*large.begin())
        large.erase(large.begin())

function remove(val):
    if val <= *small.rbegin():
        small.erase(small.find(val))
    else:
        large.erase(large.find(val))
    rebalance()
```
**Key insight.** lazy deletion — don't erase from the wrong heap immediately; track balance and fix when the invalid element reaches the top.
**Complexity.** Time O(N log K), Space O(K).

---

### Section 9 — Trie/String + Other

## 39. Word Search II — LeetCode 212
**Concepts:** Trie + DFS
**Approach.** See `../32_trie/solution.md` #6.
**Key insight.** Prune the trie when words are found to prevent redundant paths.
**Complexity.** Time O(M*N*3^L), Space O(W).

## 40. Palindrome Pairs — LeetCode 336
**Concepts:** Trie + Strings
**Approach.** Build trie of reversed words. For each word, walk the trie. At each isEnd, check if remaining suffix is palindrome.
**Key insight.** A+B is palindrome if reverse(B) is a prefix of A and the remainder of A is itself a palindrome.
**Complexity.** Time O(N * L^2), Space O(N * L).

## 41. Stream of Characters — LeetCode 1032
**Concepts:** Trie
**Approach.** See `../32_trie/solution.md` #14.
**Key insight.** Query backwards from the last added character using a reversed trie.
**Complexity.** Time O(N*L), Space O(N*L).

## 42. Concatenated Words — LeetCode 472
**Concepts:** Trie + DP
**Approach.** Sort words by length. For each word, check if it can be formed by concatenating shorter words (word break DP using trie).
**Key insight.** sorting by length ensures all component words are already in the trie when checking a longer word.
**Complexity.** Time O(N log N + N*L^2), Space O(N*L).

---

### Section 10 — Multi-Concept / Quant

## 43. Median of Two Sorted Arrays — LeetCode 4
**Concepts:** Binary Search
**Approach.** Binary search on partition of the shorter array. Partition both arrays such that left half has (m+n+1)/2 elements. Valid if maxLeftA <= minRightB and maxLeftB <= minRightA.
```cpp
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1);
        
        int x = nums1.size(), y = nums2.size();
        int low = 0, high = x;
        
        while (low <= high) {
            int partitionX = (low + high) / 2;
            int partitionY = (x + y + 1) / 2 - partitionX;
            
            int maxLeftX = (partitionX == 0) ? INT_MIN : nums1[partitionX - 1];
            int minRightX = (partitionX == x) ? INT_MAX : nums1[partitionX];
            
            int maxLeftY = (partitionY == 0) ? INT_MIN : nums2[partitionY - 1];
            int minRightY = (partitionY == y) ? INT_MAX : nums2[partitionY];
            
            if (maxLeftX <= minRightY && maxLeftY <= minRightX) {
                if ((x + y) % 2 == 0) {
                    return ((double)max(maxLeftX, maxLeftY) + min(minRightX, minRightY)) / 2;
                } else {
                    return max(maxLeftX, maxLeftY);
                }
            } else if (maxLeftX > minRightY) {
                high = partitionX - 1;
            } else {
                low = partitionX + 1;
            }
        }
        return 0.0;
    }
};
```
**Key insight.** binary search on the shorter array. The partition of the longer array is determined by the shorter's partition.
**Complexity.** Time O(log(min(M,N))), Space O(1).
**Verified:** O(log(min(m, n))) expected approach, perfectly partitioned.

## 44. Cherry Pickup — LeetCode 741
**Concepts:** DP
**Approach.** Reframe as two people walking simultaneously from (0,0) to (n-1,n-1). State: dp[r1][c1][r2] where c2 = r1+c1-r2. If both on same cell, count cherry once.
```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    vector<vector<vector<int>>> memo;
    int dfs(vector<vector<int>>& grid, int r1, int c1, int r2, int n) {
        int c2 = r1 + c1 - r2;
        if (r1 >= n || c1 >= n || r2 >= n || c2 >= n || grid[r1][c1] == -1 || grid[r2][c2] == -1) {
            return -1e9;
        }
        if (r1 == n - 1 && c1 == n - 1) return grid[r1][c1];
        if (memo[r1][c1][r2] != -1) return memo[r1][c1][r2];
        
        int ans = grid[r1][c1];
        if (r1 != r2) ans += grid[r2][c2];
        
        int nxt = max({
            dfs(grid, r1 + 1, c1, r2 + 1, n),
            dfs(grid, r1, c1 + 1, r2, n),
            dfs(grid, r1 + 1, c1, r2, n),
            dfs(grid, r1, c1 + 1, r2 + 1, n)
        });
        
        return memo[r1][c1][r2] = ans + nxt;
    }
public:
    int cherryPickup(vector<vector<int>>& grid) {
        int n = grid.size();
        memo.assign(n, vector<vector<int>>(n, vector<int>(n, -1)));
        return max(0, dfs(grid, 0, 0, 0, n));
    }
};
```
**Key insight.** 'go and return' = 'two agents going forward simultaneously'. Reduces from 4D to 3D.
**Complexity.** Time O(N^3), Space O(N^3).
**Verified:** Yes.

## 45. N-Queens — LeetCode 51
**Concepts:** Backtracking
**Approach.** Backtracking with column, diagonal, anti-diagonal tracking.
**Key insight.** row+col is constant on each anti-diagonal, row-col is constant on each diagonal. Three sets replace the O(n) board scan.
**Complexity.** Time O(N!), Space O(N).

## 46. Smallest Range from K Lists — LeetCode 632
**Concepts:** Heap
**Approach.** Min-heap with one element from each list. Track current max. Range = [heap_top, current_max]. Advance the minimum element's list.
```text
pq = min_heap_of_tuples(value, list_idx, elem_idx)
current_max = -infinity
for i in 0..k-1:
    pq.push((lists[i][0], i, 0))
    current_max = max(current_max, lists[i][0])

best_range = [-infinity, infinity]

while pq.size() == k:
    val, list_idx, elem_idx = pq.pop()
    if current_max - val < best_range[1] - best_range[0]:
        best_range = [val, current_max]
    
    if elem_idx + 1 < lists[list_idx].size():
        next_val = lists[list_idx][elem_idx + 1]
        pq.push((next_val, list_idx, elem_idx + 1))
        current_max = max(current_max, next_val)
```
**Key insight.** the min-heap ensures you always know the current minimum, and tracking current_max gives you both ends of the range.
**Complexity.** Time O(N log K), Space O(K).

## 47. Max Points on a Line — LeetCode 149
**Concepts:** Math
**Approach.** For each point, compute slope to every other point using GCD-reduced (dy, dx) pairs. Group by slope. Max group size + 1 = answer.
**Key insight.** use GCD to represent slopes as (dy/gcd, dx/gcd) integers to avoid floating-point precision issues.
**Complexity.** Time O(N^2), Space O(N).

## 48. Poor Pigs — LeetCode 458
**Concepts:** Math
**Approach.** Answer = ceil(log(buckets) / log(rounds + 1)).
**Key insight.** each pig has (rounds+1) possible states (die in round 1, 2, ..., or survive). With p pigs, you can distinguish (rounds+1)^p outcomes.
**Complexity.** Time O(1), Space O(1).

## 49. Min Refueling Stops — LeetCode 871
**Concepts:** Greedy + Heap
**Approach.** Greedy: drive as far as possible. When stuck, refuel from the station you passed with the most fuel (max-heap).
**Key insight.** past stations are like 'options' — the max-heap stores your future refueling choices.
**Complexity.** Time O(N log N), Space O(N).

## 50. Min Cost Hire K Workers — LeetCode 857
**Concepts:** Greedy + Heap
**Approach.** Sort by wage/quality ratio. Iterate; maintain min-heap of K smallest qualities. Cost = ratio * sum_of_qualities.
```text
workers = sorted pairs of (wage/quality, quality)
quality_sum = 0
pq = max_heap()
min_cost = infinity

for ratio, quality in workers:
    quality_sum += quality
    pq.push(quality)
    if pq.size() > K:
        quality_sum -= pq.pop()
    if pq.size() == K:
        min_cost = min(min_cost, quality_sum * ratio)

return min_cost
```
**Key insight.** if we pay everyone at the current worker's ratio (the highest so far), we want the K smallest qualities to minimise total cost.
**Complexity.** Time O(N log N + N log K), Space O(N).
