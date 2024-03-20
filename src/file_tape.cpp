#include "file_tape.h"


const std::size_t file_tape::FILL_DIGITS_NUMBER = std::to_string(std::numeric_limits<int>::min()).size();
const char file_tape::FILL_CHAR = ' ';


file_tape::config::config(const std::filesystem::path& filepath) 
{
    std::ifstream configFile(filepath);

    if(!configFile.is_open())
    {
        throw std::runtime_error("Cannot open or create " + filepath.filename());
    }

    std::string line;
    bool is_corrupted = false;
    while (configFile >> line) {
        auto equals_pos = line.find("=");
        if(equals_pos == std::string::npos)
        {
            is_corrupted = true;
            continue;
        }else
        {
            auto [key, value] = get_keyword_and_value(line, equals_pos);

            if(value == -1) 
            {
                is_corrupted = true;
                continue;
            }

            if(key == "r" || key == "read")
            {
                readTime = value;
            }else if(key == "w" || key == "write")
            {
                writeTime = value;
            }else if(key == "mv" || key == "move")
            {
                moveTime = value;
            }else if(key == "rd" || key == "rewind")
            {
                rewindTime = value;
            }
        }
    }

    if(is_corrupted) std::cerr << "Corrupted config file. Possible unexpected results.";
}

std::pair<std::string, int> file_tape::config::get_keyword_and_value(const std::string& str, std::size_t equals_pos)
{
    auto key = str.substr(0, equals_pos);
    auto str_value = str.substr(equals_pos, str.length());
    full_trim(key);
    full_trim(str_value);

    int value;
    try
    {
        value = std::stoi(str_value);
    }catch(const std::invalid_argument& e)
    {
        value = -1;
    }
    
    return {key, value};
}

void file_tape::config::ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void file_tape::config::rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void file_tape::config::full_trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
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

file_tape::~file_tape()
{
    _file.close();
}