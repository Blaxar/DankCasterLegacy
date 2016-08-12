#ifndef DKCST_SOURCE_H
#define DKCST_SOURCE_H

#include <inttypes.h>
#include <stddef.h>

#define NB_SOURCE_TYPES 255 

typedef uint8_t pix_fmt_t;
typedef uint8_t src_tk;
typedef char* src_type;

#define SET_SOURCE_METHODS()					\
    											\
		src_type type;                          \
		char* name;								\
		uint16_t (*get_width)(void* ctx);		\
		uint16_t (*get_height)(void* ctx);		\
		pix_fmt_t (*get_pix_fmt)(void* ctx);	\
		float (*get_fps)(void* ctx);			\
		uint16_t (*get_sample_rate)(void* ctx);	\
		uint8_t (*get_nb_channels)(void* ctx);
    


struct dkcst_source_type_t {

	SET_SOURCE_METHODS();
	
};

struct dkcst_source_t {

	uint8_t id;
	void* ctx;
    SET_SOURCE_METHODS();
	
};

struct dkcst_source_handler_t {

	char name[32];
	void* module;
	uint8_t (*dkcst_source_create)(struct dkcst_source_t* src, void* params);
	uint8_t (*dkcst_source_delete)(struct dkcst_source_t* src);
    struct dkcst_source_type_t (*dkcst_source_get_type)(void);
	
};

struct dkcst_source_mgr_t {

	struct dkcst_source_t* sources[255];
	uint8_t nb_sources;
	
};

typedef struct dkcst_source_type_t dkcst_source_type;
typedef struct dkcst_source_t dkcst_source;
typedef struct dkcst_source_handler_t dkcst_source_handler;
typedef struct dkcst_source_mgr_t dkcst_source_mgr;

void dkcst_create_source_mgr(dkcst_source_mgr** src_mgr);
void dkcst_delete_source_mgr(dkcst_source_mgr** src_mgr);

const dkcst_source* dkcst_create_source(dkcst_source_mgr* src_mgr, const char* type, const void* params);
uint8_t dkcst_delete_source(dkcst_source_mgr* src_mgr, const dkcst_source*  src);
void dkcst_register_source_type(const char* src_name);
void dkcst_register_all_source_types(void);
void dkcst_unregister_all_source_types(void);
void dkcst_ready_all_source_types(void);
void dkcst_init(void);
void dkcst_terminate(void);

#endif //DKCST_SOURCE_H	
