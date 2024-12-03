// SvgPlotWriter.cpp
#pragma warning(disable: 4996)

#include "SvgPlotWriter.h"
#include "LinearAxis.h"
#include "PathPoint.h"

#include <algorithm>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <limits>
#include <cmath>
#include <iostream>
#include <sstream>

SvgPlotWriter::SvgPlotWriter(std::ofstream& outSvg, const ControlVariables& cv,
   GlitchDetector& detector)
   : svg(outSvg), controlVars(cv), glitchDetector(detector) {}


void SvgPlotWriter::writePlot(const std::vector<std::vector<double>>& allDistances,
   const std::vector<std::unique_ptr<Distance>>& distfuncs,
   int trial, int perturbation,
   int width, int height)
{
   if (!svg.is_open()) return;

   auto now = std::chrono::system_clock::now();
   std::time_t now_time = std::chrono::system_clock::to_time_t(now);
   struct tm* timeinfo = localtime(&now_time);
   std::ostringstream datetime;
   datetime << (timeinfo->tm_year + 1900) << "/"  // Year
      << std::setfill('0') << std::setw(2) << (timeinfo->tm_mon + 1) << "/"  // Month
      << std::setfill('0') << std::setw(2) << timeinfo->tm_mday << " "  // Day
      << std::setfill('0') << std::setw(2) << timeinfo->tm_hour << ":"  // Hour
      << std::setfill('0') << std::setw(2) << timeinfo->tm_min << ":"  // Minute
      << std::setfill('0') << std::setw(2) << timeinfo->tm_sec;  // Second
   std::string datetime_str = datetime.str();

   double maxDist = 0.0;
   for (const auto& distfuncs : allDistances) {
      if (!distfuncs.empty()) {
         maxDist = std::max(maxDist, *std::max_element(distfuncs.begin(), distfuncs.end()));
      }
   }
   maxDist = std::ceil(maxDist * 1.1);

   const int margin = 50;

   writeHeader(width, height);
   writeTitle(width, datetime.str(), trial, perturbation);
   writeGridAndAxes(width, height, margin, maxDist, allDistances);
   writePlotData(width, height, margin, maxDist, allDistances);
   writeLegend(width, margin, allDistances, distfuncs);
   writeMetadata(trial, perturbation, datetime.str());

   svg << "</svg>\n";
}

void SvgPlotWriter::writeHeader(int width, int height) {
   svg << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
      << "<svg width=\"" << width << "\" height=\"" << height
      << "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
      << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
}

void SvgPlotWriter::writeTitle(int width, const std::string& datetime,
   int trial, int perturbation) {
   svg <<R"(<text x=")" << width / 2 << R"(" y="25" )"
      << R"(font-family="Arial" font-size="16" font-weight="bold" )"
      << R"(text-anchor="middle">)"
      << R"(Distance Measurements - Trial )" << trial + 1
      << R"(, Perturbation )" << perturbation
      << R"( ()" << datetime << R"()</text>)";
}

