#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_delete_source(void* ctx, uint8_t id) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  if(gst_ctx->inputs[id] != NULL) {
    gst_ctx->inputs[id];
    return OK;
  }
  
  return ERROR;
  
}
