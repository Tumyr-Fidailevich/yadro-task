#ifndef FILE_TAPE_H
#define FILE_TAPE_H

#include "pch.h"
#include "tape_interface.h"

static const std::filesystem DEFAULT_TAPE_CONFIG_PATH;

class file_tape : public tape_interface
{
public:
    struct config
    {
        config() = default;

        explicit config(const std::filesystem::path& filepath = DEFAULT_TAPE_CONFIG_PATH);

        explicit config(const config& other) = default;

        std::chrono::milliseconds readTime{0};
        std::chrono::milliseconds writeTime{0};
        std::chrono::milliseconds moveTime{0};
        std::chrono::milliseconds rewindTime{0};
    };

    file_tape(const std::filesystem::path& filepath, const config& cfg = {});

    int read() const override noexcept;

    void write(int value) override noexcept;

    bool move_left() const override noexcept;

    bool move_right() const override noexcept;    

private:
    config _config;
    mutable std::fstream _file;
    static const std::size_t FILL_DIGITS_NUMBER;
    static const char FILL_CHAR;


    std::size_t _size;
    std::size_t _pos;
};

#endif // FILE_TAPE_H