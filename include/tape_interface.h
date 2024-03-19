#ifndef TAPE_INTERFACE_H
#define TAPE_INTERFACE_H


class tape_interface
{    
public:
    virtual int read() const = 0;

    virtual void write(int) = 0;

    virtual bool moveLeft() const = 0;

    virtual bool moveRight() const = 0;

    void rewind() const
    {
        while(moveLeft());
    }

    virtual ~tape_interface() = default;
};

#endif // TAPE_INTERFACE_H