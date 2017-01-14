#ifndef DKCST_BACKEND_H
#define DKCST_BACKEND_H

#include "dkcst_app.h"

#define NB_BACKEND_TYPES 255
#define BACKEND_NAME_LENGTH 32

// Need to rewrite what's below

typedef struct DkCstBackend_s{

	pthread_mutex_t lock;
	DkCstApp* app;
    uint8_t type_id;
	void* ctx;
    char* name;
	DkCst_rc (*create_source)(void* ctx, uint8_t id, const char* type, DkCstParams* params);
	DkCst_rc (*delete_source)(void* ctx, uint8_t id);
	DkCst_rc (*create_scene)(void* ctx, uint8_t id);
	DkCst_rc (*delete_scene)(void* ctx, uint8_t id);
	DkCst_rc (*wrap_source)(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id);
	DkCst_rc (*unwrap_source)(void* ctx, uint8_t id);
	
} DkCstBackend;

typedef struct DkCstBackendHandler_s{

	char name[BACKEND_NAME_LENGTH];
	void* module;
	uint8_t (*DkCst_backend_create)(DkCstBackend* bkn, DkCstParams* params);
	uint8_t (*DkCst_backend_delete)(DkCstBackend* bkn);
	
} DkCstBackendHandler;

DkCst_rc DkCst_create_backend(DkCstApp* app, const char* type, DkCstParams* params, DkCstBackend** bkn);
DkCst_rc DkCst_delete_backend(DkCstBackend** bkn);

  
DkCst_rc DkCst_register_backend(const char* src_name);
DkCst_rc DkCst_register_all_backends(void);
DkCst_rc DkCst_unregister_all_backends(void);
DkCst_rc DkCst_ready_all_backends(void);


#endif //DKCST_BACKEND_H
