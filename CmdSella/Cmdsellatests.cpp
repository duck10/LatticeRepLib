// CmdSellaTests.cpp
//
// SUPERSEDED -- do not build this file. Its NamedTest registry has been
// replaced by FlexibleSellaTestMode.h's FlexibleSellaTestRunner (TEST /
// TEST N / TEST N1 N2 N3 / TEST 999, wired into CmdSella.cpp's main()).
// Every fixture this file exercised (the resolves-after-remediation cell,
// the six known-unresolved oC/tP characterization cells, the COMPARE
// ignores-block-window case) is already carried forward into
// FlexibleSellaTestMode.h's setupStandardTests() -- see
// FlexibleSellaTestMode.h and SELLA_handoff.md item 5.
//
// This file also relied on CmdSellaControls::getTestNumber(), which no
// longer exists (removed with the rest of the single-int TEST control in
// favor of getTestNumbers()) -- so this file will not compile as-is.
//
// Remove this file (and the still-present, already-superseded
// Cmdsellacomparetests.cpp -- SELLA_handoff.md flagged that one for
// deletion last session and it's still in the .vcxproj per the latest
// build log) from CmdSella.vcxproj rather than fixing them to compile
// again; there's no reason to maintain two parallel test frameworks.
