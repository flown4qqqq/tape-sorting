#include <iostream>
#include <filesystem>

#include <tape-file.hpp>
#include <cli.hpp>

namespace {

NCommandLine::TMainProgrammSettings GetSettings(int argc, char* const argv[]) {
    using TSettings = NCommandLine::TMainProgrammSettings;

    NCommandLine::TOptionHandler<TSettings> parser;
    parser.AddOption('h', "help", [](TSettings& settings) {
        settings.Help = true;
    });

    parser.AddOptionWithValue('m', "memory", [](TSettings& settings, const std::string& value) {
        settings.MaxMemory = static_cast<size_t>(std::stoull(value));
    });

    parser.AddArgument(0, [](TSettings& settings, const std::string& value) {
        settings.Infile = value;
    });

    parser.AddArgument(1, [](TSettings& settings, const std::string& value) {
        settings.Outfile = value;
    });

    TSettings settings;

    try {
        parser.Handle(argc, argv, settings);
    } catch (const std::exception& ex) {
        std::string message = "";
        message += static_cast<std::string>("tape-sort: ") + ex.what() + "\n";
        message += "Try \'tape-sort --help\' for more information";
        throw std::invalid_argument(message);
    }

    return settings;
}

}

signed main(int argc, char* const argv[]) {
    try {
        auto settings = GetSettings(argc, argv);

        if (settings.Help) {
            NCommandLine::ShowHelp();
            return 0;
        }

        if (!settings.Infile.has_value() || !settings.Outfile.has_value()) {
            std::string message = 
R"(Please, set InputFile and OutputFile or
use tape-sort --help)";
            std::cerr << message << std::endl;
            return 1;
        }

        if (!std::filesystem::exists(settings.Infile.value())) {
            std::string message = "No such InputFile";
            std::cerr << message << std::endl;
            return 1;
        }

        if (!std::filesystem::exists(settings.Outfile.value())) {
            std::string message = "No such OutputFile";
            std::cerr << message << std::endl;
            return 1;
        }

        NTape::Sort(settings.Infile.value(), settings.Outfile.value(), settings.MaxMemory);
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
