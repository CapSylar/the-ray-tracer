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

int main()
{
    tuple to , from ;
    camera cs ;
    world hello ;

    mat4 trans1 , trans2 , trans3 ;
    tuple color1 , color2 ;

    init_world( &hello ) ;
    hello.light.position = get_point(-10,7,-10) ;

    translate(1.5f , 1 , 0.3f , trans1 ) ;
    scale( 1.2f , 1.9f , 0.8f , trans2 ) ;
    multiply_mat4( trans1 , trans2 , hello.objects[1].trans ) ;
    hello.objects[1].mat.color = get_color( 0.47f , 0.60f , 0.788f ) ;
    hello.objects[1].mat.specular = 0.3f ;
    color1 = get_color( 1 , 1  ,1 ) ;
    color2 = get_color( 0 , 0 , 0 ) ;
    hello.objects[1].mat.has_pattern = 1;
    hello.objects[1].mat.current_pattern = get_pattern( PATTERN_STRIPES , 0 , 0 , &color1 , &color2 ) ;
    scale( 0.2f , 0.2f ,0.2f ,hello.objects[1].mat.current_pattern.pattern_trans ) ;
    translate(-0.5f,1,0.5f, hello.objects[0].trans ) ;
    hello.objects[0].mat.color = get_color( 0.94f , 0.15f , 0.89f ) ;

    init_camera ( 3000 , 3000 , PI/2 , &cs ) ;

    from = get_point (0,1.5f,-5);
    to = get_point (0,1,0);
    tuple up = get_vector(0,1,0);

    view_transform ( &to , &from , &up , cs.transform );

    // floor

    object floor = get_plane() ;
    floor.mat.color = get_color(1,1,1 ) ;
    color1 = get_color( 0.92f , 0.92f , 0.204f ) ;
    color2 = get_color( 0.741f , 0.09f , 0.294f ) ;

    floor.mat.has_pattern = 1 ;
    floor.mat.current_pattern = get_pattern( PATTERN_RING , 0 , 0 , &color1 , &color2 ) ;
    floor.mat.current_pattern.isNested2 = 1;
    pattern nested = get_pattern( PATTERN_CHECKER , 0 , 0 , &color1 , &color2 ) ;
    scale( 0.1f , 0.1f , 0.1f , nested.pattern_trans ) ;
    floor.mat.current_pattern.pattern2 = &nested ;
    add_obj_world( &hello , &floor ) ;

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
    add_obj_world(&hello , &wall) ;

    // add a sphere

    object last = get_sphere();
    translate(-2,0.7f ,-1.6f, trans1) ;
    scale( 1 , 1.7f , 0.8f , trans2 ) ;
    multiply_mat4( trans1 , trans2 , last.trans ) ;
    last.mat.color = get_color( 0 , 1 , 0.1f) ;
    last.mat.specular = 3 ;
    last.mat.has_pattern = 1 ;
    last.mat.current_pattern = get_pattern( PATTERN_RING , 0 , 0 , &color1 , &color2 ) ;
    scale( 0.2f , 0.2f , 0.2f , trans1 ) ;
    rotate_z( -PI/4 , trans2 ) ;
    multiply_mat4( trans1 , trans2 , last.mat.current_pattern.pattern_trans ) ;

    // nested pattern
    color1 = get_color( 0.6f , 0.3f , 0.9f) ;
    color2 = get_color ( 0 , 1 , 0 ) ;
    pattern new_pattern = get_pattern( PATTERN_GRADIENT , 0 , 0 , &color1 , &color2 ) ;
    last.mat.current_pattern.isNested1 = 1 ;
    last.mat.current_pattern.pattern1 = &new_pattern ;

    add_obj_world( &hello , &last ) ;

    struct timeval start , stop ;
    gettimeofday( &start , 0 ) ;

    render( &cs , &hello ) ;

    gettimeofday( &stop , 0 ) ;

    destroy_world( &hello ) ;

    canvas_ppm() ;

    printf("done in %lu seconds ", (stop.tv_sec - start.tv_sec)  );

    return 0;
}
