#include <libdkcst/backends/gstreamer-1.0/dkcst_gst_backend.h>
#include <libdkcst/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

DkCst_rc gstbkn_init(void** ctx) {

    *ctx = (void*) malloc(sizeof(GstBackendCtx));

	if(gst_init_check (NULL, NULL, NULL) == FALSE) return ERROR;

	return OK;
	
}
