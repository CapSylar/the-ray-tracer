#ifndef RAY_TRACER_PATTERNS_H
#define RAY_TRACER_PATTERNS_H

#include "defs.h"

//tuple stripe_pattern_at (pattern* pattern , tuple* point ) ;
tuple stripe_pattern_at ( object* o , pattern* pattern , tuple* point ) ;
tuple gradient_pattern_at ( object* o ,  pattern* pattern , tuple* point  );
tuple ring_pattern_at ( object* o , pattern* pattern , tuple* point );
pattern get_pattern ( enum pattern_type type , int nested1 , int nested2 , void* color1 , void* color2 ) ;
tuple pattern_at_object (object* o , pattern *current_pattern , tuple* point );
tuple checker_pattern_at ( object* o , pattern* pattern , tuple* point ) ;
#endif //RAY_TRACER_PATTERNS_H
