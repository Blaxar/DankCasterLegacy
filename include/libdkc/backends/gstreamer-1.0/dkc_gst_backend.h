#ifndef DKC_GST_BACKEND_H
#define DKC_GST_BACKEND_H

#include <libdkc/dkc_params.h>
#include <libdkc/dkc_types.h>

gboolean gstbkn_init(void** ctx, DkcParams* params);

gboolean gstbkn_start(void* ctx);

gboolean gstbkn_stop(void* ctx);

gboolean gstbkn_create_source(void* ctx, uint8_t id, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params);

gboolean gstbkn_delete_source(void* ctx, uint8_t id);

gboolean gstbkn_create_scene(void* ctx, uint8_t id);

gboolean gstbkn_delete_scene(void* ctx, uint8_t id);

gboolean gstbkn_wrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t wrpsrc_id, DkcParams* params);

gboolean gstbkn_unwrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t wrpsrc_id);

gboolean gstbkn_update_wrapped_source(void* ctx, uint8_t scn_id, uint8_t id,
                                      uint16_t x, uint16_t y,
                                      uint16_t width, uint16_t height,
                                      float volume);

gboolean gstbkn_create_sink(void* ctx, uint8_t id, DkcSinkType snk_type, const char* type, const char* name, DkcParams* params);

gboolean gstbkn_delete_sink(void* ctx, uint8_t id);

gboolean gstbkn_uninit(void** ctx);


#endif //DKCST_GST_BACKEND_H
