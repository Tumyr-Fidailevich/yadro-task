#include "file_tape.h"


const std::size_t file_tape::FILL_DIGITS_NUMBER = std::to_string(std::numeric_limits<int>::min()).size();
const char file_tape::FILL_CHAR = ' ';


file_tape::config::config(const std::string& filename) 
{
    std::fstream configFile(filename);
    readTime = std::chrono::milliseconds(0);
    writeTime = std::chrono::milliseconds(0);
    moveTime = std::chrono::milliseconds(0);
    rewindTime = std::chrono::milliseconds(0);
}

file_tape::file_tape(const std::string& filename, const config& cfg) : _file(filename, std::ios::in, std::ios::out), _config(cfg)
{
    
}

int file_tape::read() const
{
    return {};
}

void file_tape::write(int value)
{

}

bool file_tape::move_left() const
{
    return {};
}

bool file_tape::move_right() const
{
    return {};
}