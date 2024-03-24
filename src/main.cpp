#include "file_tape.h"
#include "tape_sorting.h"
#include <cxxopts.hpp>
#include <csignal>


void signalHandler(int signum) {
    std::cout << "Shutdown client" << std::endl;
    exit(signum);
}


int main(int argc, char* argv[])
{
    std::signal(SIGINT, signalHandler);
    
    cxxopts::Options options("main", "Description");

    options.add_options()
        ("h,help", "Show help")
        ("i,input", "Input file", cxxopts::value<std::string>())
        ("o,output", "Output file", cxxopts::value<std::string>()->default_value("output.txt"))
        ("c,config", "config file", cxxopts::value<std::string>());

    options.parse_positional({"input", "output", "config"});

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        std::string input_file, output_file, config_file;

        if (result.count("input")) {
            input_file = result["input"].as<std::string>();
        } else if (result.arguments().size() >= 2) {
            input_file = result.arguments().at(1).as<std::string>();
        } else {
            std::cerr << "Error: No input file specified." << std::endl;
            return 1;
        }

        if (result.count("output")) {
            output_file = result["output"].as<std::string>();
        } else if (result.arguments().size() >= 3) {
            output_file = result.arguments().at(2).as<std::string>();
        }

        if (result.count("config")) {
            config_file = result["config"].as<std::string>();
        }

        file_tape::config tape_config;

        try
        {
            tape_config = file_tape::config(config_file);
        }
        catch(const std::runtime_error& e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Config set to default" << std::endl;
        }
        
        const auto source_tape = file_tape(input_file, tape_config);
        auto dst_tape = file_tape(output_file, source_tape.size(), tape_config);

        tape_sorting(source_tape, dst_tape);

        return 0;
    } catch (const cxxopts::OptionException& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    } catch (const std::ios_base::failure& e)
    {
        std::cerr << "Error, corrupted file tape: " << e.what() << std::endl;
        return 1;
    } catch (const std::invalid_argument& e)
    {
        std::cerr << "Error, cant read from file tape" << e.what() << std::endl;
        return 1;
    }
}