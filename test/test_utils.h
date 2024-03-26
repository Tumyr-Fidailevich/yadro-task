#ifndef TEST_UTILS_H
#define TEST_UTILS_H


#include <fstream>
#include <string>
#include <filesystem>
#include <iomanip>

const auto TEST_TEMP_PATH = std::filesystem::current_path() / "test_temp";

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
    static std::size_t tape_count = 0;

    std::filesystem::create_directory(TEST_TEMP_PATH);

    std::filesystem::path tape_path(TEST_TEMP_PATH);

    tape_path /= "tape" + std::to_string(tape_count) + ".txt";

    std::ofstream file(tape_path);

    for(const auto& elem : elements)
    {
        file << std::setw(11) << std::setfill(' ') << elem;
    }

    file.close();

    tape_count++;
    return tape_path;
}

std::vector<int> generate_random_vector(std::size_t size, std::size_t seed) {
    std::vector<int> vec(size);

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dis(1, 100);

    for (std::size_t i = 0; i < size; ++i) {
        vec[i] = dis(gen);
    }

    return vec;
}

#endif // TEST_UTILS_H