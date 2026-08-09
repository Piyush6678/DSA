# 02 — Conditionals: Solutions

Full C++14 for Sections 1–3, approach-only for Section 4.
Compile with `g++ file.cpp -o file.exe`.

---

# Section 1 — Must Do

## 1. Even or Odd — correct for negatives

```cpp
#include <iostream>
using namespace std;

bool isEven(int n) {
    return n % 2 == 0;
}

int main() {
    int n;
    cout << "Enter a number: ";
    cin >> n;
    cout << (isEven(n) ? "Even" : "Odd") << "\n";
    return 0;
}
```

**The whole question is which test you write:**

| Test | `n = 7` | `n = -7` | Verdict |
|---|---|---|---|
| `n % 2 == 1` | true ✓ | **false ✗** | **wrong** |
| `n % 2 != 0` | true ✓ | true ✓ | correct |
| `n % 2 == 0` (even) | false ✓ | false ✓ | correct |

`-7 % 2` is `-1`, not `1`, because C++ `%` takes the sign of the dividend. So
`n % 2 == 1` silently classifies every negative odd number as even.

**Rule: test evenness (`% 2 == 0`) or inequality (`% 2 != 0`). Never test `% 2 == 1`.**

**The bitwise alternative:** `n & 1` is `1` for odd. This works correctly for negatives on
every real machine (two's complement), and is what you'll see in `../15_bitwise`. But
`n % 2 == 0` reads better and compilers generate identical code, so prefer it.

**Complexity:** O(1) time, O(1) space.

---

## 2. Largest of three numbers

### Approach 1 — nested (2 comparisons on every path)

```cpp
int maxOfThreeNested(int a, int b, int c) {
    if (a >= b) return (c > a) ? c : a;
    else        return (c > b) ? c : b;
}
```

### Approach 2 — flat ladder (more readable, up to 4 comparisons)

```cpp
int maxOfThreeFlat(int a, int b, int c) {
    if (a >= b && a >= c) return a;
    else if (b >= a && b >= c) return b;
    else return c;
}
```

### Approach 3 — what you'd actually ship

```cpp
#include <algorithm>
int maxOfThree(int a, int b, int c) {
    return max(a, max(b, c));
}
```

**The graded part is the `>=`.** With strict `>` in Approach 2, the input `(5, 5, 5)`
fails every branch except the final `else` — which happens to be correct here, but the same
mistake in a "find the index of the maximum" problem returns the wrong index. **Always ask
what your comparison chain does when values tie.**

Approach 1 is `lec3.cpp:56-66`, and it's correct. Worth knowing that it makes exactly 2
comparisons on every path, which is the information-theoretic minimum for 3 elements —
Approach 2 can make 4. That difference is irrelevant here, but the reasoning ("how many
comparisons does my branch structure actually perform?") is exactly the reasoning behind
tournament-style min-and-max algorithms.

**Complexity:** O(1) time, O(1) space.

---

## 3. Nim Game — LeetCode 292

> A heap of `n` stones. You and your opponent alternate, you go first, each turn removes
> 1, 2, or 3 stones. Whoever takes the last stone wins. Both play optimally. Can you win?

### Approach 1 — Brute force: recursion / DP

```cpp
// win[i] = can the player to move win with i stones remaining?
bool canWinNimDP(int n) {
    vector<bool> win(n + 1, false);
    for (int i = 1; i <= n; ++i)
        win[i] = (i >= 1 && !win[i-1]) || (i >= 2 && !win[i-2]) || (i >= 3 && !win[i-3]);
    return win[n];
}
```

**Complexity:** O(n) time, O(n) space. With `n` up to 2³¹−1 this is hopeless — which is
the hint that a pattern exists.

### Approach 2 — Optimal: find the invariant, O(1)

```cpp
bool canWinNim(int n) {
    return n % 4 != 0;
}
```

**Why it works.** Print the first few values of the DP:

| n | 1 | 2 | 3 | **4** | 5 | 6 | 7 | **8** | 9 |
|---|---|---|---|---|---|---|---|---|---|
| can win? | W | W | W | **L** | W | W | W | **L** | W |

You lose exactly at multiples of 4. The proof is a strategy argument:

- With `n = 4` you must leave 1, 2, or 3 stones — all winning positions for your opponent.
  So 4 is a **losing** position.
- With `n` **not** a multiple of 4, you can always remove `n % 4` stones (which is 1, 2,
  or 3 — a legal move) and hand your opponent a multiple of 4.
- Your opponent, facing a multiple of 4, must break it, and you restore the multiple of 4
  on your next turn. Since the number strictly decreases, they eventually face exactly 4
  and lose.

So: **you win iff `n % 4 != 0`.**

**Complexity:** O(1) time, O(1) space.

**The transferable lesson.** When a game problem has huge constraints, compute small cases
by brute force, tabulate, and look for the period. The losing positions in impartial games
are almost always an arithmetic pattern. You will do exactly this again in `../26_dp`.

---

## 4. Leap year

```cpp
#include <iostream>
using namespace std;

bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int main() {
    int year;
    cout << "Enter year: ";
    cin >> year;
    cout << year << (isLeapYear(year) ? " is" : " is not") << " a leap year\n";
    return 0;
}
```

**The rule, in the order it must be applied:**

1. Divisible by 400 → leap (e.g. 2000, 1600)
2. Otherwise divisible by 100 → **not** leap (e.g. 1900, 2100)
3. Otherwise divisible by 4 → leap (e.g. 2024, 1996)
4. Otherwise → not leap

**Test cases that separate correct from nearly-correct:**

| Year | Answer | Kills the naive rule |
|---|---|---|
| 2024 | leap | — |
| 2023 | not | — |
| **1900** | **not** | `year % 4 == 0` alone says leap ✗ |
| **2000** | **leap** | `%4==0 && %100!=0` alone says not leap ✗ |

`1900` and `2000` are the entire test suite. If a candidate writes `year % 4 == 0` and
stops, ask about 1900. If they add `&& year % 100 != 0`, ask about 2000.

**Why the `|| ` ordering above works.** Putting `% 400` first means the `% 100` exclusion
never gets a chance to wrongly reject a year like 2000. The equivalent nested form makes
the precedence explicit:

```cpp
if (year % 4 != 0)        return false;   // not divisible by 4 -> common year
else if (year % 100 != 0) return true;    // div by 4, not by 100 -> leap
else if (year % 400 != 0) return false;   // div by 100, not by 400 -> common (1900)
else                      return true;    // div by 400 -> leap (2000)
```

**Complexity:** O(1) time, O(1) space.

---

## 5. Absolute value and sign, without `abs()`

```cpp
#include <iostream>
#include <climits>
using namespace std;

int absolute(int n) {
    return (n < 0) ? -n : n;
}

// Returns -1, 0, or +1. Branchless.
int sign(int n) {
    return (n > 0) - (n < 0);
}

int main() {
    int n;
    cin >> n;
    cout << "abs  = " << absolute(n) << "\n";
    cout << "sign = " << sign(n) << "\n";
    return 0;
}
```

**How `sign` works.** `(n > 0)` and `(n < 0)` are each a `bool` that converts to `1` or
`0`. For `n = 5` that's `1 - 0 = 1`; for `n = -5`, `0 - 1 = -1`; for `n = 0`, `0 - 0 = 0`.
No branch, no `if`. This idiom appears constantly in comparator functions.

### The trap that makes this a real interview question

```cpp
absolute(INT_MIN)   // undefined behaviour
```

`INT_MIN` is `-2147483648`, but `INT_MAX` is `+2147483647`. **There is no positive `int`
equal to `|INT_MIN|`** — the two's complement range is asymmetric. So `-n` overflows, which
is undefined behaviour for signed types. (In practice `-INT_MIN` wraps back to `INT_MIN`,
so `abs()` returns a *negative* number.)

`std::abs(INT_MIN)` has exactly the same problem — this is not a flaw in your hand-rolled
version. The fixes:

```cpp
long long absoluteSafe(int n) { return (n < 0) ? -(long long)n : n; }   // widen first
```

Mentioning this unprompted is a strong signal. It's the same family as the `(low+high)/2`
overflow from `../01_basics` §9: **the boundary value of a fixed-width type is where the
bug lives.**

### On `lec3.cpp:20`

```cpp
if (n < 0) { n = -n; }
cout << endl << "absolute value of n is n";
```

Two problems. The literal `"n"` inside the quotes prints the letter `n` instead of the
value — you need `<< n` outside the string. And mutating `n` in place means every later
check in that `main()` (the three-digit test, the divisible-by-5-or-3 test) silently
operates on the absolute value rather than the input. Prefer a new variable:
`int absN = (n < 0) ? -n : n;`

**Complexity:** O(1) time, O(1) space.

---

# Section 2 — Important

## 6. Pass the Pillow — LeetCode 2582

> `n` people stand in a line numbered `1..n`. The pillow starts at person 1 and is passed
> to the next person every second. On reaching either end the direction reverses. Who holds
> the pillow after `k` seconds?

### Approach 1 — Brute force: simulate

```cpp
int passThePillowBrute(int n, int k) {
    int pos = 1, dir = 1;
    for (int t = 0; t < k; ++t) {
        if (pos == n) dir = -1;
        else if (pos == 1) dir = 1;
        pos += dir;
    }
    return pos;
}
```

**Complexity:** O(k) time, O(1) space. Fine for the given constraints, but it teaches you
nothing and doesn't generalise to large `k`.

### Approach 2 — Optimal: modular arithmetic, O(1)

```cpp
int passThePillow(int n, int k) {
    int cycle = 2 * (n - 1);        // one full round trip: forward n-1, back n-1
    int r = k % cycle;              // seconds elapsed within the current round trip
    return (r < n) ? (1 + r) : (2 * n - 1 - r);
}
```

**Why it works.**

- Travelling from person 1 to person `n` takes `n - 1` seconds.
- Coming back takes another `n - 1`. So the motion is **periodic with period `2(n-1)`**,
  and only `r = k % cycle` matters.
- **Forward leg** (`r ≤ n-1`): position is `1 + r`.
- **Backward leg** (`r > n-1`): you reached person `n` at time `n-1` and have been walking
  back for `r - (n-1)` seconds, so position is `n - (r - (n-1))` = `2n - 1 - r`.

`r < n` and `r <= n-1` are the same condition for integers, which is why the ternary reads
`r < n`.

**Dry run** — `n = 4, k = 5`: `cycle = 6`, `r = 5`. `5 < 4` is false, so
`2(4) - 1 - 5 = 8 - 6 = 2`. Trace it: `1→2→3→4→3→2`, five passes, ends at **2** ✓

**Dry run** — `n = 3, k = 2`: `cycle = 4`, `r = 2`. `2 < 3` → `1 + 2 = 3`. Trace: `1→2→3`,
ends at **3** ✓

**Complexity:** O(1) time, O(1) space.

**The transferable idea:** *any* back-and-forth traversal is periodic with period
`2 × (length − 1)`, and the two legs are mirror images. The same decomposition solves
zigzag string conversion (LeetCode 6) and bouncing-ball simulations.

---

## 7. Simple calculator using `switch`

```cpp
#include <iostream>
using namespace std;

int main() {
    double a, b;
    char op;

    cout << "Enter first number: ";  cin >> a;
    cout << "Enter operator (+ - * / %): "; cin >> op;
    cout << "Enter second number: "; cin >> b;

    switch (op) {
        case '+': cout << "Result: " << a + b << "\n"; break;
        case '-': cout << "Result: " << a - b << "\n"; break;
        case '*': cout << "Result: " << a * b << "\n"; break;
        case '/':
            if (b == 0) cout << "Error: division by zero\n";
            else        cout << "Result: " << a / b << "\n";
            break;
        case '%':
            // % is integer-only; a and b are double, so cast explicitly
            if ((int)b == 0) cout << "Error: modulo by zero\n";
            else             cout << "Result: " << (int)a % (int)b << "\n";
            break;
        default:
            cout << "Invalid operator '" << op << "'\n";
    }
    return 0;
}
```

**Your `lec4.cpp` version is correct** — it has a `default`, it guards division by zero,
and every case breaks. That's the whole graded content of this question. Three refinements:

1. **`switch` cannot take a `double`.** That's why we switch on `op` (a `char`, integral)
   and not on a value. If you tried `switch (a)` with `a` as `double`, it wouldn't compile.
2. **`%` needs integer operands.** `a % b` on two `double`s is a compile error; you must
   cast, or use `fmod(a, b)` from `<cmath>`.
3. **`b == 0` on a `double` is technically the float-equality problem** from
   `../01_basics` §10. Here it's acceptable — you genuinely want to reject exactly zero.
   But `b = 1e-300` will pass the guard and produce `inf`, so a robust version tests
   `fabs(b) < 1e-12`.

**Why `switch` and not `if-else if` here.** Five distinct constant `char` values is the
textbook case: dense integral labels, no ranges. The compiler emits a jump table.

**Complexity:** O(1) time, O(1) space.

---

## 8. Vowel or consonant

```cpp
#include <iostream>
using namespace std;

bool isVowel(char c) {
    // normalise case first so we only list five letters
    if (c >= 'A' && c <= 'Z') c = c + 32;
    return c=='a' || c=='e' || c=='i' || c=='o' || c=='u';
}

int main() {
    char c;
    cout << "Enter a character: ";
    cin >> c;

    bool isAlpha = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    if (!isAlpha)        cout << "Not an alphabet\n";
    else if (isVowel(c)) cout << "Vowel\n";
    else                 cout << "Consonant\n";
    return 0;
}
```

**The branch people forget is `!isAlpha`.** `'7'` is neither a vowel nor a consonant, but
a two-branch solution reports "consonant". Interviewers seed exactly this input.

**The `switch` alternative — a legitimate fall-through:**

```cpp
switch (tolower(c)) {
    case 'a': case 'e': case 'i': case 'o': case 'u':
        cout << "Vowel"; break;
    default:
        cout << "Consonant";
}
```

Stacked labels with no body between them is the *intended* use of fall-through. Compare
with the accidental kind — a case with a body and no `break`.

**Ordering note.** Test `!isAlpha` **first**. Writing `isVowel(c)` first would call it on a
digit, which is harmless here but is the same shape as the out-of-bounds bug that
short-circuiting exists to prevent.

**Complexity:** O(1) time, O(1) space.

---

## 9. Profit, loss, or break-even

```cpp
#include <iostream>
using namespace std;

int main() {
    double cp, sp;
    cout << "Enter cost price: ";    cin >> cp;
    cout << "Enter selling price: "; cin >> sp;

    if (sp > cp) {
        double profit = sp - cp;
        cout << "Profit: " << profit
             << "  (" << (profit / cp) * 100.0 << "%)\n";
    } else if (sp < cp) {
        double loss = cp - sp;
        cout << "Loss: " << loss
             << "  (" << (loss / cp) * 100.0 << "%)\n";
    } else {
        cout << "No profit, no loss\n";
    }
    return 0;
}
```

**Three branches, not two.** The equality case is a distinct real-world outcome, and a
two-branch version reports "loss of 0", which is wrong in kind rather than in number.
`lec3.cpp:33-42` gets this right.

**The percentage is computed against cost price**, always — that's the business
definition, and `/ cp` (a `double`) avoids integer division. If `cp` and `sp` were `int`,
`(profit / cp) * 100` would truncate to `0` for any profit smaller than the cost.

**Edge case:** `cp == 0` makes the percentage `inf` (or NaN if profit is also 0). Guard it
if the problem allows free goods.

**Complexity:** O(1) time, O(1) space.

---

## 10. Grade from marks

```cpp
#include <iostream>
using namespace std;

char gradeOf(int marks) {
    if (marks < 0 || marks > 100) return '?';   // validate BEFORE classifying
    if (marks >= 90) return 'A';
    if (marks >= 80) return 'B';
    if (marks >= 70) return 'C';
    if (marks >= 60) return 'D';
    if (marks >= 40) return 'E';
    return 'F';
}

int main() {
    int marks;
    cout << "Enter marks (0-100): ";
    cin >> marks;
    char g = gradeOf(marks);
    if (g == '?') cout << "Invalid marks\n";
    else          cout << "Grade: " << g << "\n";
    return 0;
}
```

**Two things are being graded here.**

**1. Ladder order.** Descending thresholds mean each branch needs only a lower bound — the
upper bound is implied by having failed the previous test. Reverse the order and it breaks
completely:

```cpp
if (marks >= 40) return 'E';    // catches 95 -> returns 'E'
if (marks >= 60) return 'D';    // UNREACHABLE
```

**2. Validation first.** `marks = 150` must not silently become an `A`. Range-check before
you classify.

**Note the early `return`s** rather than `else if`. In a function that returns from every
branch, `if` + `return` is equivalent to `else if` and reads flatter. Both are correct;
this style scales better as branches accumulate.

**Complexity:** O(1) time — a bounded number of comparisons. O(1) space.

---

# Section 3 — Good to Know

## 11. Triangle validity and type

```cpp
#include <iostream>
using namespace std;

bool isValidTriangle(int a, int b, int c) {
    if (a <= 0 || b <= 0 || c <= 0) return false;   // sides must be positive
    return (a + b > c) && (b + c > a) && (a + c > b);
}

int main() {
    int a, b, c;
    cout << "Enter three sides: ";
    cin >> a >> b >> c;

    if (!isValidTriangle(a, b, c)) {
        cout << "Not a triangle\n";
    } else if (a == b && b == c) {
        cout << "Equilateral\n";
    } else if (a == b || b == c || a == c) {
        cout << "Isosceles\n";
    } else {
        cout << "Scalene\n";
    }
    return 0;
}
```

**The triangle inequality:** the sum of any two sides must **strictly exceed** the third.
Three checks are needed in general — but if you know `a ≤ b ≤ c`, only `a + b > c` can
fail, so **one** check suffices. That reduction is exactly why LeetCode 976 sorts first.

**`>` must be strict.** With `a + b >= c`, the sides `(1, 2, 3)` pass — but they form a
degenerate, zero-area "triangle" that is really a straight line. Interviewers test `(1,2,3)`.

**Order matters again:** equilateral before isosceles, because an equilateral triangle
*also* satisfies `a == b`. Reversing the two branches makes the equilateral case
unreachable — the same failure mode as #10.

**Overflow note:** with `a`, `b`, `c` near `INT_MAX`, `a + b` overflows. The safe rewrite
is `a > c - b`, which keeps both sides in range.

**Complexity:** O(1) time, O(1) space.

---

## 12. Nature of the roots of a quadratic

```cpp
#include <iostream>
#include <cmath>
using namespace std;

int main() {
    double a, b, c;
    cout << "Enter a, b, c for ax^2 + bx + c = 0: ";
    cin >> a >> b >> c;

    if (a == 0) {                        // NOT a quadratic
        if (b == 0) cout << (c == 0 ? "Infinite solutions\n" : "No solution\n");
        else        cout << "Linear equation, root = " << -c / b << "\n";
        return 0;
    }

    double d = b * b - 4 * a * c;        // discriminant

    if (d > 0) {
        cout << "Two distinct real roots: "
             << (-b + sqrt(d)) / (2 * a) << " and "
             << (-b - sqrt(d)) / (2 * a) << "\n";
    } else if (d == 0) {
        cout << "One repeated real root: " << -b / (2 * a) << "\n";
    } else {
        double real = -b / (2 * a);
        double imag = sqrt(-d) / (2 * a);
        cout << "Complex roots: " << real << " + " << imag << "i and "
                                  << real << " - " << imag << "i\n";
    }
    return 0;
}
```

**The branch everyone forgets is `a == 0`.** With `a = 0` the equation is linear, and
dividing by `2 * a` is division by zero. This is the actual point of the question — the
discriminant part is mechanical.

**Use `double`, not `int`.** With `int` inputs, `b * b` overflows once `|b|` exceeds about
46,340. And the roots are rarely integers anyway.

**`d == 0` on a `double` is the float-equality problem.** With computed inputs, a
mathematically-zero discriminant may land at `1e-17`. A robust version tests
`fabs(d) < 1e-9`. Say this out loud in an interview even if you leave the code as-is.

**Complexity:** O(1) time, O(1) space.

---

## 13. Day of the week using `switch`

```cpp
#include <iostream>
using namespace std;

int main() {
    int day;
    cout << "Enter day number (1-7): ";
    cin >> day;

    switch (day) {
        case 1: cout << "Monday\n";    break;
        case 2: cout << "Tuesday\n";   break;
        case 3: cout << "Wednesday\n"; break;
        case 4: cout << "Thursday\n";  break;
        case 5: cout << "Friday\n";    break;
        case 6: cout << "Saturday\n";  break;
        case 7: cout << "Sunday\n";    break;
        default: cout << "Invalid — enter 1 to 7\n";
    }

    // Grouped cases: weekday vs weekend, using deliberate fall-through
    switch (day) {
        case 6: case 7:
            cout << "Weekend\n"; break;
        case 1: case 2: case 3: case 4: case 5:
            cout << "Weekday\n"; break;
        default:
            cout << "Invalid\n";
    }
    return 0;
}
```

`lec4.cpp:5-33` is correct as written — every case breaks and there's a `default` with a
helpful message. The second switch above is the addition worth studying: **stacked `case`
labels with no code between them** are how you say "these values share a body". That is
intentional fall-through, and it's the only kind you should ever write.

**The `default` is doing real work here.** Without it, `day = 9` produces silence — the
program appears to have done nothing, which is indistinguishable from a crash to a user.

**Complexity:** O(1) — jump table, one indexed jump regardless of case count.

---

## 14. How many digits does `n` have, without a loop

```cpp
#include <iostream>
using namespace std;

int digitCount(int n) {
    if (n < 0) n = -n;                       // sign doesn't affect digit count
    if (n < 10)         return 1;
    if (n < 100)        return 2;
    if (n < 1000)       return 3;
    if (n < 10000)      return 4;
    if (n < 100000)     return 5;
    if (n < 1000000)    return 6;
    if (n < 10000000)   return 7;
    if (n < 100000000)  return 8;
    if (n < 1000000000) return 9;
    return 10;                               // INT_MAX = 2147483647 has 10 digits
}

int main() {
    int n; cin >> n;
    cout << n << " has " << digitCount(n) << " digit(s)\n";

    if (n >= 100 && n <= 999) cout << "It is a 3-digit number\n";
    return 0;
}
```

**`0` has one digit.** `n < 10` catches it — but the loop version
(`while (n > 0) { count++; n /= 10; }`) returns **0** for input 0, which is wrong.
`03_Loops/loops2.cpp:25` special-cases this and gets it right; keep that habit.

**Negative numbers.** Take the absolute value first, or `-45` returns 0 from the loop
version (the `n > 0` condition is false immediately).

**Why a ladder rather than a loop?** For `int` the answer is bounded at 10, so this is
O(1) with no loop at all — and it's what a compiler does internally for integer-to-string
conversion. The O(1) mathematical version is `floor(log10(n)) + 1`, but `log10` is
floating-point and is off by one for values like `1000` on some libraries. **Prefer the
comparison ladder or the loop over `log10`.**

`lec3.cpp:36` writes the 3-digit test as `n > 99 && n < 1000`, which is correct.

**Complexity:** O(1) time, O(1) space.

---

## 15. Character classification

```cpp
#include <iostream>
using namespace std;

int main() {
    char c;
    cout << "Enter a character: ";
    cin >> c;

    if (c >= 'A' && c <= 'Z') {
        cout << "Uppercase letter\n";
        cout << "Lowercase form: " << char(c + 32) << "\n";
    } else if (c >= 'a' && c <= 'z') {
        cout << "Lowercase letter\n";
        cout << "Uppercase form: " << char(c - 32) << "\n";
    } else if (c >= '0' && c <= '9') {
        cout << "Digit, numeric value = " << c - '0' << "\n";
    } else {
        cout << "Special character\n";
    }

    cout << "ASCII: " << (int)c << "\n";
    return 0;
}
```

**These four branches are exactly `isupper`, `islower`, `isdigit` from `<cctype>`.**
Interviewers ask you to write them by hand to see whether you understand that characters
are integers with contiguous ranges. Once you've shown that, reaching for `<cctype>` in
real code is correct — those handle locales, yours doesn't.

**`c - '0'` vs `c`.** For input `'7'`, `c` prints as `7` but *is* `55`. `c - '0'` gives the
integer `7`. Confusing these is the #1 bug in string-to-number parsing.

**The 32 gap.** `'a' - 'A' == 32` because in ASCII the case bit is bit 5. That's why
`c ^ 32` also flips case — a trick you'll meet in `../15_bitwise`. Write `+ 32` / `- 32`
for clarity, or better, `c - 'A' + 'a'`, which doesn't hardcode the gap at all.

**Complexity:** O(1) time, O(1) space.

---

# Section 4 — Extra Practice (approach only)

### Largest Perimeter Triangle — LeetCode 976 (Easy)
Sort the array ascending. Scan from the largest element downward: for each `i`, test
`nums[i-2] + nums[i-1] > nums[i]`. The **first** triple that passes is the answer, because
sorting means no larger perimeter can be valid. Return 0 if none pass. Sorting reduces the
three triangle-inequality checks of #11 to one, since the two smaller sides are already
known. **O(n log n) time** (dominated by the sort), **O(1) extra space**. Revisit after
`../12_sorting`.

### Minimum Sum of Four Digit Number After Splitting Digits — LeetCode 2160 (Easy)
Extract the four digits, sort them ascending as `d0 ≤ d1 ≤ d2 ≤ d3`, and return
`(d0*10 + d2) + (d1*10 + d3)`. The insight: the two **smallest** digits must occupy the
tens places, and how you pair the remaining two across the units places doesn't change the
sum. **O(1) time** (exactly four digits), **O(1) space**.

### Roman numeral character → value, using `switch`
A seven-case `switch` on `char` returning `1, 5, 10, 50, 100, 500, 1000` for
`I V X L C D M`, with `default: return 0`. This is the helper inside LeetCode 13: the full
problem then scans the string and **subtracts** whenever a smaller value precedes a larger
one (`IV` = 4). Isolating the helper first is good practice for decomposing a problem.
**O(1) per character.**

### Days in a month — deliberate `switch` fall-through
Stack `case 1: case 3: case 5: case 7: case 8: case 10: case 12:` → 31; the 30-day months
→ 30; `case 2:` → `isLeap ? 29 : 28`, reusing #4. This is the canonical demonstration that
fall-through between *empty* labels is intended, while fall-through past a *body* is a bug.
**O(1).**

### Min and max of two numbers without `if`
Ternary: `int mn = (a < b) ? a : b;`. Branchless: `mn = b + ((a - b) & ((a - b) >> 31));`
— the arithmetic right-shift of a negative number yields all-ones, so the mask selects
`a - b` or `0`. Worth understanding once, but **do not ship it**: it assumes 32-bit ints
and two's complement, it overflows when `a - b` does, and modern compilers already emit a
branchless `cmov` for the ternary. **O(1).**

### Electricity bill from slab rates
A cumulative else-if ladder: e.g. first 100 units at ₹5, next 100 at ₹7, next 100 at ₹10,
beyond that ₹12. The trap is charging the *whole* consumption at the top slab's rate
instead of charging each slab's portion at its own rate — the bill is a sum of pieces, not
one multiplication. Order the ladder from the highest threshold downward and subtract as
you go. **O(1).**

### Income tax from slabs
Structurally identical to the electricity bill, with one extra subtlety: real tax brackets
are **marginal**, so income of ₹500,001 is not taxed entirely at the higher rate — only the
₹1 above the threshold is. Candidates who compute `income * rateForBracket` have modelled
the wrong thing. **O(1).**

### BMI category
`bmi = weight / (height * height)` with weight in kg and height in **metres** — the unit
conversion is half the bug surface. Then an else-if ladder over the boundaries
(under 18.5 / 18.5–24.9 / 25–29.9 / 30+). Two things to get right: use `double` and divide
by `(height*height)` as a real number, and decide explicitly whether each boundary is
inclusive — `bmi == 25.0` must land in exactly one branch. **O(1).**

---

# Bugs in your current code

Per the repo's convention I have not edited your `.cpp` files. Both files here **compile
cleanly** (verified with `g++ -fsyntax-only`) — these are logic and style issues only.

### `02_Conditional/lec3.cpp`

| Line | Problem | Fix |
|---|---|---|
| 24 | `cout << endl << "absolute value of n is n";` prints the **letter** `n`, not the value | `cout << "absolute value is " << n;` |
| 20-22 | `if (n<0) n = -n;` mutates `n` in place, so the three-digit check at line 36 and the divisibility check at line 44 silently run on the absolute value instead of the input | use a separate `int absN = (n < 0) ? -n : n;` |
| 20 | `-n` is undefined behaviour when `n == INT_MIN` | widen to `long long`, or document the precondition |
| 28-42 | reads `cp`/`sp` as `int`, so a percentage calculation added later would truncate | prefer `double` for money-like quantities |

What's **right** in this file and worth keeping: the even/odd test uses `n % 2 == 0`
(correct for negatives), the max-of-three at lines 56-66 is a correct 2-comparison nested
form, the profit/loss ladder has all three branches, and lines 41-52 correctly demonstrate
that `and`/`or` are real C++ keywords identical to `&&`/`||`.

### `02_Conditional/lec4.cpp`

**This file is correct.** Every `switch` case breaks, both switches have a `default`, and
the division case guards against zero — that is the complete graded content of a
switch-case exercise. Three optional hardenings:

| Line | Refinement |
|---|---|
| 62 | `num2 != 0` on a `double` is exact float equality; `fabs(num2) < 1e-12` is more robust |
| 40 | `double result;` is declared uninitialised — safe here since it's only printed after assignment, but initialise to `0` as a habit |
| 36 | `cin >> operation` on a `char` reads only the first character of the input; if the user types `++`, the second `+` stays in the buffer and is silently consumed by the *next* read |
