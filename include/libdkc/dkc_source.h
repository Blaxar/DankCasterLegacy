#ifndef __DKCST_SOURCE_H__
#define __DKCST_SOURCE_H__

#include "dkc_app.h"
#include "dkc_rc.h"
#include "dkc_types.h"

#define NB_SOURCES 255

typedef struct _DkcSourceCBs {

  void* bkn_ctx;
  DKC_SOURCE_CBS();
  
} DkcSourceCBs;

G_BEGIN_DECLS

#define DKC_TYPE_SOURCE dkc_source_get_type ()
G_DECLARE_FINAL_TYPE (DkcSource, dkc_source, DKC, SOURCE, GObject)

#define DKC_TYPE_SOURCE_MGR dkc_source_mgr_get_type ()
G_DECLARE_FINAL_TYPE (DkcSourceMgr, dkc_source_mgr, DKC, SOURCE_MGR, GObject)

DkcSourceMgr* dkc_sourcemgr_create(DkcSourceCBs src_cbs);
gboolean dkc_sourcemgr_delete(DkcSourceMgr* src_mgr);

DkcSource* dkc_source_create(DkcSourceMgr* src_mgr, DkcSourceType src_type, const char* uri, const char* name, ...);
DkcSource* dkc_source_vcreate(DkcSourceMgr* src_mgr, DkcSourceType src_type, const char* uri, const char* name, va_list args);
DkcSource* dkc_source_pcreate(DkcSourceMgr* src_mgr, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params);

gboolean dkc_source_delete(DkcSource*  src);

DkcSource* dkc_app_source_create(DkcApp* app, DkcSourceType src_type, const char* uri, const char* name, ...);
DkcSource* dkc_app_source_pcreate(DkcApp* app, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params);

G_END_DECLS

#endif //__DKCST_SOURCE_H__
