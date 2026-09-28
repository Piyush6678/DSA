# 14 — Advanced sections deliberately break the scope rule, and withhold their answers

## Context

Every `questions.md` from `01` onward carried a scope note: *every problem must be solvable using
only constructs from this folder and earlier ones.* That rule is what makes the lists usable in
order — a problem you cannot yet solve is discouraging, not aspirational.

After `21_tree` was finished the author asked for something the rule forbids:

> "in 21_tree folder add section 5 advanced question that use multi concept and advance tree
> apprach. you can also put questions that require the topics like bst, maps, dp, heap, that is
> not covered by far. In solution.md just explain this question and which concepts this advanced
> question use, dont write the answer."

So: problems that need folders `22`–`27`, placed in folder `21`, with the solutions withheld.

## Decision

A `questions.md` may carry a final **Section 5 — Advanced / multi-concept** that is explicitly
exempt from the scope rule, provided it:

1. **Says so at the top of the file.** The existing scope note now reads "Sections 1–4 need only
   `01`–`20`" and Section 5 is introduced as breaking that rule on purpose.
2. **Names the prerequisite per problem.** Every row has a **Needs** column (`../26_dp`,
   `../24_maps`, …) so a problem can be deferred rather than bounced off.
3. **Groups by the *other* technique, not by the folder's own.** 21's Section 5 is subdivided
   into DP-on-trees, BST, maps/sets, and graphs — because the grouping is the information.
4. **Is counted separately.** 21_tree is "34 ranked, plus 16 in Section 5", not 50. The 34 is a
   claim about what the folder teaches; the 16 is a claim about what comes after it.

In `solution.md`, Section 5 entries get **what the problem is really asking**, **which techniques
it combines**, and **what the trap is** — and no solution, no pseudocode, no recurrence.

## Why

The scope rule exists so a reader can work top to bottom without hitting a wall. Section 5 serves
a different reader — the one who has finished the folder and wants to know what it leads to — and
labelling it as out of scope costs nothing while leaving the main sections clean.

Withholding the answers is the author's instruction and it is also correct on the merits. These
problems are chosen because they stop being tree problems partway through; the skill they test is
**noticing the transition**, and a solution you read hands you the algorithm while skipping the
noticing. For Sections 1–3 the reverse is true — those teach techniques, and a worked technique is
worth reading.

The "what it combines / what the trap is" format is what remains useful once the answer is gone: it
prevents the classic failure of an advanced list, which is spending an hour on a problem whose
difficulty was a detail in the statement (LC 987's tie-break, LC 38's descending tie-break) rather
than the algorithm.

## Rejected alternative

Putting these problems in the folders whose techniques they need — 337 in `26_dp`, 652 in
`24_maps`, 1373 in `22_bst`. Rejected because it inverts what makes them hard: in `26_dp`,
House Robber III is "tree DP" and the pattern is handed to you by the folder name. The recognition
problem only exists when the problem arrives labelled *tree*. (Cross-references to those folders
are listed in both places instead, so nothing is lost.)

Also rejected: giving full solutions and marking them "spoiler". A spoiler you can scroll to is a
spoiler you will read. See [[13_pseudocode-first-solution-format]] for the same argument applied
one level lower.
