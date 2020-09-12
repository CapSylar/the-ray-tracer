#ifndef RAY_TRACER_RAY_H
#define RAY_TRACER_RAY_H

#include "defs.h"

ray get_ray ( tuple* origin , tuple* direction );
void intersect_sphere ( ray *r , object s , inter_collec* dest ) ;
void intersect_plane ( ray *r , object s , inter_collec* dest  ) ;
tuple sphere_local_normal ( object *s , tuple* local_p  ) ;
tuple local_normal ( object* s , tuple* local_p ) ;
tuple ray_pos ( ray* x , float t ) ;
object get_sphere () ;
object get_plane () ;
object get_glass_sphere() ;
inter_collec intersections( int num , ... ) ;
void destroy_coll ( inter_collec* col ) ;
void intersect ( ray* r , object s , inter_collec* dest ) ;
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

#endif //RAY_TRACER_RAY_H
