#pragma warning (disable: 4189) // Visual Studio -- local variable is initialized but not referenced
#pragma warning (disable: 4996) // Visual Studio -- localtime may not be safe
#include "C3.h"
#include "DeloneFitResults.h"
#include "DeloneTypeList.h"
#include "GrimmerTree.h"
#include "S6.h"
#include "S6BoundaryTransforms.h"
#include "Sella.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


// Define the static maps first
const std::map<std::string, double> GrimmerChains::baseDeloneMap = {
   {"A1", 0.0}, {"M1A", 0.0}, {"M1B", 0.0}, {"M2A", 0.0}, {"M4", 0.0},
   {"O1A", 0.0}, {"O1B", 0.0}, {"O2", 0.0}, {"O4", 0.0}, {"O5", 0.0},
   {"H1", 0.0}, {"H3", 0.0}, {"T1", 0.0}, {"T2", 0.0}, {"T5", 0.0},
   {"C1", 0.0}, {"C3", 0.0}, {"C5", 0.0}, {"H4", 0.0}
};

const std::map<std::string, double> GrimmerChains::baseBravaisMap = {
   {"aP", 0.0}, {"mP", 0.0}, {"mC", 0.0}, {"oF", 0.0}, {"oI", 0.0},
   {"oC", 0.0}, {"oP", 0.0}, {"hR", 0.0}, {"tI", 0.0}, {"tP", 0.0},
   {"cI", 0.0}, {"cF", 0.0}, {"cP", 0.0}, {"hP", 0.0}
};

