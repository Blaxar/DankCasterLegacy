#ifndef DKCST_SCENE_H
#define DKCST_SCENE_H

#include "dkcst_source.h"

#define NB_WRP_SOURCES 255
#define NB_SCENES 255

typedef struct {

    uint8_t source_id;
    uint16_t x, y;
	uint16_t width, height;
	float volume;

} DkCst_wrapped_source;

typedef struct {

	uint8_t scene_id;
    DkCst_wrapped_source sources[NB_WRP_SOURCES];
	uint8_t nb_sources;

} DkCst_scene;

typedef struct {

    DkCst_scene scenes[NB_SCENES];
	uint8_t nb_scenes;
	
} DkCst_scene_mgr;

/* Scene handling */

void DkCst_create_scene_mgr(DkCst_scene_mgr** scn_mgr);
void DkCst_delete_scene_mgr(DkCst_scene_mgr** scn_mgr);

void DkCst_create_scene(DkCst_scene_mgr* scn_mgr, DkCst_scene** scn);
void DkCst_delete_scene(DkCst_scene_mgr* scn_mgr, DkCst_scene** scn);

/* Source wrapping */

void DkCst_wrap_source(DkCst_scene* scn,
	                   DkCst_source* src,
					   DkCst_wrapped_source** wrpd_src);
void DkCst_unwrap_source(DkCst_scene* scn,
					     DkCst_wrapped_source** wrpd_src);

#endif //DKCST_SCENE_H	
