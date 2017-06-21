#ifndef __DKCST_APP_INTERNAL_H__
#define __DKCST_APP_INTERNAL_H__

#include "dkc_app.h"
#include "dkc_backend.h"
#include "dkc_source.h"
#include "dkc_scene.h"
#include "dkc_sink.h"

struct _DkcApp
{
  
  GObject parent_instance;

  gchar* bkn_type;
  DkcParams* params;
  pthread_mutex_t lock;
  DkcSourceMgr* src_mgr;
  DkcSceneMgr* scn_mgr;
  DkcSinkMgr* snk_mgr;
  DkcBackend* backend;
  
};

#endif //__DKCST_APP_INTERNAL_H__
