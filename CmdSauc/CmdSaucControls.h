#ifndef CMD_SAUC_CONTROLS_H
#define CMD_SAUC_CONTROLS_H

#include "BaseControlVariables.h"
#include "BlockUtils.h"
#include "InputHandler.h"
#include "LRL_StringTools.h"
#include "DistanceTypes.h"
#include "StringMatcher.h"

#include <atomic>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Default constants
static constexpr double DEFAULT_RANGEA = 1.0;
static constexpr double DEFAULT_RANGEB = 1.0;
static constexpr double DEFAULT_RANGEC = 1.0;
static constexpr double DEFAULT_RANGEALPHA = 1.0;
static constexpr double DEFAULT_RANGEBETA = 1.0;
static constexpr double DEFAULT_RANGEGAMMA = 1.0;
static const std::string DEFAULT_METRIC = "DC7UNSRT";
static constexpr int DEFAULT_SAUCMETRIC = 7;
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

static const SearchDomainInfo DEFAULT_SEARCH_DOMAIN_INFO = searchDomains[0];
static const SearchDomain DEFAULT_SEARCH_DOMAIN_TYPE = DEFAULT_SEARCH_DOMAIN_INFO.type;

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

static const int DEFAULT_METRIC_TYPE_INDEX = 7;
static const MetricType DEFAULT_METRIC_TYPE = metricTypes[DEFAULT_METRIC_TYPE_INDEX-1].type;


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
      os << "S6 (CS6Dist)";
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
             metricType = HandleMetric(LRL_StringTools::strToupper(value));
          });

       InputHandler::registerHandler("METRIC", 0.2,
          [this](const BaseControlVariables&, const std::string& value) {
             metricType = HandleMetric(LRL_StringTools::strToupper(value));
          });

       InputHandler::registerHandler("SAUCMETRIC", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             const std::string upper = LRL_StringTools::strToupper(value);
             metricType = HandleMetric(LRL_StringTools::strToupper(value));
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

       InputHandler::registerHandler("SPHERERADIUS", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             validateSphereRadius(value);
             saucSphereRange = std::stod(value);
          });
       InputHandler::registerHandler("RADIUS", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             validateSphereRadius(value);
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

       InputHandler::registerHandler("DOMAIN", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             const std::string upper = LRL_StringTools::strToupper(value);
             searchDomain = HandleSearchDomain(upper);
          });

       InputHandler::registerHandler("HITS", 0.4,
          [this](const BaseControlVariables&, const std::string& value) {
             const std::string upper = LRL_StringTools::strToupper(value);
             hits = validateHits(upper);
          });
    }
    // Accessor methods

    std::string getState() const {
       std::ostringstream oss;
       oss
          << ";Web run: " << ((this->getHasWebInput()) ? "true" : "false") << "\n"
          << ";Blockstart: " << blockstart << "\n"
          << ";Blocksize: " << blocksize << "\n"
          << "; metricType: " << metricType << "\n"
          << ";  RangeA: " << RangeA << "\n"
          << ";  RangeB: " << RangeB << "\n"
          << ";  RangeC: " << RangeC << "\n"
          << ";  RangeAlpha: " << RangeAlpha << "\n"
          << ";  RangeBeta: " << RangeBeta << "\n"
          << ";  RangeGamma: " << RangeGamma << "\n"
          << ";Sphere Range: " << saucSphereRange << "\n"
          << ";  searchDomain: " << searchDomain << "\n";
          oss << "\n";
       return oss.str();
    }

    int getSearchDomain() const {
       for (const auto& search : searchDomains) {
          if (search.type == searchDomain) {
             return search.index; // Return the corresponding index
          }
       }
       // Default case: Use DEFAULT_SEARCH_DOMAIN
       return DEFAULT_SEARCH_DOMAIN_INFO.index; // Should never reach here unless DEFAULT_SEARCH_DOMAIN_TYPE is missing
    }

    int getSearchMetric() const {
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

   MetricType MetricTypeFromName(const std::string& st) const {
      for (const auto& m : metricTypes) {
         if (st == m.name) return m.type;
      }
      return DEFAULT_METRIC_TYPE;
   }

   MetricType MetricTypeFromNum(const std::string& st) const {
      for (const auto& m : metricTypes) {
         if (st == std::to_string(m.index)) return m.type;
      }
      return DEFAULT_METRIC_TYPE;
   }

   MetricType HandleMetric(const std::string& st) const {
      constexpr double matchThreshold = 0.5;
      VectorStringMatcher matcherName(matchThreshold);

      // Populate the name matcher with valid metric names
      for (const auto& m : metricTypes) {
         matcherName.addString(m.name);
      }

      // Step 1: Attempt to match by name first
      std::string resultName;
      const bool bName = matcherName.findBestMatch(st, resultName);
      if (bName && matcherName.getTheta(st, resultName) < matchThreshold) {
         return MetricTypeFromName(resultName);
      }

      // Step 2: If name matching fails, attempt to parse the input as a number
      try {
         int num = std::stoi(st); // Attempt to convert string to integer
         if (num >= 1 && num <= 7) { // Validate range [1, 7]
            return MetricTypeFromNum(std::to_string(num)); // Match exact number
         }
      }
      catch (const std::invalid_argument&) {
         // Input is not a valid number; ignore and fallback to default
      }
      catch (const std::out_of_range&) {
         // Input number is out of range; ignore and fallback to default
      }

      // Step 3: Fallback to default metric type
      std::cout << "ERROR: using default Metric " << DEFAULT_METRIC_TYPE << std::endl;;
      return DEFAULT_METRIC_TYPE;
   }

   SearchDomain SearchDomainFromName(const std::string& st) const {
      for (const auto& s : searchDomains) {
         if (st == s.name) return s.type;
      }
      std::cout << "ERROR: using default Domain " << DEFAULT_SEARCH_DOMAIN_TYPE << std::endl;;
      return DEFAULT_SEARCH_DOMAIN_TYPE;
   }

   SearchDomain HandleSearchDomain(const std::string& st) const {
   constexpr double matchThreshold = 0.3;
      VectorStringMatcher matcher(matchThreshold);
      std::vector<SearchDomainInfo> domains;
      for (const auto& sd : searchDomains) {
         matcher.addString(sd.name);
      }

      std::string result;
      const bool bName = matcher.findBestMatch(st, result);
      return SearchDomainFromName(result);
   }

   int validateHits(const std::string& st) const {
      const std::string upper = LRL_StringTools::strToupper(st);
      try {
         const int h = std::stoi(st);
         if (h > 0 && h < 1000) {
            return h;
         }
         else {
            throw std::invalid_argument("");
         }
      }
      catch (const std::invalid_argument&) {
         std::cout << ";invalid number of hits requested, using " << DEFAULT_HITS << std::endl;
         return DEFAULT_HITS;
      }
   }

   double validateSphereRadius(const std::string& st) const {
      const std::string upper = LRL_StringTools::strToupper(st);
      try {
         const double radius = std::stod(upper);
         if (radius > 0.1 && radius < 10.0) {
            return radius;
         }
         else {
            throw std::invalid_argument("");
         }
      }

      catch (const std::invalid_argument&) {
         std::cout << "; Invalid sphere radius, using default= "
            << DEFAULT_SPHERE_RADIUS_PERCENT << " percent" << std::endl;
         return DEFAULT_SPHERE_RADIUS_PERCENT;
      }
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

   static constexpr double DEFAULT_SPHERE_RADIUS_PERCENT = 50.0;
   double saucSphereRange = DEFAULT_SPHERE_RADIUS_PERCENT;

   SearchDomain searchDomain = DEFAULT_SEARCH_DOMAIN_TYPE;
   MetricType metricType = MetricType::DC7UNSRT;

   static constexpr int DEFAULT_HITS = 50;
   int hits = DEFAULT_HITS;
};


#endif // CMD_SAUC_CONTROLS_H
