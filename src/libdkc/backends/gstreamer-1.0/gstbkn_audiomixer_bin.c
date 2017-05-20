#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

static GstPad* request_new_pad_bin (GstElement * element, GstPadTemplate * templ, const gchar* name, const GstCaps * caps) {

  gchar* element_name = GST_ELEMENT_NAME(GST_BIN(element));
  gchar* amix_name = g_strconcat(element_name, "_amix", NULL);

  GstElement* amixer = gst_bin_get_by_name(GST_BIN(element), amix_name);
  GstElement* queue;
  GstElementClass* klass = GST_ELEMENT_GET_CLASS(amixer);
  GstPad* amix_pad = klass->request_new_pad(amixer, templ, name, caps);
  g_print("AMIX BEFORE\n");
  if(amix_pad == NULL) return NULL;
  g_print("AMIX AFTER\n");
  
  gchar* amix_pad_name = gst_pad_get_name(amix_pad);
  gchar* queue_name = g_strconcat(amix_name, "_", amix_pad_name, NULL);
  
  queue = gst_element_factory_make("queue", queue_name);
  gst_bin_add(GST_BIN(element), queue);
  gst_element_link_pads(queue, NULL, amixer, amix_pad_name);
  GstPad* queue_pad = gst_element_get_static_pad(queue, "sink");
  GstPad* ghost_pad = gst_ghost_pad_new (amix_pad_name, queue_pad);
  gst_element_add_pad (element, ghost_pad);

  g_free(amix_name);
  g_free(amix_pad_name);
  g_free(queue_name);
  gst_object_unref (GST_OBJECT (queue_pad));

  return ghost_pad;
  
};

static void release_pad_bin (GstElement *element, GstPad *pad) {

  gchar* element_name = GST_ELEMENT_NAME(element);
  gchar* amix_name = g_strconcat(element_name, "_amix", NULL);
  gchar* amix_pad_name = gst_pad_get_name(pad);
  gchar* queue_name = g_strconcat(amix_name, "_", amix_pad_name, NULL);
  
  gst_element_remove_pad (element, pad);
  GstElement* queue = gst_bin_get_by_name(GST_BIN(element), queue_name);
  gst_bin_remove(GST_BIN(element), queue);

  g_free(amix_name);
  g_free(amix_pad_name);
  g_free(queue_name);

}

dkc_rc gstbkn_make_audiomixer_bin(GstElement** bin, DkcParams* params) {
  
  *bin = gst_bin_new("audio_mixer");
  GstElementClass* klass = GST_ELEMENT_GET_CLASS(*bin);
  g_print("Audio class: %p \n", klass);
  klass->request_new_pad = request_new_pad_bin;
  klass->release_pad = release_pad_bin;

  gchar* element_name = GST_ELEMENT_NAME(*bin);
  gchar* amix_name = g_strconcat(element_name, "_amix", NULL);

  GstElement* amix = gst_element_factory_make("audiomixer", amix_name);
  GstElementClass* amix_klass = GST_ELEMENT_GET_CLASS(amix);
  klass->padtemplates = amix_klass->padtemplates;
  klass->numpadtemplates = amix_klass->numpadtemplates;
  klass->pad_templ_cookie = amix_klass->pad_templ_cookie;
  
  gst_bin_add(GST_BIN(*bin), amix);

  GstPad* amix_pad = gst_element_get_static_pad(amix, "src");
  gst_element_add_pad (*bin, gst_ghost_pad_new ("src", amix_pad));
  gst_object_unref (GST_OBJECT (amix_pad));

  g_free(amix_name);
  
  return OK;
  
}
