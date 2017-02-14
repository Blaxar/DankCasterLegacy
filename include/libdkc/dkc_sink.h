#ifndef DKCST_SINK_H
#define DKCST_SINK_H

#include "dkc_app.h"
#include "dkc_rc.h"

#define NB_SINKS 255

typedef struct _DkcSink {

  pthread_mutex_t lock;
  struct _DkcSinkMgr* snk_mgr;
  uint8_t id;
  char* type;
  
} DkcSink;

typedef struct _DkcSinkMgr {

  pthread_mutex_t lock;
  DkcSink* sinks[NB_SINKS];
  uint8_t nb_sinks;
  DKCST_SINK_CBS();
  
} DkcSinkMgr;

typedef struct _DkcSinkCBs {

  DKCST_SINK_CBS();
  
} DkcSinkCBs;

dkc_rc dkc_create_sink_mgr(DkcSinkMgr** snk_mgr, DkcSinkCBs snk_cbs);
dkc_rc dkc_delete_sink_mgr(DkcSinkMgr** snk_mgr);

dkc_rc dkc_create_sink(DkcSinkMgr* snk_mgr, const char* type, DkcSink** snk, const char* name, ...);
dkc_rc dkc_delete_sink(DkcSink**  snk);

#endif //DKCST_SINK_H	