void SvgPlotWriter::writeGridAndAxes(int width, int height, int margin,
   double maxDist, const std::vector<std::vector<double>>& allDistances) {

   bool useScientific = maxDist >= 1000;
   const int leftMargin = margin;  // No extra margin needed since we don't use scientific notation for numbers

   // Define clip path
   svg << R"(<defs>)" << '\n'
      << R"(  <clipPath id="plot-area">)" << '\n'
      << R"(    <rect x=")" << leftMargin << R"(" y=")" << margin
      << R"(" width=")" << (width - leftMargin - margin)
      << R"(" height=")" << (height - 2 * margin) << R"("/>)" << '\n'
      << R"(  </clipPath>)" << '\n'
      << R"(</defs>)" << '\n';

   LinearAxis xAxis, yAxis;
   const AxisLimits xLimits = xAxis.LinearAxisLimits(0, double(allDistances[0].size() - 1));
   const AxisLimits yLimits = yAxis.LinearAxisLimits(0, double(maxDist));

   const double xMin = xLimits.GetMin();
   const double xMax = xLimits.GetMax();
   const double xStepSize = xLimits.GetStepSize();
   const double yMin = yLimits.GetMin();
   const double yMax = yLimits.GetMax();
   const double yStepSize = yLimits.GetStepSize();

   const double plotWidth = width - leftMargin - margin;
   const double plotHeight = height - 2 * margin;
   const double xScale = plotWidth / (xMax - xMin);
   const double yScale = plotHeight / (yMax - yMin);

   // Draw grid lines with clipping
   svg << "\n" << "<g id=\"GRIDLINE\" clip-path = \"url(#plot-area)\" stroke=\"#e0e0e0\" stroke-width=\"1\">\n";
   // Draw vertical grid lines
   for (double x = xMin; x <= xMax; x += xStepSize) {
      const double xPos = leftMargin + (x - xMin) * xScale;
      svg << "  <line x1=\"" << xPos << "\" y1=\"" << margin << "\" "
         << "x2=\"" << xPos << "\" y2=\"" << (height - margin) << "\"/>\n";
   }

   // Draw horizontal grid lines
   for (double y = yMin; y <= yMax; y += yStepSize) {
      const double yPos = height - margin - (y - yMin) * yScale;
      if (yPos >= margin && yPos <= height - margin) {
         svg << "  <line x1=\"" << leftMargin << "\" y1=\"" << yPos << "\" "
            << "x2=\"" << (width - margin) << "\" y2=\"" << yPos << "\"/>\n";
      }
   }
   svg << "</g>\n\n";

   // Draw y-axis label with scale indicator and exponent
   writeAxisLabels(leftMargin, width, height, margin, xMin, xMax, xStepSize,
      yMin, yMax, yStepSize, xScale, yScale, useScientific);
}

void SvgPlotWriter::writeAxisLabels(int leftMargin, int width, int height, int margin,
   const double xMin, double xMax, double xStepSize, double yMin, double yMax, double yStepSize,
   const double xScale, double yScale, bool useScientific) {

   // Calculate the exponent for the axis label if values are large
   int exponent = 0;
   if (yMax >= 1000) {
      exponent = static_cast<int>(std::floor(std::log10(yMax)));
   }

   svg << "<g id=\"X-AXIS\">\n";
   // First clear the area where the x-axis will be drawn
   svg << "<rect x=\"" << leftMargin << "\" y=\"" << (height - margin)
      << "\" width=\"" << (width - leftMargin - margin) << "\" height=\"" << margin
      << "\" fill=\"white\"/>\n";

   // Draw x-axis line - shifted to be just below the plotted area
   svg << "<line x1=\"" << leftMargin << "\" y1=\"" << (height - margin)
      << "\" x2=\"" << (width - margin) << "\" y2=\"" << (height - margin)
      << "\" stroke=\"black\"/>\n";

   // Draw x-axis tick marks and labels using same coordinate transform as grid
   for (double x = xMin; x <= xMax; x += xStepSize) {
      double xPos = leftMargin + (x - xMin) * xScale;
      // Draw tick mark
      svg << "<line x1=\"" << xPos << "\" y1=\"" << (height - margin)
         << "\" x2=\"" << xPos << "\" y2=\"" << (height - margin + 5)
         << "\" stroke=\"black\"/>\n";
      // Draw label
      svg << "<text x=\"" << xPos << "\" y=\"" << (height - margin + 20)
         << "\" text-anchor=\"middle\" font-size=\"12\">"
         << static_cast<int>(std::round(x)) << "</text>\n";
   }

   // Draw x-axis label
   svg << "<text x=\"" << (width / 2) << "\" y=\"" << (height - 10)
      << "\" text-anchor=\"middle\" font-size=\"14\">Point Index</text>\n";
   svg << "</g>\n\n";
 
   svg << "<g id=\"Y-AXIS\">\n";
   // Draw y-axis line
   svg << "<line x1=\"" << leftMargin << "\" y1=\"" << margin
      << "\" x2=\"" << leftMargin << "\" y2=\"" << (height - margin)
      << "\" stroke=\"black\"/>\n";

   // Draw y-axis tick marks and labels
   for (double y = yMin; y <= yMax; y += yStepSize) {
      const double yPos = height - margin - (y - yMin) * yScale;
      if (yPos >= margin && yPos <= height - margin) {
         svg << "<line x1=\"" << (leftMargin - 5) << "\" y1=\"" << yPos
            << "\" x2=\"" << leftMargin << "\" y2=\"" << yPos
            << "\" stroke=\"black\"/>\n";

         std::ostringstream labelStream;
         labelStream << std::fixed << std::setprecision(1);
         if (exponent > 0) {
            double scaledValue = y / std::pow(10.0, exponent);
            labelStream << std::fixed << std::setprecision(1);
            if (scaledValue < 10) {
               labelStream << std::setprecision(2);  // More precision for small numbers
            }
            labelStream << scaledValue;
         }
         else {
            labelStream << y;
         }

         svg << "<text x=\"" << (leftMargin - 10) << "\" y=\"" << (yPos + 5)
            << "\" text-anchor=\"end\" font-size=\"12\">"
            << labelStream.str() << "</text>\n";
      }
   }

   // Draw y-axis label with scale indicator and exponent
   const int yAxisLabelOffset = 40;
   svg << R"(<text x=")" << (leftMargin - yAxisLabelOffset)
      << R"(" y=")" << (height / 2)
      << R"(" text-anchor="middle" font-size="14" transform="rotate(-90 )"
      << (leftMargin - yAxisLabelOffset) << " " << (height / 2) << ")\">"
      << "Distance";

   if (exponent > 0) {
      svg << R"( (x10^)" << exponent << R"()</text>)";
   }
   else {
      svg << R"(</text>)";
   }
   svg << "</g>\n\n";
}

