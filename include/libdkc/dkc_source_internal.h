#ifndef __DKCST_SOURCE_INTERNAL_H__
#define __DKCST_SOURCE_INTERNAL_H__

#include "dkc_app_internal.h"
#include "dkc_source.h"

typedef struct _DkcSource {

  GObject parent_instance;

  pthread_mutex_t lock;
  struct _DkcSourceMgr* src_mgr;
  uint8_t id;
  DkcSourceType src_type;
  
};

typedef struct _DkcSourceMgr {

  GObject parent_instance;

  pthread_mutex_t lock;
  DkcSource* sources[NB_SOURCES];
  uint8_t nb_sources;
  void* bkn_ctx;
  DKC_SOURCE_CBS();
  
};

#endif //__DKCST_SOURCE_INTERNAL_H__
