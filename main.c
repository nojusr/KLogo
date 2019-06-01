#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "logo.h"
#include "globals.h"
#include "operations.h"
#include "cli_arg.h"

#define DEBUG 1


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
    {.name = "left", .arg_count = 1, .function = turn_left},
    {.name = "right", .arg_count = 1, .function = turn_right},
    {.name = "penup", .arg_count = 0, .function = penup},
    {.name = "pendown", .arg_count = 0, .function = pendown},
    {.name = "setw", .arg_count = 0, .function = pendown},

};

void print_turtle_dbg(){
    if (DEBUG == 1){
        printf("Turtle stats:\n");
        printf("X coord: %i, Ycoord: %i, Direction: %i\n", main_turtle.x, main_turtle.y, main_turtle.direction);
        printf("Pen width: %i, Pendown: %i\n", main_turtle.pen_width, main_turtle.pendown);
        printf("R: %i, G: %i, B: %i\n", main_turtle.pencolor_R, main_turtle.pencolor_G, main_turtle.pencolor_B);
        printf("-------------------------------------------------\n");
    }
    
}

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
        if (strcmp(*tmp.name, op_name) == 0){
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
        printf("OP_NAME: %s,\nOP_ARGCOUNT: %i,\nOP_FUNC_ADDR: %x \n", *op_to_execute.name, op_to_execute.arg_count, *op_to_execute.function);
        printf("executing....\n");
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
        if (strcmp(*tmp.name, op_name) == 0){
            op_to_find = tmp;
            op_found = 1;
            break;
        }
    }
    
    if (op_found == 0){
        throw_not_found_error(op_name);
        return -1;
    }
    
    return op_to_find.arg_count;
    
}



void get_string_inside_brackets(FILE *input, int *len, char output[], char start, char end){
    int depth = 0;
    *len = 0;
    char buf;
    for (;;){

        buf = fgetc(input);
        printf("buf: %c    depth: %d             len: %d\n", buf, depth, *len);    
        
        if (buf == -1){
            break;
        }
            
        if (depth == 0 && buf == start){
            depth += 1;
            continue;
        }
        
        if (depth == 1 && buf == end){
            break;
        }        
        if (buf == start){
            depth += 1;
        }
        
        if (buf == end){
            depth -= 1;
        }
        output[*len] = buf;
        *len += 1;
    }
    output[*len] ='\0';
    
}

void run_loop(int op_count, int len, int loopcounter, FILE *input);
void parse_next_word(int op_count, FILE *main_file, int main_scope);
//
void run_loop(int op_count, int len, int loopcounter, FILE *input){// runs a FILE* for *loopcounter* times
    
    int chk = 0;
    int last_chk = -1;
    for (int i = 0; i < loopcounter; i++){
        fseek(input, 0, SEEK_SET);
        for (;;){

            parse_next_word(op_count, input, 1);
            print_turtle_dbg();
            chk = ftell(input);
            
            
            printf("chk: %d, len: %d \n", chk, len);
            if (chk == len || chk > len){
                break;
            }
        }
        printf("loop %d out of %d\n", i, loopcounter);
    }
}


// main_scope defines if the parse func is reading at the very top,
// i.e is it allowed to define new functions?
void parse_next_word(int op_count, FILE *main_file, int main_scope){
    
    char word[255];
    
    fscanf(main_file, "%s", word);
    
    if (strcmp(word, "repeat") == 0){
        char* buffer = NULL;
        size_t bufferSize = 0;
        FILE* loop_contents = open_memstream(&buffer, &bufferSize);
        
        int loopcounter;
        fscanf(main_file, "%d", &loopcounter);// read loopcounter
        
        char stripped_contents[4096];//string for loop contents
        int cont_len = 0;
        get_string_inside_brackets(main_file, &cont_len, stripped_contents, '[', ']');
        
        fprintf(loop_contents, stripped_contents);//write char into FILE*
        
        if (DEBUG == 1){
            printf("found loop: repeating %d times\n", loopcounter);
            printf("loop contents: %s\n", stripped_contents);
        }
        
        
        run_loop(op_count, cont_len, loopcounter, loop_contents);
        return;
    }
    
    
    int arg_count = get_op_arg_count(op_count, word);
    int op_args[arg_count];
    int buf = 0;

    
    for (int i = 0; i < arg_count; i++){
        //PARSE EXPRESSIONS HERE
        fscanf(main_file, "%d", &buf);
        op_args[i] = buf;
    }
    
    if (DEBUG == 1){
        printf("OP_ARGS: ");
        for (int i = 0; i < arg_count; i++){
            printf("%i ", op_args[i]);
        }
        printf("\n");
    }
    
    execute_op(op_count, word, op_args);
    
    
    
}






// file parsing ideas:



int main(int argc, char *argv[]){
    //get operation count
    int op_len = sizeof(ops)/sizeof(struct operation);
    
    //set initial turtle values
    init_global_turtle();
    parse_cli_args(argc, argv);
    
    FILE *main_file;
    
    main_file = fopen(filename, "r");
    
    for (;;){
        if (feof(main_file)){
            break;
        }
        parse_next_word(op_len, main_file, 1);
        print_turtle_dbg();
    }
}
