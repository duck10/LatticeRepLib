#ifndef PROGRAM_SETUP_H
#define PROGRAM_SETUP_H

#include "Webio.h"
// Basic input handling
template<typename ControlType>
class BasicProgramInput {
public:
   BasicProgramInput(const std::string& programName, ControlType& controls)
      : m_controls(controls)
   {
      m_inputList = InputHandler::handleInput(m_controls);
   }

   const ControlType& getControls() const { return m_controls; }
   ControlType& getControls() { return m_controls; }
   const std::vector<LatticeCell>& getInputList() const { return m_inputList; }

private:
   ControlType& m_controls;  // Reference instead of object
   std::vector<LatticeCell> m_inputList;
};

// File block input handling
template<typename ControlType>
class FileBlockProgramInput {
public:
   FileBlockProgramInput(const std::string& programName, size_t blockStart, size_t blockSize, ControlType& controls)
      : m_blockStart(blockStart), m_blockSize(blockSize), m_programName(programName), m_controls(controls)
   {
      m_inputList = InputHandler::handleInput(m_controls);
      setupFileNames(m_controls.getHasWebInput());
   }

   void setupFileNames(const bool webRun) {
      char* dummy[1];
      WebIO webio(0, dummy, m_programName, m_inputList.size());
      webio.m_hasWebInstructions = webRun;
      webio.CreateFilenamesAndLinks(m_inputList.size(), m_controls.getPrefix());
      m_basicfileNameList = webio.m_basicfileNameList;
      m_FileNameList = webio.m_FileNameList;
      m_FullfileNameList = webio.m_FullfileNameList;
   }

   void printBlockInfo() const {
      std::cout << "; " << m_programName << " block start " << m_blockStart << std::endl;
      std::cout << "; " << m_programName << " block size " << m_blockSize << std::endl;

      for (size_t i = m_blockStart;
         i < m_inputList.size() && i < m_blockStart + m_blockSize;
         ++i) {
         std::cout << "; " << m_programName << " graphics file(s) "
            << i + 1 << "  " << m_FullfileNameList[i - m_blockStart] << std::endl;
      }
   }

   // Access methods
   const ControlType& getControls() const { return m_controls; }
   ControlType& getControls() { return m_controls; }
   const std::vector<LatticeCell>& getInputList() const { return m_inputList; }
   const std::vector<std::string>& getBasicFileNames() const { return m_basicfileNameList; }
   const std::vector<std::string>& getRawFileNames() const { return m_FileNameList; }
   const std::vector<std::string>& getFullFileNames() const { return m_FullfileNameList; }
   size_t getBlockStart() const { return m_blockStart; }
   size_t getBlockSize() const { return m_blockSize; }
   size_t getBlockEnd() const { return std::min(m_inputList.size(), m_blockStart + m_blockSize); }

   // Get input item at specific block index
   const LatticeCell& getInputAt(size_t blockIndex) const {
      if (blockIndex >= m_blockStart && blockIndex < getBlockEnd()) {
         return m_inputList[blockIndex];
      }
      throw std::out_of_range("Block index out of range");
   }

   // Get filename at specific block index
   const std::string& getFullFileNameAt(size_t blockIndex) const {
      size_t fileIndex = blockIndex - m_blockStart;
      if (fileIndex < m_FullfileNameList.size()) {
         return m_FullfileNameList[fileIndex];
      }
      throw std::out_of_range("File index out of range");
   }

   void writeOutputFile(const std::string& content, size_t index) const {
      if (content.empty()) return;

      const size_t fileIndex = index - m_blockStart;
      const std::string& fileName = getRawFileNames()[fileIndex];
      const std::string& fullFileName = getFullFileNames()[fileIndex];

      FileOperations::Write(fileName, content);
      std::cout << "; " << m_programName << " graphics file " << fullFileName << std::endl;
   }

private:
   ControlType& m_controls;
   std::vector<LatticeCell> m_inputList;
   size_t m_blockStart;
   size_t m_blockSize;
   std::string m_programName;
   std::vector<std::string> m_basicfileNameList;
   std::vector<std::string> m_FileNameList;
   std::vector<std::string> m_FullfileNameList;
};


