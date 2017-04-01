#ifndef DKCST_SINK_H
#define DKCST_SINK_H

#include "dkc_app.h"
#include "dkc_rc.h"

#define NB_SINKS 255

typedef struct _DkcSink {

  pthread_mutex_t lock;
  struct _DkcSinkMgr* snk_mgr;
  uint8_t id;
  DkcSinkType snk_type;
  
} DkcSink;

typedef struct _DkcSinkMgr {

  pthread_mutex_t lock;
  DkcSink* sinks[NB_SINKS];
  uint8_t nb_sinks;
  void* bkn_ctx;
  DKCST_SINK_CBS();
  
} DkcSinkMgr;

typedef struct _DkcSinkCBs {

  void* bkn_ctx;
  DKCST_SINK_CBS();
  
} DkcSinkCBs;

DkcSinkMgr* dkc_sinkmgr_create(DkcSinkCBs snk_cbs);
dkc_rc dkc_sinkmgr_delete(DkcSinkMgr* snk_mgr);

DkcSink* dkc_sink_create(DkcSinkMgr* snk_mgr, DkcSinkType snk_type, const char* uri, const char* name, DkcParams* params);
dkc_rc dkc_sink_delete(DkcSink* snk);

#endif //DKCST_SINK_H	
