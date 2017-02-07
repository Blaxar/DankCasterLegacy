#include <libdkcst/backends/gstreamer-1.0/dkcst_gst_backend.h>
#include <libdkcst/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

DkCst_rc gstbkn_create_source(void* ctx, uint8_t id, const char* type, const char* name, DkCstParams* params) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  GstElement* source_bin = NULL;
  GstElement* source = NULL;
  GstElement* rate = NULL;
  GstElement* convert = NULL;
  GstElement* size = NULL;
  
  source_bin = gst_bin_new(name);
  source = gst_element_factory_make(type, NULL);
  
  if(gst_ctx->inputs[id] == NULL) {
    bkn_ctx->inputs[id] = source_bin;
    return OK;
  }

  error:
  
  return ERROR;
}
