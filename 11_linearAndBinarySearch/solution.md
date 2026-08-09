# 11 — Linear & Binary Search: Solutions

Full solutions for Sections 1–3 of `questions.md`. **Every function below was compiled with
`g++ -std=gnu++14` and executed against the test cases shown — 137 assertions, all passing.**
The "Verified" lines are measured outputs.

Assume `#include <vector>`, `#include <algorithm>`, `#include <climits>`, `using namespace std;`.

---

# Section 1 — Must Do

## 1. Binary Search — LeetCode 704

```cpp
int binarySearch(vector<int>& a, int target) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == target) return mid;
        if (a[mid] < target)  lo = mid + 1;
        else                  hi = mid - 1;
    }
    return -1;
}
```

**Key insight.** Three things, all of which your version gets wrong (see the bugs section):
`mid = lo + (hi - lo) / 2` (not `lo/2 + hi/2`), `while (lo <= hi)` (not `lo < hi`), and each
branch excluding `mid` so the range strictly shrinks.

**Complexity.** O(log n) time, O(1) space. Recursively it is O(log n) space from the stack.

**Verified:** finds every element of `{1..7}` at the right index; `{-1,0,3,5,9,12}` finds 9 at 4,
first at 0, last at 5, absent → −1; size-1 hit and miss; empty array → −1.

---

## 2. Lower Bound and Upper Bound

```cpp
int lowerBound(vector<int>& a, int x) {        // first index with a[i] >= x
    int lo = 0, hi = (int)a.size();            // hi = n, not n-1
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] >= x) hi = mid;             // mid may be the answer — do not exclude it
        else             lo = mid + 1;
    }
    return lo;
}
int upperBound(vector<int>& a, int x) {        // first index with a[i] > x
    int lo = 0, hi = (int)a.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] > x) hi = mid;
        else            lo = mid + 1;
    }
    return lo;
}
```

**Key insight.** `hi = n`, not `n - 1`. The answer can legitimately be "one past the end" (every
element is smaller than `x`), and the range has to be able to represent that. Pair that with
`while (lo < hi)` and `hi = mid`, and the loop converges on the boundary rather than falling off
it.

**Do not mix the shapes.** `hi = mid` inside `while (lo <= hi)` never terminates: when
`lo == hi == mid`, `hi = mid` changes nothing.

**Complexity.** O(log n) time, O(1) space.

**Verified:** on `{1,3,5,7,9}` — `lowerBound(5)=2`, `lowerBound(4)=2`, `lowerBound(1)=0`,
`lowerBound(0)=0`, `lowerBound(10)=5` (past the end), `upperBound(5)=3`, `upperBound(0)=0`.
On `{2,2,2}` — `lowerBound(2)=0`, `upperBound(2)=3`.

---

## 3. Search Insert Position — LeetCode 35

```cpp
int searchInsert(vector<int>& a, int t) { return lowerBound(a, t); }
```

**Key insight.** It is `lowerBound` and nothing else. The position where `t` belongs is exactly
the first index holding something ≥ `t`, whether or not `t` is present.

**Verified:** `{1,3,5,6}` → insert 5 at 2, 2 at 1, 7 at 4, 0 at 0.

---

## 4. First and Last Position — LeetCode 34

```cpp
vector<int> searchRange(vector<int>& a, int t) {
    int lo = lowerBound(a, t);
    if (lo == (int)a.size() || a[lo] != t) return {-1, -1};   // t is absent
    int hi = upperBound(a, t) - 1;
    return {lo, hi};
}
int countOccurrences(vector<int>& a, int t) { return upperBound(a,t) - lowerBound(a,t); }
```

**Key insight.** Built entirely from #2 — no new binary search to get wrong. The presence check
must test `lo == size()` **first**, because `a[lo]` would be out of bounds otherwise; `||`
short-circuits, so the order is load-bearing.

**Writing it directly** (what LeetCode expects if asked for one function): on `a[mid] == t`, do
not return — record `mid` and keep going left with `hi = mid - 1` for the first occurrence,
right with `lo = mid + 1` for the last.

**Complexity.** O(log n) time, O(1) space.

**Verified:** `{5,7,7,8,8,10}` → 8 gives `[3,4]`, 7 gives `[1,2]`, 6 gives `[-1,-1]`, 5 gives
`[0,0]` (first element), 10 gives `[5,5]` (last element); count of 8 is 2; empty array →
`[-1,-1]`; `{5,5,5}` → `[0,2]`.

---

## 5. Count Occurrences

```cpp
int countOccurrences(vector<int>& a, int t) { return upperBound(a,t) - lowerBound(a,t); }
```

**Key insight.** The count falls out of the two bounds with no extra work, and it is correct
when `t` is absent too — both bounds return the same index, giving 0.

---

## 6. Sqrt(x) — LeetCode 69

```cpp
int mySqrt(int x) {
    if (x < 2) return x;                       // 0 and 1 are their own roots
    int lo = 1, hi = x / 2, ans = 1;           // for x >= 2 the root never exceeds x/2
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (mid <= x / mid) { ans = mid; lo = mid + 1; }   // i.e. mid*mid <= x, safely
        else hi = mid - 1;
    }
    return ans;
}
```

