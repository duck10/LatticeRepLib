// GrimmerDeloneTree.cpp
//
// See GrimmerDeloneTree.h for the rationale. This file lays out its own
// node positions rather than reusing BravaisHeirarchy::BoilerPlate_1's
// hardcoded circle coordinates directly, because four of those nodes now
// need to grow into ellipses (one line per Delone sibling) and the
// original spacing (as little as 55px between some neighboring node
// centers) has no room for that. The layout below keeps the same
// topology -- same rows, same left-to-right ordering, same edges -- just
// spaced out enough for the largest node (mC, three Delone siblings) to
// fit without overlapping its neighbors. Positions are computed once in
// CreateNodePositionMap so RenderConnectingLines and RenderAllNodes always
// agree on where every node actually is.

#include "GrimmerDeloneTree.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_StringTools.h"
#include "NestingCheckResults.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <utility>

namespace {

   const double LINE_HEIGHT = 16.0;
   const double RECT_WIDTH = 130.0;
   const double RECT_HEIGHT = 50.0;
   const double ELLIPSE_RX = 82.0;
   const double NODE_PADDING = 12.0;

   // Matches BravaisHeirarchy::BoilerPlate_1's line vocabulary exactly:
   // SINGLE  -- one solid stroke, width 3 (primary chain edge)
   // DASHED  -- one dashed stroke, width 3 ("conjugate" relationship)
   // DOUBLE  -- black width 6 then white width 2 laid on the same
   //            coordinates (oC-mC only, in the original: "oS-mS linea/lineb")
   // TRIPLE  -- black width 10, white width 6, black width 2 layered the
   //            same way (oP-mP, oI-mC, oF-mC in the original)
   enum class LineStyle { SINGLE, DASHED, DOUBLE, TRIPLE };

   struct EdgeSpec {
      std::string from;
      std::string to;
      LineStyle style;

      EdgeSpec(const std::string& f, const std::string& t, const LineStyle s)
         : from(f), to(t), style(s) {
      }
   };

   inline std::vector<EdgeSpec> CreateEdgeList() {
      std::vector<EdgeSpec> edges;

      edges.push_back(EdgeSpec("cP", "tP", LineStyle::DASHED));
      edges.push_back(EdgeSpec("tP", "oP", LineStyle::SINGLE));

      edges.push_back(EdgeSpec("cF", "hR", LineStyle::DASHED));
      edges.push_back(EdgeSpec("cF", "tI", LineStyle::DASHED));
      edges.push_back(EdgeSpec("cI", "tI", LineStyle::DASHED));
      edges.push_back(EdgeSpec("tI", "oI", LineStyle::SINGLE));

      edges.push_back(EdgeSpec("cI", "hR", LineStyle::DASHED));
      edges.push_back(EdgeSpec("cP", "hR", LineStyle::DASHED));

      edges.push_back(EdgeSpec("hP", "oC", LineStyle::DASHED));
      edges.push_back(EdgeSpec("oC", "mP", LineStyle::SINGLE));
      edges.push_back(EdgeSpec("tP", "oC", LineStyle::SINGLE));

      edges.push_back(EdgeSpec("oC", "mC", LineStyle::DOUBLE));
      edges.push_back(EdgeSpec("oP", "mP", LineStyle::TRIPLE));
      edges.push_back(EdgeSpec("hR", "mC", LineStyle::DASHED));
      edges.push_back(EdgeSpec("tI", "oF", LineStyle::SINGLE));
      edges.push_back(EdgeSpec("oI", "mC", LineStyle::TRIPLE));
      edges.push_back(EdgeSpec("oF", "mC", LineStyle::TRIPLE));

      // aP->mP and aP->mS each have 2 rows in Grimmer's Table 4 (unique
      // axis b vs unique axis c -- alpha=gamma=90 vs alpha=beta=90), the
      // same shape as mS->oS's 2-row case, so they get the same DOUBLE
      // treatment (2026-08-28 correction -- the original single-line
      // rendering here didn't match its own convention).
      edges.push_back(EdgeSpec("mP", "aP", LineStyle::DOUBLE));
      edges.push_back(EdgeSpec("mC", "aP", LineStyle::DOUBLE));
      return edges;
   }

} // anonymous namespace

