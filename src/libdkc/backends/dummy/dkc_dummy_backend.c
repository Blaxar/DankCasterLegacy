#include <libdkc/dkc_params.h>
#include <libdkc/dkc_types.h>

dkc_rc dummybkn_init(void** ctx, DkcParams* params) { return OK; }

dkc_rc dummybkn_start(void* ctx) { return OK; }

dkc_rc dummybkn_stop(void* ctx) { return OK; }

dkc_rc dummybkn_create_source(void* ctx, uint8_t id, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params) { return OK; }

dkc_rc dummybkn_delete_source(void* ctx, uint8_t id) { return OK; }

dkc_rc dummybkn_create_scene(void* ctx, uint8_t id) { return OK; }

dkc_rc dummybkn_delete_scene(void* ctx, uint8_t id) { return OK; }

dkc_rc dummybkn_wrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id) { return OK; }

dkc_rc dummybkn_unwrap_source(void* ctx, uint8_t id) { return OK; }

dkc_rc dummybkn_create_sink(void* ctx, uint8_t id, DkcSinkType snk_type, const char* type, const char* name, DkcParams* params) { return OK; }

dkc_rc dummybkn_delete_sink(void* ctx, uint8_t id) { return OK; }

dkc_rc dummybkn_uninit(void** ctx) { return OK; }