static S6 MakeInvalidS6() {
   return { 19191.111111111111, 0, 0, 0, 0, 0 };
}

void SvgPlotWriter::writePlotData(int width, int height, int margin, double maxDist,
   const std::vector<std::vector<double>>& allDistances) {
   int leftMargin = margin;
   LinearAxis xAxis, yAxis;

   // Find actual maximum y value first
   double actualMaxDist = 0.0;
   for (const auto& distfuncs : allDistances) {
      for (double d : distfuncs) {
         if (d > 0 && !std::isnan(d) && d != MakeInvalidS6()[0]) {
            actualMaxDist = std::max(actualMaxDist, d);
         }
      }
   }

   const AxisLimits xLimits = xAxis.LinearAxisLimits(0., double(allDistances[0].size() - 1));
   const AxisLimits yLimits = yAxis.LinearAxisLimits(0., double(actualMaxDist));  // Use actual max
   const double xMin = xLimits.GetMin();
   const double yMin = yLimits.GetMin();
   const double plotWidth = width - leftMargin - margin;
   const double plotHeight = height - 2 * margin;
   const double xScale = plotWidth / (xLimits.GetMax() - xMin);
   const double yScale = plotHeight / (yLimits.GetMax() - yMin);

   // Get the invalid marker value
   const double invalidMarker = MakeInvalidS6()[0];  // Get the first component, they're all the same

   for (size_t i = 0; i < allDistances.size(); ++i) {
      const auto& distanceValues = allDistances[i];
      if (distanceValues.empty()) continue;

      if (controlVars.shouldDetectGlitches)
      {
         glitches =
            glitchDetector.detectGlitches(distanceValues, controlVars.glitchThresholdPercent, 5);
      }

      std::string color = ColorTables::interpolateColor(i, allDistances.size());
      std::string pathId = "Path" + std::to_string(i);

      svg << "\n<path id=\"" << pathId << "\" d=\"";

      bool inLine = false;

      for (size_t j = 0; j < distanceValues.size(); ++j) {
         const double currentValue = distanceValues[j];

         if (currentValue < 0.0 || std::isnan(currentValue) || currentValue == invalidMarker) {
            inLine = false;
            continue;
         }
         const double x = leftMargin + (j - xMin) * xScale;
         const double y = height - margin - (currentValue - yMin) * yScale;

         if (!inLine) {
            svg << "M" << x << "," << y;
            inLine = true;
         }
         else {
            svg << " L" << x << "," << y;
         }
      }

      svg << R"(" stroke=")" << color << R"(" fill="none" stroke-width="2"/>\n)";

      // Draw markers only for valid values
      if (controlVars.showDataMarkers) {
         for (size_t j = 0; j < distanceValues.size(); ++j) {
            const double currentValue = distanceValues[j];
            if (currentValue >= 0 && !std::isnan(currentValue) && currentValue != invalidMarker) {
               const double x = leftMargin + (j - xMin) * xScale;
               const double y = height - margin - (currentValue - yMin) * yScale;
               if (!std::isnan(y)) {
                  svg << R"(<circle cx=")" << x << R"(" cy=")" << y
                     << R"(" r="3" fill=")" << color << R"("/>
)";
               }
            }
         }
      }

      // Draw glitch markers only for valid values
      for (const auto& glitch : glitches) {
         size_t j = glitch.index;
         const double currentValue = distanceValues[j];
         if (currentValue >= 0 && !std::isnan(currentValue) && currentValue != invalidMarker) {
            const double x = leftMargin + (j - xMin) * xScale;
            const double y = height - margin - (currentValue - yMin) * yScale;

            if (!std::isnan(y)) {
               svg << R"(<line x1=")" << x << R"(" y1=")" << (height - margin)
                  << R"(" x2=")" << x << R"(" y2=")" << margin
                  << R"(" stroke="#0000FF" stroke-width="1.5" stroke-dasharray="5,5"/>)";
               svg << R"(<path d="M)" << (x - 5) << " " << y << " L" << x << " "
                  << (y - 5) << " L" << (x + 5) << " " << y << " L" << x
                  << " " << (y + 5) << R"( Z" fill="#0000FF" stroke="black" stroke-width="1"/>)";
               svg << R"(<text x=")" << x << R"(" y=")" << (margin - 5)
                  << R"(" text-anchor="middle" font-size="12" fill="#0000FF">)" << j << R"(</text>)";
            }
         }
      }
   }
}