bool GrimmerDeloneTree::IsMultiDeloneNode(const std::string& bravaisName) {
   return bravaisName == "mC" || bravaisName == "oI" ||
      bravaisName == "hR" || bravaisName == "tI" || bravaisName == "aP";
}

std::map<std::string, GrimmerDeloneTree::NodePosition> GrimmerDeloneTree::CreateNodePositionMap() {
   std::map<std::string, NodePosition> positions;

   // Positions scaled 1.25x (2026-08-29) from the original layout to
   // spread the diagram out -- more room between nodes, same box/ellipse/
   // font sizes, so relative crowding eases without anything getting
   // visually bigger.

   // Cubic row -- right under the header, not ~270 units below it.
   positions["cP"] = NodePosition(250.0, 125.0);
   positions["cF"] = NodePosition(500.0, 125.0);
   positions["cI"] = NodePosition(750.0, 125.0);

   // Tetragonal / hexagonal / rhombohedral row
   positions["tP"] = NodePosition(250.0, 325.0);
   positions["hP"] = NodePosition(387.5, 325.0);
   positions["hR"] = NodePosition(562.5, 325.0);
   positions["tI"] = NodePosition(750.0, 325.0);

   // Orthorhombic row
   positions["oP"] = NodePosition(250.0, 525.0);
   positions["oC"] = NodePosition(387.5, 525.0);
   positions["oF"] = NodePosition(625.0, 525.0);
   positions["oI"] = NodePosition(787.5, 525.0);

   // Monoclinic row
   positions["mP"] = NodePosition(387.5, 725.0);
   positions["mC"] = NodePosition(625.0, 725.0);

   // Anorthic
   positions["aP"] = NodePosition(500.0, 937.5);

   return positions;
}

std::map<std::string, GrimmerDeloneTree::BravaisNodeData> GrimmerDeloneTree::CollectNodeData(
   GrimmerChains& chains) {

   std::map<std::string, BravaisNodeData> nodeData;

   for (const auto& oneChain : chains.GetChains()) {
      for (const auto& btype : oneChain.GetChain()) {
         const std::string bravaisName = btype.GetBravaisType();

         BravaisNodeData& data = nodeData[bravaisName];
         data.bravaisName = bravaisName;

         for (const auto& dtype : btype.GetDeloneTypes()) {
            const std::string deloneName = dtype.GetDeloneName();

            bool alreadyPresent = false;
            for (const auto& entry : data.deloneEntries) {
               if (entry.deloneName == deloneName) {
                  alreadyPresent = true;
                  break;
               }
            }
            if (!alreadyPresent) {
               data.deloneEntries.emplace_back(deloneName, dtype.GetFit());
            }
         }
      }
   }

   // Keep sibling order stable and readable (alphabetical) regardless of
   // which chain template happened to populate the node first.
   for (auto& mapEntry : nodeData) {
      std::sort(mapEntry.second.deloneEntries.begin(), mapEntry.second.deloneEntries.end(),
         [](const DeloneNodeEntry& a, const DeloneNodeEntry& b) {
            return a.deloneName < b.deloneName;
         });
   }

   return nodeData;
}

std::string GrimmerDeloneTree::FormatFitValue(const double fit) {
   std::ostringstream os;
   if (fit >= DBL_MAX || fit < 0.0) {
      os << "--";
   }
   else {
      // Fit values are stored as squared (A^2) distances, same as
      // DeloneFitResults::GetDifference().norm() elsewhere; the displayed
      // plots show Angstroms, so take the square root here, matching
      // BravaisHeirarchy::ScoreLabels' sqrt(scr.second) before display.
      os << std::fixed << std::setprecision(4) << std::sqrt(fit);
   }
   return os.str();
}

std::string GrimmerDeloneTree::EscapeForSVG(const std::string& s) {
   std::string out;
   out.reserve(s.size());
   for (const char c : s) {
      switch (c) {
      case '&':  out += "&amp;";  break;
      case '<':  out += "&lt;";   break;
      case '>':  out += "&gt;";   break;
      default:   out += c;        break;
      }
   }
   return out;
}

