#include <iostream>
#include "circular_buffer.h"

class testObj
{
public:
    testObj()
    {
        std::cout << "Default constructed! p: " << p_  << std::endl;
    }

    testObj(int param1, int param2)
        : p_{param1 + param2}
    {
        std::cout << "Constructed with params! p: " << p_  << std::endl;
    }

    testObj(const testObj& otherToCpy)
    {
        p_ = otherToCpy.p_;
        std::cout << "Copy constructed! p: " << p_  << std::endl;
    }

    testObj(testObj&& otherToMove)
    {
        p_ = otherToMove.p_;
        otherToMove.p_ = -1;
        std::cout << "Move constructed! p: " << p_  << std::endl;
    }

    ~testObj()
    {
        std::cout << "Destroyed! p: " << p_ << std::endl;
    }
private:
    int p_{0};
};

int main()
{
    circular_buffer<testObj, 10> circ_buffer;

    circ_buffer.push(1, 2);
    testObj toMove{8, 9};
    circ_buffer.push(std::move(toMove));
    circ_buffer.push(testObj(1,3));
    testObj toCpy{10, 11};
    circ_buffer.push(toCpy);
    return 0;

    /*
     * Destroyed! p: 21
     * Destroyed! p: -1
     * Destroyed! p: 3
     * Destroyed! p: 17
     * Destroyed! p: 4
     * Destroyed! p: 21
     */
}
