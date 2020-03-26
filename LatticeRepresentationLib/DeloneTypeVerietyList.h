#ifndef DELONETYPEVARIETYLIST_H
#define DELONETYPEVARIETYLIST_H

class DeloneTypeVarietyList {
public:
   DeloneTypeVarietyList(const std::vector<MatS6>& prjs, const std::vector<MatS6>& perps, const std::vector<MatS6>& toCanons)
      : m_prjs(prjs)
      , m_perps(preps)
      , m_toCanons(toCanons)
   {}

   MatS6 GetPrj(const size_t n) const { return m_prjs[n]; }
   MatS6 GetPerp(const size_t n) const { return m_perps[n]; }
   MatS6 GetToCanon(const size_t n) const { return m_toCanons[n]; }



private:
   std::vector<MatS6> m_prjs;
   std::vector<MatS6> m_perps;
   std::vector<MatS6> m_toCanons;


};


#endif // DELONETYPEVARIETYLIST_H