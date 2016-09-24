#ifndef DKCST_SOURCE_H
#define DKCST_SOURCE_H

#include "dkcst_rc.h"
#include "dkcst_param.h"
#include <inttypes.h>
#include <stddef.h>
#include <pthread.h>

#define NB_SOURCE_TYPES 255
#define NB_SOURCES 255
#define SOURCE_NAME_LENGTH 32

typedef enum {BOTH, VIDEO, AUDIO} DkCst_src_type;
typedef enum {RGB24,
			  RGB32,
			  BGR24,
			  BGR32,
			  YUV420,
			  YUV422} DkCst_pix_fmt;
typedef uint8_t src_tk;

#define SET_SOURCE_METHODS()								   \
												               \
	    DkCst_src_type type;								   \
		char* name;											   \
		uint16_t (*get_width)(void* ctx);					   \
		uint16_t (*get_height)(void* ctx);					   \
	    DkCst_pix_fmt (*get_pix_fmt)(void* ctx);			   \
		float (*get_fps)(void* ctx);						   \
		uint16_t (*get_sample_rate)(void* ctx);                \
		uint8_t (*get_nb_channels)(void* ctx);                 \
	    DkCst_rc (*alloc_video_buffer)(void* ctx, void** buf, uint32_t* size); \
        DkCst_rc (*free_video_buffer)(void* ctx, void** buf); \
        DkCst_rc (*copy_video_data)(void* ctx, void* buf);    \
        DkCst_rc (*alloc_audio_buffer)(void* ctx, void** buf, uint32_t* size); \
        DkCst_rc (*free_audio_buffer)(void* ctx, void** buf);  \
        DkCst_rc (*copy_audio_data)(void* ctx, void* buf);
    
struct DkCst_source_type_s;
struct DkCst_source_s;
struct DkCst_source_handler_s;
struct DkCst_source_mgr_s;

typedef struct DkCst_source_type_s{

	SET_SOURCE_METHODS();
	
} DkCst_source_type;

typedef struct DkCst_source_s{

	pthread_mutex_t lock;
	struct DkCst_source_mgr_s* src_mgr;
	uint8_t id;
	uint8_t type_id;
	void* ctx;
    SET_SOURCE_METHODS();
	
} DkCst_source;

typedef struct DkCst_source_handler_s{

	char name[SOURCE_NAME_LENGTH];
	void* module;
	uint8_t (*DkCst_source_create)(DkCst_source* src, DkCst_params* params);
	uint8_t (*DkCst_source_delete)(DkCst_source* src);
    DkCst_source_type (*DkCst_source_get_type)(void);
	
} DkCst_source_handler;

typedef struct DkCst_source_mgr_s{

	pthread_mutex_t lock;
	DkCst_source* sources[NB_SOURCES];
	uint8_t nb_sources;
	
} DkCst_source_mgr;

DkCst_rc DkCst_create_source_mgr(DkCst_source_mgr** src_mgr);
DkCst_rc DkCst_delete_source_mgr(DkCst_source_mgr** src_mgr);

DkCst_rc DkCst_create_source(DkCst_source_mgr* src_mgr, const char* type, DkCst_params* params, DkCst_source** src);
DkCst_rc DkCst_delete_source(DkCst_source**  src);
DkCst_rc DkCst_register_source_type(const char* src_name);
DkCst_rc DkCst_register_all_source_types(void);
DkCst_rc DkCst_unregister_all_source_types(void);
DkCst_rc DkCst_ready_all_source_types(void);
DkCst_rc DkCst_init(void);
DkCst_rc DkCst_terminate(void);

#endif //DKCST_SOURCE_H	
