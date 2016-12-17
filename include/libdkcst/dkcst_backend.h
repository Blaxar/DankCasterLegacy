#ifndef DKCST_BACKEND_H
#define DKCST_BACKEND_H

#include "dkcst_app.h"

#define NB_BACKEND_TYPES 255
#define BACKEND_NAME_LENGTH 32

// Need to rewrite what's below
#define SET_BACKEND_METHODS()								   \
												               \
		char* name;										   
    
struct DkCstBackend_s;
struct DkCstBackendHandler_s;

typedef struct DkCstBackend_s{

	pthread_mutex_t lock;
	DkCstApp* app;
	uint8_t type_id;
	void* ctx;
    SET_BACKEND_METHODS();
	
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
