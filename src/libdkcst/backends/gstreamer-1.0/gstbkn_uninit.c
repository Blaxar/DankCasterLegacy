#include <libdkcst/backends/gstreamer-1.0/dkcst_gst_backend.h>
#include <libdkcst/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

DkCst_rc gstbkn_uninit(void** ctx) {

	GstBackendCtx* gst_ctx = (GstBackendCtx*) *ctx;
	free(gst_ctx);
	*ctx = NULL;
	
}
