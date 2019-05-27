#ifndef LOGO_H
#define LOGO_H


#include <stdio.h>
#include <stdarg.h>

// procedure related limits
#define PROC_MAX_ARGS 100
#define PROC_MAX_ARG_NAME_LENGTH 63
#define PROC_MAX_AMOUNT 1000

// error defs
#define ERR_TYPE "Type Error"
#define ERR_ARG "Argument Error"
#define ERR_READ "Read Error"


struct math_primitive {
    char *name;
    void (*func)(int, int );// arguments
};


struct turtle {
    int x;
    int y;
    int direction; // 0 to 360
    int pen_width;
    
    int pencolor_R;
    int pencolor_G;
    int pencolor_B;
};


struct operation {
    char *name;
    int arg_count;
    void (*func)(int,... );
};

struct procedure {
    char *name;
    int arg_count;
    int read_pos;
    
};

#endif 




