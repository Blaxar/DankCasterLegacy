#ifndef __DKCST_SCENE_H__
#define __DKCST_SCENE_H__

#include <glib-object.h>
#include "dkc_source.h"
#include "dkc_rc.h"

#define NB_WRP_SOURCES 255
#define NB_SCENES 255

typedef struct _DkcSceneCBs {

  void* bkn_ctx;
  DKC_SCENE_CBS();
  
} DkcSceneCBs;

G_BEGIN_DECLS

#define DKC_TYPE_WRAPPED_SOURCE dkc_wrapped_source_get_type ()
G_DECLARE_FINAL_TYPE (DkcWrappedSource, dkc_wrapped_source, DKC, WRAPPED_SOURCE, GObject)

#define DKC_TYPE_SCENE dkc_scene_get_type ()
G_DECLARE_FINAL_TYPE (DkcScene, dkc_scene, DKC, SCENE, GObject)

#define DKC_TYPE_SCENE_MGR dkc_scene_mgr_get_type ()
G_DECLARE_FINAL_TYPE (DkcSceneMgr, dkc_scene_mgr, DKC, SCENE_MGR, GObject)

/* Scene handling */

DkcSceneMgr* dkc_scenemgr_create(DkcSceneCBs scn_sbs, GError** err);
gboolean dkc_scenemgr_delete(DkcSceneMgr* scn_mgr, GError** err);

DkcScene* dkc_scene_create(DkcSceneMgr* scn_mgr, GError** err);
gboolean dkc_scene_delete(DkcScene* scn, GError** err);

/* Source wrapping */

DkcWrappedSource* dkc_source_wrap(DkcScene* scn, DkcSource* src, GError** err);
gboolean dkc_source_unwrap(DkcWrappedSource** wrpd_src, GError** err);

DkcScene* dkc_app_scene_create(DkcApp* app, GError** err);

G_END_DECLS

#endif //__DKCST_SCENE_H__	
