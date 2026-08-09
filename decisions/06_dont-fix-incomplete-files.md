# 06 — Incomplete and buggy files are left alone unless asked

## Context

Many files are unfinished or contain real defects. Concrete cases found while reading:

- `29_Practice Problems/01_csesRemovingDigits.cpp` — `noOfDigits` counts digits into
  `cnt` then returns `n` (always `0`); `btmup` loops `for (int j = 0; i < d.size(); j++)`,
  testing `i` instead of `j`; `main()` only sizes `dp` and returns.
- `29_Practice Problems/02_minimizingCoins.cpp` — contains the single character `2`.
- `26_dp/dp.cpp` — `fiboTabulation` reads `arr[i+2]` while filling index `i`.
- `28_DSU/DSU.cpp` — `Union` increments `rank[a]`/`rank[b]` rather than the roots
  `rank[para]`/`rank[parb]`; `main()` is empty.

## Decision

Instruct future sessions not to opportunistically fix files they were not asked about,
and to check whether `main()` actually exercises the code before assuming it ran.

## Why

In a normal codebase these are bugs. Here they are timestamps. A half-written tabulation
loop is where the author stopped mid-lesson, and the value of finishing it yourself is
the entire reason the repo exists. Silently correcting it removes the exercise and, worse,
hides that anything was ever incomplete.

The stakes are asymmetric: leaving a bug in a file nobody asked about costs nothing,
because nothing depends on these files. Fixing it costs the author the lesson and buries
the fix in a diff they did not request.

The `main()` warning addresses a specific failure mode — an agent compiles
`01_csesRemovingDigits.cpp`, sees exit code 0, and reports the algorithm verified, when
`main()` never called it.

## Rejected alternative

Listing the known bugs in `CLAUDE.md` as a to-do. Rejected — it reads as a work queue and
invites exactly the unrequested fixing this decision exists to prevent. The examples live
here instead, as evidence for the rule.
