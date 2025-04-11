#ifndef CMD_SAUC_CONTROLS_H
#define CMD_SAUC_CONTROLS_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"
#include "DistanceTypes.h"

#include <atomic>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Default constants
constexpr double DEFAULT_RANGEA = 1.0;
constexpr double DEFAULT_RANGEB = 1.0;
constexpr double DEFAULT_RANGEC = 1.0;
constexpr double DEFAULT_RANGEALPHA = 1.0;
constexpr double DEFAULT_RANGEBETA = 1.0;
constexpr double DEFAULT_RANGEGAMMA = 1.0;
const std::string DEFAULT_SPACE = "DC7UNSRT";
constexpr int DEFAULT_SAUCMETRIC = 7;
/*#define DEFAULT_BLOCKSIZE 100*/

enum class SearchDomain {
   Nearest, // Search for the nearest point
   Sphere,  // Search within a sphere around the point
   Range    // Search within a range of x, y, z
};

struct SearchDomainInfo {
   SearchDomain type;  // Enum type
   int index;          // Index value
   std::string name;   // Text name for fuzzy matching
};

static const std::vector<SearchDomainInfo> searchDomains = {
    {SearchDomain::Nearest, 1, "NEAREST"},
    {SearchDomain::Sphere, 2, "SPHERE"},
    {SearchDomain::Range, 3, "RANGE"}
};

enum class MetricType {
   L1,      // Manhattan distance
   L2,      // Euclidean distance
   G6,    // Custom or other metric spaces
   V7,
   D7,
   S6,
   DC7UNSRT,
   DC7U
};

struct MetricTypeInfo {
   MetricType type;    // Enum type
   int index;          // Index value
   std::string name;   // Text name for fuzzy matching
};

static const std::vector<MetricTypeInfo> metricTypes = {
    { MetricType::L1,      1, "L1"},
    { MetricType::L2,      2, "L2"},
    { MetricType::G6,      3, "G6"},
    { MetricType::V7,      4, "V7" },
    { MetricType::D7,      5, "D7" },
    { MetricType::S6,      6, "S6" },
    { MetricType::DC7UNSRT,7, "DC7UNSRT" },
    { MetricType::DC7U,    7, "DC7U" }
};

const SearchDomain DEFAULT_SEARCH_DOMAIN_TYPE = searchDomains[0].type;
const MetricType DEFAULT_METRIC_TYPE = metricTypes[7].type;
const int DEFAULT_METRIC_TYPE_INDEX = 7;


// Overload operator<< for MetricType
inline std::ostream& operator<<(std::ostream& os, const MetricType& metric) {
   switch (metric) {
   case MetricType::L1:
      os << "L1 (Manhattan Distance)";
      break;
   case MetricType::L2:
      os << "L2 (Euclidean Distance)";
      break;
   case MetricType::G6:
      os << "G6 (NCDist)";
      break;
   case MetricType::V7:
      os << "V7";
      break;
   case MetricType::D7:
      os << "D7";
      break;
   case MetricType::S6:
      os << "S6 (CS6Dist";
      break;
   case MetricType::DC7UNSRT:
      os << "DC7UNSRT";
      break;
   case MetricType::DC7U:
      os << "DC7U";
      break;
   default:
      os << "Unknown Metric Type";
   }
   return os;
}


// Overload operator<< for SearchDomain
inline std::ostream& operator<<(std::ostream& os, const SearchDomain& domain) {
   switch (domain) {
   case SearchDomain::Nearest:
      os << "Nearest";
      break;
   case SearchDomain::Sphere:
      os << "Sphere";
      break;
   case SearchDomain::Range:
      os << "Range";
      break;
   default:
      os << "Unknown Search Domain";
   }
   return os;
}

// Fuzzy search function
//template <typename T>
//T fuzzySearch(const std::vector<T>& list, const std::string& input) {
//   double bestScore = std::numeric_limits<double>::max();
//   T bestMatch = list[0]; // Default match
//
//   for (const auto& item : list) {
//      double score = StringMatcher::calculateSimilarity(input, item.name); // Replace with your fuzzy match logic
//      if (score < bestScore) {
//         bestScore = score;
//         bestMatch = item;
//      }
//   }
//   return bestMatch;
//}

//int main() {
//   // Example fuzzy matching for SearchDomainInfo
//   std::string inputDomain = "Spher";
//   SearchDomainInfo matchedDomain = fuzzySearch(searchDomains, inputDomain);
//   std::cout << "Best match for Search Domain: " << matchedDomain.name
//      << " (Index: " << matchedDomain.index << ")\n";
//
//   // Example fuzzy matching for MetricTypeInfo
//   std::string inputMetric = "L2";
//   MetricTypeInfo matchedMetric = fuzzySearch(metricTypes, inputMetric);
//   std::cout << "Best match for Metric Type: " << matchedMetric.name
//      << " (Index: " << matchedMetric.index << ")\n";
//
//   return 0;
//}


