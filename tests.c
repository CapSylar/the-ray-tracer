/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "tests.check" instead.
 */

#include <check.h>

#line 1 "tests.check"
#include "defs.h"
#include "factories.h"
#include "matrix_routines.h"
#include "trans_routines.h"
#include "ray.h"
#include "world_view.h"

struct canvas
{
    int row ;
    int col ;

    tuple* mem;

};

extern struct canvas canvas ;

tuple x = ( tuple ) { 3 , 1 , 1 , 1 } ;
tuple y = ( tuple ) { 3 , 2 , 0 , 0 } ;
tuple z = ( tuple ) { 5 , 3 , 6 , 1 } ;
tuple c = ( tuple ) { 7 , 3 , 2 , 0 } ;
tuple cc = ( tuple ) { 2 , 2 , 2 , 2 };
tuple a = ( tuple ) { 1 , 2 , 3 } ;
tuple b = ( tuple ) { 2 , 3 , 4 } ;

START_TEST(test_tuple_creation)
{
#line 28
    tuple test = get_point(3,1,1) ;
    fail_unless( compare_tuple( &test, &x ) , "failure in point creation") ;
    test = get_vector(3,2,0) ;
    fail_unless( compare_tuple( &test, &y ) , "failure in vector creation") ;

}
END_TEST

START_TEST(tuple_operation)
{
#line 34
    tuple res = ( tuple ) { 12 , 6 , 8 , 1 } ;
    tuple got = add_tuples( &z , &c ) ;
    fail_unless ( compare_tuple( &got , &res ) , "failure in tuple addition" ) ;
    tuple res2 = ( tuple ) { 0 , -1 , 1 , 1 } ; // x - y
    got = sub_tuples ( &x , &y) ;
    fail_unless ( compare_tuple( &res2 , &got ) , "failure in tuple subtraction" ) ;
    res = ( tuple ) { -3 , -1, -1 , -1} ;
    neg_tuple (&res) ;
    fail_unless ( compare_tuple( &x , &res ) , "failure in tuple negation ") ;
    res = ( tuple ) { 14 , 6 , 4 , 0 } ;
    got = c ;
    mult_scalar_tuple ( &got , 2 ) ;
    fail_unless ( compare_tuple (&res , &got) , "failure in tuple by scalar multiplication ") ;
    float hello = 3.464101615 ;
    float xx = mag_tuple ( &x ) ;
    fail_unless ( float_cmp ( xx , hello ) , "failure in calculating magnitude of tuple") ;
    res = ( tuple ) { 0.5 , 0.5 , 0.5 , 0.5 } ;
    got = cc ;
    normalize_tuple (&got) ;
    fail_unless ( compare_tuple (&res , &got) , "failure in normalizing the tuples") ;
    res = ( tuple ) { -1 , 2 , -1 , 0 } ;
    got = cross_tuples ( &a , &b );
    fail_unless ( compare_tuple (&res, &got) , "failure in cross product of tuples" );
    res = ( tuple ) { 1 , -2 , 1 , 0 };
    got = cross_tuples ( &b , &a ) ;
    fail_unless ( compare_tuple ( &res , &got) , "failure in cross product of tuples" ) ;

}
END_TEST

START_TEST(color_operations)
{
#line 62
    // only test the hadamard product, all other function work
    tuple color1 = get_color ( 0.5 , 1 , 0 ) ;
    tuple color2 = get_color ( 1 , 0.25 , 0 ) ;
    tuple got = hadamard_prod( &color1 , &color2 ) ;
    tuple res = ( tuple ) { 0.5 , 0.25 , 0 } ;
    fail_unless ( compare_tuple ( &res , &got ) , "hadamard product failed") ;


}
END_TEST

