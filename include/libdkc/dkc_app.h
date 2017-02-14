#ifndef DKCST_APP_H
#define DKCST_APP_H

#include <inttypes.h>
#include <stddef.h>
#include <pthread.h>
#include "dkc_rc.h"
#include "dkc_param.h"

struct _DkcSourceMgr;
struct _DkcSceneMgr;
struct _DkcSinkMgr;
struct _DkcBackend;

typedef struct _DkcApp {

  pthread_mutex_t lock;
  struct _DkcSourceMgr* src_mgr;
  struct _DkcSceneMgr* scn_mgr;
  struct _DkcSinkMgr* snk_mgr;
  struct _DkcBackend* backend;

} DkcApp;

#define DKCST_SOURCE_CBS() dkc_rc (*create_source)(void* ctx, uint8_t id, const char* type, const char* name, ...); \
                         dkc_rc (*delete_source)(void* ctx, uint8_t id)

#define DKCST_SCENE_CBS() dkc_rc (*create_scene)(void* ctx, uint8_t id); \
                        dkc_rc (*delete_scene)(void* ctx, uint8_t id); \
              dkc_rc (*wrap_source)(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id); \
              dkc_rc (*unwrap_source)(void* ctx, uint8_t id)

#define DKCST_SINK_CBS() dkc_rc (*create_sink)(void* ctx, uint8_t id, const char* type, const char* name, ...); \
                         dkc_rc (*delete_sink)(void* ctx, uint8_t id)

/* App handling */

dkc_rc dkc_create_app(DkcApp** app);
dkc_rc dkc_delete_app(DkcApp** app);

dkc_rc dkc_start_app(DkcApp* app);
dkc_rc dkc_stop_app(DkcApp* app);

#endif //DKCST_APP_H	
