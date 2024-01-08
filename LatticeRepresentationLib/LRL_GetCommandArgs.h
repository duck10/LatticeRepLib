#include <iostream>
#include <string>
#include <unordered_map>

static bool startsWith(const std::string& str, const std::string& prefix) {
   return str.find(prefix) == 0;
}

class LRL_GetCommandArgs {
   // written in consultation with bard.google.com (who made errors)
public:
    LRL_GetCommandArgs(int argc, char* argv[]) : m_argc(argc), m_argv(argv) {
        std::unordered_map<std::string, std::string> argMap;

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (startsWith(arg, "--")) {
                const std::string value = (i + 1 < argc) ? argv[i + 1] : "";
                argMap[arg] = value;
                ++i;  // Skip the value for the next iteration
            }
        }

        m_host = argMap["--host"];
        m_rawprefix = argMap["--rawprefix"];
        m_htmlprefix = argMap["--htmlprefix"];
        m_help = argMap["--help"];
    }

    // Access functions for protected members
    int GetArgc() const { return m_argc; }
    char** GetArgv() const { return m_argv; }
    std::string GetHost() const { return m_host; }
    std::string GetRawPrefix() const { return m_rawprefix; }
    std::string GetHtmlPrefix() const { return m_htmlprefix; }
    std::string GetHelp() const { return m_help; }

    double GetDoubleFromArgv1() {
        const std::string arg1 = m_argv[1];
        return std::atof(arg1.c_str());
    }

    int GetIntFromArgv1() {
        const std::string arg1 = m_argv[1];
        return std::atoi(arg1.c_str());
    }

    std::string GetStringFromArgv1or2() {
        if (this->m_argc >= 2) {
            return this->m_argv[1];
        } else if (this->m_argc >= 3) {
            return this->m_argv[2];
        } else {
            return "";
        }
    }

private:
protected:
    int m_argc;
    char** m_argv;
    std::string m_host;
    std::string m_rawprefix;
    std::string m_htmlprefix;
    std::string m_help;
};