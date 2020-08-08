// this file contains the definitions of data structures
#ifndef RAY_TRACER_DEFS_H
#define RAY_TRACER_DEFS_H

typedef float mat4[16] ;
typedef float mat3[9] ;
typedef float mat2[4] ;

typedef struct tuple
{
    float x,y,z,w ;
}tuple;

typedef struct ray
{
    tuple dir ; // direction
    tuple org ; // origin
} ray ;

typedef struct t_collec
{
    int count ;
    float xs[2] ;
} t_collec ;

typedef struct point_light
{
    tuple position ;
    tuple intensity ;

} point_light ;

typedef struct material
{
    float ambient,diffuse,specular,shininess ;
    tuple color ;
} material ;

typedef struct object
{
    int id ;
    mat4 trans;
    material mat;
} object ;

typedef struct intersection
{
    float t ;
    object s;

} intersection ;

typedef struct inter_collec
{
    int count ;
    intersection xs[2] ;
} inter_collec ;

#define EPS 0.00001f // good enough for me :)
#define float_cmp(a,b) (( fabs((a)-(b)) < EPS ))
#define PI 3.14159265359

#endif //RAY_TRACER_DEFS_H
