# 15 — `advanced_tree_readme.md` is a fourth file, and it lives in `22_bst`

## Context

Every documented folder gets the same three files: `readme.md`, `questions.md`, `solution.md`.
The author asked for a fourth, in one folder only:

> "also add advanced_tree__readme.md for bst containing advance topic for tree like more different
> types of tree for the perspective difficult interview"

The material — AVL, red-black, B/B+ trees, tries, segment trees, Fenwick trees, augmented BSTs,
LCA preprocessing, tree decompositions — has no home in the existing three. It is not concept
teaching for the folder's problems (`readme.md`), it is not a problem list (`questions.md`), and
it answers nothing in that list (`solution.md`).

## Decision

`22_bst/advanced_tree_readme.md` exists as a standalone reference. It is:

- **Scoped to what the three files cannot hold** — structures rather than problems, and the
  reasoning behind them (why databases use B+ trees, why `std::map` chose red-black over AVL).
- **Ordered by when to read it**, not by difficulty. A three-row table at the top splits it into
  "now" (why balance matters, AVL rotations, tries), "after `../26_dp`" (segment trees, Fenwick),
  and "later" (everything else, to discuss rather than to code).
- **Held to the same verification standard as everything else.** AVL rotations, a trie, a segment
  tree, a Fenwick tree and an order-statistic tree were compiled and run — 27 assertions, all
  passing, including the check that inserting `1..7` in sorted order gives an AVL height of 3
  rather than 7.
- **Cross-referenced from the places that need it**, not left to be found: `22_bst/readme.md` §5
  points at it for self-balancing trees, `21_tree/questions.md` §5 for binary lifting, and
  `26_dp/solution.md` for the trie form of Word Break.

The filename uses a single underscore (`advanced_tree_readme.md`); the request's double underscore
was read as a typo.

## Why

The three-file structure earns its consistency by each file having one job. Adding a fifty-item
"advanced structures" appendix to `readme.md` would have doubled that file and buried the material
the folder's own problems need — which is the opposite of the point, since none of the 26 ranked
BST problems require any of it.

`22_bst` is the right host despite covering more than BSTs. Every structure in it is either a
BST with a policy attached (AVL, red-black, treap, splay), a BST with an extra field (order
statistic, interval), or something explicitly contrasted with a BST (trie keyed on prefixes,
B+ tree keyed on page size, segment tree keyed on ranges). The folder is where "a tree with an
invariant" is introduced, and that is the idea the whole file elaborates.

Keeping it a **reference** rather than a curriculum is deliberate. Most of this material is asked
about, not implemented — "why don't databases use red-black trees" is a five-minute conversation
and a two-week implementation. The reading-order table and the closing "what to actually learn"
list exist to stop the file reading as a to-do.

## Rejected alternative

A separate top-level folder — `30_advanced_trees/` — with its own three files. Rejected because it
would need a `questions.md` to match the convention, and there is no honest problem list: tries
have four or five real interview problems (LC 208, 211, 212, 421) and segment trees have almost
none outside competitive programming. Manufacturing twenty questions to justify the folder shape
would have been padding, which [[12_folder-docs-scope-and-sourcing]] rules out.

Also rejected: distributing the material across the folders it touches — tries into `09_Strings`,
segment trees into `13_prefixSum`, Fenwick into `15_bitwise`. Each placement is defensible and the
set of them is unfindable.
