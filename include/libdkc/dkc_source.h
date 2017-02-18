#ifndef DKCST_SOURCE_H
#define DKCST_SOURCE_H

#include "dkc_app.h"
#include "dkc_rc.h"

#define NB_SOURCES 255

typedef struct _DkcSource {

  pthread_mutex_t lock;
  struct _DkcSourceMgr* src_mgr;
  uint8_t id;
  char* type;
  
} DkcSource;

typedef struct _DkcSourceMgr {

  pthread_mutex_t lock;
  DkcSource* sources[NB_SOURCES];
  uint8_t nb_sources;
  DKCST_SOURCE_CBS();
  
} DkcSourceMgr;

typedef struct _DkcSourceCBs {

  DKCST_SOURCE_CBS();
  
} DkcSourceCBs;

dkc_rc dkc_create_source_mgr(DkcSourceMgr** src_mgr, DkcSourceCBs src_cbs);
dkc_rc dkc_delete_source_mgr(DkcSourceMgr** src_mgr);

dkc_rc dkc_create_source(DkcSourceMgr* src_mgr, const char* type, DkcSource** src, const char* name, DkcParams* params);
dkc_rc dkc_delete_source(DkcSource**  src);

#endif //DKCST_SOURCE_H	
