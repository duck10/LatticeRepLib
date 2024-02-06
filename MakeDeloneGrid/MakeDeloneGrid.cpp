// MakeDeloneGrid.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>

#include "MakeDeloneGrid.h"
#include "S6.h"

std::string SVGIncludeInAGroup(const int x, const int y, const std::string& s) {
   std::ostringstream o;
   o << "<g transform=\"translate(" << x << ", " << y << ")\">\n";
   o << s;
   o << "</g>\n";
   return o.str();
}

static const double rectangleTranslation = 100;

std::string  MakeANumericRow() {
   std::ostringstream o;
   int cell = 0;
   o << SVGIncludeInAGroup(rectangleTranslation * cell++, 2, BasicDeloneRectangle().MakeSVG());
   o << SVGIncludeInAGroup(rectangleTranslation * cell++, 2, Digit_1().MakeSVG());
   o << SVGIncludeInAGroup(rectangleTranslation * cell++, 2, Digit_2().MakeSVG());
   o << SVGIncludeInAGroup(rectangleTranslation * cell++, 2, Digit_3().MakeSVG());
   o << SVGIncludeInAGroup(rectangleTranslation * cell++, 2, Digit_4().MakeSVG());
   o << SVGIncludeInAGroup(rectangleTranslation * cell++, 2, Digit_5().MakeSVG());

   return SVGIncludeInAGroup(10, 10, o.str());
}

int main()
{
   std::cout << MakeANumericRow() << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
