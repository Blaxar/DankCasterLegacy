#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_update_wrapped_source(void* ctx, uint8_t scn_id, uint8_t id,
                                    uint16_t x, uint16_t y,
                                    uint16_t width, uint16_t height,
                                    float volume) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  return OK;
  
}
