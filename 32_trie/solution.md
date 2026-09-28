# 32 — Tries: Solutions

Approach, pseudocode, complexity and key insight. Real C++ appears only for the **fundamentals** (implementations), the **tricks**, and the **hard** ones.

**Everything shown as code was compiled with `g++ -std=gnu++14` and executed.**

Assume `struct TrieNode { TrieNode* children[26]; bool isEndOfWord; };`.

---

# Section 1 — Must Do

## 1. Implement Trie (Prefix Tree) — LeetCode 208 — **fundamental, code given**

**Approach.** We need a complete class with insert, search, and startsWith methods.

```cpp
class Trie {
    struct TrieNode {
        TrieNode* children[26];
        bool isEndOfWord;
        TrieNode() {
            for (int i = 0; i < 26; i++) {
                children[i] = nullptr;
            }
            isEndOfWord = false;
        }
    };
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }
    
    void insert(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
        }
        node->isEndOfWord = true;
    }
    
    bool search(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                return false;
            }
            node = node->children[idx];
        }
        return node->isEndOfWord;
    }
    
    bool startsWith(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                return false;
            }
            node = node->children[idx];
        }
        return true;
    }
};
```

**Key insight.** `startsWith` is exactly `search` without checking `isEndOfWord` at the end — this one-line difference IS the reason tries exist. It lets us check prefix existence in O(L) instead of scanning the whole dictionary.

**Complexity.** Insert: O(L) time, Search: O(L) time, StartsWith: O(L) time. Space: O(N * L) where N is number of words and L is length, worst case.

---

## 2. Implement Trie-II (counts)

**Approach.** Add `countEndsWith` and `countPrefix` integers to each node.

```
insert(word):
    node = root
    for c in word:
        idx = c - 'a'
        if not node.children[idx]:
            node.children[idx] = new TrieNode()
        node = node.children[idx]
        node.countPrefix++
    node.countEndsWith++

countWordsEqualTo(word):
    node = search_helper(word)
    return node ? node.countEndsWith : 0

countWordsStartingWith(prefix):
    node = search_helper(prefix)
    return node ? node.countPrefix : 0

erase(word):
    node = root
    for c in word:
        node = node.children[c - 'a']
        node.countPrefix--
    node.countEndsWith--
```

**Key insight.** `countPrefix` increments on insert for every node on the path; `countEndsWith` increments only at the terminal node. When deleting, we simply decrement the counters along the path.

**Complexity.** O(L) time for all operations.

---

## 3. Longest Common Prefix — LeetCode 14

**Approach.** Insert all words into the trie, then walk from the root following single-child paths until a branch or the end of a word is reached.

```
insert all words into Trie
node = root
prefix = ""
while node has exactly 1 child AND not node.isEndOfWord:
    prefix += that_one_child_char
    node = that_one_child
return prefix
```

**Key insight.** The trie approach is O(S) where S = sum of all characters, same as a vertical scan — the trie is not faster here but teaches the structure. The longest common prefix is the longest path from the root without branching and without hitting a complete word.

**Complexity.** O(S) time where S is the sum of characters of all strings. O(S) space.

---

## 4. Design Add and Search Words Data Structure — LeetCode 211 — **trick, code given**

**Approach.** The '.' wildcard means you must try ALL 26 children at that position via DFS/backtracking.

```cpp
class WordDictionary {
    struct TrieNode {
        TrieNode* children[26];
        bool isEndOfWord;
        TrieNode() {
            for (int i = 0; i < 26; i++) children[i] = nullptr;
            isEndOfWord = false;
        }
    };
    TrieNode* root;

    bool dfs(string& word, int idx, TrieNode* node) {
        if (idx == word.length()) {
            return node->isEndOfWord;
        }
        
        char c = word[idx];
        if (c == '.') {
            for (int i = 0; i < 26; i++) {
                if (node->children[i] && dfs(word, idx + 1, node->children[i])) {
                    return true;
                }
            }
            return false;
        } else {
            if (!node->children[c - 'a']) return false;
            return dfs(word, idx + 1, node->children[c - 'a']);
        }
    }

public:
    WordDictionary() {
        root = new TrieNode();
    }
    
    void addWord(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int idx = c - 'a';
            if (!node->children[idx]) node->children[idx] = new TrieNode();
            node = node->children[idx];
        }
        node->isEndOfWord = true;
    }
    
    bool search(string word) {
        return dfs(word, 0, root);
    }
};
```

**Key insight.** Without '.', this is just problem #1. The wildcard forces branching, and the worst case is O(26^L) — but in practice the trie prunes aggressively because many children are null.

**Complexity.** Add: O(L) time. Search: O(L) for normal word, O(26^L) worst case for all wildcards.

---

## 5. Maximum XOR of Two Numbers in an Array — LeetCode 421 — **implementation, code given**

