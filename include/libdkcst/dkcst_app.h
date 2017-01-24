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

/* App handling */

DkCst_rc DkCst_create_app(DkCstApp** app);
DkCst_rc DkCst_delete_app(DkCstApp** app);

DkCst_rc DkCst_start_app(DkCstApp* app);
DkCst_rc DkCst_stop_app(DkCstApp* app);

#endif //DKCST_APP_H	
