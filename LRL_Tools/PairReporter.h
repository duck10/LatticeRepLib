#ifndef PAIRREPORTER_H
#define PAIRREPORTER_H

template<typename T1, typename T2>
class PairReporter {
public:
   PairReporter(const std::pair<T1, T2>& p)
      : m_pair(p)
      , m_separator("")
   {}

   PairReporter()
      : m_pair()
      , m_separator("")
   {}

   PairReporter(const T1& t1, const T2& t2 )
      : m_pair(std::make_pair(t1,t2)) {}

   PairReporter& operator=(const std::pair<T1, T2>& p) {
      m_pair.first = p.first;
      m_pair.second = p.second;
      m_separator = "";
      return *this;
   }

   PairReporter& operator=(const PairReporter<T1, T2>& p) {
      m_pair.first = p.m_pair.first;
      m_pair.second = p.m_pair.second;
      m_separator = p.m_separator;
      return *this;
   }

   void SetSeparator(const std::string& s) { m_separator = s; }

   friend std::ostream& operator<< (std::ostream& o, const PairReporter& pr)
   {
      o << pr.m_pair.first << pr.m_separator << pr.m_pair.second;
      return o;
   }

   T1 GetFirst(void)  const {return m_pair.first; }
   T2 GetSecond(void) const {return m_pair.second; }

private:
   std::pair<T1, T2> m_pair;
   std::string m_separator;
};

#endif  // PAIRREPORTER_H