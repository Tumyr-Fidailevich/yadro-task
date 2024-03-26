#ifndef TAPE_FACTORY_H
#define TAPE_FACTORY_H

#include "pch.h"
#include "file_tape.h"
#include "tape_interface.h"

const auto TEMP_PATH = std::filesystem::current_path() / "temp";

class tape_factory 
{
public:
    virtual ~tape_factory() = default;

    virtual std::unique_ptr<tape_interface> create_tape(std::size_t size) const = 0;
};

class file_tape_factory : public tape_factory
{
public:
    file_tape_factory(const file_tape::config& cfg = {});
    std::unique_ptr<tape_interface> create_tape(std::size_t size) const override;
private:
    file_tape::config _cfg;
};

#endif // TAPE_FACTORY_h