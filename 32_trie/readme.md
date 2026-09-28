# 32 — Tries

A Trie (also known as a prefix tree or retrieval tree) is a tree-like data structure used for efficient string operations. While other structures excel at exact matches, the Trie dominates when you need fast prefix queries on a set of strings, such as for autocomplete, spell check, IP routing, or dictionary lookup.

The name comes from re**TRIE**val, but it is typically pronounced "try" to avoid confusion with the general "tree" data structure.

---

## 1. What is a Trie?

Unlike a Binary Search Tree (BST) which stores keys directly in the nodes, a Trie stores characters on the **edges** (or implicitly within the child nodes). A key or word in a Trie is defined by the path from the root to a marked "end of word" node.

When to use a Trie:
- You need fast prefix queries on a set of strings.
- You need to search for words in a dictionary or implement autocomplete.
- You want to implement longest prefix matching (e.g., IP routing).

Because strings in real-world applications often share prefixes (e.g., "apple", "app", "application"), a Trie compacts memory efficiently for highly repetitive datasets by overlapping those common paths.

---

## 2. The Node Structure

A typical Trie node contains two components:
1. An array of children pointers. For lowercase English letters, this is an array of size 26.
2. A boolean flag `isEndOfWord` to indicate if a complete word ends at this node.

Why use a 26-size array instead of a hash map (`std::unordered_map`)?
- **Array**: Provides $O(1)$ child access with a very small constant factor. For small alphabets (like 26 lowercase letters), the array is faster and simpler to implement.
- **Map**: When the alphabet is huge (e.g., Unicode) or very sparse, a hash map or balanced BST per node saves memory but adds a larger constant factor or logarithmic overhead ($O(\log k)$).

### Full C++ Code for TrieNode:

```cpp
// g++ -std=gnu++14
#include <iostream>
#include <string>

using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;
    
    TrieNode() {
        isEndOfWord = false;
        // Initialize all pointers to NULL to prevent garbage values
        for (int i = 0; i < 26; i++) {
            children[i] = NULL;
        }
    }
};
```

---

## 3. Insert — O(L)

Inserting a word into a Trie involves walking down the tree character by character. 
For each character in the word:
- If a child node for that character does not exist, we dynamically allocate and create it.
- We then move the current pointer to that child node.
- After processing all characters in the string, we mark the final node with `isEndOfWord = true` to denote a valid stopping point.

### C++ Code for Insert

```cpp
class Trie {
private:
    TrieNode* root;
    
public:
    Trie() {
        root = new TrieNode();
    }
    
    void insert(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int index = c - 'a'; // Map 'a'-'z' to 0-25
            if (curr->children[index] == NULL) {
                curr->children[index] = new TrieNode();
            }
            curr = curr->children[index];
        }
        curr->isEndOfWord = true;
    }
};
```

**Complexity Analysis**: 
The time complexity is strictly $O(L)$, where $L$ is the length of the word being inserted. This is completely independent of how many words are already stored in the Trie. A massive dictionary of a million words still allows insertions in the length of the single new word.

---

## 4. Search — O(L)

To search for a complete word, we again walk down the Trie character by character.
- If at any point a child node corresponding to the next character is missing, the word cannot exist in the Trie, and we return `false`.
- If we successfully process all characters, we return the value of `isEndOfWord` at the final node.

### C++ Code for Search

```cpp
    bool search(string word) {
        TrieNode* curr = root;
        for (char c : word) {
            int index = c - 'a';
            if (curr->children[index] == NULL) {
                return false;
            }
            curr = curr->children[index];
        }
        // Must check if it's an actual word, not just a prefix
        return curr->isEndOfWord;
    }
```

**Distinction**: There is a crucial difference between "the word exists" (`isEndOfWord == true`) and "the prefix exists" (we reached the final node, but `isEndOfWord` might be `false`). For example, if we insert "apple", a search for "app" should return `false`.

---

## 5. StartsWith (Prefix Search) — O(L)

This is the operation that makes Tries truly unique. No other standard data structure performs prefix matching gracefully in $O(L)$ time.

The logic is identical to `search()`, but we **do not** check `isEndOfWord` at the end. If we can successfully traverse the entire prefix without hitting a `NULL` child, then the prefix exists as part of some longer word (or as a standalone word).

### C++ Code for StartsWith

