#ifndef BLOCK_UTILS_H
#define BLOCK_UTILS_H

#include <iostream>
class BlockUtils {
public:

   BlockUtils(const bool webRun) {
      if (webRun) {

      }
      else
      {

      }
   }
   // Block processing members
   static constexpr size_t MIN_BLOCKSIZE = 1;
   static constexpr size_t MAX_BLOCKSIZE = 20;
   static constexpr size_t DEFAULT_BLOCKSIZE = MAX_BLOCKSIZE;
   size_t blockstart = 0;
   size_t blocksize = DEFAULT_BLOCKSIZE;


   void setBlockSize(const int size, const bool webRun) {
      long long val = static_cast<long long>(size);
      if (val <= 0) {
         std::cerr << ";Warning: Blocksize must be positive, using "
            << DEFAULT_BLOCKSIZE << std::endl;
         blocksize = DEFAULT_BLOCKSIZE;
      }
      else if (webRun && val > MAX_BLOCKSIZE) {
         std::cerr << ";Warning: Blocksize exceeds web limit, using "
            << MAX_BLOCKSIZE << std::endl;
         blocksize = MAX_BLOCKSIZE;
      }
      else {
         blocksize = static_cast<size_t>(val);
      }
   }

   void setBlockStart(const int start) {
      long long val = static_cast<long long>(start);
      if (val < 0) {
         std::cerr << ";Warning: Blockstart cannot be negative, using 0" << std::endl;
         blockstart = 0;
      }
      else {
         blockstart = start;
      }
   }

};

#endif // BLOCK_UTILS_H