#ifndef DKCST_APP_H
#define DKCST_APP_H

#include <inttypes.h>
#include <stddef.h>
#include <pthread.h>
#include "dkcst_rc.h"
#include "dkcst_param.h"

struct DkCstSourceMgr_s;
struct DkCstSceneMgr_s;
struct DkCstSinkMgr_s;
struct DkcstBackend_s;

typedef struct {

	pthread_mutex_t lock;
	struct DkCstSourceMgr_s* src_mgr;
	struct DkCstSceneMgr_s* scn_mgr;
	struct DkCstSinkMgr_s* snk_mgr;
	struct DkcstBackend_s* backend;

} DkCstApp;

/* App handling */

DkCst_rc DkCst_create_app(DkCstApp** app);
DkCst_rc DkCst_delete_app(DkCstApp** app);

DkCst_rc DkCst_start_app(DkCstApp* app);
DkCst_rc DkCst_stop_app(DkCstApp* app);

#endif //DKCST_APP_H	
