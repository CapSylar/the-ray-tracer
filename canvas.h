#ifndef RAY_TRACER_CANVAS_H
#define RAY_TRACER_CANVAS_H

#include "defs.h"

struct canvas
{
    int row ;
    int col ;

    tuple* mem;

};

int init_canvas ( int row , int col ) ;
void canvas_write ( tuple color , int x , int y ) ;
void canvas_ppm (); // converts the canvas to ppm file format ;

#endif //RAY_TRACER_CANVAS_H