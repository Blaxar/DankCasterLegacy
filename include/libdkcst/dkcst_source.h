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

typedef enum {BOTH, VIDEO, AUDIO} DkCstSrcType;
typedef enum {RGB24,
			  RGB32,
			  BGR24,
			  BGR32,
			  YUV420,
			  YUV422} DkCstPixFmt;
typedef uint8_t src_tk;
typedef uint64_t DkCstTimestamp;

#define SET_SOURCE_METHODS()								   \
												               \
	    DkCstSrcType type;								   \
		char* name;											   \
		uint16_t (*get_width)(void* ctx);					   \
		uint16_t (*get_height)(void* ctx);					   \
	    DkCstPixFmt (*get_pix_fmt)(void* ctx);			   \
		float (*get_fps)(void* ctx);						   \
		uint16_t (*get_sample_rate)(void* ctx);                \
		uint8_t (*get_nb_channels)(void* ctx);                 \
    
struct DkCstSourceType_s;
struct DkCstSource_s;
struct DkCstSourceHandler_s;
struct DkCstSourceMgr_s;

typedef struct DkCstSourceType_s{

	SET_SOURCE_METHODS();
	
} DkCstSourceType;

typedef struct DkCstSource_s{

	pthread_mutex_t lock;
	struct DkCstSourceMgr_s* src_mgr;
	uint8_t id;
	uint8_t type_id;
	void* ctx;
    SET_SOURCE_METHODS();
	
} DkCstSource;

typedef struct DkCstSourceHandler_s{

	char name[SOURCE_NAME_LENGTH];
	void* module;
	uint8_t (*DkCst_source_create)(DkCstSource* src, DkCstParams* params);
	uint8_t (*DkCst_source_delete)(DkCstSource* src);
    DkCstSourceType (*DkCst_source_get_type)(void);
	
} DkCstSourceHandler;

typedef struct DkCstSourceMgr_s{

	pthread_mutex_t lock;
	DkCstSource* sources[NB_SOURCES];
	uint8_t nb_sources;
	
} DkCstSourceMgr;

DkCst_rc DkCst_create_source_mgr(DkCstSourceMgr** src_mgr);
DkCst_rc DkCst_delete_source_mgr(DkCstSourceMgr** src_mgr);

DkCst_rc DkCst_create_source(DkCstSourceMgr* src_mgr, const char* type, DkCstParams* params, DkCstSource** src);
DkCst_rc DkCst_delete_source(DkCstSource**  src);

#endif //DKCST_SOURCE_H	
