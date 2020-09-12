#include "linked_list.h"
#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

void remove_inter ( linked_l* list , intersection* inter )
{
    // remove the node that has the same object

    for ( node* i = list->start , *last = 0 ; i ; )
    {
        if( i->i->obj.id == inter->obj.id )
        {
            // delete the node
            // check if this is the first node
            if ( !last )
            {
                list->start = i->next ;
            }
            else
                last->next = i->next ;

            // check if we are the end
            if ( list->end == i )
                list->end = last ;

            free(i) ; // what the actual fuckh
        }
        else
            last = i ;



        i = last ? last -> next : 0  ;

    }
}

void add_inter ( linked_l* list , intersection *inter )
{
    node *new_node = malloc ( sizeof(node) ) ;
    new_node->i = inter ;
    new_node->next = 0 ;

    // if this is the first node
    if ( !list->start )
        list->start = new_node ;
    if ( list->end )
        list->end->next = new_node ;
    list->end = new_node ;
}

int list_contains ( linked_l* list , object *o )
{
    // search for the intersection in the list, linear search is more than enough for our purposes
    for ( node* i = list->start ; i ; i=i->next )
        if ( i->i->obj.id == o->id )
            return 1 ;
    return 0;
}

int is_empty ( linked_l* list )
{
    return list->start ? 0 : 1 ;
}

void print_list ( linked_l* list )
{
    for ( node* i = list->start ; i ; i=i->next )
        printf("node: %g\n", i->i->t ) ;
}