START_TEST(matrix_operations)
{
#line 71
    mat2 x = { 1 , 1 , 1 , 1 } ;
    mat2 y = { 1 , 1 , 1 , 1 } ;

    fail_unless( compare_mat2( x , y ) == 0 , "failure in matrix comparison") ;
    y[1] = 0 ;
    fail_unless ( compare_mat2(x,y) == 1 , "failure in matrix comparison" ) ;

    mat4 xx = { 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 8 , 7 , 6 , 5 , 4 , 3 , 2 } ;
    mat4 yy = { -2 , 1 , 2 , 3 , 3 , 2 , 1 , -1 , 4 , 3 , 6 , 5 , 1 , 2 , 7 , 8 };
    mat4 res = { 20 , 22 , 50 , 48 , 44 , 54 , 114 , 108 , 40 , 58 , 110 , 102 , 16 , 26 , 46 , 42 };

    mat4 got;
    multiply_mat4 ( xx , yy , got ) ;
    fail_unless ( compare_mat4( got , res ) == 0 , "failure in matrix multiplication" ) ;

    mat4 test = { 1 , 2 , 3 , 4 , 2 , 4 , 4 , 2 , 8 , 6 , 4 , 1 , 0 , 0 , 0 , 1 } ;
    tuple xxx = ( tuple ) { 1 , 2 , 3 , 1 } ;
    tuple res5 = ( tuple ) { 18 , 24 , 33 , 1 } ;
    tuple got2 ;
    multiply_mat4_tuple ( test , &xxx , &got2 );

    fail_unless ( compare_tuple( &got2 , &res5 ) , "failure in matrix and tuple multiplication" ) ;

    mat4 n = { 0 , 9 , 3 , 0 , 9 , 8 , 0 , 8 , 1 , 8 , 5 , 3 , 0 , 0 , 5 , 8 };
    mat4 m = { 0 , 9 , 1 , 0 , 9 , 8 , 8 , 0 , 3 , 0 , 5 , 5 , 0 , 8 , 3 , 8 };

    transpose_mat4( n , got );

    fail_unless ( compare_mat4( m , got ) == 0 , "failure in transposing matrix" ) ;

    mat2 deter = { 1 , 5 , -3 , 2 } ;
    fail_unless ( float_cmp( 17 , determ_mat2( deter ) ) , "failure in calculating the 2x2 matrix determinant" ) ;

    mat3 i = { 1 , 5 , 0 , -3 , 2 , 7 , 0 , 6 , -3 } ;
    mat2 got3 ;
    mat2 o = { -3 , 2 , 0 , 6 };

    submat_mat3 ( i , got3 , 0 , 2 ) ;

    fail_unless ( compare_mat2( got3 , o) == 0, "failure in calculating the submatrix of a 3x3 matrix" ) ;

    mat3 k = { -6 , 1 , 6 , -8 , 8 , 6 , -7 , -1 , 1 } ;
    mat4 l = { -6 , 1 , 1 , 6 , -8 , 5 , 8 , 6 , -1 , 0 , 8 , 2 , -7 , 1 , -1 , 1 } ;

    mat3 got10;
    submat_mat4 ( l , got10 , 2 ,1  ) ;

    fail_unless ( compare_mat3 ( got10 , k ) == 0 , " failure in calculating the submatrix of a 4x4 matrix" ) ;

    mat3 b = { 3 , 5 , 0 , 2 , -1 , -7 , 6 , -1 , 5 } ;
    fail_unless ( float_cmp(  minor_mat3 ( b , 1 , 0 ) , 25 )  , " failure in calculating the minor of a 3x3 matrix" ) ;

    fail_unless ( float_cmp ( cofactor_mat3 ( b , 1 , 0 ) , -25 ) , "failure in calculating the cofactor of a 3x3 matrix" ) ;

    mat3 yyyy = { 1 , 2 , 6 , -5 , 8 , -4 , 2 , 6 , 4 } ;
    fail_unless ( float_cmp ( cofactor_mat3(yyyy , 0 , 0) , 56 ) , "failure in calculating the cofactor of a 3x3 matrix" ) ;
    fail_unless ( float_cmp ( cofactor_mat3(yyyy , 0 , 1) , 12 ) , "failure in calculating the cofactor of a 3x3 matrix" ) ;
    fail_unless ( float_cmp ( cofactor_mat3(yyyy , 0 , 2) , -46 ) , "failure in calculating the cofactor of a 3x3 matrix" ) ;

    fail_unless ( float_cmp ( determ_mat3(yyyy) , -196 ) , "failure in calculating the determinant of a 3x3 matrix" ) ;

    mat4 bbbb = { -2 , -8 , 3 , 5 , -3 , 1 , 7 , 3 , 1 ,2 , -9 , 6 , -6 , 7 , 7 , -9 } ;

    fail_unless ( float_cmp ( cofactor_mat4(bbbb,0,0) , 690 ) , "failure in calculating the cofactor of a 4x4 matrix" ) ;
    fail_unless ( float_cmp ( cofactor_mat4(bbbb,0,1) , 447 ) , "failure in calculating the cofactor of a 4x4 matrix" ) ;
    fail_unless ( float_cmp ( cofactor_mat4(bbbb,0,2) , 210 ) , "failure in calculating the cofactor of a 4x4 matrix" ) ;
    fail_unless ( float_cmp ( cofactor_mat4(bbbb,0,3) , 51 ) , "failure in calculating the cofactor of a 4x4 matrix" ) ;

    fail_unless ( float_cmp ( determ_mat4(bbbb) , -4071 ) , "failure in calculating the determinant of a 4x4 matrix" ) ;

    mat4 zzzz = { 8 , -5 , 9 , 2 , 7 , 5 , 6 , 1 , -6 , 0 , 9 , 6 , -3 , 0 , -9 , -4 } ;
    mat4 erre ;
    inverse_mat4 ( zzzz , erre );
    fail_unless ( float_cmp ( erre[0] , -0.15385 )  , "failure in 4x4 matrix inversion");
    fail_unless ( float_cmp ( erre[3] , -0.53846 ) , "failure in 4x4 matrix inversion");
    fail_unless ( float_cmp ( erre[7] , 0.92308 ) == 0 , "failure in 4x4 matrix inversion");

    mat4 jounieh = { 3,-9,7,3,3,-8,2,-9,-4,4,4,1,-6,5,-1,1} ; // A
    mat4 beirut = { 8,2,2,2,3,-1,7,0,7,0,5,4,6,-2,0,5} ; // B

    mat4 jbeil; // C
    multiply_mat4 ( jounieh , beirut , jbeil );
    mat4 batroun ; // INV(B)
    inverse_mat4( beirut , batroun ) ;
    mat4 dahye;
    multiply_mat4 ( jbeil , batroun , dahye ) ;

    fail_unless ( compare_mat4( dahye , jounieh ) == 0 , "failure: something wrong with the matrix routines") ;


}
END_TEST

