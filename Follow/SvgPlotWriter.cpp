#pragma warning(disable: 4996)

#include "FollowControls.h"
#include "InputHandler.h"
#include "NiceAxisScaling.h"
#include "PathPoint.h"
#include "SvgPlotWriter.h"

#include <algorithm>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <limits>
#include <cmath>
#include <iostream>
#include <sstream>

std::vector<LineStyle> LineStyle::getStyles(size_t count,
   const std::vector<std::unique_ptr<Distance>>* distfuncs) {
   std::vector<LineStyle> styles;
   static const std::vector<std::string> fallbackColors = {
       "#000000", "#E69F00", "#56B4E9", "#009E73",
       "#FFA07A", "#0072B2", "#D55E00", "#CC79A7"
   };
   static const std::vector<std::string> dashPatterns = {
       "", "5,5", "2,2", "8,3,2,3",
       "8,3,2,3,2,3", "10,3", "3,5", "8,8"
   };
   static const std::vector<std::string> markers = {
       "M-6,0 L6,0 M0,-6 L0,6",         // plus
       "M-4.5,-4.5 L4.5,4.5 M-4.5,4.5 L4.5,-4.5", // X
       "M0,-6 L6,6 L-6,6 Z",            // triangle
       "M-4.5,0 A4.5,4.5 0 1,0 4.5,0 A4.5,4.5 0 1,0 -4.5,0", // circle
       "M-4.5,-4.5 L4.5,-4.5 L4.5,4.5 L-4.5,4.5 Z", // square
       "M-6,0 L6,0 M-3,-3 L3,3\" stroke-width=\"3.5", // cross with thicker stroke
       "M-4.5,-4.5 L4.5,-4.5 L4.5,4.5 L-4.5,4.5 L-4.5,-4.5", // diamond
       "M-6,-3 L0,6 L6,-3 Z",           // inverted triangle
       "M0,0 L4.5,-6 L-4.5,-6 L4.5,6 L-4.5,6 Z"  // bowtie
   };

   for (size_t i = 0; i < count; ++i) {
      std::string color;

      // Use Distance colors if available, otherwise fall back to hardcoded colors
      if (distfuncs != nullptr && i < distfuncs->size()) {
         color = (*distfuncs)[i]->getColor();
      }
      else {
         color = fallbackColors[i % fallbackColors.size()];
      }

      styles.push_back({
          color,
          dashPatterns[i % dashPatterns.size()],
          markers[i % markers.size()]
         });
   }
   return styles;
}


SvgPlotWriter::SvgPlotWriter(std::ofstream& outSvg, const FollowControls& controls)
   : svg(outSvg)
   , controls(controls)
   , glitches() {
}

std::string SvgPlotWriter::reportGlitches(const int n) {
   std::vector<std::pair<double, size_t>> sorted;
   for (const auto& g : glitches) {
      sorted.emplace_back(g.value, g.index);
   }
   std::sort(sorted.begin(), sorted.end(),
      [](const auto& a, const auto& b) { return a.first > b.first; });

   std::stringstream os;
   for (size_t i = 0; i < std::min(size_t(n), sorted.size()); ++i) {
      os << "      point index: " << sorted[i].second
         << " value: " << sorted[i].first << "\n";
   }
   return os.str();
}

void SvgPlotWriter::writeHeader(int width, int height) {
   svg << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
      << "<svg width=\"" << (width + 200) << "\" height=\"" << height
      << "\" xmlns=\"http://www.w3.org/2000/svg\">\n"
      << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
}

void SvgPlotWriter::writeTitle(int width, const std::string& datetime,
   int trial, int perturbation) {
   svg << R"(<text x=")" << width / 2 << R"(" y="25" )"
      << R"(font-family="Arial" font-size="16" font-weight="bold" )"
      << R"(text-anchor="middle">)"
      << R"(Distance Measurements - Trial )" << trial + 1
      << R"(, Perturbation )" << perturbation
      << R"( ()" << datetime << R"()</text>)";
}