**Key insight — the overflow.** The natural `if (mid * mid <= x)` **overflows** for `x` near
`INT_MAX`: `mid` reaches ~46341 and `mid * mid` exceeds 2,147,483,647. That is undefined
behaviour, not merely a wrong number. Dividing instead (`mid <= x / mid`) keeps every
intermediate in range. `(long long)mid * mid <= x` is equally fine.

`ans` carries the last feasible value, so you never need `hi` to mean anything at the end.

**Complexity.** O(log x) time, O(1) space.

**Verified:** `0→0`, `1→1`, `2→1`, `4→2`, `8→2`, `9→3`, `16→4`, `2147395599→46339`,
`2147483647→46340` (no overflow), and **exhaustively for every x from 0 to 10000** against
`r*r <= x < (r+1)*(r+1)`.

---

## 7. Peak Index in a Mountain Array — LeetCode 852

```cpp
int peakIndexInMountainArray(vector<int>& a) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < a[mid + 1]) lo = mid + 1;   // still ascending: peak is strictly right
        else                     hi = mid;       // descending: mid might BE the peak
    }
    return lo;                                   // lo == hi
}
```

**Key insight.** There is no target to compare against — you search on a **predicate**,
"am I still ascending?", which is true for a prefix and false after. That is all binary search
needs.

**Why `lo < hi` matters here specifically.** With `hi = size()-1` and `lo < hi`, `mid` is always
strictly less than `hi`, so `a[mid + 1]` is **always in bounds**. You never need a guard, and you
never read `a[mid - 1]`. Your version compares against both neighbours, which is what forces the
out-of-bounds read on a 3-element array.

**Complexity.** O(log n) time, O(1) space.

**Verified:** `{0,1,0}→1` (the length-3 case yours fails), `{0,2,1,0}→1`, `{0,10,5,2}→1`,
`{3,4,5,1}→2`, and a length-10 mountain → 2.

---

## 8. Search in Rotated Sorted Array — LeetCode 33

```cpp
int searchRotated(vector<int>& a, int t) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == t) return mid;
        if (a[lo] <= a[mid]) {                          // LEFT half is sorted
            if (a[lo] <= t && t < a[mid]) hi = mid - 1; // target is inside it
            else                          lo = mid + 1;
        } else {                                        // RIGHT half is sorted
            if (a[mid] < t && t <= a[hi]) lo = mid + 1;
            else                          hi = mid - 1;
        }
    }
    return -1;
}
```

**Key insight — you never need the pivot.** However the array is rotated, a cut at `mid` leaves
**at least one half fully sorted**, and `a[lo] <= a[mid]` tells you which. Inside a sorted half
you can test membership with two comparisons; if the target is not there, it must be in the
other half. One pass, no pivot hunt.

That matters because pivot-finding is where the bugs live — `rotatedsortedarray.cpp` spends 25
lines on it and reads an uninitialised variable.

Note `a[lo] <= a[mid]` uses `<=`, not `<`: when `lo == mid` (a two-element range) the left half
is trivially sorted and `<` would misclassify it.

**Complexity.** O(log n) time, O(1) space.

**Verified:** `{4,5,6,7,0,1,2}` → 0 at index 4, 4 at 0, 2 at 6, absent 3 → −1; `{1}` → 0;
`{3,1}` finds 1 at index 1; and on a **non-rotated** `{1,2,3,4,5}` every element is found.

---

## 9. Find Minimum in Rotated Sorted Array — LeetCode 153

```cpp
int findMin(vector<int>& a) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] > a[hi]) lo = mid + 1;   // minimum is strictly right of mid
        else                hi = mid;       // mid could be the minimum
    }
    return a[lo];
}
int countRotations(vector<int>& a) { /* identical, but return lo */ }
```

**Key insight — compare against `a[hi]`, not `a[lo]`.** This is the detail people get wrong.
`a[mid] > a[hi]` means the drop happens after `mid`, so the minimum is to the right.
Comparing against `a[lo]` fails on an array that is **not rotated at all**, where `a[mid] > a[lo]`
is true everywhere and you walk right past the answer at index 0.

The index of the minimum is also the **number of rotations**, which is a free bonus.

**Complexity.** O(log n) time, O(1) space.

**Verified:** `{3,4,5,1,2}→1`, `{4,5,6,7,0,1,2}→0` with 4 rotations, `{11,13,15,17}→11` with
0 rotations (the unrotated case).

---

## 10. Single Element in a Sorted Array — LeetCode 540

```cpp
int singleNonDuplicate(vector<int>& a) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (mid % 2 == 1) --mid;                 // snap to the START of a pair
        if (a[mid] == a[mid + 1]) lo = mid + 2;  // pairing still intact: answer is right
        else                      hi = mid;      // pairing broken at or before mid
    }
    return a[lo];
}
```

