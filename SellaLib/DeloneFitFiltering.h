#ifndef DELONE_FIT_FILTERING_H
#define DELONE_FIT_FILTERING_H

// DeloneFitFiltering.h
//
// CreateMapForBestExamples / FilterForBestExample / GetMatchedTypeNames --
// extracted verbatim from CmdSella.cpp so a second program (CellClassify)
// can call the exact same "which fit actually counts for this type" logic
// CmdSella.cpp's own ListMatchingTypes uses, rather than risking a second,
// drifting implementation. Same motivation as DeloneFitComputation.h:
// one definition, shared, not duplicated. No behavior change from the
// CmdSella.cpp originals -- pure relocation.
//
// A DeloneFitResults vector for one input cell can contain more than one
// entry for the same GetGeneralType() -- e.g. a raw entry plus a later
// remediated entry appended by ComputeDeloneFits's chain-failure repair
// loop (see RemediatedFit.h / CmdSella.cpp's remediation loop). Any code
// that wants "the one fit that matters per type" for a cell -- which
// includes anything doing near-tie detection across types, not just
// display -- must go through FilterForBestExample first, or it risks
// treating a stale raw entry and its own corrected remediated entry as
// if they were two different types.

#include "DeloneFitResults.h"

#include <map>
#include <set>
#include <string>
#include <vector>

inline std::map<std::string, DeloneFitResults> CreateMapForBestExamples(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{
   std::map<std::string, DeloneFitResults>  bravaisMap;

   for (size_t i = 0; i < vDeloneFitResults.size(); ++i) {
      std::string name = vDeloneFitResults[i].GetGeneralType();
      const double& delta = vDeloneFitResults[i].GetDifference().norm();

      auto mapElement = bravaisMap.find(name);
      if (mapElement == bravaisMap.end())
         bravaisMap.insert(std::make_pair(name, vDeloneFitResults[i]));
      else
         if (delta < (*mapElement).second.GetDifference().norm())
            (*mapElement).second = vDeloneFitResults[i];
   }
   return bravaisMap;
}

inline std::vector<DeloneFitResults> FilterForBestExample(
   const std::vector<DeloneFitResults>& vDeloneFitResults)
{
   std::map<std::string, DeloneFitResults>  bravaisMap = CreateMapForBestExamples(vDeloneFitResults);

   std::vector<DeloneFitResults> out;
   for (auto i = bravaisMap.begin(); i != bravaisMap.end(); ++i) {
      out.push_back((*i).second);
   }
   return out;
}

// GetMatchedTypeNames -- single source of truth for "is this Delone type in
// the reported shortlist." Both ListMatchingTypes (formats the shortlist
// for display) and RemediateIfShortlistChanges (decides whether remediation
// actually changed the shortlist) build on this, so the two can never
// silently disagree about what counts as a match. vFilteredDeloneFitResults
// is expected to already be FilterForBestExample'd (one entry per general
// type) -- this function does not do that filtering itself.
inline std::set<std::string> GetMatchedTypeNames(
   const std::vector<DeloneFitResults>& vFilteredDeloneFitResults,
   const double latticeNorm,
   const double maxDeltaForMatch) {
   std::set<std::string> out;
   for (const auto& type : vFilteredDeloneFitResults) {
      if (type.GetBravaisType()[0] == 'a') continue;
      if (type.GetRawFit() / latticeNorm < maxDeltaForMatch) {
         out.insert(type.GetGeneralType());
      }
   }
   return out;
}

#endif  // DELONE_FIT_FILTERING_H
