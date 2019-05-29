#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>

// PROGRAMMING ERROR THROW FUNCTIONS
void throw_fatal_error (char type[], char desc[]){
    
    printf("Fatal %s: %s\n", type, desc);
    
    exit(1);
    
}

void throw_not_found_error(char op_name[]){
    
    printf("I don't know how to %s\n", op_name);
    
}


// CLI ARGUMENT THROW FUNCTIONS

void throw_unknown_arg_error (char argname[]){
    printf("Unknown argument: %s\n", argname);
    exit(1);
}

void throw_no_val_provided_error(char argname[]){
    printf("No value provided for argument: %s\n", argname);
    exit(1);
    
}

void throw_incorrect_arg_type_error(char argname[], char arg_type[]){
    printf("Failed to parse argument: %s requires a(n) %s\n", argname, arg_type);
    exit(1);
}

void throw_no_file_provided_error(){
    printf("No input file provided\n");
    exit(1);
}



#endif