**Key insight — the predicate is index parity.** Before the single element, every pair starts at
an **even** index: `(0,1)`, `(2,3)`, … After it, that shifts by one and pairs start at odd
indices. Forcing `mid` to be even and asking "is `a[mid] == a[mid+1]`?" tests exactly that, in
O(1).

**Complexity.** O(log n) time, O(1) space. XOR-ing everything is O(n) — mention it, then give
this.

**Verified:** `{1,1,2,3,3,4,4,8,8}→2`, `{3,3,7,7,10,11,11}→10`, `{1}→1`, `{1,1,2}→2` (at the
end), `{1,2,2}→1` (at the start).

---

## 11. Koko Eating Bananas — LeetCode 875 — **the answer-space archetype**

```cpp
long long hoursNeeded(vector<int>& piles, int speed) {
    long long h = 0;
    for (int p : piles) h += (p + speed - 1) / speed;   // ceil(p / speed), integer-only
    return h;
}
int minEatingSpeed(vector<int>& piles, int H) {
    int lo = 1, hi = *max_element(piles.begin(), piles.end());
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (hoursNeeded(piles, mid) <= H) hi = mid;     // feasible: try slower
        else                              lo = mid + 1;
    }
    return lo;
}
```

**Key insight, three parts.**

1. **`feasible()` must depend on its argument.** `hoursNeeded` is where the actual work happens.
   Your `check()` never reads `speed` — the binary search still runs and still returns a number,
   which is what makes this failure so hard to spot.
2. **`(p + speed - 1) / speed` is integer ceiling division.** Koko cannot eat from two piles in
   the same hour, so a pile of 7 at speed 3 costs 3 hours, not 2.33. Never use
   `ceil(p / (double)speed)` — floating point at these magnitudes is a needless risk.
3. **Bounds.** `lo = 1` (speed 0 makes no progress and divides by zero); `hi = max(piles)` —
   eating faster than the biggest pile cannot help, since each pile still takes a whole hour.

**Complexity.** O(n log(max pile)) time, O(1) space.

**Verified:** `({3,6,7,11}, 8) → 4`, `({30,11,23,4,20}, 5) → 30`, `({30,11,23,4,20}, 6) → 23`,
`({1,1,1,1}, 4) → 1`, and a single huge pile with plenty of hours → 1.

---

## 12. Capacity To Ship Packages Within D Days — LeetCode 1011

```cpp
int daysNeeded(vector<int>& w, int cap) {
    int days = 1, cur = 0;
    for (int x : w) {
        if (cur + x > cap) { ++days; cur = 0; }   // start a new day
        cur += x;
    }
    return days;
}
int shipWithinDays(vector<int>& w, int D) {
    int lo = *max_element(w.begin(), w.end());    // must fit the heaviest single package
    int hi = 0; for (int x : w) hi += x;          // one day: carry everything
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (daysNeeded(w, mid) <= D) hi = mid;
        else                         lo = mid + 1;
    }
    return lo;
}
```

**Key insight — the lower bound is `max(weights)`, not 1.** A capacity below the heaviest package
can never ship it at all, no matter how many days you allow. Starting at 1 doesn't produce a
wrong answer here (those candidates are simply infeasible) but it does mean `daysNeeded` must
cope with a package that doesn't fit, and the version above would silently give it its own day.
Setting `lo` correctly removes the case entirely.

`days` starts at **1**, not 0 — you are already loading on day one before any split.

**Complexity.** O(n log(sum)) time, O(1) space.

**Verified:** `{1..10}` with D=5 → 15, D=1 → 55 (the whole sum), D=10 → 10 (the max element);
`{3,2,2,4,1,4}` D=3 → 6; `{1,2,3,1,1}` D=4 → 3.

---

## 13. Minimum Time to Complete Trips — LeetCode 2187

```cpp
long long tripsIn(vector<int>& t, long long time) {
    long long n = 0;
    for (int x : t) n += time / x;                 // time DIVIDED BY rate
    return n;
}
long long minimumTime(vector<int>& t, int totalTrips) {
    long long lo = 1;
    long long hi = (long long)*min_element(t.begin(), t.end()) * totalTrips;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (tripsIn(t, mid) >= totalTrips) hi = mid;
        else                               lo = mid + 1;
    }
    return lo;
}
```

**Key insight — get the division the right way round.** `time[i]` is how long **one trip** takes
for bus `i`. In `T` units of time that bus completes `T / time[i]` trips. Your version computes
`time[i] / hours`, which is trips-per-unit-time inverted, and gives 0 for every bus whenever the
candidate time exceeds the trip length.

**The upper bound needs `long long`.** `min(time) * totalTrips` — the fastest bus doing all the
trips alone — can reach 10⁷ × 10⁷ = 10¹⁴, far past `int`. Using `max` instead of `min` is also
valid but wastefully large.

**Complexity.** O(n log(answer)) time, O(1) space.

**Verified:** `({1,2,3}, 5) → 3`, `({2}, 1) → 2`, `({5,10,10}, 9) → 25`.

---

# Section 2 — Important

## 14. Search in Rotated Sorted Array II — LeetCode 81

