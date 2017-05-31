#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_init(void** ctx, DkcParams* params) {

  *ctx = (void*) malloc(sizeof(GstBackendCtx));
  GstBackendCtx* bkn_ctx = *ctx;

  /* fetching parameters */
  int width = dkc_params_fetch_int(params, "width", 480);
  int height  = dkc_params_fetch_int(params, "height", 360);
  DkcFraction fps = dkc_params_fetch_fraction(params, "framerate", (DkcFraction){25, 1});
  gchar* v_format = dkc_params_fetch_string(params, "videoformat", "NV12");
  int channels = dkc_params_fetch_int(params, "channels", 2);
  int rate = dkc_params_fetch_int(params, "rate", 48000);
  gchar* a_format = dkc_params_fetch_string(params, "audioformat", "S16LE");

  bkn_ctx->v_caps = NULL;
  bkn_ctx->a_caps = NULL;

  dkc_params_unref(params);

  if(gst_init_check (NULL, NULL, NULL) == FALSE) return ERROR;

  /* Setting video specific caps */
  bkn_ctx->v_caps = gst_caps_new_simple("video/x-raw",
                                          "framerate", GST_TYPE_FRACTION, fps.num, fps.den,
                                          "format", G_TYPE_STRING, v_format,
                                          "width", G_TYPE_INT, width,
                                          "height", G_TYPE_INT, height,
                                          NULL);
  g_free(v_format);

  /* Setting audio specific caps */
  bkn_ctx->a_caps = gst_caps_new_simple("audio/x-raw",
                                          "rate", G_TYPE_INT, rate,
                                          "format", G_TYPE_STRING, a_format,
                                          "channels", G_TYPE_INT, channels,
                                          NULL);
  g_free(a_format);
  
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

  gst_element_link(bkn_ctx->video_selector, bkn_ctx->video_tee);
  gst_element_link(bkn_ctx->audio_selector, bkn_ctx->audio_tee);

  return OK;
  
}