std::string GrimmerDeloneTree::RenderBoldLabel(const double cx, const double y,
   const double fontSize, const std::string& text) {
   std::ostringstream ss;
   const std::string escaped = EscapeForSVG(text);
   const double offset = 0.45;

   // Four copies at (0,0)/(offset,0)/(0,offset)/(offset,offset): the
   // overlap thickens every stroke of the glyphs, same visual effect as
   // bold, without depending on font-weight or stroke support.
   const double dx[4] = { 0.0, offset, 0.0, offset };
   const double dy[4] = { 0.0, 0.0, offset, offset };
   for (int i = 0; i < 4; ++i) {
      ss << "  <text x=\"" << (cx + dx[i]) << "\" y=\"" << (y + dy[i])
         << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"" << fontSize
         << "\" font-weight=\"bold\" fill=\"black\">" << escaped << "</text>" << std::endl;
   }
   return ss.str();
}

std::string GrimmerDeloneTree::DisplayNameFor(const std::string& bravaisName) {
   if (bravaisName == "mC") return "mS";
   if (bravaisName == "oC") return "oS";
   return bravaisName;
}

std::string GrimmerDeloneTree::RenderRectNode(const BravaisNodeData& data, const NodePosition& pos) {
   std::ostringstream ss;

   const double x = pos.cx - RECT_WIDTH / 2.0;
   const double y = pos.cy - RECT_HEIGHT / 2.0;

   ss << "  <rect x=\"" << x << "\" y=\"" << y
      << "\" width=\"" << RECT_WIDTH << "\" height=\"" << RECT_HEIGHT
      << "\" rx=\"6\" fill=\"white\" stroke=\"black\" stroke-width=\"2\" />" << std::endl;

   ss << RenderBoldLabel(pos.cx, pos.cy - 6.0, 17.0, DisplayNameFor(data.bravaisName));

   // Single-Delone node: exactly one entry expected, but fall back
   // gracefully to the Bravais name alone if fit data isn't populated yet.
   if (!data.deloneEntries.empty()) {
      const DeloneNodeEntry& only = data.deloneEntries.front();
      ss << "  <text x=\"" << pos.cx << "\" y=\"" << (pos.cy + 14.0)
         << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"13\">"
         << EscapeForSVG(only.deloneName) << "  " << FormatFitValue(only.fit)
         << "</text>" << std::endl;
   }

   return ss.str();
}

std::string GrimmerDeloneTree::RenderEllipseNode(const BravaisNodeData& data, const NodePosition& pos) {
   std::ostringstream ss;

   const size_t siblingCount = data.deloneEntries.size();
   const double textBlockHeight = LINE_HEIGHT * static_cast<double>(1 + siblingCount);
   const double ry = textBlockHeight / 2.0 + NODE_PADDING;

   ss << "  <ellipse cx=\"" << pos.cx << "\" cy=\"" << pos.cy
      << "\" rx=\"" << ELLIPSE_RX << "\" ry=\"" << ry
      << "\" fill=\"white\" stroke=\"black\" stroke-width=\"2\" />" << std::endl;

   // First line: the Bravais name, bold, at the top of the text block.
   double lineY = pos.cy - textBlockHeight / 2.0 + LINE_HEIGHT * 0.75;
   ss << RenderBoldLabel(pos.cx, lineY, 16.0, DisplayNameFor(data.bravaisName));

   for (const auto& entry : data.deloneEntries) {
      lineY += LINE_HEIGHT;
      ss << "  <text x=\"" << pos.cx << "\" y=\"" << lineY
         << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"12\">"
         << EscapeForSVG(entry.deloneName) << "  " << FormatFitValue(entry.fit)
         << "</text>" << std::endl;
   }

   return ss.str();
}

std::string GrimmerDeloneTree::RenderAllNodes(
   const std::map<std::string, BravaisNodeData>& nodeData,
   const std::map<std::string, NodePosition>& positions) {

   std::ostringstream ss;

   for (const auto& posEntry : positions) {
      const std::string& bravaisName = posEntry.first;
      const NodePosition& pos = posEntry.second;

      BravaisNodeData data;
      data.bravaisName = bravaisName;
      const auto found = nodeData.find(bravaisName);
      if (found != nodeData.end()) {
         data = found->second;
      }

      if (IsMultiDeloneNode(bravaisName)) {
         ss << RenderEllipseNode(data, pos);
      }
      else {
         ss << RenderRectNode(data, pos);
      }
   }

   return ss.str();
}

