# 09 — Mass working-tree deletes are folder renames, not accidents

## Context

`git status` at the time of writing showed large blocks of deletions — every file under
`graphs/` and `heap/` staged as deleted — alongside untracked `25_heap/` and `27_graphs/`
directories with matching contents. Git did not pair them as renames because the moves
were untracked on one side.

## Decision

Explain this in `CLAUDE.md` explicitly: the author is renaming topic folders to add the
numeric prefix (`graphs/` → `27_graphs/`, `heap/` → `25_heap/`), and these deletes should
not be read as accidental data loss.

## Why

A status showing a dozen deleted `.cpp` files is alarming, and the plausible reactions
are all bad: restoring the "lost" files with `git checkout`, which would resurrect the
old un-prefixed folders next to the new ones; or flagging it to the user as data loss and
stalling the actual task.

The numbering scheme is the repo's organizing principle (see
[01](01_claude-md-at-repo-root.md)), so folders acquiring prefixes is ordinary
housekeeping, not a mistake. Two minutes of correlation while reading `git status` saves
a future session from a confident wrong move.

## Rejected alternative

Staging the renames to make the status clean. Rejected — mutating the user's index during
a documentation task is a side effect they did not ask for, and the pairing is theirs to
confirm.
