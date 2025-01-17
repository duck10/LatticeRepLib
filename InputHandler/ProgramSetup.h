#ifndef PROGRAM_SETUP_H
#define PROGRAM_SETUP_H

template<typename ControlType>
class ProgramSetup {
public:
   ProgramSetup(int argc, char* argv[], const std::string& programName, int webMode = 0)
      : m_webio(argc, argv, programName, webMode)
   {
      m_controls.setWebRun(m_webio.m_hasWebInstructions);
      m_inputList = InputHandler::handleInput(m_controls, m_webio);
      std::cout << m_controls << std::endl;

      setupBlockProcessing(argc, argv);
      setupFileNames();
      printBlockInfo();
   }

   void setupBlockProcessing(int argc, char* argv[]) {
      m_webio.GetWebBlockSize(argc, argv);
      m_webio.m_blocksize = m_controls.getBlockSize();
      m_webio.m_blockstart = m_controls.getBlockStart();
   }

   void setupFileNames() {
      m_webio.CreateFilenamesAndLinks(m_inputList.size(), m_controls.getPrefix());
   }

   void printBlockInfo() {
      std::cout << "; " << m_webio.m_programName << " block start " << m_controls.getBlockStart() << std::endl;
      std::cout << "; " << m_webio.m_programName << " block size " << m_controls.getBlockSize() << std::endl;

      for (size_t i = m_controls.getBlockStart();
         i < m_inputList.size() && i < m_controls.getBlockStart() + m_controls.getBlockSize();
         ++i) {
         std::cout << "; " << m_webio.m_programName << " graphics file(s) "
            << i + 1 << "  " << m_webio.m_FullfileNameList[i - m_controls.getBlockStart()] << std::endl;
      }
   }

   const WebIO& getWebIO() const { return m_webio; }
   const ControlType& getControls() const { return m_controls; }
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
private:
   WebIO m_webio;
   ControlType m_controls;
   std::vector<LatticeCell> m_inputList;
};



#endif // PROGRAM_SETUP_H

