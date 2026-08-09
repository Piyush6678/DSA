# 12 — Scope and sourcing for the per-folder doc sets (01–13)

## Context

Folders `01_basics` … `13_prefixSum` each received `readme.md`, `questions.md` and
`solution.md`, with Striver's A2Z Steps 1–6 and 9 as the coverage floor. Four choices shaped
the result; all four were put to the author before writing.

**Amended for 11–13 — question counts track topic breadth, and 13 is deliberately the
smallest set.** 30 for `11_linearAndBinarySearch`, 22 for `12_sorting`, 18 for `13_prefixSum`.
Binary search earns the largest set so far because it is three non-transferring skills (array
search, answer-space search, 2D) sharing ten lines of code. Prefix sum gets 18 and the file says
plainly that the topic is narrower than arrays or binary search — padding it to match the others
would have meant more subtraction problems, which is not a skill. Stating the honest reason for a
*small* count matters as much as justifying a large one.

**Also amended: two folders had problem lists that map onto a single technique, and the ordering
preserves that.** `12_sorting`'s readme grouped 268/287/448/41 under "cycle sort" — which is
exactly right, they are one placement loop with four different final scans — so §1 #7–#10 keeps
them consecutive and `solution.md` presents the shared loop once. Similarly `11`'s four
answer-space problems (875, 1011, 2187, plus 410) are kept together. Splitting either group
across sections by difficulty was rejected: the grouping *is* the lesson.

**Amended for 09–10 — the author asked for readme problem lists to be folded into
`questions.md`.** Both folders had a `readme.md` that was purely a list of LeetCode problems
(3 for strings, 6 for recursion). Per [[05_readmes-are-problem-lists]] those are working notes,
so as with 07–08 the original text is preserved verbatim at the foot of the new `readme.md` —
but the author additionally asked that every listed problem *appear in `questions.md`*. All
nine do, in Sections 1–2 (never Section 4), each tagged **"on your list"**, and each
`readme.md` closes with a table mapping the author's entry to its new question number and to
the state of their existing `.cpp` for it. Demoting any of them to "extra practice" was
rejected: the author chose those problems, and burying them would defeat the point of asking.

**Also amended: question counts grew with topic breadth, not with folder size.** 26 for
`09_Strings` and 28 for `10_Recursion` — the largest sets so far — against 14 for
`08_2d array`. The justification stated at the top of each file is the number of *independent
techniques*, not the number of source files: recursion earns 28 because it is the substrate for
four later folders (`21`, `22`, `26`, `27`) and its duplicate-handling rules (LC 78 / 90 / 39 /
40) only make sense done consecutively. `09_Strings` has exactly one `.cpp` file and still
earns 26, which is the clearest evidence the count is driven by the topic rather than by how
much the author has already written.

**Amended for 07–08 — existing readmes are preserved, not overwritten.** These were the
first folders that already had a `readme.md`, and per
[[05_readmes-are-problem-lists]] those files are the author's working notes, not
documentation. `07_Array/readme.md` in particular held two hand-written LeetCode solutions.
Rather than clobber them, the original content is reproduced verbatim in a *"Your original
notes (preserved)"* section at the foot of the new `readme.md`, with pointers to where each
listed problem is now solved. `08_2d array` had its list in a file named `readme .md` — with
a space — so the new `readme.md` sits alongside it; the old file is left in place and flagged
to the author as redundant rather than deleted.

**Also amended: repo order diverges from Striver's.** Striver teaches hashing in Step 1.5,
before arrays; this repo defers maps to `23`/`24` and sorting to `12`. Problems whose optimal
solution needs those are kept (they are core) and marked `[hash]` / `[sort]`, with an in-scope
approach given alongside the optimal one, so nothing is blocked.

**Amended for 05–06.** The author revised the brief so the model chooses the question count
per folder rather than a fixed 15, justified by a stated mastery test. That produced 16 for
`05_Function` and 14 for `06_Pointer` — counts driven by the number of distinct techniques,
not by a template. `06_Pointer` is deliberately drill-dominated and says so up front:
pointers are machinery, and the problems that exercise them are linked lists, which arrive
in `17_linked_list`. Padding it with problems the reader cannot yet solve was rejected for
the same reason invented problem IDs were.

## Decision