void SvgPlotWriter::writeGrid(int width, int height, int leftMargin, int margin,
   const AxisLimits& xLimits, const AxisLimits& yLimits,
   double xScale, double yScale) {
   svg << "\n<g class=\"GRIDLINE\" clip-path=\"url(#plot-area)\" stroke=\"#e0e0e0\" stroke-width=\"1\">\n";

   // Build vertical grid lines path
   std::ostringstream vGridStr;
   auto xTicks = xLimits.GetTicksWithLabels();
   for (const auto& tick : xTicks) {
      if (tick.position == xLimits.GetMin()) continue; // Skip first tick (axis line)
      const double xPos = leftMargin + (tick.position - xLimits.GetMin()) * xScale;
      vGridStr << "M " << xPos << "," << margin << " "
         << "L " << xPos << "," << (height - margin) << " ";
   }

   // Build horizontal grid lines path
   std::ostringstream hGridStr;
   auto yTicks = yLimits.GetTicksWithLabels();
   for (const auto& tick : yTicks) {
      if (tick.position == yLimits.GetMin()) continue; // Skip first tick (axis line)
      const double yPos = height - margin - (tick.position - yLimits.GetMin()) * yScale;
      if (yPos >= margin && yPos <= height - margin) {
         hGridStr << "M " << leftMargin << "," << yPos << " "
            << "L " << (width - margin) << "," << yPos << " ";
      }
   }

   // Write the grid paths
   svg << "  <path d=\"" << vGridStr.str() << "\" />\n";
   svg << "  <path d=\"" << hGridStr.str() << "\" />\n";
   svg << "</g>\n";
}


void SvgPlotWriter::writeYAxis(int leftMargin, int height, int margin,
   const AxisLimits& limits, double yScale) {
   svg << "<g class=\"Y-AXIS\">\n";

   // Build path for axis line and ticks
   std::ostringstream pathStr;

   // Main axis line
   pathStr << "M " << leftMargin << "," << margin << " "
      << "L " << leftMargin << "," << (height - margin) << " ";

   // Add tick marks
   const int tickLength = 5;
   auto ticks = limits.GetTicksWithLabels();
   for (const auto& tick : ticks) {
      const double yPos = height - margin - (tick.position - limits.GetMin()) * yScale;
      if (yPos >= margin && yPos <= height - margin) {
         pathStr << "M " << (leftMargin - tickLength) << "," << yPos << " "
            << "L " << leftMargin << "," << yPos << " ";
      }
   }

   // Write the path
   svg << "  <path d=\"" << pathStr.str() << "\" stroke=\"black\" stroke-width=\"1\"/>\n";

   // Add labels
   for (const auto& tick : ticks) {
      const double yPos = height - margin - (tick.position - limits.GetMin()) * yScale;
      if (yPos >= margin && yPos <= height - margin) {
         svg << "  <text x=\"" << (leftMargin - 10)
            << "\" y=\"" << (yPos + 5)
            << "\" text-anchor=\"end\" font-size=\"12\">"
            << tick.label
            << "</text>\n";
      }
   }

   // Add Y-axis label
   svg << "  <text x=\"10\" y=\"" << (height / 2)
      << "\" text-anchor=\"middle\" font-size=\"14\" "
      << "transform=\"rotate(-90 10 " << (height / 2) << ")\">Distance</text>\n";

   svg << "</g>\n";
}

