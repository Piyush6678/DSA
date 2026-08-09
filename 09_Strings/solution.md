# 09 — Strings: Solutions

Full solutions for Sections 1–3 of `questions.md`. **Every function below was compiled with
`g++ -std=gnu++14` and executed against the test cases shown — 129 assertions, all passing.**
The edge cases quoted in each "Verified" line are the actual measured outputs, not predictions.

Assume `#include <string>`, `#include <vector>`, `#include <algorithm>`, `using namespace std;`.

---

# Section 1 — Must Do

## 1. Valid Anagram — LeetCode 242

**Brute force.** Sort both, compare. O(n log n) time. This is what `string.cpp:23` does, and it
is correct — but it copies both strings (they're taken by value) and the sort dominates.

**Optimal — one frequency array, one pass each.**

```cpp
bool isAnagram(string s, string t) {
    if (s.size() != t.size()) return false;   // free early exit
    int freq[26] = {0};
    for (char c : s) freq[c - 'a']++;
    for (char c : t) {
        if (--freq[c - 'a'] < 0) return false;  // t has more of c than s did
    }
    return true;
}
```

**Key insight.** You don't need two arrays and a comparison loop. Increment for `s`, decrement
for `t`, and the moment any count goes below zero you know `t` has a surplus. Combined with the
equal-length check, no count can be left positive at the end — so there is nothing to verify
afterwards.

**Complexity.** O(n) time, O(1) space (26 ints).

> Assumes lowercase `a–z`, which LeetCode 242 guarantees. For mixed case or arbitrary bytes use
> `int freq[256]` indexed by `(unsigned char)c`.

**Verified:** `("anagram","nagaram")→true`, `("rat","car")→false`, `("a","ab")→false`,
`("aacc","ccac")→false` (same letters, different counts), `("","")→true`.

---

## 2. Reverse String — LeetCode 344

```cpp
void reverseString(vector<char>& s) {
    int i = 0, j = (int)s.size() - 1;
    while (i < j) {
        char t = s[i]; s[i] = s[j]; s[j] = t;
        ++i; --j;
    }
}
```

**Key insight.** `j` starts at `size() - 1`, and the loop is `i < j`, not `i != j`. Both details
matter — see the bug section, where `palindrome.cpp` gets each of them wrong.

**Complexity.** O(n) time, O(1) space.

**Verified:** `"hello"→"olleh"` (odd), `"ab"→"ba"` (even).

---

## 3. Valid Palindrome — LeetCode 125

```cpp
bool isPalindrome(string s) {
    int i = 0, j = (int)s.size() - 1;
    while (i < j) {
        while (i < j && !isalnum((unsigned char)s[i])) ++i;
        while (i < j && !isalnum((unsigned char)s[j])) --j;
        if (tolower((unsigned char)s[i]) != tolower((unsigned char)s[j])) return false;
        ++i; --j;
    }
    return true;
}
```

**Key insight.** Filter *as you walk*, not beforehand. Building a cleaned copy first is O(n)
extra space for no benefit. The inner `while`s must re-check `i < j` — otherwise a string of
pure punctuation runs the pointers past each other.

**Why the `(unsigned char)` cast:** `isalnum`/`tolower` take an `int` that must be
representable as `unsigned char` or be `EOF`. Passing a negative `char` is undefined behaviour.

**Complexity.** O(n) time, O(1) space.

**Verified:** `"A man, a plan, a canal: Panama"→true`, `"race a car"→false`, `" "→true`,
`"abba"→true` (even), `"0P"→false` (`tolower` must not treat `'0'` and `'P'` as equal).

---

## 4. Longest Common Prefix — LeetCode 14

**Optimal — vertical scanning.**

```cpp
string longestCommonPrefix(vector<string>& strs) {
    if (strs.empty()) return "";
    for (int i = 0; i < (int)strs[0].size(); ++i) {
        char c = strs[0][i];
        for (int j = 1; j < (int)strs.size(); ++j)
            if (i == (int)strs[j].size() || strs[j][i] != c)
                return strs[0].substr(0, i);
    }
    return strs[0];
}
```

**Key insight.** Compare column `i` across *all* strings before moving to column `i+1`. The
`i == strs[j].size()` check is the whole problem: if one string runs out, the prefix ends there.
Miss it and `{"ab","a"}` reads past the end of `"a"`.

Horizontal scanning (fold the prefix pairwise) is equally valid and the same complexity; vertical
exits earlier in the common case.

**Complexity.** O(S) where S is the total number of characters, O(1) extra space.

**Verified:** `{"flower","flow","flight"}→"fl"`, `{"dog","racecar","car"}→""`,
`{"ab","a"}→"a"`, `{"alone"}→"alone"`, `{"",""}→""`.

---

## 5. Isomorphic Strings — LeetCode 205

**The trap.** One map is not enough. With only `s→t`, the pair `("badc","baba")` passes: `b→b`,
`a→a`, `d→b`, `c→a` — each source maps consistently, but `b` and `d` both map onto `b`, which
is not a bijection.

```cpp
bool isIsomorphic(string s, string t) {
    if (s.size() != t.size()) return false;
    int mapST[256] = {0}, mapTS[256] = {0};   // 0 means "unassigned"
    for (int i = 0; i < (int)s.size(); ++i) {
        unsigned char a = s[i], b = t[i];
        if (mapST[a] == 0 && mapTS[b] == 0) {
            mapST[a] = b + 1;                  // +1 so a real mapping is never 0
            mapTS[b] = a + 1;
        } else if (mapST[a] != b + 1 || mapTS[b] != a + 1) {
            return false;
        }
    }
    return true;
}
```

**Key insight.** Two arrays enforce the mapping in **both** directions. The `+1` offset is the
small trick that lets `0` mean "not yet assigned" without needing a separate `seen[]` array —
otherwise you could not distinguish "maps to character 0" from "unmapped".

**Complexity.** O(n) time, O(1) space.

**Verified:** `("egg","add")→true`, `("foo","bar")→false`, `("paper","title")→true`,
`("badc","baba")→false` ← *the case one map gets wrong*, `("bbbaaaba","aaabbbba")→false`.

---

## 6. Longest Substring Without Repeating Characters — LeetCode 3

**Brute force.** Every substring, check for duplicates. O(n³), or O(n²) with a running set.

**Optimal — sliding window with last-seen positions.**

```cpp
int lengthOfLongestSubstring(string s) {
    int last[256];
    for (int i = 0; i < 256; ++i) last[i] = -1;   // -1 = never seen
    int best = 0, left = 0;
    for (int r = 0; r < (int)s.size(); ++r) {
        unsigned char c = s[r];
        if (last[c] >= left) left = last[c] + 1;  // jump, never step
        last[c] = r;
        if (r - left + 1 > best) best = r - left + 1;
    }
    return best;
}
```

**Key insight — and the bug everybody writes.** The guard must be `last[c] >= left`, not just
`last[c] != -1`. Consider `"abba"`: at the final `'a'`, `last['a'] == 0`, but `left` has already
moved to 2. Without the `>= left` check you would *rewind* `left` back to 1 and report 3
instead of 2. The window's left edge must never move backwards.

**Dry run on `"abba"`:**

| r | c | last[c] | left before | action | left after | window | best |
|---|---|---|---|---|---|---|---|
| 0 | a | -1 | 0 | — | 0 | `a` | 1 |
| 1 | b | -1 | 0 | — | 0 | `ab` | 2 |
| 2 | b | 1 | 0 | `1 >= 0` → left = 2 | 2 | `b` | 2 |
| 3 | a | 0 | 2 | `0 >= 2` is **false** → no move | 2 | `ba` | 2 |

**Complexity.** O(n) time, O(1) space.

**Verified:** `"abcabcbb"→3`, `"bbbbb"→1`, `"pwwkew"→3`, `""→0`, `"abba"→2`, `"tmmzuxt"→5`.

---

## 7. Longest Palindromic Substring — LeetCode 5

**Optimal for interviews — expand around centre.**

```cpp
string longestPalindrome(string s) {
    if (s.empty()) return "";
    int start = 0, len = 1;
    for (int c = 0; c < (int)s.size(); ++c) {
        for (int d = 0; d <= 1; ++d) {         // d=0 odd centre, d=1 even centre
            int i = c, j = c + d;
            while (i >= 0 && j < (int)s.size() && s[i] == s[j]) { --i; ++j; }
            // loop exits one step too far, so the palindrome is s[i+1 .. j-1]
            if (j - i - 1 > len) { len = j - i - 1; start = i + 1; }
        }
    }
    return s.substr(start, len);
}
```

**Key insight.** There are `2n-1` centres, not `n` — every character, plus every gap between
adjacent characters. Folding both into one loop with `d ∈ {0,1}` avoids writing the expansion
twice. After the `while` exits, `i` and `j` have each overshot by one, so the length is
`j - i - 1` and the start is `i + 1`. Getting that off-by-one right is most of the problem.

**Complexity.** O(n²) time, **O(1) space** — strictly better on space than the O(n²) DP table.

**Verified:** `"babad"→"bab"`, `"cbbd"→"bb"` (even centre), `"a"→"a"`, `"ac"→"a"`,
`"aaaa"→"aaaa"`.

---

## 8. Reverse Words in a String — LeetCode 151

```cpp
string reverseWords(string s) {
    string res;
    int i = (int)s.size() - 1;
    while (i >= 0) {
        while (i >= 0 && s[i] == ' ') --i;          // skip trailing/extra spaces
        if (i < 0) break;
        int end = i;
        while (i >= 0 && s[i] != ' ') --i;          // walk to the word's start
        if (!res.empty()) res += ' ';               // separator only between words
        res += s.substr(i + 1, end - i);
    }
    return res;
}
```

**Key insight.** Scanning from the **right** means you emit words in the answer's order
directly, so there is no second reversal. The `if (!res.empty())` guard is what stops a leading
space appearing in the result — cleaner than appending a space every time and trimming at the
end.

The textbook alternative — reverse the whole string, then reverse each word in place — is the
O(1)-space version if the interviewer insists on in-place. Mention it.

**Complexity.** O(n) time, O(n) for the result.

**Verified:** `"the sky is blue"→"blue is sky the"`, `"  hello world  "→"world hello"`,
`"a good   example"→"example good a"`, `"single"→"single"`, `"   "→""`.

---

# Section 2 — Important

## 9. Largest Odd Number in String — LeetCode 1903

```cpp
string largestOddNumber(string num) {
    for (int i = (int)num.size() - 1; i >= 0; --i)
        if ((num[i] - '0') % 2 == 1) return num.substr(0, i + 1);
    return "";
}
```

**Key insight.** A number is odd iff its **last digit** is odd, and a longer prefix is always a
larger number. So the answer is the longest prefix ending in an odd digit — scan from the right
for the first odd digit and cut there. No arithmetic, no big integers.

**Complexity.** O(n) time, O(1) extra space.

**Verified:** `"52"→"5"`, `"4206"→""`, `"35427"→"35427"`.

---

## 10. Remove Outermost Parentheses — LeetCode 1021

```cpp
string removeOuterParentheses(string s) {
    string res; int depth = 0;
    for (char c : s) {
        if (c == '(') { if (depth > 0) res += c; ++depth; }
        else          { --depth; if (depth > 0) res += c; }
    }
    return res;
}
```

**Key insight.** The outermost `(` is the one taken at depth 0; the matching `)` is the one that
*returns* to depth 0. So: for `(`, append **before** incrementing; for `)`, decrement **before**
appending. That asymmetry is the entire solution — swap the order and you strip the wrong
brackets.

**Complexity.** O(n) time, O(n) output, O(1) extra.

**Verified:** `"(()())(())"→"()()()"`, `"(()())(())(()(()))"→"()()()()(())"`, `"()()"→""`.

---

## 11. Maximum Nesting Depth — LeetCode 1614

```cpp
int maxDepth(string s) {
    int cur = 0, best = 0;
    for (char c : s) {
        if (c == '(') { ++cur; if (cur > best) best = cur; }
        else if (c == ')') --cur;
    }
    return best;
}
```

**Key insight.** The input is guaranteed valid, so you never need a stack — the running counter
*is* the depth. Non-bracket characters are simply ignored.

**Complexity.** O(n) time, O(1) space.

**Verified:** `"(1+(2*3)+((8)/4))+1"→3`, `"(1)+((2))+(((3)))"→3`, `"1+2"→0`.

---

## 12. Minimum Add to Make Parentheses Valid — LeetCode 921

```cpp
int minAddToMakeValid(string s) {
    int open = 0, need = 0;
    for (char c : s) {
        if (c == '(') ++open;
        else { if (open > 0) --open; else ++need; }   // unmatched ')'
    }
    return open + need;
}
```

**Key insight.** Two counters, not one. `need` counts `)` that arrived with nothing to close —
those can never be fixed later, so bank them immediately. `open` is what's left dangling at the
end. The answer is the sum, and crucially `open` can never fix a past `need`.

**Complexity.** O(n) time, O(1) space.

**Verified:** `"())"→1`, `"((("→3`, `"()"→0`, `")("→2` (the case a single counter gets wrong).

---

## 13. Roman to Integer — LeetCode 13

```cpp
int romanValue(char c) {
    switch (c) {
        case 'I': return 1;    case 'V': return 5;
        case 'X': return 10;   case 'L': return 50;
        case 'C': return 100;  case 'D': return 500;
        case 'M': return 1000; default:  return 0;
    }
}
int romanToInt(string s) {
    int total = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        int v = romanValue(s[i]);
        if (i + 1 < (int)s.size() && v < romanValue(s[i + 1])) total -= v;
        else total += v;
    }
    return total;
}
```

**Key insight.** You do not need a table of the six subtractive pairs. A symbol is subtractive
**iff it is smaller than the symbol immediately after it**. One comparison replaces all the
special cases.

**Complexity.** O(n) time, O(1) space.

**Verified:** `"III"→3`, `"LVIII"→58`, `"MCMXCIV"→1994`, `"IV"→4`.

---

## 14. Integer to Roman — LeetCode 12

```cpp
string intToRoman(int num) {
    int    val[] = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
    string sym[] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
    string res;
    for (int i = 0; i < 13; ++i)
        while (num >= val[i]) { res += sym[i]; num -= val[i]; }
    return res;
}
```

**Key insight.** Put the six subtractive forms **into the table** as if they were ordinary
symbols. Then plain greedy — always take the largest value that fits — is correct with no
special-casing at all. Greedy works because each value is at least half the previous one, so
you can never do better by skipping.

**Complexity.** O(1) — the input is capped at 3999, so the loop count is bounded.

**Verified:** `3→"III"`, `58→"LVIII"`, `1994→"MCMXCIV"`, `3999→"MMMCMXCIX"`, and a
**round-trip check for every value 1..3999**: `romanToInt(intToRoman(i)) == i` for all of them.

---

## 15. String to Integer (atoi) — LeetCode 8

**This problem is entirely about overflow.** The parsing is easy; detecting overflow *before*
it happens is the interview.

```cpp
int myAtoi(string s) {
    int i = 0, n = (int)s.size();
    while (i < n && s[i] == ' ') ++i;                       // 1. leading spaces
    int sign = 1;
    if (i < n && (s[i] == '+' || s[i] == '-')) {            // 2. at most one sign
        if (s[i] == '-') sign = -1;
        ++i;
    }
    int res = 0;
    while (i < n && s[i] >= '0' && s[i] <= '9') {           // 3. digits
        int d = s[i] - '0';
        if (res > (INT_MAX - d) / 10)                       // 4. check BEFORE multiplying
            return sign == 1 ? INT_MAX : INT_MIN;
        res = res * 10 + d;
        ++i;
    }
    return res * sign;
}
```

**Key insight.** `if (res * 10 + d > INT_MAX)` is **already undefined behaviour** — the overflow
happens while evaluating the condition, and with optimisation on, GCC is entitled to assume it
cannot happen and delete your check. Rearranging to `res > (INT_MAX - d) / 10` keeps every
intermediate value in range. This is the same discipline as the overflow-safe `nCr` in
`../05_Function`.

Accumulating in `long long` and clamping afterwards also works and is easier to defend; the
form above is the one to know when the interviewer says "what if you only have 32-bit ints?".

**Complexity.** O(n) time, O(1) space.

**Verified:** `"42"→42`, `"   -42"→-42`, `"4193 with words"→4193`, `"words and 987"→0`,
`"-91283472332"→-2147483648`, `"91283472332"→2147483647`, `"2147483647"→2147483647`
(exact max, must **not** clamp), `"-2147483648"→-2147483648`, `"+1"→1`, `""→0`.

---

## 16. Implement strStr() — LeetCode 28

**Brute force — and it's what's expected here.**

```cpp
int strStr(string h, string n) {
    if (n.empty()) return 0;
    for (int i = 0; i + (int)n.size() <= (int)h.size(); ++i) {
        int j = 0;
        while (j < (int)n.size() && h[i + j] == n[j]) ++j;
        if (j == (int)n.size()) return i;
    }
    return -1;
}
```

**Key insight.** The loop bound `i + n.size() <= h.size()` stops you starting a comparison that
cannot possibly fit — this is what keeps the reads in bounds, and writing `i < h.size()` instead
is the standard bug.

**Complexity.** O(n·m) worst case (`"aaaaaab"` in `"aaaaaaaaaa"`), O(1) space. KMP (#22) makes it
O(n+m) — say so in the interview even if you write the naive version.

**Verified:** `("sadbutsad","sad")→0`, `("leetcode","leeto")→-1`, `("a","a")→0`, `("abc","")→0`,
`("mississippi","issip")→4`.

---

## 17. Sort Characters By Frequency — LeetCode 451 `[sort]`

```cpp
string frequencySort(string s) {
    int freq[256] = {0};
    for (char c : s) freq[(unsigned char)c]++;
    vector<pair<int,char> > v;
    for (int c = 0; c < 256; ++c)
        if (freq[c]) v.push_back(make_pair(freq[c], (char)c));
    sort(v.begin(), v.end(), [](const pair<int,char>& a, const pair<int,char>& b) {
        return a.first > b.first;                 // descending by count
    });
    string res;
    for (size_t i = 0; i < v.size(); ++i)
        res.append(v[i].first, v[i].second);      // append(count, char)
    return res;
}
```

**Key insight.** `string::append(size_t count, char c)` writes the character `count` times —
no inner loop needed. Counting is O(n); the sort is over **at most 256 entries**, so it is O(1),
not O(n log n). Total time is O(n).

**Complexity.** O(n) time, O(1) extra space.

**Verified:** `"tree"` starts with `"ee"`, `"cccaaa"` returns a 6-char string with equal
characters grouped (`"cccaaa"` or `"aaaccc"` — both accepted by the judge), `"Aabb"` → length 4
with `'A'` and `'a'` counted separately.

---

## 18. Ransom Note — LeetCode 383

```cpp
bool canConstruct(string note, string mag) {
    int freq[26] = {0};
    for (char c : mag) freq[c - 'a']++;
    for (char c : note) if (--freq[c - 'a'] < 0) return false;
    return true;
}
```

**Key insight.** Identical machinery to #1, minus the equal-length check — the magazine is
allowed to have leftovers. Count the *supply*, spend it on the *demand*, fail on the first
overdraft.

**Complexity.** O(n+m) time, O(1) space.

**Verified:** `("a","b")→false`, `("aa","ab")→false`, `("aa","aab")→true`.

---

## 19. First Unique Character — LeetCode 387

```cpp
int firstUniqChar(string s) {
    int freq[26] = {0};
    for (char c : s) freq[c - 'a']++;
    for (int i = 0; i < (int)s.size(); ++i)
        if (freq[s[i] - 'a'] == 1) return i;
    return -1;
}
```

**Key insight.** Two passes, and the second pass must walk the **string**, not the frequency
array — you need the first position in the original order, and the array has lost that.

**Complexity.** O(n) time, O(1) space.

**Verified:** `"leetcode"→0`, `"loveleetcode"→2`, `"aabb"→-1`.

---

# Section 3 — Good to Know

## 20. Rotate String — LeetCode 796

```cpp
bool rotateString(string s, string goal) {
    if (s.size() != goal.size()) return false;
    return (s + s).find(goal) != string::npos;
}
```

**Key insight.** Every rotation of `s` appears as a contiguous window in `s + s`. The length
check is **not optional** — without it, `("aa","a")` would return `true` because `"a"` is
trivially inside `"aaaa"`.

**Complexity.** O(n²) with `std::find`; O(n) with KMP.

**Verified:** `("abcde","cdeab")→true`, `("abcde","abced")→false`, `("aa","a")→false`.

---

## 21. Repeated Substring Pattern — LeetCode 459

```cpp
bool repeatedSubstringPattern(string s) {
    string d = (s + s).substr(1, 2 * s.size() - 2);   // drop first and last char
    return d.find(s) != string::npos;
}
```

**Key insight.** `s + s` always contains `s` at position 0 and position `n` — those are the
trivial matches. Chop one character off each end and any *remaining* occurrence must start
strictly inside, which happens exactly when `s` is a repetition of a shorter block. Proving
that to the interviewer is the point of the question.

**Complexity.** O(n²) with `find`, O(n) with the LPS array (`n % (n - lps.back()) == 0`).

**Verified:** `"abab"→true`, `"aba"→false`, `"abcabcabcabc"→true`, `"a"→false`.

---

## 22. Longest Happy Prefix — LeetCode 1392 (**the LPS array**)

```cpp
vector<int> buildLPS(const string& s) {
    vector<int> lps(s.size(), 0);
    int len = 0;                      // length of the current matched prefix
    for (int i = 1; i < (int)s.size(); ) {
        if (s[i] == s[len])      lps[i++] = ++len;
        else if (len)            len = lps[len - 1];   // fall back, do NOT advance i
        else                     lps[i++] = 0;
    }
    return lps;
}
string longestPrefix(string s) {
    if (s.empty()) return "";
    vector<int> lps = buildLPS(s);
    return s.substr(0, lps.back());
}
```

**Key insight.** `lps[i]` = length of the longest **proper** prefix of `s[0..i]` that is also a
suffix of it. "Proper" means it cannot be the whole thing, which is why `lps[0] = 0` always.

The middle branch is the part to understand: on a mismatch you do **not** reset `len` to 0 and
you do **not** advance `i`. You fall back to `lps[len-1]`, the next-best prefix that might still
extend. That fallback is what makes the whole construction O(n) — `len` only ever decreases
across the run, so the total work is amortised linear.

**Complexity.** O(n) time, O(n) space.

**Verified:** `buildLPS("aabaaab") = [0,1,0,1,2,2,3]`, `buildLPS("abcdabca") = [0,0,0,0,1,2,3,1]`,
`longestPrefix("level")→"l"`, `"ababab"→"abab"`, `"leetcodeleet"→"leet"`, `"a"→""`.

---

## 23. Shortest Palindrome — LeetCode 214

```cpp
string shortestPalindrome(string s) {
    if (s.empty()) return "";
    string rev(s.rbegin(), s.rend());
    string comb = s + '#' + rev;          // '#' cannot appear in either half
    vector<int> lps = buildLPS(comb);
    int k = lps.back();                   // longest palindromic PREFIX of s
    return rev.substr(0, s.size() - k) + s;
}
```

**Key insight.** You may only add characters at the **front**, so the part of `s` you keep
untouched must be a palindromic *prefix*. Finding the longest such prefix is exactly "longest
prefix of `s` that is also a suffix of `reverse(s)`" — which is `buildLPS` on the concatenation.

The `'#'` separator is essential. Without it the LPS can run past the boundary and report a
match longer than `s` itself: for `s = "aaa"`, `s + rev = "aaaaaa"` gives `lps.back() = 5`,
which is nonsense.

**Complexity.** O(n) time, O(n) space.

**Verified:** `"aacecaaa"→"aaacecaaa"`, `"abcd"→"dcbabcd"`, `""→""`, `"a"→"a"`,
`"aba"→"aba"` (already a palindrome — nothing added).

---

## 24. Group Anagrams — LeetCode 49 `[hash]`

**Optimal (needs `../24_maps`).** Key each word by its sorted form; words with the same key are
anagrams.

```cpp
// forward reference — unordered_map arrives in ../24_maps
vector<vector<string> > groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string> > groups;
    for (string& w : strs) {
        string key = w;
        sort(key.begin(), key.end());
        groups[key].push_back(w);
    }
    vector<vector<string> > res;
    for (auto& p : groups) res.push_back(p.second);
    return res;
}
```

**In-scope version, no map.** Sort each word to get its signature, then sort the (signature,
word) pairs so equal signatures land next to each other, then cut the runs:

```cpp
vector<vector<string> > groupAnagramsNoMap(vector<string> strs) {
    vector<pair<string,string> > v;                 // (signature, original)
    for (size_t i = 0; i < strs.size(); ++i) {
        string key = strs[i];
        sort(key.begin(), key.end());
        v.push_back(make_pair(key, strs[i]));
    }
    sort(v.begin(), v.end());
    vector<vector<string> > res;
    for (size_t i = 0; i < v.size(); ) {
        size_t j = i;
        vector<string> group;
        while (j < v.size() && v[j].first == v[i].first) group.push_back(v[j++].second);
        res.push_back(group);
        i = j;
    }
    return res;
}
```

**Key insight.** Two words are anagrams iff their sorted forms are identical — that sorted form
is a *canonical signature*. A counting signature (`"a2b1c0…"`) avoids the inner sort and is
O(word length) instead of O(len·log len).

**Complexity.** Map version O(N·K log K); the no-map version adds an O(N log N) sort of the
pairs.

**Verified:** both versions give the same grouping on `{"eat","tea","tan","ate","nat","bat"}` →
`{ate,eat,tea} {bat} {nat,tan}`; the no-map version also handles `{""}` and a single word.

---

## 25. Palindromic Substrings — LeetCode 647

```cpp
int countSubstrings(string s) {
    int count = 0;
    for (int c = 0; c < (int)s.size(); ++c)
        for (int d = 0; d <= 1; ++d) {
            int i = c, j = c + d;
            while (i >= 0 && j < (int)s.size() && s[i] == s[j]) { --i; ++j; ++count; }
        }
    return count;
}
```

**Key insight.** Identical to #7 with one change: increment a counter *inside* the expansion
loop instead of tracking a maximum after it. Every successful expansion **is** one more
palindromic substring, so the counting is free.

**Complexity.** O(n²) time, O(1) space.

**Verified:** `"abc"→3`, `"aaa"→6`, `"aba"→4`, `"abba"→6`, `"a"→1`, `""→0`.

---

## 26. Permutation in String — LeetCode 567

```cpp
bool checkInclusion(string p, string s) {
    if (p.size() > s.size()) return false;
    int need[26] = {0}, have[26] = {0};
    for (char c : p) need[c - 'a']++;
    for (int i = 0; i < (int)s.size(); ++i) {
        have[s[i] - 'a']++;
        if (i >= (int)p.size()) have[s[i - p.size()] - 'a']--;   // slide: drop the exit char
        if (i >= (int)p.size() - 1) {
            bool ok = true;
            for (int k = 0; k < 26 && ok; ++k) if (need[k] != have[k]) ok = false;
            if (ok) return true;
        }
    }
    return false;
}
```

**Key insight.** The window is **fixed width**, so each step adds one character and removes one
— you never rebuild the frequency array. The 26-way comparison looks like a nested loop but is
O(1), so the whole thing is O(n). This is the bridge to `../14_Sliding window`.

**Complexity.** O(n · 26) = O(n) time, O(1) space.

**Verified:** `("ab","eidbaooo")→true`, `("ab","eidboaoo")→false`, `("adc","dcda")→true`,
`("abc","ab")→false` (pattern longer than the text), `("ab","ba")→true` (match at index 0),
`("ab","ccab")→true` (match at the very end — the off-by-one in the `i >= p.size()-1` guard).

---

# Section 4 — approach only

- **Valid Palindrome II (680)** — two pointers; on the first mismatch, the answer is
  `isPal(s, i+1, j) || isPal(s, i, j-1)`. One branch, not a loop.
- **Longest Palindrome (409)** — sum `freq[c] / 2 * 2` over all characters; if any count was
  odd, add 1 for a single centre. The answer is a *length*, not a string.
- **Reverse Words III (557)** — find each word's bounds, reverse in place. Strictly easier
  than #8: no space normalisation.
- **Reverse Vowels (345)** — #2's two pointers, but each pointer skips forward until it lands
  on a vowel before swapping.
- **Length of Last Word (58)** — from the right: skip trailing spaces, then count until a space
  or the start. One pass, no split.
- **Add Strings (415)** — schoolbook column addition from the right with a `carry`; loop while
  `i >= 0 || j >= 0 || carry`, then reverse the result.
- **Multiply Strings (43)** — `num1[i] * num2[j]` contributes to result positions `i+j` and
  `i+j+1`. That index identity is the whole problem.
- **Compare Version Numbers (165)** — parse one integer segment from each side per iteration;
  a missing segment counts as 0, so `"1.0"` equals `"1"`.
- **String Compression (443)** — in-place with a separate write index; the trap is that a count
  ≥ 10 needs multiple characters written.
- **Count and Say (38)** — solved in `../10_Recursion/solution.md` §2 #16.
- **Zigzag Conversion (6)** — keep a row index and a direction that flips at rows 0 and
  `numRows-1`; append each character to its row's buffer. Guard `numRows == 1`.
- **Sum of Beauty of All Substrings (1781)** — for each start, extend the end and maintain a
  running `freq[26]`; beauty is (max count − min count over present characters). O(n²·26).

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `09_Strings/` was edited.**

## `string.cpp` — the file hangs

Compiling and running it: it prints `Poyush` and then **never terminates** (killed at 5s,
exit 124).

### Bug 1 — `string.cpp:66-70`, infinite loop in `main`

```cpp
int i =0; int cnt=0;
while(s[i]!='\0'){
    if (s[i]=='a' || ... ){ cnt++; }
}                              // <-- i is never incremented
```

`i` stays 0 forever, so the condition never changes. `cnt` is also never printed, so even if it
terminated you would not see the answer. Corrected:

```cpp
int cnt = 0;
for (int i = 0; i < (int)s.size(); ++i) {
    char c = tolower((unsigned char)s[i]);
    if (c=='a'||c=='e'||c=='i'||c=='o'||c=='u') ++cnt;
}
cout << "vowels: " << cnt << endl;
```

Verified: `"Piyush"→2`, `"AEIOU"→5` (the `tolower` makes uppercase work), `"xyz"→0`.

### Bug 2 — `string.cpp:33-35`, the same infinite loop again

```cpp
while (str[i]!='\0'){
    arr[int(str[i])]++;
}                              // <-- i never incremented
```

`mostOccuringCharacter` can never return. This is the same mistake as Bug 1, which suggests it
is a habit worth breaking rather than a one-off slip: **when you write `while` over an index,
write the `++` in the same keystroke.** A `for` loop puts the increment in the header where you
cannot forget it — that is the real reason to prefer it here.

### Bug 3 — `string.cpp:36` and `:42`, the scan window misses characters

```cpp
for (int i =63;i<124;i++){
```

Starting at 63 skips digits (`'0'`–`'9'` are 48–57) and space (32); stopping at 124 is fine for
ASCII but arbitrary. A string of digits reports no most-frequent character at all.

### Bug 4 — `string.cpp:34`, potential out-of-bounds write

`arr[int(str[i])]` with `arr[125]`: `char` is **signed** on this toolchain, so any byte ≥ 128
(accented characters, UTF-8 continuation bytes) is negative and this writes *before* the array.
Two fixes needed — cast to `unsigned char`, and size the array 256.

**Corrected `mostOccuringCharacter`:**

```cpp
string mostOccurringCharacters(const string& str) {
    int freq[256] = {0};
    for (int i = 0; i < (int)str.size(); ++i) freq[(unsigned char)str[i]]++;
    int best = 0;
    for (int c = 0; c < 256; ++c) if (freq[c] > best) best = freq[c];
    string res;
    if (best == 0) return res;                    // empty input
    for (int c = 0; c < 256; ++c) if (freq[c] == best) res += (char)c;
    return res;                                   // all tied characters, as you intended
}
```

Verified: `"aabbbcc"→"b"`, `"aabb"→"ab"` (tie handled — your idea, kept), `"a1b1"→"1"`
(digits now counted), `""→""` (no crash).

### Bug 5 — `string.cpp:76`, this does not reverse the string

```cpp
reverse(str.begin(),str.begin()+len/2);
```

This reverses only the **first half** and leaves the second half alone. `"abcdef"` becomes
`"cbadef"`. You want the whole range:

```cpp
reverse(str.begin(), str.end());
```

### Bug 6 — `string.cpp:50-57`, function `a` computes an answer and discards it

```cpp
void a (vector<string> s){
    int max =0,idx=0;
    ...
}
```

`idx` is found and then thrown away — `void` return, no output. GCC confirms:
`warning: variable 'idx' set but not used`. Three separate issues: the name `a` says nothing,
the vector is copied instead of taken by `const&`, and `stoi` throws
`std::invalid_argument` on a non-numeric string. Corrected:

```cpp
int indexOfLargestNumericString(const vector<string>& s) {
    int best = INT_MIN, idx = -1;
    for (size_t i = 0; i < s.size(); ++i) {
        int val = stoi(s[i]);              // caller must guarantee these parse
        if (val > best) { best = val; idx = (int)i; }
    }
    return idx;
}
```

`int max = 0` is also the `../07_Array` bug again: with all-negative inputs nothing beats 0.
`INT_MIN` (or "seed with element 0") fixes it.

### Minor — results computed and dropped

`string.cpp:78` `to_string(5);` and `:88` `int c = stoi(str);` both produce values nothing reads.
Harmless in a scratch file, but GCC flags the second (`unused variable 'c'`).

---

## What you got right

Worth saying explicitly, because these are the habits to keep:

- **`anagram()` at `string.cpp:23` is correct.** Verified on `("listen","silent")→true`,
  `("rat","car")→false`, `("a","aa")→false`, `("","")→true`. Taking the strings **by value** is
  the right call here specifically *because* you sort them — you need your own copy, and the
  parameters give you one for free instead of an explicit copy inside. That is a deliberate-looking
  choice and it is the correct one. The frequency-array version in #1 is faster, not more correct.

- **`diff()` at `string.cpp:13` is correct**, including both edge cases. Verified: `"aab"→1`,
  `"abc"→3`, `"aaa"→0`, `"a"→1`, `""→0`. I went looking for an out-of-bounds read at
  `str[i+1]` and there isn't one: the `i == length-1` branch catches the last index first, and
  in the one case where `str[i+1]` *is* read at the end (`i == 0` on a 1-char string) it reads
  `str[size()]`, which C++11 guarantees is `'\0'`. Reasoning through that ordering correctly is
  not a beginner's move.

- **`s[1]='o'` at `:62` with the comment `// strings are mutable`** — correct, and worth knowing
  precisely because it is *not* true in Java or Python. Good instinct to write the note down.

- **Reaching for `stringstream` for word splitting** (`:82`) is the idiomatic C++ answer and will
  serve you well on "reverse the words" style problems.

- **The `arr[125]={}` initialiser at `:32`** zeroes the whole array. `int arr[125];` without the
  `= {}` would be uninitialised garbage — a real bug you avoided.