1. **Questions are real judge problems, scoped per folder** — each question is solvable
   using only that folder's constructs plus earlier folders. Striver's Basic Maths (1.3)
   lands in `03_Loops`; Striver's 22 patterns (1.2) map one-to-one onto `04_pattern`.
2. **Solution depth**: brute → better → optimal with full compilable C++14, a dry run and
   complexity for the 15 ranked questions; approach and key insight only for Section 4.
3. **LeetCode first, GFG by exact title.** GFG has no numeric problem IDs, so those are
   cited by title with a note to search; patterns are not on LeetCode at all and are
   pointed at takeuforward / Code360.
4. **Existing `.cpp` bugs are flagged in `solution.md`, not fixed** — consistent with
   [[06_dont-fix-incomplete-files]].

## Why

The hard part was (1). `01_basics` and `02_Conditional` have almost no native interview
problems, because virtually every LeetCode problem hands you an array or string and
therefore needs a loop. The alternatives were to pad those folders with theory Q&A, or to
dump all of Basic Maths into `03_Loops` and leave 01–02 thin. Scoping per folder keeps
every folder practice-heavy while staying honest: entries with no judge equivalent are
marked *Drill* rather than dressed up, and `02_Conditional` opens by explaining why it is
drill-heavy.

On (3), citing a problem ID that doesn't exist is worse than citing none — it sends the
reader somewhere wrong and quietly undermines every other reference. Only IDs I was
confident in were used; GFG entries carry titles precisely because GFG has no IDs to cite.

**Every code sample was compiled and executed before publishing.** All 12 pattern functions
were diffed against the documented output, and ~60 assertions covered the Basic Maths
solutions including the boundary cases the prose claims matter (`isPrime(0/1/2/4)`,
`isPal(-121/10/0)`, `myPow(2, INT_MIN)`, `mySqrt(2147395599)`, `rev7(1534236469)`). One
test *expectation* was wrong and the code was right, which is the failure direction to
prefer. Documentation that ships uncompiled code teaches the reader to distrust it.

For 09–10 that came to 284 assertions across five harnesses. Three process notes worth
keeping:

1. **Run each risky function in its own process.** Four of the author's functions abort with
   `0xC00000FD` (stack overflow) and one with `0xC0000094` (integer divide by zero). Because
   `cout` is buffered, a crash discards *all* pending output — a single harness that hits one
   of them prints nothing and tells you nothing about the tests that already ran. Splitting the
   harness by test group is what turned "no output" into five precisely located faults.
2. **`cmd 2>&1 | head; echo $?` reports `head`'s status, not the command's.** An early compile
   sweep looked clean for this reason. Capture the exit code directly, or the check is theatre.
3. **A passing test can be a coincidence.** `subseqK("abcd", k=1)` returns 4, which happens to
   be the right *count* for k=1 — but the function ignores `k` entirely and was returning the
   four length-3 subsequences. Only the k=2 case (4 vs. the correct 6) exposed it. Where a bug
   is suspected, pick the input that distinguishes, not the input that is easy to predict.

For 11–13, another 199 assertions, and two more notes:

4. **My own expected values are the least reliable part of the harness.** Across 09–13, five
   test *expectations* were wrong while the code under test was right: the Dutch-flag output
   earlier, `judgeSquareSum(2147483600)` (it really is 7060² + 45800²), `pivotIndex` on the
   author's `ques.cpp` array (genuinely −1 — the file answers a *different* question, correctly),
   and cycle sort on `{1,1,2}` (it terminates but does not sort, which is a real property of the
   algorithm, not a bug). Every one was resolved by computing the answer independently rather
   than by adjusting the code. When a test fails, suspect the expectation first.
5. **Where a bug is claimed, show the wrong output next to the right one.** The strongest entries
   in these files run the author's version *and* the corrected version on the same input —
   `subarraysDivByK` giving 4 instead of 7, `bubbleSort` returning its input untouched,
   `mergeSort({3,1,2})` producing `{1,1,2}`. A diagnosis without a counterexample is an opinion.

## Rejected alternative

Padding `01_basics` and `02_Conditional` with invented problem IDs to make the platform
column look uniformly populated. Rejected for the reason above. The visible cost is that
`02_Conditional` shows seven *Drill* rows; the file states why, which is more useful to the
reader than a tidier-looking table would have been.
