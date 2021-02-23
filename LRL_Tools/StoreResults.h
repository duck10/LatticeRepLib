#ifndef STORERESULTS_H
#define STORERESULTS_H

#include <algorithm>    // std::sort
#include <functional>   // std::greater
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <ctime>
#include <utility>
#include <vector>

template <typename TKEY, typename TDATA>
class StoreResults {

template<typename TSAMPLEKEY, typename TSAMPLE>
class SampleData {
public:
   SampleData(const TSAMPLEKEY& key, const TSAMPLE& sample)
      : m_sampleData(1, sample)
      , m_countPerKey(1)
      , m_key(key)
      , m_creationTime()
   {}

   bool operator< (const SampleData<TSAMPLEKEY, TSAMPLE>& ts) const { return m_key < ts.m_key; }

   size_t GetCount(void) const { return m_countPerKey; }
      SampleData operator= ( const SampleData& data ){ 
         m_sampleData = data.m_sampleData;
         m_key = data.m_key;
         m_countPerKey = data.m_countPerKey;
         m_creationTime = data.m_creationTime;
         return *this;
      }
   std::vector<TSAMPLE> m_sampleData;
   size_t m_countPerKey;
   TSAMPLEKEY m_key;
   protected:
      time_t m_creationTime;
};

public:

   friend std::ostream& operator<<(std::ostream& os, const StoreResults<TKEY, TDATA>& store) {
      os << "Contents of StoreResults" << std::endl;
      os << "m_title          " << store.GetTitle() << std::endl;
      os << "m_nmax           " << store.GetNmax() << std::endl;
      os << "Total Seen       " << store.GetTotalSeen() << std::endl;
      os << "m_data.size()    " << store.size() << std::endl;
      os << "m_itemSeparator  " << store.GetItemSeparator() << std::endl;
      os << "m_valueSeparator " << store.GetValueSeparator() << std::endl;
      return os;
   }

   StoreResults(const size_t nMax = 1)
      : m_nmax(nMax)
      , m_keyName("Key")
      , m_creationTime(static_cast<long int>(time(NULL))) 
   {}

   void Store(const TKEY& key, const TDATA& data) {
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::iterator itfind = m_tree.find(key);
      if (itfind == m_tree.end()) {
         m_tree.insert(std::make_pair(key, SampleData<TKEY, TDATA>(key, data)));
      }
      else {
         if ((*itfind).second.m_sampleData.size() < m_nmax)
            (*itfind).second.m_sampleData.push_back(data);
         ++(*itfind).second.m_countPerKey;
      }
   }

   size_t size(void) const { return m_tree.size(); }

   bool empty(void) const { return m_tree.empty(); }

   void erase( const TKEY& key) {
      m_tree.erase(key);   
   }

   TKEY GetByTreeIndex(const size_t index) const {
      return m_tree[index];
   }

   size_t GetItemCount(const TKEY& key) const {
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator itfind = m_tree.find(key);
      if (itfind != m_tree.end()) {
         return (*itfind).second.GetCount();
      }
      else
         return 0UL;
   }

   size_t GetTotalSampleCount(void) const {
      std::vector < TKEY > keys = GetKeys();
      size_t totalSampleCount = 0UL;

      for (size_t i = 0; i < keys.size(); ++i) {
         totalSampleCount += GetResult(keys[i]).size();
      }
      return totalSampleCount;
   }

   size_t GetTotalSeen() const {
      size_t totalSeen = 0UL;
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it;
      for (it = m_tree.begin(); it != m_tree.end(); ++it) {
         totalSeen += (*it).second.m_countPerKey;
      }
      return totalSeen;
   }

   std::string GetHerald() const {
      std::ostringstream ostr;
      if (m_title.empty()) {
         ostr << "ShowResults       ";
      }
      else {
         ostr << m_title << std::endl;
      }
      ostr << "Total Keys = " << GetKeys().size()
         << "  Total Samples = " << GetTotalSampleCount()
         << "  Total Seen = " << GetTotalSeen() << std::endl;
      return ostr.str();
   }

