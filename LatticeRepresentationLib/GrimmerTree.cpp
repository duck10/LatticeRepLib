#pragma warning (disable: 4189) // Visual Studio -- local variable is initialized but not referenced
#pragma warning (disable: 4996) // Visual Studio -- localtime may not be safe
#include "C3.h"
#include "DeloneFitResults.h"
#include "DeloneTypeList.h"
#include "GrimmerTree.h"
#include "LRL_Cell_Degrees.h"
#include "LRL_StringTools.h"
#include "LRL_ToString.h"
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

std::vector<double> CreateBetterYAxisGridValues(const double maxValue, const int plotHeight) {
   // Always include these standard reference values if they're below maxValue
   std::vector<double> standardValues = {
       0.0, 1.0, 10.0, 50.0, 100.0, 200.0, 500.0, 1000.0, 2000.0, 5000.0, 10000.0
   };

   std::vector<double> yAxisValues;

   // Keep all standard values that are below maxValue * 1.1
   for (double value : standardValues) {
      if (value <= maxValue * 1.1) {
         yAxisValues.push_back(value);
      }
   }

   // Add the maxValue if it's significantly higher than the last standard value
   if (!yAxisValues.empty() && maxValue > yAxisValues.back() * 1.2) {
      // Round up to a nice number
      double roundedMax = maxValue;
      if (maxValue > 10 && maxValue < 100) {
         roundedMax = std::ceil(maxValue / 10.0) * 10.0; // Round to nearest 10
      }
      else if (maxValue >= 100 && maxValue < 1000) {
         roundedMax = std::ceil(maxValue / 100.0) * 100.0; // Round to nearest 100
      }
      else if (maxValue >= 1000) {
         roundedMax = std::ceil(maxValue / 1000.0) * 1000.0; // Round to nearest 1000
      }

      yAxisValues.push_back(roundedMax);
   }

   return yAxisValues;
}



