#include <iostream>
#include <tape-file.hpp>
#include <cli.hpp>

namespace {

NCommandLine::TMainProgrammSettings GetSettings(size_t argc, const char* argv[]) {
    using TSettings = NCommandLine::TMainProgrammSettings;

    NCommandLine::TOptionHandler<TSettings> parser;
    parser.AddOption("--help", [](TSettings& settings) {
        settings.Help = true;
    });

    parser.AddOption("-h", [](TSettings& settings) {
        settings.Help = true;
    });

    parser.AddArgument(0, [](TSettings& settings, std::string value) {
        settings.Infile = value;
    });

    parser.AddArgument(1, [](TSettings& settings, std::string value) {
        settings.Outfile = value;
    });

    TSettings settings;

    try {
        parser.TryHandle(argc, argv, settings);
    } catch (std::invalid_argument ex) {
        std::string message = "";
        message += static_cast<std::string>("tape-sort: ") + ex.what() + "\n";
        message += "Try \'tape-sort --help\' for more information";
        throw std::invalid_argument(message);
    } catch (std::exception ex) {
        throw ex;
    }

    return settings;
}

}

signed main(int argc, const char* argv[]) {
    try {
        auto settings = GetSettings(static_cast<size_t>(argc), argv);

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
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
