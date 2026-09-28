# 16 — `29_Practice Problems` is a revision set: 150 problems, no solutions, and 30 theory answers instead

## Context

Folders `01`–`28` each got the same three files: `readme.md` teaches the concept, `questions.md`
ranks problems for that concept, `solution.md` explains how to solve them. That shape works because
each folder is *about* something.

Folder `29` is not about anything. It is the last folder, it holds two CSES scratch files, and the
author asked for something structurally different:

> "in 29_practiceProblem add questions.md in which there are 150 question that i should practice
> after completetion of course and before interview for revision and practice contain mrq and pyq
> of faang companies also add 30 theoretical question in solution.md of 29 folder only write
> solution of theoretical question"

So: 150 coding problems with **no** solutions, and 30 *theoretical* questions **with** solutions,
in the file normally reserved for coding solutions.

## Decision

`29_Practice Problems` departs from the three-file convention in four specific ways.

1. **No `readme.md`.** The author named two files and the folder teaches no concept. Adding a third
   to satisfy a pattern would be filler.

2. **`questions.md` carries no solutions and no per-problem hints** — only the LeetCode number,
   difficulty, a **technique label** (four to eight words), and company tags. The technique label is
   the deliberate substitute for a solution: enough to check recognition, not enough to skip the
   work.

3. **`solution.md` answers only the 30 theoretical questions.** It opens by saying the coding
   problems have no solutions here and why.

4. **A three-pass protocol is stated at the top**, and Pass 1 is *recognise without coding*. The
   file is explicit that this is the pass people skip and the one that matters, because interview
   failure is rarely "I did not know Dijkstra" and usually "I did not notice this was Dijkstra".

Two supporting decisions:

**Company tags carry a stated provenance and a stated limitation.** They come from public
sources — LeetCode's company lists, GFG archives, Glassdoor, Blind — and the file says so, then says
plainly: *"Use the tags to prioritise, never to predict."* The reframe offered is that a
four-company tag is worth doing first because it marks a *widely useful pattern*, which is also why
four companies converged on it.

**Two explicit subsets, both counted.** ★ marks 87 problems (verified by grep, not asserted), and a
separate hard cut names exactly 50 (verified: 50 numbers, no duplicates). An earlier draft labelled
the star tier "the ★ 50" while actually starring 87 — caught by counting rather than by reading.

## Why

**Solutions would destroy the file.** Revision is retrieval practice: the value is entirely in
failing to recall something and then noticing that you failed. A visible solution converts that into
recognition, which feels like learning and is not. This is the same argument as
[[14_out-of-scope-advanced-sections]], applied to a whole folder instead of one section, and it is
also why the technique labels are short enough to confirm an answer but too short to reconstruct
one.

**The theory questions genuinely have no other home.** "Why is comparison sorting Ω(n log n)",
"what is amortised complexity", "what is iterator invalidation" are asked constantly and belong to
no topic folder. Scattering them across 28 `readme.md` files is where they already are, in fragments;
gathering the cross-cutting ones in the last folder is what makes them revisable.

**And they are the half of the interview a problem list cannot cover.** The 150 problems test
whether you can produce an algorithm. The 30 questions test whether you can explain one — which is
what is actually happening while you write it.

Every C++ claim in the theory answers was compiled and run rather than recalled: the vector growth
factor (measured: exactly ×2, 11 reallocations for 1000 `push_back`s), the binary-search overflow
(measured: `(lo+hi)/2` gives −147483647), `std::sort`'s instability (measured at n=32 with tied
keys — and note it *agreed* with `stable_sort` on regular inputs, so an unverified claim here would
have been wrong in the other direction), iterator invalidation after a reallocating `push_back`, and
silent integer overflow. See [[12_folder-docs-scope-and-sourcing]].

## Rejected alternative

**Writing solutions for all 150.** Rejected on the author's instruction, and independently on the
merits — it would be roughly 40,000 words duplicating what `01`–`28`'s `solution.md` files already
say, since every one of the 150 is an instance of a pattern already documented. The
cross-references in the technique column point back at those files instead.

**Also rejected: inventing per-company sub-lists** ("Amazon's 40", "Google's 30"). The public data
does not support that granularity — it supports "reported at", not "asked by, currently". Splitting
by company would have implied a precision the sources do not have, and would have made the list
worse by hiding that the same forty problems dominate every company's tag set.

**Also rejected: a fourth file** (`theory.md`) to keep `solution.md` free for coding solutions that
do not exist. It would leave `solution.md` empty or absent, which is worse than repurposing it with
a one-paragraph explanation at the top.
