#ifndef RAY_TRACER_FACTORIES_H
#define RAY_TRACER_FACTORIES_H

#include <math.h>
#include "defs.h"

tuple get_point ( float x , float y , float z ) ;
tuple get_vector ( float x , float y , float z ) ;
tuple get_color ( float r , float g , float b ) ;
tuple get_clear_color () ;
int compare_tuple ( tuple* tp1 , tuple* tp2 ) ;
tuple add_tuples ( tuple* tp1 , tuple* tp2 ) ;
tuple sub_tuples ( tuple* tp1 , tuple* tp2 ) ;// subtract tp2 from tp1
void neg_tuple ( tuple* tp1 ) ;
void mult_scalar_tuple ( tuple* tp1 , float scalar ) ;
float mag_tuple ( tuple* tp1 ) ;
void normalize_tuple ( tuple *tp1 ) ;
tuple cross_tuples ( tuple *tp1 , tuple *tp2 ) ;
tuple hadamard_prod ( tuple *tp1 , tuple *tp2 ) ;
float dot_tuples ( tuple *tp1 , tuple *tp2 ) ;


#endif //RAY_TRACER_FACTORIES_H
