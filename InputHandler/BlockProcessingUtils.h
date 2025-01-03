#ifndef BLOCKPROCESSINGUTILS_H
#define BLOCKPROCESSINGUTILS_H

#include <cmath>
#include <string>
#include <iostream>

struct BlockProcessingUtils {
   static constexpr size_t MIN_BLOCKSTART = 0;
   static constexpr size_t MAX_BLOCKSTART = 1000000;
   static constexpr size_t MIN_BLOCKSIZE = 1;
   static constexpr size_t MAX_BLOCKSIZE = 20;
   static constexpr size_t DEFAULT_BLOCKSIZE = 10;

   static size_t validateBlockstart(const std::string& value) {
      try {
         long long start = std::stoll(value);
         if (start < 0) {
            std::cerr << ";Warning: Negative blockstart, using 0" << std::endl;
            return 0;
         }
         if (start > MAX_BLOCKSTART) {
            std::cerr << ";Warning: Blockstart too large, using " << MAX_BLOCKSTART << std::endl;
            return MAX_BLOCKSTART;
         }
         return static_cast<size_t>(start);
      }
      catch (...) {
         std::cerr << ";Warning: Invalid blockstart, using 0" << std::endl;
         return 0;
      }
   }

   static size_t validateBlocksize(const std::string& value) {
      try {
         int size = std::stoi(value);
         if (size < MIN_BLOCKSIZE || size > MAX_BLOCKSIZE) {
            std::cerr << ";Warning: Blocksize out of range [" << MIN_BLOCKSIZE
               << "," << MAX_BLOCKSIZE << "], using " << DEFAULT_BLOCKSIZE << std::endl;
            return DEFAULT_BLOCKSIZE;
         }
         return static_cast<size_t>(size);
      }
      catch (...) {
         std::cerr << ";Warning: Invalid blocksize, using " << DEFAULT_BLOCKSIZE << std::endl;
         return DEFAULT_BLOCKSIZE;
      }
   }
};

#endif // BLOCKPROCESSINGUTILS_H
