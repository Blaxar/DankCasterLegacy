#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_init(void** ctx, DkcParams* params) {

  *ctx = (void*) malloc(sizeof(GstBackendCtx));
  GstBackendCtx* bkn_ctx = *ctx;

  if(gst_init_check (NULL, NULL, NULL) == FALSE) return ERROR;

  for(int i=0; i<NB_INPUTS; i++) bkn_ctx->inputs[i] = NULL;
  for(int i=0; i<NB_SCENES; i++) bkn_ctx->scenes[i] = NULL;
  for(int i=0; i<NB_OUTPUTS; i++) bkn_ctx->outputs[i] = NULL;

  bkn_ctx->nb_inputs = 0;
  bkn_ctx->nb_scenes = 0;
  bkn_ctx->nb_outputs = 0;
  
  bkn_ctx->pipeline = gst_pipeline_new(NULL);

  bkn_ctx->video_selector = gst_element_factory_make("input-selector", "video_selector");
  bkn_ctx->audio_selector = gst_element_factory_make("input-selector", "audio_selector");

  bkn_ctx->video_tee = gst_element_factory_make("tee", "video_tee");
  bkn_ctx->audio_tee = gst_element_factory_make("tee", "audio_tee");

  gst_bin_add_many(GST_BIN(bkn_ctx->pipeline),
                   bkn_ctx->video_selector, bkn_ctx->audio_selector,
                   bkn_ctx->video_tee, bkn_ctx->audio_tee,
                   NULL);

  g_object_set(bkn_ctx->video_tee, "allow-not-linked", TRUE, NULL);
  g_object_set(bkn_ctx->audio_tee, "allow-not-linked", TRUE, NULL);

  gst_element_link_many(bkn_ctx->video_selector, bkn_ctx->video_tee, NULL);
  gst_element_link_many(bkn_ctx->audio_selector, bkn_ctx->audio_tee, NULL);

  dkc_params_unref(params);
  
  return OK;
  
}
