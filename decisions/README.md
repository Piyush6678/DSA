# Decisions

Decision records for this repository — the judgment calls made about how the repo is
described, organized, and worked in, and *why*. One decision per file.

These are not rules imposed on the code. Most of them are observations of what the repo
already does, written down so the next person (or Claude Code session) does not have to
re-derive them by reading 108 files.

| # | Decision | Status |
|---|---|---|
| [01](01_claude-md-at-repo-root.md) | `CLAUDE.md` lives at the repo root and describes the repo as a learning journal | accepted |
| [02](02_no-build-system-single-file-compile.md) | No build system; single-file `g++` compile is the canonical workflow | accepted |
| [03](03_cpp14-is-the-ceiling.md) | C++14 is the language ceiling (toolchain-imposed, verified) | accepted |
| [04](04_filename-conventions-as-contract.md) | Filenames signal intent and decide where new code goes | accepted |
| [05](05_readmes-are-problem-lists.md) | Per-topic `readme.md` files stay terse problem lists | accepted |
| [06](06_dont-fix-incomplete-files.md) | Incomplete or buggy files are left alone unless asked | accepted |
| [07](07_tracked-exe-artifacts.md) | Committed `.exe` artifacts are documented, not retroactively purged | accepted |
| [08](08_commit-message-style.md) | Commit messages follow the existing short-lowercase style | accepted |
| [09](09_folder-renames-not-deletions.md) | Mass working-tree deletes are folder renames, not accidents | accepted |
| [10](10_vscode-settings-not-propagated.md) | `.vscode/settings.json` contents were not carried into `CLAUDE.md` | accepted |
| [11](11_decision-record-format.md) | This folder's own format | accepted |
| [12](12_folder-docs-scope-and-sourcing.md) | Scope and sourcing for the per-folder doc sets in 01–26 | accepted |
| [13](13_pseudocode-first-solution-format.md) | From folder 14 on, solutions are pseudocode-first; code only for fundamentals, implementations, hard and trick problems | accepted |
| [14](14_out-of-scope-advanced-sections.md) | A `questions.md` may carry a Section 5 that breaks the scope rule on purpose — and withholds its answers | accepted |
| [15](15_advanced-tree-readme-as-a-fourth-file.md) | `advanced_tree_readme.md` is a fourth file in `22_bst`, a reference rather than a curriculum | accepted |
| [16](16_revision-folder-has-no-per-problem-solutions.md) | `29_Practice Problems` is a revision set: 150 problems with no solutions, plus 30 answered theory questions | accepted |

## Format

Each record has the same four sections: **Context**, **Decision**, **Why**, **Rejected
alternative**. Add new ones with the next number prefix and a line in the table above.
