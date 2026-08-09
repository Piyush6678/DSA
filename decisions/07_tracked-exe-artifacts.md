# 07 — Committed `.exe` artifacts are documented, not purged

## Context

About 40 compiled `.exe` binaries are tracked in git alongside their sources
(`01_basics/lec1.exe`, `06_Pointer/pointer.exe`, and so on). `.gitignore` contains a
single line: `.claude`. The working tree also shows several `.exe` files staged as
deleted.

## Decision

State the situation in `CLAUDE.md` as existing practice, and add one preference: adding a
new binary to a commit matches the pattern, but prefer not to unless asked.

## Why

Committing build output is conventionally wrong, and the fix is one `.gitignore` line
plus a `git rm --cached`. But that is a repo-wide history-touching change nobody
requested, and it would collide with the folder renames already in flight (see
[09](09_folder-renames-not-deletions.md)).

The soft preference is the useful half. It stops an agent from adding *more* binaries
while compiling during a task, without unilaterally rewriting what is already there. If
the author wants the cleanup, it is one sentence away and better done deliberately on a
clean tree.

## Rejected alternative

Extending `.gitignore` with `*.exe` as part of this work. Rejected as an unrequested
change with real consequences — it would immediately mark 40 tracked files as ignored
while leaving them tracked, a confusing half-state. Worth proposing to the author; not
worth doing silently.
