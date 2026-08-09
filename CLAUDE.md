# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repository is

A personal C++ DSA learning journal, not an application. ~108 standalone `.cpp` files spread across numbered topic folders (`01_basics` … `29_Practice Problems`), each compiled and run individually. There is no build system, no test suite, no shared library, and no `#include` relationships between files — every file is self-contained with its own `main()`.

The numbering encodes chronological learning order, and folder order roughly matches commit history (basics → arrays → recursion → sorting → linked list → stack/queue → tree/BST → sets/maps → heap → DP → graphs → DSU).

`decisions/` records the reasoning behind the guidance below, one file per decision, including the alternatives that were considered and rejected. Read the relevant record before overriding anything here.

## Build and run

Single file, from the repo root (MinGW g++ 6.3.0 on Windows, defaults to `-std=gnu++14`):

```powershell
g++ "27_graphs/bfs.cpp" -o "27_graphs/bfs.exe"
./27_graphs/bfs.exe
```

The toolchain is old — C++17 features (structured bindings, `std::optional`, `if constexpr`) will not compile. Stay within C++14.

There is no "run the tests" command. Verification means compiling a file and running its `main()`, which usually prints to stdout or does nothing at all.

## File conventions within a topic folder

Filenames signal intent and are worth reading before choosing where to put new code:

- `basics.cpp`, `intro.cpp`, `<topic>Algo.cpp` — annotated walkthroughs of the core concept/algorithm.
- `*Implementation.cpp` / `userDefined*.cpp` (e.g. `18_stack/userDefinedStackArray.cpp`, `20_queu/arrayImplementation.cpp`) — hand-rolled data structures, deliberately not using the STL equivalent.
- `ques.cpp` — a grab-bag of solved practice problems for that topic, many free functions plus one `main()`. This is the usual home for new problem solutions in an existing topic.
- `<problemName>.cpp` or `<name>(<number>).cpp` (e.g. `10_Recursion/kthGrammar(779).cpp`, `11_linearAndBinarySearch/ship(1011).cpp`) — one LeetCode problem per file; the number in parentheses is the LeetCode ID.
- `readme.md` — not prose. It is a bare running list of LeetCode problem numbers and titles the author intends to solve or has solved for that topic (see `27_graphs/readme.md`). Append lines in the same terse style; do not reformat these into structured docs.

Solutions are written in the LeetCode-submission idiom: free functions or a `Solution`-style method taking the input directly, `using namespace std;`, minimal formatting. Match that — do not introduce namespaces, headers, or abstraction layers.

## Working with existing files

Many files are incomplete or contain bugs by nature of being learning artifacts: empty `main()` bodies that only size a `dp` vector, half-written tabulation loops, functions that return the wrong variable. Do not opportunistically "fix" files you were not asked about — an apparently broken file may be exactly where the author left off mid-lesson. When asked to work in a file, check whether `main()` actually exercises the code before assuming it ran.

## Git

- Compiled `.exe` artifacts are committed alongside sources (~40 tracked). `.gitignore` currently contains only `.claude`. Adding a new binary to a commit matches existing practice, but prefer not to unless the user asks.
- Commit messages are short, lowercase, unpunctuated descriptions of the lesson just learned (`bfs`, `find  parent`, `tabulation lc 746`, `heapify algo for min heap`). Follow that voice.
- The working tree may show large blocks of deletes paired with untracked folders — this is the author renaming topic folders to add the numeric prefix (`graphs/` → `27_graphs/`, `heap/` → `25_heap/`). Do not interpret these as accidental deletions.
