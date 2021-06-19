#ifndef PARSEDATA_H
#define PARSEDATA_H

#include <string>

class ParseData {
public:
   ParseData(const std::string& label, const std::string& dataType, void* dataLocation)
      : m_label(label)
      , m_dataTypeToRead(dataType)
      , m_dataLocation(dataLocation)
   {
   };
   std::string GetLabel() const { return m_label; }
   std::string GetDataType() const { return m_dataTypeToRead; }
   void* GetLocation() const { return m_dataLocation; }
private:
   std::string m_label;
   std::string m_dataTypeToRead;
   void* m_dataLocation;
};

#endif // PARSEDATA_H