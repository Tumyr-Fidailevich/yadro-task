#ifndef TAPE_INTERFACE_H
#define TAPE_INTERFACE_H


class tape_interface
{    
public:
    virtual int read() const = 0;

    virtual void write(int) = 0;

    virtual bool move_left() const = 0;

    virtual bool move_right() const = 0;

    void rewind() const
    {
        while(move_left());
    }

    virtual ~tape_interface() = default;
};

#endif // TAPE_INTERFACE_H