```cpp
bool searchRotated2(vector<int>& a, int t) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == t) return true;
        if (a[lo] == a[mid] && a[mid] == a[hi]) { ++lo; --hi; continue; }   // ambiguous
        if (a[lo] <= a[mid]) {
            if (a[lo] <= t && t < a[mid]) hi = mid - 1; else lo = mid + 1;
        } else {
            if (a[mid] < t && t <= a[hi]) lo = mid + 1; else hi = mid - 1;
        }
    }
    return false;
}
```

**Key insight — where #8's trick breaks.** With duplicates, `a[lo] == a[mid] == a[hi]` gives you
no information about which half is sorted: `{1,1,1,0,1}` and `{1,0,1,1,1}` look identical at the
endpoints. The only sound move is to shrink both ends by one, which costs **O(n) worst case**.
Say that in the interview — pretending it stays O(log n) is the wrong answer.

**Complexity.** O(log n) average, **O(n) worst case**, O(1) space.

**Verified:** `{2,5,6,0,0,1,2}` → finds 0, rejects 3; `{1,0,1,1,1}` → finds 0 (the case that
needs the ambiguity branch).

---

## 15. Find Peak Element — LeetCode 162

```cpp
int findPeakElement(vector<int>& a) {
    int lo = 0, hi = (int)a.size() - 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] < a[mid + 1]) lo = mid + 1;
        else                     hi = mid;
    }
    return lo;
}
```

**Key insight.** Byte-for-byte the same as #7, with **no mountain guarantee** — the array may
have many peaks and you may return any. It still works because the problem states
`a[i] != a[i+1]`, so at every `mid` you are strictly ascending or strictly descending; following
the ascent must terminate at *a* peak (the boundary counts as one, since `a[-1]` and `a[n]` are
treated as −∞).

**Complexity.** O(log n) time, O(1) space.

**Verified:** `{1,2,3,1}→2`, `{1}→0`, `{3,2,1}→0` (peak at the left edge), `{1,2,3}→2` (right edge).

---

## 16. Find K Closest Elements — LeetCode 658

```cpp
vector<int> findClosestElements(vector<int>& a, int k, int x) {
    int lo = 0, hi = (int)a.size() - k;                 // lo indexes the WINDOW START
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (x - a[mid] > a[mid + k] - x) lo = mid + 1;  // right end closer: slide right
        else                             hi = mid;
    }
    return vector<int>(a.begin() + lo, a.begin() + lo + k);
}
```

**Key insight.** The answer is always a **contiguous window** of length `k` (the array is
sorted), so instead of searching for a value you binary search the *window start*, over the range
`[0, n-k]`. The comparison asks: for a window starting at `mid`, is the element just outside on
the right closer to `x` than the element at the left edge? If so, shift right.

Using `x - a[mid] > a[mid+k] - x` rather than `abs()` is deliberate — both quantities are already
correctly signed for a sorted array, and it avoids the tie-breaking ambiguity that `abs` creates
(the problem wants the smaller element on a tie, which `>` gives you for free).

**Complexity.** O(log(n−k) + k) time — far better than the O(n log n) sort-by-distance approach.

**Verified:** `{1,2,3,4,5}` — `k=4,x=3 → {1,2,3,4}`, `k=4,x=-1 → {1,2,3,4}`,
`k=4,x=9 → {2,3,4,5}`, `k=5 → the whole array`; `{1,1,1,10,10,10}` with `k=1,x=9 → {10}`.

---

## 17. Sum of Square Numbers — LeetCode 633

```cpp
bool judgeSquareSum(int c) {
    long long lo = 0, hi = (long long)sqrt((double)c) + 1;
    while (lo <= hi) {
        long long s = lo*lo + hi*hi;
        if (s == c) return true;
        if (s < c)  ++lo;                 // need a bigger sum
        else        --hi;                 // need a smaller sum
    }
    return false;
}
```

**Key insight.** Two pointers on the implicit sorted list of squares — no search structure
needed. `hi` starts at `⌈√c⌉` because neither term can exceed `√c`. Both must be `long long`:
`hi*hi` reaches `c`, and with `c` near `INT_MAX` an `int` product overflows.

**Your `sumOfSquares.cpp` is correct** — verified on c = 0, 1, 2, 3, 4, 5. The approach is
unusual (jumping `y` down to the nearest perfect square rather than decrementing) but it
terminates and gets the right answers. The two-pointer version above is easier to prove correct
and easier to explain.

**Complexity.** O(√c) time, O(1) space.

**Verified:** `c=5→true` (1+4), `c=3→false`, `c=4→true`, `c=2→true` (1+1), `c=0→true`,
`c=1→true`, `c=2147483600→true` (7060² + 45800²), `c=2147483646→false`.

---

## 18–20. Split Array / Book Allocation / Aggressive Cows

**One algorithm, three stories.** Learn it once.

