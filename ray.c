// this file contains routines that work with rays
#include <stdio.h>
#include <math.h>
#include "defs.h"
#include "factories.h"
#include <stdlib.h>
#include <string.h>
#include "matrix_routines.h"
#include "ray.h"
#include "patterns.h"

ray get_ray ( tuple* origin , tuple* direction )
{
    ray ret ;
    ret.dir = *direction ;
    ret.org = *origin ;

    return ret;
}

tuple ray_pos ( ray* x , float t )
{
    tuple ret = x->dir ;
    mult_scalar_tuple( &ret , t );
    return add_tuples( &(x->org) , &ret ) ;
}

void intersect ( ray* r , object s , inter_collec* dest )
{
    // take the matrix of the get_sphere , invert it and then transform the ray using it
    mat4 inver ;
    inverse_mat4( s.trans , inver ) ;
    ray new_r = transform_ray( r , inver ) ;
    *dest = ( inter_collec ){0} ; // for sanity

    // determine what kind of shape we are working with

    switch ( s.type )
    {
        case SPHERE_OBJECT:
            intersect_sphere( &new_r , s , dest ) ;
            break ;
        case PLANE_OBJECT:
            intersect_plane ( &new_r , s , dest ) ;
            break ;
        default:
            fprintf( stderr , "shape type not supported\n" ) ;
            break ;
    }

}
void intersect_sphere ( ray *r , object s , inter_collec* dest )
{
    tuple sphere_to_ray = get_point (0,0,0) ;
    sphere_to_ray = sub_tuples( &(r->org) , &sphere_to_ray ) ;

    float a = dot_tuples( &(r->dir) , &(r->dir) ) ;
    float b = 2 * dot_tuples( &(r->dir) , &sphere_to_ray ) ;
    float c = dot_tuples( &sphere_to_ray , &sphere_to_ray ) - 1 ; // could have used value^2 , 1 because radius is 1

    float disc = b*b -4*a*c ;

    if ( disc < 0 ) // no solution
    {
        dest -> count = 0 ;
    }
    else
    {
        dest -> xs = malloc ( sizeof( intersection ) *  2 ) ;
        dest -> count = 2;
        dest -> xs[0].t = ( -b -sqrtf(disc)) / (2*a) ;
        dest -> xs[0].obj = s ;
        dest -> xs[1].t = ( -b +sqrtf(disc)) / (2*a) ;
        dest -> xs[1].obj = s ;
    }
}
void intersect_plane ( ray *r , object s , inter_collec* dest  )
{
    if ( fabsf(r->dir.y) < EPS ) // will *never* intersect the plane
    {
        dest->count = 0 ;
        return;
    }

    dest->xs = malloc ( sizeof(intersection) ) ;
    dest->count = 1 ;
    dest->xs[0].t = -(r->org.y) / r->dir.y ;
    dest->xs[0].obj = s ;
}
object get_sphere ()
{
    object sphere;
    ident_mat4( sphere.trans ) ;
    def_material(&sphere.mat) ;
    sphere.type = SPHERE_OBJECT ;

    return sphere ;
}
object get_plane ()
{
    object plane;
    ident_mat4( plane.trans ) ;
    def_material(&plane.mat) ;
    plane.type = PLANE_OBJECT ;

    return plane ;
}

int comp_intersections (const void* elem1 , const void* elem2 )
{
    intersection i1 = *((intersection *) elem1) ;
    intersection i2 = *((intersection *) elem2) ;

    if ( i1.t > i2.t )
        return 1 ;
    else if ( i1.t < i2.t )
        return -1 ;
    else
        return 0 ;
}

/*inter_collec intersections( int num , ... )
{
    inter_collec col = { num , 0 } ;
    col.xs = malloc ( sizeof(intersection) * num ) ;
    va_list ap;

    va_start(ap, num);

    for ( int i = 0 ; i < num ; ++i )
    {
        col.xs[i] = *( va_arg(ap, intersection * )) ;
    }

    // traverse rest of the arguments to find out minimum
    va_end(ap);

    //sort the list according to the t value
    qsort( col.xs , num , sizeof(intersection) , comp_intersections ) ;
    return col;
}*/

void destroy_coll ( inter_collec* col )
{
    if ( col->xs )
        free(col->xs) ;
}

intersection* hit( inter_collec* col )
{
    // search in the list for the first non zero t value
    for ( int i = 0 ; i < col->count ; ++i )
    {
        if ( col->xs[i].t >= 0 )
            return &(col->xs[i]) ;
    }

    return 0 ;
}

