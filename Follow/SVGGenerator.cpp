#pragma warning(disable:4996)

#include "SVGGenerator.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <limits>
#include <cmath>

std::string SVGGenerator::generateSVG(const std::map<std::string, std::pair<std::vector<S6>, std::vector<double>>>& results,
   const ControlVariables& controlVars,
   const std::vector<std::unique_ptr<Cell>>& cells) {
   std::ostringstream svg;
   int width = 1000;
   int height = 1000;
   int margin = 150;
   int plotWidth = 800;
   int plotHeight = 800;

   auto [minY, maxY] = findMinMax(results);
   size_t maxSteps = findMaxSteps(results);

   svg << "<?xml version=\"1.0\" standalone=\"no\"?>\n"
      << "<svg width=\"" << width << "\" height=\"" << height << "\" viewBox=\"1 0 1200 1200\" "
      << "version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
      << "<desc>ID (and file name) = " << controlVars.currentFilename << "  mode = "
      << (controlVars.followerMode == FollowerMode::POINT ? "Single Point" : "Line") << "</desc>\n";

   // Add white background
   svg << "<rect fill=\"rgb(255,255,255)\" width=\"1200\" height=\"1200\" />\n";

   // Add left legend (vertical text)
   svg << "<text x=\"50\" y=\"650\" transform=\"rotate(-90, 50, 650)\" "
      << "font-family=\"sans-serif\" font-size=\"40\" stroke=\"lightgray\" fill=\"none\">"
      << "Follower, " << getCurrentDate() << "</text>\n";

   svg << "<g transform=\"scale(1 1)\">\n";

   // Add title with filename and time
   svg << "<text x=\"200\" y=\"30\" font-family=\"sans-serif\" font-size=\"20\">"
      << controlVars.currentFilename << " &#160;&#160;&#160;&#160;&#160; Wall time passed: "
      << getElapsedTime() << " msec.</text>\n";

   // Add top legend
   int legendY = 60;
   std::vector<std::string> colors = { "lightblue", "olive", "green", "purple", "orange" };
   int colorIndex = 0;
   for (const auto& [distType, data] : results) {
      svg << "<text x=\"300\" y=\"" << legendY << "\" font-family=\"sans-serif\" font-size=\"20\" fill=\"" << colors[colorIndex] << "\"> " << distType << " " << getElapsedTime() << " msec </text>\n"
         << "<line x1=\"600\" y1=\"" << legendY << "\" x2=\"900\" y2=\"" << legendY << "\" fill=\"none\" stroke=\"" << colors[colorIndex] << "\" stroke-width=\"" << (colorIndex == 0 ? 12 : 3) << "\" stroke-dasharray=\"" << (colorIndex == 0 ? "10,5" : "5,10") << "\"></line>\n";
      legendY += 25;
      colorIndex++;
   }

   svg << "\n\n <!--DISTANCE PLOT BEGINS   mode = " << (controlVars.followerMode == FollowerMode::POINT ? "Single Point" : "Line") << "-->\n\n"
      << "<!-- the following will scale and place the distance plot -->\n"
      << "<g transform=\"translate(" << margin << ",900) scale(1 -1)\">\n";

   // Plot data
   colorIndex = 0;
   for (const auto& [distType, data] : results) {
      const auto& distfuncs = data.second;
      svg << "   <polyline fill=\"none\" stroke=\"" << colors[colorIndex % colors.size()]
         << "\" stroke-width=\"" << (colorIndex == 0 ? 12 : 3) << "\" stroke-dasharray=\"" << (colorIndex == 0 ? "10,5" : "5,10")
         << "\" points=\"";
      for (size_t i = 0; i < distfuncs.size(); ++i) {
         double x = static_cast<double>(i) * plotWidth / (static_cast<double>(maxSteps) - 1.0);
         double y = plotHeight * (distfuncs[i] - minY) / (maxY - minY);
         if (!std::isnan(y)) {
            svg << x << "," << y << " ";
         }
      }
      svg << "\" />\n\n";
      colorIndex++;
   }

   // Draw axes
   svg << "<!--Draw the X and Y axes -->\n"
      << "<polyline fill=\"none\" stroke=\"black\" stroke-width=\"6\" points=\""
      << plotWidth << ",0 0,0 0," << plotHeight << "\" />\n"
      << "<polyline fill=\"none\" stroke=\"black\" stroke-width=\"1\" points=\"0 "
      << plotHeight << " " << plotWidth << " " << plotHeight << " " << plotWidth << " 0\" />\n"
      << "<!-- -->\n\n";

   // Draw X axis tics and labels
   svg << "<!--Draw labels and tics for X axis -->\n\n";
   for (int i = 0; i <= 5; ++i) {
      double x = i * plotWidth / 5;
      int label = i * 20;
      svg << "<line fill=\"none\" stroke=\"black\" stroke-width=\"6\" x1=\"" << x << "\" y1=\"0\" x2=\"" << x << "\" y2=\"-20\" />\n"
         << "<text transform=\"scale(1,-1)\" font-family=\"sans-serif\" font-size=\"30\" x=\"" << (x - 25) << "\" y=\"45\">" << label << "</text>\n";
   }

   // Draw Y axis tics and labels
   svg << "\n\n<!--Draw labels and tics for Y axis -->\n\n";
   int numYTics = 8;
   for (int i = 0; i <= numYTics; ++i) {
      double y = i * plotHeight / numYTics;
      double label = minY + (i * (maxY - minY) / numYTics);
      svg << "<line fill=\"none\" stroke=\"black\" stroke-width=\"6\" x1=\"0\" y1=\"" << y << "\" x2=\"-20\" y2=\"" << y << "\" />\n"
         << "<text transform=\"scale(1,-1)\" text-anchor=\"end\" font-family=\"sans-serif\" font-size=\"30\" x=\"-25\" y=\"-" << (y - 10) << "\">" << std::fixed << std::setprecision(1) << label << "</text>\n";
   }

   svg << "-->\n</g>\n</g>\n";

   // Add control variables as comments
   svg << "<!-- Run Constants\n";
   std::ostringstream controlVarStream;
   controlVars.printControlVariables(controlVarStream);
   svg << controlVarStream.str() << "\n-->\n";

   svg << "</svg>";

   return svg.str();
}

std::pair<double, double> SVGGenerator::findMinMax(const std::map<std::string, std::pair<std::vector<S6>, std::vector<double>>>& results) {
   double minY = std::numeric_limits<double>::max();
   double maxY = std::numeric_limits<double>::lowest();

   for (const auto& [distType, data] : results) {
      const auto& distfuncs = data.second;
      for (const auto& dist : distfuncs) {
         if (!std::isnan(dist)) {
            minY = std::min(minY, dist);
            maxY = std::max(maxY, dist);
         }
      }
   }

   return { minY, maxY };
}

size_t SVGGenerator::findMaxSteps(const std::map<std::string, std::pair<std::vector<S6>, std::vector<double>>>& results) {
   size_t maxSteps = 0;
   for (const auto& [distType, data] : results) {
      maxSteps = std::max(maxSteps, data.second.size());
   }
   return maxSteps;
}

std::string SVGGenerator::getCurrentDate() {
   auto now = std::chrono::system_clock::now();
   auto in_time_t = std::chrono::system_clock::to_time_t(now);
   std::stringstream ss;
   ss << std::put_time(std::localtime(&in_time_t), "%b %d %Y");
   return ss.str();
}

std::string SVGGenerator::getElapsedTime() {
   // This is a placeholder. You'll need to implement actual timing in your main program.
   return "44";
}