```cpp
int piecesNeeded(vector<int>& a, long long limit) {
    int cnt = 1; long long cur = 0;
    for (int x : a) { if (cur + x > limit) { ++cnt; cur = 0; } cur += x; }
    return cnt;
}
int splitArray(vector<int>& a, int k) {
    long long lo = *max_element(a.begin(), a.end()), hi = 0;
    for (int x : a) hi += x;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (piecesNeeded(a, mid) <= k) hi = mid;   // fits in k or fewer: try a smaller cap
        else                           lo = mid + 1;
    }
    return (int)lo;
}
```

| Problem | "limit" means | Want |
|---|---|---|
| Split Array Largest Sum (410) | max subarray sum | **minimise** |
| Allocate Minimum Pages | max pages per student | **minimise** |
| Painter's Partition | max work per painter | **minimise** |
| **Aggressive Cows** | **min** distance between cows | **maximise** |

**Aggressive cows is the one that flips.** You want the *largest* minimum spacing, so
feasibility runs the other way: a small distance is easy to satisfy, a large one is hard. The
update becomes `if (feasible(mid)) lo = mid; else hi = mid - 1;` — and with `lo = mid` you must
use `mid = lo + (hi - lo + 1) / 2` (round **up**), or `lo` sticks and the loop hangs. That
rounding detail is the single most common bug in the maximisation form.

```cpp
bool canPlace(vector<int>& p, int cows, int dist) {
    int cnt = 1, last = p[0];
    for (int i = 1; i < (int)p.size(); ++i)
        if (p[i] - last >= dist) { ++cnt; last = p[i]; }
    return cnt >= cows;
}
int aggressiveCows(vector<int> p, int cows) {
    sort(p.begin(), p.end());                  // stalls are NOT given sorted
    int lo = 1, hi = p.back() - p[0];
    while (lo < hi) {
        int mid = lo + (hi - lo + 1) / 2;      // round UP: with lo = mid, rounding down hangs
        if (canPlace(p, cows, mid)) lo = mid;  // feasible: try BIGGER
        else                        hi = mid - 1;
    }
    return lo;
}
```

**Complexity.** O(n log(sum)) time, O(1) space.

**Verified:** split array `{7,2,5,10,8}` k=2 → 18; `{1,2,3,4,5}` k=2 → 9; `{1,4,4}` k=3 → 4.
Aggressive cows `{0,3,4,7,10,9}` 4 cows → 3; `{1,2,4,8,9}` 3 cows → 3; `{1,2,3}` 2 cows → 2;
`{1,2,8,4,9}` (unsorted input) 3 cows → 3.

---

## 21. Nth Root of a Number

```cpp
int nthRoot(int n, int m) {
    int lo = 1, hi = m, ans = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        long long p = 1; bool over = false;
        for (int i = 0; i < n && !over; ++i) { p *= mid; if (p > m) over = true; }  // early exit
        if (!over && p == m) return mid;
        if (over || p > m) hi = mid - 1; else lo = mid + 1;
    }
    return ans;
}
```

**Key insight.** Computing `mid^n` overflows fast — `2^31` already exceeds `int`. Accumulate in
`long long` **and bail out the moment the running product passes `m`**; without the early exit,
`mid^n` for large `mid` and `n` overflows even a `long long`.

**Complexity.** O(n log m) time, O(1) space.

**Verified:** `nthRoot(3,27)=3`, `nthRoot(4,69)=-1` (no integer root), `nthRoot(2,64)=8`,
`nthRoot(3,1000000000)=1000` (no overflow).

---

## 22. Search a 2D Matrix — LeetCode 74

```cpp
bool searchMatrix(vector<vector<int> >& mat, int t) {
    int R = (int)mat.size(), C = (int)mat[0].size();
    int lo = 0, hi = R * C - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int v = mat[mid / C][mid % C];              // unflatten
        if (v == t) return true;
        if (v < t) lo = mid + 1; else hi = mid - 1;
    }
    return false;
}
```

**Key insight.** Because each row starts after the previous row ends, the matrix reads as one
sorted array of length `R*C`. `mid / C` and `mid % C` convert a flat index back to (row, column)
— the same identity as `../08_2d array`. **Divide by the column count, not the row count.**

**Complexity.** O(log(R·C)) time, O(1) space.

**Verified:** the standard 3×4 matrix finds 3, 1 (first) and 60 (last), rejects 13; 1×1 hit and
miss.

---

## 23. Smallest Missing Non-negative Number

```cpp
int smallestMissing(vector<int>& a) {          // sorted, distinct, non-negative
    int lo = 0, hi = (int)a.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == mid) lo = mid + 1;       // prefix is intact: answer is right
        else               hi = mid;
    }
    return lo;
}
```

**Key insight.** In a sorted array of distinct non-negatives, `a[i] == i` exactly while nothing
is missing. The first index where `a[i] != i` is the answer, and that predicate is monotonic.
`hi = n` so that "nothing is missing" correctly returns `n`.

**Complexity.** O(log n) time, O(1) space.

**Verified:** `{0,1,2,3,5,6}→4`, `{0,1,2,3}→4` (nothing missing), `{1,2,3}→0` (0 is missing),
`{}→0`.

---

