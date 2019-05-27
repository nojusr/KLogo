#include <stdio.h>
#include <stdarg.h>

#include "logo.h"
#include "globals.h"
#include "operations.h"

struct operation ops[] = {
    {"print", 1, op_print}
    
};

void init_global_turtle(){
    main_turtle.x = screen_width/2;
    main_turtle.y = screen_height/2;
    
    main_turtle.direction = 0;
    
    main_turtle.pen_width = 1;
    main_turtle.pencolor_R = 0;
    main_turtle.pencolor_G = 0;
    main_turtle.pencolor_B = 0;
}

void execute_op(int op_count, char op_name[], int op_args[]){
    
    
    
}


int main(int argc, char *argv[]){
    //get operation count
    int op_len = sizeof(ops)/sizeof(struct operation);
    
    
    
    
    printf("amount of ops: %i", op_len);
    
}
