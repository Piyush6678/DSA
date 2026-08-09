# 13 — Solutions are pseudocode-first from folder 14 onward

## Context

Folders `01`–`13` got `solution.md` files with full, compilable C++ for every ranked question.
By `11_linearAndBinarySearch` that file had reached 6,400 words and the code was starting to
crowd out the reasoning — three near-identical binary searches in a row teach less than one
plus a note saying "the other two differ only in the comparison".

Before folder 14 the author changed the brief:

> "from now on dont write full code for every problem solution only write pseudo code, algo and
> explanation. only write code for very fundamental problem, implementation or hard problem or
> trick problem."

## Decision

`solution.md` from `14_Sliding window` onward leads with **approach → pseudocode → complexity →
key insight**. Real compilable C++ appears only when it falls into one of the author's four
categories:

1. **Fundamental** — the thing itself, where the code *is* the lesson. Pointer reversal, the
   two window templates, `slow`/`fast`.
2. **Implementation** — hand-rolled data structures, where the whole exercise is writing it.
   `16_oops` is almost entirely this, so it keeps the most code of the four folders.
3. **Hard** — where an ordering or edge case cannot be conveyed in prose. Reverse-in-k-groups,
   the LRU cache.
4. **Trick** — where the solution hinges on one non-obvious line that must be seen exactly.
   Floyd's cycle entrance, `ones/twos` for Single Number II, the `x & -x` isolation.

Everything else gets pseudocode in a neutral, language-free style, so the reader has to write
the C++ themselves.

Two things do **not** change:

- **Bug reports still carry real code.** A claimed bug needs the author's exact line and a
  corrected version — a diagnosis in pseudocode is not checkable. `solution.md`'s bug section
  stays as concrete as it was.
- **Everything shipped is still compiled and executed.** The code samples in 14–17 came to 77
  assertions. Fewer samples means fewer to verify, not a weaker standard, and pseudocode is
  written *from* verified code rather than instead of it.

## Why

The author is past the stage where transcribing a solution helps. Reading working code and
believing you could have written it is the main failure mode of a curated problem list, and
pseudocode forces the translation step that turns recognition into recall. It also compresses
the files enough that the ranking rationale and the complexity discussion stay visible instead
of being buried between code blocks.

The four exception categories are the author's own, and they are well chosen: they are exactly
the cases where prose loses information. "Reverse a linked list by rewiring three pointers" is a
sentence anyone can nod at and few can write correctly on the first try.

## Rejected alternative

Dropping code entirely, including from the bug sections. Rejected because a bug report without
the corrected line is an assertion rather than a finding — the reader cannot check it, and the
whole value of [[06_dont-fix-incomplete-files]] is that the author fixes their own code from a
precise description. Also rejected: keeping full code but moving it to a collapsed appendix,
which is the same volume of text with an extra click.
