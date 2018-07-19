#ifndef CREATEFILENAME_H
#define CREATEFILENAME_H

#include <string>

class CreateFileName
{
public:
   static std::string Create( const std::string& prefix, const std::string& extension );
};


#endif
