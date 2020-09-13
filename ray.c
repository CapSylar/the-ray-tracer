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
#include "world_view.h"


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
            break;
        case CUBE_OBJECT:
            intersect_cube( &new_r , s , dest ) ;
            break;
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
    ident_mat4(sphere.trans);
    def_material(&sphere.mat);
    sphere.type = SPHERE_OBJECT;
    sphere.id = rand() ;

    return sphere;
}
// just for the lolz
object get_glass_sphere()
{
    object sphere = get_sphere() ;
    sphere.mat.transparency = 1.0f ;
    sphere.mat.refractive_index = 1.5f ;
    sphere.id = rand() ;

    return sphere ;
}
object get_plane ()
{
    object plane;
    ident_mat4( plane.trans ) ;
    def_material(&plane.mat) ;
    plane.id = rand() ;
    plane.type = PLANE_OBJECT ;

    return plane ;
}

object get_cube ()
{
    object cube;
    ident_mat4( cube.trans ) ;
    def_material( &cube.mat ) ;
    cube.id = rand();
    cube.type = CUBE_OBJECT ;

    return cube;
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
        case CUBE_OBJECT:
            normal = cube_local_normal( s , local_p ) ;
            break;
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
    tuple x = *normal ;
    mult_scalar_tuple( &x , temp ) ;
    return sub_tuples( in , &x ) ;
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
    def->reflective = 0 ;
    def->refractive_index = 1.0f ;
    def->transparency = 0 ;
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

tuple reflected_color ( world* w , contact_calc* calc , int depth_limit )
{
    if ( depth_limit <= 0 )
        return ( tuple ) { 0 , 0 , 0 , 0 } ; // black
    // calculate the reflective color at this point by throwing rays originating at this point into the world
    ray god_ray = get_ray( &calc->adjusted_p , &calc->reflectv ) ;
    tuple color = color_at( w , &god_ray , 1 , depth_limit-1 ) ;

    mult_scalar_tuple ( &color , calc->obj.mat.reflective ) ;
    return color ;
}

tuple refracted_color ( world* w , contact_calc* calc , int depth_limit )
{
    if ( depth_limit <= 0 || float_cmp(calc->obj.mat.transparency,0) )
        return ( tuple ) { 0 , 0 , 0, 0 } ; // black

    // calculate the refracted ray direction

    float n_ratio = calc->n1 / calc->n2 ;
    float inc_cos = dot_tuples( &calc->normal , &calc->eye_v ) ;
    float sin_sq = n_ratio * ( 1 - inc_cos*inc_cos ) ;

    if ( sin_sq > 1 ) // not possible, total internal reflection occurred
        return ( tuple ) { 0 , 0 , 0 , 0 } ;

    float cos_t = sqrtf( 1 - sin_sq ) ;
    tuple refracted_dir , temp1 ;
    refracted_dir = calc->normal ;
    mult_scalar_tuple( &refracted_dir , n_ratio * inc_cos - cos_t ) ;
    temp1 = calc->eye_v ;
    mult_scalar_tuple( &temp1 , n_ratio ) ;
    refracted_dir = sub_tuples( &refracted_dir , &temp1 ) ;

    ray god_ray = get_ray( &calc->inner_point , &refracted_dir ) ;
    tuple color = color_at( w , &god_ray , 1 , depth_limit-1 ) ;
    mult_scalar_tuple( &color , calc->obj.mat.transparency ) ;

    return color ;
}

float schlick_approx ( contact_calc* calc )
{
    // returns 1 means total reflectance and 0 means all refractance

    float cos_inc = dot_tuples( &calc->normal , &calc->eye_v ) ;

    // check for total internal reflection
    if ( calc->n1 > calc->n2 )
    {
        float ratio = calc->n1 / calc->n2 ;
        float sin_sqr = ratio*ratio * ( 1 - cos_inc*cos_inc ) ;

        if ( sin_sqr > 1 )
            return 1;

        cos_inc = sqrtf( 1 - sin_sqr ) ;
    }

    // christopher schlick approximation
    float r0 = (calc->n1 - calc->n2)/(calc->n1 + calc->n2) ;
    r0 = r0*r0 ;

    return r0 + ( 1 - r0 ) * powf( (1 - cos_inc) , 5 ) ;
}

// cube intersection logic

void intersect_axis( float axis_origin , float axis_direction , float *tmin , float *tmax )
{
    // return type indicates if intersections occured

    float plane1_d = -1 -axis_origin ;
    float plane2_d = 1 - axis_origin ;

    if ( fabs(axis_origin) >= 0 )
    {
        *tmin = plane1_d / axis_direction ;
        *tmax = plane2_d / axis_direction ;

        if ( *tmin > *tmax )
        {
            float temp = *tmin ;
            *tmin = *tmax ;
            *tmax = temp ;
        }
    }
    else
    {
        *tmin = INFINITY; // pray for availability
        *tmax = INFINITY;
    }

}

void intersect_cube ( ray* god_ray , object cube , inter_collec *collec )
{
    // the cube is really just a collection of 6 infinite planes

    float xmin,xmax,ymin,ymax,zmin,zmax ;
    intersect_axis( god_ray->org.x , god_ray->dir.x , &xmin , &xmax ) ;
    intersect_axis( god_ray->org.y , god_ray->dir.y , &ymin , &ymax ) ;
    intersect_axis( god_ray->org.z , god_ray->dir.z , &zmin , &zmax ) ;

    // get maxs of minimums and minimums of maxs, reuse xmin and xmax as tmin and tmax :)
    xmin = xmin > ymin ? ( xmin > zmin ? xmin : zmin  ) : (ymin > zmin ? ymin : zmin) ;
    xmax = xmax < ymax ? ( xmax < zmax ? xmax : zmax ) : ( ymax < zmax ? ymax : zmax ) ;

    if ( xmin > xmax ) // ray missed the cube
    {
        collec->count = 0 ;
    }
    else
    {
        collec->count = 2 ;
        collec->xs = malloc ( sizeof(intersection) * 2 ) ;
        collec->xs[0] = ( intersection ) { xmin , cube } ;
        collec->xs[1] = ( intersection ) { xmax , cube } ;
    }
}

tuple cube_local_normal ( object *s , tuple* local_p  )
{
    // the normal for an axis alligned bounding box is in the direction of the biggest component (x,y,z)
    // can't compute to 1 due to roundings, potentially dangerous
    tuple ret = {0} ;

    float absx = fabsf(local_p->x) , absy = fabsf(local_p->y) ,absz = fabsf( local_p->z );
    float max = absx > absy ? absx > absz ? absx : absz : absy > absz ? absy : absz ;

    if ( absx == max ) // normal is in the x direction
        ret.x = local_p->x ;
    else if ( max == absy )
        ret.y = local_p->y ;
    else
        ret.z = local_p->z ;

    return ret;
}