```cpp
    bool startsWith(string prefix) {
        TrieNode* curr = root;
        for (char c : prefix) {
            int index = c - 'a';
            if (curr->children[index] == NULL) {
                return false;
            }
            curr = curr->children[index];
        }
        return true; // We successfully traversed the prefix
    }
```

---

## 6. Delete — O(L)

Deleting a word from a Trie is the trickiest operation. You cannot simply find the node and set `isEndOfWord = false`, because you might leave behind dangling nodes that do not contribute to any other word, which is a memory leak in systems handling dynamic dictionaries.

**Recursive Approach**:
- Traverse down to the end of the word using recursion.
- Unmark `isEndOfWord`.
- On the way back up the recursion stack, delete the node **if and only if** it has no non-null children and is not the end of another valid word.

### C++ Code for Delete

```cpp
    bool deleteHelper(TrieNode* curr, const string& word, int depth) {
        if (curr == NULL) return false;
        
        // Base case: Reached the end of the word
        if (depth == word.length()) {
            if (!curr->isEndOfWord) return false; // Word doesn't exist
            
            curr->isEndOfWord = false; // Unmark
            
            // Check if node has no children
            for (int i = 0; i < 26; i++) {
                if (curr->children[i] != NULL) return false;
            }
            return true; // Node can be safely deleted
        }
        
        int index = word[depth] - 'a';
        bool canDeleteChild = deleteHelper(curr->children[index], word, depth + 1);
        
        if (canDeleteChild) {
            delete curr->children[index];
            curr->children[index] = NULL;
            
            // Return true if current node is not end of another word and has no other children
            if (curr->isEndOfWord) return false;
            for (int i = 0; i < 26; i++) {
                if (curr->children[i] != NULL) return false;
            }
            return true;
        }
        
        return false;
    }
    
    void deleteWord(string word) {
        deleteHelper(root, word, 0);
    }
```

---

## 7. Trie vs Other Structures

When should you use a Trie versus other common structures? See the comparison below. (For more details on strings, refer to `../09_Strings`; for maps, see `../24_maps`; for trees, see `../21_tree`).

| Data Structure | Insert | Search | Prefix Search | Space | When to use |
|---|---|---|---|---|---|
| **Hash Set/Map** | $O(L)$ avg | $O(L)$ avg | $O(L \cdot n)$ — must check all | $O(\text{total chars})$ | Exact lookup only |
| **Sorted Array + Binary Search** | $O(n)$ | $O(L \cdot \log n)$ | $O(L \cdot \log n)$ | $O(\text{total chars})$ | Static dictionary |
| **BST of strings** | $O(L \cdot \log n)$| $O(L \cdot \log n)$ | $O(L \cdot \log n)$ | $O(\text{total chars})$ | Ordered iteration needed |
| **Trie** | $O(L)$ | $O(L)$ | $O(L)$ | $O(\Sigma \cdot \text{nodes})$ | Prefix queries, autocomplete |

**The Space Tradeoff**: 
Tries provide unmatched $O(L)$ prefix search, but this comes at the cost of memory overhead. A node might allocate 26 pointers (often 208 bytes on a 64-bit architecture) even if it only has a single valid child path. If memory is a strict bottleneck and you *only* need exact lookups, a Hash Map is generally a superior choice.

---

## 8. Bit-Trie (XOR Trie)

A Bit-Trie is a specialized variant where the "alphabet" consists of only `0` and `1`. Each node has exactly 2 children rather than 26.
This structure is heavily used in **Maximum XOR** problems (e.g., LeetCode 421) and connects directly to bitwise operation optimizations (see `../15_bitwise`).

For each number, you insert its binary representation (e.g., all 32 bits, padded from most significant to least significant) into the Trie. 
To find the maximum XOR for a given number, you traverse the Trie greedily, always trying to pick the branch that gives the *opposite* bit at each level (since $1 \oplus 0 = 1$ and $0 \oplus 1 = 1$, maximizing the XOR result locally at the most significant bits).

### C++ Code Snippet for Bit-Trie

