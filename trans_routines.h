#ifndef RAY_TRACER_TRANS_ROUTINES_H
#define RAY_TRACER_TRANS_ROUTINES_H

#include "defs.h"

void translate ( float x , float y , float z , mat4 res ) ;
void scale ( float x , float y , float z , mat4 res ) ;
void rotate_x ( float angle , mat4 res ) ;
void rotate_y ( float angle , mat4 res ) ;
void rotate_z ( float angle , mat4 res ) ;
void shear ( float x_y , float x_z , float y_x , float y_z , float z_x , float z_y , mat4 res ) ;

#endif //RAY_TRACER_TRANS_ROUTINES_H
