// GrimmerDeloneTree.h
//
// Delone-expanded version of the Grimmer partial-order plot produced by
// BravaisHeirarchy::ProduceSVG. That plot draws one node per Bravais type
// (14 nodes total), labeled only with the Bravais name and a single fit
// value. Four of those nodes actually cover more than one Delone type
// (mC: M1A/M1B/M2A, oI: O1B/O2, hR: R1/R3, tI: T1/T2), and several of the
// Grimmer-chain violations traced during the SELLA investigation came from
// that collapse: the chain-check compares against whichever sibling has
// the best raw fit, not necessarily the sibling that actually nests the
// child type.
//
// GrimmerDeloneTree draws the same 14-node diagram, in the same layout,
// but every node shows both its Bravais name and its full set of Delone
// siblings with their current fit values. The four multi-Delone nodes are
// drawn as ellipses (tall enough to hold one line per sibling); the other
// ten stay as rectangles, now with a second line for the Delone name.
//
// This is a new, standalone SVG (does not modify BravaisHeirarchy::
// ProduceSVG or its output), so the existing plot keeps working unchanged.
//
// Data source: GrimmerChains::GetChains(), which already carries, for
// each node in each chain template, the node's full GetDeloneTypes()
// list (name + current fit) via BravaisTypeForGrimmer. No new concordance
// data is needed -- the same static Delone/Bravais mapping GrimmerTree.cpp
// already encodes (m_btype_mC, m_btype_oI, m_btype_hR, m_btype_tI, etc.)
// is walked here, not re-declared.

#ifndef GRIMMERDELONETREE_H
#define GRIMMERDELONETREE_H

#include "GrimmerTree.h"
#include "LatticeCell.h"
#include "S6.h"

#include <map>
#include <string>
#include <vector>

class GrimmerDeloneTree {
public:
   // chains must already have current fit data (i.e. UpdateFits has been
   // called on it for this cell) -- this function only reads from it.
   // matches is the same projected-best-fits line list ProcessSella builds
   // via ListMatchingTypes and hands to BravaisHeirarchy::ProduceSVG --
   // passed here as plain text lines (not pre-wrapped in SVG <text>), so
   // this function can lay them out against its own (differently sized)
   // diagram rather than reusing BravaisHeirarchy's fixed x=700 column.
   static std::string ProduceSVG(
      GrimmerChains& chains,
      const LatticeCell& input,
      const std::vector<std::string>& matches);

private:
   struct DeloneNodeEntry {
      std::string deloneName;
      double fit;

      DeloneNodeEntry(const std::string& name, const double f)
         : deloneName(name), fit(f) {
      }
   };

   struct BravaisNodeData {
      std::string bravaisName;
      std::vector<DeloneNodeEntry> deloneEntries;
   };

   struct NodePosition {
      double cx;
      double cy;

      NodePosition() : cx(0.0), cy(0.0) {}
      NodePosition(const double x, const double y) : cx(x), cy(y) {}
   };

   // The four nodes drawn as ellipses because they cover more than one
   // Delone type. Everything else is drawn as a rectangle.
   static bool IsMultiDeloneNode(const std::string& bravaisName);

   static std::map<std::string, BravaisNodeData> CollectNodeData(GrimmerChains& chains);
   static std::map<std::string, NodePosition> CreateNodePositionMap();

   static std::string RenderHeader(const LatticeCell& input);
   static std::string RenderFooter();
   static std::string RenderProjectedFits(const std::vector<std::string>& matches);
   static std::string RenderConnectingLines();
   static std::string RenderNestingLabels();
   static std::string RenderOneStroke(const NodePosition& from, const NodePosition& to,
      const std::string& color, const double width, const bool dashed);
   static std::string RenderAllNodes(const std::map<std::string, BravaisNodeData>& nodeData,
      const std::map<std::string, NodePosition>& positions);
   static std::string RenderRectNode(const BravaisNodeData& data, const NodePosition& pos);
   static std::string RenderEllipseNode(const BravaisNodeData& data, const NodePosition& pos);

   static std::string FormatFitValue(const double fit);
   static std::string EscapeForSVG(const std::string& s);
   // Grimmer's paper (Sec. 1): "mS" is the modern standard name, "mC" is
   // what older ITC-A figures call this same type. Internal identifiers
   // (map keys, edge references, ConfirmedNesting facts) all stay "mC" --
   // that's what GrimmerChains itself uses throughout -- this only
   // changes what gets printed on the node.
   static std::string DisplayNameFor(const std::string& bravaisName);
   // Draws text as several overlapping copies at tiny offsets so it reads
   // bold from the overlap alone -- unlike font-weight or stroke-on-text,
   // plain filled <text> is something every SVG renderer supports, so
   // this doesn't depend on the renderer's font-weight/stroke handling.
   static std::string RenderBoldLabel(const double cx, const double y,
      const double fontSize, const std::string& text);
};

#endif // GRIMMERDELONETREE_H