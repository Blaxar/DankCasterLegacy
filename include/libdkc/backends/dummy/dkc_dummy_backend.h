#ifndef DKC_DUMMY_BACKEND_H
#define DKC_DUMMY_BACKEND_H

#include <libdkc/dkc_params.h>
#include <libdkc/dkc_types.h>

gboolean dummybkn_init(void** ctx, DkcParams* params);

gboolean dummybkn_start(void* ctx);

gboolean dummybkn_stop(void* ctx);

gboolean dummybkn_create_source(void* ctx, uint8_t id, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params);

gboolean dummybkn_delete_source(void* ctx, uint8_t id);

gboolean dummybkn_create_scene(void* ctx, uint8_t id);

gboolean dummybkn_delete_scene(void* ctx, uint8_t id);

gboolean dummybkn_wrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t wrpsrc_id);

gboolean dummybkn_unwrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t wrpsrc_id);

gboolean dummybkn_create_sink(void* ctx, uint8_t id, DkcSinkType snk_type, const char* type, const char* name, DkcParams* params);

gboolean dummybkn_delete_sink(void* ctx, uint8_t id);

gboolean dummybkn_uninit(void** ctx);


#endif //DKCST_DUMMY_BACKEND_H