// Define full CmdSaucControls class here
class CmdSaucControls : public BaseControlVariables {
public:
   friend std::ostream& operator<< (std::ostream& os, const CmdSaucControls& fc) {
      os << fc.getState();
      return os;
   }
    
    CmdSaucControls() {
       InputHandler::registerHandler("BLOCKSIZE", 0.35,
          [this](const BaseControlVariables&, const std::string& value) {
             blocksize = std::stoul(value);
          });

       InputHandler::registerHandler("BLOCKSTART", 0.35,
          [this](const BaseControlVariables&, const std::string& value) {
             blockstart = std::stoul(value);
          });

       InputHandler::registerHandler("SPACE", 0.2,
          [this](const BaseControlVariables&, const std::string& value) {
             SPACE = HandleMetric(LRL_StringTools::strToupper(value));
          });

       InputHandler::registerHandler("METRIC", 0.2,
          [this](const BaseControlVariables&, const std::string& value) {
             SPACE = HandleMetric(LRL_StringTools::strToupper(value));
          });

       InputHandler::registerHandler("RANGEA", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             RangeA = std::stod(value);
          });

       InputHandler::registerHandler("RANGEB", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             RangeB = std::stod(value);
          });

       InputHandler::registerHandler("RANGEC", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             RangeC = std::stod(value);
          });

       InputHandler::registerHandler("RANGEALPHA", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             RangeAlpha = std::stod(value);
          });

       InputHandler::registerHandler("RANGEBETA", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             RangeBeta = std::stod(value);
          });

       InputHandler::registerHandler("RANGEGAMMA", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             RangeGamma = std::stod(value);
          });

       InputHandler::registerHandler("RANGESPHERE", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             saucSphereRange = std::stod(value);
          });

       InputHandler::registerHandler("NEAREST", 0.2,
          [this](const BaseControlVariables&, const std::string& value) {
             searchDomain = HandleSearchDomain("NEAREST");
          });

       InputHandler::registerHandler("SPHERE", 0.2,
          [this](const BaseControlVariables&, const std::string& value) {
             searchDomain = HandleSearchDomain("SPHERE");
          });

       InputHandler::registerHandler("RANGE", 0.2,
          [this](const BaseControlVariables&, const std::string& value) {
             searchDomain = HandleSearchDomain("RANGE");
          });

       InputHandler::registerHandler("SAUCMETRIC", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             const std::string upper = LRL_StringTools::strToupper(value);
             SPACE = HandleMetric(LRL_StringTools::strToupper(value));
          });

       InputHandler::registerHandler("SEARCHDOMAIN", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             const std::string upper = LRL_StringTools::strToupper(value);
             searchDomain = HandleSearchDomain(upper);
          });

       InputHandler::registerHandler("DOMAIN", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             const std::string upper = LRL_StringTools::strToupper(value);
             searchDomain = HandleSearchDomain(upper);
          });

       if ((!saucSphere) && (!saucRange) && (!saucNearest))
          saucNearest = true;
    }
    // Accessor methods

    std::string getState() const {
       std::ostringstream oss;
       oss
          << ";Web run: " << ((this->getHasWebInput()) ? "true" : "false") << "\n"
          << ";Blockstart: " << blockstart << "\n"
          << ";Blocksize: " << blocksize << "\n"
          << "; Sauc Metric: " << SAUCMETRIC << "\n"
          << "; SPACE: " << SPACE << "\n"
          << "; metricType: " << metricType << "\n"
          << ";  RangeA: " << RangeA << "\n"
          << ";  RangeB: " << RangeB << "\n"
          << ";  RangeC: " << RangeC << "\n"
          << ";  RangeAlpha: " << RangeAlpha << "\n"
          << ";  RangeBeta: " << RangeBeta << "\n"
          << ";  RangeGamma: " << RangeGamma << "\n"
          << ";Sphere Range: " << saucSphereRange << "\n"
          << ";  searchDomain: " << searchDomain << "\n"
          << ";  saucNearest: " << ((saucNearest) ? "true" : "false") << "\n"
          << ";  saucSphere: " << ((saucSphere) ? "true" : "false") << "\n"
          << ";  saucRange: " << ((saucRange) ? "true" : "false") << "\n";

          oss << "\n\n";
       return oss.str();
    }

    int getsimilarity() const {
       for (const auto& search : searchDomains) {
          if (search.type == searchDomain) {
             return search.index; // Return the corresponding index
          }
       }
       // Default case: Use DEFAULT_SEARCH_DOMAIN
       for (const auto& search : searchDomains) {
          if (search.type == DEFAULT_SEARCH_DOMAIN_TYPE) {
             return search.index;
          }
       }
       return searchDomains[0].index; // Should never reach here unless DEFAULT_SEARCH_DOMAIN_TYPE is missing
    }

    int getalgorithm() const {
       for (const auto& metric : metricTypes) {
          if (metric.type == metricType) {
             return metric.index; // Return the corresponding index
          }
       }
       return DEFAULT_METRIC_TYPE_INDEX; // Should never reach here unless DEFAULT_METRIC_TYPE is missing
    }

    double getRangeA() const {
       return RangeA;
    }

    double getRangeB() const {
       return RangeB;
    }

    double getRangeC() const {
       return RangeC;
    }

    double getRangeAlpha() const {
       return RangeAlpha;
    }

    double getRangeBeta() const {
       return RangeBeta;
    }

    double getRangeGamma() const {
       return RangeGamma;
    }

    double getsaucSphereRange() const {
       return saucSphereRange;
    }

