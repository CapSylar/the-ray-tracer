// this file contains the definitions of data structures
#ifndef RAY_TRACER_DEFS_H
#define RAY_TRACER_DEFS_H

typedef float mat4[16] ;
typedef float mat3[9] ;
typedef float mat2[4] ;

typedef struct
{
    float x,y,z,w ;
}tuple;

typedef struct
{
    tuple dir ; // direction
    tuple org ; // origin
} ray ;

typedef struct
{
    int count ;
    float xs[2] ;
} t_collec ;

typedef struct
{
    tuple position ;
    tuple intensity ;

} point_light ;

typedef struct
{
    float ambient,diffuse,specular,shininess ;
    tuple color ;
} material ;

typedef struct
{
    int id ;
    mat4 trans;
    material mat;
} object ;

typedef struct
{
    float t ;
    object obj;

} intersection ;

typedef struct
{
    int count ;
    intersection *xs ;

} inter_collec ;

typedef struct
{
    int obj_count ;
    object* objects ;
    point_light light ;

} world ;

typedef struct
{
    int inside ; // if it hits the inside of a shape
    tuple p_contact ; // point of contact
    float t ;
    object obj ;
    tuple eye_v ;
    tuple normal ;
    tuple adjusted_p ; // point that has been adjusted to remove the problem of self intersection for shadows

} contact_calc ;

typedef struct
{
    int h_size ; // horizontal size of the canvas
    int v_size ; // vertical size of the canvas
    float fov ; // field of view in radians
    float pixel_size ; // used to calculate

    // used soon
    float half_width;
    float half_height ;

    mat4 transform ;

} camera ;

#define EPS 0.00001f // good enough for me :)
#define float_cmp(a,b) (( fabs((a)-(b)) < EPS ))
#define PI 3.14159265359

#endif //RAY_TRACER_DEFS_H