```cpp
struct BitNode {
    BitNode* children[2];
    BitNode() {
        children[0] = children[1] = NULL;
    }
};

void insertBit(BitNode* root, int num) {
    BitNode* curr = root;
    for (int i = 31; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (curr->children[bit] == NULL) {
            curr->children[bit] = new BitNode();
        }
        curr = curr->children[bit];
    }
}

int getMaxXor(BitNode* root, int num) {
    BitNode* curr = root;
    int maxXor = 0;
    for (int i = 31; i >= 0; i--) {
        int bit = (num >> i) & 1;
        // Try to pick the opposite bit to maximize XOR
        if (curr->children[1 - bit] != NULL) {
            maxXor |= (1 << i);
            curr = curr->children[1 - bit];
        } else {
            curr = curr->children[bit];
        }
    }
    return maxXor;
}
```

---

## 9. Applications

Tries are highly applicable to string manipulation problems, including:
1. **Autocomplete / Search Suggestions** (e.g., LC 1268 Search Suggestions System).
2. **Spell Checker**: Quickly validating if a typed word exists in a given dictionary.
3. **IP Routing (Longest Prefix Match)**: Hardware routers use a compressed variant of Tries to match IP address subnets efficiently.
4. **Word Games**: Solvers for Boggle, Scrabble, or Word Search II (LC 212) use Tries coupled with DFS backtracking.
5. **Counting distinct substrings**: A Trie can natively avoid duplicates while inserting all suffixes of a string, allowing you to count unique paths.
6. **XOR optimization**: Using a Bit-Trie for subset XOR maximization problems.

---

## 10. Counting with Tries (Trie-II)

A common extension to the basic Trie (often covered by instructors like Striver as "Trie-II") is to store integer counts at each node instead of just a single boolean flag.

- `countEndsWith`: How many exact word instances perfectly end at this node.
- `countPrefix`: How many word instances pass through this node as a prefix.

This modification allows the Trie to answer complex statistical queries:
- "How many times does the exact word 'apple' exist?"
- "How many total words start with the prefix 'app'?"

**Implementation note:** You increment `countPrefix` on every single node visited during an insertion, and increment `countEndsWith` only at the final node. For deletion, you decrement these counters accordingly on the downward pass.

---

## 11. Interview Q&A

**Q1: What is a Trie and when would you use one over a hash map?**
A Trie is a tree data structure that stores strings character by character. You use it over a hash map when you need to perform **prefix matching**, implement **autocomplete**, or find all words sharing a common prefix. A hash map can only do exact string lookups, not partial or prefix lookups.

**Q2: What is the time and space complexity of Trie operations?**
Time complexity for insert, search, and prefix search is $O(L)$, where $L$ is the length of the queried string. The space complexity is $O(\Sigma \cdot N)$, where $\Sigma$ is the alphabet size (e.g., 26 for English) and $N$ is the total number of instantiated nodes in the Trie.

**Q3: How would you implement autocomplete using a Trie?**
You would insert all dictionary words into the Trie. Given a user-typed prefix, you navigate to the node representing the end of that prefix in $O(L)$ time. From that node, you perform a Depth-First Search (DFS) or Breadth-First Search (BFS) to gather and return all reachable end-of-word nodes (which will all implicitly share the typed prefix).

**Q4: What is a Bit-Trie and when is it used?**
A Bit-Trie is a binary Trie where the alphabet consists solely of `0` and `1`, and it typically stores the binary representations of integers. It is primarily used to optimize bitwise operations over arrays, specifically for finding the Maximum XOR pair in $O(\log_2(\text{max\_val}))$ time instead of an $O(n^2)$ nested brute-force loop.

**Q5: How do you delete a word from a Trie without breaking other words?**
You must recursively traverse to the end of the targeted word, set `isEndOfWord = false`, and as you backtrack upwards, you delete any node that has **no children** and is **not the end of another valid word**. If you just nullify the string or delete nodes eagerly from the top, you risk destroying paths of other strings that share the same prefix.

**Q6: Can a Trie handle words with different character sets (not just lowercase)?**
Yes, but the fixed array size `children[26]` won't work gracefully for massive sets like Unicode. You would replace the array in each node with a `std::unordered_map<char, TrieNode*>` to handle arbitrary character sets efficiently. This saves memory at the cost of a slight hash map lookup overhead per character.

**Q7: How would you find all words matching a pattern with wildcards (like '.' in LC 211)?**
You use standard Trie search logic, but when you encounter the wildcard character ('.'), you must branch out and recursively explore **every single non-null child** of the current node. If any recursive exploration path returns true, it means the pattern successfully matched some word in the Trie.
