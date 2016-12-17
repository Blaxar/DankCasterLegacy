#ifndef DKCST_SCENE_H
#define DKCST_SCENE_H

#include "dkcst_source.h"

#define NB_WRP_SOURCES 255
#define NB_SCENES 255

struct DkCstWrappedSource_s;
struct DkCstScene_s;
struct DkCstSceneMgr_s;

typedef struct DkCstWrappedSource_s{

	pthread_mutex_t lock;
	struct DkCstScene_s* scn;
	uint8_t id;
    uint8_t source_id;
    uint16_t x, y;
	uint16_t width, height;
	float volume;

} DkCstWrappedSource;

typedef struct DkCstScene_s{

	pthread_mutex_t lock;
	struct DkCstSceneMgr_s* scn_mgr;
	uint8_t id;
    DkCstWrappedSource* sources[NB_WRP_SOURCES];
	uint8_t nb_sources;

} DkCstScene;

typedef struct DkCstSceneMgr_s{

	pthread_mutex_t lock;
	DkCstApp* app;
    DkCstScene* scenes[NB_SCENES];
	uint8_t nb_scenes;
	
} DkCstSceneMgr;

/* Scene handling */

DkCst_rc DkCst_create_scene_mgr(DkCstApp* app, DkCstSceneMgr** scn_mgr);
DkCst_rc DkCst_delete_scene_mgr(DkCstSceneMgr** scn_mgr);

DkCst_rc DkCst_create_scene(DkCstSceneMgr* scn_mgr, DkCstScene** scn);
DkCst_rc DkCst_delete_scene(DkCstScene** scn);

/* Source wrapping */

DkCst_rc DkCst_wrap_source(DkCstScene* scn,
	                   DkCstSource* src,
					   DkCstWrappedSource** wrpd_src);
DkCst_rc DkCst_unwrap_source(DkCstWrappedSource** wrpd_src);

#endif //DKCST_SCENE_H	