void SvgPlotWriter::writeXAxis(int width, int height, int leftMargin, int margin,
   const AxisLimits& limits, double xScale) {
   svg << "<g class=\"X-AXIS\">\n";

   // Build path for axis line and ticks
   std::ostringstream pathStr;

   // Main axis line
   pathStr << "M " << leftMargin << "," << (height - margin) << " "
      << "L " << (width - margin) << "," << (height - margin) << " ";

   // Add tick marks
   const int tickLength = 5;
   auto ticks = limits.GetTicksWithLabels();
   for (const auto& tick : ticks) {
      const double xPos = leftMargin + (tick.position - limits.GetMin()) * xScale;
      if (xPos >= leftMargin && xPos <= width - margin) {
         pathStr << "M " << xPos << "," << (height - margin) << " "
            << "L " << xPos << "," << (height - margin + tickLength) << " ";
      }
   }

   // Write the path
   svg << "  <path d=\"" << pathStr.str() << "\" stroke=\"black\" stroke-width=\"1\"/>\n";

   // Add labels
   for (const auto& tick : ticks) {
      const double xPos = leftMargin + (tick.position - limits.GetMin()) * xScale;
      if (xPos >= leftMargin && xPos <= width - margin) {
         svg << "  <text x=\"" << xPos
            << "\" y=\"" << (height - margin + 20)
            << "\" text-anchor=\"middle\" font-size=\"12\">"
            << std::stoi(tick.label)
            << "</text>\n";
      }
   }

   // Add X-axis label
   svg << "  <text x=\"" << (width / 2) << "\" y=\"" << (height - 10)
      << "\" text-anchor=\"middle\" font-size=\"14\">Point Index</text>\n";

   svg << "</g>\n";
}

void SvgPlotWriter::writeGridAndAxes(int width, int height, int margin, double maxDist,
   const std::vector<std::vector<double>>& allDistances) {
   const int leftMargin = margin;

   // Write clip path definition
   svg << R"(<defs>)" << '\n'
      << R"(  <clipPath id="plot-area">)" << '\n'
      << R"(    <rect x=")" << leftMargin << R"(" y=")" << margin
      << R"(" width=")" << (width - leftMargin - margin)
      << R"(" height=")" << (height - 2 * margin) << R"("/>)" << '\n'
      << R"(  </clipPath>)" << '\n'
      << R"(</defs>)" << '\n';

   // Calculate axis properties using NiceAxisScaling
   NiceAxisScaling xAxis, yAxis;
   const AxisLimits xLimits = xAxis.LinearAxisLimits(0, double(allDistances[0].size() - 1));
   const AxisLimits yLimits = yAxis.LinearAxisLimits(0, double(maxDist));

   // Calculate scales
   const double plotWidth = width - leftMargin - margin;
   const double plotHeight = height - 2 * margin;
   const double xScale = plotWidth / (xLimits.GetMax() - xLimits.GetMin());
   const double yScale = plotHeight / (yLimits.GetMax() - yLimits.GetMin());

   // Write grid first (behind axes)
   writeGrid(width, height, leftMargin, margin, xLimits, yLimits, xScale, yScale);

   // Write axes
   writeYAxis(leftMargin, height, margin, yLimits, yScale);
   writeXAxis(width, height, leftMargin, margin, xLimits, xScale);
}





void SvgPlotWriter::writePlot(const std::vector<std::vector<double>>& allDistances,
   const std::vector<std::unique_ptr<Distance>>& distfuncs,
   int trial, int perturbation,
   int width, int height) {
   if (!svg.is_open()) return;

   auto now = std::chrono::system_clock::now();
   std::time_t now_time = std::chrono::system_clock::to_time_t(now);
   struct tm* timeinfo = localtime(&now_time);
   std::ostringstream datetime;
   datetime << (timeinfo->tm_year + 1900) << "/"
      << std::setfill('0') << std::setw(2) << (timeinfo->tm_mon + 1) << "/"
      << std::setfill('0') << std::setw(2) << timeinfo->tm_mday << " "
      << std::setfill('0') << std::setw(2) << timeinfo->tm_hour << ":"
      << std::setfill('0') << std::setw(2) << timeinfo->tm_min << ":"
      << std::setfill('0') << std::setw(2) << timeinfo->tm_sec;

   double maxDist = 0.0;
   for (const auto& distances : allDistances) {
      if (!distances.empty()) {
         maxDist = std::max(maxDist, *std::max_element(distances.begin(), distances.end()));
      }
   }
   maxDist = std::ceil(maxDist * 1.1);

   const int margin = 50;

   writeHeader(width, height);
   writeTitle(width, datetime.str(), trial, perturbation);
   writeGridAndAxes(width, height, margin, maxDist, allDistances);
   writePlotData(width, height, margin, maxDist, allDistances, distfuncs);
   writeLegend(width, margin, allDistances, distfuncs);
   writeGlitchComments(distfuncs);
   writeMetadata(trial, perturbation, datetime.str());
   writeInputLines();
   WriteDistanceSummary(allDistances);

   svg << "</svg>\n";
}