START_TEST(transformations)
{
#line 162
    mat4 x ;
    translate ( 5 , -3 , 2 , x ) ;
    tuple point = get_point( -3 ,4 , 5 ) ;
    tuple got ;
    tuple res = get_point ( 2 , 1 , 7 );

    multiply_mat4_tuple( x , &point , &got ) ;
    fail_unless (  compare_tuple ( &got , &res ) , "failure in translating a point" ) ;

    tuple vector = get_vector ( -3,4,5 ) ;
    multiply_mat4_tuple ( x , &vector , &got ) ;
    res = get_vector ( -3,4,5 ) ;

    fail_unless ( compare_tuple ( &got , &vector ) , "failure in NOT translating a vector" ) ;

    scale ( -1 , 1, 1 , x ) ;
    point = get_point( 2 , 3 , 4 ) ;

    multiply_mat4_tuple ( x , &point , &got ) ;
    res = get_point ( -2 ,3  ,4 );

    fail_unless ( compare_tuple (&res , &got) , "failure in scaling a point or vector " ) ;

    vector = get_vector ( 2 , 3 , 4 ) ;
    multiply_mat4_tuple ( x , &vector , &got ) ;
    res = get_vector ( -2 ,3  ,4 );

    fail_unless ( compare_tuple (&res , &got) , "failure in scaling a point or vector " ) ;

    // ROTATION AROUND X

    rotate_x ( PI / 2 , x) ;
    point = get_point ( 0 , 1 , 0 ) ;
    multiply_mat4_tuple ( x , &point , &got );
    res = get_point ( 0 , 0 , 1 ) ;

    fail_unless ( compare_tuple ( &res , &got ) , "failure in rotating the point around the x axis" ) ;

    rotate_x ( PI / 4 , x) ;
    point = get_point ( 0 , 1 , 0 ) ;
    multiply_mat4_tuple ( x , &point , &got );
    res = get_point ( 0 , 0.7071067 , 0.7071067 ) ;

    fail_unless ( compare_tuple ( &res , &got ) , "failure in rotating the point around the x axis" ) ;

    // ROTATION AROUND Y

    rotate_y ( PI / 2 , x ) ;
    point = get_point ( 0 , 0 , 1 ) ;
    multiply_mat4_tuple ( x , &point , &got );
    res = get_point ( 1 , 0 , 0 ) ;

    fail_unless ( compare_tuple ( &res , &got ) , "failure in rotating the point around the y axis" ) ;

    rotate_y ( PI / 4 , x) ;
    point = get_point ( 0 , 0 , 1 ) ;
    multiply_mat4_tuple ( x , &point , &got );
    res = get_point ( 0.7071067 , 0 , 0.7071067 ) ;

    fail_unless ( compare_tuple ( &res , &got ) , "failure in rotating the point around the y axis" ) ;

    // ROTATION AROUND Z

    rotate_z ( PI / 2 , x ) ;
    point = get_point ( 0 , 1 , 0 ) ;
    multiply_mat4_tuple ( x , &point , &got );
    res = get_point ( -1 , 0 , 0 ) ;

    fail_unless ( compare_tuple ( &res , &got ) , "failure in rotating the point around the z axis" ) ;

    rotate_z ( PI / 4 , x) ;
    point = get_point ( 0 , 1 , 0 ) ;
    multiply_mat4_tuple ( x , &point , &got );
    res = get_point ( -0.7071067 , 0.7071067 , 0 ) ;

    fail_unless ( compare_tuple ( &res , &got ) , "failure in rotating the point around the z axis" ) ;

    shear ( 0 , 1 , 0 , 0 , 0 , 0 , x );
    point = get_point ( 2 , 3 , 4 ) ;
    res = get_point ( 6 , 3 , 4 ) ;
    multiply_mat4_tuple ( x , &point , &got ) ;

    fail_unless ( compare_tuple ( &res , &got ) , "failure in shear transformation of point" ) ;

    shear ( 0 , 0 , 1 , 0 , 0 , 0 , x );
    point = get_point ( 2 , 3 , 4 ) ;
    res = get_point ( 2 , 5 , 4 ) ;
    multiply_mat4_tuple ( x , &point , &got ) ;

    fail_unless ( compare_tuple ( &res , &got ) , "failure in shear transformation of point" ) ;

    point = get_point ( 1 , 0 , 1 );
    mat4 rot ;
    rotate_x ( PI/2 , rot ) ;
    mat4 scale1 ;
    scale ( 5,5,5 , scale1 ) ;
    mat4 trans ;
    translate ( 10 , 5 , 7 , trans ) ;

    res = get_point ( 1 , -1 , 0 ) ;
    tuple res1 = get_point ( 5 , -5 , 0 );
    tuple res2 = get_point ( 15 , 0 , 7 );

    multiply_mat4_tuple ( rot , &point , &got ) ;

    fail_unless ( compare_tuple ( &res , &got ) , "failure in rotating a point around the x axis" ) ;

    multiply_mat4_tuple ( scale1 , &res , &got ) ;

    fail_unless ( compare_tuple ( &res1 , &got ) , "failure in scaling a point" ) ;

    multiply_mat4_tuple ( trans , &res1 , &got ) ;

    fail_unless ( compare_tuple ( &res2 , &got ) , "failure in translating a point ") ;

}
END_TEST

