#include <iostream>
#include <string>
#include <unordered_map>

static bool startsWith(const std::string& str, const std::string& searchString) {
   return str.find(searchString) == 0;
}

class LRL_GetCommandArgs {
   // written in consultation with bard.google.com (who made errors)

   // NOTE: the keywords should be in 1:1 correspondence to the member
   // variables, BUT THEY ARE NOT
   // so "--host" is used as a key to retrieve the value for m_host.
   // THUS, the key values occur in TWO PLACES in this function !!!!!!!
   const std::vector<std::string> keywordList{ "--host", "--rawprefix", "--htmlprefix", "--help" };
public:
   LRL_GetCommandArgs(int argc, char* argv[]) : m_argc(argc), m_argv(argv) {
      std::unordered_map<std::string, std::string> argMap;

      for (int i = 1; i < argc; ++i) {
         const std::string& arg = argv[i];
         for (std::string key : keywordList) {
            if (startsWith(arg, key)) {
               argMap[key] = arg;
               break;
            }
         }
      }

      m_host = argMap["--host"];
      m_rawprefix = argMap["--rawprefix"];
      m_htmlprefix = argMap["--htmlprefix"];
      m_help = argMap["--help"];
   }

   // Access functions for private members
   size_t size() const { return GetArgc(); }
   int GetArgc() const { return m_argc; }
   char** GetArgv() const { return m_argv; }
   char* GetArgv(const int n) const { return m_argv[n]; }
   std::string GetHost() const { return m_host; }
   std::string GetRawPrefix() const { return m_rawprefix; }
   std::string GetHtmlPrefix() const { return m_htmlprefix; }
   std::string GetHelp() const { return m_help; }

   double GetDoubleFromArgv(const size_t n) const {
      const std::string arg1 = m_argv[n];
      return std::atof(arg1.c_str());
   }

   int GetIntFromArgv(const size_t n) const {
      const std::string arg1 = m_argv[n];
      return std::atoi(arg1.c_str());
   }

   std::string GetStringFromArgv(const size_t n) const {
      const std::string arg = m_argv[n];
      return arg;
   }

private:
   int m_argc;
   char** m_argv;
   std::string m_host;
   std::string m_rawprefix;
   std::string m_htmlprefix;
   std::string m_help;
};