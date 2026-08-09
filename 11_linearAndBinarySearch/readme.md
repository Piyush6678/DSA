# 11 — Linear & Binary Search

Binary search is the highest-value/lowest-code topic in the whole repo. Ten lines, and it turns
O(n) into O(log n) — but those ten lines have about six places to get it subtly wrong, and a
buggy binary search usually *looks* like it works. Five of the six functions in
`binary search.cpp` are wrong, and one of them hangs. That is normal, and it is why the
invariant below matters more than memorising the code.

---

## 1. Linear search — and when it is the right answer

```cpp
int linearSearch(vector<int>& a, int t) {
    for (int i = 0; i < (int)a.size(); ++i) if (a[i] == t) return i;
    return -1;
}
```

O(n), works on **unsorted** data. Do not dismiss it: if you search an array once, sorting it
first (O(n log n)) to enable binary search is *slower* than a single linear scan. Binary search
pays off when the data is already sorted, or when you will search it many times.

---

## 2. The one invariant

> **The answer, if it exists, is always inside `[lo, hi]`.**

Every line either preserves that or the code is broken. Work through the three moves:

- `mid` is examined. It is not the answer, so it can be excluded → `lo = mid+1` or `hi = mid-1`.
- The range must **shrink every iteration**, or you loop forever.
- The loop ends when the range is empty (`lo > hi`) or has converged (`lo == hi`).

```cpp
int binarySearch(vector<int>& a, int target) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo <= hi) {                        // <= : a one-element range still needs checking
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == target) return mid;
        if (a[mid] < target)  lo = mid + 1;
        else                  hi = mid - 1;
    }
    return -1;
}
```

### `mid = lo + (hi - lo) / 2` — why not something else

| Form | Verdict |
|---|---|
| `(lo + hi) / 2` | correct value, but **overflows** when `lo + hi > INT_MAX` |
| `lo + (hi - lo) / 2` | **use this** — same value, no overflow |
| `lo/2 + hi/2` | **wrong** — truncates each half separately |

That last one is the bug in five of your functions. Each division throws away a fraction, so it
can land **below `lo`** — outside the search range entirely:

| `lo` | `hi` | `lo/2 + hi/2` | correct | |
|---|---|---|---|---|
| 1 | 1 | **0** | 1 | below `lo` |
| 1 | 3 | 1 | 2 | |
| 3 | 3 | **2** | 3 | below `lo` |
| 5 | 5 | **4** | 5 | below `lo` |

When `mid < lo`, `lo = mid + 1` does not advance and `hi = mid - 1` jumps backwards past the
answer. Both loop shapes fail, and which one you get depends on the data — which is exactly
why it passes some tests.

### `while (lo <= hi)` vs `while (lo < hi)`

Both are correct **for their own shape**, and mixing them is the second classic bug:

| | `lo <= hi` | `lo < hi` |
|---|---|---|
| `hi` starts at | `n - 1` | `n - 1` or `n` |
| Excludes `mid` | yes — `hi = mid - 1` | **no** — `hi = mid` |
| Ends with | `lo > hi`, empty | `lo == hi`, one candidate |
| Use for | "is it present?" | "find the boundary" |

`while (lo < hi)` with `hi = mid` **must not** also use `hi = mid - 1`, and `while (lo <= hi)`
with `hi = mid` loops forever. Pick a shape and keep it consistent.

---

## 3. Lower bound and upper bound — build everything else on these

```cpp
int lowerBound(vector<int>& a, int x) {       // first index with a[i] >= x
    int lo = 0, hi = (int)a.size();           // hi = n, so "not found" answers n
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] >= x) hi = mid;            // mid might BE the answer — keep it
        else             lo = mid + 1;
    }
    return lo;
}
```

`upperBound` is the same with `>` instead of `>=` (first index with `a[i] > x`).

Once you have both, a surprising amount is free:

| Want | Expression |
|---|---|
| First occurrence of x | `lowerBound(a,x)`, then check `a[i] == x` |
| Last occurrence of x | `upperBound(a,x) - 1` |
| Count of x | `upperBound(a,x) - lowerBound(a,x)` |
| Insert position | `lowerBound(a,x)` |
| Number of elements < x | `lowerBound(a,x)` |

Learning these two properly is worth more than learning ten problems separately. Note `hi = n`,
not `n - 1` — the answer may legitimately be "past the end", and the range must be able to
express that.

---

## 4. Binary search on the **answer**, not the array

This is the pattern that does not look like binary search, and it is roughly half of all
binary-search interview questions — including four of the problems on your own list (875, 1011,
2187, 410).

**The shape.** You are asked for a minimum (or maximum) value that satisfies some condition.
You cannot compute it directly, but given a candidate you can *check* it in O(n). If the
condition is **monotonic** — true for everything above some threshold and false below it — the
search space is a sorted boolean array and you can binary search it.

```
speed:      1     2     3     4     5     6
feasible?   F     F     F     T     T     T
                              ^ binary search for this boundary
```

```cpp
int lo = <smallest possible answer>, hi = <largest possible answer>;
while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (feasible(mid)) hi = mid;      // works: try smaller
    else               lo = mid + 1;  // fails: must go bigger
}
return lo;                            // lo == hi == the boundary
```

**Three things to get right, in order of how often they are wrong:**

