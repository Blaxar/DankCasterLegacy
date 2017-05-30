#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_uninit(void** ctx) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) *ctx;

  gst_element_set_state (gst_ctx->pipeline, GST_STATE_NULL);
  gst_object_unref (gst_ctx->pipeline);

  gst_caps_unref(gst_ctx->v_caps);
  gst_caps_unref(gst_ctx->a_caps);
  
  free(gst_ctx);
  *ctx = NULL;
  
}
