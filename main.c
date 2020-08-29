#include "defs.h"
#include "factories.h"
#include "canvas.h"
#include "matrix_routines.h"
#include "trans_routines.h"
#include "ray.h"
#include "world_view.h"
#include <sys/time.h>
#include <stdio.h>

int main()
{
    tuple to , from ;
    camera cs ;
    world hello ;

    mat4 trans1 , trans2 , trans3 ;

    init_world( &hello ) ;
    hello.light.position = get_point(-10,7,-10) ;

    translate(1.5f , 1 , 0.3f , trans1 ) ;
    scale( 1.2f , 1.9f , 0.8f , trans2 ) ;
    multiply_mat4( trans1 , trans2 , hello.objects[1].trans ) ;
    hello.objects[1].mat.color = get_color( 0.47f , 0.60f , 0.788f ) ;
    hello.objects[1].mat.specular = 0.3f ;

    translate(-0.5f,1,0.5f, hello.objects[0].trans ) ;
    hello.objects[0].mat.color = get_color( 0.94f , 0.15f , 0.89f ) ;

    init_camera ( 2000 , 2000 , PI/2 , &cs ) ;

    from = get_point (0,1.5f,-5);
    to = get_point (0,1,0);
    tuple up = get_vector(0,1,0);

    view_transform ( &to , &from , &up , cs.transform );

    // floor

    object floor = get_plane() ;
    floor.mat.color = get_color(1,1,1 ) ;
    add_obj_world( &hello , &floor ) ;

    object wall = get_plane() ;
    wall.mat.color = get_color( 1 , 0 , 0.7f ) ;
    wall.mat.specular = 0.1f ;
    rotate_x( -PI/2 , trans1 ) ;
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
