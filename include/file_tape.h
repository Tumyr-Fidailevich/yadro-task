#ifndef FILE_TAPE_H
#define FILE_TAPE_H

#include "pch.h"
#include "tape_interface.h"


class file_tape : public tape_interface
{
public:
    struct config
    {
        config() = default;

        config(const std::filesystem::path& filepath);

        config(const config& other) = default;

        std::chrono::milliseconds readTime{0};
        std::chrono::milliseconds writeTime{0};
        std::chrono::milliseconds moveTime{0};
        std::chrono::milliseconds rewindTime{0};

        static std::pair<std::string, int> get_keyword_and_value(const std::string& str, std::size_t equals_pos) noexcept;

        static void ltrim(std::string& s) noexcept;

        static void rtrim(std::string& s) noexcept;

        static void full_trim(std::string& s) noexcept;
    };

    file_tape(const std::filesystem::path& filepath, const config& cfg = {});

    file_tape(const std::filesystem::path& filepath, std::size_t tape_size, const config& cfg = {});

    file_tape(file_tape&& other) noexcept;

    int read() const override;

    void write(int value) noexcept override;

    bool move_left() const noexcept override;

    bool move_right() const noexcept override;    

    void rewind() const noexcept override;

    std::string get_str_repr() const noexcept;

    std::size_t size() const noexcept override;

    std::size_t pos() const noexcept override;

    ~file_tape() override;

protected:
    void update_pos() const noexcept;

private:
    config _config;
    mutable std::fstream _file;
    static const std::size_t FILL_DIGITS_NUMBER = 11;
    static const char FILL_CHAR = ' ';


    mutable std::size_t _size;
    mutable std::size_t _pos;
};

constexpr bool operator==(const file_tape::config& lhs, const file_tape::config& rhs)
{
    return lhs.readTime == rhs.readTime && lhs.writeTime == rhs.writeTime && lhs.rewindTime == rhs.rewindTime && lhs.moveTime == rhs.moveTime;
}
#endif // FILE_TAPE_H