void SvgPlotWriter::writeInputLines() const {
   svg << "\n<!-- Input text lines\n";
   for (const auto& g : InputHandler::globalInputLines)
   {
      svg << g << std::endl;;
   }
   svg << "-->\n\n";
}

void SvgPlotWriter::writePlotData(int width, int height, int margin, double maxDist,
   const std::vector<std::vector<double>>& allDistances,
   const std::vector<std::unique_ptr<Distance>>& distfuncs) {

   PlotDimensions dims = calculatePlotDimensions(width, height, margin, allDistances);
   auto styles = LineStyle::getStyles(allDistances.size(), &distfuncs);  // Pass address

   for (size_t i = 0; i < allDistances.size(); ++i) {
      if (allDistances[i].empty()) continue;
      drawPlotLine(allDistances[i], dims, styles[i]);
      drawGlitches(dims, distfuncs[i]->getName());
   }
}

void SvgPlotWriter::writeLegend(int width, int margin,
   const std::vector<std::vector<double>>& allDistances,
   const std::vector<std::unique_ptr<Distance>>& distfuncs) {

   auto styles = LineStyle::getStyles(distfuncs.size(), &distfuncs);  // Pass address
   const int plotWidth = width - 2 * margin;
   const int xOffset = margin + plotWidth + 20;
   const int yOffset = margin + 20;
   const int lineLength = 80;

   svg << "\n<g class=\"LEGEND\" transform=\"translate(" << xOffset << "," << yOffset << ")\">\n"
      << "<rect x=\"-5\" y=\"-15\" width=\"140\" height=\""
      << (distfuncs.size() * 25 + 10) << "\" fill=\"white\" stroke=\"black\"/>\n";

   for (size_t i = 0; i < distfuncs.size(); ++i) {
      const auto& style = styles[i];
      svg << "<line x1=\"0\" y1=\"" << (i * 25) << "\" x2=\"" << lineLength << "\" y2=\"" << (i * 25)
         << "\" stroke=\"" << style.color << "\" stroke-width=\"" << PLOT_LINE_WIDTH << "\"";
      if (!style.dashArray.empty()) {
         svg << " stroke-dasharray=\"" << style.dashArray << "\"";
      }
      svg << "/>\n";

      if (controls.shouldShowMarkers()) {
         for (int x : {0, lineLength / 2, lineLength}) {
            svg << "<path d=\"" << style.markerType << "\" "
               << "transform=\"translate(" << x << "," << (i * 25) << ")\" "
               << "stroke=\"" << style.color << "\" fill=\"none\"/>\n";
         }
      }

      svg << "<text x=\"" << (lineLength + 5) << "\" y=\"" << (i * 25 + 5)
         << "\" font-family=\"Arial\" font-size=\"14\">"
         << distfuncs[i]->getName() << "</text>\n";
   }
   svg << "</g>\n\n";
}


