#include "defs.h"
#include "factories.h"
#include "canvas.h"
#include "matrix_routines.h"
#include "trans_routines.h"
#include "ray.h"
#include "world_view.h"
#include <sys/time.h>
#include <stdio.h>
#include "patterns.h"
#include <stdlib.h>
#include <time.h>
#include "interface.h"

#define RESOLUTION_WIDTH 1000
#define RESOLUTION_HEIGHT 800

int main()
{
    srand( time(NULL) ) ;

    tuple to , from ;
    camera cs ;
    world hello ;

    mat4 trans1 , trans2 , trans3 ;
    tuple color1 , color2 ;

    init_world( &hello ) ;
    hello.light.position = get_point(-10,50,-20) ;

    translate(-2 , 1 , -8 , trans1 ) ;
    scale( 0,0,0 , trans2 ) ;
    multiply_mat4( trans1 , trans2 , hello.objects[1].trans ) ;
    hello.objects[1].mat.color = get_color( 0.4f , 0.1f , 0.3f ) ;
    hello.objects[1].mat.specular = 1 ;
    color1 = get_color( 1 , 1  ,1 ) ;
    color2 = get_color( 0 , 0 , 0 ) ;
    hello.objects[1].mat.reflective = 1 ;
    scale( 0.2f , 0.2f ,0.2f ,hello.objects[1].mat.current_pattern.pattern_trans ) ;
    scale ( 1.2f , 1.2f , 1.2f , trans1 ) ;
    translate(-0.5f,1,0.5f, trans2 ) ;
    multiply_mat4( trans1 , trans2 , hello.objects[0].trans ) ;
    //hello.objects[0].mat.color = get_color( 0.94f , 0.15f , 0.89f ) ;
    hello.objects[0].mat.color = get_color( 0 ,0,0.2f ) ;
    hello.objects[0].mat.reflective = 0.9f ;
    hello.objects[0].mat.refractive_index = 1.5f ;
    hello.objects[0].mat.transparency = 1 ;

    init_camera ( RESOLUTION_WIDTH , RESOLUTION_HEIGHT , PI/3 , &cs ) ;

    from = get_point (-4,1,-8);
    to = get_point (3,0.7f,0);
    tuple up = get_vector(0,1,0);

    view_transform ( &to , &from , &up , cs.transform );

    // sphere3

    object last = get_sphere();
    translate(-3,1.2f ,-1.6f, trans1) ;
    scale( 1.2f , 1.2f , 1.2f , trans2 ) ;
    multiply_mat4( trans1 , trans2 , last.trans ) ;
    last.mat.color = get_color( 0 , 1 , 0.1f) ;
    last.mat.specular = 1 ;
    last.mat.has_pattern = 0 ;
    last.mat.reflective = 0.9f ;
    last.mat.transparency = 0.99f ;
    last.mat.refractive_index = 1.5f;

    //add_obj_world( &hello , &last ) ;

    // sphere4

    object sphere4 = get_sphere();
    translate(-3,0.8f ,-4, trans1) ;
    scale( 0.8f , 0.8f , 0.8f  , trans2 ) ;
    multiply_mat4( trans1 , trans2 , sphere4.trans ) ;
    sphere4.mat.color = get_color( 0,0,0) ;
    sphere4.mat.specular = 3 ;
    sphere4.mat.has_pattern = 0 ;
    sphere4.mat.reflective = 1 ;

    add_obj_world( &hello , &sphere4 ) ;

    //sphere 5

    object sphere5 = get_sphere();
    translate(3,0.8f ,-8, trans1) ;
    scale( 1 , 1 , 1  , trans2 ) ;
    multiply_mat4( trans1 , trans2 , sphere5.trans ) ;
    sphere5.mat.color = get_color( 0,0,0) ;
    sphere5.mat.specular = 3 ;
    sphere5.mat.has_pattern = 0 ;
    sphere5.mat.reflective = 0.9f ;
    sphere5.mat.transparency = 0.9f ;
    sphere5.mat.refractive_index = 1.4f;

    add_obj_world( &hello , &sphere5 ) ;

    // cube 1

    object cube1 = get_cube();
    translate( -2 , 0.4f , -3 , trans1 );
    scale ( 0.4f , 0.4f , 0.4f , trans2 ) ;
    multiply_mat4( trans1 , trans2 , cube1.trans ) ;
    cube1.mat.color = get_color( 0.7f , 0.5f , 0.9f ) ;
    cube1.mat.reflective = 0.8f ;
    cube1.mat.has_pattern = 0;
    cube1.mat.transparency = 0.5f ;
    cube1.mat.refractive_index = 1.5f ;
    cube1.mat.specular = 3;
    add_obj_world( &hello , &cube1 ) ;

    // cube2

    object cube2 = get_cube();
    translate( -3 , 0.4f , -4 , trans1 ) ;
    scale ( 0.4f , 0.4f , 0.4f , trans2 ) ;
    rotate_y( PI/4 , trans3 ) ;
    mat4 trans4 ;
    multiply_mat4( trans1 , trans2 , trans4 ) ;
    multiply_mat4( trans4 , trans3 , cube2.trans ) ;
    cube2.mat.color = get_color( 0.9f , 0.3f , 0.9f ) ;
    cube2.mat.reflective = 0.8f ;
    cube2.mat.has_pattern = 0;
    cube2.mat.transparency = 0.5f ;
    cube2.mat.refractive_index = 1.5f ;
    cube2.mat.specular = 3;
    add_obj_world( &hello , &cube2 ) ;

    // cube3

    object cube3 = get_cube();
    translate( 3 , 0.4f , -4 , trans1 ) ;
    scale ( 0.4f , 0.4f , 0.4f , trans2 ) ;
    rotate_y( PI/5 , trans3 ) ;

    multiply_mat4( trans1 , trans2 , trans4 ) ;
    multiply_mat4( trans4 , trans3 , cube3.trans ) ;
    cube3.mat.color = get_color( 0.7f , 0.7f , 0.3f ) ;
    cube3.mat.reflective = 0.8f ;
    cube3.mat.has_pattern = 0;
    cube3.mat.transparency = 0.5f ;
    cube3.mat.refractive_index = 1.5f ;
    cube3.mat.specular = 3;
    add_obj_world( &hello , &cube3 ) ;

    // cube4

    object cube4 = get_cube();
    translate( 2 , 0.6f , -6 , trans1 ) ;
    scale ( 0.6f , 0.6f , 0.6f , trans2 ) ;
    rotate_y( -PI/3 , trans3 ) ;

    multiply_mat4( trans1 , trans2 , trans4 ) ;
    multiply_mat4( trans4 , trans3 , cube4.trans ) ;
    cube4.mat.color = get_color( 0.3f, 0.8f , 0.2f ) ;
    cube4.mat.reflective = 0.8f ;
    cube4.mat.has_pattern = 0;
    cube4.mat.transparency = 0.5f ;
    cube4.mat.refractive_index = 1.5f ;
    cube4.mat.specular = 3;
    add_obj_world( &hello , &cube4 ) ;

    // cylinder
    mat4 trans5;
    object cylo = get_sphere();
    scale( 1.5f , 1.5f , 1.5f , trans1 ) ;
    translate( 0 , 1.5f , -4 , trans2 ) ;
    multiply_mat4( trans2 , trans1 , cylo.trans ) ;
    cylo.mat.color = get_color(0,0.2f,0) ;
    cylo.mat.reflective = 1 ;

    cylo.closed = 1 ;
    cylo.min = 0;
    cylo.max = 2.5f ;
    add_obj_world( &hello , &cylo ) ;

    // floor

    object floor = get_plane() ;
    floor.mat.color = get_color(1,1,1 ) ;
    color1 = get_color( 0.2f , 0.2f, 0.2f ) ;
    color2 = get_color( 0 , 0 , 0.1f ) ;

    floor.mat.has_pattern = 1 ;
    floor.mat.current_pattern = get_pattern( PATTERN_CHECKER , 0 , 0 , &color1 , &color2 ) ;
    floor.mat.reflective = 0.4f ;
    add_obj_world( &hello , &floor ) ;

    // wall

    object wall = get_plane() ;
    wall.mat.color = get_color( 1 , 0 , 0.7f ) ;
    wall.mat.specular = 0.1f ;
    wall.mat.has_pattern = 1 ;
    color1 = get_color( 0.65f,0.23f,0.9f ) ;
    color2 = get_color ( 1 , 0 , 0 ) ;
    wall.mat.current_pattern = get_pattern( PATTERN_CHECKER , 0 , 0 , &color1 , &color2 ) ;

    rotate_y( -PI/4 , wall.mat.current_pattern.pattern_trans ) ;
    rotate_x( +PI/2 , trans1 ) ;
    translate( 0 , 0 , 5 , trans2 ) ;
    multiply_mat4( trans2 , trans1 , wall.trans ) ;
    //add_obj_world(&hello , &wall) ;

    // nested pattern
    color1 = get_color( 0.6f , 0.3f , 0.9f) ;
    color2 = get_color ( 0 , 1 , 0 ) ;
    pattern new_pattern = get_pattern( PATTERN_GRADIENT , 0 , 0 , &color1 , &color2 ) ;
    last.mat.current_pattern.isNested1 = 1 ;
    last.mat.current_pattern.pattern1 = &new_pattern ;

    if ( init_interface( RESOLUTION_WIDTH , RESOLUTION_HEIGHT ) == EXIT_FAILURE )
        return EXIT_FAILURE;

    struct timeval start , stop ;
    gettimeofday( &start , 0 ) ;

    //render( &cs , &hello ) ;

    init_canvas(cs.v_size,cs.h_size) ;

    for ( int y = 0 ; y < cs.v_size ; ++y )
    {
        if ( poll_inputs_exit() )
            break ;

        for (int x = 0; x < cs.h_size; ++x)
        {
            ray god_ray;
            ray_for_pixel(&cs, x, y, &god_ray);
            tuple color = color_at( &hello , &god_ray, 1, 5);
            canvas_write(color, x, y);
            interface_write(color, x, y);
        }

        update_interface();
    }


    gettimeofday( &stop , 0 ) ;

    destroy_world( &hello ) ;

    canvas_ppm() ;

    printf("done in %lu seconds ", (stop.tv_sec - start.tv_sec)  );

    close_interface() ; // clean up and close

    return 0;
}