## 24. Kth Missing Positive Number — LeetCode 1539

**Key insight.** At index `i`, the count of missing positives before `a[i]` is `a[i] - (i+1)`.
That is monotonic, so binary search for the first index where it is `>= k`; the answer is
`lo + k`. O(log n) instead of the obvious O(n) scan.

---

# Section 3 — Good to Know

## 25. Median of Two Sorted Arrays — LeetCode 4

```cpp
double findMedianSortedArrays(vector<int>& a, vector<int>& b) {
    if (a.size() > b.size()) return findMedianSortedArrays(b, a);   // search the SHORTER
    int n1 = (int)a.size(), n2 = (int)b.size();
    int lo = 0, hi = n1, half = (n1 + n2 + 1) / 2;
    while (lo <= hi) {
        int cut1 = lo + (hi - lo) / 2;
        int cut2 = half - cut1;                        // the cuts are linked
        int l1 = (cut1 == 0)  ? INT_MIN : a[cut1 - 1];
        int l2 = (cut2 == 0)  ? INT_MIN : b[cut2 - 1];
        int r1 = (cut1 == n1) ? INT_MAX : a[cut1];
        int r2 = (cut2 == n2) ? INT_MAX : b[cut2];
        if (l1 <= r2 && l2 <= r1) {                    // valid partition found
            if ((n1 + n2) % 2 == 1) return max(l1, l2);
            return (max(l1, l2) + min(r1, r2)) / 2.0;
        }
        if (l1 > r2) hi = cut1 - 1; else lo = cut1 + 1;
    }
    return 0.0;
}
```

**Key insight.** You are not searching for a value — you are searching for a **cut position**.
Choose how many elements of `a` go left; `cut2` is then forced, because the two halves must
total `half`. The partition is correct when every element left of both cuts is ≤ every element
right of both, which is just `l1 <= r2 && l2 <= r1`.

**The sentinels are the fiddly part.** When a cut is at 0 there is nothing on its left, so the
"largest on the left" is `INT_MIN`; at the end, the "smallest on the right" is `INT_MAX`. Those
four ternaries let one comparison handle every edge case, including an empty array.

Searching the **shorter** array keeps `cut2` in range and makes it O(log(min(n1,n2))).

**Complexity.** O(log(min(n1,n2))) time, O(1) space.

**Verified:** `{1,3}` & `{2}` → 2.0 (odd total); `{1,2}` & `{3,4}` → 2.5 (even); `{}` & `{1}` →
1.0 (one array empty); `{1,2,3,4,5}` & `{6,7,8}` → 4.5.

---

## 26. Kth Element of Two Sorted Arrays

Same machinery as #25 with `half` replaced by `k`, and the answer is `max(l1, l2)`. Do it after
#25 and it is a five-minute problem.

---

## 27–28. Minimum Days for Bouquets (1482) / Smallest Divisor (1283)

Both are #11 with a different `feasible()`.

- **1482** — `feasible(day)`: scan the garden counting runs of consecutive flowers already
  bloomed by `day`; each run of length `L` yields `L / k` bouquets. Feasible if the total ≥ m.
  Bounds `[min(bloomDay), max(bloomDay)]`. Return −1 if `m * k > n`, checked **before** searching
  — the multiplication needs `long long`.
- **1283** — `feasible(d)`: `sum of ceil(a[i]/d) <= threshold`. Bounds `[1, max(a)]`. Literally
  Koko with the words changed.

---

## 29. Search a 2D Matrix II — LeetCode 240 — **the counterexample**

**Key insight — do not binary search this.** Rows and columns are individually sorted but the
matrix is *not* globally sorted, so #22's flattening is invalid. Start at the **top-right**
corner: if the value is too big, the whole column is too big → move left; too small, the whole
row is too small → move down.

O(R+C), which beats row-by-row binary search at O(R log C). Recognising that a problem which
*looks* like binary search is better served by something else is worth as much as the technique.
Full solution in `../08_2d array/solution.md` #6.

---

## 30. Row with Maximum Number of 1s

**Key insight.** Each row is sorted, so the count of 1s in a row is `C - lowerBound(row, 1)` —
O(log C) per row, O(R log C) total. Better still, the staircase from #29 gives O(R+C): start
top-right, move left while you see 1s (recording the row), move down when you see a 0.

---

# Bugs in your existing code

**Everything below was compiled and run. Nothing in `11_linearAndBinarySearch/` was edited.**

## The one bug that appears in five functions: `mid = lo/2 + hi/2`

`binary search.cpp` lines 10, 25, 49, 66, 83 — plus `peakmountainindex.cpp:8` and six places in
`rotatedsortedarray.cpp`.

Integer division truncates, and doing it **twice** loses up to 1. Measured across small ranges:

| `lo` | `hi` | `lo/2 + hi/2` | correct `lo + (hi-lo)/2` | |
|---|---|---|---|---|
| 1 | 1 | **0** | 1 | **below `lo`** |
| 1 | 3 | 1 | 2 | |
| 3 | 3 | **2** | 3 | **below `lo`** |
| 3 | 5 | 3 | 4 | |
| 5 | 5 | **4** | 5 | **below `lo`** |

