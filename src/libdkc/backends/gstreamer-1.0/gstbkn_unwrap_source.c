#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_unwrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t wrpsrc_id) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  /* get the wrapped source first */
  GstScene* scene = gst_ctx->scenes[scn_id];
  GstElement* wrpsrc = scene->sources[wrpsrc_id];
  GstElement* source_bin = gst_ctx->inputs[src_id];
  if(scene->sources[wrpsrc_id] == NULL) return ERROR;

  gchar* video_tee_name = g_strconcat(GST_ELEMENT_NAME(source_bin), "_video_tee", NULL);
  gchar* audio_tee_name = g_strconcat(GST_ELEMENT_NAME(source_bin), "_audio_tee", NULL);

  GstElement* source_video_tee = gst_bin_get_by_name(GST_BIN(source_bin), video_tee_name);
  GstElement* source_audio_tee = gst_bin_get_by_name(GST_BIN(source_bin), audio_tee_name);

  g_free(video_tee_name);
  g_free(audio_tee_name);

  if(source_video_tee) { // Source has video capablities

    // Iterate over each video tee src pad
    // to check if some of them are still connected
    
  }

  if(source_audio_tee) { // Source has audio capablities

    // Iterate over each audio tee src pad
    // to check if some of them are still connected

  }
  
  gst_bin_remove(GST_BIN(gst_ctx->pipeline), wrpsrc);
  
}
