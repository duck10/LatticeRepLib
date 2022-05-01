#ifndef IT_TYPELIST_H
#define IT_TYPELIST_H

#include "Niggli_Types.h"
#include <string>
#include <vector>

class IT_TypeList {
public:
//   IT_TypeList(const bool build = true);
//   IT_Lat_Char_Base operator[] (const std::string& s) const;
//   IT_Lat_Char_Base operator[] (const size_t n) const;
//   size_t size() const { return m_deloneTypes.size(); }
//   std::vector<IT_Lat_Char_Base> GetAllTypes() const;
   static std::vector<std::pair<std::string, std::vector<double> > > Make3dCenteringMatrices();
//   //std::vector<DeloneFitResults> Fit(const std::string& type, const S6& s6, const S6& sig, const MatS6& m) const;
//   //std::vector<DeloneFitResults> Fit(const S6& s6, const S6& sig, const MatS6& m) const;
//   static const std::vector<std::string> typelist;
//   static std::vector<std::pair<std::string, std::vector<MatS6> > > CreateAllPrjs();
//   static std::vector<std::pair<std::string, std::vector<MatS6> > > CreateAllPerps();
//   static std::vector<std::pair<std::string, std::vector<MatS6> > > CreateAllToCanon();
//   static std::vector<std::pair<std::string, std::string> > CreateE3CenteringMatrixList();
   static std::vector<std::pair<std::string, std::string> > CreateCharacterList();
//   static std::vector<std::pair<std::string, std::string> > CreateBravaisTypeList();
//
//   std::vector <std::string> GetStrBravaisTypes(const std::string& s) const;
//
//protected:
//   static double GetFreeParams(const std::string& s);
//   static std::vector<double> Make3dVector(const std::string& s);
//   static std::pair<std::string, MatS6 > CreateCenteringMatrix(const std::string& lattice, const std::string& threespaceMatrix);
//   static std::vector<std::pair<std::string, MatS6> > CreateListOfCenteringMatrices();
//   static std::vector<std::pair<std::string, int> > CreateListOfFreeParams();
//   std::vector<std::shared_ptr<IT_Lat_Char_Base> > m_IT_Types;
};


#endif // IT_TYPELIST_H
