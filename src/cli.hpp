#include <functional>
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <map>
#include <optional>

namespace NCommandLine {

void ShowHelp() {
    std::string help =
R"(Usage: tape-sort [OPTION]... InputFile OutputFile
Sorts InputFile into OutputFile.

Options:
    -h, --help     display this help and exit)";
    std::cout << help << std::endl;
}

struct TMainProgrammSettings {
    bool Help {false};
    std::optional<std::string> Infile;
    std::optional<std::string> Outfile;
};

template<typename TSettings>
class TOptionHandler {
private:
    typedef std::function<void(TSettings&)> TOptHandle;
    typedef std::function<void(TSettings&, std::string)> TArgHandle;

    struct TOption {
        enum TType {
            Short,
            Long
        };

        TType Type;
        std::string Name;
    };

    std::unordered_map<std::string, TOptHandle> optionHandles;
    std::map<size_t, TArgHandle> argumentHandles;
public:
    void AddOption(std::string name, TOptHandle handle) {
        optionHandles[name] = handle;
    }

    void AddArgument(size_t index, TArgHandle handle) {
        argumentHandles[index] = handle;
    }

    void TryHandle(size_t argc, const char* argv[], TSettings& settings) {
        const std::string programName = argv[0];

        std::vector<std::string> parsedArguments;
        parsedArguments.reserve(argc);
        std::vector<TOption> parsedOptions;
        parsedOptions.reserve(argc);

        for (size_t i = 1; i < argc; i++) {
            std::string arg(argv[i]);

            if (arg[0] == '-') {
                auto type = TOption::TType::Short;

                if (arg.size() > 1 && arg[1] == '-') {
                    type = TOption::TType::Long;
                }

                if (type == TOption::TType::Short) {
                    for (size_t j = 1; j < arg.size(); j++) {
                        parsedOptions.push_back(TOption{type, static_cast<std::string>("-") + arg[j]});
                    }
                } else {
                    parsedOptions.push_back(TOption{type, arg});
                }
            } else {
                parsedArguments.push_back(argv[i]);
            }
        }

        for (const auto& option : parsedOptions) {
            if (!optionHandles.contains(option.Name)) {
                std::string message = static_cast<std::string>("option ") + "\'" + option.Name + "\'" + " doesn\'t allow an argument";
                throw std::invalid_argument(message);
            }

            optionHandles[option.Name](settings);
        }

        for (size_t i = 0; i < parsedArguments.size(); i++) {
            const auto& argument = parsedArguments[i];

            if (!argumentHandles.contains(i)) {
                std::string message = "too much arguments";
                throw std::invalid_argument(message);
            }

            argumentHandles[i](settings, argument);
        }
    }
};

} // namespace NCommandLine
