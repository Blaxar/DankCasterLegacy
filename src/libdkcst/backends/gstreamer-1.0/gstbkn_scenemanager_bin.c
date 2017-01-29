#include <libdkcst/backends/gstreamer-1.0/dkcst_gst_backend.h>
#include <libdkcst/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

static GstPad* request_new_pad_bin (GstElement * element, GstPadTemplate * templ, const gchar* name, const GstCaps * caps) {

	GstElement* compositor = gst_bin_get_by_name(GST_BIN(element), "scene_compositor");
	GstElement* queue;
	GstElementClass* klass = GST_ELEMENT_GET_CLASS(compositor);
	GstPad* comp_pad = klass->request_new_pad(compositor, templ, name, caps);
	if(comp_pad == NULL) return NULL;
	
	gchar* comp_pad_name = gst_pad_get_name(comp_pad);

	queue = gst_element_factory_make("queue", NULL);
	gst_bin_add(GST_BIN(element), queue);
	
	gst_element_add_pad (element, gst_ghost_pad_new (comp_pad_name, comp_pad));
	
	g_free(comp_pad_name);	
    gst_object_unref (GST_OBJECT (comp_pad));
	gst_object_unref (GST_OBJECT (compositor));

	
};

DkCst_rc gstbkn_make_scenemanager_bin(GstElement** bin) {

	GstElement* comp = NULL;
	
    *bin = gst_bin_new(NULL);
	GstElementClass* klass = GST_ELEMENT_GET_CLASS(*bin);
	request_new_pad* old_rnp = klass->request_new_pad;
	klass->request_new_pad = request_new_pad_bin;

	comp = gst_element_factory_make("compositor", "scene_compositor");
	gst_bin_add(GST_BIN(*bin), comp);
	
	return OK;
	
}
