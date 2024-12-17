// BlockLimits.h
#ifndef BLOCK_LIMITS_H
#define BLOCK_LIMITS_H

#include <string>

class BlockLimits {
public:
   static bool validateBlockSize(size_t size) {
      return size > 0 && size <= 20;  // Maximum block size is 20
   }

   static bool validateBlockStart(size_t start, size_t totalSize) {
      return start < totalSize;
   }

   static std::string getBlockValidationMessage(size_t blockStart, size_t blockSize, size_t totalSize) {
      if (!validateBlockSize(blockSize)) {
         return ";Warning: Block size must be between 1 and 20";
      }
      if (!validateBlockStart(blockStart, totalSize)) {
         return ";Warning: Block start point is beyond the end point";
      }
      if (blockStart + blockSize > totalSize) {
         return ";Warning: Block extends beyond available data";
      }
      return "";
   }
};

#endif // BLOCK_LIMITS_H