#ifndef GSTBACKENDCTX_H
#define GSTBACKENDCTX_H

#include <libdkcst/dkcst_param.h>
#include <gst/gst.h>

typedef struct {

	GstBin* pipeline;
	GSList* inputs;
	GSList* outputs;
	
} GstBackendCtx;


#endif //GSTBACKENDCTX_H
