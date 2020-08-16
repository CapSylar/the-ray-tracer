#ifndef RAY_TRACER_WORLD_VIEW_H
#define RAY_TRACER_WORLD_VIEW_H

#include "defs.h"

void destroy_world ( world *dead ) ;
void init_world (world *new ) ;
void inter_ray_world ( ray* r , world* w , inter_collec *dest ) ;
void merge_destroy ( inter_collec *dest , inter_collec *inter ) ;
void compute_contact ( ray* r , intersection* i , contact_calc* dest ) ;
tuple shade_hit( world* w , contact_calc* calc ) ;
tuple color_at ( world *w , ray *r ) ;
void view_transform ( tuple* to , tuple* from , tuple* up , mat4 res ) ;
void init_camera (int hsize , int vsize , float fov , camera *c ) ;
void ray_for_pixel ( camera* c , int px , int py , ray *r ) ;
void render( camera *c , world *w ) ;
void add_obj_world ( world* w , object* new_obj ) ;

#endif //RAY_TRACER_WORLD_VIEW_H
