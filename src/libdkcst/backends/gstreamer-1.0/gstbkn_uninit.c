#include <libdkcst/backends/gstreamer-1.0/dkcst_gst_backend.h>
#include <libdkcst/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

DkCst_rc gstbkn_uninit(void** ctx) {

	GstBackendCtx* gst_ctx = (GstBackendCtx*) *ctx;

	gst_element_set_state (gst_ctx->pipeline, GST_STATE_NULL);
    gst_object_unref (gst_ctx->pipeline);
	
	free(gst_ctx);
	*ctx = NULL;
	
}
