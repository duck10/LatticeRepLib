# SELLA / CmdSella session handoff

## Session summary

This session started from an observed instability in SELLA's fit reporting near
lattice-type boundaries (the O1B/O2/O3 case) and moved through several rounds
of diagnosis, fixes, and verification in `CmdSella`/`GrimmerChains`. The O1B/O2/O3
question itself was **not resolved** and is the main subject for the next chat,
but several real, confirmed bugs were found and fixed along the way in the
general SELLA/Remediation/COMPARE machinery. Those fixes should be considered
settled; the next chat can build on them rather than re-litigating them.

## Confirmed fixes made this session

1. **`EnhancedS6::SetReflectionFunctions()`** — `Reflection22` was listed twice;
   `Reflection23` was never called. Fixed (already checked in by the user
   before this handoff).

2. **`CmdSellaControls.h`** — added `TEST`, `COMPARE`, `SHOWCHAINFAILURES`
   boolean controls (same `InputHandler::registerHandler` pattern as
   `DEBUG`/`PRINTMATRICES`). `SHOWCHAINFAILURES` is drafted but **not yet
   merged** into the user's live build; the user has `DETAILS` (a synonym for
   `DEBUG`) merged instead, and chain-failure printing in `CmdSella.cpp` is
   currently gated on `shouldDebug()` as a result (see note in code).

3. **`LatticeCell.h`** — added a single-string constructor, accepting either
   a leading lattice designator (`"P 10 10 10 90 90 90"`) or bare cell
   parameters (defaults to `P`), and separately `S`/`S6`/`G`/`G6` designators
   for constructing directly from Selling- or Niggli-reduced scalars. The
   `S`/`G` branches construct `S6`/`G6` objects and convert via
   `LRL_Cell(s6)` / assignment from `g6` — **this specific conversion path
   has not been compiler-verified**; confirm `LRL_Cell` actually has an `S6`
   constructor when this is built.

4. **`CmdSella.cpp` — `ComputeDeloneFits`** (new function, factored out of
   `ProcessSella`):
   - Remediation loop changed from a single `if (gcs.HasFailure())` to a
     bounded `while` loop (cap: 10 passes), so more than one violation (or
     one violation needing more than one pass) can be addressed, not just
     the first `GrimmerChainFailure` ever detected.
   - Loop now tracks `lastRepairedType`/`lastRepairedFit` and stops early
     when the **same** violated type is repaired to the **same** fit twice
     in a row (type-aware — a genuinely different violation with a
     different fit value is never mistaken for "no progress").
   - Fixed the duplicate-row bug this uncovered: previously a stalled
     remediation pass appended an identical `DeloneFitResults` row up to 10
     times; now it appends once and stops.
   - `GrimmerChains` is now constructed from `oneLattice` (the Selling-
     reduced S6, matching what the rest of the function's fits are measured
     against) instead of `S6(input.getCell())` (the raw, unreduced cell).
     This also fixed a latent inconsistency in `GenerateSortedFitPlots`
     (its own `s6Norm`/title-line code already assumed `m_s6` was reduced,
     per its own comments — `GenerateSparklinesSVG` is unimplemented and
     was not a risk here).
   - Added debug printing of chain-violation state before/after remediation
     (currently gated on `shouldDebug()`, see item 2).

5. **`GrimmerTree.cpp` — the actual root cause of the "still bad after
   remediation" symptom** (two compounding bugs, both required together):
   - `CheckAllGrimmerChains()` reset `m_hasChainFailure = false` but never
     cleared `m_GrimmerFailures`, so repeated calls **appended** rather than
     rebuilt. `GetFirstFailure()` returns `m_GrimmerFailures[0]`, so it was
     structurally incapable of ever reflecting anything but the very first
     failure ever recorded for an object's lifetime. Fixed: `.clear()`
     added. (Also removed a vestigial, unused `uniqueFailures` local set.)
   - `ReplaceRemediation()` updated the chain **data** (fit values inside
     `m_GrimmerChains`) but never re-ran `CheckAllGrimmerChains()` before
     returning, so `m_hasChainFailure`/`m_GrimmerFailures` were frozen at
     the pre-remediation state regardless of what remediation actually
     found. Fixed: `out.CheckAllGrimmerChains();` added before `return out;`.
   - **Verified fix, live output**: `P 7.062 16.933 17.019 144.338 90.898
     113.499` now correctly reports `"Grimmer chain violation resolved
     after 1 remediation pass(es)"` — the first time this ever worked
     correctly in this session's testing.

