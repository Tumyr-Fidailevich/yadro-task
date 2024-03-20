#include "file_tape.h"


const std::size_t file_tape::FILL_DIGITS_NUMBER = std::to_string(std::numeric_limits<int>::min()).size();
const char file_tape::FILL_CHAR = ' ';
static const std::filesystem DEFAULT_TAPE_CONFIG_PATH = ".." / "default_tape.cfg";


file_tape::config::config(const std::filesystem::path& filepath) 
{
    std::ifstream configFile(filepath);

}

file_tape::file_tape(const std::filesystem::path& filepath, const config& cfg) : _file(filepath, std::ios::in, std::ios::out), _config(cfg), _pos(0)
{
    if(!_file.is_open())
    {
        throw std::runtime_error("Cannot open or create " + filepath.filename());
    }

    _size = (std::filesystem::file_size(filepath) + 1) / (FILL_DIGITS_NUMBER + 1);
}

int file_tape::read() const noexcept
{
    return {};
}

void file_tape::write(int value) noexcept
{

}

bool file_tape::move_left() const noexcept
{
    return {};
}

bool file_tape::move_right() const noexcept
{
    return {};
}