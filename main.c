#include <stdio.h>
#include <math.h>
#include "defs.h"
#include "factories.h"
#include "canvas.h"

int main()
{

    init_canvas(200,200) ;

    tuple velocity = get_vector(7,25,0) ;
    tuple position = get_point(0,0,0) ;
    tuple gravity = get_vector(0,-2,0);
    tuple wind = get_vector(0,0,0) ;


    // TODO: implement tick function

    do // while still in the air
    {
        canvas_write( get_color(1,0,1) , position.x , 200 - position.y ) ;
        position = add_tuples(&position,&velocity) ;
        // update the velocity vector

        velocity = add_tuples(&velocity,&gravity) ;

    } while ( position.y ) ;

    canvas_ppm();

    return 0;
}