6. **`CmdSella.cpp`/new `CmdSellaTests.cpp`** — built-in test suite, wired to
   the `TEST` control (type `test` then `end` at the `cmdsella` prompt).
   `TEST` alone runs the whole suite; `TEST N` runs only test number N
   (matching CmdLMP3's convention) — tests live in a numbered registry
   (`std::vector<NamedTest>` in `CmdSellaTests.cpp`) so adding a test means
   adding one line to that registry, no `InputHandler`/control changes
   needed. Contains:
   - A confirmed-resolves fixture (`P 7.062 ...`, item 5 above).
   - A no-duplicate-rows regression test (item 4 above).
   - A `BuildCompareResults` ignores-block-window test.
   - Six **characterization tests** for confirmed-still-failing cases
     (`oC` vs `tP` violations that `Remediation`'s search cannot currently
     resolve) — these intentionally assert the *current broken* state
     (`HasFailure() == true`) as a tripwire; they should be inverted once
     `Remediation`'s search is improved (see open items).
   - An old, superseded standalone draft, `CmdSellaCompareTests.cpp`, exists
     from earlier in the session and should be **deleted** — it was replaced
     by `CmdSellaTests.cpp` and never wired into the build.

## Correction to an earlier claim in this session

Earlier in this session I incorrectly stated that `InputHandler` could not
parse parenthesized/comma-separated input like `S6(-10, -20, 0, -10, -20, 0)`.
**This was wrong** — `InputHandler::cleanLatticeInput`'s regex
(`[^a-zA-Z0-9.; +\-?]`, replaced with a space) already strips parentheses and
commas into whitespace before tokenization, so that syntax parses identically
to the space-separated form. Confirmed directly via `checkinput` producing
identical output for both forms. No `InputHandler` changes are needed for
this. (I had read `cleanLatticeInput` earlier in the session but failed to
apply it when reasoning about this later — a reminder to check, not guess,
when the source is already in hand.)

## Open items — genuinely unresolved, for the next chat

### 1. The O3/M2B/M3 boundary-type problem (the original motivating issue)

Confirmed, clean, reproducible violation (no remediation-loop noise involved
— this is a raw `sella` fit table, nothing derived):

```
S -10 -20 0 -10 -20 0
```
gives `O1B = 10`, `O2 = 0`, `O3 = 0` in the "Fits for Types" table. Since
O3 ⊂ O1B and O3 ⊂ O2 (O3 is a genuine boundary stratum nested inside both),
the required inequality `fit(O1B) ≤ fit(O3)` is violated (`10 ≤ 0` is false).

**Established this session, not to be re-litigated:**
- `GrimmerChains` is *correctly* scoped to real Bravais types and their
  full-dimensional Delone realizations (O1B, O2, etc.) — it is not missing
  O3 by a bug, it's missing O3 **by design**, because O3 is "not a
  full-dimensional Bravais type" (per the original Delone-grid SVG's own
  footnote). Do not try to extend `GrimmerChains` itself to cover this.
- `MapOfBravaisFits::CreateMapOFBravaisFits` takes the **minimum** raw fit
  across every Delone type sharing a Bravais name (confirmed by reading the
  source) — this is why an O1B-vs-O2 disagreement is invisible to
  `GrimmerChains` even before the O3 question: whichever fits better wins
  silently, before any chain check runs.
