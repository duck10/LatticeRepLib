#ifndef GLITCH_UTILS_H
#define GLITCH_UTILS_H

#include <string>
#include <iostream>

struct GlitchUtils {
   static constexpr double MIN_PERCENT = 0;
   static constexpr double MAX_PERCENT = 100;
   static constexpr double DEFAULT_PERCENT = 4.0;

   static double validateGlitchThresholdPercent(const std::string& value) {
      try {
         const double percent(std::stod(value));
         if (percent < MIN_PERCENT || percent > MAX_PERCENT) {  // Changed < to >
            std::cerr << ";Warning: Glitch threshold percent " << percent
               << " out of range [" << MIN_PERCENT << ","
               << MAX_PERCENT << "], using " << DEFAULT_PERCENT << std::endl;
            return DEFAULT_PERCENT;
         }
         return percent;
      }
      catch (const std::exception&) {
         std::cerr << ";Warning: Invalid glitch threshold value '"
            << value << "', using " << DEFAULT_PERCENT << std::endl;
         return DEFAULT_PERCENT;
      }
   }
};


#endif  // GLITCH_UTILS_H
