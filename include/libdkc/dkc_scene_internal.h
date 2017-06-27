#ifndef __DKCST_SCENE_INTERNAL_H__
#define __DKCST_SCENE_INTERNAL_H__

#include "dkc_scene.h"
#include "dkc_app_internal.h"

typedef struct _DkcWrappedSource {

  GObject parent_instance;
  
  pthread_mutex_t lock;
  struct _DkcScene* scn;
  uint8_t id;
  uint8_t source_id;
  int16_t x, y;
  uint16_t width, height;
  float volume;

};

typedef struct _DkcScene {

  GObject parent_instance;
  
  pthread_mutex_t lock;
  struct _DkcSceneMgr* scn_mgr;
  uint8_t id;
  DkcWrappedSource* sources[NB_WRP_SOURCES];
  uint8_t nb_sources;

};

typedef struct _DkcSceneMgr {

  GObject parent_instance;
  
  pthread_mutex_t lock;
  DkcScene* scenes[NB_SCENES];
  uint8_t nb_scenes;
  void* bkn_ctx;
  DKC_SCENE_CBS();
  
};

#endif //__DKCST_SCENE_INTERNAL_H__
