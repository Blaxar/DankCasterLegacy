#ifndef DKC_DUMMY_BACKEND_H
#define DKC_DUMMY_BACKEND_H

#include <libdkc/dkc_params.h>
#include <libdkc/dkc_types.h>

dkc_rc dummybkn_init(void** ctx, DkcParams* params);

dkc_rc dummybkn_create_source(void* ctx, uint8_t id, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params);

dkc_rc dummybkn_delete_source(void* ctx, uint8_t id);

dkc_rc dummybkn_create_scene(void* ctx, uint8_t id);

dkc_rc dummybkn_delete_scene(void* ctx, uint8_t id);

dkc_rc dummybkn_wrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id);

dkc_rc dummybkn_unwrap_source(void* ctx, uint8_t id);

dkc_rc dummybkn_create_sink(void* ctx, uint8_t id, DkcSinkType snk_type, const char* type, const char* name, DkcParams* params);

dkc_rc dummybkn_delete_sink(void* ctx, uint8_t id);

dkc_rc dummybkn_uninit(void** ctx);


#endif //DKCST_DUMMY_BACKEND_H
