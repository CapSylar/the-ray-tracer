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

    normalize_tuple(&ret.dir);

    return ret;
}

tuple ray_pos ( ray* x , float t )
{
    tuple ret = x->dir ;
    mult_scalar_tuple( &ret , t );
    return add_tuples( &(x->org) , &ret ) ;
}

void intersect ( ray* r , object* s , inter_collec* dest )
{
    ray new_r = transform_ray( r , s->inverse_trans ) ;

    // determine what kind of shape we are working with

    switch ( s->type )
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
        case CYLINDER_OBJECT:
            intersect_cylinder( &new_r , s , dest ) ;
            break;
        default:
            fprintf( stderr , "shape type not supported: %d \n" , s->type ) ;
            break ;
    }

}

void intersect_sphere ( ray *r , object* s , inter_collec* dest )
{
    tuple sphere_to_ray = get_point (0,0,0) ;
    sphere_to_ray = sub_tuples( &(r->org) , &sphere_to_ray ) ;

    float a = dot_tuples( &(r->dir) , &(r->dir) ) ;
    float b = 2 * dot_tuples( &(r->dir) , &sphere_to_ray ) ;
    float c = dot_tuples( &sphere_to_ray , &sphere_to_ray ) - 1 ; // could have used value^2 , 1 because radius is 1

    float disc = b*b -4*a*c ;

    if ( disc >= 0 )
    {
        intersection inter1 = { .obj = s , .t = ( -b -sqrtf(disc)) / (2*a) };
        intersection inter2 = { .obj = s , .t = ( -b +sqrtf(disc)) / (2*a) };
        list_add( dest , inter1 );
        list_add( dest , inter2 );
    }
}

void intersect_cylinder ( ray* r , object* s , inter_collec* dest )
{
    // just a circle and ray intersection

    float a = r->dir.x*r->dir.x + r->dir.z*r->dir.z ;

    if ( !float_cmp(a , 0 )) // quick out
    {
        float b = 2 * r->org.x * r->dir.x + 2 * r->org.z * r->dir.z;
        float c = r->org.x * r->org.x + r->org.z * r->org.z - 1;

        float d = b * b - 4 * a * c;

        if (d < 0)
            return;

        float inter1 = (-b - sqrtf(d)) / (2 * a);
        float inter2 = (-b + sqrtf(d)) / (2 * a);

        if (inter1 > inter2) // swap to keep the list sorted
        {
            float temp = inter1;
            inter1 = inter2;
            inter2 = temp;
        }

        // check is we exceeded the limits
        a = r->org.y + r->dir.y * inter1;

        if (a < s->max && a > s->min) // gets rendered
            list_add( dest , (intersection) {inter1, s} );

        a = r->org.y + r->dir.y * inter2;
        if (a < s->max && a > s->min) // gets rendered
            list_add( dest , (intersection) { inter2 , s  } );
    }

    intersect_cyl_caps( r , s , dest ) ;
}

void intersect_cone( ray* r , object s , inter_collec* dest )
{
    float a = r->dir.x*r->dir.x - r->dir.y*r->dir.y + r->dir.z*r->dir.z ;
    float b = 2*r->org.x*r->dir.x - 2*r->org.y*r->dir.y + 2*r->org.z*r->dir.z ;
    float c = r->org.x*r->org.x - r->org.y*r->org.y + r->org.z*r->org.z ;
}

void intersect_plane ( ray *r , object* s , inter_collec* dest  )
{
    if ( fabsf(r->dir.y) < EPS ) // will *never* intersect the plane
    {
        return;
    }

    intersection inter1 = { .obj = s , .t = -(r->org.y) / r->dir.y };
    list_add( dest , inter1 );
}
object* get_sphere ()
{
    object* sphere = malloc ( sizeof(object) ) ;
    sphere->parent = 0;
    ident_mat4(sphere->trans);
    def_material(&sphere->mat);
    sphere->type = SPHERE_OBJECT;
    sphere->id = rand() ;

    return sphere;
}
// just for the lolz
object* get_glass_sphere()
{
    object* sphere = get_sphere() ;
    sphere->mat.transparency = 1.0f ;
    sphere->mat.refractive_index = 1.5f ;
    sphere->mat.reflective = 0.9f ;
    sphere->id = rand() ;

    return sphere ;
}
object* get_plane ()
{
    object* plane = malloc ( sizeof(object) ) ;
    plane->parent = 0;
    ident_mat4( plane->trans ) ;
    def_material(&plane->mat) ;
    plane->id = rand() ;
    plane->type = PLANE_OBJECT ;

    return plane ;
}

object* get_cube ()
{
    object* cube = malloc ( sizeof(object) ) ;
    cube->parent = 0;
    ident_mat4( cube->trans ) ;
    def_material( &cube->mat ) ;
    cube->id = rand();
    cube->type = CUBE_OBJECT ;

    return cube;
}

object* get_cylinder()
{
    object* cylinder = malloc ( sizeof(object) ) ;
    cylinder->parent = 0;
    ident_mat4(cylinder->trans) ;
    def_material( &cylinder->mat ) ;
    cylinder->id = rand() ;
    cylinder->type = CYLINDER_OBJECT ;

    cylinder->min = -INFINITY;
    cylinder->max = INFINITY;
    cylinder->closed = 0 ;

    return cylinder;
}

object* get_cone()
{
    object* cone = malloc ( sizeof(object) ) ;
    cone->parent = 0;
    ident_mat4(cone->trans);
    def_material(&cone->mat);
    cone->id = rand() ;
    cone->type = CONE_OBJECT;

    cone->min = -INFINITY;
    cone->max = INFINITY;
    cone->closed = 0;

    return cone;
}

