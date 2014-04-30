#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>


// source: http://stackoverflow.com/questions/1941307/c-debug-print-macros
#ifdef DEBUG
    #define DEBUG_PRINT(arg, ...) do { fprintf(stderr, "[DEBUG] %s:%d:%s: " arg "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); } while (0)
#else
    #define DEBUG_PRINT(...) do{ } while ( 0 )
#endif


#endif
