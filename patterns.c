#include "patterns.h"
#include "defs.h"
#include <stdio.h>
#include <math.h>
#include "matrix_routines.h"
#include "factories.h"

tuple stripe_pattern_at ( object* o , pattern* pattern , tuple* point )
{
    int x = (((int)point->x) % 2 ) == 0 ;
    if (point->x < 0 ) // TODO: fix this shit
        x = x ? 0 : 1 ;
    //return ( x ) ? pattern->color1 : pattern->color2 ;
    if ( x )
    { // pick first color or pattern
        return pattern->isNested1 ? pattern_at_object( o , pattern->pattern1 , point ) : pattern->color1 ;
    }
    else
    {
        return pattern->isNested2 ? pattern_at_object( o , pattern->pattern2 , point ) : pattern->color2 ;
    }
}
tuple gradient_pattern_at ( object* o ,  pattern* pattern , tuple* point  )
{
    // linear interpolation
    tuple color1 = pattern->isNested1 ? pattern_at_object( o , pattern->pattern1 , point ) : pattern->color1 ;
    tuple color2 = pattern->isNested2 ? pattern_at_object( o , pattern->pattern2 , point ) : pattern->color2 ;

    tuple x = sub_tuples( &color2 , &color1 ) ;
    float frac = point->x - floorf( point->x ) ;
    mult_scalar_tuple( &x , frac ) ;
    return add_tuples( &x , &color1 );
}

tuple ring_pattern_at ( object* o , pattern* pattern , tuple* point )
{
    float dist = sqrtf(point->x * point->x + point->z * point->z );

    if ( (int)(floorf(dist)) % 2 == 0 )
    { // pick first color or pattern
        return pattern->isNested1 ? pattern_at_object( o , pattern->pattern1 , point ) : pattern->color1 ;
    }
    else
    {
        return pattern->isNested2 ? pattern_at_object( o , pattern->pattern2 , point ) : pattern->color2 ;
    }
}
tuple checker_pattern_at ( object* o ,  pattern* pattern , tuple* point )
{
    int dist = ((int)(floorf(point->x) + floorf(point->y) + floorf(point->z)) % 2) == 0 ;
    if ( dist )
    { // pick first color or pattern
        return pattern->isNested1 ? pattern_at_object( o , pattern->pattern1 , point ) : pattern->color1 ;
    }
    else
    {
        return pattern->isNested2 ? pattern_at_object( o , pattern->pattern2 , point ) : pattern->color2 ;
    }
}
tuple pattern_at_object (object* o , pattern *current_pattern , tuple* point )
{
    // convert world point ot object space , and then convert the point from object space to pattern space
    mat4 world_object ;
    mat4 object_pattern ;
    inverse_mat4( o->trans , world_object ) ;
    inverse_mat4( current_pattern -> pattern_trans , object_pattern ) ;

    tuple pt , ft ;
    multiply_mat4_tuple( world_object , point , &pt ) ;
    multiply_mat4_tuple( object_pattern , &pt , &ft ) ; // ft is not in pattern space

    return current_pattern->pattern_at( o , current_pattern , &ft ) ;
}

pattern get_pattern ( enum pattern_type type , int nested1 , int nested2 , void* color1 , void* color2 )
{
    // fill in the first field
    pattern ret ;
    ret.isNested1 = nested1 ;
    ret.isNested2 = nested2 ;
    ret.pattern1 = 0 ;
    ret.pattern2 = 0 ;
    ident_mat4(ret.pattern_trans) ;

    // determine what call function to use

    switch ( type )
    {
        case PATTERN_CHECKER:
            ret.pattern_at = checker_pattern_at ;
            break;
        case PATTERN_GRADIENT:
            ret.pattern_at = gradient_pattern_at ;
            break;
        case PATTERN_STRIPES:
            ret.pattern_at = stripe_pattern_at ;
            break;
        case PATTERN_RING:
            ret.pattern_at = ring_pattern_at ;
            break;
        default:
            fprintf( stderr , "unsupported pattern type ") ;
            break ;
    }

    if ( nested1 )
        ret.pattern1 = ( pattern *) color1 ;
    else
        ret.color1 = *(( tuple *) color1) ;

    if ( nested2 )
        ret.pattern2 = ( pattern *) color2 ;
    else
        ret.color2 = *((tuple *) color2 ) ;

    return ret ;
}