// Web input handling
template<typename ControlType>
class WebProgramInput {
public:
   WebProgramInput(int argc, char* argv[], const std::string& programName, int webMode, ControlType& controls)
      : m_webio(argc, argv, programName, webMode), m_controls(controls)
   {
      m_controls.setWebRun(m_webio.m_hasWebInstructions);
      m_inputList = InputHandler::handleInput(m_controls, m_webio);
   }

   const WebIO& getWebIO() const { return m_webio; }
   const ControlType& getControls() const { return m_controls; }
   ControlType& getControls() { return m_controls; }
   const std::vector<LatticeCell>& getInputList() const { return m_inputList; }

private:
   WebIO m_webio;
   ControlType& m_controls;
   std::vector<LatticeCell> m_inputList;
};

// Web and file block input handling
template<typename ControlType>
class WebFileBlockProgramInput {
public:
   WebFileBlockProgramInput(int argc, char* argv[], const std::string& programName, int webMode, ControlType& controls)
      : m_webio(argc, argv, programName, webMode), m_controls(controls)
   {
      m_controls.setWebRun(m_webio.m_hasWebInstructions);
      m_inputList = InputHandler::handleInput(m_controls, m_webio);

      setupBlockProcessing(argc, argv);
      setupFileNames();
   }

   void setupBlockProcessing(int argc, char* argv[]) {
      m_webio.GetWebBlockSize(argc, argv);
      m_webio.m_blocksize = m_controls.getBlockSize();
      m_webio.m_blockstart = m_controls.getBlockStart();
   }

   void setupFileNames() {
      m_webio.CreateFilenamesAndLinks(m_inputList.size(), m_controls.getPrefix());
   }

   void printBlockInfo() const {
      std::cout << "; " << m_webio.m_programName << " block start " << m_controls.getBlockStart() << std::endl;
      std::cout << "; " << m_webio.m_programName << " block size " << m_controls.getBlockSize() << std::endl;

      for (size_t i = m_controls.getBlockStart();
         i < m_inputList.size() && i < m_controls.getBlockStart() + m_controls.getBlockSize();
         ++i) {
         std::cout << "; " << m_webio.m_programName << " graphics file(s) "
            << i + 1 << "  " << m_webio.m_FullfileNameList[i - m_controls.getBlockStart()] << std::endl;
      }
   }

   // Access methods
   const WebIO& getWebIO() const { return m_webio; }
   const ControlType& getControls() const { return m_controls; }  // for const objects
   ControlType& getControls() { return m_controls; }       

   const std::vector<LatticeCell>& getInputList() const { return m_inputList; }
   const std::vector<std::string>& getBasicFileNames() const { return m_webio.m_basicfileNameList; }
   const std::vector<std::string>& getRawFileNames() const { return m_webio.m_FileNameList; }
   const std::vector<std::string>& getFullFileNames() const { return m_webio.m_FullfileNameList; }
   size_t getBlockStart() const { return m_controls.getBlockStart(); }
   size_t getBlockSize() const { return m_controls.getBlockSize(); }

   // Iterator helper methods for block processing
   size_t getBlockEnd() const {
      return std::min(m_inputList.size(), getBlockStart() + getBlockSize());
   }

   // Get input item at specific block index
   const LatticeCell& getInputAt(size_t blockIndex) const {
      if (blockIndex >= getBlockStart() && blockIndex < getBlockEnd()) {
         return m_inputList[blockIndex];
      }
      throw std::out_of_range("Block index out of range");
   }

   // Get filename at specific block index
   const std::string& getFullFileNameAt(size_t blockIndex) const {
      size_t fileIndex = blockIndex - getBlockStart();
      if (fileIndex < m_webio.m_FullfileNameList.size()) {
         return m_webio.m_FullfileNameList[fileIndex];
      }
      throw std::out_of_range("File index out of range");
   }

   void writeOutputFile(const std::string& content, size_t index) const {
      if (content.empty()) return;

      const size_t fileIndex = index - getBlockStart();
      const std::string& fileName = getRawFileNames()[fileIndex];
      const std::string& fullFileName = getFullFileNames()[fileIndex];

      FileOperations::Write(fileName, content);
      std::cout << "; " << m_webio.m_programName << " graphics file " << fullFileName << std::endl;
   }

private:
   WebIO m_webio;
   ControlType& m_controls;
   std::vector<LatticeCell> m_inputList;
}; 

#endif // PROGRAM_SETUP_H

