#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "world_view.h"
#include "defs.h"
#include "ray.h"
#include "factories.h"
#include "trans_routines.h"
#include "matrix_routines.h"
#include "canvas.h"

void init_world (world *new)
{
    // create a default world where two sphere exist
    new->obj_count = 2 ;
    new->objects = malloc ( sizeof(object) * 2 ) ;
    new->light = get_plight( get_point (-10,10,-10 ) , get_color(1,1,1)) ;
    new->objects[0] = get_sphere() ;
    new->objects[1] = get_sphere() ;

    // configure the spheres beyond the default settings
    new->objects[0].mat.color = get_color(0.8f,1.0f,0.6f) ;
    new->objects[0].mat.diffuse = 0.7f ;
    new->objects[0].mat.specular = 0.2f ;

    scale( 0.5f , 0.5f , 0.5f , (new->objects[1].trans )) ;
}

void destroy_world ( world *dead )
{
    if ( dead->obj_count )
        free(dead->objects) ;
    dead->obj_count = 0;
}

void add_obj_world ( world* w , object* new_obj )
{
    w->obj_count++ ;
    w->objects = realloc( w->objects , sizeof(object) * w->obj_count ) ;
    // copy the object into its new place
    w->objects[w->obj_count-1] = *new_obj ;
}

void inter_ray_world ( ray* r , world* w , inter_collec *dest )
{
    // iterate over all the object in the world and intersect them

    // clear dest array to avoid problems
    *dest = ( inter_collec ) {0} ;

    for ( int i = 0 ; i < w->obj_count ; ++i )
    {
        inter_collec temp ;
        intersect( r , w->objects[i] , &temp ) ;
        merge_destroy( dest , &temp ) ;
    }

    // sort the list before returning
    qsort( dest->xs , dest->count , sizeof(intersection) , comp_intersections ) ;
}

void merge_destroy ( inter_collec *dest , inter_collec *inter )
{
    // merge the intersection collection to dest and then destroy it
    if ( !inter->count )
        return ;

    dest-> xs = realloc( dest->xs , (dest->count + inter->count) * sizeof( intersection )) ;
    // copy the intersections to the destination

    for ( int i = 0 ; i < inter->count ; ++i )
        dest->xs[dest->count + i] = inter->xs[i] ;

    dest->count += inter->count ;
    free ( inter->xs ) ;
}

void compute_contact ( ray* r , intersection* i , contact_calc* dest ) // computers contract information used for shading and such
{
    dest->t = i->t ;
    dest->obj = &(i->obj) ;
    dest->eye_v = r->dir ;
    neg_tuple(&(dest->eye_v)) ;

    dest->p_contact = ray_pos( r , dest->t ) ;
    dest->normal = sphere_normal( dest->obj , &dest->p_contact ) ;

    dest->inside = dot_tuples( &dest->normal , &dest->eye_v ) < 0 ? 1 : 0  ;
}

tuple shade_hit( world* w , contact_calc* calc )
{
    return lighting( &calc->obj->mat , &w->light , &calc->p_contact , &calc->eye_v , &calc->normal ) ;
}

tuple color_at ( world *w , ray *r )
{
    inter_collec interCollec ;
    intersection *inter ;
    inter_ray_world( r , w , &interCollec );
    if ( (inter = hit( &interCollec )) ) // if there is an intersection
    {
        // we have a hit, compute the paramters then shade
        contact_calc calculations ;
        compute_contact( r , inter , &calculations ) ;
        destroy_coll( &interCollec ) ;
        return shade_hit( w , &calculations ) ;
    }

    // free the intersection collection

    destroy_coll( &interCollec ) ;
    return get_color( 0 , 0 , 0 ) ; // black
}
/**
 * creates the view transform for the camera
 * @param to
 * @param from
 * @param up
 * @param res
 */
void view_transform ( tuple* to , tuple* from , tuple* up , mat4 res )
{
    // calculate forward vector
    tuple new_up = *up ;
    tuple forward = sub_tuples( to , from );
    normalize_tuple( &forward );
    normalize_tuple( &new_up );
    // calculate left vector using cross product
    tuple left = cross_tuples( &forward , &new_up ) ;
    // calculate the precise up vector, left cross forward
    tuple true_up = cross_tuples( &left , &forward ) ;

    // construct view matrix
    memset( res , 0 , 16 * sizeof(float) );
    res[0] = left.x ;
    res[1] = left.y ;
    res[2] = left.z ;
    res[4] = true_up.x ;
    res[5] = true_up.y ;
    res[6] = true_up.z ;
    res[8] = -forward.x ;
    res[9] = -forward.y ;
    res[10] = -forward.z ;
    res[15] = 1;

    // first translate then orient
    mat4 trans_mat ;
    translate( -from -> x , -from -> y , -from -> z , trans_mat ) ;
    mat4 result ;
    multiply_mat4( res , trans_mat , result ) ;

    memcpy( res , result , 16 * sizeof(float)) ;
}

void init_camera (int hsize , int vsize , float fov , camera *c )
{
    c->h_size = hsize ;
    c->v_size = vsize ;
    c->fov = fov ;
    ident_mat4( c->transform ) ;

    // calculate the pixel size
    float half_view = tan( fov/2 ) ;
    float aspect_ratio = hsize / ( float ) vsize ;

    if ( aspect_ratio >= 1 )
    {
        c->half_width = half_view ;
        c->half_height = half_view / aspect_ratio ;
    }
    else
    {
        c->half_width = half_view * aspect_ratio ; // TODO: possible error here , half_height should be divided only
        c->half_height = half_view ;
    }

    c->pixel_size = c->half_width*2 / hsize ;
}

void ray_for_pixel ( camera* c , int px , int py , ray *r )
{
    float x_offset = (px+0.5) * c->pixel_size ;
    float y_offset = (py+0.5) * c->pixel_size ;

    float world_x = c->half_width - x_offset ;
    float world_y = c->half_height - y_offset ;

    mat4 reverse ;

    inverse_mat4( c->transform , reverse ) ;
    tuple pixel ;
    tuple end = get_point (world_x , world_y , -1 ) ;
    multiply_mat4_tuple( reverse , &end , &pixel );
    end = get_point (0,0,0);
    tuple origin ;
    multiply_mat4_tuple( reverse , &end , &origin ) ;

    tuple direction = sub_tuples( &pixel , &origin ) ;
    normalize_tuple( &direction ) ;
    r->dir = direction ;
    r->org = origin ;
}

void render( camera *c , world *w )
{
    init_canvas(c->h_size,c->v_size) ;
    for ( int y = 0 ; y < c->v_size ; ++y )
        for ( int x = 0; x < c->h_size ; ++x )
        {
            if ( y == 5 && x == 5 )
            {
                int sdfdff = 0 ;
            }

            ray god_ray ;
            ray_for_pixel( c , x , y , &god_ray );
            tuple color = color_at( w , &god_ray ) ;
            canvas_write( color , x , y ) ;
        }
}


