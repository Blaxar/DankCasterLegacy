#ifndef DKC_GST_BACKEND_H
#define DKC_GST_BACKEND_H

#include <libdkc/dkc_param.h>
#include <libdkc/dkc_types.h>

dkc_rc gstbkn_init(void** ctx);

dkc_rc gstbkn_create_source(void* ctx, uint8_t id, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params);

dkc_rc gstbkn_delete_source(void* ctx, uint8_t id);

dkc_rc gstbkn_create_scene(void* ctx, uint8_t id);

dkc_rc gstbkn_delete_scene(void* ctx, uint8_t id);

dkc_rc gstbkn_wrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id);

dkc_rc gstbkn_unwrap_source(void* ctx, uint8_t id);

dkc_rc gstbkn_create_sink(void* ctx, uint8_t id, const char* type, DkcParams* params);

dkc_rc gstbkn_delete_sink(void* ctx, uint8_t id);

dkc_rc gstbkn_uninit(void** ctx);


#endif //DKCST_GST_BACKEND_H
