#ifndef RAY_TRACER_INTERFACE_H
#define RAY_TRACER_INTERFACE_H

#include "defs.h"

int init_interface( int width , int height ) ;
void update_interface() ;
void interface_write( tuple color , int x , int y ) ;
void close_interface() ;
int poll_inputs_exit() ;

#endif //RAY_TRACER_INTERFACE_H
