#include "cli.hpp"

namespace NCommandLine {

void ShowHelp() {
    std::string help =
R"(Usage: tape-sort [OPTION]... InputFile OutputFile
Sorts InputFile into OutputFile.

Options:
    -h, --help                    display this help and exit
    -m, --memory=coefficient      set RAM-coefficient, which means the maximum number of numbers that can be stored in RAM [default = 1024])";
    std::cout << help << std::endl;
}



template<typename TSettings>
void TOptionHandler<TSettings>::AddOption(char shortName, const std::string& fullName, TOptHandle&& handle) {
    opts.push_back({fullName.c_str(), no_argument, nullptr, shortName});
    optHandles[shortName] = handle;
    format += shortName;
    shortNameByLong[fullName] = shortName;
}

template<typename TSettings>
void TOptionHandler<TSettings>::AddOptionWithValue(char shortName, const std::string& fullName, TOptWithValueHandle&& handle) {
    opts.push_back({fullName.c_str(), required_argument, nullptr, shortName});
    optWithValuesHandles[shortName] = handle;
    format += shortName;
    format += ':';
    shortNameByLong[fullName] = shortName;
}

template<typename TSettings>
void TOptionHandler<TSettings>::AddArgument(size_t index, TArgHandle&& handle) {
    argHandles[index] = handle;
}

template<typename TSettings>
void TOptionHandler<TSettings>::Handle(int argc, char* const argv[], TSettings& settings) {
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
            std::string arg = optarg;
            if (arg[0] == '=') {
                arg = arg.substr(1);
            }

            optWithValuesHandles[optNow](settings, arg);
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

            std::string arg = argv[optind];
            argHandles[i](settings, arg);
        }
    }
}

template class TOptionHandler<TMainProgrammSettings>;

} // namespace NCommandLine
