#ifndef TEST_UTILS_H
#define TEST_UTILS_H


#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>


std::string create_temp_config(const std::string& read_name, int read, const std::string& write_name, int write,const std::string& rewind_name, int rewind, const std::string& move_name, int move)
{
    std::string config_file_name{"test_config.cfg"};

    std::ofstream file(config_file_name);

    file << read_name << "=" << read << std::endl;
    file << write_name << "=" << write << std::endl;
    file << rewind_name << "=" << rewind << std::endl;
    file << move_name << "=" << move << std::endl;

    file.close();
    return config_file_name;
}

std::filesystem::path create_temp_tape(const std::vector<int>& elements)
{
    auto target_path = std::filesystem::current_path() / "temp";

    std::filesystem::create_directory(target_path);

    target_path /= "tape.txt";

    std::fstream file(target_path, std::ios::out);

    for(const auto& elem : elements)
    {
        file << std::setw(11) << std::setfill(' ') << elem;
    }

    file.close();
    return target_path;
}

#endif // TEST_UTILS_H