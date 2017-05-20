#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_init(void** ctx, DkcParams* params) {

  *ctx = (void*) malloc(sizeof(GstBackendCtx));
  GstBackendCtx* bkn_ctx = *ctx;

  if(gst_init_check (NULL, NULL, NULL) == FALSE) return ERROR;

  for(int i=0; i<NB_INPUTS; i++) bkn_ctx->inputs[i] = NULL;
  for(int i=0; i<NB_OUTPUTS; i++) bkn_ctx->outputs[i] = NULL;
  
  GstElement* outputs[NB_OUTPUTS];
  uint8_t nb_outputs;
  
  bkn_ctx->pipeline = gst_pipeline_new(NULL);
  
  //gstbkn_make_scenemanager_bin(&bkn_ctx->scenemanager, params);
  bkn_ctx->scenemanager = gst_element_factory_make("videomixer", NULL);
  gst_bin_add(GST_BIN(bkn_ctx->pipeline), bkn_ctx->scenemanager);
  
  //gstbkn_make_audiomixer_bin(&bkn_ctx->audiomixer, params);
  bkn_ctx->audiomixer = gst_element_factory_make("audiomixer", NULL);
  gst_bin_add(GST_BIN(bkn_ctx->pipeline), bkn_ctx->audiomixer);

  dkc_params_unref(params);
  
  return OK;
  
}
