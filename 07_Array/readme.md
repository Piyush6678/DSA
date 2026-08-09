# 07 — Arrays & Vectors

> **Striver A2Z mapping:** **all of Step 3 — "Solve Problems on Arrays" (Easy → Medium → Hard)**,
> minus the three matrix problems, which live in `../08_2d array`.
> Prerequisites: `../01_basics` … `../06_Pointer`.

This is the largest topic so far and the one interviews actually run on. Roughly a third of
all interview questions are array questions wearing a costume.

> ### A note on ordering
> Striver introduces **hashing** in Step 1.5, *before* arrays. This repo defers maps and
> sets to `../23_sets` and `../24_maps`, and sorting to `../12_sorting`. So a few optimal
> solutions here need a tool you haven't formally met. Rather than skip those problems —
> they're core — `solution.md` gives the in-scope approach **and** the optimal one, with the
> forward dependency marked. Come back and re-read after `../24_maps`.

---

## 1. C-style arrays

```cpp
int x[7];                    // 7 ints, UNINITIALISED — contains garbage
int y[5] = {1,23,1,5,3};     // explicit
int z[]  = {1,2,3,4,5,6};    // size deduced: 6
int w[5] = {0};              // all five set to 0
int v[5] = {};               // also all zero
```

**Size must be a compile-time constant.**

```cpp
int n; cin >> n;
int arr[n];        // NOT standard C++ — a GCC extension (VLA)
```

`1_array.cpp:33` does this. It compiles here because GCC allows it, but it is **not portable
C++**, it silently allocates on the small stack, and it crashes for large `n`. Use
`vector<int> arr(n);` instead — that's the whole reason vectors exist.

**Getting the length:**

```cpp
int s = sizeof(x) / sizeof(x[0]);    // works ONLY in the scope where x was declared
```

Inside a function this silently gives `1`, because the array decayed to a pointer — see
`../06_Pointer` §5, where I measured `40` in `main` and `4` in a function. This is why
`2_array.cpp:6` (`display(int a[])`) has to hardcode `5`.

---

## 2. `std::vector` — use this instead

```cpp
#include <vector>

vector<int> v;                 // empty
vector<int> v(n);              // n elements, all 0        <- runtime size, no VLA needed
vector<int> v(n, 7);           // n elements, all 7
vector<int> v = {1,2,3};       // initialiser list

v.push_back(4);                // append
v.pop_back();                  // remove last
v.size();                      // O(1) — it knows its own length
v.empty();
v[i];                          // NO bounds check
v.at(i);                       // throws out_of_range — slower, safer
```

**`push_back` is amortised O(1).** When the vector runs out of capacity it allocates a
bigger block (typically 2×) and copies everything — an O(n) operation. But because the
capacity doubles, that cost is spread over the next n insertions, so the *average* is O(1).
Use `v.reserve(n)` when you know the final size to avoid the reallocations entirely.

### The out-of-bounds trap

```cpp
vector<int> v;
v.push_back(6); v.push_back(1); v.push_back(4); v.push_back(2);   // size is 4: indices 0..3
cout << v[4];      // UNDEFINED BEHAVIOUR — not "a garbage value"
v[4] = 3;          // WORSE — writes past the end, corrupting the heap
```

`2_array.cpp:47-51` does exactly this and the comment calls `v[4]` a *"garbage value"*. It
isn't: reading is undefined behaviour, and **writing** `v[4] = 3` corrupts the allocator's
memory. It may appear to work and then crash somewhere unrelated. To grow a vector use
`push_back` or `resize`; `operator[]` never grows it.

---

## 3. Passing arrays and vectors to functions

```cpp
void f(int arr[], int n);         // C array: decays to int*, MUST pass the length
void f(vector<int> v);            // COPIES the whole vector — usually a bug
void f(vector<int> &v);           // by reference: no copy, can modify
void f(const vector<int> &v);     // by reference: no copy, read-only  <- the default
```

`2_array.cpp:16` (`lastOccurence(vector<int> v, int trgt)`) takes the vector **by value**,
copying every element on every call. For a 10⁵-element vector inside a loop that's a real
performance bug. The fix is one character: `const vector<int> &v`.

---

## 4. Iterating

```cpp
for (int i = 0; i < (int)v.size(); ++i) cout << v[i];   // index — when you need i
for (int x : v) cout << x;                              // range-for — read only
for (int &x : v) x *= 2;                                // range-for by reference — modifies
```

