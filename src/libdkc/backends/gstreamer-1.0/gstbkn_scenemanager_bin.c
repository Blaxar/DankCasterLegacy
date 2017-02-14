#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

static GstPad* request_new_pad_bin (GstElement * element, GstPadTemplate * templ, const gchar* name, const GstCaps * caps) {

  gchar* element_name = GST_ELEMENT_NAME(GST_BIN(element));
  gchar* comp_name = g_strconcat(element_name, "_comp", NULL);

  GstElement* compositor = gst_bin_get_by_name(GST_BIN(element), comp_name);
  GstElement* queue;
  GstElementClass* klass = GST_ELEMENT_GET_CLASS(compositor);
  GstPad* comp_pad = klass->request_new_pad(compositor, templ, name, caps);
  if(comp_pad == NULL) return NULL;

  gchar* comp_pad_name = gst_pad_get_name(comp_pad);
  gchar* queue_name = g_strconcat(comp_name, "_", comp_pad_name, NULL);
  
  queue = gst_element_factory_make("queue", queue_name);
  gst_bin_add(GST_BIN(element), queue);
  gst_element_link_pads(queue, NULL, compositor, comp_pad_name);
  GstPad* queue_pad = gst_element_get_static_pad(queue, "sink");
  GstPad* ghost_pad = gst_ghost_pad_new (comp_pad_name, queue_pad);
  gst_element_add_pad (element, ghost_pad);

  g_free(comp_name);
  g_free(comp_pad_name);
  g_free(queue_name);
  gst_object_unref (GST_OBJECT (queue_pad));
  //gst_object_unref (GST_OBJECT (compositor));

  return ghost_pad;
  
};

static void release_pad_bin (GstElement *element, GstPad *pad) {

  gchar* element_name = GST_ELEMENT_NAME(element);
  gchar* comp_name = g_strconcat(element_name, "_comp", NULL);
  gchar* comp_pad_name = gst_pad_get_name(pad);
  gchar* queue_name = g_strconcat(comp_name, "_", comp_pad_name, NULL);
  
  gst_element_remove_pad (element, pad);
  GstElement* queue = gst_bin_get_by_name(GST_BIN(element), queue_name);
  gst_bin_remove(GST_BIN(element), queue);

  g_free(comp_name);
  g_free(comp_pad_name);
  g_free(queue_name);

}

dkc_rc gstbkn_make_scenemanager_bin(GstElement** bin) {
  
    *bin = gst_bin_new(NULL);
  GstElementClass* klass = GST_ELEMENT_GET_CLASS(*bin);
  klass->request_new_pad = request_new_pad_bin;
  klass->release_pad = release_pad_bin;

  gchar* element_name = GST_ELEMENT_NAME(*bin);
  gchar* comp_name = g_strconcat(element_name, "_comp", NULL);

  GstElement* comp = gst_element_factory_make("compositor", comp_name);
  GstElementClass* comp_klass = GST_ELEMENT_GET_CLASS(comp);
  klass->padtemplates = comp_klass->padtemplates;
  klass->numpadtemplates = comp_klass->numpadtemplates;
    klass->pad_templ_cookie = comp_klass->pad_templ_cookie;
  
  gst_bin_add(GST_BIN(*bin), comp);

  g_free(comp_name);
  
  return OK;
  
}
