#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

gboolean gstbkn_unwrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t wrpsrc_id) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  /* get the wrapped source first */
  GstScene* scene = gst_ctx->scenes[scn_id];
  GstElement* wrpsrc = scene->sources[wrpsrc_id];
  GstElement* source_bin = gst_ctx->inputs[src_id];
  
  if(scene->sources[wrpsrc_id] == NULL) {
    g_critical("Wrapped source [%d] does not exist in scene [%d].", wrpsrc_id, scn_id);
    return ERROR;
  }

  gchar* video_tee_name = g_strconcat(GST_ELEMENT_NAME(source_bin), "_video_tee", NULL);
  gchar* audio_tee_name = g_strconcat(GST_ELEMENT_NAME(source_bin), "_audio_tee", NULL);

  GstElement* source_video_tee = gst_bin_get_by_name(GST_BIN(source_bin), video_tee_name);
  GstElement* source_audio_tee = gst_bin_get_by_name(GST_BIN(source_bin), audio_tee_name);

  g_free(video_tee_name);
  g_free(audio_tee_name);

  GstGhostPad* video_ghost_pad = NULL;
  GstPad* video_tee_pad = NULL;
  GstGhostPad* audio_ghost_pad = NULL;
  GstPad* audio_tee_pad = NULL;

  gboolean pad_err = FALSE;
  
  if(source_video_tee) { // Source has video capablities
    
    GstPad* video_sink_pad = gst_element_get_static_pad(wrpsrc, "video_sink");
    if(video_sink_pad == NULL) {
      g_critical("Wrapped source [%d] has no 'video_sink' pad despite having video capabilities.", wrpsrc_id);
      pad_err = TRUE;
    } else {
      
      video_ghost_pad = GST_GHOST_PAD(gst_pad_get_peer(video_sink_pad));
      gst_object_unref(video_sink_pad);
      
      if(video_ghost_pad == NULL) {
        g_critical("Wrapped source [%d] 'video_sink' pad not linked.", wrpsrc_id);
        pad_err = TRUE;
      } else if(GST_PAD_PARENT(video_ghost_pad) != source_bin) {
        g_critical("Wrapped source [%d] 'video_sink' linked to something other than the source ghost pad.", wrpsrc_id);
        gst_object_unref(video_ghost_pad);
        pad_err = TRUE;
      } else {
        video_tee_pad = gst_ghost_pad_get_target(video_ghost_pad);
        if(GST_PAD_PARENT(video_tee_pad) != source_video_tee) {
          g_critical("Wrapped source [%d] 'video_sink' linked to something other than the inner video tee pad through the ghost pad.", wrpsrc_id);
          gst_object_unref(video_ghost_pad);
          gst_object_unref(video_tee_pad);
          pad_err = TRUE;
        }
      }
      
    }
    
  }
  
  if(source_audio_tee) { // Source has audio capablities

    GstPad* audio_sink_pad = gst_element_get_static_pad(wrpsrc, "audio_sink");
    if(audio_sink_pad == NULL) {
      g_critical("Wrapped source [%d] has no 'audio_sink' pad despite having audio capabilities.", wrpsrc_id);
      pad_err = TRUE;
    } else {
      
      audio_ghost_pad = GST_GHOST_PAD(gst_pad_get_peer(audio_sink_pad));
      gst_object_unref(audio_sink_pad);
      
      if(audio_ghost_pad == NULL) {
        g_critical("Wrapped source [%d] 'audio_sink' pad not linked.", wrpsrc_id);
        pad_err = TRUE;
      } else if(GST_PAD_PARENT(audio_ghost_pad) != source_bin) {
        g_critical("Wrapped source [%d] 'audio_sink' linked to something other than the source ghost pad.", wrpsrc_id);
        gst_object_unref(audio_ghost_pad);
        pad_err = TRUE;
      } else {
        audio_tee_pad = gst_ghost_pad_get_target(audio_ghost_pad);
        if(GST_PAD_PARENT(audio_tee_pad) != source_audio_tee) {
          g_critical("Wrapped source [%d] 'audio_sink' linked to something other than the inner audio tee pad through the ghost pad.", wrpsrc_id);
          gst_object_unref(audio_ghost_pad);
          gst_object_unref(audio_tee_pad);
          pad_err = TRUE;
        }
      }
      
    }

  }

  if(pad_err) return ERROR;

  if(video_ghost_pad) // Remove ghost video pad from source bin
    gst_element_remove_pad(source_bin, GST_PAD(video_ghost_pad));

  if(video_tee_pad) { // Release pad from video source tee
    gst_element_release_request_pad(source_video_tee, video_tee_pad);
    gst_object_unref(video_tee_pad);
  }

  if(audio_ghost_pad) // Remove ghost audio pad from source bin
    gst_element_remove_pad(source_bin, GST_PAD(audio_ghost_pad));

  if(audio_tee_pad) { // Release pad from audio source tee
    gst_element_release_request_pad(source_audio_tee, audio_tee_pad);
    gst_object_unref(audio_tee_pad);
  }
    

  gst_bin_remove(GST_BIN(gst_ctx->pipeline), wrpsrc);
  scene->sources[wrpsrc_id] = NULL;

  return OK;
  
}
