#include "tape_factory.h"


file_tape_factory::file_tape_factory(const file_tape::config& cfg) : _cfg(cfg) {}

std::unique_ptr<tape_interface> file_tape_factory::create_tape(std::size_t size) const 
{
    static std::size_t count = 0;

    std::filesystem::create_directory(TEMP_PATH);

    std::filesystem::path tape_path(TEMP_PATH);

    tape_path /= "temp_tape" + std::to_string(count++) + ".txt";
    return std::make_unique<file_tape>(tape_path, size, _cfg);
}