- The user's stated position, which should anchor the next chat's scope:
  building a new, separate check *just* for O3/M2B/M3 (the three named
  boundary types per the Delone grid) is **not sufficient**, because the
  same kind of boundary-contact instability can occur between other type
  pairs that have no catalogued/named boundary type at all — i.e. the
  general problem is broader than the three named cases, and those three
  are just the ones that happen to have names.
- Do **not** re-derive boundary-type parentage (which full-dimensional
  types a boundary type nests inside) by eyeballing the Delone-grid SVG's
  letter patterns (`rst`, `rrs`, etc.) — this session got that wrong twice
  by pattern-matching (M3/A2/A3 parentage). The reliable method, established
  but not yet executed, is: test whether a candidate parent's projector
  leaves a candidate child's canonical representative unchanged (checked
  across the 24-reflection orbit), using the real `MatS6` projectors
  already in the codebase (`SellaBuild::vDeloneTypes` /
  `LabeledDeloneTypeMatrices`) — not the SVG text.
- The `S -10 -20 0 -10 -20 0` fixture above (and its two near-boundary
  perturbations from the very first table image in this session) are
  ready-made regression fixtures for whatever check gets built.

### 2. COMPARE mode — verify with real multi-line input

Works structurally for `BuildCompareResults`/`PrintCompareSummary` (verified:
ignores `blockstart`/`blocksize` correctly). **Not yet tested this session
with real multiple lattice-input lines** followed by `compare` + `end` at
the interactive prompt — that's the first thing to actually try. Input
syntax is not a blocker: both `S6(-10, -20, 0, -10, -20, 0)` and
`S -10 -20 0 -10 -20 0` already parse identically today (confirmed via
`checkinput` — `cleanLatticeInput`'s regex normalizes punctuation into
whitespace before tokenization). No `InputHandler` changes are expected to
be needed for this item.

### 3. `Remediation`'s search is still genuinely inadequate

Confirmed via six real fixtures (in `CmdSellaTests.cpp` as characterization
tests) that `Remediation` cannot resolve certain violations (`oC` vs `tP`)
even with correct bookkeeping. Leading suspect, not yet confirmed: `oC`'s
library (`O4`, 36 entries — confirmed NOT sparse) is fully searched by
`SellaFitXXXXXX`, but `Remediation` only tries the 6 fixed elementary
boundary-transform matrices applied to the **input**, with no depth
composition and no reflection-orbit search on the transformed candidate
before fitting. This was flagged as a real gap but not yet fixed or
disproven.

### 4. Confirmed but unfixed, lower priority

- `LabeledSellaMatrices.cpp`: `O4` and `O4B` are byte-for-byte identical
  (confirmed by diff, all 36 matrices, same order) — dead weight / risk if
  one is ever edited without the other, but not the cause of any bug found
  this session (both are used by different Delone subtypes but return
  identical results either way).

### 5. Generic/expandable test framework, matching CmdLMP3's `FlexibleTestMode.h`

