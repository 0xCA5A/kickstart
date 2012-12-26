#ifndef _HELPERSTUFF_H_
#define _HELPERSTUFF_H_


#include <stdio.h>
#include <errno.h>
#include <unistd.h>


#define die(str, args...) do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while(0)


#ifdef DEBUG
    #define printFunctonNameMacro(void) do { \
            std::cout << "[printFunctonNameMacro] enter " << __PRETTY_FUNCTION__ << std::endl; \
        } while(0)
    #else
    #define printFunctonNameMacro(str, args...) do { \
        } while(0)
#endif


#endif
