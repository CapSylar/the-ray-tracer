#include "defs.h"
#include <math.h>

tuple get_point ( float x , float y , float z )
{
    return ( tuple ) { x , y , z , 1 } ;
}

tuple get_vector ( float x , float y , float z )
{
    return ( tuple ) { x , y , z , 0 } ;
}

tuple get_color ( float r , float g , float b )
{
    return ( tuple ) { r , g , b , 0 } ; // w component unused
}

int compare_tuple ( tuple* tp1 , tuple* tp2 )
{
    return ( float_cmp(tp1->x,tp2->x) && float_cmp(tp1->y,tp2->y) &&
            float_cmp(tp1->x,tp2->x) && float_cmp(tp1->w , tp2->w) ) ;
}

tuple add_tuples ( tuple* tp1 , tuple* tp2 )
{
    return ( tuple ) { tp1->x + tp2->x , tp1->y + tp2->y , tp1->z + tp2->z , tp1->w + tp2->w } ;
}

tuple sub_tuples ( tuple* tp1 , tuple* tp2 ) // subtract tp2 from tp1
{
    return ( tuple ) { tp1->x - tp2->x , tp1->y - tp2->y , tp1->z - tp2->z , tp1->w - tp2->w } ;
}

void neg_tuple ( tuple* tp1 )
{
    tp1->x = -tp1->x;
    tp1->y = -tp1->y;
    tp1->z = -tp1->z;
    tp1->w = -tp1->w;
}

void mult_scalar_tuple ( tuple* tp1 , float scalar )
{
    tp1->x *= scalar ;
    tp1->y *= scalar ;
    tp1->z *= scalar ;
    tp1->w *= scalar ;
}

float mag_tuple ( tuple* tp1 )
{
    float res = tp1->x * tp1->x + tp1->y * tp1->y + tp1->z * tp1->z + tp1->w * tp1->w ;
    return sqrt(res);
}

void normalize_tuple ( tuple *tp1 )
{
    float div = mag_tuple( tp1 );
    // divide all the components by div
    tp1->x /= div;
    tp1->y /= div;
    tp1->z /= div;
    tp1->w /= div;
}

float dot_tuples ( tuple *tp1 , tuple *tp2 )
{
    return (tp1->x * tp2->x) + (tp1->y * tp2->y) + (tp1->z * tp2->z) + (tp1->w * tp2->w) ;
}

tuple cross_tuples ( tuple *tp1 , tuple *tp2 )
{
    // do a 3x3 cross since 4x4 is unneeded and way too expensive
    return ( tuple ) { tp1->y * tp2->z - tp1->z * tp2->y , tp1->z * tp2->x - tp1->x * tp2->z ,
                       tp1->x * tp2-> y - tp1->y * tp2->x , 0 } ; // forced to be a vector
}

tuple hadamard_prod ( tuple *tp1 , tuple *tp2 )
{
    return ( tuple ) { tp1->x * tp2->x , tp1->y * tp2->y , tp1->z * tp2->z , tp1->w * tp2->w } ;
}


