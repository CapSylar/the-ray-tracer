#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "world_view.h"
#include "defs.h"
#include "ray.h"
#include "factories.h"
#include "trans_routines.h"
#include "matrix_routines.h"
#include "canvas.h"
#include "linked_list.h"
#include "interface.h"

void init_world (world *new)
{
    // create a default world where two sphere exist
    new->obj_count = 0 ;
    new->children = 0;
    new->light = get_plight( get_point (-10,10,-10 ) , get_color(1,1,1)) ;

    object* sp1 = get_sphere();
    object* sp2 = get_sphere();

    add_to_world( new , sp1 , OBJECT );
    add_to_world( new , sp2 , OBJECT );

    // configure the spheres beyond the default settings
    sp1->mat.color = get_color(0.8f, 1.0f, 0.6f) ;
    sp1->mat.diffuse = 0.7f;
    sp1->mat.specular = 0.2f;

    scale( 0.5f , 0.5f , 0.5f , (sp1->trans )) ;
}

void init_empty_world(world *new)
{ // empty but has light, like my brain
    new->children = 0;
    new->light = get_plight( get_point (-10,10,-10 ) , get_color(1,1,1)) ;
    new->obj_count = 0;
}

void destroy_world ( world *dead )
{
    // first we free all the objects and then free the shape_s children list from world
    for ( int i = 0 ; i < dead->obj_count ; ++i )
    {
        free(dead->children[i].child);
    }

    if ( dead->obj_count )
        free(dead->children);

    dead->obj_count = 0;
}

void add_to_world( world* w , void* new_grp , enum child_type type )
{
    w->children = realloc(w->children , sizeof(shape_s) * (w->obj_count + 1) ) ;
    w->children[w->obj_count].child = new_grp;
    w->children[w->obj_count++].type = type;
}

void inter_ray_world ( ray* r , world* w , inter_collec *dest )
{
    // iterate over all the object in the world and intersect them
    // clear dest array to avoid problems

    for ( int i = 0 ; i < w->obj_count ; ++i )
    {
        shape_s child = w->children[i];

        if ( child.type == GROUP )
            inter_ray_group( r , child.child , dest ) ;
        else
            intersect(r , child.child , dest ) ;

    }

    // sort the list before returning
    qsort( dest->xs , dest->count , sizeof(intersection) , comp_intersections ) ;
}

void compute_contact ( ray* r , intersection* i , contact_calc* dest, inter_collec* collec ) // computers contract information used for shading and such
{
    dest->t = i->t;
    dest->obj = (i->obj);
    dest->eye_v = r->dir;
    neg_tuple(&(dest->eye_v));

    dest->p_contact = ray_pos(r, dest->t);
    dest->normal = normal_at(dest->obj, &dest->p_contact);

    dest->inside = dot_tuples(&dest->normal, &dest->eye_v) < 0 ? 1 : 0;

    if ( dest->inside ) // flip the normal for correct calculations/
        neg_tuple( &dest->normal ) ;
    // calculate the reflection vector

    dest->reflectv = reflect( &r->dir , &dest->normal ) ;

    tuple offset = dest->normal;
    mult_scalar_tuple(&offset, EPS*200 ); // TODO: EPS*200 used is too big, there seems to be a problem
    dest->adjusted_p = add_tuples( &offset , &dest->p_contact );

    // same for the inner_point except we subtract the offset

    dest->inner_point = sub_tuples( &dest->p_contact , &offset ) ;

    // compute n1 and n2 for refraction
    linked_l list = {0} ;

    for ( int j = 0 ; j < collec->count ; ++j )
    {
        if ( i->t == collec->xs[j].t ) // hit occurred here
        {
            if (is_empty(&list)) // entered the first object
            {
                dest->n1 = 1; // empty space
            }
            else
            {
                dest->n1 = list.end->i->obj->mat.refractive_index;
            }
        }

        if ( list_contains( &list , collec->xs[j].obj ) )
        {
            remove_inter( &list , &collec->xs[j] ) ;
        }
        else
        {
            add_inter( &list , &collec->xs[j] ) ;
        }

        if ( i->t == collec->xs[j].t )
        {
            if ( is_empty(&list))
                dest->n2 = 1 ;
            else
                dest->n2 = list.end->i->obj->mat.refractive_index ;

            break ;
        }
    }

    free_list( &list ) ;

}
tuple shade_hit( world* w , contact_calc * calc , int calc_shadows , int depth_limit )
{
    // first we see if the point is in shadow
    int in_shadow = ( calc_shadows ) ? is_shadowed( w , &calc->adjusted_p ) : 0 ;
    // tests if the point is shadowed by another object, cast a ray from point to see if it arrived to the light source
    tuple surface =  lighting( &calc->obj->mat , calc->obj ,&w->light , &calc->adjusted_p , &calc->eye_v , &calc->normal , in_shadow ) ;

    tuple reflected = reflected_color( w , calc , depth_limit ) ;
    tuple refracted = refracted_color( w , calc, depth_limit ) ;

    if ( calc->obj->mat.transparency > 0 && calc->obj->mat.reflective > 0 ) // use fresnel
    {
        float approx = schlick_approx(calc) ;
        mult_scalar_tuple( &reflected , approx ) ;
        mult_scalar_tuple( &refracted , 1 - approx ) ;
    }

    reflected = add_tuples( &surface , &reflected );
    tuple final = add_tuples( &refracted , &reflected ) ;

    return final;
}