When `lo == hi` and both are odd, `mid` lands **outside the range**. `lo = mid + 1` then fails to
advance (infinite loop) or `hi = mid - 1` skips backwards past the answer.

### `binary search.cpp:5` `binarysearch` — misses half the array

Also uses `while (lo < hi)` where it needs `lo <= hi`, so a one-element range is never examined.

**Verified on the sorted array `{1,2,3,4,5,6,7}`: searching for 1, 3, 5 and 7 all return −1.**
Four of eight targets are unfindable — and they are exactly the ones at even indices.

```cpp
int mid = lo + (hi - lo) / 2;    // fix 1
while (lo <= hi) { ... }         // fix 2
```

### `binary search.cpp:21` `lowerBound` — wrong on every input tested

Three separate problems: the `mid` bug, `hi = n-1` (so "past the end" is unreachable), and
`if (nums[mid] == x) return mid - 1;` which returns the index *before* the match.

**Verified on `{1,3,5,7,9}`: `lowerBound(5)→1` (want 2), `lowerBound(4)→1` (want 2),
`lowerBound(1)→-1` (want 0), `lowerBound(10)→4` (want 5). Zero of four correct.**

Correct version in #2. It is worth rewriting this one from scratch rather than patching.

### `binary search.cpp:45` `firstOccurence` — **hangs**

```cpp
if(nums[mid]==x){
    if(nums[mid-1]==x)hi=mid-1;     // reads nums[-1] when mid == 0
    else return mid ;
}
```

Two faults. `nums[mid-1]` is an out-of-bounds read when `mid == 0`. And combined with the `mid`
bug the range stops shrinking — **verified: `firstOccurence({5,7,7,8,8,10}, 7)` did not
terminate and was killed after 10 seconds.**

Correct version in #4, built from `lowerBound`.

### `binary search.cpp:61` `smallestMissing` — wrong when 0 is the answer

The `a[mid] == mid` predicate is exactly right — good instinct. But `while (lo < hi)` with
`hi = mid - 1` overshoots, and `ans` stays at its initial −1 in some paths.

**Verified: `{0,1,2,3,5,6}→4` (correct), `{0,1,2,3}→-1` (arguably fine), but `{1,2,3}→1`
when the answer is 0.** Corrected in #23.

### `binary search.cpp:79` `sqrt` — compares against `hi`, and hangs

```cpp
if(mid*mid==hi)return mid;          // should be x
else if(mid*mid<hi)lo=mid+1;        // should be x
```

`hi` **changes every iteration**, so the target of the comparison keeps moving. The function is
not searching for anything fixed.

**Verified: `sqrt(4)→2` (correct by luck), `sqrt(8)→1` (want 2), `sqrt(9)→1` (want 3),
`sqrt(16)→1` (want 4), and `sqrt(1)` never terminated** — killed at 10 seconds.

`mid*mid` also overflows for large `x`. Corrected version in #6.

---

## `peakmountainindex.cpp` — out-of-bounds read, fails on length 3

```cpp
int lo=1,hi=n-2,mid;
mid=lo/2 +hi/2;
if(nums[mid]>nums[mid+1] && nums[mid]>nums[mid-1] ){return mid;}
```

For `n == 3`: `lo=1, hi=1`, and `mid = 1/2 + 1/2 = 0`. Then `nums[mid-1]` is `nums[-1]` — an
**out-of-bounds read**. **Verified: `peak({0,1,0})` returns −1; the answer is 1.**

Longer mountains happen to work (`{0,2,1,0}`, `{3,4,5,1}` and a length-10 case all returned the
right index), which is why the bug survived — the smallest legal input is the one that breaks it.

The fix is structural, not a patch: with `lo=0, hi=n-1` and `while (lo < hi)`, `mid` can never
equal `hi`, so `a[mid+1]` is always safe and `a[mid-1]` is never needed. See #7.

---

## `rotatedsortedarray.cpp` — uninitialised variable read

```cpp
int pivot;              // :16  never initialised
...
if (pivot==-1){         // :31  reads an uninitialised variable
```

If the pivot-finding loop exits without hitting a `break` (which happens whenever the array is
**not rotated**), `pivot` still holds garbage, and `pivot == -1` is a coin flip. Every path after
that — `nums[pivot-1]`, `lo = pivot` — indexes with a garbage value.

The loop also starts at `lo=1, hi=n-2`, excluding indices 0 and `n-1` from consideration, and
uses `lo/2 + hi/2` in all six of its binary searches.

**The whole 60-line structure is unnecessary.** #8 solves this in 15 lines with no pivot at all,
because at any `mid` one half is guaranteed sorted. That is the insight worth taking away.

---

## `kokoEatingBanana.cpp` — `check()` never uses `speed`

```cpp
bool check (int speed ,vector<int>piles,int hours){
    int n =piles.size();
    int s =speed;                      // assigned, never read
    for (int i=0;i<n;i++){
        if (hours<0)break;             // hours never changes -> loop does nothing
    }
    if(hours<0)return false;
    return true;                       // ALWAYS returns true for hours >= 0
}
```

