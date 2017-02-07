#ifndef DKCST_SCENE_H
#define DKCST_SCENE_H

#include "dkcst_source.h"
#include "dkcst_rc.h"

#define NB_WRP_SOURCES 255
#define NB_SCENES 255

typedef struct _DkCstWrappedSource {

  pthread_mutex_t lock;
  struct _DkCstScene* scn;
  uint8_t id;
  uint8_t source_id;
  uint16_t x, y;
  uint16_t width, height;
  float volume;

} DkCstWrappedSource;

typedef struct _DkCstScene {

  pthread_mutex_t lock;
  struct _DkCstSceneMgr* scn_mgr;
  uint8_t id;
  DkCstWrappedSource* sources[NB_WRP_SOURCES];
  uint8_t nb_sources;

} DkCstScene;

typedef struct _DkCstSceneMgr {

  pthread_mutex_t lock;
  DkCstScene* scenes[NB_SCENES];
  uint8_t nb_scenes;
  DKCST_SCENE_CBS();
  
} DkCstSceneMgr;

typedef struct _DkCstSceneCBs {

  DKCST_SCENE_CBS();
  
} DkCstSceneCBs;

/* Scene handling */

DkCst_rc DkCst_create_scene_mgr(DkCstSceneMgr** scn_mgr, DkCstSceneCBs scn_sbs);
DkCst_rc DkCst_delete_scene_mgr(DkCstSceneMgr** scn_mgr);

DkCst_rc DkCst_create_scene(DkCstSceneMgr* scn_mgr, DkCstScene** scn);
DkCst_rc DkCst_delete_scene(DkCstScene** scn);

/* Source wrapping */

DkCst_rc DkCst_wrap_source(DkCstScene* scn,
                     DkCstSource* src,
             DkCstWrappedSource** wrpd_src);
DkCst_rc DkCst_unwrap_source(DkCstWrappedSource** wrpd_src);

#endif //DKCST_SCENE_H	
