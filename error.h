#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>

void throw_fatal_error (char type[], char desc[]){
    
    printf("Fatal %s: %s", type, desc);
    
    exit(1);
    
    
}

#endif
