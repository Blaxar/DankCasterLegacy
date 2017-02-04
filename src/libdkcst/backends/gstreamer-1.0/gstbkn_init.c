#include <libdkcst/backends/gstreamer-1.0/dkcst_gst_backend.h>
#include <libdkcst/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

DkCst_rc gstbkn_init(void** ctx) {

    *ctx = (void*) malloc(sizeof(GstBackendCtx));
	GstBackendCtx* bkn_ctx = *ctx;

	if(gst_init_check (NULL, NULL, NULL) == FALSE) return ERROR;

	bkn_ctx->pipeline = gst_pipeline_new(NULL);
	
	gstbkn_make_scenemanager_bin(&bkn_ctx->scenemanager);
	GstElement* phony_src = gst_element_factory_make("videotestsrc", NULL);
	
	gst_bin_add_many(GST_BIN(bkn_ctx->pipeline), bkn_ctx->scenemanager, phony_src, NULL);
	
	if(gst_element_link(phony_src, bkn_ctx->scenemanager) == FALSE)
		g_printerr("Failed to link %s and %s :(\n", GST_ELEMENT_NAME(phony_src), GST_ELEMENT_NAME(bkn_ctx->scenemanager));

	gst_object_unref(bkn_ctx->pipeline);

	return OK;
	
}
