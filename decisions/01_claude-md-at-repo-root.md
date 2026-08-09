# 01 — `CLAUDE.md` at the repo root, framed as a learning journal

## Context

The repo had no `CLAUDE.md`, no top-level `README.md`, and no Cursor or Copilot rule
files. The only configuration present was `.vscode/settings.json`. Nothing described the
repository's purpose in writing.

## Decision

Write a single `CLAUDE.md` at the repository root, opening with an explicit statement
that this is a personal C++ DSA learning journal rather than an application: ~108
standalone files, no shared library, no `#include` relationships between them, one
`main()` per file.

## Why

Every other decision in this folder follows from that framing. Without it, a reader
arriving at a directory of numbered folders full of `.cpp` files will reach for the
wrong instincts — look for an entry point, look for a test runner, try to wire files
together, treat duplication across files as a refactoring opportunity. Stating the
nature of the repo in the first paragraph prevents all of those at once.

Root placement is what Claude Code loads automatically. Per-folder `CLAUDE.md` files
were unnecessary because the topics do not differ in workflow, only in subject.

## Rejected alternative

A `README.md` aimed at human visitors instead. Rejected because the request was
specifically for agent-facing guidance, and a second overlapping document would drift
out of sync with the first. If a human-facing README is ever wanted, it should link here
rather than restate.
