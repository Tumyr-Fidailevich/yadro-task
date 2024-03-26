#ifndef TAPE_INTERFACE_H
#define TAPE_INTERFACE_H


class tape_interface
{    
public:
    virtual int read() const = 0;

    virtual void write(int) noexcept = 0;

    virtual bool move_left() const noexcept = 0;

    virtual bool move_right() const noexcept = 0;

    virtual void rewind() const noexcept = 0;

    virtual std::size_t size() const noexcept = 0;

    virtual std::size_t pos() const noexcept = 0;

    virtual ~tape_interface() = default;
};

#endif // TAPE_INTERFACE_H