START_TEST(ray_operations)
{
#line 278
    tuple point = get_point ( 2, 3 , 4 );
    tuple direction = get_vector ( 1 , 0 ,0 ) ;
    ray n = get_ray ( &point , &direction) ;

    tuple got = ray_pos ( &n , 0 ) ;
    tuple res = get_point ( 2 , 3 , 4 );
    tuple res1 = get_point ( 3 , 3 , 4 ) ;
    tuple res2 = get_point ( 4.5 , 3 , 4 ) ;

    fail_unless ( compare_tuple( &got , &res ) , "failure in finding the position of a ray for input t") ;

    got = ray_pos ( &n , 1 ) ;
    fail_unless ( compare_tuple ( &got , &res1 ) , "failure in finding the position of a ray for input t" ) ;

    got = ray_pos ( &n , 2.5 ) ;
    fail_unless ( compare_tuple ( &got , &res2 ) , "failure in finding the position of a ray for input t" ) ;

    point = get_point ( 0 , 0, -5 );
    direction = get_vector ( 0 , 0 , 1 );

    n = get_ray ( &point , &direction );
    object sphere = get_sphere() ;

    inter_collec ret ;
    intersect ( &n , sphere , &ret ) ;

    fail_unless ( ret.count == 2 , "failure in finding the correct sphere-line intersection points" ) ;
    fail_unless ( float_cmp ( ret.xs[0].t , 4 ) , "failure in finding the correct sphere-line intersection points" ) ;
    fail_unless ( float_cmp ( ret.xs[1].t , 6 ) , "failure in finding the correct sphere-line intersection points" ) ;

    point = get_point ( 0 , 2, -5 );
    direction = get_vector ( 0 , 0 , 1 );

    n = get_ray ( &point , &direction );
    intersect ( &n , sphere , &ret  ) ;

    fail_unless ( ret.count == 0 , "failure in finding the correct sphere-line intersection points" ) ;

    //intersection test = { 2.5 } ;
    //intersection test1 = { 3.5 } ;
    //inter_collec test2 = intersections ( 2 , &test , &test1 ) ;

    //fail_unless ( test2.count == 2 , "failure occured in intersections data structure") ;
    //fail_unless ( test2.xs[0].t == 2.5 , "failure occured in intersections data structure") ;

    //float t = (hit( &test2 )) -> t ;
    //fail_unless ( float_cmp ( t , 2.5 ) , "failure occured in determining the hit in the collection" ) ;

    point = get_point ( 1 ,2 ,3 ) ;
    direction = get_vector ( 0,1,0 ) ;

    n = get_ray ( &point , &direction );
    mat4 trans ;
    translate ( 3 , 4 , 5 , trans ) ;

    ray new = transform_ray ( &n , trans ) ;
    tuple dd = get_point ( 4 , 6 , 8 ) ;

    fail_unless ( compare_tuple( &new.org , &dd ) , "failure occured while transforming the ray" ) ;
    fail_unless ( compare_tuple( &new.dir , &direction ) , "failure occured while transforming the ray" ) ;

    sphere = get_sphere() ;
    mat4 ident = { 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 1 , 0 , 0 , 0 , 0 , 1 } ;

    fail_unless ( compare_mat4( ident , sphere.trans ) == 0 , "failure to create a correct sphere" ) ;

    translate ( 2,3,4 , trans ) ;
    set_transform ( &sphere , trans ) ;

    fail_unless ( compare_mat4 ( trans , sphere.trans ) == 0, "failure to create a correct sphere" ) ;

    // intersecting a scaled sphere with a ray

    point = get_point ( 0 , 0 , -5 ) ;
    direction = get_vector (  0 , 0 , 1 );
    n = get_ray ( &point , &direction );
    scale ( 2 , 2 , 2 , trans ) ;

    set_transform ( &sphere , trans ) ;
    intersect ( &n , sphere , &ret  ) ;

    fail_unless ( ret.count == 2 , "failure in intersecting scaled sphere with ray " ) ;
    fail_unless ( float_cmp ( ret.xs[0].t , 3 )  , "failure in intersecting scaled sphere with ray" ) ;
    fail_unless ( float_cmp ( ret.xs[1].t , 7 )  , "failure in intersecting scaled sphere with ray" ) ;

    // testing the normal at function for the sphere

    sphere = get_sphere () ;
    point = get_point ( 1 , 0 , 0 ) ;
    point = sphere_normal ( &sphere , &point );
    res = get_vector ( 1 , 0 , 0 ) ;

    fail_unless ( compare_tuple (&point , &res ) , "failure in calculating the normal on a sphere") ;

    point = get_point ( 0,1,0 ) ;
    point = sphere_normal ( &sphere , &point ) ;
    res = get_vector ( 0,1,0 ) ;

    fail_unless ( compare_tuple (&point , &res ) , "failure in calculating the normal on a sphere") ;

    point = get_point ( 0.577350 , 0.577350 , 0.577350 );
    point = sphere_normal ( &sphere , &point ) ;
    res = get_vector( 0.577350 , 0.577350 , 0.577350 ) ;

    fail_unless ( compare_tuple (&point , &res ) , "failure in calculating the normal on a sphere") ;

    // lets see if the same works with a transformed sphere

    sphere = get_sphere() ;
    scale ( 1 , 0.5 , 1 , trans ) ;
    mat4 jfk, llkk ;
    rotate_z ( PI/5 , jfk ) ;
    multiply_mat4 ( trans , jfk , llkk );
    set_transform ( &sphere , llkk ) ;

    point = get_point ( 0 , 0.707106 , 0.707106 ) ;
    res = get_vector ( 0 ,  0.97014 , -0.24254 );

    point = sphere_normal ( &sphere , &point ) ;

    fail_unless ( compare_tuple ( &point , &res ) , "failure in calculating the normal on a sphere") ;

    tuple v = get_vector ( 0 , -1 , 0 );
    tuple normal = get_vector ( 0.707106 , 0.707106 , 0 );
    tuple ref = reflect ( &v , &normal ) ;
    point = get_vector ( 1,0,0 );

    fail_unless ( compare_tuple ( &ref , &point ) , "failure to calculate the reflection " ) ;

    v = get_vector ( 1 ,-1 , 0 );
    normal = get_vector ( 0 , 1 , 0 ) ;
    ref = reflect ( &v , &normal ) ;

    point = get_vector ( 1 , 1 , 0 ) ;

    fail_unless ( compare_tuple ( &ref , &point ) , "failure to calculate the reflection " ) ;

    point_light light ;
    light.position = get_point ( 0,0,0 ) ;
    light.intensity = get_color (1,1,1 ) ;

    point = get_point ( 0,0,0 ) ;

    fail_unless ( compare_tuple ( &light.position , &point ) , "failure in creating a point light " ) ;

    point = get_color ( 1,1,1 ) ;

    fail_unless ( compare_tuple ( &light.intensity , &point ) , "failure in creating a point light " ) ;
    sphere = get_sphere() ;
    fail_unless ( compare_tuple ( &sphere.mat.color, &point ) , "failure in creating a point light " ) ;

    material mat ;
    def_material ( &mat );
    point = get_point ( 0,0,0 ) ;

    tuple eyev = get_vector ( 0,0,-1 );
    tuple normalv = get_vector ( 0,0,-1 ) ;
    light = ( point_light ) { ( tuple ) { 0,0,-10,1 } , ( tuple ) { 1,1,1,0}  } ;
    tuple result = lighting ( &mat , &light , &point , &eyev , &normalv ) ;

    res = get_color ( 1.9 , 1.9 , 1.9 ) ;

    fail_unless ( compare_tuple ( &res , &result ) , "failure in shading the object" ) ;

    eyev = get_vector ( 0 , 0.707106 , -0.707106 ) ;
    normalv = get_vector ( 0 , 0 , -1 );
    result = lighting ( &mat , &light , &point , &eyev , &normalv ) ;

    res = get_color ( 1 , 1 , 1 ) ;

    fail_unless ( compare_tuple ( &res , &result ) , "failure in shading the object" ) ;


    eyev = get_vector ( 0 , 0 , -1 ) ;
    light = ( point_light ) { ( tuple ) { 0,10,-10,1 } , ( tuple ) { 1,1,1,0}  } ;
    normalv = get_vector ( 0 , 0 , -1 );
    result = lighting ( &mat , &light , &point , &eyev , &normalv ) ;

    res = get_color ( 0.7364 , 0.7364 , 0.7364 ) ;

    fail_unless ( compare_tuple ( &res , &result ) , "failure in shading the object" ) ;

    eyev = get_vector ( 0 , -0.707106  , -0.707106  ) ;
    light = ( point_light ) { ( tuple ) { 0,10,-10,1 } , ( tuple ) { 1,1,1,0}  } ;
    normalv = get_vector ( 0 , 0 , -1 );
    result = lighting ( &mat , &light , &point , &eyev , &normalv ) ;

    res = get_color ( 1.6362 , 1.6362  , 1.6362  ) ;

    fail_unless ( compare_tuple ( &res , &result ) , "failure in shading the object" ) ;

    eyev = get_vector ( 0 , 0  , -1 ) ;
    normalv = get_vector ( 0 , 0 , -1 );
    light = ( point_light ) { ( tuple ) { 0,0,10,1 } , ( tuple ) { 1,1,1,0}  } ;

    result = lighting ( &mat , &light , &point , &eyev , &normalv ) ;

    res = get_color ( 0.1 , 0.1   , 0.1   ) ;

    fail_unless ( compare_tuple ( &res , &result ) , "failure in shading the object" ) ;



}
END_TEST

