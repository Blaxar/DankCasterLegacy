#ifndef DKCST_GST_BACKEND_H
#define DKCST_GST_BACKEND_H

#include <libdkcst/dkcst_param.h>

DkCst_rc gstbkn_init(void** ctx);

DkCst_rc gstbkn_create_source(void* ctx, uint8_t id, const char* type, DkCstParams* params);

DkCst_rc gstbkn_delete_source(void* ctx, uint8_t id);

DkCst_rc gstbkn_create_scene(void* ctx, uint8_t id);

DkCst_rc gstbkn_delete_scene(void* ctx, uint8_t id);

DkCst_rc gstbkn_wrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id);

DkCst_rc gstbkn_unwrap_source(void* ctx, uint8_t id);

DkCst_rc gstbkn_create_sink(void* ctx, uint8_t id, const char* type, DkCstParams* params);

DkCst_rc gstbkn_delete_sink(void* ctx, uint8_t id);

DkCst_rc gstbkn_uninit(void** ctx);


#endif //DKCST_GST_BACKEND_H