   size_t GetNmax(void) const { return m_nmax; }

   std::vector<std::pair<TKEY, TDATA> > GetResult(const TKEY& key) const {
      const typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it = m_tree.find(key);
      if (it == m_tree.end()) {
         return std::vector<std::pair<TKEY, TDATA> >();
      }
      else {
         std::vector<std::pair<TKEY, TDATA> > v;
         for (size_t i = 0; i < (*it).second.m_sampleData.size(); ++i) {
            const std::pair<TKEY, TDATA>  sd(key, (*it).second.m_sampleData[i]);
            v.push_back(std::make_pair(key, (*it).second.m_sampleData[i]));
         }
         return v;
      }
   }

   std::vector<TKEY> GetKeys() const {
      std::vector<TKEY> v;
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it;
      for (it = m_tree.begin(); it != m_tree.end(); ++it) {
         const std::pair<TKEY, SampleData<TKEY, TDATA> > ctree = *it;
         v.push_back(ctree.first);
      }
      return v;
   }

   void ShowItem(const TKEY& key) const {
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it = m_tree.find(key);

      std::cout << m_keyName << "= " << key << "    (total=" << (*it).second.m_countPerKey << ")" << std::endl;
      for (size_t k = 0; k < (*it).second.m_sampleData.size(); ++k) {
         std::cout << (*it).second.m_sampleData[k] << std::endl;
      }
   }

   void ShowResultsByKeyAscending(void) const {
      std::vector<TKEY> vv(GetKeys());
      std::sort(vv.begin(), vv.end());
      ShowResultsBySortedKey(vv);
   }

   void ShowResultsByKeyDescending(void) const {
      std::vector<TKEY> vv(GetKeys());
      std::reverse(vv.begin(), vv.end());
      ShowResultsBySortedKey(vv);
   }

   void ShowResults(void) const {
      std::cout << GetHerald() << std::endl;
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it;
      std::vector<std::pair<int, SampleData<TKEY, TDATA> > > indexToSort(PrepareListOfItemsSortedByCount());

      for (size_t i = 0; i < indexToSort.size(); ++i) {
         if (!m_itemSeparator.empty()) std::cout << m_itemSeparator << std::endl;
         std::cout << "item " << i << "    (total=" << indexToSort[i].first << ")" << "  " << m_keyName << " = " << indexToSort[i].second.m_key << std::endl;
         it = m_tree.find(indexToSort[i].second.m_key);

         if (m_nmax > 0) {
            for (size_t k = 0; k < (*it).second.m_sampleData.size(); ++k) {
               if (!m_valueSeparator.empty()) std::cout << m_valueSeparator << std::endl;
               std::cout << (*it).second.m_sampleData[k] << std::endl;
            }
            if (m_itemSeparator.empty()) std::cout << std::endl;
         }
      }

      ShowTableOfKeysVersusCount();
   }

   void SetTitle(const std::string& s) {
      m_title = s;
   }

   void SetFooter(const std::string& s) {
      m_footer = s;
   }

   void SetKeyLabel(const std::string& s) { m_keyName = s; }

   void AppendTitle(const std::string s) { m_title += " " + s; }

   void SetMaxItemStore(const int n) { m_nmax = n; }
   void SetItemSeparator(const std::string& s) { m_itemSeparator = s; }
   void SetValueSeparator(const std::string& s) { m_valueSeparator = s; }
   bool HasKey(const TKEY& key) const { return (m_tree.find(key) != m_tree.end()); }

   std::string GetItemSeparator(void) const { return m_itemSeparator; }
   std::string GetValueSeparator(void) const { return m_valueSeparator; }
   std::string GetTitle(void) const { return m_title; }

   StoreResults operator= (const StoreResults& sr) {
      m_nmax = sr.m_nmax;
      m_tree = sr.m_tree;
      m_itemSeparator = sr.m_itemSeparator;
      m_valueSeparator = sr.m_valueSeparator;
      m_notes = sr.m_notes;
      return *this;
   }