**Approach.** Build a bit-trie from MSB to LSB (32 levels). For each number, greedily choose the opposite bit.

```cpp
class Solution {
    struct BitNode {
        BitNode* left;  // represents bit 0
        BitNode* right; // represents bit 1
        BitNode() : left(nullptr), right(nullptr) {}
    };

    void insert(BitNode* root, int num) {
        BitNode* node = root;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (bit == 0) {
                if (!node->left) node->left = new BitNode();
                node = node->left;
            } else {
                if (!node->right) node->right = new BitNode();
                node = node->right;
            }
        }
    }

    int getMaxXor(BitNode* root, int num) {
        BitNode* node = root;
        int maxXor = 0;
        for (int i = 31; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (bit == 0) {
                if (node->right) {
                    maxXor |= (1 << i);
                    node = node->right;
                } else {
                    node = node->left;
                }
            } else {
                if (node->left) {
                    maxXor |= (1 << i);
                    node = node->left;
                } else {
                    node = node->right;
                }
            }
        }
        return maxXor;
    }

public:
    int findMaximumXOR(vector<int>& nums) {
        BitNode* root = new BitNode();
        for (int num : nums) {
            insert(root, num);
        }
        int ans = 0;
        for (int num : nums) {
            ans = max(ans, getMaxXor(root, num));
        }
        return ans;
    }
};
```

**Key insight.** Greedy works because choosing the opposite bit at a higher position contributes more to XOR than any combination of bits at lower positions — same as why we read binary numbers left to right.

**Complexity.** O(N) time (32 operations per number). O(N) space.

---

# Section 2 — Important

## 6. Word Search II — LeetCode 212 — **hard, code given**

**Approach.** Build a trie from the given words, then run DFS on the grid with trie-guided pruning.

```cpp
class Solution {
    struct TrieNode {
        TrieNode* children[26];
        string word;
        TrieNode() {
            for(int i = 0; i < 26; i++) children[i] = nullptr;
            word = "";
        }
    };
    
    void buildTrie(vector<string>& words, TrieNode* root) {
        for (string& w : words) {
            TrieNode* node = root;
            for (char c : w) {
                if (!node->children[c - 'a']) {
                    node->children[c - 'a'] = new TrieNode();
                }
                node = node->children[c - 'a'];
            }
            node->word = w;
        }
    }
    
    void dfs(vector<vector<char>>& board, int i, int j, TrieNode* node, vector<string>& result) {
        char c = board[i][j];
        if (c == '#' || !node->children[c - 'a']) return;
        
        node = node->children[c - 'a'];
        if (node->word != "") {
            result.push_back(node->word);
            node->word = ""; // Pruning: avoid duplicate additions
        }
        
        board[i][j] = '#'; // Mark visited
        
        if (i > 0) dfs(board, i - 1, j, node, result);
        if (i < board.size() - 1) dfs(board, i + 1, j, node, result);
        if (j > 0) dfs(board, i, j - 1, node, result);
        if (j < board[0].size() - 1) dfs(board, i, j + 1, node, result);
        
        board[i][j] = c; // Backtrack
    }
    
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        TrieNode* root = new TrieNode();
        buildTrie(words, root);
        
        vector<string> result;
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                dfs(board, i, j, root, result);
            }
        }
        return result;
    }
};
```

**Key insight.** The trie replaces checking each word separately — instead of K word-searches on the grid, do ONE grid-search guided by the trie. Prune branches by removing found words (setting `node->word = ""`). Without pruning, it gets TLE.

**Complexity.** Time: O(M * N * 3^L) where L is the max length of a word. Space: O(K) where K is total characters in words.

---

## 7. Search Suggestions System — LeetCode 1268

**Approach.** Insert all products, for each prefix DFS from the node collecting up to 3 lexicographically smallest.

```
insert all products into trie
for each char c in searchWord:
    node = node.children[c]
    if not node:
        add empty list to result for this and remaining chars
        break
    list = dfs_collect_upto_3_words(node)
    add list to result
```

**Key insight.** Sorting products first and using binary search is simpler and faster in many languages. The trie approach teaches the structure but isn't optimal here. For trie DFS, you iterate 0-25 so it naturally returns sorted words.

**Complexity.** O(S) time to build. O(L * 26) to search, where S is sum of products and L is searchWord length.

---

## 8. Replace Words — LeetCode 648

**Approach.** Build trie from roots. For each word in sentence, walk trie — if you hit isEndOfWord before finishing the word, replace with the prefix.

```
build trie from roots
for word in sentence.split():
    node = root
    prefix = ""
    for c in word:
        if not node.children[c]: break
        node = node.children[c]
        prefix += c
        if node.isEndOfWord:
            replace word with prefix
            break
```

**Key insight.** The trie naturally finds the SHORTEST prefix along the path, which is exactly what the problem asks for.

**Complexity.** O(N) to build, O(W) to query, where W is length of sentence.

