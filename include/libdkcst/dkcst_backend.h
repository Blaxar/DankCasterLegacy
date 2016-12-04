#ifndef DKCST_BACKEND_H
#define DKCST_BACKEND_H

#include "dkcst_rc.h"
#include "dkcst_param.h"
#include <inttypes.h>
#include <stddef.h>
#include <pthread.h>

#define NB_BACKENDS 255
#define BACKEND_NAME_LENGTH 32

// Need to rewrite what's below
#define SET_BACKEND_METHODS()								   \
												               \
	    DkCstSrcType type;								   \
		char* name;											   \
		uint16_t (*get_width)(void* ctx);					   \
		uint16_t (*get_height)(void* ctx);					   \
	    DkCstPixFmt (*get_pix_fmt)(void* ctx);			   \
		float (*get_fps)(void* ctx);						   \
		uint16_t (*get_sample_rate)(void* ctx);                \
		uint8_t (*get_nb_channels)(void* ctx);                 \
    
struct DkCstBackend_s;
struct DkCstBackendHandler_s;

typedef struct DkCstBackend_s{

	pthread_mutex_t lock;
	void* ctx;
    SET_BACKEND_METHODS();
	
} DkCstSource;

typedef struct DkCstBackendHandler_s{

	char name[BACKEND_NAME_LENGTH];
	void* module;
	uint8_t (*DkCst_backend_create)(DkCstBackend* src, DkCstParams* params);
	uint8_t (*DkCst_backend_delete)(DkCstBackend* src);
	
} DkCstBackendHandler;

DkCst_rc DkCst_create_backend(const char* type, DkCstParams* params, DkCstBackend** bkn);
DkCst_rc DkCst_delete_backend(DkCstBackend** bkn);

DkCst_rc DkCst_register_backend(const char* src_name);
DkCst_rc DkCst_register_all_backends(void);
DkCst_rc DkCst_unregister_all_backends(void);
DkCst_rc DkCst_ready_all_backends(void);

#endif //DKCST_BACKEND_H	
