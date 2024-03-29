#ifndef DKC_GST_BACKEND_H
#define DKC_GST_BACKEND_H

#include <libdkc/dkc_params.h>
#include <libdkc/dkc_types.h>

dkc_rc gstbkn_init(void** ctx, DkcParams* params);

dkc_rc gstbkn_start(void* ctx);

dkc_rc gstbkn_stop(void* ctx);

dkc_rc gstbkn_create_source(void* ctx, uint8_t id, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params);

dkc_rc gstbkn_delete_source(void* ctx, uint8_t id);

dkc_rc gstbkn_create_scene(void* ctx, uint8_t id);

dkc_rc gstbkn_delete_scene(void* ctx, uint8_t id);

dkc_rc gstbkn_wrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id);

dkc_rc gstbkn_unwrap_source(void* ctx, uint8_t id);

dkc_rc gstbkn_update_wrapped_source(void* ctx, uint8_t scn_id, uint8_t id,
                                    uint16_t x, uint16_t y,
                                    uint16_t width, uint16_t height,
                                    float volume);

dkc_rc gstbkn_create_sink(void* ctx, uint8_t id, DkcSinkType snk_type, const char* type, const char* name, DkcParams* params);

dkc_rc gstbkn_delete_sink(void* ctx, uint8_t id);

dkc_rc gstbkn_uninit(void** ctx);


#endif //DKCST_GST_BACKEND_H
