#include "list.h"
#include <stdlib.h>

void init_list ( inter_collec* list )
{
    list->count = 0;
    list->size = 10 ;
    list->xs = malloc (sizeof(intersection) * list->size ) ;
}

void list_add ( inter_collec* list , intersection new )
{
    if ( list->count >= list->size ) // need to expand
        expand_list( list );

    list->xs[list->count++] = new ;
}

static void expand_list ( inter_collec* list )
{
    list->size += 10;
    list->xs = realloc ( list->xs , list->size * sizeof(intersection) ) ;
}

void destroy_list ( inter_collec* list )
{
    list->count = 0;

    if ( list->size )
        free(list->xs);

    list->xs = 0 ;
    list->size = 0;
}

void clear_list ( inter_collec* list )
{
    // keep memory :)
    list->count = 0;
}
