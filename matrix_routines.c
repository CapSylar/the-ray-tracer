//this file contains some routines for matrix operations
#include "defs.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "matrix_routines.h"

int compare_mat4 ( mat4 x , mat4 y )
{
    for ( int i = 0 ; i < 16 ; ++i )
    {
        if ( !(float_cmp(x[i] , y[i])) )
            return 1;
    }

    return 0;
}

int compare_mat3 ( mat3 x , mat3 y )
{
    for ( int i = 0 ; i < 9 ; ++i )
    {
        if ( !(float_cmp(x[i] , y[i])) )
            return 1;
    }

    return 0;
}

int compare_mat2 ( mat2 x , mat2 y )
{
    for ( int i = 0 ; i < 4 ; ++i )
    {
        if ( !(float_cmp(x[i] , y[i])) )
            return 1;
    }

    return 0;
}

void multiply_mat4 (mat4 x , mat4 y , mat4 res )
{
    for ( int row = 0 ; row < 4 ; ++row )
        for ( int col = 0 ; col < 4 ; ++col )
        {
            res[col + row*4] = x[row*4] * y[col] + x[row*4 + 1] * y[4 + col] +
                            x[row*4 +2] * y[8 +col] + x[row*4+3] * y[12 +col] ;
        }
}

void multiply_mat4_tuple ( mat4 x , tuple* y , tuple* res )
{
    res->x = x[0] * y->x + x[1] * y->y + x[2] * y->z + x[3] * y ->w ;
    res->y = x[4] * y->x + x[5] * y->y + x[6] * y->z + x[7] * y ->w ;
    res->z = x[8] * y->x + x[9] * y->y + x[10] * y->z + x[11] * y ->w ;
    res->w = x[12] * y->x + x[13] * y->y + x[14] * y->z + x[15] * y ->w ;
}

void transpose_mat4 ( mat4 x , mat4 res )
{
    for ( int row = 0 ; row < 4 ; ++row )
        for ( int col = 0 ; col < 4 ; ++col )
            res[col*4 + row] = x[row*4 + col] ;
}

float determ_mat2 ( mat2 x )
{
    return x[0] * x[3] - x[1] * x[2] ;
}

void submat_mat3 ( mat3 x , mat2 y , int r , int c )
{
    int i = 0 ;
    for ( int row = 0 ; row < 3 ; ++row )
    {
        if ( row == r )
            continue;
        for (int col = 0; col < 3; ++col)
        {
            if ( col == c )
                continue;
            y[i++] = x[row*3 + col] ;
        }
    }
}

void submat_mat4 ( mat4 x , mat3 y , int r , int c )
{
    int i = 0 ;
    for ( int row = 0 ; row < 4 ; ++row )
    {
        if ( row == r )
            continue;
        for (int col = 0; col < 4; ++col)
        {
            if ( col == c )
                continue;
            y[i++] = x[row*4 + col] ;
        }
    }
}

float minor_mat3 ( mat3 x , int row , int col )
{
    mat2 res ;
    submat_mat3( x , res , row , col ) ;
    return determ_mat2( res ) ;
}

float cofactor_mat3 ( mat3 x , int row , int col )
{
    float res = minor_mat3( x , row , col ) ;
    return (( row + col ) % 2) ? -res : res ;
}

float minor_mat4 ( mat4 x , int row , int col )
{
    mat3 res ;
    submat_mat4( x , res , row , col );
    return determ_mat3( res ) ;
}

 float cofactor_mat4 ( mat4 x , int row , int col )
{
    float res = minor_mat4( x , row , col ) ;
    return ((row+col) % 2) ? -res : res ;
}

float determ_mat3 ( mat3 x )
{
    return x[0] * cofactor_mat3( x , 0 , 0 ) + x[1] * cofactor_mat3( x , 0 , 1 ) + x[2] * cofactor_mat3( x , 0 , 2 );
}

float determ_mat4 ( mat4 x )
{
    return x[0] * cofactor_mat4( x , 0 , 0 ) + x[1] * cofactor_mat4(x,0,1) + x[2] * cofactor_mat4(x,0,2) + x[3] * cofactor_mat4(x,0,3) ;
}

int inverse_mat4 ( mat4 x , mat4 res )
{

    float det ;
    if ( float_cmp( (det = determ_mat4( x )) , 0 ) ) // matrix cannot be inverted
        return 1;

    for ( int row = 0 ; row < 4 ; ++row )
        for ( int col = 0 ; col < 4 ; ++col )
        {
            res[col*4+row] = cofactor_mat4( x , row , col ) / det ;
        }

    return 0 ;
}

int gluInvertMatrix( mat4 m , mat4 invOut )
{ // from MESA implementation

    mat4 inv;
    float det;
    int i;

    inv[0] = m[5]  * m[10] * m[15] -
             m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] +
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] -
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
             m[4]  * m[11] * m[14] +
             m[8]  * m[6]  * m[15] -
             m[8]  * m[7]  * m[14] -
             m[12] * m[6]  * m[11] +
             m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
             m[4]  * m[11] * m[13] -
             m[8]  * m[5] * m[15] +
             m[8]  * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
              m[4]  * m[10] * m[13] +
              m[8]  * m[5] * m[14] -
              m[8]  * m[6] * m[13] -
              m[12] * m[5] * m[10] +
              m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
             m[1]  * m[11] * m[14] +
             m[9]  * m[2] * m[15] -
             m[9]  * m[3] * m[14] -
             m[13] * m[2] * m[11] +
             m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
             m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15] +
             m[8]  * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
             m[0]  * m[11] * m[13] +
             m[8]  * m[1] * m[15] -
             m[8]  * m[3] * m[13] -
             m[12] * m[1] * m[11] +
             m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
              m[0]  * m[10] * m[13] -
              m[8]  * m[1] * m[14] +
              m[8]  * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
             m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15] +
             m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
             m[0]  * m[7] * m[14] +
             m[4]  * m[2] * m[15] -
             m[4]  * m[3] * m[14] -
             m[12] * m[2] * m[7] +
             m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
              m[0]  * m[7] * m[13] -
              m[4]  * m[1] * m[15] +
              m[4]  * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
              m[0]  * m[6] * m[13] +
              m[4]  * m[1] * m[14] -
              m[4]  * m[2] * m[13] -
              m[12] * m[1] * m[6] +
              m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
             m[1] * m[7] * m[10] +
             m[5] * m[2] * m[11] -
             m[5] * m[3] * m[10] -
             m[9] * m[2] * m[7] +
             m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
              m[0] * m[7] * m[9] +
              m[4] * m[1] * m[11] -
              m[4] * m[3] * m[9] -
              m[8] * m[1] * m[7] +
              m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return 1;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return 0;
}

void ident_mat4 ( mat4 res )
{
    memset( res , 0 , 16 * sizeof(float) );
    res[0] = res[5] = res[10] = res[15] = 1 ;
}