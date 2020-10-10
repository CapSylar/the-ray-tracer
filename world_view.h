#ifndef RAY_TRACER_WORLD_VIEW_H
#define RAY_TRACER_WORLD_VIEW_H

#include "defs.h"
#include "list.h"

void destroy_world ( world *dead ) ;
void init_world (world *new ) ;
void inter_ray_world ( ray* r , world* w , inter_collec *dest ) ;
void merge_destroy ( inter_collec *dest , inter_collec *inter ) ;
void compute_contact ( ray* r , intersection* i , contact_calc* dest, inter_collec* collec ); // computes contact information used for shading and such
tuple shade_hit( world* w , contact_calc* calc , int calc_shadows , int depth_limit ) ;
tuple color_at ( world *w , ray *r , int calc_shadows , int depth_limit );
void view_transform ( tuple* to , tuple* from , tuple* up , mat4 res ) ;
void init_camera (int hsize , int vsize , float fov , camera *c ) ;
void ray_for_pixel ( camera* c , int px , int py , ray *r ) ;
void render( camera *c , world *w ) ;
void add_obj_world ( world* w , object* new_obj ) ;
int is_shadowed ( world* w , tuple *point) ;
void init_empty_world(world *new);
void add_grp_world ( world *w , group* new_grp );
void add_to_world( world* w , void* new_grp , enum child_type type );
void precompute_inv_world( world* w );
void precompute_inv_shape( shape_s* child );

#endif //RAY_TRACER_WORLD_VIEW_H
