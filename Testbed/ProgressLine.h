#ifndef PROCESSLINE_H

#define PROCESSLINE_H

#include <string>

class ProgressLine {
public:
   ProgressLine();
   void AddStep();

private:
   std::string m_latticeName;
   std::vector<ProgressPointBase> m_line;
};
#endif  //  PROCESSLINE_H