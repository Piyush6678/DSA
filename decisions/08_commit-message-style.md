# 08 — Follow the existing short-lowercase commit style

## Context

The history is consistent and unlike most conventions. Recent messages: `dsu union`,
`find  parent`, `connectedcomponent`, `bfs`, `all  path dfs`, `tabulation lc 746`,
`heapify algo for min heap`, `push and pop edge case handle`. Lowercase, unpunctuated,
no type prefixes, no scopes, occasional double spaces and typos.

## Decision

Document the style and instruct future sessions to follow that voice.

## Why

Each message names the concept learned in that sitting, which makes `git log --oneline`
a syllabus — a readable record of the path through the material. That is a real property
of this history, and it survives only if new entries match.

Introducing Conventional Commits (`feat:`, `chore:`) would break the reading and impose
software-delivery vocabulary on a study log where "feature" and "fix" do not mean
anything. There is no changelog generation or release tooling to justify the structure.

## Rejected alternative

Standardizing on Conventional Commits going forward. Rejected — it optimizes for tooling
this repo does not have, at the cost of the one thing the log currently does well.