**Cast `v.size()` when comparing against an `int`.** `size()` returns an *unsigned* type, so
`i < v.size()` with a negative `i` compares as a huge positive number. The classic bug:

```cpp
for (int i = 0; i < v.size() - 1; ++i)    // if v is EMPTY, v.size()-1 wraps to 4294967295
```

---

## 5. The seven patterns that solve most array problems

| Pattern | Signature | Typical use |
|---|---|---|
| **Two pointers (opposite ends)** | `l = 0, r = n-1`, move inward | reverse, palindrome, 2-sum on sorted, trapping rain water |
| **Two pointers (same direction)** | slow/fast, both forward | remove duplicates, move zeroes, partition |
| **Sliding window** | expand `r`, shrink `l` | longest/shortest subarray with a property |
| **Prefix sum** | `pre[i] = pre[i-1] + a[i]` | range sums, subarray-sum-equals-K |
| **Kadane's** | running sum, reset at 0 | maximum subarray |
| **Dutch national flag** | `lo`, `mid`, `hi` | sort 0/1/2, three-way partition |
| **Moore's voting** | candidate + counter | majority element |

Nearly every problem in `questions.md` is one of these seven, sometimes two combined.
**Recognising the pattern is the skill** — the code is short once you have.

### Two more that are really preprocessing steps

- **Sort first** (`../12_sorting`) — turns "find a pair" into a two-pointer scan, turns
  "group duplicates" into "look at neighbours". Costs O(n log n), usually worth it.
- **Hash it** (`../24_maps`) — trades O(n) space for O(1) lookup. This is what turns 2-Sum
  from O(n²) into O(n).

---

## 6. Complexity you should expect

For `n` up to:

| n | Acceptable |
|---|---|
| 10⁸ | O(n) only |
| 10⁷ | O(n) |
| 10⁶ | O(n), O(n log n) |
| 10⁵ | O(n log n) |
| 10⁴ | O(n²) |
| 500 | O(n³) |
| 25 | O(2ⁿ) |

**Read `n` from the constraints and pick your target complexity before you start coding.**
If `n = 10⁵` you know immediately that O(n²) will time out, which tells you the intended
solution is a single pass, a sort, or a hash — and that's most of the way to the answer.

---

## 7. Bugs this folder exists to prevent

**Out-of-bounds access.** `v[4]` on a 4-element vector. Undefined behaviour, and writing is
worse than reading.

**`int max = -1`.** `1_array.cpp:51` initialises the running maximum to `-1`, which returns
`-1` for an all-negative array. Use `INT_MIN`, or `a[0]`.

**Additive swap.** `3_array.cpp:3` implements swap as `*a=*a+*b; *b=*a-*b; *a=*a-*b;`. This
**silently zeroes the element when both pointers are the same**, which I verified — and it
overflows for large values. See the bugs section in `solution.md`; it produces a wrong
answer for `{0,1,2}`.

**Copying vectors by value** into functions.

**Unsigned `size()` in comparisons.**

**Losing the original array.** In-place algorithms destroy their input. If you need it
later, copy first.

---

## Interview Q&A

### Q1. Array or `vector` — which do you use, and how does `vector` grow?

Use `std::vector` in essentially all C++ code. A raw array's size must be a compile-time
constant, it decays to a pointer the moment you pass it anywhere (losing its length), and it
gives you no bounds checking, no `size()`, and no ability to grow.

`vector` fixes all of that at effectively zero cost: elements are stored contiguously, so
indexing is the same single pointer offset and it's equally cache-friendly.

**Growth is the part people get wrong.** `push_back` is **amortised O(1)**, not O(1). When
size reaches capacity, the vector allocates a larger block — typically 2× — copies or moves
every element, and frees the old block. That single operation is O(n).

The amortisation argument: doubling means that inserting n elements triggers reallocations
at sizes 1, 2, 4, 8, … n, and those copies total `1+2+4+…+n < 2n`. So n insertions cost
O(n) overall — O(1) each on average.

Two practical consequences worth volunteering:
- **`reserve(n)` when you know the final size** — it does one allocation and eliminates
  every reallocation.
- **Any reallocation invalidates all pointers, references and iterators** into the vector.
  Holding `&v[0]` across a `push_back` is a dangling-pointer bug.

---

### Q2. When does the two-pointer technique apply, and what are its two forms?

