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

DkcApp *dkc_app_new (void);

G_END_DECLS

#define DKCST_SOURCE_CBS() dkc_rc (*create_source)(void* ctx, uint8_t id, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params); \
                         dkc_rc (*delete_source)(void* ctx, uint8_t id)

#define DKCST_SCENE_CBS() dkc_rc (*create_scene)(void* ctx, uint8_t id); \
                        dkc_rc (*delete_scene)(void* ctx, uint8_t id); \
              dkc_rc (*wrap_source)(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id); \
              dkc_rc (*unwrap_source)(void* ctx, uint8_t id)

#define DKCST_SINK_CBS() dkc_rc (*create_sink)(void* ctx, uint8_t id, DkcSinkType snk_type, const char* uri, const char* name, DkcParams* params); \
                         dkc_rc (*delete_sink)(void* ctx, uint8_t id)

#define DKCST_APP_CBS() dkc_rc (*start)(void* ctx); \
                        dkc_rc (*stop)(void* ctx)
                        
/* App handling */

DkcApp* dkc_app_create(const char* bkn_type, DkcParams* params);
dkc_rc dkc_app_delete(DkcApp* app);

dkc_rc dkc_app_start(DkcApp* app);
dkc_rc dkc_app_stop(DkcApp* app);

#endif //__DKCST_APP_H__
