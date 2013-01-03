#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <iostream>

// stolen here
// stackoverflow.com/questions/2179623/how-does-qdebug-stuff-add-a-newline-automatically
struct debug
{
#if defined(DEBUG)
    template<typename T>
    std::ostream& operator<<(T const& a) const
    {
        std::cout << a;
        return std::cout;
    }
#else
    template<typename T>
    debug const& operator<<(T const&) const
    {
        return *this;
    }

    /* must handle manipulators (endl) separately:
     * manipulators are functions that take a stream& as argument and return a
     * stream&
     */
    debug const& operator<<(std::ostream& (*manip)(std::ostream&)) const
    {
        // do nothing with the manipulator
        return *this;
    }
#endif
};


#if defined(FUNCTIONTRACE)
    #define printFunctonNameMacro(void) do { \
            std::cout << "[printFunctonNameMacro] enter " << __PRETTY_FUNCTION__ << std::endl; \
        } while(0)
    #else
    #define printFunctonNameMacro(str, args...) do { \
        } while(0)
#endif


#endif