// 1. Helper function to wrap content in an SVG group with translation
std::string wrapInGroup(const std::string& content, const std::string& id, int translateX, int translateY) {
   std::stringstream ss;
   ss << "  <g id=\"" << id << "\" transform=\"translate(" << translateX << "," << translateY << ")\">\n";
   ss << content;
   ss << "  </g>\n";
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

// Create basic plot structure: border, title, axes labels
std::string createPlotStructure(const std::string& title,
   int plotWidth,
   int plotHeight) {
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

   return ss.str();
}

// Create y-axis and gridlines
std::string createYAxisAndGridlines(const std::vector<double>& yAxisValues,
   double maxFit,
   int plotWidth,
   int plotHeight) {
   std::stringstream ss;

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

   return ss.str();
}

// Create reference lines (0.1%, 1%, 5% of S6 norm)
std::string createReferenceLines(double s6Norm,
   double maxFit,
   int plotWidth,
   int plotHeight) {
   std::stringstream ss;

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

   if (yPos_1pct > 0.0)
   {
      ss << SVGUtil::createLine(0, yPos_1pct, plotWidth, yPos_1pct,
         "#FF6600", 1, "5,3");
      ss << SVGUtil::createText("1% (" + std::to_string(onePctValue) + ")",
         plotWidth - 5, yPos_1pct - 5, "end", "Arial", 12,
         "normal", "#FF6600");
   }

   // 5% reference line
   const double fivePctValue = 0.05 * s6Norm;
   double relativeY_5pct = CompressYAxisValue(fivePctValue, 0.0, maxFit);
   int yPos_5pct = plotHeight - relativeY_5pct * plotHeight;

   if (yPos_5pct > 0.0)
   {
      ss << SVGUtil::createLine(0, yPos_5pct, plotWidth, yPos_5pct,
         "#CC0000", 1, "5,3");
      ss << SVGUtil::createText("5% (" + std::to_string(fivePctValue) + ")",
         plotWidth - 5, yPos_5pct - 5, "end", "Arial", 12,
         "normal", "#CC0000");
   }

   return ss.str();
}

// Plot the data points and connecting lines
std::string plotDataPoints(const std::vector<FitData>& data,
   double maxFit,
   int plotWidth,
   int plotHeight,
   const std::string& lineColor) {
   std::stringstream ss;

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

// Main plotting function that calls the other functions
std::string createPlot(const std::string& title,
   const std::vector<FitData>& data,
   const double maxFit,
   const int plotWidth,
   const int plotHeight,
   const std::string& lineColor,
   const double s6Norm) {
   std::stringstream ss;

   // Create basic plot structure
   ss << createPlotStructure(title, plotWidth, plotHeight);

   // Create Y-axis and gridlines
   const std::vector<double> yAxisValues = CreateBetterYAxisGridValues(maxFit, plotHeight);
   ss << createYAxisAndGridlines(yAxisValues, maxFit, plotWidth, plotHeight);

   // Create reference lines
   ss << createReferenceLines(s6Norm, maxFit, plotWidth, plotHeight);

   // Plot the data points
   ss << plotDataPoints(data, maxFit, plotWidth, plotHeight, lineColor);

   return ss.str();
}


// Specialized function for Delone plot
std::string GrimmerChains::createDelonePlot(const std::vector<FitData>& deloneFits,
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
std::string GrimmerChains::createPearsonPlot(const std::vector<FitData>& pearsonFits,
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

// Implementation of createTitle method for GrimmerChains
std::string GrimmerChains::createTitle(const std::string& mainTitle, const std::string& subTitle, int width) {
   std::stringstream ss;
   ss << "  <text x=\"" << width / 2 << "\" y=\"20\" text-anchor=\"middle\" font-family=\"Arial\" "
      << "font-size=\"20\" font-weight=\"bold\">" << mainTitle << "</text>\n";
   ss << "  <text x=\"" << width / 2 << "\" y=\"40\" text-anchor=\"middle\" font-family=\"Arial\" "
      << "font-size=\"16\">" << subTitle << "</text>\n";
   return ss.str();
}

// Also need to fix the createFitListColumn function to ensure proper asterisk display
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

      // Draw type with fit value, marking invalid values
      ss << "  <text x=\"0\" y=\"" << yPos << "\" font-family=\"Arial\" "
         << "font-size=\"14\" font-weight=\"bold\">"
         << typeName << " (";

      // Explicitly check for DBL_MAX values and negative values
      if (typeValue >= DBL_MAX || typeValue < 0.0) {
         ss << "*";  // Use asterisk for invalid values
      }
      else {
         ss << std::fixed
            << (typeValue < 1.5 ? std::setprecision(3) : std::setprecision(2))
            << typeValue;
      }

      ss << ")</text>\n";

      yPos += titleHeight;

      // Draw associated details
      if (itemDetails.find(typeName) != itemDetails.end()) {
         for (const auto& detail : itemDetails.at(typeName)) {
            ss << "  <text x=\"20\" y=\"" << yPos << "\" font-family=\"Arial\" "
               << "font-size=\"12\">"
               << detail.first << ": ";

            if (detail.second >= DBL_MAX || detail.second < 0.0) {
               ss << "*";  // Use asterisk for invalid values
            }
            else {
               ss << std::fixed
                  << (detail.second < 1.5 ? std::setprecision(3) : std::setprecision(2))
                  << detail.second;
            }

            ss << "</text>\n";

            yPos += itemHeight;
         }
      }

      yPos += spaceBetweenGroups;
   }

   return ss.str();
}


// Function to create the double-column fit list
std::string GrimmerChains::createFitList(const std::vector<std::pair<std::string, double>>& sortedPearsonList,
   const std::map<std::string, std::vector<std::pair<std::string, double>>>& pearsonToDeloneMap) const
{
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
std::string GrimmerChains::createLegend(const std::string& lineColorDelone, const std::string& lineColorPearson) const {
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
std::string GrimmerChains::createFooter(int width) const {
   std::stringstream ss;
   ss << "  <text x=\"" << width / 3 << "\" y=\"15\" text-anchor=\"middle\" "
      << "font-family=\"Arial\" font-size=\"12\" fill=\"#666666\">"
      << "Created by SELLA - " << GrimmerGetCurrentDate() << "</text>\n";
   return ss.str();
}

// Main function to generate the SVG
// Modified GenerateSortedFitPlots method with a simpler approach
std::string GrimmerChains::GenerateSortedFitPlots(const int width, const int height, const std::string& st) {
   // Convert maps to vectors for sorting (including all values for the list display)
   std::vector<FitData> allDeloneFits;
   for (const auto& [name, value] : m_deloneTypeFits) {
      allDeloneFits.push_back(FitData(name, value));
   }

   std::vector<FitData> allPearsonFits;
   for (const auto& [name, value] : m_pearsonTypeFits) {
      allPearsonFits.push_back(FitData(name, value));
   }

   // Create filtered versions for plotting (excluding DBL_MAX values)
   std::vector<FitData> plotDeloneFits;
   for (const auto& fit : allDeloneFits) {
      if (fit.fitValue < DBL_MAX && fit.fitValue >= 0.0) {
         plotDeloneFits.push_back(fit);
      }
   }

   std::vector<FitData> plotPearsonFits;
   for (const auto& fit : allPearsonFits) {
      if (fit.fitValue < DBL_MAX && fit.fitValue >= 0.0) {
         plotPearsonFits.push_back(fit);
      }
   }

   // Sort all versions by fit value
   std::sort(allDeloneFits.begin(), allDeloneFits.end());
   std::sort(allPearsonFits.begin(), allPearsonFits.end());
   std::sort(plotDeloneFits.begin(), plotDeloneFits.end());
   std::sort(plotPearsonFits.begin(), plotPearsonFits.end());

   // Get Pearson list for fit list display
   std::vector<std::pair<std::string, double>> sortedPearsonList;
   for (const auto& fit : allPearsonFits) {
      sortedPearsonList.push_back(std::make_pair(fit.typeName, fit.fitValue));
   }

   // Calculate reduced S6 norm
   const double s6Norm = m_s6.norm();

   // Define layout parameters
   const std::string lineColorDelone = "#56B4E9";  // Sky blue
   const std::string lineColorPearson = "#E69F00";  // Orange

   const int titleAreaHeight = 60;
   const int plotWidth = (width - 400) * 0.75;
   const int plotHeight = (height - 300) / 2;
   const int plotX = 100;
   const int topPlotY = titleAreaHeight + 20;
   const int bottomPlotY = topPlotY + plotHeight + 120;
   const int listX = plotX + plotWidth + 40;
   const int legendY = bottomPlotY + 60;
   const int footerY = height - 40;

   // Find max values for scaling from the filtered plot data
   double maxDeloneFit = 1.0;  // Default to 1.0 if no data
   double maxPearsonFit = 1.0;

   if (!plotDeloneFits.empty()) {
      maxDeloneFit = plotDeloneFits.back().fitValue;
   }

   if (!plotPearsonFits.empty()) {
      maxPearsonFit = plotPearsonFits.back().fitValue;
   }


   // Use the filtered datasets for the plots
   std::string stCell = st;
   std::string upper = LRL_StringTools::strToupper(st);
   if (upper.find("RANDOM") != std::string::npos) {
      stCell += "   " + LRL_ToString(LRL_Cell_Degrees(m_s6));
   }
   // Now build the SVG components
   const std::string& titleGroup = createTitle("SELLA Lattice Fit Analysis", stCell, width);
   const std::string& delonePlotGroup = this->createDelonePlot(
      plotDeloneFits,
      maxDeloneFit,
      plotWidth, plotHeight,
      lineColorDelone,
      s6Norm);

   const std::string& pearsonPlotGroup = createPearsonPlot(
      plotPearsonFits,
      maxPearsonFit,
      plotWidth, plotHeight,
      lineColorPearson,
      s6Norm);

   // Use the complete dataset for the fit list (with asterisks for DBL_MAX values)
   const std::string& fitListGroup = createFitList(
      sortedPearsonList,
      m_pearsonToDeloneMap);

   const std::string& legendGroup = createLegend(lineColorDelone, lineColorPearson);

   const std::string& footerGroup = createFooter(width);

   // Wrap each component in a group with proper positioning
   const std::string& titleGroupPositioned = wrapInGroup(titleGroup, "title", 0, 0);
   const std::string& delonePlotGroupPositioned = wrapInGroup(delonePlotGroup, "delonePlot", plotX, topPlotY);
   const std::string& pearsonPlotGroupPositioned = wrapInGroup(pearsonPlotGroup, "pearsonPlot", plotX, bottomPlotY);
   const std::string& fitListGroupPositioned = wrapInGroup(fitListGroup, "fitList", listX, topPlotY);
   const std::string& legendGroupPositioned = wrapInGroup(legendGroup, "legend", listX, legendY);
   const std::string& footerGroupPositioned = wrapInGroup(footerGroup, "footer", 0, footerY);

   // Assemble the SVG
   std::stringstream ss;
   ss << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
   ss << "<svg width=\"" << width << "\" height=\"" << height
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

// New method to update simplified data structures
void GrimmerChains::UpdateFits(const std::vector<DeloneFitResults>& fits) {
   // Clear existing data
   m_deloneTypeFits.clear();
   m_pearsonTypeFits.clear();
   m_pearsonToDeloneMap.clear();

   // Process each fit result
   for (const auto& fit : fits) {
      const std::string deloneName = fit.GetGeneralType();
      const std::string pearsonName = fit.GetBravaisType();
      const double fitValue = fit.GetRawFit();

      // For DBL_MAX values, we want to store them and mark them with asterisks later
      // Instead of skipping, we store them as-is

      // Update Delone type fit (keep the best fit)
      if (m_deloneTypeFits.find(deloneName) == m_deloneTypeFits.end() ||
         (fitValue < m_deloneTypeFits[deloneName] ||
            (m_deloneTypeFits[deloneName] >= DBL_MAX && fitValue >= DBL_MAX))) {
         m_deloneTypeFits[deloneName] = fitValue;
      }

      // Update Pearson type fit (keep the best fit)
      if (m_pearsonTypeFits.find(pearsonName) == m_pearsonTypeFits.end() ||
         (fitValue < m_pearsonTypeFits[pearsonName] ||
            (m_pearsonTypeFits[pearsonName] >= DBL_MAX && fitValue >= DBL_MAX))) {
         m_pearsonTypeFits[pearsonName] = fitValue;
      }

      // Update Pearson-to-Delone mapping
      bool found = false;
      for (auto& pair : m_pearsonToDeloneMap[pearsonName]) {
         if (pair.first == deloneName) {
            // Only update if the new value is better or both are DBL_MAX
            if (fitValue < pair.second || (pair.second >= DBL_MAX && fitValue >= DBL_MAX)) {
               pair.second = fitValue;
            }
            found = true;
            break;
         }
      }
      if (!found) {
         m_pearsonToDeloneMap[pearsonName].push_back(std::make_pair(deloneName, fitValue));
      }
   }

   // Create non-const copy for the original data structures
   // Only filter out DBL_MAX values for the original fitting mechanisms
   // but keep them for display purposes
   std::vector<DeloneFitResults> nonConstFits;
   for (const auto& fit : fits) {
      if (fit.GetRawFit() < DBL_MAX) {
         nonConstFits.push_back(fit);
      }
   }

   // Update the original data structures with the filtered fit data
   m_deloneFits.CreateMapOFDeloneFits(nonConstFits);
   m_bravaisFits.CreateMapOFBravaisFits(nonConstFits);

   // Update the GrimmerChains
   initializeChains();
}

// Helper function for wrapping content in a group
std::string GrimmerChains::wrapInGroup(const std::string& content, const std::string& id,
   int translateX, int translateY) const {
   std::stringstream ss;
   ss << "  <g id=\"" << id << "\" transform=\"translate(" << translateX
      << "," << translateY << ")\">\n";
   ss << content;
   ss << "  </g>\n";
   return ss.str();
}

void GrimmerChains::updateChains(MapOFDeloneFits& deloneFits, MapOfBravaisFits& bravaisFits) {
   m_deloneFits = deloneFits;
   m_bravaisFits = bravaisFits;
   initializeChains();
}

