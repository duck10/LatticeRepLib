#include "IT_TypeList.h"

//IT_TypeList::IT_TypeList( const bool build /*=true*/) {
//   if (build) {
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_1)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_2)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_3)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_4)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_5)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_6)));
//
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_7)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_8)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_9)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_10)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_11)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_12)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_13)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_14)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_15)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_16)));
//      //m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_17))); NA
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_18)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_19)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_20)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_21)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_22)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_23)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_24)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_25)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_26)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_27)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_28)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_29)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_30)));
//      // IT31 is triclinic
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_32)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_33)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_34)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_35)));
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_36)));
//      //m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_37))); NA
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_38)));
//      //m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_39))); NA
//      m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_40)));
//      //m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_41))); NA
//      //m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_42))); NA
//      //m_IT_Types.push_back(std::move(std::shared_ptr<IT_Lat_Char_Base>(new IT_Lat_Char_43))); NA
//      // IT44 is triclinic
//   }
//}
//
//
//std::vector<std::string> NiggliTypeList::GetStrBravaisTypes(const std::string& s) const {
//   const std::vector<IT_Lat_Char_Base>& types = m_deloneTypes;
//   std::string ss(s);
//   if (s.empty()) ss = "**";
//   if (s.size() == 1) { ss.resize(2); ss[1] = '*'; }
//   std::set<std::string> out;
//   for (size_t i = 0; i < types.size(); ++i) {
//      const std::string match = types[i].GetName();
//      if ((ss[0] == match[0] || ss[0] == '*')
//         && (ss[1] == match[1] || ss[1] == '*'))
//         out.insert(types[i].GetBravaisType());
//   }
//   return out;
//}