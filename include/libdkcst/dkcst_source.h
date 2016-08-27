#ifndef DKCST_SOURCE_H
#define DKCST_SOURCE_H

#include <inttypes.h>
#include <stddef.h>

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
		uint32_t (*alloc_video_buffer)(void* ctx, void** buf); \
        uint32_t (*free_video_buffer)(void* ctx, void** buf);  \
        uint32_t (*copy_video_data)(void* ctx, void* buf);    \
        uint32_t (*alloc_audio_buffer)(void* ctx, void** buf); \
        uint32_t (*free_audio_buffer)(void* ctx, void** buf);  \
        uint32_t (*copy_audio_data)(void* ctx, void* buf);
    


typedef struct {

	SET_SOURCE_METHODS();
	
} DkCst_source_type;

typedef struct {

	uint8_t id;
	uint8_t type_id;
	void* ctx;
    SET_SOURCE_METHODS();
	
} DkCst_source;

typedef struct {

	char name[SOURCE_NAME_LENGTH];
	void* module;
	uint8_t (*DkCst_source_create)(DkCst_source* src, void* params);
	uint8_t (*DkCst_source_delete)(DkCst_source* src);
    DkCst_source_type (*DkCst_source_get_type)(void);
	
} DkCst_source_handler;

typedef struct {

	DkCst_source* sources[NB_SOURCES];
	uint8_t nb_sources;
	
} DkCst_source_mgr;

void DkCst_create_source_mgr(DkCst_source_mgr** src_mgr);
void DkCst_delete_source_mgr(DkCst_source_mgr** src_mgr);

const DkCst_source* DkCst_create_source(DkCst_source_mgr* src_mgr, const char* type, const void* params);
uint8_t DkCst_delete_source(DkCst_source_mgr* src_mgr, const DkCst_source*  src);
void DkCst_register_source_type(const char* src_name);
void DkCst_register_all_source_types(void);
void DkCst_unregister_all_source_types(void);
void DkCst_ready_all_source_types(void);
void DkCst_init(void);
void DkCst_terminate(void);

#endif //DKCST_SOURCE_H	
