#ifndef __DKCST_SINK_H__
#define __DKCST_SINK_H__

#include "dkc_app.h"
#include "dkc_rc.h"

#define NB_SINKS 255

typedef struct _DkcSinkCBs {

  void* bkn_ctx;
  DKC_SINK_CBS();
  
} DkcSinkCBs;

G_BEGIN_DECLS

#define DKC_TYPE_SINK dkc_sink_get_type ()
G_DECLARE_FINAL_TYPE (DkcSink, dkc_sink, DKC, SINK, GObject)

#define DKC_TYPE_SINK_MGR dkc_sink_mgr_get_type ()
G_DECLARE_FINAL_TYPE (DkcSinkMgr, dkc_sink_mgr, DKC, SINK_MGR, GObject)

DkcSinkMgr* dkc_sinkmgr_create(DkcSinkCBs snk_cbs);
dkc_rc dkc_sinkmgr_delete(DkcSinkMgr* snk_mgr);

DkcSink* dkc_sink_create(DkcSinkMgr* snk_mgr, DkcSinkType snk_type, const char* uri, const char* name, DkcParams* params);
dkc_rc dkc_sink_delete(DkcSink* snk);

DkcSink* dkc_app_sink_create(DkcApp* app, DkcSinkType snk_type, const char* uri, const char* name, DkcParams* params);

G_END_DECLS

#endif //__DKCST_SINK_H__
