#ifndef _MOUSEMOVER_H_
#define _MOUSEMOVER_H_

#include <iostream>

class MouseMover
{
    public:
        ~MouseMover(void);

        void rock(void);

    public:

    private:
        //default constructor
        MouseMover();

        //copy constructor
        MouseMover(const MouseMover& other);

        //assignment operator
        MouseMover&  operator = (const MouseMover& other);

};

#endif