Two pointers works when you can **eliminate possibilities without checking them** — that is,
when some ordering or monotonicity lets you rule out a whole region in one step.

**Form 1 — opposite ends, converging.** `l = 0`, `r = n-1`, move inward. Requires the array
to be **sorted** (or otherwise ordered). Two-Sum on a sorted array is the canonical case: if
`a[l] + a[r]` is too small, no pair involving `a[l]` can work — because `a[r]` is already the
largest — so you advance `l` and discard n−1 pairs in one comparison. That's what turns O(n²)
into O(n). Also: reversing, palindrome checks, container-with-most-water, trapping rain water.

**Form 2 — same direction, slow and fast.** Both move forward; the slow pointer marks where
the next kept element goes, the fast one scans. This is the **in-place partition** idiom —
remove duplicates, move zeroes, remove element. The invariant is "everything before `slow` is
already correct", which is what makes it O(1) space.

The distinguishing question is: *does moving a pointer let me discard candidates I'll never
need to revisit?* If yes, two pointers; if I might need to come back, it's probably a sliding
window or a hash map instead.

---

### Q3. Explain Kadane's algorithm and why the reset works.

Kadane's finds the maximum-sum contiguous subarray in one pass:

```cpp
int best = INT_MIN, cur = 0;
for (int x : a) { cur += x; best = max(best, cur); if (cur < 0) cur = 0; }
```

**The insight is about prefixes, not subarrays.** `cur` is the best sum of a subarray ending
at the current position. When extending to the next element, you have two choices: continue
the previous subarray, or start fresh. Continuing is better exactly when the previous sum is
positive — a negative running sum can only drag down whatever follows, so you drop it.

That's why `if (cur < 0) cur = 0;` is correct: **any prefix with a negative sum is never
worth keeping.** It is not a heuristic; it's a proof that no optimal subarray starts with a
negative-sum prefix.

**The all-negative case is the interview trap.** Initialising `best = 0` returns 0 for
`[-3,-1,-2]`, but the correct answer is `-1` — the least-bad single element. Initialise
`best = INT_MIN` (or `a[0]`) and take the max **before** the reset. I verified both cases:
`[-2,1,-3,4,-1,2,1,-5,4]` → 6, `[-3,-1,-2]` → −1.

**O(n) time, O(1) space.** To recover the actual subarray, record the start index whenever
you reset and the end index whenever you update `best`.

---

### Q4. Why does sorting so often unlock an array problem?

Because sorting converts *global* questions into *local* ones. After sorting:

- **Duplicates become adjacent** — finding them is a single neighbour comparison instead of
  an all-pairs search.
- **Order becomes monotonic** — which is the precondition for both binary search (O(log n)
  lookup) and the converging two-pointer scan (O(n) pair finding).
- **Greedy choices become safe** — in Merge Intervals, sorting by start time means any
  interval that overlaps the current one must be the *next* one, so a single pass suffices.

The trade-off is the O(n log n) cost and, if you sort in place, the loss of original indices.
That second point decides real problems: **2-Sum asks for indices**, so sorting destroys the
answer unless you pair each value with its index first — which is exactly why the hash-map
solution is preferred there. 3-Sum asks for *values*, so sorting is free and is the standard
approach.

So the decision rule is: sort when you need order or adjacency and the output doesn't depend
on original positions; hash when you need O(n) or must preserve indices.

---

### Q5. What's the trade-off when you reach for a hash map?

You are buying **time with space**: O(n) extra memory for O(1) average lookup.

The canonical case is 2-Sum. Brute force is O(n²): for each element, scan for its complement.
With a hash map you store each value as you pass it and ask "have I already seen
`target - a[i]`?" — one pass, O(n) time, O(n) space.

Three caveats worth raising unprompted:

1. **O(1) is average, not worst case.** Hash collisions degrade `unordered_map` to O(n) per
   operation. Adversarial inputs can trigger this deliberately — a real concern on
   Codeforces, where people hack `unordered_map` solutions. `map` (a balanced tree) is a
   guaranteed O(log n) and is often faster in practice for small n.
2. **The constant factor is large.** Hashing, allocation and pointer chasing mean an
   `unordered_map` pass can lose to an O(n log n) sort on real data.
3. **Sometimes you don't need the map.** If the values are a bounded range — 0..25 for
   letters, 0..n for a permutation — a plain array indexed by value is a perfect hash: O(1)
   guaranteed, tiny constant. Missing Number is even better: use the arithmetic sum, which is
   O(1) space.