void SvgPlotWriter::writeMetadata(int trial, int perturbation, const std::string& datetime) {
   svg << "<metadata>\n"
      << "  <filename>" << controls.getFilenames().front() << "</filename>\n"
      << "  <trial>" << trial + 1 << "</trial>\n"
      << "  <perturbation>" << perturbation + 1 << "</perturbation>\n"
      << "  <datetime>" << datetime << "</datetime>\n"
      << "  <controlVariables>\n"
      << "    <followerMode>" << FollowerModeUtils::toString(controls.getMode()) << "</followerMode>\n"
      << "    <perturbations>" << controls.getPerturbations() << "</perturbations>\n"
      << "    <perturbBy>" << controls.getPerturbBy() << "</perturbBy>\n"
      << "    <glitchThresholdPercent>" << controls.getGlitchThreshold() << "</glitchThresholdPercent>\n"
      << "    <glitchesOnly>" << (controls.shouldShowOnlyGlitches() ? "true" : "false") << "</glitchesOnly>\n"
      << "    <numFollowerPoints>" << controls.getNumPoints() << "</numFollowerPoints>\n"
      << "    <printDistanceData>" << (controls.shouldPrintDistanceData() ? "true" : "false") << "</printDistanceData>\n"
      << "    <enabledDistances>\n";

   const auto enabledTypes = controls.getDistanceTypes().getEnabledTypes();

   for (const auto& distType : enabledTypes) {
      svg << "      <distance>" << distType << "</distance>\n";
   }

   svg << "    </enabledDistances>\n"
      << "    <glitches>\n" << reportGlitches(100) << "    </glitches>\n"
      << "  </controlVariables>\n"
      << "</metadata>\n";
}

std::string SvgPlotWriter::WriteDistanceSummary(const std::vector<std::vector<double>>& alldistances) const {
   std::stringstream os;
   for (const auto& onePath : alldistances) {
      os << "<pathsummary>\n";
      for (size_t i = 0; i < onePath.size(); ++i) {
         if (i < 5 || i > onePath.size() - 6) {
            if (onePath[i] != -19191)
            {
               os << i << "  " << onePath[i] << std::endl;
            }
            else {
               os << i << "  " << onePath[i] << "  invalid cell\n";
            }
         }
         if (i == 5)
         {
            os << " ... ..." << std::endl;
         }
      }
      os << "</pathsummary>\n";
   }
   svg << os.str();
   return os.str();
}

SvgPlotWriter::PlotDimensions SvgPlotWriter::calculatePlotDimensions(int width, int height, int margin,
   const std::vector<std::vector<double>>& allDistances) const {
   PlotDimensions dims;
   dims.leftMargin = margin;
   dims.height = height;
   dims.margin = margin;

   double actualMaxDist = 0.0;
   for (const auto& distfuncs : allDistances) {
      for (double d : distfuncs) {
         if (d > 0 && !std::isnan(d) && d != S6::MakeInvalidS6()[0]) {
            actualMaxDist = std::max(actualMaxDist, d);
         }
      }
   }

   NiceAxisScaling xAxis, yAxis;
   const AxisLimits xLimits = xAxis.LinearAxisLimits(0., double(allDistances[0].size() - 1));
   const AxisLimits yLimits = yAxis.LinearAxisLimits(0., double(actualMaxDist));

   dims.xMin = xLimits.GetMin();
   dims.yMin = yLimits.GetMin();
   dims.plotWidth = width - dims.leftMargin - margin;
   dims.plotHeight = height - 2 * margin;
   dims.xScale = dims.plotWidth / (xLimits.GetMax() - dims.xMin);
   dims.yScale = dims.plotHeight / (yLimits.GetMax() - dims.yMin);

   return dims;
}

