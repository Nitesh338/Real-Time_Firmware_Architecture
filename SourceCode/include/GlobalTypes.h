#ifndef GLOBAL_TYPES_
#define GLOBAL_TYPES_
    #include"stdbool.h"

    typedef unsigned char   uint8;
    typedef signed char     sint8;
    typedef bool            boolean;
    typedef unsigned short  uint16;
    typedef signed short    sint16;
    typedef unsigned int    uint32;
    typedef signed int      sint32;
    typedef unsigned long   uint64;
    typedef signed long     sint64;
    #define TRUE            (uint8)1
    #define FALSE           (uint8)0
    #define b_TRUE          (boolean)1
    #define b_FALSE         (boolean)0

    #define E_OK            (boolean)0
    #define E_NOT_OK        (boolean)1
    #define NULL            (void*)0


#endif