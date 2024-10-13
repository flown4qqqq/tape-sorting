#include <iostream>
#include <optional>
#include <getopt.h>
#include <vector>
#include <map>
#include <functional>

namespace NCommandLine {

void ShowHelp();



struct TMainProgrammSettings {
    bool Help {false};
    size_t MaxMemory {1024};
    std::optional<std::string> Infile;
    std::optional<std::string> Outfile;
};



template<typename TSettings>
class TOptionHandler {
private:
    typedef std::function<void(TSettings&, const std::string&)> TOptWithValueHandle;
    typedef std::function<void(TSettings&)> TOptHandle;
    typedef std::function<void(TSettings&, const std::string&)> TArgHandle;

    std::vector<option> opts;
    std::map<char, TOptHandle> optHandles;
    std::map<char, TOptWithValueHandle> optWithValuesHandles;
    std::map<size_t, TArgHandle> argHandles;
    std::map<std::string, char> shortNameByLong;
    std::string format = "";

public:
    void AddOption(char shortName, const std::string& fullName, TOptHandle&& handle);
    void AddOptionWithValue(char shortName, const std::string& fullName, TOptWithValueHandle&& handle);
    void AddArgument(size_t index, TArgHandle&& handle);
    void Handle(int argc, char* const argv[], TSettings& settings);
};

} // namespace NCommandLine
