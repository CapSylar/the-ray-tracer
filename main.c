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

    translate(1.5 , 1 , -0.7f , trans1 ) ;
    scale( 1.2f , 0.9f , 0.8f , trans2 ) ;
    multiply_mat4( trans1 , trans2 , hello.objects[1].trans ) ;
    hello.objects[1].mat.color = get_color( 0.47f , 0.60f , 0.788f ) ;
    hello.objects[1].mat.specular = 0.3f ;

    translate(-0.5f,1,0.5f, hello.objects[0].trans ) ;
    hello.objects[0].mat.color = get_color( 0.94f , 0.15f , 0.89f ) ;

    init_camera ( 1500 , 1500 , PI/2 , &cs ) ;

    from = get_point (0,1.5f,-5);
    to = get_point (0,1,0);
    tuple up = get_vector(0,1,0);

    view_transform ( &to , &from , &up , cs.transform );

    // floor
    object floor = get_sphere() ;
    scale(10,0.01f,10 , floor.trans ) ;
    floor.mat.color = get_color (1,0.9f,0.9f) ;
    floor.mat.specular = 0 ;

    add_obj_world( &hello , &floor ) ;

    // left wall

    object left_wall = get_sphere();
    scale(10,0.01f,10 , left_wall.trans ) ;
    left_wall.mat.color = get_color (1,0.9f,0.9f) ;
    left_wall.mat.specular = 0;


    scale(10,0.01f,10, trans1 );
    rotate_x(PI/2 , trans2 ) ;
    multiply_mat4( trans2 , trans1 , trans3 ) ;
    rotate_y(PI/4 , trans1 ) ;
    multiply_mat4( trans1 , trans3 , trans2 ) ;
    translate(0,0,5 , trans1 ) ;
    multiply_mat4( trans1 ,  trans2 , left_wall.trans ) ;

    add_obj_world( &hello , &left_wall );

    // right wall

    object right_wall = get_sphere();
    scale(10,0.01f,10 , right_wall.trans ) ;
    right_wall.mat.color = get_color (1,0.9f,0.9f) ;
    right_wall.mat.specular = 0;

    scale(10,0.01f,10, trans1 );
    rotate_x(PI/2 , trans2 ) ;
    multiply_mat4( trans2 , trans1 , trans3 ) ;
    rotate_y(-PI/4 , trans1 ) ;
    multiply_mat4( trans1 , trans3 , trans2 ) ;
    translate(0,0,5 , trans1 ) ;
    multiply_mat4( trans1 ,  trans2 , right_wall.trans ) ;

    add_obj_world( &hello , &right_wall );

    // add a sphere

    object last = get_sphere();
    translate(-2,0.8f ,-1.7f, trans1) ;
    scale( 0.5f , 0.7f , 0.4f , trans2 ) ;
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
