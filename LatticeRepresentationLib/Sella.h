#ifndef SELLA_H
#define SELLA_H

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "Delone.h"
#include "DeloneFitResults.h"
//#include "DeloneType.h"
#include "GenerateLatticeTypeExamples.h"
#include "LabeledSellaMatrices.h"
#include "MatS6.h"
#include "S6.h"
#include "StoreResults.h"

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
      return m_distance <sr.m_distance;
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

   Sella();
   double TestOneType(const std::string& label, const S6 &s6, const std::vector<MatS6>& vm);
   std::vector<std::pair<std::string, double> > GetVectorOfFits(const S6& s6);

   std::vector<LabeledSellaMatrices> GetPerps(void) const {
      return perps;
   }

   std::vector<LabeledSellaMatrices> GetProjectors(void) const {
      return projectors;
   }

   std::pair<std::string, double>  GetBestFitForCrystalSystem(const std::string& type, const S6& s6);
   double GetFitForDeloneType(const std::string& type, const S6& s6);
   std::vector<std::pair<std::string, double> > GetAllFitsForCrystalSystem(const std::string& type);

   void GetDeloneTypesForCrystalType();
   void GetCrystalTypesForDeloneType();

   std::vector<DeloneFitResults> SellaFit(
      const std::vector<std::shared_ptr<GenerateDeloneBase> >& sptypes,
      const S6& s6,
      const S6& errors,
      const MatS6& reductionMatrix);

   DeloneFitResults SellaFitXXXXXX(
      const std::shared_ptr<GenerateDeloneBase>& sptype,
      const S6& s6,
      const MatS6& reductionMatrix);

   static double Zscore(const S6& s6, const S6& sigmas, const MatS6& reductionMatrix);

private:
   static const std::vector<LabeledSellaMatrices> projectors;
   static const std::vector<LabeledSellaMatrices> perps;
	//static std::vector<DeloneType> m_latticeCharacters;

private:
};

#endif // SELLA_H



