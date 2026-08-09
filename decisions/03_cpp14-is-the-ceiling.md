# 03 — C++14 is the language ceiling

## Context

The installed compiler is MinGW.org GCC 6.3.0 (2016). Modern C++ habits — structured
bindings, `std::optional`, `if constexpr` — are reflexive for anyone writing C++ today
and would fail to compile here.

## Decision

Record C++14 as a hard ceiling in `CLAUDE.md`, and state the compiler version and its
default standard rather than just the version number.

## Why

The default standard was verified rather than assumed. GCC's default has moved across
releases (`gnu++98` → `gnu++14` → `gnu++17`), so it was checked directly by compiling a
file that prints `__cplusplus`; it returned `201402`, i.e. `gnu++14`. That means no
`-std=` flag is needed for C++14, which is worth knowing because it makes the documented
compile command a bare `g++ file.cpp -o file.exe` with no flags.

Writing this down converts a confusing failure — an inscrutable compiler error on
idiomatic modern code — into a known constraint stated before the first line is written.

## Rejected alternative

Recommending a compiler upgrade. Rejected as unsolicited: the toolchain works for what
the repo does, and changing it risks breaking 40 committed binaries and the author's
setup for a benefit the repo does not need.
