#ifndef PAIRREPORTER_H
#define PAIRREPORTER_H

template<typename T1, typename T2>
class PairReporter {
public:
   PairReporter(const std::pair<T1, T2>& p)
      : m_pair(p) {}

   friend std::ostream& operator<< (std::ostream& o, const PairReporter& pr)
   {
      o << pr.m_pair.first << " " << pr.m_pair.second << std::endl;
      return o;
   }

   T1 GetFirst(void)  const {return m_pair.first; }
   T2 GetSecond(void) const {return m_pair.second; }

private:
   std::pair<T1, T2> m_pair;
};

#endif  // PAIRREPORTER_H