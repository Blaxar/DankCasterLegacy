#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_create_source(void* ctx, uint8_t id, const char* type, const char* name, ...) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  GstElement* source_bin = NULL;
  GstElement* source = NULL;
  GstElement* rate = NULL;
  GstElement* convert = NULL;
  GstElement* scale = NULL;
  GstElement* tee = NULL;
  GstCaps* caps = NULL;

  int width, height, fps;
  gchar* format;
  
  source_bin = gst_bin_new(name);
  source = gst_element_factory_make(type, NULL);
  rate = gst_element_factory_make("videorate", NULL);
  convert = gst_element_factory_make("videoconvert", NULL);
  scale = gst_element_factory_make("videoscale", NULL);
  tee = gst_element_factory_make("tee", NULL);
  
  if(!source_bin || !source || !rate || !convert || !scale || !tee) goto error;

  gst_bin_add_many(GST_BIN(source_bin), source, rate, convert, scale, tee, NULL);

  gst_element_link(source, rate);
  
  caps = gst_caps_new_simple("video/x-raw",
                             "framerate", GST_TYPE_FRACTION, 1, fps,
                             NULL);
  if(!gst_element_link_filtered(rate, convert, caps)) goto error;
  gst_caps_unref(caps);

  caps = gst_caps_new_simple("video/x-raw",
                             "format", G_TYPE_STRING, format,
                             NULL);
  if(!gst_element_link_filtered(convert, scale, caps)) goto error;
  gst_caps_unref(caps);

  caps = gst_caps_new_simple("video/x-raw",
                             "width", G_TYPE_INT, width,
                             "height", G_TYPE_INT, height,
                             NULL);
  if(!gst_element_link_filtered(scale, tee, caps)) goto error;
  gst_caps_unref(caps);
  
  if(gst_ctx->inputs[id] == NULL) {
    gst_ctx->inputs[id] = source_bin;
    return OK;
  }

  error:

  if(source_bin) gst_object_unref(source_bin);
  if(source) gst_object_unref(source);
  if(rate) gst_object_unref(rate);
  if(convert) gst_object_unref(convert);
  if(scale) gst_object_unref(scale);
  if(tee) gst_object_unref(tee);
  if(caps) gst_caps_unref(caps);
  
  return ERROR;
}
