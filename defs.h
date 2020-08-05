// this file contains the definitions of data structures
#ifndef RAY_TRACER_DEFS_H
#define RAY_TRACER_DEFS_H

typedef struct tuple
{
    float x,y,z,w ;
}tuple;

#define EPS 0.000001f // good enough for me :)
#define float_cmp(a,b) (( fabs((a)-(b)) < EPS ))

#endif //RAY_TRACER_DEFS_H
