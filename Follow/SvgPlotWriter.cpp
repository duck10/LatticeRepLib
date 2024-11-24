// SvgPlotWriter.cpp
#pragma warning(disable: 4996)

#include "SvgPlotWriter.h"
#include "LinearAxis.h"

#include <iomanip>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <limits>
#include <cmath>
#include <iostream>
#include <sstream>

SvgPlotWriter::SvgPlotWriter(std::ofstream& outSvg, const ControlVariables& cv,
   GlitchDetector& detector)
   : svg(outSvg), controlVars(cv), glitchDetector(detector) {}


void SvgPlotWriter::writePlot(const std::vector<std::vector<double>>& allDistances,
   const std::vector<std::unique_ptr<Distance>>& distances,
   int trial, int perturbation,
   int width, int height)
{
   if (!svg.is_open()) return;

   auto now = std::chrono::system_clock::now();
   std::time_t now_time = std::chrono::system_clock::to_time_t(now);
   std::string datetime = std::ctime(&now_time);
   datetime.pop_back();  // Remove newline

   double maxDist = 0.0;
   for (const auto& distances : allDistances) {
      if (!distances.empty()) {
         maxDist = std::max(maxDist, *std::max_element(distances.begin(), distances.end()));
      }
   }
   maxDist = std::ceil(maxDist * 1.1);

   const int margin = 50;

   writeHeader(width, height);
   writeTitle(width, datetime, trial, perturbation);
   writeGridAndAxes(width, height, margin, maxDist, allDistances);
   writePlotData(width, height, margin, maxDist, allDistances);
   writeLegend(width, margin, allDistances, distances);
   writeMetadata(trial, perturbation, datetime);

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
   svg << "<text x=\"" << width / 2 << "\" y=\"25\" "
      << "font-family=\"Arial\" font-size=\"16\" font-weight=\"bold\" "
      << "text-anchor=\"middle\">"
      << "Distance Measurements - Trial " << trial + 1
      << ", Perturbation " << perturbation + 1
      << " (" << datetime << ")</text>\n";
}

void SvgPlotWriter::writeGridAndAxes(int width, int height, int margin,
   double maxDist, const std::vector<std::vector<double>>& allDistances) {

   bool useScientific = maxDist >= 1000;
   int leftMargin = margin;  // No extra margin needed since we don't use scientific notation for numbers

   // Define clip path
   svg << "<defs>\n"
      << "  <clipPath id=\"plot-area\">\n"
      << "    <rect x=\"" << leftMargin << "\" y=\"" << margin
      << "\" width=\"" << (width - leftMargin - margin)
      << "\" height=\"" << (height - 2 * margin) << "\"/>\n"
      << "  </clipPath>\n"
      << "</defs>\n";

   LinearAxis xAxis, yAxis;
   AxisLimits xLimits = xAxis.LinearAxisLimits(0, allDistances[0].size() - 1);
   AxisLimits yLimits = yAxis.LinearAxisLimits(0, maxDist);

   double xMin = xLimits.GetMin();
   double xMax = xLimits.GetMax();
   double xStepSize = xLimits.GetStepSize();
   double yMin = yLimits.GetMin();
   double yMax = yLimits.GetMax();
   double yStepSize = yLimits.GetStepSize();

   const double plotWidth = width - leftMargin - margin;
   const double plotHeight = height - 2 * margin;
   const double xScale = plotWidth / (xMax - xMin);
   const double yScale = plotHeight / (yMax - yMin);

   // Draw grid lines with clipping
   svg << "<g clip-path=\"url(#plot-area)\" stroke=\"#e0e0e0\" stroke-width=\"1\">\n";
   // Draw vertical grid lines
   for (double x = xMin; x <= xMax; x += xStepSize) {
      double xPos = leftMargin + (x - xMin) * xScale;
      svg << "  <line x1=\"" << xPos << "\" y1=\"" << margin << "\" "
         << "x2=\"" << xPos << "\" y2=\"" << (height - margin) << "\"/>\n";
   }
   // Draw horizontal grid lines
   for (double y = yMin; y <= yMax; y += yStepSize) {
      double yPos = height - margin - (y - yMin) * yScale;
      if (yPos >= margin && yPos <= height - margin) {
         svg << "  <line x1=\"" << leftMargin << "\" y1=\"" << yPos << "\" "
            << "x2=\"" << (width - margin) << "\" y2=\"" << yPos << "\"/>\n";
      }
   }
   svg << "</g>\n";

   // Draw y-axis label with scale indicator and exponent
   writeAxisLabels(leftMargin, width, height, margin, xMin, xMax, xStepSize,
      yMin, yMax, yStepSize, xScale, yScale, useScientific);
}