std::string GrimmerDeloneTree::RenderOneStroke(const NodePosition& from, const NodePosition& to,
   const std::string& color, const double width, const bool dashed) {
   std::ostringstream ss;
   ss << "  <line x1=\"" << from.cx << "\" y1=\"" << from.cy
      << "\" x2=\"" << to.cx << "\" y2=\"" << to.cy
      << "\" stroke=\"" << color << "\" stroke-width=\"" << width << "\"";
   if (dashed) {
      ss << " stroke-dasharray=\"10, 9\"";
   }
   ss << " />" << std::endl;
   return ss.str();
}

std::string GrimmerDeloneTree::RenderConnectingLines() {
   const std::map<std::string, NodePosition> positions = CreateNodePositionMap();
   const std::vector<EdgeSpec> edges = CreateEdgeList();

   std::ostringstream ss;

   for (const auto& edge : edges) {
      const auto fromIt = positions.find(edge.from);
      const auto toIt = positions.find(edge.to);
      if (fromIt == positions.end() || toIt == positions.end()) continue;

      const NodePosition& from = fromIt->second;
      const NodePosition& to = toIt->second;

      switch (edge.style) {
      case LineStyle::SINGLE:
         ss << RenderOneStroke(from, to, "black", 3.0, false);
         break;
      case LineStyle::DASHED:
         ss << RenderOneStroke(from, to, "black", 3.0, true);
         break;
      case LineStyle::DOUBLE:
         // Same coordinates, decreasing width, alternating black/white --
         // reproduces BoilerPlate_1's "linea"/"lineb" hollow-line look.
         ss << RenderOneStroke(from, to, "black", 6.0, false);
         ss << RenderOneStroke(from, to, "white", 2.0, false);
         break;
      case LineStyle::TRIPLE:
         ss << RenderOneStroke(from, to, "black", 10.0, false);
         ss << RenderOneStroke(from, to, "white", 6.0, false);
         ss << RenderOneStroke(from, to, "black", 2.0, false);
         break;
      }
   }

   return ss.str();
}

