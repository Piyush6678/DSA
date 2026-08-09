# 02 — No build system; single-file `g++` compile is the workflow

## Context

`/init` asks for build, lint, and test commands. This repo has none of them. There is no
Makefile, no CMakeLists, no task runner, no test framework. Files do not include one
another, so nothing needs linking beyond a single translation unit.

## Decision

Document the actual workflow — compile one file, run the resulting binary — and state
plainly that there is no test command:

```powershell
g++ "27_graphs/bfs.cpp" -o "27_graphs/bfs.exe"
./27_graphs/bfs.exe
```

"Verification" is defined as compiling a file and running its `main()`.

## Why

Inventing a Makefile or a CMake setup would have been fabricating a workflow the author
does not use, and `/init` explicitly warns against making things up. The honest answer is
more useful than a plausible one: a future session that believes a test suite exists will
waste a turn looking for it, then waste another inventing one.

Naming the absence also protects against a subtler failure — an agent reporting "tests
pass" when it merely compiled a file whose `main()` is empty.

## Rejected alternative

Adding a build system as an improvement. Rejected as out of scope and actively harmful
here: the per-file compile *is* the pedagogy. Each file is meant to be opened, compiled,
and run in isolation while learning that one topic. A build graph would add ceremony to
the single operation the repo exists to perform.
