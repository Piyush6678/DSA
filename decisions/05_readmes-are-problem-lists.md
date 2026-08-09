# 05 — Per-topic `readme.md` files stay terse problem lists

## Context

18 folders contain a `readme.md`. None is prose. `27_graphs/readme.md` reads in full as a
bare list — `733 flood fill`, `1791 center ofstar graph`, `841 keys and row`, `133 clone
graph`, and so on. `10_Recursion/readme.md` is the same shape under a `#Leet code`
heading. They are working checklists of problems to solve or already solved.

## Decision

Document them as lists, not documentation, and instruct explicitly: append in the same
terse style; do not reformat into structured docs.

## Why

A file named `readme.md` containing unpunctuated fragments and typos looks like an
unfinished document, and the reflex is to tidy it — add headings, fix `ofstar` to
`of star`, expand entries into descriptions, sort by difficulty. That reflex would
destroy a scratch list the author appends to while studying, and would produce a large
diff of pure noise across 18 files.

The instruction is stated as a prohibition rather than a description because describing
the current state is not enough to stop a well-intentioned cleanup.

## Rejected alternative

Normalizing them into a consistent template with problem name, number, and link.
Rejected: the format's value is that it takes two seconds to append to. Structure would
make the author stop using them.
