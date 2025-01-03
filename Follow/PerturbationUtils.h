#ifndef PERTURBAIONUTILS_H
#define PERTURBAIONUTILS_H

#ifndef PERTURBATION_UTILS_H
#define PERTURBATION_UTILS_H

#include <string>
#include <iostream>

struct PerturbationUtils {
   static constexpr int MIN_PERTURBATIONS = 1;
   static constexpr int MAX_PERTURBATIONS = 100;
   static constexpr double MIN_PERTURB_BY = 0.000001;
   static constexpr double MAX_PERTURB_BY = 1.0;
   static constexpr double DEFAULT_PERTURB_BY = 0.1;

   static int validatePerturbCount(const std::string& value) {
      try {
         int count = std::stoi(value);
         if (count < MIN_PERTURBATIONS || count > MAX_PERTURBATIONS) {
            std::cerr << ";Warning: Perturbation count " << count
               << " out of range [" << MIN_PERTURBATIONS
               << "," << MAX_PERTURBATIONS << "], using "
               << MIN_PERTURBATIONS << std::endl;
            return MIN_PERTURBATIONS;
         }
         return count;
      }
      catch (...) {
         std::cerr << ";Warning: Invalid perturbation count '" << value
            << "', using " << MIN_PERTURBATIONS << std::endl;
         return MIN_PERTURBATIONS;
      }
   }

   static double validatePerturbAmount(const std::string& value) {
      try {
         double amount = std::stod(value);
         if (amount < MIN_PERTURB_BY || amount > MAX_PERTURB_BY) {
            std::cerr << ";Warning: Perturb amount " << amount
               << " out of range [" << MIN_PERTURB_BY
               << "," << MAX_PERTURB_BY << "], using "
               << DEFAULT_PERTURB_BY << std::endl;
            return DEFAULT_PERTURB_BY;
         }
         return amount;
      }
      catch (...) {
         std::cerr << ";Warning: Invalid perturb amount '" << value
            << "', using " << DEFAULT_PERTURB_BY << std::endl;
         return DEFAULT_PERTURB_BY;
      }
   }
};

#endif // PERTURBATION_UTILS_H

#endif // PERTURBAIONUTILS_H