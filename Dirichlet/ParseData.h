#ifndef PARSEDATA_H
#define PARSEDATA_H

#include <string>

class ParseData {
public:
   std::string m_label;
   std::string m_dataTypeToRead;
   void* m_dataLocation;
   ParseData(const std::string& label, const std::string& dataType, void* dataLocation)
      : m_label(label)
      , m_dataTypeToRead(dataType)
      , m_dataLocation(dataLocation)
   {
   };
};

#endif // PARSEDATA_H