---

## 9. Map Sum Pairs — LeetCode 677

**Approach.** Each node stores a running sum of all words passing through it.

```
insert(key, val):
    delta = val - map[key]
    map[key] = val
    node = root
    for c in key:
        if not node.children[c]: node.children[c] = new TrieNode()
        node = node.children[c]
        node.sum += delta

sum(prefix):
    node = root
    for c in prefix:
        if not node.children[c]: return 0
        node = node.children[c]
    return node.sum
```

**Key insight.** On insert, walk the path and add the delta (new value - old value) to every node. You need a hashmap to remember the old value of a key.

**Complexity.** O(L) time for insert and sum.

---

## 10. Count Distinct Substrings — GFG

**Approach.** Insert ALL suffixes of the string. Count total nodes (including root).

```
root = new TrieNode()
count = 0
for i from 0 to s.length-1:
    node = root
    for j from i to s.length-1:
        if not node.children[s[j]]:
            node.children[s[j]] = new TrieNode()
            count++
        node = node.children[s[j]]
return count + 1 // +1 for empty string
```

**Key insight.** Each node in the trie corresponds to a unique prefix of some suffix, which IS a unique substring.

**Complexity.** O(N^2) time and space.

---

# Section 3 — Good to Know

## 11. Word Break with Trie — LeetCode 139

**Approach.** Build trie from dictionary. DP where `dp[i]` = true if `s[0..i-1]` is breakable.

```
dp[0] = true
for i from 0 to s.length-1:
    if not dp[i]: continue
    node = root
    for j from i to s.length-1:
        if not node.children[s[j]]: break
        node = node.children[s[j]]
        if node.isEndOfWord:
            dp[j+1] = true
```

**Key insight.** The trie replaces the inner loop of checking all dictionary words — O(L) per position instead of O(dict_size * L).

**Complexity.** O(N * L) where N is string length and L is max length of word in dict.

---

## 12. Palindrome Pairs — LeetCode 336

**Approach.**

```
build trie with reversed words, storing original index at end node
for each word A:
    search A in trie to find B where reverse(B) is prefix of A, check if suffix is palindrome
    also handle case where A is prefix of reverse(B), check if remaining is palindrome
```

**Key insight.** For word A+B to be a palindrome, either A's reverse is a prefix of B (remaining suffix of B is palindrome), or B's reverse is a suffix of A (remaining prefix of A is palindrome). The trie stores reversed words to facilitate these prefix checks.

**Complexity.** O(N * L^2) where N is number of words and L is average length.

---

## 13. Maximum XOR With Element From Array — LeetCode 1707

**Approach.** Sort queries by max element allowed. Insert numbers into bit-trie in sorted order.

```
sort nums
sort queries by m_i
trie = new BitTrie()
idx = 0
for each query (x, m, original_index):
    while idx < nums.size() and nums[idx] <= m:
        trie.insert(nums[idx])
        idx++
    if idx == 0: ans[original_index] = -1
    else: ans[original_index] = trie.getMaxXor(x)
```

**Key insight.** Offline processing — sort both the array and the queries, and process them in order of the constraint. This avoids rebuilding or filtering the trie.

**Complexity.** O(N log N + Q log Q) for sorting. O((N + Q) * 32) for trie operations.

---

## 14. Stream of Characters — LeetCode 1032

**Approach.** Build trie from REVERSED words. On each new character, append to a running buffer, then walk trie backwards.

```
build trie from reversed words
query(char):
    buffer.append(char)
    node = root
    for i from buffer.length-1 down to 0:
        if not node.children[buffer[i]]: return false
        node = node.children[buffer[i]]
        if node.isEndOfWord: return true
    return false
```

**Key insight.** Reversing the words means you can match suffixes of the stream starting from the most recent character, which is exactly what the problem asks.

**Complexity.** O(W * L) to build. O(L) per query where L is max word length.

---

# Section 4 — Approach Only

### 15. Concatenated Words (LC 472)
**Approach.** Sort words by length. For each word, try to break it using a trie built from words processed so far (like Word Break). If it can be broken, add to result. Otherwise, insert it into the trie.

### 16. Prefix and Suffix Search (LC 745)
**Approach.** For a word like "apple", insert "apple{apple", "pple{apple", "ple{apple", "le{apple", "e{apple" into the trie, storing the word's weight at each node. To query prefix "ap" and suffix "le", search for "le{ap" in the trie.

### 17. Camelcase Matching (LC 1023)
**Approach.** Build a trie. Walk the query through the trie. Uppercase letters must match exactly. Lowercase letters can be skipped if they don't match the current node.

### 18. Maximum Genetic Difference Query (LC 1938)
**Approach.** Build a tree from the parent array. Run offline DFS on the tree. As you enter a node, insert it into a bit-trie. Answer queries for that node. As you leave the node, remove it from the bit-trie (decrement counts).
