
#include <iostream>
#include <vector>

#include "G6.h"
#include "LRL_Cell_Degrees.h"

#include <sstream>

std::string  doGruber() {
   std::ostringstream o;
   for (size_t i = 0; i < 10; ++i)
   {
      ++i;
      std::cout << i << std::endl;
   }
      o << "G6 " << LRL_Cell_Degrees{G6(std::vector<double>{ 4., 16., 16., 16., 3., 4.   })} << std::endl;// Niggli reduced
      o << "G6 " << LRL_Cell_Degrees{G6(std::vector<double>{ 4., 16., 16., 16., 1., 4.   })} << std::endl;
      o << "G6 " << LRL_Cell_Degrees{G6(std::vector<double>{ 4., 16., 16., -16., -1., -3.})} << std::endl;
      o << "G6 " << LRL_Cell_Degrees{G6(std::vector<double>{ 4., 16., 16., -15., -1., -4.})} << std::endl;
      o << "G6 " << LRL_Cell_Degrees{G6(std::vector<double>{ 4., 16., 16., -13., -3., -4.})} << std::endl;
   return o.str();
}

int main() {

   std::string line1("Hello World");
   std::string line2("This is the end!");

   std::cout << line1 << std::endl;

   std::cout << line2 << std::endl;

   std::cout << doGruber() << std::endl;
}