struct DataPoint {
   double x;
   double y;
   size_t index;
};

int CalculateXOffset(double bestX, int width, int margin) {
   return (bestX < 0.3) ? margin + 20 :
      (bestX > 0.7) ? width - margin - 100 :
      (width / 2) - 50;
}

void SvgPlotWriter::writeLegend(int width, int margin,
   const std::vector<std::vector<double>>& allDistances,
   const std::vector<std::unique_ptr<Distance>>& distfuncs) {

   std::vector<DataPoint> sampledPoints;
   for (const auto& distfuncs : allDistances) {
      if (!distfuncs.empty()) {
         size_t len = distfuncs.size();
         sampledPoints.push_back({ 0.0, distfuncs[0], 0 });
         sampledPoints.push_back({ 0.5, distfuncs[len / 2], len / 2 });
         sampledPoints.push_back({ 1.0, distfuncs[len - 1], len - 1 });
      }
   }

   // put the legend near the lowest point of the distance curve
   double minY = std::numeric_limits<double>::max();
   double bestX = 1.0;
   for (const auto& point : sampledPoints) {
      if (point.y < minY) {
         minY = point.y;
         bestX = point.x;
      }
   }

   const int xOffset = CalculateXOffset(bestX, width, margin);

   svg << "\n" << R"(<g id="LEGEND" transform="translate()" << xOffset << "," << margin + 20 << ")\">\n"
      << "<rect x=\"-5\" y=\"-15\" width=\"90\" height=\""
      << (distfuncs.size() * 20 + 10) << R"(" fill="white" stroke="black"/>)" << "\n";

   for (size_t i = 0; i < distfuncs.size(); ++i) {
      // Get interpolated color instead of using CS6_COLOR or NC_COLOR
      std::string color = ColorTables::interpolateColor(i, distfuncs.size());

      svg << R"(<line x1="0" y1=")" << (i * 20) << R"(" x2="20" y2=")" << (i * 20)
         << R"(" stroke=")" << color << R"(" stroke-width="10"/>)" << "\n"
         << R"(<text x="25" y=")" << (i * 20 + 5)
         << R"(" font-family="Arial" font-size="12">)"
         << distfuncs[i]->getName() << R"(</text>)" << "\n";
   }
   svg << "</g>\n\n";
}

