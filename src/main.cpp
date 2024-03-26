#include "file_tape.h"
#include "tape_sorting.h"
#include "tape_factory.h"
#include <cxxopts.hpp>
#include <csignal>


const std::size_t DEFAULT_BYTES = 40;

void signalHandler(int signum) {
    std::cout << "Shutdown client" << std::endl;
    exit(signum);
}


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "RU");

    std::signal(SIGINT, signalHandler);
    
    cxxopts::Options options("./tape", "This program sorts tapes consisting of integers from input tape to output tape");

    options.add_options()
        ("h,help", "Show help")
        ("i,input", "Input file", cxxopts::value<std::string>())
        ("o,output", "Output file", cxxopts::value<std::string>()->default_value("output.txt"))
        ("c,config", "Config file", cxxopts::value<std::string>())
        ("b,bytes", "Memory", cxxopts::value<int>()->default_value(std::to_string(DEFAULT_BYTES)))
        ("t,tapes", "Temporary tapes", cxxopts::value<int>());

    options.parse_positional({"input", "output"});

    options.positional_help("<input> <output>");

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            options.show_positional_help();
            std::cout << options.help() << std::endl;
            return 0;
        }

        std::string input_file, output_file, config_file;
        std::size_t bytes = DEFAULT_BYTES;

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
        }else
        {
            output_file = "output.txt";
        }

        if (result.count("config")) {
            config_file = result["config"].as<std::string>();
        }

        if (result.count("bytes"))
        {
            bytes = result["bytes"].as<std::size_t>();
            if(bytes == 0)
            {
                std::cerr << "Memory size cannot be negative or zero, set to default" << std::endl;
                bytes = DEFAULT_BYTES;
            }
        }

        file_tape::config tape_config;

        try
        {
            tape_config = file_tape::config(config_file);
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "Config set to default" << std::endl;
        }
        
        auto input_file_path = std::filesystem::current_path() / input_file;
        auto output_file_path = std::filesystem::current_path() / output_file;

        const auto source_tape = file_tape(input_file_path, tape_config);
        auto dst_tape = file_tape(output_file_path, source_tape.size(), tape_config);

        const auto factory = file_tape_factory(tape_config);

        if (result.count("tapes") && result["tapes"].as<int>() > 0)
        {
            std::cerr << "Number of temporary tapes cannot equals or less then zero, parameter ignored" << std::endl;
            tape_sorting(source_tape, dst_tape, factory, bytes, result["tapes"].as<int>());
        }else
        {
            tape_sorting(source_tape, dst_tape, factory, bytes);
        }

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
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}