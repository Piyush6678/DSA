# 09 — Strings

A string is an array of characters with a length attached. Almost every technique in this
folder is an array technique you already know from `../07_Array`, applied to `char` instead of
`int`. What is genuinely new is the **26-slot frequency array**, which is the cheapest hash
table in existence and solves a surprising fraction of string interview problems.

---

## 1. `char`, C-strings, and `std::string`

A `char` is a 1-byte integer. `'a'` is not a letter to the compiler — it is the number 97.
That single fact is what makes frequency counting work:

```cpp
char c = 'd';
int idx = c - 'a';        // 100 - 97 = 3  -> 'd' is the 4th letter
char back = 'a' + idx;    // 97 + 3 = 100 -> 'd' again
```

Two representations exist side by side, and mixing them up is the classic beginner bug:

| | C-string (`char[]` / `const char*`) | `std::string` |
|---|---|---|
| Length | walk to the `'\0'` — **O(n)** | stored — `.size()` is **O(1)** |
| Terminator | `'\0'` is mandatory and occupies a slot | none needed; `"abc"` is 3 chars |
| Grows? | no, fixed buffer | yes, reallocates |
| Compare | `strcmp(a,b)` | `a == b` works |
| Concatenate | `strcat` (must have room) | `a + b` |

`char s[] = "abc";` is **4 bytes**, not 3. Forgetting the terminator slot overruns buffers.
Use `std::string` unless a problem specifically hands you a `char*`.

### The one indexing rule worth memorising

For a `std::string s`, `s[s.size()]` is **defined** and returns `'\0'` (C++11 onwards). Any
index beyond that is undefined behaviour. So `s[i+1]` inside a loop over `i < s.size()` is
safe at the last position — it reads the terminator — but `s[i-1]` at `i == 0` is **not**.
That asymmetry catches people.

---

## 2. The frequency array — the single most useful string idea

You do not need `unordered_map` for lowercase-letter problems. You need 26 ints.

```cpp
int freq[26] = {0};                  // ALL 26 zeroed; `int freq[26];` is garbage
for (char c : s) freq[c - 'a']++;
```

- **O(1) space** — 26 ints is a constant, no matter how long the string is.
- **Faster than a hash map** by a wide margin: no hashing, no allocation, perfect cache locality.
- Use `freq[256]` (indexed by `(unsigned char)c`) when the alphabet isn't just `a-z`.

> **Cast to `unsigned char` before indexing.** Plain `char` is *signed* on this toolchain, so a
> byte ≥ 128 becomes negative and `freq[c]` writes **before** the array. That is a real
> out-of-bounds write, not a theoretical one.

Anagram, ransom note, first unique character, frequency sort, and permutation-in-string are
all this one idea wearing different hats.

---

## 3. The five techniques this folder teaches

| Technique | Shape | Problems |
|---|---|---|
| **Frequency count** | one pass to build, one pass to read | anagram, ransom note, first unique |
| **Two pointers** | `i` from front, `j` from back, move inward | palindrome, reverse, reverse words |
| **Expand around centre** | fix a centre, grow outward while equal | longest palindromic substring |
| **Parsing / state machine** | walk once, track state in a counter | atoi, roman numerals, parentheses depth |
| **Prefix function (LPS)** | longest proper prefix that is also a suffix | KMP, shortest palindrome, happy prefix |

