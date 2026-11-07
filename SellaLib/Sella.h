#ifndef SELLA_H
#define SELLA_H
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "Delone.h"
#include "DeloneFitResults.h"
//#include "DeloneType.h"
#include "GenerateLatticeTypeExamples.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6.h"
//#include "StoreResults.h"

class SellaResult {
public:
   friend std::ostream& operator<< (std::ostream& o, const SellaResult& sr) {
      o << "Delone name " << sr.GetName() << std::endl;
      o << "Bravais Type " << sr.GetBravaisType() << std::endl;
      o << "distance " << sr.GetDistance() << std::endl;
      o << "original " << sr.GetOriginal() << std::endl;
      o << "projected " << sr.GetProjected() << std::endl;
      //o << "perp " << sr.GetPerp() << std::endl;
      o << "zscore " << sr.GetZscore() << std::endl;
      return o;
   }
   SellaResult() {}
   std::string GetName() const { return m_label; }
   std::string GetBravaisType() const { return m_BravaisType; }
   std::string GetGeneralBravaisType() const { return m_generalBravaisType; }
   double GetDistance() const { return m_distance; }
   S6 GetProjected() const { return m_projected; }
   S6 GetPerp() const { return m_perped; }
   double GetZscore() const { return m_zscore; }
   S6 GetOriginal() const { return m_original; }
   void SetName(const std::string& label) { m_label = label; }
   void SetBravaisType(const std::string& type) { m_BravaisType = type; }
   void SetGeneralBravaisType(const std::string& generalBravaisType) { m_generalBravaisType = generalBravaisType; }
   void SetDistance(const double distance) { m_distance = distance; }
   void SetProjected(const S6& projected) { m_projected = projected; }
   void SetPerp(const S6& perped) { m_perped = perped; }
   void SetZscore(const double zscore) { m_zscore = zscore; }
   void SetOriginal(const S6& s6) { m_original = s6; }
   bool operator< (const SellaResult& sr) {
      return m_distance < sr.m_distance;
   }

private:
   std::string m_label;
   double m_distance;
   double m_zscore;
   S6 m_original;
   S6 m_projected;
   S6 m_perped;
   std::string m_BravaisType;
   std::string m_generalBravaisType;
};

class Sella {
public:
   Sella(const bool useDynamicMatrices = false);  // NEW: constructor with flag to choose matrix source

   // CHANGED: Made non-static to use instance matrices
   std::vector<DeloneFitResults> SellaFit(
      const std::string& selectBravaisCase,
      const S6& s6,
      const S6& errors,
      const MatS6& reductionMatrix);

   // Keep these static for backward compatibility with GrimmerTree
   static DeloneFitResults SellaFitXXXXXX(
      const std::shared_ptr<GenerateDeloneBase>& sptype,
      const S6& s6);
   static double Zscore(const S6& s6, const S6& sigmas, const MatS6& reductionMatrix);

   // FACECROSSED (Nov 2026): when on, SellaFitXXXXXX appends the missing
   // face arrangements (white paper v5, Appendix B) to the type's own
   // projector list. Static because SellaFitXXXXXX is static and is called
   // from many places (SellaFit, Remediation168ForOneSubtype, diagnostics).
   static void SetUseFaceCrossed(const bool b) { s_useFaceCrossed = b; }
   static bool GetUseFaceCrossed() { return s_useFaceCrossed; }

   // CONEFIT (Nov 2026): when on, each projection is clamped to the reduced
   // cone before the distance is taken (see SellaFitXXXXXX).
   static void SetUseConeFit(const bool b) { s_useConeFit = b; }
   static bool GetUseConeFit() { return s_useConeFit; }


private:

   // Defaults match CmdSellaControls (FACECROSSED and CONEFIT both on), so
   // a caller that never sets them gets the same fits as CmdSella.
   inline static bool s_useFaceCrossed = true;
   inline static bool s_useConeFit = true;
};

#endif // SELLA_H
