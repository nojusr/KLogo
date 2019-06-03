#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "logo.h"
#include "globals.h"
#include "operations.h"
#include "cli_arg.h"
#include "math_primitive.h"

#define DEBUG 0


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

struct math_primitive mathops[] = {
    {.name = '+', .function = add},
    {.name = '-', .function = sub},
    {.name = '/', .function = divide},
    {.name = '*', .function = mul},
    {.name = '^', .function = power},
    {.name = '%', .function = mod}
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

void remove_whitespace(char input[]){
    char no_whitespace[1024];
    int nwlen = 0;

    //remove all whitespace
    for (int i = 0; i < strlen(input); i++){
        if (isspace(input[i])){
            continue;
        }else{
            no_whitespace[nwlen] = input[i];
            nwlen++;
        }
    }
    no_whitespace[nwlen] = '\0';

    strcpy(input, no_whitespace);
}

void parse_parentheses(char input[]);
int parse_math_expression(char input[]);

void parse_parentheses(char input[]){

    int len = strlen(input);
    int starting_point = 0;
    int depth = 0;

    char buffer[1024] = {0};


    for (int i = 0; i < len; i++){

        if (input[i] == '('){

            if (depth == 0){
                starting_point = i;
            }

            depth++;

        }else if (input[i] == ')'){

            if (depth == 0){
                throw_fatal_error(ERR_SYNTAX, "Mismatched parentheses");
            } else if (depth == 1){

                char seperated_str[1024] = {0};
                strncpy(seperated_str, input+starting_point+1, i - starting_point-1);

                int output = parse_math_expression(seperated_str);//recursion happens here
                sprintf(buffer+strlen(buffer), "%d", output);
            }

            depth--;
        }else{
            if (depth == 0){
                sprintf(buffer+strlen(buffer), "%c", input[i]);
            }
        }

    }

    buffer[strlen(buffer)] = '\0';
    strcpy(input, buffer);
}

int get_mathop_len(){
    return sizeof(mathops)/sizeof(struct math_primitive);
}

struct math_primitive get_math_primitive(char symbol){
    int len = get_mathop_len();

    struct math_primitive chk;

    for (int i = 0; i < len; i++){
        struct math_primitive chk = mathops[i];
        if (chk.name == symbol){
            return chk;
        }
    }
    char err[50];
    sprintf(err, "Unknown math operator: %c", symbol);
    throw_fatal_error(ERR_SYNTAX, err);

}

int check_for_symbol(char input[], char symbol){
    for (int i = 0; i < strlen(input); i++){
        if (input[i] == symbol){
            return 1;
        }
    }
    return 0;
}

void execute_math_op(char input[], char symbol){
    struct math_primitive op_to_execute = get_math_primitive(symbol);

    char buffer[1024] = {0};
    int bufnum = 0;

    int left = 0;
    int right = 0;
    char operator = 0;

    int symbol_chk;

    symbol_chk = check_for_symbol(input, symbol);

    if (symbol_chk == 0){
        return;
    }

    FILE *stream;

    stream = fmemopen (input, strlen (input), "r");

    for (;;){

        if (feof(stream)){
            break;
        }

        symbol_chk = check_for_symbol(input, symbol);

        if (symbol_chk == 0){
            break;
        }


        fscanf(stream, "%d", &left);
        fscanf(stream, "%c", &operator);

        buffer[strlen(buffer)+1] = '\0';

        if (operator == symbol){
            fscanf(stream, "%d", &right);
            // if found, just write the rest of the array and say fuck it

            int out = op_to_execute.function(left, right);
            sprintf(buffer+strlen(buffer), "%d", out);


            // seek to beginning
            char ch;
            while ((ch = fgetc (stream)) != EOF){
                sprintf(buffer+strlen(buffer), "%c", ch);
            }

            fseek(stream, 0, SEEK_SET);
            left = 0;
            operator = 0;

            fclose(stream);


            strcpy(input, buffer);//copy over modified text

            stream = fmemopen (input, strlen (input), "r");//reset stream

            buffer[0] = '\0';// reset buffer
            continue;
        }else{
            sprintf(buffer+strlen(buffer), "%d%c", left, operator);
        }
        left = right;
    }
    fclose (stream);
    return;
}


int parse_math_expression(char input[]){

    remove_whitespace(input);
    parse_parentheses(input);

    execute_math_op(input, '^');
    execute_math_op(input, '*');
    execute_math_op(input, '/');
    execute_math_op(input, '+');
    execute_math_op(input, '-');

    int out = atoi(input);
    return out;

}


void run_loop(int op_count, int len, int loopcounter, FILE *input);
void parse_next_word(int op_count, FILE *main_file, int main_scope);

void run_loop(int op_count, int len, int loopcounter, FILE *input){// runs a FILE* *loopcounter* times

    int chk = 0;
    int last_chk = -1;
    for (int i = 0; i < loopcounter; i++){
        fseek(input, 0, SEEK_SET);
        for (;;){

            parse_next_word(op_count, input, 1);
            print_turtle_dbg();
            chk = ftell(input);

            if (chk == len || chk > len){
                break;
            }
        }

    }
}


// main_scope defines if the parse func is reading at the very top,
// i.e is it allowed to define new functions?
void parse_next_word(int op_count, FILE *main_file, int main_scope){

    char word[255];

    fscanf(main_file, "%s", word);

    if (word[0] == '#'){// comment handling (comments will work until new line)
        char buf;
        for (;;){
            buf = fgetc(main_file);
            if (buf == '\n'){
                fscanf(main_file, "%s", word);
                break;
            }
        }
    }


    if (strcmp(word, "to") == 0){// ignore all functions (for now)
        for (;;){
            fscanf(main_file, "%s", word);
            if (strcmp(word, "end") == 0){
                fscanf(main_file, "%s", word);
                break;
            }
        }
    }



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

    char expr[255];


    for (int i = 0; i < arg_count; i++){
        //PARSE EXPRESSIONS HERE
        fscanf(main_file, "%[^,\n]", expr);
        fscanf(main_file, "%*c");//burn off residue chars
        expr[strlen(expr)+1] = '\0';
        if (DEBUG == 1){
            printf("expression found: %s\n", expr);
        }
        int buf = parse_math_expression(expr);


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

        parse_next_word(op_len, main_file, 1);
        if (feof(main_file)){
            break;
        }
        print_turtle_dbg();
    }
}
