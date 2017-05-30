#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_create_scene(void* ctx, uint8_t id) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  gst_ctx->scenes[id] = malloc(sizeof(GstScene));
  for(int i=0; i<NB_INPUTS; i++) gst_ctx->scenes[id]->sources[i] = NULL;
  gst_ctx->scenes[id]->nb_sources = 0;

  gst_ctx->scenes[id]->video_mixer = gst_element_factory_make("videomixer", NULL);
  gst_bin_add(GST_BIN(gst_ctx->pipeline), gst_ctx->scenes[id]->video_mixer);
  
  gst_ctx->scenes[id]->audio_mixer = gst_element_factory_make("audiomixer", NULL);
  gst_bin_add(GST_BIN(gst_ctx->pipeline), gst_ctx->scenes[id]->audio_mixer);

  //gst_element_link_filtered(gst_ctx->scenes[id]->video_mixer, gst_ctx->video_selector, gst_ctx->v_caps);
  //gst_element_link_filtered(gst_ctx->scenes[id]->audio_mixer, gst_ctx->audio_selector, gst_ctx->a_caps);

  gst_element_link(gst_ctx->scenes[id]->video_mixer, gst_ctx->video_selector);
  gst_element_link(gst_ctx->scenes[id]->audio_mixer, gst_ctx->audio_selector);

  return OK;
  
}
