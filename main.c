#include "defs.h"
#include "factories.h"
#include "canvas.h"
#include "trans_routines.h"
#include "ray.h"
#include "world_view.h"
#include <sys/time.h>
#include <stdio.h>
#include "patterns.h"
#include <stdlib.h>
#include <time.h>
#include "interface.h"
#include "hexagon.h"
#include "matrix_routines.h"

#define RESOLUTION_WIDTH 300
#define RESOLUTION_HEIGHT 300

int main()
{
    srand( time(NULL) ) ;

    tuple to , from ;
    camera cs ;
    world hello ;

    mat4 trans1 , trans2 , trans3 ;
    tuple color1 , color2 ;

    init_empty_world(&hello);
    hello.light.position = get_point(0,2,0) ;

    init_camera ( RESOLUTION_WIDTH , RESOLUTION_HEIGHT , PI/2 , &cs ) ;

    from = get_point (0,2 ,-1);
    to = get_point (0,0.5f,1);
    tuple up = get_vector(0,1,0);

    view_transform ( &to , &from , &up , cs.transform );
    gluInvertMatrix( cs.transform , cs.inverse );

    // hexagon

    group* hex = get_hexagon();
    translate( 0 , 0.5f , 1 , hex->trans );
    add_to_world( &hello , hex , GROUP );

    object* sp1 = get_glass_sphere();
    sp1->mat.color = get_color(0,0.1f ,0) ;
    sp1->mat.reflective = 1.5f ;
    translate(1,0.5f,2, sp1->trans );

    //add_to_world( &hello , sp1 , OBJECT );
//
//    object* sp2 = get_glass_sphere();
//    sp2->mat.color = get_color(0.2f,0.2f,0.2f) ;
//    translate(-1,0.5f,2, sp2->trans );
//    //add_to_world( &hello , sp2 , OBJECT );
//
//    object* sp3 = get_glass_sphere();
//    sp3->mat.color = get_color(0,0,0) ;
//    translate(-1,1,2, sp3->trans );
//    //add_to_world( &hello , sp3 , OBJECT );
//
//    object* sp4 = get_glass_sphere();
//    sp4->mat.color = get_color(0,0,0) ;
//    translate(1,1,4, sp4->trans );
//    //add_to_world( &hello , sp4 , OBJECT );
//
    // floor

    object* floor = get_plane() ;
    floor->mat.color = get_color(1,1,1 ) ;
    color1 = get_color( 0.2f , 0.2f, 0.2f ) ;
    color2 = get_color( 0 , 0 , 0.1f ) ;

    floor->mat.has_pattern = 1 ;
    floor->mat.current_pattern = get_pattern( PATTERN_CHECKER , 0 , 0 , &color1 , &color2 ) ;
    floor->mat.reflective = 0 ;
    add_to_world( &hello , floor , OBJECT ) ;

    // god's ceiling

    object* ceiling = get_plane() ;
    ceiling->mat.color = get_color(1,1,1 ) ;
    color1 = get_color( 0.2f , 0.2f, 0.2f ) ;
    color2 = get_color( 0 , 0 , 0.1f ) ;

    ceiling->mat.has_pattern = 1 ;
    ceiling->mat.current_pattern = get_pattern( PATTERN_CHECKER , 0 , 0 , &color1 , &color2 ) ;
    ceiling->mat.reflective = 0 ;

    rotate_x( PI , trans1 );
    translate ( 0 , 3 , 0 , trans2 );
    multiply_mat4( trans2 , trans1 , ceiling->trans ) ;

    add_to_world( &hello , ceiling , OBJECT ) ;

    // front wall

    object* front = get_plane();
    front->mat.color = get_color (0.6f , 0 , 0 );
    front->mat.reflective = 0.1f;
    rotate_x ( PI/2 , trans1 );
    translate( 0 , 0 ,3 , trans2 );
    multiply_mat4( trans2 , trans1 , front->trans );

    add_to_world( &hello , front , OBJECT );

    // right wall

    object* right_wall = get_plane();
    right_wall->mat.color = get_color (0.6f , 0 , 0.5f );
    rotate_z ( PI/2 , trans1 );
    translate( 3 , 0 ,0 , trans2 );
    multiply_mat4( trans2 , trans1 , right_wall->trans );

    add_to_world( &hello , right_wall , OBJECT );

    // left wall

    object* left_wall = get_plane();
    left_wall->mat.color = get_color (0.6f , 0 , 0.5f );
    rotate_z ( -PI/2 , trans1 );
    translate( -3 , 0 ,0 , trans2 );
    multiply_mat4( trans2 , trans1 , left_wall->trans );

    add_to_world( &hello , left_wall , OBJECT );

    // back wall

    object* back_wall = get_plane();
    back_wall->mat.color = get_color (0.6f , 0 , 0.5f );
    rotate_x ( -PI/2 , trans1 );
    translate( 0 , 0 ,-3 , trans2 );
    multiply_mat4( trans2 , trans1 , back_wall->trans );

    add_to_world( &hello , back_wall , OBJECT );

//    // wall
//
//    object* wall = get_plane() ;
//    wall->mat.color = get_color( 1 , 0 , 0.7f ) ;
//    wall->mat.specular = 0.1f ;
//    wall->mat.has_pattern = 1 ;
//    color1 = get_color( 0.65f,0.23f,0.9f ) ;
//    color2 = get_color ( 1 , 0 , 0 ) ;
//    wall->mat.current_pattern = get_pattern( PATTERN_CHECKER , 0 , 0 , &color1 , &color2 ) ;
//
//    rotate_y( -PI/4 , wall->mat.current_pattern.pattern_trans ) ;
//    rotate_x( +PI/2 , trans1 ) ;
//    translate( 0 , 0 , 5 , trans2 ) ;
//    multiply_mat4( trans2 , trans1 , wall->trans ) ;
//    add_obj_world(&hello , wall) ;
//
//     nested pattern
//    color1 = get_color( 0.6f , 0.3f , 0.9f) ;
//    color2 = get_color ( 0 , 1 , 0 ) ;
//    pattern new_pattern = get_pattern( PATTERN_GRADIENT , 0 , 0 , &color1 , &color2 ) ;
//    last->mat.current_pattern.isNested1 = 1 ;
//    last->mat.current_pattern.pattern1 = &new_pattern ;

    if ( init_interface( RESOLUTION_WIDTH , RESOLUTION_HEIGHT ) == EXIT_FAILURE )
        return EXIT_FAILURE;

    struct timeval start , stop , result ;
    gettimeofday( &start , 0 ) ;

    init_canvas(cs.v_size,cs.h_size) ;
    precompute_inv_world( &hello );

    for ( int y = 0 ; y < cs.v_size ; ++y )
    {
        if ( poll_inputs_exit() )
            break ;

        for (int x = 0; x < cs.h_size; ++x)
        {
            ray god_ray;
            ray_for_pixel(&cs, x, y, &god_ray);
            tuple color = color_at( &hello , &god_ray, 1, 5 );

            canvas_write(color, x, y);
            interface_write(color, x, y);
        }

        update_interface();
    }

    gettimeofday( &stop , 0 ) ;

    destroy_world( &hello ) ;

    canvas_ppm() ;

    timersub( &stop , &start , &result );
    printf("Time elapsed: %ld.%06lu seconds \n", (long int)(result.tv_sec), (unsigned long)(result.tv_usec));

    close_interface() ; // clean up and close

    return 0;
}