---

### Q6. What are prefix sums and which problems do they solve?

`pre[i]` is the sum of `a[0..i]`. Building it is one O(n) pass, after which the sum of any
range `[l, r]` is `pre[r] - pre[l-1]` in **O(1)**.

That alone answers range-sum queries. The more powerful use is the **prefix-sum + hash map**
pattern for counting subarrays with a given sum:

> A subarray `(l, r]` sums to `k` exactly when `pre[r] - pre[l] == k`, i.e.
> `pre[l] == pre[r] - k`.

So as you sweep `r`, you ask the map how many earlier prefixes equalled `pre[r] - k`. One
pass, O(n) time and space. That single idea solves Subarray Sum Equals K (LeetCode 560),
longest subarray with sum K, largest subarray with zero sum (`k = 0`), and — replacing `+`
with XOR — count subarrays with XOR K.

**Why not a sliding window?** A window only works when all elements are **non-negative**, so
that growing the window can only increase the sum and shrinking can only decrease it. With
negative numbers that monotonicity is gone and the window breaks. **Non-negative → sliding
window, O(1) space. Any signs → prefix sum + map, O(n) space.** Knowing which applies is the
actual question being asked.

---

### Q7. What does "O(1) extra space" mean, and when is an in-place algorithm worth it?

It means the memory you use beyond the input doesn't grow with `n` — a constant number of
scalars. The input array itself doesn't count, and modifying it is allowed unless the
problem says otherwise.

Interviewers ask for it because it forces a genuinely different algorithm rather than a
copy-and-massage. Sort Colors in O(1) space requires the Dutch national flag three-pointer
partition; with an extra array it's a trivial counting pass. Rotating an array in O(1) space
requires the reverse-reverse-reverse trick; with a temp array it's a copy.

Two things to be precise about:

- **Recursion is not free.** A recursive solution uses O(depth) stack space. Merge sort is
  O(n) space; quicksort is O(log n) *stack* even though it partitions in place.
- **"In place" and "O(1) space" are not identical.** In place means you overwrite the input;
  you can still use O(log n) auxiliary space and be in place.

The trade-off is real: in-place algorithms destroy the input and are usually harder to get
right. The two clean wins are the ones above, plus using the input array itself as your hash
table when values are bounded — as Set Matrix Zeroes does by storing flags in row 0 and
column 0 (`../08_2d array`).

---

## Common mistakes checklist

- [ ] `vector<int> v(n)` instead of a VLA `int arr[n]`
- [ ] Index in `[0, size)` — `v[size]` is undefined behaviour
- [ ] `const vector<int>&` parameters, not by value
- [ ] `INT_MIN` (or `a[0]`) for a running maximum, never `-1` or `0`
- [ ] `(int)v.size()` when comparing against a signed value
- [ ] Empty-array case handled before touching `a[0]`
- [ ] `long long` for sums — n elements of 10⁹ overflows `int`
- [ ] `std::swap`, never the additive trick
- [ ] Original array copied if an in-place algorithm will destroy it

---

## Your original notes (preserved)

*The content below is your own `readme.md` from before this doc set was added, kept verbatim.
Your brute-force Two Sum is solution #11 in `solution.md`; the merge is #27; next permutation
is #17 and trapping rain water is #30.*

```
# Leet Code Questions
- 1 Two sum
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> ans;
        for (int i=0;i<nums.size();i++){
            for (int j=i+1;j<nums.size();j++){
                if (nums[i]+nums[j]==target) {
                    ans.push_back(i);
                    ans.push_back(j);
                    return ans;
                }
            }
        }
        return ans;
    }
};
- 2 merge two sorted array
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int i =m-1,j=n-1,k=m+n-1;
        while(i>=0 &&j>=0){
            if(nums1[i]>nums2[j]){ nums1[k]=nums1[i]; i--;k--; }
            else{ nums1[k]=nums2[j]; k--;j--; }
        }
        while(j>=0){ nums1[k]=nums2[j]; j--;k--; }
    }
};
- 3 nextpermutation 31
- 4 traping rain water 42
```

**Both of those solutions are correct.** The merge is the standard fill-from-the-back
approach, and writing it backwards — rather than forwards into a temp array — is exactly the
right instinct, because it's the only way to do it in O(1) extra space. See `solution.md` #27.

---

## Next

`questions.md` → `solution.md` → `../08_2d array`.
