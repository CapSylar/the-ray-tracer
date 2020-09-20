// this file contains the definitions of data structures
#ifndef RAY_TRACER_DEFS_H
#define RAY_TRACER_DEFS_H

typedef float mat4[16] ;
typedef float mat3[9] ;
typedef float mat2[4] ;

#define SPHERE_OBJECT 0
#define PLANE_OBJECT 1
#define CUBE_OBJECT 2
#define CYLINDER_OBJECT 3
#define CONE_OBJECT 4

enum pattern_type { PATTERN_CHECKER , PATTERN_GRADIENT , PATTERN_STRIPES , PATTERN_RING };

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

struct object ;

typedef struct pattern
{
    tuple (*pattern_at)( struct object* o , struct pattern* , tuple* ) ; // function pointer for patterns
    mat4 pattern_trans ;// controls the pattern
    //color1
    int isNested1 ;
    tuple color1 ;
    struct pattern* pattern1;
    // color2
    int isNested2;
    tuple color2 ;
    struct pattern* pattern2;

} pattern ;



typedef struct
{
    float ambient,diffuse,specular,shininess,reflective;
    float transparency,refractive_index ;
    tuple color ;

    int has_pattern ;
    pattern current_pattern ;

} material ;

typedef struct object
{
    int id ;
    int type;
    mat4 trans;
    material mat;

    // ugly booger
    float min,max ; // for cylinder, should be removed from here
    int closed; // if the caps are present or not
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
    tuple inner_point ; // slightly adjusted in the opp direction of the normal
    tuple reflectv ; // reflection vector from point
    float n1,n2 ; // for refraction

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
#define float_cmp(a,b) (( fabs((float)(a)-(float)(b)) < EPS ))
#define PI 3.14159265359f

#endif //RAY_TRACER_DEFS_H