**Depth-as-a-counter deserves special mention.** Every "valid parentheses" problem with one
bracket type needs no stack at all — an `int` that goes up on `(` and down on `)` is enough.
It goes negative exactly when a `)` has no partner. You only need a real stack when there are
*multiple* bracket types (that's `../18_stack`).

---

## 4. Costs you should be able to state in an interview

| Operation | Cost | Note |
|---|---|---|
| `s[i]` | O(1) | |
| `s.size()` | O(1) | stored, unlike `strlen` |
| `s += c` | **amortised O(1)** | same doubling story as `vector` |
| `s + t` | O(n+m) | **builds a new string** |
| `s.substr(i, len)` | **O(len)** | copies — it is not a view |
| `s.find(t)` | O(n·m) worst case | not KMP in practice |
| `sort(s.begin(), s.end())` | O(n log n) | |

**The `substr` cost is the trap.** A recursion that passes `s.substr(1)` at every level looks
elegant and is **O(n²)** — every call copies the rest of the string. Passing an index instead
is O(n). You have this exact pair in `../10_Recursion`: the commented-out `removeChar` uses
`substr`, and the live one uses an index. The live one is the right instinct.

Similarly, building a result with `res = res + c` in a loop is O(n²); `res += c` is O(n).

---

## 5. Immutability

In C++, `std::string` **is mutable** — `s[1] = 'o'` works, and your `string.cpp:62` proves it.
This is worth flagging because it is the opposite of Java, Python and C#, where strings are
immutable and every "modification" allocates a new object. Interviewers ask this to check you
know which language you are in.

---

## Interview Q&A

**Q1. Why is `int freq[26]` considered O(1) space when it clearly uses memory?**
Because space complexity measures growth as a function of input size. 26 ints is 26 ints
whether the string has 10 characters or 10 million. It never grows, so it is constant. The
same argument makes `freq[256]` O(1) — 256 is also a constant, just a bigger one.

**Q2. Two strings are anagrams. What's the fastest check, and what does it cost?**
Frequency array: **O(n) time, O(1) space**. Sort-and-compare is O(n log n) and mutates or
copies both inputs. Start by comparing lengths — unequal lengths mean not anagrams, and it
costs nothing. The one-array trick is to increment for `s` and decrement for `t` in the same
array, bailing out the moment a count goes negative.

**Q3. Why does checking for a palindrome need `i < j` rather than `i != j`?**
With `i != j` an even-length string never terminates: the pointers swap past each other
without ever being equal, and you read off both ends of the string. `i < j` (or `i >= j` as the
recursive base case) covers odd *and* even lengths, because the odd case stops when they meet
and the even case stops when they cross. **This exact bug is in your
`../10_Recursion/palindrome.cpp:15`** — see that folder's `solution.md`.

**Q4. How do you find the longest palindromic substring without DP?**
Expand around centre. A palindrome is defined by its centre, and there are `2n-1` of them —
`n` single characters and `n-1` gaps between characters. Try each, expand while the ends match,
keep the best. **O(n²) time, O(1) space** — better space than the O(n²) DP table, and easier to
get right. Manacher's is O(n) but is not expected in a normal interview.

**Q5. What is the LPS array and why does it make substring search linear?**
`lps[i]` is the length of the longest proper prefix of `s[0..i]` that is also a suffix of it.
On a mismatch at pattern position `j`, instead of restarting at `j = 0`, you jump to
`lps[j-1]` — because those characters are *known* to match already. The text pointer never
moves backwards, so KMP is **O(n+m)** rather than O(n·m).

**Q6. `s.substr(1)` in a recursion — what's wrong with it?**
`substr` copies. Passing `s.substr(1)` down n levels copies n, n-1, n-2 … characters, which is
**O(n²) time and O(n²) total allocation**. Pass the original string by `const&` plus an index
instead: same logic, O(n).

**Q7. When would you actually use `unordered_map<char,int>` over `int freq[26]`?**
When the key space is large or unknown — full Unicode, arbitrary words (group anagrams keyed by
a sorted word), or when you need to iterate only the keys that are present. For a fixed small
alphabet the array wins on every axis.

---

## Your original notes (preserved)

The problem list that was in this file before is kept verbatim below. All three are covered —
see `questions.md` §1 and `solution.md`.

```
# Leetcode question
- 242 Valid Anagram
- 14 longest common prefix
- 205 Isomorphic
```

| Your entry | Now at |
|---|---|
| 242 Valid Anagram | `questions.md` §1 #1 — you already have a working `anagram()` in `string.cpp:23` |
| 14 Longest Common Prefix | `questions.md` §1 #4 |
| 205 Isomorphic Strings | `questions.md` §1 #5 |
