#ifndef CLI_ARG_H
#define CLI_ARG_H

#include <string.h>

#include "globals.h"
#include "error.h"
#include "logo.h"

int is_digit(char str[]){
    
    
    for (int i = 0; i < strlen(str); i++){
        if (str[i] > '9' || str[i] < '0'){
            return 0;
        }
    }
    return 1;
}

void parse_cli_args(int argc, char *argv[]){
    
    for (int i = 1; i < argc; i++){
        if (argv[i][0] == '-'){
            if (i+1 == argc){
                throw_no_val_provided_error(argv[i]);
            }
            
            switch (argv[i][1]){
                case 'h':
                    if (is_digit(argv[i+1]) == 1){
                        screen_height = atoi(argv[i+1]);
                        i++;
                        break;                        
                    }else{
                        throw_incorrect_arg_type_error(argv[i], ERR_TYPE_INT);
                        break;
                    }
                    
                    
                case 'w':
                    if (is_digit(argv[i+1]) == 1){
                        screen_width = atoi(argv[i+1]);
                        i++;
                        break;                        
                    }else{
                        throw_incorrect_arg_type_error(argv[i], ERR_TYPE_INT);
                        break;
                    }
                    
                default:
                    throw_unknown_arg_error(argv[i]);
            }
            
        }else{
            strcpy(filename, argv[i]);
        }
    }
    
    if (strcmp(filename, "null") == 0){
        throw_no_file_provided_error();
    }
}



#endif