void SvgPlotWriter::writeMetadata(int trial, int perturbation, const std::string& datetime) {
   svg << "<metadata>\n"
      << "  <filename>" << controlVars.filenames[trial * controlVars.perturbations + perturbation]
      << "</filename>\n"
      << "  <trial>" << trial + 1 << "</trial>\n"
      << "  <perturbation>" << perturbation + 1 << "</perturbation>\n"
      << "  <datetime>" << datetime << "</datetime>\n"
      << "  <controlVariables>\n\n"
      << "    <followerMode>";

   switch (controlVars.followerMode) {
   case FollowerMode::POINT: svg << "POINT"; break;
   case FollowerMode::LINE: svg << "LINE"; break;
   case FollowerMode::CHORD: svg << "CHORD"; break;
   case FollowerMode::CHORD3: svg << "CHORD3"; break;
   case FollowerMode::TRIANGLE: svg << "TRIANGLE"; break;
   }
   svg << "\n";

   svg << "  <pathStart>\n";
   for (const auto& point : controlVars.pathStart) {
      svg << "   S6 " << S6(point) << "\n";
   }
   svg << "  </pathStart>\n\n";

   svg << "  <path>\n";
   const Path& path = controlVars.path;
   for (size_t i = 0; i < path.size(); ++i ) {
      if ( i < 4 || i > path.size()-5)
      {
         const S6& pnt1(path[i].first);
         const S6& pnt2(path[i].second);
         const std::string invalid1 = (pnt1.IsValid()) ? "" : " invalid";
         const std::string invalid2 = (pnt2.IsValid()) ? "" : " invalid";
         svg << "   S6 " << S6(path[i].first) << invalid1 << "  -------- - \n";
         svg << "   S6 " << S6(path[i].second) << invalid2 << "\n";
      }
      else
      {
         svg << " ... ... ... ... \n";
         i = path.size() - 5;
      }
   }
   svg << "</path>\n";

   svg << "</followerMode>\n"
      << "    <perturbations>" << controlVars.perturbations << "</perturbations>\n"
      << "    <perturbBy>" << controlVars.perturbBy << "</perturbBy>\n"
      << "    <glitchThresholdPercent>" << controlVars.glitchThresholdPercent << "</glitchThresholdPercent>\n"
      << "    <numFollowerPoints>" << controlVars.numFollowerPoints << "</numFollowerPoints>\n"
      << "    <printDistanceData>" << (controlVars.printDistanceData ? "true" : "false")
      << "</printDistanceData>\n"
      << "    <glitchesOnly>" << (controlVars.glitchesOnly ? "true" : "false")
      << "</glitchesOnly>\n"
      << "    <fixRandomSeed>" << (controlVars.fixRandomSeed ? "true" : "false")
      << "</fixRandomSeed>\n"
      << "    <randomSeed>" << controlVars.randomSeed << "</randomSeed>\n"
      << "    <timestamp>" << (controlVars.timestamp ? "true" : "false") << "</timestamp>\n"
      << "    <enabledDistances>\n";

   for (const auto& dist : controlVars.enabledDistances) {
      svg << "      <distance>" << dist << "</distance>\n";
   }
   svg << "    </enabledDistances>\n";

   svg << "<glitches>\n" << reportGlitches(100) << "</glitches>\n";

    svg  << "  </controlVariables>\n"
      << "</metadata>\n";
}

// Custom comparator to sort by the double value in descending order 
bool compareDescending(const std::pair<double, GlitchDetector::Glitch>& a, const std::pair<double, GlitchDetector::Glitch>& b)
{
   return a.first > b.first;
}

std::string  SvgPlotWriter::reportGlitches(const int n) {
   std::vector < std::pair<double, GlitchDetector::Glitch>> sorted;
   for (const auto& g : glitches) {
      sorted.emplace_back(g.value, g);
   }
   std::sort(sorted.begin(), sorted.end(), compareDescending);

   std::stringstream os;
   for (size_t i = 0; i < std::min(size_t(n), sorted.size()); ++i) {
      os << " " << controlVars.path[glitches[i].index].first.getLatticeType() << " S6 "
         << S6(controlVars.path[glitches[i].index].first.getCell())
         << " percent change " << glitches[i].changePercent << "\n "
         << controlVars.path[glitches[i].index].second.getLatticeType() << " S6 "
         << S6(controlVars.path[glitches[i].index].second.getCell()) << std::endl;
   }

   return os.str();
}
