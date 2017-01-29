#include <libdkcst/backends/gstreamer-1.0/dkcst_gst_backend.h>
#include <libdkcst/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

DkCst_rc gstbkn_init(void** ctx) {

    *ctx = (void*) malloc(sizeof(GstBackendCtx));
	GstBackendCtx* bkn_ctx = *ctx;

	if(gst_init_check (NULL, NULL, NULL) == FALSE) return ERROR;

	bkn_ctx->pipeline = gst_pipeline_new(NULL);
	gstbkn_make_scenemanager_bin(&bkn_ctx->scenemanager);
	gst_bin_add(GST_BIN(bkn_ctx->pipeline), bkn_ctx->scenemanager);

	return OK;
	
}
