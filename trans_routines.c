#include "trans_routines.h"
#include "defs.h"
#include <string.h>
#include <math.h>

void translate ( float x , float y , float z , mat4 res )
{
    memset( res , 0 , 16 * sizeof(float)  ) ;
    res[0] = res[5] = res[10] = res[15] = 1 ;
    res[3] = x ;
    res[7] = y ;
    res[11] = z ;
}

void scale ( float x , float y , float z , mat4 res )
{
    memset( res , 0 , 16 * sizeof(float)  ) ;
    res[0] = x ;
    res[5] = y ;
    res[10] = z ;
    res[15] = 1 ;
}

void rotate_x ( float angle , mat4 res )
{
    memset( res , 0 , 16 * sizeof(float)  ) ;
    res[0] = res[15] = 1 ;
    res[5] = res[10] = cosf(angle) ;
    res[9] = sinf(angle) ;
    res[6] = -res[9] ;
}

void rotate_y ( float angle , mat4 res )
{
    memset( res , 0 , 16 * sizeof(float)  ) ;
    res[0] = res[10] = cosf(angle) ;
    res[2] = sinf(angle) ;
    res[8] = -res[2] ;
    res[5] = res[15] = 1 ;
}

void rotate_z ( float angle , mat4 res )
{
    memset( res , 0 , 16 * sizeof(float)  ) ;
    res[0] = res[5] = cosf(angle) ;
    res[4] = sinf(angle) ;
    res[1] = -res[4] ;
    res[15] = res[10] = 1 ;
}

void shear ( float x_y , float x_z , float y_x , float y_z , float z_x , float z_y , mat4 res )
{
    memset( res , 0 , 16 * sizeof(float)  ) ;
    res[0] = res[5] = res[10] = res[15] = 1;
    res[1] = x_y ;
    res[2] = x_z ;
    res[4] = y_x ;
    res[6] = y_z ;
    res[8] = z_x ;
    res[9] = z_y ;
}