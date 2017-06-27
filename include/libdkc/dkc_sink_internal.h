#ifndef __DKCST_SINK_INTERNAL_H__
#define __DKCST_SINK_INTERNAL_H__

#include "dkc_app_internal.h"
#include "dkc_sink.h"

typedef struct _DkcSink {

  GObject parent_instance;
  
  pthread_mutex_t lock;
  struct _DkcSinkMgr* snk_mgr;
  uint8_t id;
  DkcSinkType snk_type;
  
};

typedef struct _DkcSinkMgr {

  GObject parent_instance;

  pthread_mutex_t lock;
  DkcSink* sinks[NB_SINKS];
  uint8_t nb_sinks;
  void* bkn_ctx;
  DKC_SINK_CBS();
  
};

#endif //__DKCST_SINK_INTERNAL_H__
