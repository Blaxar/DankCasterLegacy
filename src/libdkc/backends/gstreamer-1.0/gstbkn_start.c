#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_start(void* ctx) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;
  
  gst_element_set_state(gst_ctx->pipeline, GST_STATE_PLAYING);
  
  if (gst_element_get_state (gst_ctx->pipeline, NULL, NULL, -1) == GST_STATE_CHANGE_FAILURE)
      return ERROR;
  
  return OK;
  
}
