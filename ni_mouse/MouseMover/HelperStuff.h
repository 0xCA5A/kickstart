#ifndef _HELPERSTUFF_H_
#define _HELPERSTUFF_H_


#include <stdio.h>
#include <errno.h>
#include <unistd.h>


#define die(str, args...) do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while(0)


#endif
