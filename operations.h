#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdarg.h>

#include "logo.h"
#include "globals.h"
#include "error.h"

void op_print(int argc ,...){
    va_list valist;

    /* initialize valist for num number of arguments */
    va_start(valist, argc);
    
    /* access all the arguments assigned to valist */
    for (int i = 0; i < argc; i++) {
        printf("%i ", va_arg(valist, int));
    }

    va_end(valist);

}

void movexy(int argc,...){
    va_list valist;

    /* initialize valist for num number of arguments */
    va_start(valist, argc);
    
    int new_turtle_x = va_arg(valist, int);
    int new_turtle_y = va_arg(valist, int);
    
    main_turtle.x = new_turtle_x;
    main_turtle.y = new_turtle_y;
    
    /* access all the arguments assigned to valist */
    va_end(valist);
}

#endif