void SvgPlotWriter::writeAxisLabels(int leftMargin, int width, int height, int margin,
   double xMin, double xMax, double xStepSize, double yMin, double yMax, double yStepSize,
   double xScale, double yScale, bool useScientific) {

   // Calculate the exponent for the axis label if values are large
   int exponent = 0;
   if (yMax >= 1000) {
      exponent = static_cast<int>(std::floor(std::log10(yMax)));
   }

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

   // Draw y-axis line
   svg << "<line x1=\"" << leftMargin << "\" y1=\"" << margin
      << "\" x2=\"" << leftMargin << "\" y2=\"" << (height - margin)
      << "\" stroke=\"black\"/>\n";

   // Draw y-axis tick marks and labels
   for (double y = yMin; y <= yMax; y += yStepSize) {
      double yPos = height - margin - (y - yMin) * yScale;
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
   int yAxisLabelOffset = 40;
   svg << "<text x=\"" << (leftMargin - yAxisLabelOffset) << "\" y=\"" << (height / 2)
      << "\" text-anchor=\"middle\" font-size=\"14\" transform=\"rotate(-90 "
      << (leftMargin - yAxisLabelOffset) << " " << (height / 2) << ")\">"
      << "Distance";

   if (exponent > 0) {
      svg << " (x10^" << exponent;  // Changed to include the exponent directly
      svg << ")</text>\n";
   }
   else {
      svg << "</text>\n";  // Just close the label if no exponent
   }
}
void SvgPlotWriter::writePlotData(int width, int height, int margin, double maxDist,
   const std::vector<std::vector<double>>& allDistances) {

   int leftMargin = margin;

   LinearAxis xAxis, yAxis;
   AxisLimits xLimits = xAxis.LinearAxisLimits(0, allDistances[0].size() - 1);
   AxisLimits yLimits = yAxis.LinearAxisLimits(0, maxDist);

   double xMin = xLimits.GetMin();
   double yMin = yLimits.GetMin();

   const double plotWidth = width - leftMargin - margin;
   const double plotHeight = height - 2 * margin;
   const double xScale = plotWidth / (xLimits.GetMax() - xMin);
   const double yScale = plotHeight / (yLimits.GetMax() - yMin);

   for (size_t i = 0; i < allDistances.size(); ++i) {
      const auto& distanceValues = allDistances[i];
      if (distanceValues.empty()) continue;

      std::vector<GlitchDetector::Glitch> glitches;
      if (controlVars.shouldDetectGlitches)
      {
         glitches =
            glitchDetector.detectGlitches(distanceValues, controlVars.glitchThresholdPercent, 5);
      }

      // Use interpolated color
      std::string color = ColorTables::interpolateColor(i, allDistances.size());
      std::string pathId = "Path" + std::to_string(i);

      svg << "<path id=\"" << pathId << "\" d=\"M";
      for (size_t j = 0; j < distanceValues.size(); ++j) {
         double x = leftMargin + (j - xMin) * xScale;  // Modified this line
         double y = height - margin - (distanceValues[j] - yMin) * yScale;
         if (j > 0) svg << " L";
         svg << x << "," << y;
      }
      svg << "\" stroke=\"" << color << "\" fill=\"none\" stroke-width=\"2\"/>\n";

      if (controlVars.showDataMarkers) {
         for (size_t j = 0; j < distanceValues.size(); ++j) {
            double x = leftMargin + (j - xMin) * xScale;  // Modified this line
            double y = height - margin - (distanceValues[j] - yMin) * yScale;
            svg << "<circle cx=\"" << x << "\" cy=\"" << y
               << "\" r=\"3\" fill=\"" << color << "\"/>\n";
         }
      }

      for (const auto& glitch : glitches) {
         size_t j = glitch.index;
         double x = leftMargin + (j - xMin) * xScale;  // Modified this line
         double y = height - margin - (distanceValues[j] - yMin) * yScale;

         svg << "<line x1=\"" << x << "\" y1=\"" << (height - margin)
            << "\" x2=\"" << x << "\" y2=\"" << margin
            << "\" stroke=\"#0000FF\" stroke-width=\"1.5\" stroke-dasharray=\"5,5\"/>\n";

         svg << "<path d=\"M" << (x - 5) << " " << y << " L" << x << " "
            << (y - 5) << " L" << (x + 5) << " " << y << " L" << x
            << " " << (y + 5) << " Z\" fill=\"#0000FF\" "
            << "stroke=\"black\" stroke-width=\"1\"/>\n";

         svg << "<text x=\"" << x << "\" y=\"" << (margin - 5)
            << "\" text-anchor=\"middle\" font-size=\"12\" fill=\"#0000FF\">"
            << j << "</text>\n";
      }
   }
}

struct DataPoint {
   double x;
   double y;
   size_t index;
};

void SvgPlotWriter::writeLegend(int width, int margin,
   const std::vector<std::vector<double>>& allDistances,
   const std::vector<std::unique_ptr<Distance>>& distances) {



   std::vector<DataPoint> sampledPoints;
   for (const auto& distances : allDistances) {
      if (!distances.empty()) {
         size_t len = distances.size();
         sampledPoints.push_back({ 0.0, distances[0], 0 });
         sampledPoints.push_back({ 0.5, distances[len / 2], len / 2 });
         sampledPoints.push_back({ 1.0, distances[len - 1], len - 1 });
      }
   }

   double minY = std::numeric_limits<double>::max();
   double bestX = 1.0;

   for (const auto& point : sampledPoints) {
      if (point.y < minY) {
         minY = point.y;
         bestX = point.x;
      }
   }

   int xOffset = (bestX < 0.3) ? margin + 20 :
      (bestX > 0.7) ? width - margin - 100 :
      (width / 2) - 50;

   svg << "<g transform=\"translate(" << xOffset << "," << margin + 20 << ")\">\n"
      << "<rect x=\"-5\" y=\"-15\" width=\"90\" height=\""
      << (distances.size() * 20 + 10) << "\" fill=\"white\" stroke=\"black\"/>\n";

   auto xxxx1 = colorRange.GetColorFromRangeFraction(0);
   auto xxxx2 = colorRange.GetColorFromRangeFraction(.99999);


   for (size_t i = 0; i < distances.size(); ++i) {
      // Get interpolated color instead of using CS6_COLOR or NC_COLOR
      std::string color = ColorTables::interpolateColor(i, distances.size());

      svg << "<line x1=\"0\" y1=\"" << (i * 20) << "\" x2=\"20\" y2=\"" << (i * 20)
         << "\" stroke=\"" << color << "\" stroke-width=\"10\"/>\n"
         << "<text x=\"25\" y=\"" << (i * 20 + 5)
         << "\" font-family=\"Arial\" font-size=\"12\">"
         << distances[i]->getName() << "</text>\n";
   }
   svg << "</g>\n";
}


void SvgPlotWriter::writeMetadata(int trial, int perturbation, const std::string& datetime) {
   svg << "<metadata>\n"
      << "  <filename>" << controlVars.filenames[trial * controlVars.perturbations + perturbation]
      << "</filename>\n"
      << "  <trial>" << trial + 1 << "</trial>\n"
      << "  <perturbation>" << perturbation + 1 << "</perturbation>\n"
      << "  <datetime>" << datetime << "</datetime>\n"
      << "  <controlVariables>\n"
      << "    <followerMode>";

   switch (controlVars.followerMode) {
   case FollowerMode::POINT: svg << "POINT"; break;
   case FollowerMode::LINE: svg << "LINE"; break;
   case FollowerMode::CHORD: svg << "CHORD"; break;
   case FollowerMode::CHORD3: svg << "CHORD3"; break;
   case FollowerMode::TRIANGLE: svg << "TRIANGLE"; break;
   }

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

   svg << "    </enabledDistances>\n"
      << "  </controlVariables>\n"
      << "</metadata>\n";
}

