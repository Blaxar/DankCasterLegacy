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
	DkCst_rc (*create_source)(void* ctx, uint8_t id, const char* type, DkCstParams* params);
	DkCst_rc (*delete_source)(void* ctx, uint8_t id);
	DkCst_rc (*create_scene)(void* ctx, uint8_t id);
	DkCst_rc (*delete_scene)(void* ctx, uint8_t id);
	DkCst_rc (*wrap_source)(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id);
	DkCst_rc (*unwrap_source)(void* ctx, uint8_t id);
	DkCst_rc (*create_sink)(void* ctx, uint8_t id, const char* type, DkCstParams* params);
	DkCst_rc (*delete_sink)(void* ctx, uint8_t id);
	DkCst_rc (*uninit)(void** ctx);
	
} DkCstBackend;

DkCst_rc DkCst_create_backend(DkCstBackend** bkn, const char* type, DkCstParams* params);
DkCst_rc DkCst_delete_backend(DkCstBackend** bkn);


#endif //DKCST_BACKEND_H
