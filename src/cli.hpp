#include <iostream>
#include <optional>
#include <getopt.h>
#include <vector>
#include <map>
#include <functional>

namespace NCommandLine {

void ShowHelp() {
    std::string help =
R"(Usage: tape-sort [OPTION]... InputFile OutputFile
Sorts InputFile into OutputFile.

Options:
    -h, --help        display this help and exit
    -m, --memory      set RAM-coefficient, which means the maximum number of numbers that can be stored in RAM [default = 1024])";
    std::cout << help << std::endl;
}

struct TMainProgrammSettings {
    bool Help {false};
    size_t MaxMemory {1024};
    std::optional<std::string> Infile;
    std::optional<std::string> Outfile;
};

template<typename TSettings>
class TOptionHandler {
private:
    typedef std::function<void(TSettings&, std::string)> TOptWithValueHandle;
    typedef std::function<void(TSettings&)> TOptHandle;
    typedef std::function<void(TSettings&, std::string)> TArgHandle;

    std::vector<option> opts;
    std::map<char, TOptHandle> optHandles;
    std::map<char, TOptWithValueHandle> optWithValuesHandles;
    std::map<size_t, TArgHandle> argHandles;
    std::map<std::string, char> shortNameByLong;
    std::string format = "";
public:

    void AddOption(char shortName, std::string fullName, TOptHandle handle) {
        opts.push_back({fullName.c_str(), no_argument, nullptr, shortName});
        optHandles[shortName] = handle;
        format += shortName;
        shortNameByLong[fullName] = shortName;
    }

    void AddOptionWithValue(char shortName, std::string fullName, TOptWithValueHandle handle) {
        opts.push_back({fullName.c_str(), required_argument, nullptr, shortName});
        optWithValuesHandles[shortName] = handle;
        format += shortName;
        format += ':';
        shortNameByLong[fullName] = shortName;
    }

    void AddArgument(size_t index, TArgHandle handle) {
        argHandles[index] = handle;
    }

    void Handle(int argc, char* const argv[], TSettings& settings) {
        opts.push_back({nullptr, 0, nullptr, 0});

        int optNowTmp;
        int optionIndex;

        while ((optNowTmp = getopt_long(argc, argv, format.c_str(), opts.data(), &optionIndex)) != -1) {
            char optNow;
            if (optNowTmp == 0) {
                optNow = shortNameByLong[opts[static_cast<size_t>(optionIndex)].name];
            } else {
                optNow = static_cast<char>(optNowTmp);
            }

            if (optHandles.contains(optNow)) {
                optHandles[optNow](settings);
            } else if (optWithValuesHandles.contains(optNow)) {
                optWithValuesHandles[optNow](settings, optarg);
            } else {
                std::string message = "There is not option \'";
                message += static_cast<char>(optNow);
                message += "\'";
                throw std::invalid_argument(message);
            }
        }

        if (optind < argc) {
            for (size_t i = 0; optind < argc; i++, optind++) {
                if (!argHandles.contains(i)) {
                    std::string message = "There is no argument with index \'";
                    message += std::to_string(i);
                    message += "\'";
                    throw std::invalid_argument(message);
                }

                argHandles[i](settings, argv[optind]);
            }
        }
    }
};

} // namespace NCommandLine
