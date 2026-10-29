#ifndef WEBIO_H
#define WEBIO_H

#include <algorithm>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Maximum number of output files per web run. Web servers run out of disk
// space otherwise. WEBIO_BLOCKSIZELIMIT can lower this but never raise it.
// Desktop runs have no limit.
inline constexpr size_t WEB_MAX_BLOCKSIZE = 20;

class WebIO {
public:
   friend std::ostream& operator<< (std::ostream& o, const WebIO& bcf);
   WebIO(int argc, char* argv[], const std::string& progName, const size_t fileCount);
   std::string RecreateFileNames(const std::string& name);
   void GetWebBlockSize(int argc, char* argv[]);

   // Final authority on block size and start. Call after any values from
   // stdin controls have been copied into m_blockstart / m_blocksize.
   // In web runs the number of OUTPUT FILES is capped at WEB_MAX_BLOCKSIZE
   // (or lower, from WEBIO_BLOCKSIZELIMIT); with filesPerItem files written
   // per item, that allows WEB_MAX_BLOCKSIZE / filesPerItem items.
   // filesPerItem = 0 means the items write no files, so no file cap.
   // In all runs the block is clamped to itemCount.
   void ApplyBlockLimits(const size_t itemCount, const bool isWebRun,
      const size_t filesPerItem = 1);

   void CreateFilenamesAndLinks(const size_t n, const std::string& programName,
      const std::string& extension = "svg");
   void CompareHtmlFromWebIO(
      const std::string host,
      const std::string rawprefix,
      const std::string htmlprefix,
      const bool usetimestamp,
      const bool usehttps,
      const bool usetarget,
      const WebIO& webio);
   void SetTimeStamp(const bool set) { m_usetimestamp = set; }

public:
   std::string m_programName;
   bool m_hasWebInstructions;
   std::string m_host = "";
   std::string m_rawprefix = "";
   std::string m_htmlprefix = "";
   bool m_usetimestamp = true;
   bool m_usehttps = false;
   bool m_usetarget = true;
   const size_t m_fileNameCount;
   size_t m_blocksize = WEB_MAX_BLOCKSIZE;
   size_t m_blockstart = 0;

   std::vector<std::string> m_FullfileNameList;
   std::vector<std::string> m_basicfileNameList;
   std::vector<std::string> m_FileNameList;
};
class WebLimits {
public:

   friend std::ostream& operator<<(std::ostream& os, const WebLimits& wl) {
      os << ";WebLimits" << std::endl;
      os << ";   block size " << wl.blocksize << std::endl;
      os << ";   block start " << wl.blockstart << std::endl;
      os << ";   block size max " << wl.blocksizemax << std::endl;
      return os;
   }

   WebLimits() = default;

   void Update(const WebIO& wio) {
      blocksize = std::min(static_cast<int>(wio.m_blocksize), blocksizemax);
      blockstart = std::max(static_cast<int>(wio.m_blockstart), blockstart);
   }

   void Update(const int cvblocksize, const int cvblockstart) {  // for use with ControlVariables
      blocksize = std::min(blocksize, (std::min(static_cast<int>(cvblocksize), blocksizemax)));
      blockstart = std::max(static_cast<int>(cvblockstart), blockstart);
   }

   static std::string  CheckWebFileLimits(const int ntotalFiles, const bool hasWebInstructions,
      const int blocksize, const int blockstart) {
      std::stringstream ss;
      if (hasWebInstructions) {
         if ((ntotalFiles > blocksize) && (ntotalFiles - blockstart) > blocksize) {
            // limits load to blocksize svg files
            ss
               << ";the total number files generated in a single web" << std::endl
               << ";run is limited to blocksize (" << blocksize
               << "). If you need more, consider" << std::endl
               << ";running directly on a computer from a command interface." << std::endl
               << ";On the web the blocksize limit is usually 20." << std::endl
               << ";Try with a smaller number of files and/or perturbations" << std::endl
               << ";or do multiple runs using control variable blockstart" << std::endl << std::endl;
         }
         // exit(-1);
      }
      return ss.str();
   }

   static std::pair<int, int> GetProcessingLimits(
      const int blockstart,
      const int blocksize,
      const int numFiles,
      const int perturbations,
      const bool isWeb) {

      const int totalFiles = numFiles * perturbations;
      if (blockstart >= totalFiles) {
         throw std::runtime_error("; start point " + std::to_string(blockstart) +
            " is already past the end point " + std::to_string(totalFiles));
      }

      int newBlockSize = std::max(1, blocksize);
      newBlockSize = std::min(newBlockSize, totalFiles);
      const int newBlockStart = std::max(0, blockstart);
      int runEnd;

      if (isWeb) {
         newBlockSize = std::min(newBlockSize, static_cast<int>(WEB_MAX_BLOCKSIZE));
         runEnd = std::min(totalFiles, newBlockStart + newBlockSize);
      }
      else {
         runEnd = totalFiles;
      }

      return { newBlockStart, runEnd };
   }

   int GetBlockSize() const { return blocksize; }
   int GetBlockStart() const { return blockstart; }
   int GetGlockSizeMax() const { return blocksizemax; }

private:
   const int blocksizemax = static_cast<int>(WEB_MAX_BLOCKSIZE);
   int blocksize = blocksizemax;
   int blockstart = 0;
}; // end of class WebLimits

#endif // WEBIO_H