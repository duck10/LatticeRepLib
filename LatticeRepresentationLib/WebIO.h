#ifndef WEBIO_H
#define WEBIO_H

#include <algorithm>

#include <sstream>
#include <string>
#include <utility>

class WebIO {
public:
   friend std::ostream& operator<< (std::ostream& o, const WebIO& bcf);
   WebIO(int argc, char* argv[], const std::string& progName, const size_t fileCount);
   std::string RecreateFileNames(const std::string& name);
   void GetWebBlockSize(int argc, char* argv[]);
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
   size_t m_blocksize = 20;
   size_t m_blockstart = 0;

   std::vector<std::string> m_FullfileNameList;
   std::vector<std::string> m_basicfileNameList;
   std::vector<std::string> m_FileNameList;
};
class WebLimits {
public:

   friend std::ostream& operator<<(std::ostream& os, const WebLimits& wl) {
      os << ";WebLimits\n";
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
               << ";the total number files generated in a single web\n"
               << ";run is limited to blocksize (" << blocksize
               << "). If you need more, consider\n"
               << ";running directly on a computer from a command interface.\n"
               << ";On the web the blocksize limit is usually 20.\n"
               << ";Try with a smaller number of files and/or perturbations\n"
               << ";or do multiple runs using control variable blockstart\n\n";
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
         newBlockSize = std::min(newBlockSize, 20);
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
   const int blocksizemax = 20;
   int blocksize = blocksizemax;
   int blockstart = 0;
}; // end of class WebLimits

#endif // WEBIO_H