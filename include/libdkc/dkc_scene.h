#ifndef DKCST_SCENE_H
#define DKCST_SCENE_H

#include "dkc_source.h"
#include "dkc_rc.h"

#define NB_WRP_SOURCES 255
#define NB_SCENES 255

typedef struct _DkcWrappedSource {

  pthread_mutex_t lock;
  struct _DkcScene* scn;
  uint8_t id;
  uint8_t source_id;
  uint16_t x, y;
  uint16_t width, height;
  float volume;

} DkcWrappedSource;

typedef struct _DkcScene {

  pthread_mutex_t lock;
  struct _DkcSceneMgr* scn_mgr;
  uint8_t id;
  DkcWrappedSource* sources[NB_WRP_SOURCES];
  uint8_t nb_sources;

} DkcScene;

typedef struct _DkcSceneMgr {

  pthread_mutex_t lock;
  DkcScene* scenes[NB_SCENES];
  uint8_t nb_scenes;
  void* bkn_ctx;
  DKCST_SCENE_CBS();
  
} DkcSceneMgr;

typedef struct _DkcSceneCBs {

  void* bkn_ctx;
  DKCST_SCENE_CBS();
  
} DkcSceneCBs;

/* Scene handling */

DkcSceneMgr* dkc_scenemgr_create(DkcSceneCBs scn_sbs);
dkc_rc dkc_scenemgr_delete(DkcSceneMgr* scn_mgr);

DkcScene* dkc_scene_create(DkcSceneMgr* scn_mgr);
dkc_rc dkc_scene_delete(DkcScene* scn);

/* Source wrapping */

DkcWrappedSource* dkc_source_wrap(DkcScene* scn, DkcSource* src);
dkc_rc dkc_source_unwrap(DkcWrappedSource* wrpd_src);

DkcScene* dkc_app_scene_create(DkcApp* app);

#endif //DKCST_SCENE_H	
