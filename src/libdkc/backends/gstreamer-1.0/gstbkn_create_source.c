#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/dkc_gst_sources.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_create_source(void* ctx, uint8_t id,  DkcSourceType src_type, const char* uri, const char* name, DkcParams* params) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  GstElement* source_bin = NULL;
  GstElement* source = NULL;
  GstElement* rate = NULL;
  GstElement* convert = NULL;
  GstElement* scale = NULL;
  GstElement* tee = NULL;
  GstCaps* rate_caps = NULL;
  GstCaps* convert_caps = NULL;
  GstCaps* scale_caps = NULL;
  
  int width = dkc_pop_int_param(params, "width", 480);
  int height  = dkc_pop_int_param(params, "height", 360);
  int fps = dkc_pop_int_param(params, "fps", 25);
  gchar* format = dkc_pop_string_param(params, "fps", "NV12");
  
  source_bin = gst_bin_new(name);
  rate = gst_element_factory_make("videorate", NULL);
  convert = gst_element_factory_make("videoconvert", NULL);
  scale = gst_element_factory_make("videoscale", NULL);
  tee = gst_element_factory_make("tee", NULL);

  dkc_params_ref(params);
  
  switch(src_type) {

    case DUMMY_SRC:
      gstbkn_create_source_dummy(&source, params);
    break;
    default:
      dkc_params_unref(params);
    break;
      
  }

  dkc_params_unref(params);
  
  if(!source_bin || !source || !rate || !convert || !scale || !tee) {

    if(source_bin) gst_object_unref(source_bin);
    if(source) gst_object_unref(source);
    if(rate) gst_object_unref(rate);
    if(convert) gst_object_unref(convert);
    if(scale) gst_object_unref(scale);
    if(tee) gst_object_unref(tee);

    return ERROR;
      
  }

  gst_bin_add_many(GST_BIN(source_bin), source, rate, convert, scale, tee, NULL);

  g_object_set(tee, "allow-not-linked", TRUE, NULL);

  rate_caps = gst_caps_new_simple("video/x-raw",
                             "framerate", GST_TYPE_FRACTION, fps, 1,
                             NULL);
  convert_caps = gst_caps_new_simple("video/x-raw",
                             "format", G_TYPE_STRING, format,
                             NULL);
  scale_caps = gst_caps_new_simple("video/x-raw",
                             "width", G_TYPE_INT, width,
                             "height", G_TYPE_INT, height,
                             NULL);
  
  gboolean link_res = gst_element_link(source, rate) &&
                       gst_element_link_filtered(rate, convert, rate_caps) &&
                       gst_element_link_filtered(convert, scale, convert_caps) &&
                       gst_element_link_filtered(scale, tee, scale_caps);
  
  free(format);
  
  gst_caps_unref(rate_caps);
  gst_caps_unref(convert_caps);
  gst_caps_unref(scale_caps);

  if(!link_res) {
    gst_bin_remove_many(GST_BIN(source_bin), source, rate, convert, scale, tee, NULL);
    return ERROR;
  }
  
  if(gst_ctx->inputs[id] == NULL) {
    gst_ctx->inputs[id] = source_bin;
    return OK;
  }
    
  return ERROR;
}
