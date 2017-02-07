#ifndef DKCST_APP_H
#define DKCST_APP_H

#include <inttypes.h>
#include <stddef.h>
#include <pthread.h>
#include "dkcst_rc.h"
#include "dkcst_param.h"

struct _DkCstSourceMgr;
struct _DkCstSceneMgr;
struct _DkCstSinkMgr;
struct _DkCstBackend;

typedef struct _DkCstApp {

	pthread_mutex_t lock;
    struct _DkCstSourceMgr* src_mgr;
	struct _DkCstSceneMgr* scn_mgr;
	struct _DkCstSinkMgr* snk_mgr;
    struct _DkCstBackend* backend;

} DkCstApp;

#define DKCST_SOURCE_CBS() DkCst_rc (*create_source)(void* ctx, uint8_t id, const char* type, const char* name, DkCstParams* params); \
	                       DkCst_rc (*delete_source)(void* ctx, uint8_t id)

#define DKCST_SCENE_CBS() DkCst_rc (*create_scene)(void* ctx, uint8_t id); \
	                      DkCst_rc (*delete_scene)(void* ctx, uint8_t id); \
						  DkCst_rc (*wrap_source)(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id); \
						  DkCst_rc (*unwrap_source)(void* ctx, uint8_t id)

#define DKCST_SINK_CBS() DkCst_rc (*create_sink)(void* ctx, uint8_t id, const char* type, DkCstParams* params); \
                         DkCst_rc (*delete_sink)(void* ctx, uint8_t id)

/* App handling */

DkCst_rc DkCst_create_app(DkCstApp** app);
DkCst_rc DkCst_delete_app(DkCstApp** app);

DkCst_rc DkCst_start_app(DkCstApp* app);
DkCst_rc DkCst_stop_app(DkCstApp* app);

#endif //DKCST_APP_H	