group* get_group()
{
    group* new_grp = malloc ( sizeof(group) );
    new_grp->parent = 0;
    new_grp->count = 0;
    new_grp->children = 0;
    ident_mat4(new_grp->trans);

    return new_grp;
}

void add_g_gchild (group *father , shape_s* son )
{
    father->children = realloc( father->children , ++father->count * sizeof(shape_s) ) ; //TODO: change this but works for now
    father->children[father->count-1] = *son ; // add to list
}

void add_g_object( group *father , object *son ) // wrapper
{
    son->parent = father;
    shape_s *new = malloc (sizeof( shape_s ));
    new->child = son;
    new->type = OBJECT;

    add_g_gchild( father , new );
}

void add_g_group ( group *father , group *son ) // wrapper
{
    son->parent = father;
    shape_s *new = malloc (sizeof( shape_s ));
    new->child = son;
    new->type = GROUP ;

    add_g_gchild( father , new );
}

void inter_ray_group ( ray* r , group* grp , inter_collec *dest )
{
    ray new_r = transform_ray( r , grp->inverse_trans );
    // iterate over all the object in the world and intersect them
    // clear dest array to avoid problems

    for ( int i = 0 ; i < grp->count ; ++i )
    {
        // unpack the child and intersect

        if ( grp->children[i].type == GROUP )
            inter_ray_group( &new_r , grp->children[i].child , dest );
        else
            intersect( &new_r , grp->children[i].child , dest ) ;

    }
}

tuple world_to_object ( void* child , enum child_type type , tuple point )
{
    // transform a point from world space to object space taking into account the "groups" in between
    float* mat; //TODO: refactor

    if ( type == OBJECT )
    {
        if (((object* ) child)->parent)
            point = world_to_object(((object *) child)->parent, GROUP, point);
        //mat = ((object *) child)->trans;
        mat = ((object *) child)->inverse_trans;
    }
    else if ( type == GROUP )
    {
        if (((group*) child)->parent)
            point = world_to_object(((group *) child)->parent, GROUP, point);
        mat = ((group *) child)->inverse_trans;
    }

    tuple res;

    multiply_mat4_tuple( mat , &point , &res ) ;

    return res;
}

tuple normal_to_world ( void *child , enum child_type type , tuple normal )
{
    // first transform the normal here then pass it to the parents
    float* mat;
    void* parent;

    if ( type == OBJECT )
    {
        mat = ((object *) child)->inverse_trans;
        parent = ((object *) child) ->parent ;
    }

    else
    {
        mat = ((group *) child)->inverse_trans;
        parent = ((group *) child) ->parent ;
    }

    mat4 transposed ;
    tuple new_normal;

    transpose_mat4( mat , transposed );
    multiply_mat4_tuple( transposed , &normal , &new_normal );
    new_normal.w = 0 ;
    normalize_tuple( &new_normal );

    if ( parent )
    {
        new_normal = normal_to_world(parent , GROUP , new_normal );
    }

    return new_normal;
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
    tuple local ;

    local = world_to_object( s , OBJECT , *world_p );
    local = local_normal( s , &local );

    return normal_to_world( s, OBJECT , local );
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
        case CYLINDER_OBJECT:
            normal = cylinder_local_normal ( s , local_p ) ;
            break ;
        default:
            fprintf( stderr , "error, shape not supported") ;
            break ;
    }

    return normal ;
}
tuple cylinder_local_normal ( object *s , tuple *local_p )
{
    float d = local_p->x*local_p->x + local_p->z*local_p->z ;
    tuple ret;

    // check if this point is on the caps
    if ( d < 1 && local_p->y >= (s->max - EPS) ) // on the upper cap
        ret = get_vector(0,1,0);
    else if ( d < 1 && local_p->y <= (s->min + EPS) ) // on the lower cap
        ret = get_vector( 0,-1,0) ;
    else
        ret = get_vector( local_p->x , 0 , local_p->z ) ;

    return ret;

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

    mult_scalar_tuple ( &color , calc->obj->mat.reflective ) ;
    return color ;
}

tuple refracted_color ( world* w , contact_calc* calc , int depth_limit )
{
    if ( depth_limit <= 0 || float_cmp(calc->obj->mat.transparency,0) )
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
    mult_scalar_tuple( &color , calc->obj->mat.transparency ) ;

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

void intersect_cube ( ray* god_ray , object* cube , inter_collec *collec )
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
//        collec->count = 0 ;
    }
    else
    {
        list_add( collec , (intersection) { .obj = cube , .t = xmin } );
        list_add ( collec , (intersection ) { .obj = cube , .t = xmax } );

//        collec->count = 2 ;
//        collec->xs = malloc ( sizeof(intersection) * 2 ) ;
//        collec->xs[0] = ( intersection ) { xmin , cube } ;
//        collec->xs[1] = ( intersection ) { xmax , cube } ;
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

int check_cyl_caps(ray *r , float t)
{
    // check if the ray is in the caps which is a circle in the XZ plane of radius 1
    float x = r->dir.x * t + r->org.x ;
    float z = r->dir.z * t + r->org.z ;

    return ( x*x + z*z ) <= 1 ;
}

void intersect_cyl_caps( ray* r , object* cylinder , inter_collec* collec )
{
    if ( !cylinder->closed || float_cmp(r->dir.y,0) )
        return;

    // check intersection with the bottom cap
    float tb = (cylinder->min - r->org.y) / r->dir.y ;
    if ( check_cyl_caps(r,tb) )
    {
        list_add( collec , (intersection ){ .obj = cylinder , .t = tb } );
    }

    // check intersection with the upper cap
    float tu = (cylinder->max - r->org.y) / r->dir.y ;
    if ( check_cyl_caps(r,tu))
    {
        list_add( collec , (intersection){ .obj = cylinder , .t = tu } );
    }
}