std::string GrimmerDeloneTree::RenderNestingLabels() {
   const std::map<std::string, NodePosition> positions = CreateNodePositionMap();
   const std::vector<EdgeSpec> edges = CreateEdgeList();
   const std::vector<NestingCheckResult> facts = CreateNestingCheckResults();

   std::ostringstream ss;

   for (const auto& edge : edges) {
      const auto fromIt = positions.find(edge.from);
      const auto toIt = positions.find(edge.to);
      if (fromIt == positions.end() || toIt == positions.end()) continue;

      // A fact is directional (parent -> child); an edge's from/to is
      // just line endpoints, so check both orderings. Collect every
      // matching fact -- a multi-Delone child (e.g. oI's O1B and O2)
      // can have more than one row for the same edge.
      std::vector<std::string> lines;
      const NodePosition* parentPos = nullptr;
      const NodePosition* childPos = nullptr;
      for (const auto& fact : facts) {
         const bool forward = fact.parentBravais == edge.from && fact.childBravais == edge.to;
         const bool backward = fact.parentBravais == edge.to && fact.childBravais == edge.from;
         if (!forward && !backward) continue;

         if (parentPos == nullptr) {
            parentPos = forward ? &fromIt->second : &toIt->second;
            childPos = forward ? &toIt->second : &fromIt->second;
         }

         std::string line;
         if (fact.ExcludesNothing()) {
            // Every tested sibling nests -- nothing for the chain-check
            // to get wrong here. Marked with an asterisk rather than
            // omitted, so the diagram shows the audit ran even where it
            // found no problem (see footnote in RenderFooter).
            line = fact.childDeloneType + " *";
         }
         else {
            line = fact.childDeloneType + " via ";
            if (fact.nestingSiblings.empty()) {
               // A real negative result (e.g. mC->hR/R3), not a missing
               // entry -- say so rather than rendering a blank line.
               line += "none confirmed";
            }
            else {
               for (size_t i = 0; i < fact.nestingSiblings.size(); ++i) {
                  if (i > 0) line += ", ";
                  line += fact.nestingSiblings[i];
               }
            }
         }
         lines.push_back(line);
      }
      if (lines.empty()) continue;

      // Label positions in this diagram's own coordinate space, all scaled
      // 1.25x (2026-08-29) to match the scaled node positions above -- same
      // relative placement and margins, just spread out with everything
      // else. Provenance before scaling: six of the eight (tI-cI through
      // mC-oF) are manually-verified (2026-08-28), reverse-engineered from
      // Larry's own hand-corrected SVG after he applied translate() nudges
      // to fix overlaps the bias formula kept getting wrong -- not a guess,
      // his verified-good result. aP->mP and aP->mC were freshly computed
      // (2026-08-29) after mC grew from 3 to 5 siblings and broke their
      // prior verified positions, then raised again per Larry's follow-up
      // ("aP box is too low") -- checked by calculation for real margins,
      // but not yet confirmed against an actual render the way the other
      // six were.
      //
      // oF->tI added 2026-09-01: this edge previously never rendered (its
      // only fact, T1/T2, was ExcludesNothing and got skipped entirely),
      // so it never needed a manual position. Once asterisk labels for
      // ExcludesNothing facts started always rendering, the fallback
      // formula placed this one overlapping the tI ellipse (bottom edge
      // at y=361; fallback put the label's top at y=351). This position
      // is computed to clear that by ~10px -- checked by calculation, not
      // yet confirmed against an actual render.
      static const std::map<std::pair<std::string, std::string>, std::pair<double, double>> manualPositions = {
         { {"tI", "cI"}, {750.0, 222.5} },
         { {"oI", "tI"}, {761.25, 447.5} },
         { {"mC", "oC"}, {483.75, 610.0} },
         { {"mC", "hR"}, {575.0, 460.0} },
         { {"mC", "oI"}, {738.75, 635.0} },
         { {"mC", "oF"}, {612.5, 622.5} },
         { {"aP", "mP"}, {443.75, 831.25} },
         { {"aP", "mC"}, {538.25, 872.5} },
         { {"oF", "tI"}, {700.0, 405.0} },
      };

      const std::string factParent = (parentPos == &fromIt->second) ? edge.from : edge.to;
      const std::string factChild = (parentPos == &fromIt->second) ? edge.to : edge.from;
      const auto overrideIt = manualPositions.find({ factParent, factChild });

      double mx;
      double edgeMidY;
      if (overrideIt != manualPositions.end()) {
         mx = overrideIt->second.first;
         edgeMidY = overrideIt->second.second;
      }
      else {
         // Fallback for any future edge without a verified manual position
         // yet: bias toward the child, away from the (often more crowded)
         // parent. Known to be imprecise -- check the real render before
         // trusting it, the way every use of it so far has needed a fix.
         const double childBias = 0.70;
         mx = parentPos->cx + childBias * (childPos->cx - parentPos->cx);
         edgeMidY = parentPos->cy + childBias * (childPos->cy - parentPos->cy);
      }
      const double topY = edgeMidY - 8.0 - 14.0 * static_cast<double>(lines.size() - 1);

      double maxLineWidth = 0.0;
      for (const auto& line : lines) {
         maxLineWidth = std::max(maxLineWidth, 14.0 + 6.0 * static_cast<double>(line.size()));
      }
      const double boxHeight = 15.0 * static_cast<double>(lines.size());

      // One bordered backing rect covering every stacked line, so a
      // multi-fact edge reads as one labeled box, not several floating
      // ones.
      ss << "  <rect x=\"" << (mx - maxLineWidth / 2.0) << "\" y=\"" << (topY - 12.0)
         << "\" width=\"" << maxLineWidth << "\" height=\"" << boxHeight << "\" rx=\"3\" "
         << "fill=\"white\" stroke=\"#0055AA\" stroke-width=\"1\" />" << std::endl;

      double lineY = topY;
      for (const auto& line : lines) {
         ss << "  <text x=\"" << mx << "\" y=\"" << lineY
            << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"11\" "
            << "font-style=\"italic\" fill=\"#0055AA\">"
            << EscapeForSVG(line) << "</text>" << std::endl;
         lineY += 14.0;
      }
   }

   return ss.str();
}

