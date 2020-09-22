#include "interface.h"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include "defs.h"

uint32_t *screen ; // memory for main display

static int w;
static int h;

SDL_Texture *texture;
SDL_Renderer *renderer ;

int init_interface( int width , int height )
{
    w = width ;
    h = height ;

    if ( SDL_Init ( SDL_INIT_VIDEO ) != 0 )
    {
        fprintf(stderr, "%s\n", SDL_GetError() );
        return EXIT_FAILURE ;
    }

    screen = malloc ( 4 * w * h ) ;

    SDL_Window *window = SDL_CreateWindow( "ray tracing...", SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED, 700 , 700 ,
                                           0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture( renderer , SDL_PIXELFORMAT_ARGB8888 ,
                                 SDL_TEXTUREACCESS_STREAMING , width , height ) ;

    return EXIT_SUCCESS ;
}

void update_interface()
{
    SDL_UpdateTexture( texture , 0 , screen , w * 4 ) ;

    SDL_RenderCopy( renderer , texture , NULL , NULL ) ;
    SDL_RenderPresent( renderer ) ;
}

void interface_write( tuple color , int x , int y )
{
    // using ARGB8888 format, colors should be stored accordingly

    if ( color.x > 1 )
        color.x = 1 ;
    if ( color.y > 1 )
        color.y = 1 ;
    if ( color.z > 1 )
        color.z = 1 ;


    if ( x >= w || y >= h )
    {
        fprintf( stderr , "error: invalid index while trying to write to SDL buffer\n");
        return;
    }

    uint32_t id = (255 << 24) | (int)(color.x * 255) << 16 | (int)(color.y * 255) << 8 | (int)(color.z * 255) ;
    screen[ y*w + x ] = id ;
}

int poll_inputs_exit()
{
    int halt = 0 ;
    static uint8_t mapped_key ;
    SDL_Event event;

    while (SDL_PollEvent(&event))
        switch (event.type)
        {
            case SDL_QUIT: // handles x button on the upper right of the window
                halt = 1;
                break;

            case SDL_KEYUP:
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_ESCAPE:
                        halt = 1 ;
                        break ;
                    default :
                        break;
                }
                break ;
            default:
                break ;
        }

    return halt ;
}

void close_interface()
{
    free(screen) ;
    SDL_Quit() ;
}
