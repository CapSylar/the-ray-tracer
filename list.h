#ifndef RAY_TRACER_LIST_H
#define RAY_TRACER_LIST_H

#include "defs.h"

typedef struct
{
    int size ; // max number of elements
    int count; // current number of elements we have
    intersection *xs ;

} inter_collec ;

static void expand_list ( inter_collec* list ) ;
void init_list ( inter_collec* list ) ;
void list_add ( inter_collec* list , intersection new ) ;
void destroy_list ( inter_collec* list ) ;
void clear_list ( inter_collec* list ) ;

#endif //RAY_TRACER_LIST_H