The user shared `FlexibleTestMode.h` (CmdLMP3's test framework) at the end
of this session. Key structure, for direct reference in the next chat:

- `FlexibleTestCase`: a data object holding `name`, `description`, and
  `inputCells` (`std::vector<LatticeCell>` — CmdLMP3 always uses 2, for
  pairwise matching, but nothing about the shape requires exactly 2), plus
  one of several **expectation types** (`QualityThresholds`,
  `ExactExpectation`, `ContainsExpectation` — all CmdLMP3-specific: P3
  distance, S6 angle, matrix determinant).
- `FlexibleTestRunner`: holds `testCases` (built via `setupStandardTests()`,
  52 hand-written cases in CmdLMP3), with `runAllTests(controls)` reading
  `controls.getTestNumbers()` — **a `std::vector<int>`, not a single int**
  — supporting `TEST` (run all), `TEST N` (run just N), and `TEST N1 N2 N3`
  (run several specific tests in one invocation). `TEST 999` triggers
  `listTests()`, printing every test's number/name/description without
  running anything — a cheap discoverability feature.
- Per-test result classification is PASS / FAIL / MANUAL (a test with no
  expectations set just displays results for human inspection — see
  CmdLMP3's `test4 "ManualCheck"`).

**Resolved design question — individual vs. comparison mode is not a
fork.** `inputCells` is already a vector. An individual-cell SELLA test is
just the `inputCells.size() == 1` case; a comparison-mode test (e.g. the
O1B/O2/O3 exact-boundary-point-plus-two-perturbations case from early this
session) is the `size() > 1` case. One framework covers both, the same way
CmdLMP3's already does — comparison mode is "more than one input cell,"
not a structurally different kind of test.

What genuinely needs to be new for SELLA is the **expectation types**,
since CmdLMP3's three are all built around P3 distance/S6 angle/matrix
determinant, which are meaningless here. Proposed (not yet built) SELLA
equivalents:
- `FitRangeExpectation` — named type, expected fit value or range
  (individual case).
- `HasFailureExpectation` — expected `true`/`false` for
  `GrimmerChains::HasFailure()` after remediation (individual case).
- `NestingInequalityExpectation` — a pair of type names where one must
  nest inside the other (e.g. O1B ⊃ O3), checked via the certificate
  established early this session (`fit(parent) ≤ fit(child)`) — works for
  both the individual case (one cell) and the comparison case (checked
  per-cell across a set).
- `RankStabilityExpectation` — comparison-case only: no named type's
  best-vs-second-best ranking should flip between consecutive input cells
  in the set. This is the literal, direct check for the original O1B/O2/O3
  instability that started this whole session.

This session's `CmdSellaTests.cpp`/`CmdSellaControls.h` additions
(`TEST`/single `testNumber`, hand-written `NamedTest` registry) are a
smaller, non-generalized stopgap in the meantime — they work, but should
likely be replaced by the above once built, rather than maintained
alongside it. In particular, `CmdSellaControls::getTestNumber()` (single
int) should become `getTestNumbers()` (vector, matching CmdLMP3) if/when
this generalization happens.



Definitely needed:
- `CmdSella.cpp` (this session's version, in outputs)
- `CmdSellaControls.h` (this session's version, in outputs)
- `CmdSellaTests.cpp` (this session's version, in outputs)
- `LatticeCell.h` (this session's version, in outputs)
- `GrimmerTree.cpp` (this session's version, in outputs)
- `GrimmerTree.h`
- `FlexibleTestMode.h` (CmdLMP3's test framework — reference architecture
  for generalizing CmdSella's tests, see item 5 above)
- `SellaBuild.cpp` / `SellaBuild.h` (for the projector/reflection data
  needed to do real containment testing for boundary-type parentage)
- `LabeledDeloneTypeMatrices.h`/`.cpp` if available (holds `vDeloneTypes`
  and the real `MatS6` projectors per type)
- The original Delone-grid SVG (already posted this session — shows the
  24 Delone types, levels, and the O3/M2B/M3 boundary annotations)

Useful if picking up the `Remediation`-search-depth question:
- `S6BoundaryTransforms.h`/`.cpp` (referenced but never posted this
  session — defines `generateOneBoundaryTransform`, the 6 elementary
  matrices `Remediation` uses)
- `Sella.h`/`Sella.cpp` (for `SellaFitXXXXXX` and `GetSellaPerps`/
  `GetSellaProjectors`)

Not needed again (already fully reviewed/settled this session):
- `Selling.cpp`, `MatS6.cpp`/`.h`, `EnhancedS6.cpp`/`.h`, `S6_Ordinals.*`,
  `BravaisHeirarchy.cpp` (confirmed legacy/superseded by `GrimmerChains`,
  do not re-examine), `Grimmer_Grid.svg` (Bravais-letter-level graph,
  already established as too coarse for this problem)