START_TEST(scene_world_creation)
{
#line 482
    world hello ;
    init_world ( &hello ) ;

    fail_unless ( hello.obj_count == 2 , "failed to create a new world" ) ;
    tuple point = get_point ( -10,10,-10 ) ;
    tuple color = get_color (0.8,1.0,0.6) ;
    fail_unless ( compare_tuple ( &hello.light.position , &point ) , "failed to create a new world" ) ;
    fail_unless ( compare_tuple( &hello.objects[0].mat.color , &color ) , "failed to create a new world" ) ;

    // try to intersect ray with world
    point = get_point (0,0,-5 ) ;
    tuple direction = get_vector ( 0,0,1 ) ;

    ray god_ray = get_ray ( &point , &direction );
    inter_collec ret ;
    inter_ray_world ( &god_ray , &hello , &ret ) ;

    fail_unless ( ret.count == 4 , "failure to intersect ray and world" ) ;
    fail_unless ( float_cmp ( 4 , ret.xs[0].t ) , "failure to intersect ray and world" ) ;
    fail_unless ( float_cmp ( 4.5 , ret.xs[1].t ) , "failure to intersect ray and world" ) ;
    fail_unless ( float_cmp ( 5.5 , ret.xs[2].t ) , "failure to intersect ray and world" ) ;
    fail_unless ( float_cmp ( 6 , ret.xs[3].t ) , "failure to intersect ray and world" ) ;

    intersection i ;
    i.obj = get_sphere() ;
    i.t = 4 ;

    contact_calc calc ;
    compute_contact ( &god_ray , &i , &calc ) ;

    point = get_point ( 0,0,-1 ) ;

    fail_unless ( float_cmp(calc.t , 4 ) , "failure to create a contract_calc structure" ) ;
    fail_unless ( compare_tuple ( &point , &calc.p_contact) , "failure to create a contact_calc structure " ) ;

    direction = get_vector ( 0,0,-1 ) ;
    fail_unless ( compare_tuple ( &direction , &calc.normal) , "failure to create a contact_calc structure " ) ;
    fail_unless ( compare_tuple ( &direction , &calc.eye_v) , "failure to create a contact_calc structure " ) ;
    fail_unless ( calc.inside == 0 , "failure to create a contact" );

    point = get_point (0,0,0);
    direction = get_vector ( 0,0,1);
    god_ray = get_ray ( &point , &direction );

    i.obj = get_sphere() ;
    i.t = 1 ;
    compute_contact ( &god_ray , &i , &calc ) ;

    direction = get_vector(0,0,-1);

    fail_unless ( calc.inside == 1 , "failure to create a contact");
    fail_unless ( compare_tuple( &direction , &calc.eye_v ) , "failure to create a contact" ) ;
    fail_unless ( compare_tuple ( &direction , &calc.normal) , "failure to create a contact" ) ;

    // test the color_at function

    point = get_point (0,0,-5) ;
    direction = get_vector (0,1,0);
    god_ray = get_ray ( &point , &direction ) ;

    color = color_at ( &hello, &god_ray ) ;
    tuple black = get_color ( 0,0,0 ) ;

    fail_unless ( compare_tuple ( &black , &color ) , "error in color_at function" );

    direction = get_vector ( 0,0,1) ;
    god_ray = get_ray ( &point , &direction ) ;
    color = color_at ( &hello , &god_ray ) ;

    black = get_color (0.38066 , 0.47583 , 0.2855 ) ;

    fail_unless ( compare_tuple ( &black , &color ) , "error in color_at function" );

    // final test for color_at

    hello.objects[0].mat.ambient = 1;
    hello.objects[1].mat.ambient = 1;

    point = get_point( 0 , 0, 0.75 );
    direction = get_vector ( 0,0,-1 );
    god_ray = get_ray ( &point , &direction ) ;

    color = color_at ( &hello , &god_ray ) ;

    fail_unless ( compare_tuple( &color , &hello.objects[1].mat.color ) , "error in color_at function");

    // test the view transform

    mat4 view ;
    tuple from = get_point (0,0,0) ;
    tuple to = get_point (0,0,-1) ;
    tuple up_v = get_vector (0,1,0) ;

    view_transform ( &to , &from , &up_v  , view );

    mat4 ident ;
    ident_mat4 ( ident ) ;

    fail_unless ( compare_mat4 ( ident , view ) == 0 , "failed to calculate the view matrix" ) ;
    to = get_point ( 0,0,1 ) ;
    view_transform ( &to , &from , &up_v  , view );
    scale (-1,1,-1 , ident ) ;
    fail_unless ( compare_mat4 ( ident , view ) == 0 , "failed to calculate the view matrix" ) ;

    from = get_point (0,0,8) ;
    to = get_point (0,0,0);
    up_v = get_vector (0,1,0) ;

    view_transform ( &to , &from , &up_v  , view );
    translate ( 0,0,-8 , ident ) ;

    fail_unless ( compare_mat4 ( ident , view ) == 0 , "failed to calculate the view matrix" ) ;

    // test the camera ds

    camera cs ;
    init_camera ( 100 , 100 , PI/2 , &cs );
    ident_mat4 ( ident ) ;

    fail_unless ( compare_mat4 ( cs.transform , ident ) == 0 , "failed to create a camera" ) ;
    fail_unless ( cs.h_size == 100 , "failed to create a camera" ) ;
    fail_unless ( cs.v_size == 100 , "failed to create a camera" ) ;

    init_camera ( 200 , 125, PI/2 , &cs ) ;
    fail_unless ( float_cmp( 0.01 , cs.pixel_size )  , "failed to create a camera ") ;

    init_camera ( 125 , 200 , PI/2 , &cs ) ;
    fail_unless ( float_cmp( 0.01 , cs.pixel_size ) , "failed to create a camera " ) ;

    // test the ray_for_pixel function

    init_camera ( 201, 101, PI/2 , &cs ) ;
    ray_for_pixel ( &cs , 100 , 50 , &god_ray ) ;

    point = get_point (0,0,0);
    direction = get_vector (0,0,-1);

    fail_unless ( compare_tuple(&point , &god_ray.org )) ;
    fail_unless ( compare_tuple(&direction , &god_ray.dir ));

    ray_for_pixel ( &cs , 0 , 0 , &god_ray ) ;

    fail_unless ( compare_tuple ( &point , &god_ray.org ) ) ;

    // test the render function

    init_camera ( 11 , 11, PI/2 , &cs ) ;
    from = get_point (0,0,-5);
    to = get_point (0,0,0);
    tuple up = get_vector(0,1,0);

    view_transform ( &to , &from , &up , cs.transform );
    render( &cs , &hello ) ;

    color = get_color ( 0.380661 , 0.475827 , 0.285496 ) ;

    fail_unless ( compare_tuple ( &canvas.mem[5*11+5] , &color ) , "error in render function" ) ;


























}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_tuple_creation);
    tcase_add_test(tc1_1, tuple_operation);
    tcase_add_test(tc1_1, color_operations);
    tcase_add_test(tc1_1, matrix_operations);
    tcase_add_test(tc1_1, transformations);
    tcase_add_test(tc1_1, ray_operations);
    tcase_add_test(tc1_1, scene_world_creation);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