The function never computes how long eating at `speed` would take. **Verified:
`check(speed=1, hours=8)` returns `true` — at speed 1, `{3,6,7,11}` needs 27 hours, so it must
be `false`.** Since `check` is always true, the binary search always narrows downward and
returns its initial `lo`.

**Verified end to end: `koko({3,6,7,11}, 8) → 3` (want 4); `koko({30,11,23,4,20}, 5) → 17`
(want 30); `koko({30,11,23,4,20}, 6) → 14` (want 23).**

Two further issues: `int ans;` is uninitialised and would be returned as garbage if the loop
never assigned it, and `lo = sum/hours` can be 0, which as a speed means no progress at all.

Corrected in #11. **The lesson: unit-test `feasible()` by itself before wiring it into a binary
search.** A predicate that ignores its argument produces a confident, wrong number.

---

## `timeToCompleteTrips.cpp` — the division is inverted

```cpp
trips-=(time[i]/hours);       // should be hours / time[i]
```

`time[i]` is seconds per trip; `hours` is the candidate total time. Trips completed is
`hours / time[i]`. As written, any candidate time larger than the trip length contributes 0.

**Verified: `check({1,2,3}, trips=5, hours=3)` returns `false`. In 3 units, the buses complete
3 + 1 + 1 = 5 trips, so it must be `true`.**

`long long ans;` is also uninitialised. Corrected in #13.

---

## `ship(1011).cpp` — no return statement

GCC says so directly:

```
ship(1011).cpp:47:1: warning: no return statement in function returning non-void
ship(1011).cpp:18:37: warning: variable 'minCap' set but not used
```

Falling off the end of a non-`void` function is **undefined behaviour** — the caller reads
whatever happens to be in the return register.

Two more issues in the counting loop:

1. **`cap` is never reset between binary-search iterations.** It is declared at `:18` outside the
   `while` and only zeroed on a split, so each candidate starts with leftover weight from the
   previous one. `days = 0` is reset correctly at `:23`; `cap` needs the same treatment.
2. `if (days == Days)` records `minCap`, but `days < Days` is **also feasible** and does not
   record it — so a capacity that ships in fewer days than allowed is discarded.

Corrected in #12.

---

## What you got right

- **`sumOfSquares.cpp` is correct** — verified on c = 0, 1, 2, 3, 4, 5, all six matching. The
  approach is unusual: rather than stepping `y` down one at a time you jump it to the nearest
  perfect square below. It terminates and it works. The two-pointer form in #17 is easier to
  justify in an interview, but there is nothing wrong with yours.

- **`kclosest.cpp` is completely correct** — the only fully working non-trivial solution in the
  folder. Verified on seven cases: `k=4,x=3 → {1,2,3,4}`, `x=-1` (target below the whole array)
  → `{1,2,3,4}`, `x=9` (above it) → `{2,3,4,5}`, `k` equal to the array length, `k=1`, a
  duplicate-heavy array, and a symmetric tie. The two-pointer expansion at lines 29–39 handles
  both pointers running off their ends, and the `hi >= n` guard is correctly placed **before**
  the dereference of `nums[hi]` — `||` short-circuits, so that ordering is doing real work. The
  `else { hi = lo; lo = lo - 1; }` branch for a target that isn't present is also right: after a
  failed search `lo` is the insertion point, so it is the first element ≥ x. That is a
  non-obvious fact to have used deliberately.

- **The overflow-safe `mid` is in four files.** `kclosest.cpp:11`, `kokoEatingBanana.cpp:28`,
  `ship(1011).cpp:22` and `timeToCompleteTrips.cpp:24` all use `lo + (hi - lo) / 2`. So the habit
  is there — it is specifically `binary search.cpp`, `peakmountainindex.cpp` and
  `rotatedsortedarray.cpp` (all dated a day earlier) that carry the old `lo/2 + hi/2`. Those
  three are the ones to go back and fix.

- **The `nums[mid] == mid` predicate in `smallestMissing`** is exactly the right idea, and it is
  not an obvious one. Only the loop mechanics around it are wrong.

- **`timeToCompleteTrips.cpp` widens to `long long` for the bounds** (`max*trips` at `:21`) and
  casts inside the max loop. That overflow awareness is the thing most people miss on 2187 — the
  arithmetic direction is a one-character fix, the type discipline is the harder habit and you
  already have it.

- **`ship(1011).cpp` sets `lo = max(weights)`, not 1.** That is the subtle bound most people get
  wrong on this problem, and you had it right.

- **`kokoEatingBanana.cpp` has the binary-search skeleton right** — record the answer on success,
  move `hi` down, else move `lo` up. Only `check()` is missing its body.

- **The `if (o > c) return;`-style early structure in `kclosest.cpp`'s merge loop** (lines 29–39)
  correctly handles both pointers running off their ends, including the `hi >= n` guard before
  dereferencing. That is careful boundary work.
