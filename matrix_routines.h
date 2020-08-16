#ifndef RAY_TRACER_MATRIX_ROUTINES_H
#define RAY_TRACER_MATRIX_ROUTINES_H

#include "defs.h"

int compare_mat4 ( mat4 x , mat4 y ) ;
int compare_mat3 ( mat3 x , mat3 y ) ;
int compare_mat2 ( mat2 x , mat2 y ) ;
void multiply_mat4 (mat4 x , mat4 y , mat4 res) ;
void multiply_mat4_tuple ( mat4 x , tuple* y , tuple* res ) ;
void transpose_mat4 ( mat4 x , mat4 res ) ;
float determ_mat2 ( mat2 x ) ;
void submat_mat3 ( mat3 x , mat2 y , int r , int c ) ;
void submat_mat4 ( mat4 x , mat3 y , int r , int c ) ;
float minor_mat3 ( mat3 x , int row , int col ) ;
float cofactor_mat3 ( mat3 x , int row , int col );
float determ_mat3 ( mat3 x ) ;
float cofactor_mat4 ( mat4 x , int row , int col ) ;
float determ_mat4 ( mat4 x );
int inverse_mat4 ( mat4 x , mat4 res ) ;
void ident_mat4 ( mat4 res ) ;
float minor_mat4 ( mat4 x , int row , int col ) ;


#endif //RAY_TRACER_MATRIX_ROUTINES_H
