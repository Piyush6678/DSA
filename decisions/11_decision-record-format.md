# 11 — This folder's own format

## Context

The decisions behind `CLAUDE.md` were not visible in it. `CLAUDE.md` states conclusions
("do not reformat the readmes") without the reasoning, which is correct for a file read
at the start of every session — but it leaves the conclusions unarguable. You cannot tell
whether a rule was considered or assumed.

## Decision

One decision per file, numbered with the same `NN_` prefix the repo uses for its topic
folders, each with four fixed sections: **Context**, **Decision**, **Why**, **Rejected
alternative**. `README.md` holds the index.

## Why

The numeric prefix keeps the folder in chronological order and matches the repo's own
convention, so it does not read as imported from elsewhere.

**Rejected alternative** is the section that earns its place. A decision recorded without
its discarded option looks inevitable, and the next person cannot tell whether reopening
it is safe. Recording that `*.exe` in `.gitignore` was considered and deliberately not
done ([07](07_tracked-exe-artifacts.md)) is more useful than the decision itself — it
turns a silent omission into an open, revisitable question.

One file per decision keeps diffs legible when a single decision is revised or reversed.

## Rejected alternative

A single `DECISIONS.md` with one section per entry. Rejected — every revision touches the
same file, and the entries drift toward one-liners without room for the reasoning that
makes them worth keeping.
