#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

namespace
{
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

}