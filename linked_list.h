#ifndef RAY_TRACER_LINKED_LIST_H
#define RAY_TRACER_LINKED_LIST_H

#include "defs.h"

typedef struct node
{
    intersection *i ;
    struct node *next;

} node ;

typedef struct linked_l
{
    node* start , *end ;
} linked_l ;

void remove_inter ( linked_l* list , intersection* inter ) ;
void add_inter ( linked_l* list , intersection *inter ) ;
void print_list ( linked_l* list ) ;
int is_empty ( linked_l* list ) ;
int list_contains ( linked_l* list , object *o ) ;


#endif //RAY_TRACER_LINKED_LIST_H
