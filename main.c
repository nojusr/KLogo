#include <stdio.h>
#include <stdarg.h>

#include "logo.h"
#include "globals.h"
#include "operations.h"
#include "cli_arg.h"

#define DEBUG 1

        //char err_desc[255];
        //sprintf(err_desc, "Expected %i argument(s) for function %s, got %i\n", op_to_execute.arg_count, *op_to_execute.name, n);
        //throw_fatal_error(ERR_ARG, err_desc);
//

struct operation ops[] = {
    {.name = "print", .arg_count = 1, .function = op_print},
    {.name = "setpos", .arg_count = 2, .function = movexy},
    {.name = "setxy", .arg_count = 2, .function = movexy},
    {.name = "setheading", .arg_count = 1, .function = setheading},
    {.name = "seth", .arg_count = 1, .function = setheading},
    {.name = "home", .arg_count = 0, .function = movehome},
    {.name = "forward", .arg_count = 1, .function = forward},
    {.name = "fw", .arg_count = 1, .function = forward},
    {.name = "backward", .arg_count = 1, .function = backward},
    {.name = "bw", .arg_count = 1, .function = backward},
};

void init_global_turtle(){
    main_turtle.x = screen_width/2;
    main_turtle.y = screen_height/2;
    
    main_turtle.direction = 0;
    
    main_turtle.pendown = 0;
    
    main_turtle.pen_width = 1;
    main_turtle.pencolor_R = 0;
    main_turtle.pencolor_G = 0;
    main_turtle.pencolor_B = 0;
    
    
}

// can only take in basic number args, expressions have to be processed in other funcs
void execute_op(int op_count, char op_name[], int op_args[]){
    
    struct operation op_to_execute;
    struct operation tmp;
    
    int op_found = 0;
    
    for (int i = 0; i < op_count; i++){
        tmp = ops[i];
        if (*tmp.name == op_name){
            op_to_execute = tmp;
            op_found = 1;
            break;
        }
    }
    

    if (op_found == 0){
        throw_not_found_error(op_name);
        return;
    }

    if (DEBUG == 1){
        printf("OP_NAME: %s,\nOP_ARGCOUNT: %i,\nOP_FUNC: %x \n", *op_to_execute.name, op_to_execute.arg_count, *op_to_execute.function);
        printf("-------------------------------------------------\n");
    }
    
    // execute
    op_to_execute.function(op_to_execute.arg_count,  op_args);
    
    
    
    
}

int get_op_arg_count(int op_count, char op_name[]){
    struct operation op_to_find;
    struct operation tmp;
    
    int op_found = 0;
    
    for (int i = 0; i < op_count; i++){
        tmp = ops[i];
        if (*tmp.name == op_name){
            op_to_find = tmp;
            op_found = 1;
            break;
        }
    }
    
    return op_to_find.arg_count;
    
}

void print_turtle_dbg(){
    if (DEBUG == 1){
        printf("Turtle stats:\n");
        printf("X coord: %i, Ycoord: %i, Direction: %i\n", main_turtle.x, main_turtle.y, main_turtle.direction);
        printf("Pen width: %i, Pendown: %i\n", main_turtle.pen_width, main_turtle.pendown);
        printf("R: %i, G: %i, B: %i\n", main_turtle.pencolor_R, main_turtle.pencolor_G, main_turtle.pencolor_B);
        printf("-------------------------------------------------\n");
    }
    
}


int main(int argc, char *argv[]){
    //get operation count
    int op_len = sizeof(ops)/sizeof(struct operation);
    
    //set initial turtle values
    init_global_turtle();
    parse_cli_args(argc, argv);
    
    
    
}