private:
   std::string HandleMetric(const std::string& st) {
      static const std::vector<std::string> validNumbers = { "1", "2", "3", "4", "5", "6", "7", "7" };
      static const std::vector<std::string> validNames = {
          "L1", "L2", "G6", "V7", "D7", "S6", "DC7UNSRT", "DC7U"
      };

      // Check if input is a valid number
      try {
         int num = std::stoi(st); // Convert to integer
         if (num > 0 && num <= validNames.size()) {
            SAUCMETRIC = num;              // Set SAUCMETRIC
            SPACE = validNames[num - 1];   // Set SPACE
            metricType = static_cast<MetricType>(num - 1); // Set metricType
            return validNames[num - 1];    // Return name corresponding to number
         }
      }
      catch (const std::invalid_argument&) {
         // Ignore and proceed to name matching
      }

      // Check if input matches a valid name
      for (size_t i = 0; i < validNames.size(); ++i) {
         if (st == validNames[i]) {
            try {
               SAUCMETRIC = std::stoi(validNumbers[i]); // Update SAUCMETRIC
               SPACE = validNames[i];                  // Set SPACE
               metricType = static_cast<MetricType>(i); // Set metricType
               return validNames[i];                   // Return the valid name
            }
            catch (const std::invalid_argument&) {
               return "ERROR: Invalid number in validNumbers.";
            }
         }
      }

      // Fallback to defaults if no match is found
      SAUCMETRIC = DEFAULT_SAUCMETRIC;
      SPACE = DEFAULT_SPACE;
      metricType = DEFAULT_METRIC_TYPE; // Set metricType to the default
      return DEFAULT_SPACE;
   }


   SearchDomain HandleSearchDomain(const std::string& st) {
      // Iterate through `searchDomains` to find a match
      for (const auto& search : searchDomains) {
         if (st == search.name) {
            saucNearest = search.type == SearchDomain::Nearest;
            saucSphere = search.type == SearchDomain::Sphere;
            saucRange = search.type == SearchDomain::Range;
            return search.type;         // Return the matching search domain
         }
      }

      // Fallback to the default search domain
      return DEFAULT_SEARCH_DOMAIN_TYPE;
   }

   MetricType HandleMetricType(const std::string& st) const {
      for (const auto& search : metricTypes) {
         if (st == search.name) {
            return search.type;
         }
      }
      return DEFAULT_METRIC_TYPE;
   }

private:
   size_t blocksize = 0;
   size_t blockstart = 0;

   double RangeA = DEFAULT_RANGEA;
   double RangeB = DEFAULT_RANGEB;
   double RangeC = DEFAULT_RANGEC;;
   double RangeAlpha = DEFAULT_RANGEALPHA;
   double RangeBeta = DEFAULT_RANGEBETA;
   double RangeGamma = DEFAULT_RANGEGAMMA;

   double saucSphereRange = 50.0;
   int SAUCMETRIC = DEFAULT_SAUCMETRIC;
   std::string SPACE = std::string(DEFAULT_SPACE);   

   SearchDomain searchDomain = DEFAULT_SEARCH_DOMAIN_TYPE;
   MetricType metricType = MetricType::DC7UNSRT;

   bool saucNearest = true;
   bool saucSphere = false;
   bool saucRange = false;
};


#endif // CMD_SAUC_CONTROLS_H
