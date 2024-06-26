#include "file_tape.h"


file_tape::config::config(const std::filesystem::path& filepath) 
{
    std::ifstream configFile(filepath);

    if(!configFile.is_open())
    {
        throw std::runtime_error("Cannot open or create " + filepath.filename().u8string());
    }

    std::string line;
    bool is_corrupted = false;
    while (std::getline(configFile, line)) {
        auto equals_pos = line.find("=");
        if(equals_pos == std::string::npos)
        {
            is_corrupted = true;
            continue;
        }else
        {
            auto [key, value] = get_keyword_and_value(line, equals_pos);

            if(value < 0) 
            {
                is_corrupted = true;
                continue;
            }

            if(key == "r" || key == "read")
            {
                readTime = std::chrono::milliseconds(value);
            }else if(key == "w" || key == "write")
            {
                writeTime = std::chrono::milliseconds(value);
            }else if(key == "mv" || key == "move")
            {
                moveTime = std::chrono::milliseconds(value);
            }else if(key == "rd" || key == "rewind")
            {
                rewindTime = std::chrono::milliseconds(value);
            }
        }
    }

    if(is_corrupted) std::cerr << "Corrupted config file. Possible unexpected results.";
}

std::pair<std::string, int> file_tape::config::get_keyword_and_value(const std::string& str, std::size_t equals_pos) noexcept
{
    auto key = str.substr(0, equals_pos);
    auto str_value = str.substr(equals_pos + 1, str.length());
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

void file_tape::config::ltrim(std::string& s) noexcept
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

void file_tape::config::rtrim(std::string& s) noexcept
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void file_tape::config::full_trim(std::string& s) noexcept
{
    ltrim(s);
    rtrim(s);
}

file_tape::file_tape(const std::filesystem::path& filepath, const config& cfg) : _config(cfg), _pos(0)
{
    _file = std::fstream(filepath);

    if(!_file.is_open())
    {
        throw std::runtime_error("Cannot open " + filepath.filename().u8string());
    }

    _size = (std::filesystem::file_size(filepath) + 1) / FILL_DIGITS_NUMBER;

    if(_size == 0)
    {
        throw std::runtime_error("Tape cannot be empty: " + filepath.filename().u8string());
    }

    update_pos();
}

file_tape::file_tape(const std::filesystem::path& filepath, std::size_t tape_size, const config& cfg) : _config(cfg), _pos(0)
{
    if(tape_size == 0)
    {
        throw std::runtime_error("Tape cannot be empty: " + filepath.filename().u8string());
    }
    _size = tape_size;

    if(!std::filesystem::exists(filepath))
    {
        std::ofstream temp(filepath);
        if(!temp.is_open())
        {
            throw std::runtime_error("Cannot create " + filepath.filename().u8string());
        }
        temp.close();
    }
    
    _file = std::fstream(filepath);

    if(!_file.is_open())
    {
        throw std::runtime_error("Cannot open " + filepath.filename().u8string());
    }

    std::string spaces(_size * FILL_DIGITS_NUMBER, FILL_CHAR);

    _file << spaces;

    update_pos();
}

file_tape::file_tape(file_tape&& other) noexcept
{
    _file.close();
    _file = std::move(other._file);
    _config = std::move(other._config),
    _size = other._size;
    _pos = other._pos;
}

int file_tape::read() const
{
    std::this_thread::sleep_for(_config.readTime);

    std::string buffer;
    buffer.resize(FILL_DIGITS_NUMBER);
    _file.read(&buffer[0], FILL_DIGITS_NUMBER);
    buffer[FILL_DIGITS_NUMBER] = '\0';

    update_pos();

    if(buffer.find_first_not_of(" \t\n\v\f\r") == std::string::npos) return 0;
    int value = std::stoi(buffer);
    return value;
}

void file_tape::write(int value) noexcept
{
    std::this_thread::sleep_for(_config.writeTime);

    std::stringstream ss;
    ss << std::setw(FILL_DIGITS_NUMBER) << std::setfill(FILL_CHAR) << value;
    _file.write(ss.str().c_str(), FILL_DIGITS_NUMBER);

    update_pos();
}

bool file_tape::move_left() const noexcept
{
    if(_pos == 0) return false;
    std::this_thread::sleep_for(_config.moveTime);
    _pos--;
    update_pos();
    return true;
}

bool file_tape::move_right() const noexcept
{
    if(_pos == _size - 1) return false;
    std::this_thread::sleep_for(_config.moveTime);
    _pos++;
    update_pos();
    return true;
}

void file_tape::rewind() const noexcept
{
    std::this_thread::sleep_for(_config.rewindTime);
    _pos = 0;
    update_pos();
}

void file_tape::update_pos() const noexcept
{
    _file.seekg(_pos * FILL_DIGITS_NUMBER);
    _file.seekp(_pos * FILL_DIGITS_NUMBER);
}

std::string file_tape::get_str_repr() const noexcept
{
    std::stringstream ss;
    ss << _file.rdbuf();
    return ss.str();
}

std::size_t file_tape::size() const noexcept
{
    return _size;
}

std::size_t file_tape::pos() const noexcept
{
    return _pos;
}

file_tape::~file_tape()
{
    _file.close();
}