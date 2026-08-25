
#include "LatticeCell.h"
#include "InputHandler.h"

// See LatticeCell.h for why this is out-of-line. The previous inline
// version reimplemented its own narrower parser (designators = {P, A, B,
// C, F, I, H} only -- no S/S6/G/G6/RANDOM/P3), which silently mis-parsed
// anything outside that set instead of erroring: e.g. LatticeCell("S -10
// -20 0 -10 -20 0") folded the literal token "S" into the numeric cell
// string, produced an empty/invalid LRL_Cell with no exception, and the
// failure only surfaced later as a downstream out-of-bounds crash when
// something indexed into the empty vector (confirmed this session via
// FlexibleSellaTestMode.h's S6 fixtures). InputHandler::processSingleLatticeInput
// already has correct, exercised parsing for every format -- this just
// stops duplicating a narrower copy of it.
LatticeCell::LatticeCell(const std::string & s) {
   *this = InputHandler::processSingleLatticeInput(s);
}
