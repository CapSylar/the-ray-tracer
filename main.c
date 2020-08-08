#include <stdio.h>
#include <math.h>
#include "defs.h"
#include "factories.h"
#include "canvas.h"
#include "matrix_routines.h"
#include "trans_routines.h"
#include "ray.h"

#define PIXEL 1000

int main()
{
    init_canvas(PIXEL,PIXEL) ;

    float wall_z = 10 ;
    ray gold ;
    gold.org = get_point( 0 , 0 , -5 ) ;
    float wall_size = 7 ;
    float pixel_size = wall_size / PIXEL ;
    float half = wall_size / 2 ;

    object s = get_sphere() ;
    s.mat.color = ( tuple ) { 0.3f , 0.1f , 0.8f } ;
    mat4 tranform ;
    scale( 1 , 1.1f , 0.5 , tranform );

    // create the light source

    point_light light = get_plight( get_point(-15,15,-10) , get_color(1,1,1) ) ;

    mat4 temp ;
    mat4 final ;
    rotate_z( PI/6 , temp );
    multiply_mat4( temp , tranform , final ) ;

    //set_transform( &s , final ) ;

    for ( int y = 0 ; y < PIXEL ; ++y )
    {
        float world_y = -half + pixel_size*y ;
        for (int x = 0; x < PIXEL; ++x)
        {
            float world_x = -half + pixel_size*x ;

            // we now have the point on the wall
            tuple wall_point = get_point( world_x , world_y , wall_z ) ;
            // calculate the direction of the ray
            tuple dir = sub_tuples( &wall_point , &gold.org ) ;
            normalize_tuple( &dir ) ;

            gold.dir = dir ;
            inter_collec coll = intersect( &gold , s ) ;

            if ( hit( &coll ) ) // there is a hit
            {
                // find the normal and the point of contact
                tuple intersection = ray_pos( &gold , coll.xs[0].t ) ;
                tuple normal = sphere_normal( &s , &intersection ) ;
                tuple eye_dir = gold.dir ;
                neg_tuple( &eye_dir ) ;
                tuple color = lighting( &s.mat , &light , &intersection , &eye_dir , &normal ) ;

                canvas_write( color , x , PIXEL - y ) ;
            }
        }
    }

    canvas_ppm();
    return 0;
}
