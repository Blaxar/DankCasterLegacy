#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_wrap_source(void* ctx, uint8_t scn_id, uint8_t src_id, uint8_t id) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  /* Video specific elements */
  GstElement* v_queue = NULL;
  GstElement* v_rate = NULL;
  GstElement* v_convert = NULL;
  GstElement* v_scale = NULL;
  GstElement* v_filter = NULL;

  /* Audio specific elements */
  GstElement* a_queue = NULL;
  GstElement* a_rate = NULL;
  GstElement* a_convert = NULL;
  GstElement* a_filter = NULL;

  GstElement* source_bin = gst_ctx->inputs[src_id];
  GstScene* scene = gst_ctx->scenes[scn_id];
  if(scene->sources[id] != NULL) return ERROR;
  scene->sources[id] = gst_bin_new(NULL);

  gchar* video_tee_name = g_strconcat(GST_ELEMENT_NAME(source_bin), "_video_tee", NULL);
  gchar* audio_tee_name = g_strconcat(GST_ELEMENT_NAME(source_bin), "_audio_tee", NULL);

  GstElement* source_video_tee = gst_bin_get_by_name(GST_BIN(source_bin), video_tee_name);
  GstElement* source_audio_tee = gst_bin_get_by_name(GST_BIN(source_bin), audio_tee_name);

  g_free(video_tee_name);
  g_free(audio_tee_name);
  
  if(!source_video_tee && !source_audio_tee) { //If the source has no capabilities

    gst_object_unref(scene->sources[id]);
    scene->sources[id] = NULL;
    return ERROR;

  }

  gboolean link_res = TRUE;
  
  if(source_video_tee) { //Source has video capablities

    v_queue = gst_element_factory_make("queue", NULL);
    v_rate = gst_element_factory_make("videorate", NULL);
    v_convert = gst_element_factory_make("videoconvert", NULL);
    v_scale = gst_element_factory_make("videoscale", NULL);
    v_filter = gst_element_factory_make("capsfilter", NULL);

    gst_bin_add_many(GST_BIN(scene->sources[id]), v_queue,
                     v_rate, v_convert, v_scale, v_filter,
                     NULL);
    link_res = link_res && gst_element_link_many(v_queue, v_rate, v_convert, v_scale, v_filter, NULL);

    if(!link_res)
      gst_bin_remove_many(GST_BIN(scene->sources[id]), v_queue,
                          v_rate, v_convert, v_scale, v_filter,
                          NULL);

  }

  if(source_audio_tee) { //Source has audio capablities

    a_queue = gst_element_factory_make("queue", NULL);
    a_rate = gst_element_factory_make("audiorate", NULL);
    a_convert = gst_element_factory_make("audioconvert", NULL);
    a_filter = gst_element_factory_make("capsfilter", NULL);

    gst_bin_add_many(GST_BIN(scene->sources[id]), a_queue, a_rate, a_convert, a_filter,
                     NULL);
    link_res = link_res && gst_element_link_many(a_queue, a_rate, a_convert, a_filter, NULL);

    if(!link_res)
      gst_bin_remove_many(GST_BIN(scene->sources[id]), a_queue, a_rate, a_convert, a_filter,
                          NULL);

  }

  if(!link_res) {
    gst_object_unref(scene->sources[id]);
    scene->sources[id] = NULL;
    return ERROR;
  }

  if(source_video_tee) {
    
    GstPad* sink_pad = gst_element_get_static_pad(v_queue, "sink");
    gst_element_add_pad(scene->sources[id], gst_ghost_pad_new("video_sink", sink_pad));
    gst_object_unref(GST_OBJECT(sink_pad));

    GstPad* src_pad = gst_element_get_static_pad(v_filter, "src");
    gst_element_add_pad(scene->sources[id], gst_ghost_pad_new("video_src", src_pad));
    gst_object_unref(GST_OBJECT(src_pad));
    
  }

  if(source_audio_tee) {
  
    GstPad* sink_pad = gst_element_get_static_pad(a_queue, "sink");
    gst_element_add_pad(scene->sources[id], gst_ghost_pad_new("audio_sink", sink_pad));
    gst_object_unref(GST_OBJECT(sink_pad));

    GstPad* src_pad = gst_element_get_static_pad(a_filter, "src");
    gst_element_add_pad(scene->sources[id], gst_ghost_pad_new("audio_src", src_pad));
    gst_object_unref(GST_OBJECT(src_pad));
    
  }
      
  gst_bin_add(GST_BIN(gst_ctx->pipeline), scene->sources[id]);
  gst_element_set_locked_state(source_bin, FALSE);
  
  if(source_video_tee) {

      link_res = link_res && gst_element_link_pads(source_video_tee, NULL, scene->sources[id], "video_sink") &&
                             gst_element_link_pads(scene->sources[id], "video_src", gst_ctx->scenes[scn_id]->video_mixer, NULL);

  }

  if(source_audio_tee) {

    link_res = link_res && gst_element_link_pads(source_audio_tee, NULL, scene->sources[id], "audio_sink") &&
                           gst_element_link_pads(scene->sources[id], "audio_src", gst_ctx->scenes[scn_id]->audio_mixer, NULL);

  }

  if(!link_res) {
    gst_bin_remove_many(GST_BIN(gst_ctx->pipeline), source_bin,
                        scene->sources[id],
                        NULL);
    scene->sources[id] = NULL;
    return ERROR;
  }

  return OK;
  
}
