#ifndef DKCST_BACKEND_H
#define DKCST_BACKEND_H

#include "dkcst_app.h"
#include "dkcst_rc.h"

#define BACKEND_NAME_LENGTH 32

typedef struct _DkCstBackend {

	pthread_mutex_t lock;
    uint8_t type_id;
	void* ctx;
    char* name;
	DkCst_rc (*init)(void** ctx);
	DKCST_SOURCE_CBS();
	DKCST_SCENE_CBS();
	DKCST_SINK_CBS();
	DkCst_rc (*uninit)(void** ctx);
	
} DkCstBackend;

DkCst_rc DkCst_create_backend(DkCstBackend** bkn, const char* type, DkCstParams* params);
DkCst_rc DkCst_delete_backend(DkCstBackend** bkn);


#endif //DKCST_BACKEND_H