   void clear(void) {
      m_tree.clear();
      m_notes.clear();
      m_herald.clear();
      m_title.clear();
      m_itemSeparator.clear();
      m_valueSeparator.clear();
   }

   size_t DeleteIfCountLessThan(const size_t n) {
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::iterator it;
      size_t count = 0;
      for (it = m_tree.begin(); it != m_tree.end();) {
         if ((*it).second.m_countPerKey < n) {
            it = m_tree.erase(it);
            ++count;
         }
         else {
            ++it;
         }
      }
      return count;
   }
   long DeleteIfCountGreaterThan(const long n) {
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::iterator it;
      long count = 0;
      for (it = m_tree.begin(); it != m_tree.end();) {
         if ((*it).second.m_countPerKey > (size_t)(n)) {
            it = m_tree.erase(it);
            ++count;
         }
         else {
            ++it;
         }
      }
      return count;
   }

   TKEY GetMaxKey() const {
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it =
         std::max_element(m_tree.begin(), m_tree.end());
      return (*it).first;
   }
   TKEY GetMinKey() const {
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it =
         std::min_element(m_tree.begin(), m_tree.end());
      return (*it).first;
   }

   std::pair<TKEY,TDATA> GetFirstItem( const TKEY& key) const {
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it;
      it = m_tree.find(key);
      if (it == m_tree.end())
         return std::make_pair(TKEY(), TDATA());
      return std::make_pair( (*it).first, (*it).second.m_sampleData[0]);
   }

   void ShowTableOfKeysVersusCount() const {
      std::vector<std::pair<int, SampleData<TKEY, TDATA> > > v(PrepareListOfItemsSortedByCount());
      if (!v.empty()) std::cout << m_title << "\n" << "item   count   " << m_keyName << std::endl;
      for (size_t i = 0; i<v.size(); ++i) {
         std::cout << i << "      " << v[i].first << "       " << v[i].second.m_key << std::endl;
      }

   }

private:

   std::vector<std::pair<int, SampleData<TKEY, TDATA> > > PrepareListOfItemsSortedByCount(void) const {
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it;
      std::vector<std::pair<int, SampleData<TKEY, TDATA> > > indexToSort;

      for (it = m_tree.begin(); it != m_tree.end(); ++it) {
         indexToSort.push_back(std::make_pair((*it).second.m_countPerKey, (*it).second));
      }

      std::sort(indexToSort.begin(), indexToSort.end(), std::greater<std::pair<int, SampleData<TKEY, TDATA> > >());
      return indexToSort;
   }

   void ShowResultsBySortedKey(const std::vector<TKEY>& keylist) const {
      std::cout << GetHerald() << std::endl;
      typename std::map<TKEY, SampleData<TKEY, TDATA> >::const_iterator it;

      for (size_t i = 0; i < keylist.size(); ++i) {
         if (!m_itemSeparator.empty()) std::cout << m_itemSeparator << std::endl;
         it = m_tree.find(keylist[i]);
         std::cout << "item " << i << "  " << m_keyName << "= " << (*it).first << "  count=" << (*it).second.GetCount() << std::endl;

         if (m_nmax > 0) {
            for (size_t k = 0; k < (*it).second.m_sampleData.size(); ++k) {
               if (!m_valueSeparator.empty()) std::cout << m_valueSeparator << std::endl;
               std::cout << (*it).second.m_sampleData[k] << std::endl;
            }
            if (m_itemSeparator.empty()) std::cout << std::endl;
         }
      }
      ShowTableOfKeysVersusCount();
      std::cout << m_footer << std::endl;
   }

private:
   size_t m_nmax;
   std::map<TKEY, SampleData<TKEY, TDATA> > m_tree;
   std::string m_itemSeparator;
   std::string m_valueSeparator;
   std::string m_herald;
   std::string m_title;
   std::string m_footer;
   std::string m_notes;
   std::string m_keyName;
   int m_creationTime;
};

#endif