1. **`feasible()` must actually use the candidate.** If it ignores its parameter the whole
   search is meaningless — the loop still runs and still returns *a* number. `check()` in
   `kokoEatingBanana.cpp` never uses `speed`, and the function still returns an answer, just the
   wrong one. Test `feasible()` on its own before wiring it up.
2. **The bounds must be tight and legal.** For ship capacity, `lo` is the *heaviest package*,
   not 1 — a capacity smaller than the largest item can never work no matter how many days.
3. **Monotonicity.** If feasibility flips back and forth, binary search is invalid and you
   have not proved anything by getting the right answer on one test.

Complexity is `O(n log(range))` — the `log` of the *value* range, not the array length.

---

## 5. Overflow, and why `mid * mid` is a trap

`sqrt(x)` by binary search invites `if (mid * mid <= x)`. For `x` near `INT_MAX`, `mid` reaches
about 46341 and `mid * mid` overflows a 32-bit `int` — **undefined behaviour**, and with
optimisation the compiler may assume it cannot happen. Two safe forms:

```cpp
if (mid <= x / mid)              { ... }   // division never overflows
if ((long long)mid * mid <= x)   { ... }   // widen before multiplying
```

Same discipline as `myAtoi` in `../09_Strings` and `nCr` in `../05_Function`: **check before
you overflow, not after.**

---

## Interview Q&A

**Q1. Why `lo + (hi - lo) / 2` instead of `(lo + hi) / 2`?**
They compute the same value, but `lo + hi` can exceed `INT_MAX` when both are large, and signed
overflow is undefined behaviour. `hi - lo` is a non-negative difference that always fits. This
is a famous bug — it sat in Java's own `Arrays.binarySearch` for nine years.

**Q2. When do you use `lo <= hi` and when `lo < hi`?**
`lo <= hi` when you are looking for an exact match and will return from inside the loop; the
range shrinks by excluding `mid`. `lo < hi` when you are looking for a *boundary* and `mid`
might be the answer, so you write `hi = mid` — the loop then converges on a single index that
you return. The fatal combination is `lo <= hi` with `hi = mid`, which never terminates.

**Q3. Binary search needs a sorted array. What is the real requirement?**
Not sortedness — **monotonicity of the predicate you are testing**. `a[i] < target` must be
true for a prefix and false for the rest. That is why binary search works on a rotated array
(one comparison identifies which half is sorted), on a mountain array (the "am I still
ascending?" predicate is monotonic), and on an answer space that is not an array at all.

**Q4. Explain "binary search on the answer".**
When you cannot compute the answer directly but can *verify* a candidate in O(n), and
feasibility is monotonic, binary search the value range instead of the array. Koko's bananas,
ship capacity, and split-array-largest-sum are all the same problem with different `feasible()`
functions. Complexity O(n log(range)).

**Q5. How do you find the first and last occurrence of a value in O(log n)?**
Two binary searches. For the first, when `a[mid] == target` do **not** return — record it and
continue searching left (`hi = mid - 1`). For the last, continue right. Or use
`lowerBound` / `upperBound - 1` and get both from primitives you already trust.

**Q6. Why does searching a rotated array still work?**
At any `mid`, **at least one half is guaranteed sorted** — compare `a[lo]` with `a[mid]` to find
out which. If the target lies inside that sorted half's range, search it; otherwise search the
other. You never need to find the pivot first, which removes an entire class of bug. With
duplicates (LC 81) the trick fails when `a[lo] == a[mid] == a[hi]`, and the only fix is to
shrink both ends by one — worst case O(n).

**Q7. What is the space complexity of binary search?**
O(1) iteratively. **O(log n)** recursively, from the call stack — the recursive version is not
free, and interviewers ask about this specifically.

---

## Your original notes (preserved)

The problem list from this file is kept verbatim. **All ten are covered** — see `questions.md`.

```
Leetcode Quesation 
704 -Binary Search
34 -first and last occurence
69 sqrt x 
852 peak index in mountain array
33 Search in rotated sorted array 
658 K Closest Elements
633 SUM OF SQUARES
1011 capacity to ship within d days 
875 koko eating banaa
2187 min time to complete Trips
```

| Your entry | Now at | Your file |
|---|---|---|
| 704 Binary Search | `questions.md` §1 #1 | `binary search.cpp:5` — wrong `mid`, wrong loop guard |
| 34 First and Last Occurrence | `questions.md` §1 #4 | `binary search.cpp:45` — **hangs** |
| 69 Sqrt(x) | `questions.md` §1 #6 | `binary search.cpp:79` — compares against `hi` |
| 852 Peak Index in Mountain Array | `questions.md` §1 #7 | `peakmountainindex.cpp` — fails on length 3 |
| 33 Search in Rotated Sorted Array | `questions.md` §1 #8 | `rotatedsortedarray.cpp` — uninitialised `pivot` |
| 658 K Closest Elements | `questions.md` §2 #16 | `kclosest.cpp` — **correct `mid`**, see solution.md |
| 633 Sum of Square Numbers | `questions.md` §2 #17 | `sumOfSquares.cpp` — **correct** |
| 1011 Capacity to Ship | `questions.md` §1 #12 | `ship(1011).cpp` — no `return` statement |
| 875 Koko Eating Bananas | `questions.md` §1 #11 | `kokoEatingBanana.cpp` — `check()` ignores speed |
| 2187 Min Time to Complete Trips | `questions.md` §1 #13 | `timeToCompleteTrips.cpp` — division inverted |
