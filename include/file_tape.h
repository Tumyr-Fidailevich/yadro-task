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

        explicit config(const std::filesystem::path& filepath);

        explicit config(const config& other) = default;

        std::chrono::milliseconds readTime{0};
        std::chrono::milliseconds writeTime{0};
        std::chrono::milliseconds moveTime{0};
        std::chrono::milliseconds rewindTime{0};

    protected:
        std::pair<std::string, int> get_keyword_and_value(const std::string& str, std::size_t equals_pos);

        inline void ltrim(std::string& s) noexcept;

        inline void rtrim(std::string& s) noexcept;

        inline void full_trim(std::string& s) noexcept;
    };

    file_tape(const std::filesystem::path& filepath, const config& cfg = {});

    int read() const override noexcept;

    void write(int value) override noexcept;

    bool move_left() const override noexcept;

    bool move_right() const override noexcept;    

    ~file_tape();

private:
    config _config;
    mutable std::fstream _file;
    static const std::size_t FILL_DIGITS_NUMBER;
    static const char FILL_CHAR;


    std::size_t _size;
    std::size_t _pos;
};

#endif // FILE_TAPE_H