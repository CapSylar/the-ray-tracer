// contains functions and definitions for the canvas that will store and produce the final image
#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

struct canvas
{
    int row ;
    int col ;

    tuple* mem;

} canvas ;

int init_canvas ( int row , int col )
{
    canvas.row = row ;
    canvas.col = col ;

    canvas.mem = malloc ( sizeof(tuple) * row * col ) ;

    return canvas.mem == 0 ; // true on error
}

void canvas_write ( tuple color , int x , int y )
{
    if ( x < 0 || x >= canvas.col || y < 0 || y >= canvas.row ) // out of bounds check
        return;

    canvas.mem[x + y*canvas.col] = color ;
}

void canvas_ppm () // converts the canvas to ppm file format
{
    int len = 20 + canvas.row * canvas.col * 12 + 1; // 20 for header , 12 for each line and 1 for terminator
    char* ppm_str = malloc ( sizeof(char) * len ) ;
    int header_len = snprintf( ppm_str , 20 , "P3\n%d %d\n255\n" , canvas.row , canvas.col ); // header info
    char *running = ppm_str + header_len ;
    // save the pixels to the string, each on a separate line
    for ( int i = 0 ; i < canvas.col * canvas.row ; ++i )
    {
        tuple current = canvas.mem[i] ;
        running += snprintf( running , 12 , "%d %d %d\n", (int)(current.x * 255) , (int)(current.y * 255) , (int)(current.z * 255) ) ;

        if ( current.x || current.y || current.z )
            printf("hit\n");
    }

    *running = 0 ;

    // now save the file
    FILE* fptr = fopen( "ray.ppm" , "wb");
    fputs( ppm_str , fptr );

    fclose(fptr) ;
}
