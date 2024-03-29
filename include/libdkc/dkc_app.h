#ifndef __DKCST_APP_H__
#define __DKCST_APP_H__

#include <inttypes.h>
#include <stddef.h>
#include <pthread.h>
#include "dkc_rc.h"
#include "dkc_params.h"
#include "dkc_types.h"

#include <glib-object.h>

G_BEGIN_DECLS

#define DKC_TYPE_APP dkc_app_get_type ()
G_DECLARE_FINAL_TYPE (DkcApp, dkc_app, DKC, APP, GObject)

/* App handling */

DkcApp* dkc_app_create(const char* bkn_type, GError** err, ...);
DkcApp* dkc_app_pcreate(const char* bkn_type, DkcParams* params, GError** err);

gboolean dkc_app_delete(DkcApp* app, GError** err);

gboolean dkc_app_start(DkcApp* app, GError** err);
gboolean dkc_app_stop(DkcApp* app, GError** err);

G_END_DECLS

#define DKC_SOURCE_CBS() gboolean (*create_source)(void* ctx, uint8_t id, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params); \
                         gboolean (*delete_source)(void* ctx, uint8_t id)

#define DKC_SCENE_CBS() gboolean (*create_scene)(void* ctx, uint8_t id); \
                        gboolean (*delete_scene)(void* ctx, uint8_t id); \
                        gboolean (*wrap_source)(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id); \
                        gboolean (*unwrap_source)(void* ctx, uint8_t id)

#define DKC_SINK_CBS() gboolean (*create_sink)(void* ctx, uint8_t id, DkcSinkType snk_type, const char* uri, const char* name, DkcParams* params); \
                       gboolean (*delete_sink)(void* ctx, uint8_t id)

#define DKC_APP_CBS() gboolean (*start)(void* ctx); \
                      gboolean (*stop)(void* ctx)
                       

#endif //__DKCST_APP_H__