ray transform_ray ( ray* r , mat4 mat )
{
    // multiply both the direction and origin by this matrix
    ray ret ;
    tuple start ;
    multiply_mat4_tuple( mat , &(r->org) , &start ) ;
    tuple dire ;
    multiply_mat4_tuple( mat , &(r->dir) , &dire ) ;
    ret.dir = dire ;
    ret.org = start ;

    return ret;
}

void set_transform ( object *o , mat4 trans )
{
    memcpy( o->trans , trans , 16 * sizeof(float) );
}

tuple normal_at (object *s , tuple *world_p )
{
    // first revert the point to object coordinates
    tuple local , ret ;
    mat4 inv ;
    inverse_mat4( s->trans , inv ) ;
    multiply_mat4_tuple( inv , world_p , &local ) ;
    // calculate the local normal for the shape
    local = local_normal ( s , &local ) ;

    // convert the normal back to world coordinates by multiplying with transpose of the inverse
    mat4 final ;
    transpose_mat4( inv , final ) ;
    multiply_mat4_tuple( final , &local , &ret  ) ;
    ret.w = 0 ; // hack again, convert to vector
    normalize_tuple( &ret );

    return ret ;
}
tuple local_normal ( object* s , tuple* local_p )
{
    tuple normal ;
    switch ( s->type )
    {
        case SPHERE_OBJECT:
            normal = sphere_local_normal( s , local_p ) ;
            break ;
        case PLANE_OBJECT:
            normal = ( tuple ) { 0 , 1 , 0 , 0 } ; // plane is on XZ
            break ;
        default:
            fprintf( stderr , "error, shape not supported") ;
            break ;
    }

    return normal ;
}
tuple sphere_local_normal ( object *s , tuple* local_p  )
{ // just returns the normal vector in the local system
  // calculate the "local" normal by subtracting from the origin of the sphere which is at 0
  tuple ret = *local_p ;
  ret.w = 0 ; // hack , convert to vector
  return ret ;
}

tuple reflect ( tuple* in , tuple* normal )
{
    float temp = 2 * dot_tuples( in , normal ) ;
    mult_scalar_tuple( normal , temp ) ;
    return sub_tuples( in , normal ) ;
}

point_light get_plight ( tuple origin , tuple color )
{
    point_light l ;
    l.position = origin ;
    l.intensity = color ;

    return l;
}

void def_material ( material *def )
{
    def->color = get_color(1,1,1) ;
    def->ambient = 0.1f ;
    def->diffuse = 0.9f ;
    def->specular = 0.9f ;
    def->shininess = 200 ;
    def->has_pattern = 0 ;
}

tuple lighting ( material* mat , object* o , point_light* light , tuple* point , tuple* eye_dir , tuple* normal , int in_shadow )
{
    tuple ambient , diffuse , specular ;
    // we will calculate the 3 components of the phong shading model
    //first determine the base color of the object

    tuple used_color ;
    used_color = ( mat->has_pattern ) ? pattern_at_object( o , &mat->current_pattern , point ) : mat->color ;

    tuple eff_color = hadamard_prod( &(light->intensity) , &used_color ) ;
    tuple light_dir = sub_tuples( &(light->position) , point ) ;
    normalize_tuple( &light_dir ) ;
    //first calculate the ambient color
    ambient = eff_color ;
    mult_scalar_tuple( &ambient , mat->ambient ) ;

    // if the point is in shadow, then the ambient component suffices
    if ( in_shadow )
        return ambient ;

    // calculate if we can see light on this side of the surface
    float light_dot_normal = dot_tuples( &light_dir , normal ) ;

    if ( light_dot_normal < 0 ) // on the other side
    {
        diffuse = get_color ( 0 , 0 , 0 ) ;
        specular = get_color ( 0 , 0 , 0 ) ;
    }
    else
    {
        // second calculate the diffuse color
        diffuse = eff_color ;
        mult_scalar_tuple( &diffuse , light_dot_normal * mat->diffuse ) ;

        // calculate the specular component
        // first calculate the reflection vector
        neg_tuple( &light_dir );
        tuple reflected = reflect( &light_dir , normal ) ;
        float dot_eye_light = dot_tuples( &reflected , eye_dir ) ;

        if ( dot_eye_light < 0 ) // no specular
        {
            specular = get_color( 0 , 0 , 0 ) ;
        }
        else
        {
            float factor = pow( dot_eye_light , mat->shininess ) ;
            specular = light->intensity ;
            mult_scalar_tuple( &specular , factor * mat->specular ) ;
        }
    }

    tuple temp = add_tuples( &ambient , &diffuse ) ; // TODO: make a function for this
    return add_tuples( &temp , &specular ) ;
}