tuple color_at ( world *w , ray *r , int calc_shadows , int depth_limit )
{
    inter_collec inter_collec;
    init_list(&inter_collec);

    intersection *inter ;

    inter_ray_world( r , w , &inter_collec );
    if ( (inter = hit( &inter_collec )) ) // if there is an intersection
    {
        // we have a hit, compute the parameters then shade
        contact_calc calculations ;
        compute_contact( r , inter , &calculations , &inter_collec ) ;
        destroy_list(&inter_collec);
        return shade_hit( w , &calculations , calc_shadows , depth_limit ) ;
    }

    destroy_list(&inter_collec);
    return get_clear_color() ;
}

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
    /*Since the camera is always one unit away from the actual canvas, to calculate the canvas' (width/2)  we just use tan(POV/2)
     * aspect ratio is the horizontal dimension / vertical dimension
     * */

    float half_view = tanf( fov/2 ) ;
    float aspect_ratio = hsize / ( float ) vsize ;

/*    if ( aspect_ratio >= 1 )
    {
        c->half_width = half_view ;
        c->half_height = half_view / aspect_ratio ;
    }
    else
    {
        c->half_width = half_view * aspect_ratio ; // TODO: possible error here , half_height should be divided only
        c->half_height = half_view  ;
    }*/

    c->half_width = half_view;
    c->half_height = half_view / aspect_ratio;

    c->pixel_size = c->half_width*2 / hsize ;
}

void ray_for_pixel ( camera* c , int px , int py , ray *r )
{
    float x_offset = (px+0.5f) * c->pixel_size ;
    float y_offset = (py+0.5f) * c->pixel_size ;

    float world_x = c->half_width - x_offset ;
    float world_y = c->half_height - y_offset ;

    tuple pixel ;
    tuple end = get_point (world_x , world_y , -1 ) ;
    multiply_mat4_tuple( c->inverse , &end , &pixel );
    end = get_point (0,0,0);
    tuple origin ;
    multiply_mat4_tuple( c->inverse , &end , &origin ) ;

    tuple direction = sub_tuples( &pixel , &origin ) ;
    normalize_tuple( &direction ) ;
    r->dir = direction ;
    r->org = origin ;
}

/*void render( camera *c , world *w )
{
    init_canvas(c->h_size,c->v_size) ;
    for ( int y = 0 ; y < c->v_size ; ++y )
    {
        for (int x = 0; x < c->h_size; ++x)
        {
            ray god_ray;
            ray_for_pixel(c, x, y, &god_ray);
            tuple color = color_at(w, &god_ray, 1, 3);
            canvas_write(color, x, y);
            interface_write(color, x, y);
        }

        update_interface();
    }
}*/

int is_shadowed ( world* w , tuple *point)
{
    // tests if the point is shadowed by another object, cast a ray from point to see if it arrived to the light source
    tuple point_to_light = sub_tuples( &w->light.position , point ) ;
    float distance = mag_tuple(&point_to_light) ;
    normalize_tuple( &point_to_light ) ;

    // create a ray
    ray god_ray = get_ray( point , &point_to_light ) ;
    // cast the ray into the world
    inter_collec intersections ;
    init_list( &intersections );
    inter_ray_world( &god_ray , w , &intersections ) ;
    intersection* inter = hit( &intersections ) ;
    int isHit = inter != 0 ;
    float t = isHit ? inter->t : -1  ;
    destroy_list( &intersections ) ; // free the collection

    return ( isHit && t < distance) ; // there is an object lying between the point and the light source
}

void precompute_inv_world( world* w )
{

    // enter the world and precompute the inverse transform for both groups and children
    for ( int i = 0 ; i < w->obj_count ; ++i )
    {
        precompute_inv_shape( &w->children[i] );
    }
}

void precompute_inv_shape( shape_s* child )
{
    float *src, *dest;

    if ( child->type == OBJECT )
    {
        src = ((object *)(child->child))->trans ;
        dest = ((object *)(child->child))->inverse_trans;
    }
    else
    {
        group* x = ((group *)(child->child));
        src = x->trans ;
        dest = x->inverse_trans;

        for ( int j = 0 ; j < x->count ; ++j )
        {
            precompute_inv_shape( &x->children[j] );
        }
    }

    gluInvertMatrix( src , dest );
}


