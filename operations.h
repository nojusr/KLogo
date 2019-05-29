#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "logo.h"
#include "globals.h"
#include "error.h"

void op_print(int argc , int args[]){

    /* access all the arguments assigned to valist */
    for (int i = 0; i < argc; i++) {
        printf("%i ", args[i]);
    }

}

void movexy(int argc, int args[]){

    int new_turtle_x = args[0];
    int new_turtle_y = args[1];
    
    main_turtle.x = new_turtle_x;
    main_turtle.y = new_turtle_y;
    
}

void setheading (int argc, int args[]){
    
    int new_turtle_heading = args[0];
    
    main_turtle.direction = new_turtle_heading;
    
}

void movehome (int argc, int args[]){
    int center[] = {screen_width/2, screen_height/2};
    movexy(2, center);
}

void forward (int argc, int args[]){
    
    double dist = (double)args[0];
    
    double angle_in_rad = (M_PI/180)*(double)main_turtle.direction;
    
    double newx = dist*sin(angle_in_rad);
    double newy = dist*cos(angle_in_rad);
    
    int newx_int = (int)newx;
    int newy_int = (int) newy;
    
    //TODO CHECK FOR OUT OF BOUNDS
    main_turtle.x = newx_int;
    main_turtle.y = newy_int;
    
}

void backward(int argc, int args[]){
    double dist = (double)args[0];
    
    double angle_in_rad = (M_PI/180)*(double)main_turtle.direction;
    
    double newx = dist*sin(angle_in_rad+M_PI);
    double newy = dist*cos(angle_in_rad+M_PI);
    
    int newx_int = (int)newx;
    int newy_int = (int) newy;
    
    //TODO CHECK FOR OUT OF BOUNDS
    main_turtle.x = newx_int;
    main_turtle.y = newy_int;
}

#endif
