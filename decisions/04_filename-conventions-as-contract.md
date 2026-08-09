# 04 — Filenames signal intent and decide where new code goes

## Context

`/init` warns against listing every file, since a directory listing is discoverable. But
a flat listing of 108 files is genuinely uninformative here, while the *naming pattern*
is not — it took reading across many folders to see it.

## Decision

Document the naming vocabulary instead of the file inventory:

- `basics.cpp`, `intro.cpp`, `<topic>Algo.cpp` — annotated concept walkthroughs
- `*Implementation.cpp`, `userDefined*.cpp` — hand-rolled structures that deliberately
  avoid the STL equivalent
- `ques.cpp` — the per-topic grab-bag of solved practice problems
- `<name>(<number>).cpp` — one LeetCode problem per file, number is the LeetCode ID

And state the consequence: new problem solutions in an existing topic go in that topic's
`ques.cpp`.

## Why

This is the one piece of repo knowledge that actually requires reading multiple files to
acquire, which is exactly what `/init` asks for. It also answers the most common question
an agent will face — "where does this new code go?" — without another round of
exploration.

The `*Implementation.cpp` entry carries a warning that is easy to get wrong: those files
reimplement stacks and queues by hand *on purpose*. An agent that "improves"
`userDefinedStackArray.cpp` by swapping in `std::stack` destroys the entire point of the
file. Naming the convention names the trap.

## Rejected alternative

A folder-by-folder table of contents. Rejected — it duplicates `ls`, goes stale on the
next commit, and answers a question nobody has.
