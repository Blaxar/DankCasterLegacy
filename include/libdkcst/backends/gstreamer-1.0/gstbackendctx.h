#ifndef GSTBACKENDCTX_H
#define GSTBACKENDCTX_H

#include <libdkcst/dkcst_param.h>
#include <gst/gst.h>

typedef struct {

	GstElement* pipeline;
	GstElement* scenemanager;
	GSList* inputs;
	GSList* outputs;
	
} GstBackendCtx;

typedef GstPad* request_new_pad (GstElement * element, GstPadTemplate * templ, const gchar* name, const GstCaps * caps);


// Bin handling

DkCst_rc gstbkn_make_scenemanager_bin(GstElement** bin);

#endif //GSTBACKENDCTX_H
