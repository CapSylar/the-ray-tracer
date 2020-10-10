#ifndef RAY_TRACER_RAY_H
#define RAY_TRACER_RAY_H

#include "defs.h"
#include "list.h"

ray get_ray ( tuple* origin , tuple* direction );
void intersect_sphere ( ray *r , object* s , inter_collec* dest ) ;
void intersect_plane ( ray *r , object* s , inter_collec* dest  ) ;
tuple sphere_local_normal ( object *s , tuple* local_p  ) ;
tuple local_normal ( object* s , tuple* local_p ) ;
tuple ray_pos ( ray* x , float t ) ;
object* get_sphere () ;
object* get_plane () ;
object* get_cube () ;
object* get_cylinder() ;
object* get_cone();
group* get_group();
object* get_glass_sphere() ;
void intersect ( ray* r , object* s , inter_collec* dest ) ;
intersection* hit( inter_collec* col ) ;
ray transform_ray ( ray* r , mat4 mat ) ;
void set_transform ( object *o , mat4 trans ) ;
tuple normal_at (object *s , tuple *world_p ) ;
tuple reflect ( tuple* in , tuple* normal ) ;
point_light get_plight ( tuple origin , tuple color ) ;
void def_material ( material *def ) ;
tuple lighting ( material* mat , object* o , point_light* light , tuple* point , tuple* eye_dir , tuple* normal , int in_shadow ) ;
int comp_intersections (const void* elem1 , const void* elem2 ) ;
tuple reflected_color ( world* w , contact_calc* calc , int depth_limit ) ;
tuple refracted_color ( world* w , contact_calc* calc , int depth_limit ) ;
float schlick_approx ( contact_calc* calc ) ;
void intersect_axis( float axis_origin , float axis_direction , float *tmin , float *tmax ) ;
void intersect_cube ( ray* god_ray , object* cube , inter_collec *collec ) ;
tuple cube_local_normal ( object *s , tuple* local_p  ) ;
void intersect_cylinder ( ray* r , object* s , inter_collec* dest ) ;
int check_cyl_caps(ray *r , float t) ;
tuple cylinder_local_normal ( object *s , tuple *local_p ) ;
void intersect_cyl_caps( ray* r , object* cylinder , inter_collec* collec ) ;

void add_g_gchild (group *father , shape_s* son );
void add_g_object( group *father , object *son ); // wrapper;
void add_g_group ( group *father , group *son ); // wrapper
void inter_ray_group ( ray* r , group* grp , inter_collec *dest );
tuple world_to_object ( void* child , enum child_type type , tuple point );
tuple normal_to_world ( void *child , enum child_type type , tuple normal );

#endif //RAY_TRACER_RAY_H
