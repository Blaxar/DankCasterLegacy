#ifndef DKCST_APP_H
#define DKCST_APP_H

#include "dkcst_scene.h"
#include "dkcst_sink.h"

struct DkCstApp_s;

typedef struct DkCstApp_s{

	pthread_mutex_t lock;
	struct DkCstSourceMgr_s* src_mgr;
	struct DkCstSceneMgr_s* scn_mgr;
	struct DkCstSinkMgr_s* snk_mgr;
	struct DkcstBackend_s* backend;

} DkCstApp;

/* App handling */

DkCst_rc DkCst_create_app(DkCstApp** app, DkCstSourceMgr* src_mgr, DkCstSceneMgr* scn_mgr, DkCstSinkMgr* snk_mgr, const char* backend);
DkCst_rc DkCst_delete_app(DkCstApp** app);

DkCst_rc DkCst_start_app(DkCstApp* app);
DkCst_rc DkCst_stop_app(DkCstApp* app);

DkCst_rc DkCst_init(void);
DkCst_rc DkCst_terminate(void);

#endif //DKCST_APP_H	
