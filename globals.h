#ifndef GLOBALS_H
#define GLOBALS_H

// default values for globals

int screen_width = 1000;
int screen_height = 1000;

// if true (1), when turtle reaches edge, it will loop over to opposite edge
int loop_around_window = 1;

// filename to read
char filename[255] = "null";

struct turtle main_turtle;



#endif
