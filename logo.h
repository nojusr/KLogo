#ifndef LOGO_H
#define LOGO_H


#include <stdio.h>
#include <stdarg.h>

// procedure related limits
#define PROC_MAX_ARGS 100
#define PROC_MAX_ARG_NAME_LENGTH 63
#define PROC_MAX_AMOUNT 1000

// programming error defs
#define ERR_TYPE "Type Error"
#define ERR_ARG "Argument Error"
#define ERR_READ "Read Error"
#define ERR_SYNTAX "Syntax Error"

// used when throwing cli argument type errors
#define ERR_TYPE_INT "Integer"
#define ERR_TYPE_STRING "String"

typedef int (*math_function)(int left, int right);

struct math_primitive {
    char name;
    math_function function;
};


struct turtle {
    int x;
    int y;
    int direction; // 0 to 360
    int pen_width;
    
    int pendown;
    
    
    int pencolor_R;
    int pencolor_G;
    int pencolor_B;
};

typedef void (*func)(int argc, int args[]);

struct operation {
    char *name[255];
    int arg_count;
    func function;
};

struct procedure {
    char *name[255];
    int arg_count;
    int read_pos;
    
};

#endif 