std::string GrimmerDeloneTree::RenderHeader(const LatticeCell& input, const S6& reducedLattice) {
   std::ostringstream ss;
   ss << "<?xml version=\"1.0\" standalone=\"no\" ?>" << std::endl;
   ss << "<svg width=\"1172.25\" height=\"903.75\" viewBox=\"0,0,1172.25,903.75\" version=\"1.1\" "
      << "xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">" << std::endl;
   ss << "<g transform=\"scale(0.75)\">" << std::endl;
   ss << "<g transform=\"translate(20,30)\">" << std::endl;
   ss << "  <text x=\"0\" y=\"0\" font-family=\"Arial\" font-size=\"22\">SELLA RESULTS by Delone type (Angstroms)</text>" << std::endl;

   std::ostringstream resolved;
   resolved << LRL_Cell_Degrees(reducedLattice);
   const std::string stCell = input.GetInput() + "   " + resolved.str();

   ss << "  <text x=\"0\" y=\"22\" font-family=\"Arial\" font-size=\"16\">" << EscapeForSVG(stCell)
      << "</text>" << std::endl;
   return ss.str();
}
std::string GrimmerDeloneTree::RenderProjectedFits(const std::vector<std::string>& matches) {
   std::ostringstream ss;

   const double x = 975.0;
   double y = 45.0;

   ss << "  <text x=\"" << x << "\" y=\"" << y
      << "\" font-family=\"Arial\" font-size=\"15\">"
      << "Projected best fits ( reported distances (in A^2))</text>" << std::endl;

   for (const auto& cell : matches) {
      y += 20.0;
      ss << "  <text x=\"" << x << "\" y=\"" << y
         << "\" font-family=\"Arial\" font-size=\"14\">"
         << EscapeForSVG(cell) << "</text>" << std::endl;
   }

   return ss.str();
}

std::string GrimmerDeloneTree::RenderFooter() {
   std::ostringstream ss;
   // aP is at scaled cy=937.5 (unscaled 750, ry=44 unscaled), bottom edge
   // at scaled ~1035.5 -- these two lines leave real clearance below it.
   // (Corrects a stale value here from when aP was briefly at cy=800.)
   ss << "  <text x=\"0\" y=\"1017.5\" font-family=\"Arial\" font-size=\"14\" fill=\"#666666\">"
      << "Grimmer, Hans. \"Partial order among the 14 Bravais types of lattices: basics and applications.\""
      << " Acta Crystallographica Section A: Foundations and Advances 71, no. 2 (2015): 143-149."
      << "</text>" << std::endl;
   ss << "  <text x=\"0\" y=\"1046.25\" font-family=\"Arial\" font-size=\"14\" fill=\"#666666\">"
      << "Solid lines: normal-subgroup relationships (a single, direct specialization)."
      << "</text>" << std::endl;
   ss << "  <text x=\"0\" y=\"1075\" font-family=\"Arial\" font-size=\"14\" fill=\"#666666\">"
      << "Dashed lines: conjugate-subgroup relationships (several symmetry-equivalent specializations related by basis choice)."
      << "</text>" << std::endl;
   ss << "  <text x=\"0\" y=\"1103.75\" font-family=\"Arial\" font-size=\"14\" fill=\"#666666\">"
      << "Double/triple strokes: that number of geometrically distinct conditions (e.g. which angle reaches 90 deg) produce the same specialization."
      << "</text>" << std::endl;
   ss << "  <text x=\"0\" y=\"1132.5\" font-family=\"Arial\" font-size=\"14\" fill=\"#666666\">"
      << "Ellipse nodes cover more than one Delone type."
      << "</text>" << std::endl;
   ss << "  <text x=\"0\" y=\"1161.25\" font-family=\"Arial\" font-size=\"14\" fill=\"#666666\">"
      << "* -- every tested sibling nests here; excludes nothing."
      << "</text>" << std::endl;
   ss << "</g>" << std::endl;
   ss << "</g>" << std::endl;
   ss << "</svg>" << std::endl;
   return ss.str();
}

std::string GrimmerDeloneTree::ProduceSVG(
   GrimmerChains& chains,
   const LatticeCell& input,
   const std::vector<std::string>& matches,
   const S6& reducedLattice) {

   const std::map<std::string, BravaisNodeData> nodeData = CollectNodeData(chains);
   const std::map<std::string, NodePosition> positions = CreateNodePositionMap();

   std::string svg;
   svg += RenderHeader(input, reducedLattice);
   svg += RenderConnectingLines();
   svg += RenderAllNodes(nodeData, positions);
   svg += RenderNestingLabels();
   svg += RenderProjectedFits(matches);
   svg += RenderFooter();
   return svg;
}