#ifndef DKCST_SOURCE_H
#define DKCST_SOURCE_H

#include "dkc_app.h"
#include "dkc_rc.h"
#include "dkc_types.h"

#define NB_SOURCES 255

typedef struct _DkcSource {

  pthread_mutex_t lock;
  struct _DkcSourceMgr* src_mgr;
  uint8_t id;
  DkcSourceType src_type;
  
} DkcSource;

typedef struct _DkcSourceMgr {

  pthread_mutex_t lock;
  DkcSource* sources[NB_SOURCES];
  uint8_t nb_sources;
  void* bkn_ctx;
  DKCST_SOURCE_CBS();
  
} DkcSourceMgr;

typedef struct _DkcSourceCBs {

  void* bkn_ctx;
  DKCST_SOURCE_CBS();
  
} DkcSourceCBs;

DkcSourceMgr* dkc_sourcemgr_create(DkcSourceCBs src_cbs);
dkc_rc dkc_sourcemgr_delete(DkcSourceMgr* src_mgr);

DkcSource* dkc_source_create(DkcSourceMgr* src_mgr, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params);
dkc_rc dkc_source_delete(DkcSource*  src);

DkcSource* dkc_app_source_create(DkcApp* app, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params);

#endif //DKCST_SOURCE_H	