// Define Delone types first (they're used in Bravais types)
const DeloneTypeForGrimmer GrimmerChains::m_dtype_a1("A1", "aP", baseDeloneMap.at("A1"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_m1a("M1A", "mC", baseDeloneMap.at("M1A"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_m1b("M1B", "mC", baseDeloneMap.at("M1B"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_m2a("M2A", "mC", baseDeloneMap.at("M2A"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_m4("M4", "mP", baseDeloneMap.at("M4"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o1a("O1A", "oF", baseDeloneMap.at("O1A"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o1b("O1B", "oI", baseDeloneMap.at("O1B"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o2("O2", "oI", baseDeloneMap.at("O2"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o4("O4", "oS", baseDeloneMap.at("O4"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_o5("O5", "oP", baseDeloneMap.at("O5"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_r1("H1", "hR", baseDeloneMap.at("H1"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_r3("H3", "hR", baseDeloneMap.at("H3"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_t1("T1", "tI", baseDeloneMap.at("T1"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_t2("T2", "tI", baseDeloneMap.at("T2"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_t5("T5", "tP", baseDeloneMap.at("T5"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_c1("C1", "cI", baseDeloneMap.at("C1"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_c3("C3", "cF", baseDeloneMap.at("C3"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_c5("C5", "cP", baseDeloneMap.at("C5"));
const DeloneTypeForGrimmer GrimmerChains::m_dtype_h4("H4", "hP", baseDeloneMap.at("H4"));

// Now define Bravais types
const BravaisTypeForGrimmer GrimmerChains::m_btype_aP("aP", { m_dtype_a1 }, baseBravaisMap.at("aP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_mP("mP", { m_dtype_m4 }, baseBravaisMap.at("mP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_mC("mC", { m_dtype_m1a, m_dtype_m1b, m_dtype_m2a }, baseBravaisMap.at("mC"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_oF("oF", { m_dtype_o1a }, baseBravaisMap.at("oF"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_oI("oI", { m_dtype_o1b, m_dtype_o2 }, baseBravaisMap.at("oI"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_oC("oC", { m_dtype_o4 }, baseBravaisMap.at("oC"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_oP("oP", { m_dtype_o5 }, baseBravaisMap.at("oP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_hR("hR", { m_dtype_r1, m_dtype_r3 }, baseBravaisMap.at("hR"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_tI("tI", { m_dtype_t1, m_dtype_t2 }, baseBravaisMap.at("tI"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_tP("tP", { m_dtype_t5 }, baseBravaisMap.at("tP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_cI("cI", { m_dtype_c1 }, baseBravaisMap.at("cI"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_cF("cF", { m_dtype_c3 }, baseBravaisMap.at("cF"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_cP("cP", { m_dtype_c5 }, baseBravaisMap.at("cP"));
const BravaisTypeForGrimmer GrimmerChains::m_btype_hP("hP", { m_dtype_h4 }, baseBravaisMap.at("hP"));

// Finally, define the chainTemplates static variable
const std::vector<std::vector<const BravaisTypeForGrimmer*>> GrimmerChains::chainTemplates = {
   {&m_btype_aP, &m_btype_mP, &m_btype_oP, &m_btype_tP, &m_btype_cP},
   {&m_btype_aP, &m_btype_mP, &m_btype_oC, &m_btype_tP, &m_btype_cP},
   {&m_btype_aP, &m_btype_mP, &m_btype_oC, &m_btype_hP},
   {&m_btype_aP, &m_btype_mC, &m_btype_oC, &m_btype_tP, &m_btype_cP},
   {&m_btype_aP, &m_btype_mC, &m_btype_oC, &m_btype_hP},

   {&m_btype_aP, &m_btype_mC, &m_btype_oI, &m_btype_tI, &m_btype_cI},
   {&m_btype_aP, &m_btype_mC, &m_btype_oI, &m_btype_tI, &m_btype_cF},
   {&m_btype_aP, &m_btype_mC, &m_btype_oF, &m_btype_tI, &m_btype_cI},
   {&m_btype_aP, &m_btype_mC, &m_btype_oF, &m_btype_tI, &m_btype_cF},
   {&m_btype_aP, &m_btype_mC, &m_btype_hR, &m_btype_cP},

   {&m_btype_aP, &m_btype_mC, &m_btype_hR, &m_btype_cI},
   {&m_btype_aP, &m_btype_mC, &m_btype_hR, &m_btype_cF}
};


namespace SVGUtil {
   // Create a basic SVG element with attributes
   std::string createElement(const std::string& tag,
      const std::map<std::string, std::string>& attributes,
      const std::string& content = "") {
      std::stringstream ss;
      ss << "  <" << tag;

      for (const auto& attr : attributes) {
         ss << " " << attr.first << "=\"" << attr.second << "\"";
      }

      if (content.empty()) {
         ss << " />\n";
      }
      else {
         ss << ">" << content << "</" << tag << ">\n";
      }

      return ss.str();
   }

   // Create text element with common attributes
   std::string createText(const std::string& text, int x, int y,
      const std::string& anchor = "start",
      const std::string& fontFamily = "Arial",
      int fontSize = 12,
      const std::string& fontWeight = "normal",
      const std::string& fill = "#000000") {
      std::map<std::string, std::string> attributes = {
          {"x", std::to_string(x)},
          {"y", std::to_string(y)},
          {"text-anchor", anchor},
          {"font-family", fontFamily},
          {"font-size", std::to_string(fontSize)},
          {"font-weight", fontWeight},
          {"fill", fill}
      };

      return createElement("text", attributes, text);
   }

   // Create line element
   std::string createLine(int x1, int y1, int x2, int y2,
      const std::string& stroke = "#000000",
      int strokeWidth = 1,
      const std::string& dashArray = "") {
      std::map<std::string, std::string> attributes = {
          {"x1", std::to_string(x1)},
          {"y1", std::to_string(y1)},
          {"x2", std::to_string(x2)},
          {"y2", std::to_string(y2)},
          {"stroke", stroke},
          {"stroke-width", std::to_string(strokeWidth)}
      };

      if (!dashArray.empty()) {
         attributes["stroke-dasharray"] = dashArray;
      }

      return createElement("line", attributes);
   }

   // Create rectangle element
   std::string createRect(int x, int y, int width, int height,
      const std::string& fill = "#FFFFFF",
      const std::string& stroke = "#000000",
      int strokeWidth = 1) {
      std::map<std::string, std::string> attributes = {
          {"x", std::to_string(x)},
          {"y", std::to_string(y)},
          {"width", std::to_string(width)},
          {"height", std::to_string(height)},
          {"fill", fill},
          {"stroke", stroke},
          {"stroke-width", std::to_string(strokeWidth)}
      };

      return createElement("rect", attributes);
   }

   // Create circle element
   std::string createCircle(int cx, int cy, int r,
      const std::string& fill = "#000000") {
      std::map<std::string, std::string> attributes = {
          {"cx", std::to_string(cx)},
          {"cy", std::to_string(cy)},
          {"r", std::to_string(r)},
          {"fill", fill}
      };

      return createElement("circle", attributes);
   }

   // Create polyline element
   std::string createPolyline(const std::string& points,
      const std::string& stroke = "#000000",
      int strokeWidth = 1,
      const std::string& fill = "none") {
      std::map<std::string, std::string> attributes = {
          {"points", points},
          {"stroke", stroke},
          {"stroke-width", std::to_string(strokeWidth)},
          {"fill", fill}
      };

      return createElement("polyline", attributes);
   }

   // Wrap content in an SVG group with translation
   std::string wrapInGroup(const std::string& content, const std::string& id,
      int translateX, int translateY) {
      std::stringstream ss;
      ss << "  <g id=\"" << id << "\" transform=\"translate(" << translateX
         << "," << translateY << ")\">\n";
      ss << content;
      ss << "  </g>\n";
      return ss.str();
   }
}

DeloneTypeForGrimmer::DeloneTypeForGrimmer(const std::string& deloneName, const std::string bravaisType, const double fit/*=DBL_MAX*/)
   :m_DeloneName(deloneName)
   , m_BravaisType(bravaisType)
   , m_fit(fit)
{
}


// FitData struct for storing type and fit value pairs
struct FitData {
   std::string typeName;
   double fitValue;

   // Default constructor required for std::vector
   FitData() : typeName(""), fitValue(0.0) {}

   // Constructor with parameters
   FitData(const std::string& name, double fit) : typeName(name), fitValue(fit) {}

   // For sorting from smallest to highest fit value
   bool operator<(const FitData& other) const {
      return fitValue < other.fitValue;
   }
};

// Class to collect and process fit data
class FitDataCollector {
private:
   std::map<std::string, double> deloneFits;
   std::map<std::string, double> pearsonFits;
   std::map<std::string, std::vector<std::pair<std::string, double>>> pearsonToDeloneMap;
   std::vector<FitData> sortedDeloneFits;
   std::vector<FitData> sortedPearsonFits;
   double maxDeloneFit = 0.0;
   double maxPearsonFit = 0.0;

public:
   // Extract fit data from Grimmer chains
   void collectFitData(const std::vector<OneGrimmerChain>& chains) {
      for (const auto& chain : chains) {
         for (const auto& bravaisType : chain.GetChain()) {
            // Store Bravais (Pearson) type fits
            const std::string bravaisName = bravaisType.GetBravaisType();
            const double bravaisFit = bravaisType.GetFit();

            if (pearsonFits.find(bravaisName) == pearsonFits.end() ||
               pearsonFits[bravaisName] > bravaisFit) {
               pearsonFits[bravaisName] = bravaisFit;
            }

            // Store Delone type fits and build Pearson-to-Delone mapping
            for (const auto& deloneType : bravaisType.GetDeloneTypes()) {
               const std::string deloneName = deloneType.GetDeloneName();
               const double deloneFit = deloneType.GetFit();

               if (deloneFits.find(deloneName) == deloneFits.end() ||
                  deloneFits[deloneName] > deloneFit) {
                  deloneFits[deloneName] = deloneFit;

                  // Add this Delone type to the corresponding Pearson type
                  bool found = false;
                  for (auto& pair : pearsonToDeloneMap[bravaisName]) {
                     if (pair.first == deloneName) {
                        pair.second = deloneFit;
                        found = true;
                        break;
                     }
                  }
                  if (!found) {
                     pearsonToDeloneMap[bravaisName].push_back(std::make_pair(deloneName, deloneFit));
                  }
               }
            }
         }
      }

      // Convert to sortable vectors
      for (const auto& pair : deloneFits) {
         sortedDeloneFits.push_back(FitData(pair.first, pair.second));
      }

      for (const auto& pair : pearsonFits) {
         sortedPearsonFits.push_back(FitData(pair.first, pair.second));
      }

      // Sort from smallest to highest fit value
      std::sort(sortedDeloneFits.begin(), sortedDeloneFits.end());
      std::sort(sortedPearsonFits.begin(), sortedPearsonFits.end());

      // Find max fit value for scaling
      if (!sortedDeloneFits.empty())
         maxDeloneFit = sortedDeloneFits.back().fitValue;
      if (!sortedPearsonFits.empty())
         maxPearsonFit = sortedPearsonFits.back().fitValue;

      // Make sure max values are not zero
      if (maxDeloneFit <= 0.0) maxDeloneFit = 1.0;
      if (maxPearsonFit <= 0.0) maxPearsonFit = 1.0;
   }

   const std::vector<FitData>& getDeloneFits() const { return sortedDeloneFits; }
   const std::vector<FitData>& getPearsonFits() const { return sortedPearsonFits; }
   double getMaxDeloneFit() const { return maxDeloneFit; }
   double getMaxPearsonFit() const { return maxPearsonFit; }

   std::vector<std::pair<std::string, double>> getSortedPearsonList() const {
      std::vector<std::pair<std::string, double>> sortedPearsonList;
      for (const auto& pair : pearsonFits) {
         sortedPearsonList.push_back(std::make_pair(pair.first, pair.second));
      }
      std::sort(sortedPearsonList.begin(), sortedPearsonList.end(),
         [](const auto& a, const auto& b) { return a.second < b.second; });
      return sortedPearsonList;
   }

   const std::map<std::string, std::vector<std::pair<std::string, double>>>& getPearsonToDeloneMap() const {
      return pearsonToDeloneMap;
   }
};



// Helper for drawing the Y-axis with better distributed grid values
void CreateBetterYAxisGridValues(std::vector<double>& yAxisValues, double maxValue) {
   // Clear any existing values
   yAxisValues.clear();

   // Always start with 0
   yAxisValues.push_back(0.0);

   // Add 0.3 instead of 0.5 for better placement
   yAxisValues.push_back(0.3);

   // Add 1.0
   yAxisValues.push_back(1.0);

   // Add 2.0 and 5.0 (skip 3.0 and 4.0 to avoid overcrowding)
   yAxisValues.push_back(2.0);
   yAxisValues.push_back(5.0);

   // Add 10.0, 20.0, 50.0
   yAxisValues.push_back(10.0);
   yAxisValues.push_back(20.0);
   yAxisValues.push_back(50.0);

   // Add 100.0, 200.0, 500.0 if within range
   if (maxValue > 100.0) {
      yAxisValues.push_back(100.0);
   }
   if (maxValue > 200.0) {
      yAxisValues.push_back(200.0);
   }
   if (maxValue > 500.0) {
      yAxisValues.push_back(500.0);
   }

   // Always add max value if it's higher than our predefined values
   if (maxValue > 500.0) {
      yAxisValues.push_back(maxValue);
   }
}

// Helper function to compress Y-axis values for better visualization
double CompressYAxisValue(const double value, const double minValue, const double maxValue, const double powerFactor = 0.3) {
   // Handle out-of-range values
   if (value <= minValue) return 0.0;
   if (value >= maxValue) return 1.0;

   if (maxValue < minValue) return CompressYAxisValue(value, maxValue, minValue, powerFactor);

   // Normalize the value to 0-1 range
   const double normalizedValue = (value - minValue) / (maxValue - minValue);

   // Apply power function for smooth compression
   // This gives more space to smaller values
   return std::pow(normalizedValue, powerFactor);
}


// 1. Helper function to wrap content in an SVG group with translation
std::string wrapInGroup(const std::string& content, const std::string& id, int translateX, int translateY) {
   std::stringstream ss;
   ss << "  <g id=\"" << id << "\" transform=\"translate(" << translateX << "," << translateY << ")\">\n";
   ss << content;
   ss << "  </g>\n";
   return ss.str();
}

// Function to generate main title
std::string createTitle(const std::string& mainTitle, const std::string& subTitle, int width) {
   std::stringstream ss;
   ss << "  <text x=\"" << width / 2 << "\" y=\"20\" text-anchor=\"middle\" font-family=\"Arial\" "
      << "font-size=\"20\" font-weight=\"bold\">" << mainTitle << "</text>\n";
   // Reduce the y value from 45 to 40 to move subtitle closer to main title
   ss << "  <text x=\"" << width / 2 << "\" y=\"40\" text-anchor=\"middle\" font-family=\"Arial\" "
      << "font-size=\"16\">" << subTitle << "</text>\n";
   return ss.str();
}

std::map<std::string, double> MapOfBravaisFits::CreateMapOFBravaisFits(std::vector<DeloneFitResults>& fits) {
   std::map<std::string, double> valuemap;
   for (const auto& fit : fits) {
      const double fitvalue = fit.GetRawFit();
      const std::string bravaisName = fit.GetDeloneType();
      if (valuemap.find(bravaisName) == valuemap.end()) {
         valuemap[bravaisName] = fitvalue;
      }
      else
      {
         if (valuemap[bravaisName] > fitvalue)
         {
            valuemap[bravaisName] = fitvalue;
         }
      }
   }
   m_valuemap = valuemap;
   return valuemap;
}

std::map<std::string, double> MapOFDeloneFits::CreateMapOFDeloneFits(std::vector<DeloneFitResults>& fits) {
   std::map<std::string, double> valuemap;
   for (const auto& fit : fits) {
      const std::string name = fit.GetGeneralType();
      const double fitvalue = fit.GetRawFit();
      m_valuemap[name] = fitvalue;
   }
   return m_valuemap;
}

void GrimmerChains::CreateGrimmerChains(MapOFDeloneFits& theDeloneMap, MapOfBravaisFits& theBravaisMap) {
   m_deloneFits = theDeloneMap;
   m_bravaisFits = theBravaisMap;
   initializeChains();
}

void GrimmerChains::CheckAllGrimmerChains()
{
   m_hasChainFailure = false;
   std::set<std::string> uniqueFailures; //lca save these up!!!!!!!!!!!!!!
   for (auto& chain : m_GrimmerChains) {
      const GrimmerChainFailure fail = chain.CheckOneGrimmerChain();
      if (!fail.empty()) {
         std::stringstream ss;
         ss << fail << std::endl;
         uniqueFailures.insert(ss.str());
         m_GrimmerFailures.emplace_back(fail);
         m_hasChainFailure = true;
         if (const bool debug = false == true)
         {
            std::cout << fail << std::endl;
         }
      }
   }
}

// Function to create a plot with common elements and styling
std::string createPlot(const std::string& title,
   const std::vector<FitData>& data,
   const double maxFit,
   const int plotWidth,
   const int plotHeight,
   const std::string& lineColor,
   const double s6Norm) {
   std::stringstream ss;

   // Plot border and background
   ss << SVGUtil::createRect(0, 0, plotWidth, plotHeight, "#FFFFFF", "#333333");

   // Plot title
   ss << SVGUtil::createText(title, plotWidth / 2, -20, "middle", "Arial", 16, "bold");

   // Y-axis label
   ss << "  <text x=\"-60\" y=\"" << plotHeight / 2 << "\" transform=\"rotate(-90,-60,"
      << plotHeight / 2 << ")\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"14\">"
      << "Fit Value</text>\n";

   // X-axis label
   ss << SVGUtil::createText("Types (sorted by fit value)", plotWidth / 2, plotHeight + 35,
      "middle", "Arial", 14);

   // Draw Y-axis and gridlines
   std::vector<double> yAxisValues;
   CreateBetterYAxisGridValues(yAxisValues, maxFit);

   for (double value : yAxisValues) {
      double relativeY = CompressYAxisValue(value, 0.0, maxFit);
      int yPos = plotHeight - relativeY * plotHeight;

      // Grid line
      ss << SVGUtil::createLine(0, yPos, plotWidth, yPos, "#DDDDDD");

      // Y-axis label
      std::stringstream valueStr;
      if (value == 0.0) {
         valueStr << "0";
      }
      else if (value < 1.5) {
         valueStr << std::fixed << std::setprecision(3) << value;
      }
      else if (value < 10.0) {
         valueStr << std::fixed << std::setprecision(2) << value;
      }
      else if (value < 100.0) {
         valueStr << std::fixed << std::setprecision(1) << value;
      }
      else {
         valueStr << std::fixed << std::setprecision(0) << value;
      }

      ss << SVGUtil::createText(valueStr.str(), -5, yPos + 5, "end", "Arial", 12);
   }

   // Draw reference lines for S6 norm
   // 0.1% reference line
   const double pointOnePctValue = 0.001 * s6Norm;
   double relativeY_pointOnePct = CompressYAxisValue(pointOnePctValue, 0.0, maxFit);
   int yPos_pointOnePct = plotHeight - relativeY_pointOnePct * plotHeight;

   ss << SVGUtil::createLine(0, yPos_pointOnePct, plotWidth, yPos_pointOnePct,
      "#00CC00", 1, "5,3");
   ss << SVGUtil::createText("0.1% (" + std::to_string(pointOnePctValue) + ")",
      plotWidth - 5, yPos_pointOnePct - 5, "end", "Arial", 12,
      "normal", "#00CC00");

   // 1% reference line
   const double onePctValue = 0.01 * s6Norm;
   double relativeY_1pct = CompressYAxisValue(onePctValue, 0.0, maxFit);
   int yPos_1pct = plotHeight - relativeY_1pct * plotHeight;

   ss << SVGUtil::createLine(0, yPos_1pct, plotWidth, yPos_1pct,
      "#FF6600", 1, "5,3");
   ss << SVGUtil::createText("1% (" + std::to_string(onePctValue) + ")",
      plotWidth - 5, yPos_1pct - 5, "end", "Arial", 12,
      "normal", "#FF6600");

   // 5% reference line
   const double fivePctValue = 0.05 * s6Norm;
   double relativeY_5pct = CompressYAxisValue(fivePctValue, 0.0, maxFit);
   int yPos_5pct = plotHeight - relativeY_5pct * plotHeight;

   ss << SVGUtil::createLine(0, yPos_5pct, plotWidth, yPos_5pct,
      "#CC0000", 1, "5,3");
   ss << SVGUtil::createText("5% (" + std::to_string(fivePctValue) + ")",
      plotWidth - 5, yPos_5pct - 5, "end", "Arial", 12,
      "normal", "#CC0000");

   // Draw data points
   if (!data.empty()) {
      const int pointRadius = 4;

      // Determine X spacing
      double xSpacing = static_cast<double>(plotWidth - 20) / (data.size() - 1);
      if (data.size() == 1) xSpacing = 0; // Special case for single point

      // Build points string for polyline
      std::stringstream pointsStr;
      for (size_t i = 0; i < data.size(); i++) {
         const auto& item = data[i];
         double xPos = 10 + xSpacing * i;
         double relativeY = CompressYAxisValue(item.fitValue, 0.0, maxFit);
         double yPos = plotHeight - relativeY * plotHeight;

         pointsStr << xPos << "," << yPos << " ";
      }

      // Draw the polyline connecting all points
      ss << SVGUtil::createPolyline(pointsStr.str(), lineColor, 2);

      // Draw the points, labels, and guidelines
      for (size_t i = 0; i < data.size(); i++) {
         const auto& item = data[i];
         double xPos = 10 + xSpacing * i;
         double relativeY = CompressYAxisValue(item.fitValue, 0.0, maxFit);
         double yPos = plotHeight - relativeY * plotHeight;

         // Draw the point
         ss << SVGUtil::createCircle(xPos, yPos, pointRadius, lineColor);

         // Draw value tooltip on hover
         ss << "  <title>" << item.typeName << ": "
            << std::fixed
            << (item.fitValue < 1.5 ? std::setprecision(3) : std::setprecision(2))
            << item.fitValue << "</title>\n";

         // Draw type labels
         ss << "  <text x=\"" << xPos << "\" y=\"" << (plotHeight + 15)
            << "\" text-anchor=\"middle\" font-family=\"Arial\" font-size=\"10\" "
            << "transform=\"rotate(45," << xPos << "," << (plotHeight + 15) << ")\">"
            << item.typeName << "</text>\n";

         // Draw vertical guide line for each point
         ss << SVGUtil::createLine(xPos, yPos, xPos, plotHeight, "#DDDDDD", 1, "2,2");
      }
   }
   else {
      // No data message
      ss << SVGUtil::createText("No data available", plotWidth / 2, plotHeight / 2,
         "middle", "Arial", 14, "normal", "#999999");
   }

   return ss.str();
}

// Specialized function for Delone plot
std::string createDelonePlot(const std::vector<FitData>& deloneFits,
   const double maxDeloneFit,
   const int plotWidth,
   const int plotHeight,
   const std::string& lineColor,
   const double s6Norm) {
   return createPlot("Delone Types (Sorted by Fit Value)",
      deloneFits,
      maxDeloneFit,
      plotWidth,
      plotHeight,
      lineColor,
      s6Norm);
}

// Specialized function for Pearson plot
std::string createPearsonPlot(const std::vector<FitData>& pearsonFits,
   const double maxPearsonFit,
   const int plotWidth,
   const int plotHeight,
   const std::string& lineColor,
   const double s6Norm) {
   return createPlot("Pearson Types (Sorted by Fit Value)",
      pearsonFits,
      maxPearsonFit,
      plotWidth,
      plotHeight,
      lineColor,
      s6Norm);
}


DeloneFitResults GrimmerChains::Remediation(const std::string& bravaisName, const double initialFit) {

   static const std::vector<std::shared_ptr<GenerateDeloneBase> > sptypes =
      GenerateDeloneBase().Select(bravaisName);

   const DeloneFitResults dfrInit = Sella::SellaFitXXXXXX(sptypes[0], m_s6);
   auto dfr = dfrInit;

   for (size_t i = 1; i <= 6; ++i)
   {
      // apply a boundary transform
      const MatS6 matrix = S6BoundaryTransforms::generateOneBoundaryTransform(i);
      const S6 mod = matrix * m_s6;
      const auto dfrTemp = Sella::SellaFitXXXXXX(sptypes[0], matrix * m_s6);
      if (dfrTemp.GetRawFit() < dfr.GetRawFit()) {
         dfr = dfrTemp;
      }
   }
   return dfr;
}

GrimmerChains GrimmerChains::ReplaceRemediation(const DeloneFitResults& newFit) const {
   GrimmerChains out(*this);
   const std::string bravaisName = newFit.GetBravaisType();
   const std::string deloneName = newFit.GetGeneralType();
   const double newFitValue = newFit.GetRawFit();

   for (auto& oneChain : out.m_GrimmerChains) {
      std::vector<BravaisTypeForGrimmer> chain = oneChain.GetChain();
      for (auto& btype : chain) {
         if (btype.GetBravaisType() == bravaisName) {
            // Update the Bravais fit
            btype.SetFit(newFitValue);

            // Update the associated Delone type fits
            std::vector<DeloneTypeForGrimmer> deloneTypes = btype.GetDeloneTypes();
            for (auto& deloneType : deloneTypes) {
               if (deloneType.GetDeloneName() == deloneName) {
                  deloneType.SetFit(newFitValue);
               }
            }

            // Update the chain with modified Delone types
            btype = BravaisTypeForGrimmer(bravaisName, deloneTypes, newFitValue);
         }
      }

      // Update the chain in the OneGrimmerChain
      oneChain = OneGrimmerChain(chain);
   }

   return out;
}

void GrimmerChains::initializeChains() {
   m_GrimmerChains.clear();
   for (const auto& chainTemplate : chainTemplates) {
      std::vector<BravaisTypeForGrimmer> chain;
      for (const auto* btype : chainTemplate) {
         // Create a new BravaisTypeForGrimmer with the template's Delone types
         BravaisTypeForGrimmer newBType(
            btype->GetBravaisType(),
            btype->GetDeloneTypes(),
            m_bravaisFits[btype->GetBravaisType()]);

         // Update the Delone fits from m_deloneFits
         newBType.UpdateDeloneFits(m_deloneFits.GetValueMap());

         chain.push_back(newBType);
      }
      m_GrimmerChains.emplace_back(chain);
   }
}

// BravaisTypeForGrimmer constructor implementation
BravaisTypeForGrimmer::BravaisTypeForGrimmer(const std::string& name,
   const std::vector<DeloneTypeForGrimmer>& vdeloneTypes,
   const double fit)
   : m_name(name)
   , m_vtypes(vdeloneTypes)
   , m_fit(fit)
{
}

// BravaisTypeForGrimmer::UpdateDeloneFits implementation
void BravaisTypeForGrimmer::UpdateDeloneFits(const std::map<std::string, double>& deloneFits) {
   for (auto& deloneType : m_vtypes) {
      const std::string& deloneName = deloneType.GetDeloneName();
      if (deloneFits.find(deloneName) != deloneFits.end()) {
         deloneType.SetFit(deloneFits.at(deloneName));
      }
   }
}

// OneGrimmerChain::CheckOneGrimmerChain implementation
GrimmerChainFailure OneGrimmerChain::CheckOneGrimmerChain()
{
   m_fail = false;
   double prevFit = m_chain[0].GetFit();
   GrimmerChainFailure fails;
   for (size_t i = 0; i < m_chain.size(); ++i) {
      const double fit = m_chain[i].GetFit();
      if (fit < (1.0 - 1.0E-8) * prevFit) {
         const double test = fit - prevFit;
         m_fail = true;
         fails.AddBravaistypeAndFit(m_chain[i - 2].GetBravaisType(), m_chain[i - 2].GetFit());
         fails.AddBravaistypeAndFit(m_chain[i - 1].GetBravaisType(), m_chain[i - 1].GetFit());
         fails.AddBravaistypeAndFit(m_chain[i].GetBravaisType(), m_chain[i].GetFit());
         if (const bool debug = false == true)
         {
            std::cout << fails << std::endl;
         }
         m_failures = fails;
      }
      prevFit = fit;
   }
   return fails;
}


// Function to create a single column of the fit list
std::string createFitListColumn(
   const std::vector<std::pair<std::string, double>>& items,
   const std::map<std::string, std::vector<std::pair<std::string, double>>>& itemDetails) {

   std::stringstream ss;
   const int titleHeight = 20;
   const int itemHeight = 18;
   const int spaceBetweenGroups = 5;
   int yPos = 0;

   for (const auto& item : items) {
      const std::string& typeName = item.first;
      const double typeValue = item.second;

      // Draw type with fit value
      ss << "  <text x=\"0\" y=\"" << yPos << "\" font-family=\"Arial\" "
         << "font-size=\"14\" font-weight=\"bold\">"
         << typeName << " (" << std::fixed
         << (typeValue < 1.5 ? std::setprecision(3) : std::setprecision(2))
         << typeValue << ")</text>\n";

      yPos += titleHeight;

      // Draw associated details
      for (const auto& detail : itemDetails.at(typeName)) {
         ss << "  <text x=\"20\" y=\"" << yPos << "\" font-family=\"Arial\" "
            << "font-size=\"12\">"
            << detail.first << ": " << std::fixed
            << (detail.second < 1.5 ? std::setprecision(3) : std::setprecision(2))
            << detail.second << "</text>\n";

         yPos += itemHeight;
      }

      yPos += spaceBetweenGroups;
   }

   return ss.str();
}

// Function to create the double-column fit list
std::string createFitList(
   const std::vector<std::pair<std::string, double>>& sortedPearsonList,
   const std::map<std::string, std::vector<std::pair<std::string, double>>>& pearsonToDeloneMap) {

   std::stringstream ss;

   // Title for fit list
   ss << "  <text x=\"0\" y=\"-20\" font-family=\"Arial\" font-size=\"16\" font-weight=\"bold\">"
      << "Delone Types by Pearson Type</text>\n";

   // Count and divide items between columns
   int totalItems = 0;
   for (const auto& pearsonPair : sortedPearsonList) {
      totalItems += 1 + pearsonToDeloneMap.at(pearsonPair.first).size() + 1;
   }

   int itemsPerColumn = totalItems / 2 + (totalItems % 2);

   // Prepare left column items
   std::vector<std::pair<std::string, double>> leftColumnItems;
   int currentCount = 0;

   for (const auto& item : sortedPearsonList) {
      const int itemSize = 1 + pearsonToDeloneMap.at(item.first).size() + 1;

      if (currentCount + itemSize <= itemsPerColumn) {
         leftColumnItems.push_back(item);
         currentCount += itemSize;
      }
      else {
         break;
      }
   }

   // Prepare right column items
   std::vector<std::pair<std::string, double>> rightColumnItems;
   for (size_t i = leftColumnItems.size(); i < sortedPearsonList.size(); ++i) {
      rightColumnItems.push_back(sortedPearsonList[i]);
   }

   // Create left column
   std::string leftColumn = createFitListColumn(leftColumnItems, pearsonToDeloneMap);

   // Create right column
   std::string rightColumn = createFitListColumn(rightColumnItems, pearsonToDeloneMap);

   // Wrap columns in groups with proper translation
   ss << wrapInGroup(leftColumn, "leftFitColumn", 0, 0);
   ss << wrapInGroup(rightColumn, "rightFitColumn", 160, 0);

   return ss.str();
}

// Function to create legend
std::string createLegend(const std::string& lineColorDelone, const std::string& lineColorPearson) {
   std::stringstream ss;

   // Legend title
   ss << "  <text x=\"0\" y=\"0\" font-family=\"Arial\" font-size=\"14\" font-weight=\"bold\">"
      << "Legend</text>\n";

   // Reference lines explanation
   ss << "  <text x=\"0\" y=\"25\" font-family=\"Arial\" font-size=\"12\">"
      << "Reference Lines:</text>\n";

   // 0.1% reference line
   ss << "  <line x1=\"20\" y1=\"40\" x2=\"50\" y2=\"40\" stroke=\"#00CC00\" "
      << "stroke-width=\"1\" stroke-dasharray=\"5,3\" />\n";
   ss << "  <text x=\"60\" y=\"45\" font-family=\"Arial\" font-size=\"12\">"
      << "0.1% of S6 norm</text>\n";

   // 1% reference line
   ss << "  <line x1=\"20\" y1=\"60\" x2=\"50\" y2=\"60\" stroke=\"#FF6600\" "
      << "stroke-width=\"1\" stroke-dasharray=\"5,3\" />\n";
   ss << "  <text x=\"60\" y=\"65\" font-family=\"Arial\" font-size=\"12\">"
      << "1% of S6 norm</text>\n";

   // 5% reference line
   ss << "  <line x1=\"20\" y1=\"80\" x2=\"50\" y2=\"80\" stroke=\"#CC0000\" "
      << "stroke-width=\"1\" stroke-dasharray=\"5,3\" />\n";
   ss << "  <text x=\"60\" y=\"85\" font-family=\"Arial\" font-size=\"12\">"
      << "5% of S6 norm</text>\n";

   // Plot line colors
   ss << "  <text x=\"0\" y=\"105\" font-family=\"Arial\" font-size=\"12\">"
      << "Plot Lines:</text>\n";

   // Delone types line
   ss << "  <line x1=\"20\" y1=\"120\" x2=\"50\" y2=\"120\" stroke=\""
      << lineColorDelone << "\" stroke-width=\"2\" />\n";
   ss << "  <text x=\"60\" y=\"125\" font-family=\"Arial\" font-size=\"12\">"
      << "Delone types</text>\n";

   // Pearson types line
   ss << "  <line x1=\"20\" y1=\"140\" x2=\"50\" y2=\"140\" stroke=\""
      << lineColorPearson << "\" stroke-width=\"2\" />\n";
   ss << "  <text x=\"60\" y=\"145\" font-family=\"Arial\" font-size=\"12\">"
      << "Pearson types</text>\n";

   // Additional explanation
   ss << "  <text x=\"0\" y=\"165\" font-family=\"Arial\" font-size=\"12\" font-style=\"italic\">"
      << "Lower fit values indicate better matches</text>\n";
   ss << "  <text x=\"0\" y=\"185\" font-family=\"Arial\" font-size=\"12\" fill=\"#666666\">"
      << "Y-axis uses logarithmic compression</text>\n";
   ss << "  <text x=\"0\" y=\"205\" font-family=\"Arial\" font-size=\"12\" fill=\"#666666\">"
      << "to emphasize small values</text>\n";

   return ss.str();
}

std::string GrimmerGetCurrentDate() {
   std::time_t t = std::time(nullptr);
   std::tm* now = std::localtime(&t);
   char buffer[128];
   std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", now);
   return std::string(buffer);
}

// Function to create footer
std::string createFooter(int width) {
   std::stringstream ss;
   ss << "  <text x=\"" << width / 2 << "\" y=\"15\" text-anchor=\"middle\" "
      << "font-family=\"Arial\" font-size=\"12\" fill=\"#666666\">"
      << "Created by SELLA - " << GrimmerGetCurrentDate() << "</text>\n";
   return ss.str();
}

// Main function to generate the SVG
// In the GenerateSortedFitPlots method:
std::string GrimmerChains::GenerateSortedFitPlots(const int width, const int height, const std::string& st) const {
   // Colors
   const std::string lineColorDelone = "#56B4E9";  // Sky blue
   const std::string lineColorPearson = "#E69F00";  // Orange

   // Component dimensions and positions - with improved spacing
   const int titleAreaHeight = 60;     // Increased space for title and subtitle
   const int plotWidth = (width - 400) * 0.75;
   const int plotHeight = (height - 300) / 2;  // Reduce plot height to ensure room for everything
   const int plotX = 100;
   const int topPlotY = titleAreaHeight + 20;  // Move top plot down to avoid overlap with title
   const int bottomPlotY = topPlotY + plotHeight + 120; // More space between plots
   const int listX = plotX + plotWidth + 40;
   // Adjust the legend and footer positions
   const int legendY = bottomPlotY + 60;  // Move legend up (was 220, now 60)
   const int footerY = height - 40;       // Move footer up slightly (was 20, now 40)


   // Calculate reduced S6 norm
   const double s6Norm = m_s6.norm();

   // Collect and process data
   FitDataCollector dataCollector;
   dataCollector.collectFitData(m_GrimmerChains);

   // Build SVG components
   std::string titleGroup = createTitle("SELLA Lattice Fit Analysis", st, width);

   std::string delonePlotGroup = createDelonePlot(
      dataCollector.getDeloneFits(),
      dataCollector.getMaxDeloneFit(),
      plotWidth, plotHeight,
      lineColorDelone,
      s6Norm);

   std::string pearsonPlotGroup = createPearsonPlot(
      dataCollector.getPearsonFits(),
      dataCollector.getMaxPearsonFit(),
      plotWidth, plotHeight,
      lineColorPearson,
      s6Norm);

   std::string fitListGroup = createFitList(
      dataCollector.getSortedPearsonList(),
      dataCollector.getPearsonToDeloneMap());

   std::string legendGroup = createLegend(lineColorDelone, lineColorPearson);

   std::string footerGroup = createFooter(width);

   // Wrap each component in a group with proper positioning
   std::string titleGroupPositioned = wrapInGroup(titleGroup, "title", 0, 0);
   std::string delonePlotGroupPositioned = wrapInGroup(delonePlotGroup, "delonePlot", plotX, topPlotY);
   std::string pearsonPlotGroupPositioned = wrapInGroup(pearsonPlotGroup, "pearsonPlot", plotX, bottomPlotY);
   std::string fitListGroupPositioned = wrapInGroup(fitListGroup, "fitList", listX, topPlotY);
   std::string legendGroupPositioned = wrapInGroup(legendGroup, "legend", listX, legendY);
   std::string footerGroupPositioned = wrapInGroup(footerGroup, "footer", -60, footerY);

   // Assemble the SVG
   std::stringstream ss;
   ss << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";

   // Recommend a minimum height of 800px for comfortable spacing
   const int recommendedHeight = std::max(height, 800);

   ss << "<svg width=\"" << width << "\" height=\"" << recommendedHeight
      << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

   // Add all positioned components
   ss << titleGroupPositioned;
   ss << delonePlotGroupPositioned;
   ss << pearsonPlotGroupPositioned;
   ss << fitListGroupPositioned;
   ss << legendGroupPositioned;
   ss << footerGroupPositioned;

   // Close SVG
   ss << "</svg>";

   return ss.str();
}

