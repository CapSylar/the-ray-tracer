#include "hexagon.h"
#include "defs.h"
#include "ray.h"
#include "matrix_routines.h"
#include "trans_routines.h"
#include "factories.h"

object* hexagon_corner()
{
    object* corner = get_sphere();

    //setup material
    corner->mat.reflective=1;
    corner->mat.color = get_color(0.5f,0,0.5f);

    mat4 trans1 , trans2 ;
    translate( 0,0,-1 , trans1 );
    scale( 0.25f , 0.25f , 0.25f , trans2 );
    multiply_mat4( trans1 , trans2 , corner->trans );

    return corner;
}

object* hexagon_edge()
{
    object* edge = get_cylinder();
    edge->min = 0;
    edge->max = 1;

    edge->mat.reflective=1;
    edge->mat.color = get_color(0.5f,0,0.5f);

    mat4 trans1 , trans2 , trans3 ;
    translate( 0 , 0 , -1 , trans1 );
    rotate_y(-PI/6 , trans2 );
    multiply_mat4( trans1 , trans2 , trans3 );
    // then rotate_z

    rotate_z( -PI/2 , trans1 );
    multiply_mat4( trans3 , trans1 , trans2 );
    scale( 0.25f , 1 , 0.25f , trans1 );
    multiply_mat4( trans2 , trans1 , edge->trans );

    return edge;
}

group* hexagon_side()
{
    // add an edge and a corner to form a side
    group* new_grp = get_group();
    add_g_object( new_grp , hexagon_corner() );
    add_g_object( new_grp , hexagon_edge() ) ;

    return new_grp;
}

group* get_hexagon()
{
    group* grp = get_group();

    for ( int i = 0 ; i < 6 ; ++i )
    {
        group* side = hexagon_side();
        rotate_y( PI/3 * i , side->trans );
        add_g_group( grp , side );
    }

    return grp;
}