void SvgPlotWriter::drawPlotLine(const std::vector<double>& values,
   const PlotDimensions& dims, const LineStyle& style) {

   svg << "\n\n<path d=\"";
   bool inLine = false;
   const double invalidMarker = 19191.111111111111;

   for (size_t j = 0; j < values.size(); ++j) {
      const double currentValue = values[j];
      if (currentValue < 0.0 || std::isnan(currentValue) || currentValue == invalidMarker) {
         inLine = false;
         continue;
      }
      const double x = dims.leftMargin + (j - dims.xMin) * dims.xScale;
      const double y = dims.height - dims.margin - (currentValue - dims.yMin) * dims.yScale;

      if (!inLine) {
         svg << "M" << x << "," << y;
         inLine = true;
      }
      else {
         svg << " L" << x << "," << y;
      }
   }
   svg << "\" stroke=\"" << style.color << "\" fill=\"none\" stroke-width=\"" << PLOT_LINE_WIDTH << "\"";
   if (!style.dashArray.empty()) {
      svg << " stroke-dasharray=\"" << style.dashArray << "\"";
   }
   svg << "/>\n";

   if (controls.shouldShowMarkers()) {
      const int interval = std::max(1, static_cast<int>(values.size() / NUM_MARKERS));

      for (size_t j = 0; j < values.size(); j += interval) {
         const double currentValue = values[j];
         if (currentValue < 0.0 || std::isnan(currentValue) || currentValue == invalidMarker) continue;

         const double x = dims.leftMargin + (j - dims.xMin) * dims.xScale;
         const double y = dims.height - dims.margin - (currentValue - dims.yMin) * dims.yScale;

         svg << "<path d=\"" << style.markerType << "\" "
            << "transform=\"translate(" << x << "," << y << ")\" "
            << "stroke=\"" << style.color << "\" fill=\"none\"/>\n";
      }
   }
}

void SvgPlotWriter::drawGlitches(const PlotDimensions& dims, const std::string& distanceType) {
   for (const auto& glitch : glitches) {
      if (glitch.distanceType != distanceType) continue;
      if (glitch.value == -19191.0) continue;

      const double x = dims.leftMargin + (glitch.index - dims.xMin) * dims.xScale;
      const double y = dims.height - dims.margin - (glitch.value - dims.yMin) * dims.yScale;

      drawGlitchLine(x, dims.height, dims.margin);
      drawGlitchMarker(x, y);
      drawGlitchIndex(x, y, dims.margin, glitch.index);
   }
}

void SvgPlotWriter::drawGlitchLine(double x, int height, int margin) {
   svg << "\n" << R"(<line x1=")" << x << R"(" y1=")" << (height - margin)
      << R"(" x2=")" << x << R"(" y2=")" << margin
      << R"(" stroke="#0000FF" stroke-width="1.5" stroke-dasharray="5,5"/>)";
}

void SvgPlotWriter::drawGlitchMarker(double x, double y) {
   svg << "\n" << R"(<path id="GlitchMarker" d="M)" << (x - 5) << " " << y
      << " L" << x << " " << (y - 5) << " L" << (x + 5) << " " << y
      << " L" << x << " " << (y + 5)
      << R"( Z" fill="#0000FF" stroke="black" stroke-width="1"/>)";
}

void SvgPlotWriter::drawGlitchIndex(double x, double y, int margin, size_t index) {
   svg << "\n" << R"(<text id="GlitchIndex" x=")" << x << R"(" y=")" << (margin - 5)
      << R"(" text-anchor="middle" font-size="12" fill="#0000FF">)" << index << R"(</text>)";
}

void SvgPlotWriter::writeGlitchComments(const std::vector<std::unique_ptr<Distance>>& distfuncs) {
   svg << "\n<!--\nGlitch Summary\n";
   for (const auto& distfunc : distfuncs) {
      const std::string& distType = distfunc->getName();
      std::vector<Glitch> typeGlitches;

      std::copy_if(glitches.begin(), glitches.end(), std::back_inserter(typeGlitches),
         [&distType](const Glitch& g) { return g.distanceType == distType; });

      std::sort(typeGlitches.begin(), typeGlitches.end(),
         [](const Glitch& a, const Glitch& b) { return a.changePercent > b.changePercent; });

      const size_t numToShow = std::min(size_t(10), typeGlitches.size());
      if (numToShow > 0) {
         svg << "\n" << distType << " glitches:\n";
         for (size_t j = 0; j < numToShow; ++j) {
            svg << typeGlitches[j] << "\n";
         }
      }
   }
   svg << "-->\n\n";
}
