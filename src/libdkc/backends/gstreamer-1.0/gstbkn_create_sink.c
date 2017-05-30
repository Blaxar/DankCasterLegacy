#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/dkc_gst_sinks.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_create_sink(void* ctx, uint8_t id, DkcSinkType snk_type, const char* type, const char* name, DkcParams* params) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;


  /* Base elements */
  GstElement* sink_bin = NULL;
  GstElement* sink = NULL;

  /* Video specific elements */
  GstElement* v_queue = NULL;
  GstElement* v_rate = NULL;
  GstElement* v_convert = NULL;
  GstElement* v_scale = NULL;
  GstElement* v_convert2 = NULL;

  /* Audio specific elements */
  GstElement* a_queue = NULL;
  GstElement* a_rate = NULL;
  GstElement* a_convert = NULL;

  /* Video specific caps */
  GstCaps* v_rate_caps = NULL;
  GstCaps* v_convert_caps = NULL;
  GstCaps* v_scale_caps = NULL;

  /* Audio specific caps */
  GstCaps* a_rate_caps = NULL;
  GstCaps* a_convert_caps = NULL;

  GstPad *video_pad = NULL;
  GstPad *audio_pad = NULL;

  /* fetching parameters */
  int width = dkc_params_pop_int(params, "width", 480);
  int height  = dkc_params_pop_int(params, "height", 360);
  DkcFraction fps = dkc_params_pop_fraction(params, "framerate", (DkcFraction){25, 1});
  gchar* v_format = dkc_params_pop_string(params, "videoformat", "NV12");
  int channels = dkc_params_pop_int(params, "channels", 2);
  int rate = dkc_params_pop_int(params, "rate", 48000);
  gchar* a_format = dkc_params_pop_string(params, "audioformat", "S16LE");
  
  sink_bin = gst_bin_new(name);

  dkc_params_ref(params);
  
  switch(snk_type) {

    case DUMMY_SNK:
      gstbkn_create_sink_dummy(&sink, params);
    break;
    default:
    break;
      
  }

  dkc_params_unref(params);

  if(!sink_bin || !sink) {

    // Something went wrong, either sink or bin not created
    
    if(sink_bin) gst_object_unref(GST_OBJECT(sink_bin));
    if(sink) gst_object_unref(GST_OBJECT(sink));

    return ERROR;

  }

  gst_bin_add(GST_BIN(sink_bin), sink);
  
  video_pad = gst_element_get_static_pad(sink, "video_sink");
  if(video_pad){ // If the source has video capabilites...
  
    /* Making video specific elements */
    v_queue = gst_element_factory_make("queue", NULL);
    v_rate = gst_element_factory_make("videorate", NULL);
    v_convert = gst_element_factory_make("videoconvert", NULL);
    v_scale = gst_element_factory_make("videoscale", NULL);
    v_convert2 = gst_element_factory_make("videoconvert", NULL);

    gst_bin_add_many(GST_BIN(sink_bin), v_queue, v_rate, v_convert, v_scale, v_convert2, NULL);

  }

  audio_pad = gst_element_get_static_pad(sink, "audio_sink");
  if(audio_pad){ // If the source has audio capabilites...
  
    /* Making audio specific elements */
    a_queue = gst_element_factory_make("queue", NULL);
    a_rate = gst_element_factory_make("audiorate", NULL);
    a_convert = gst_element_factory_make("audioconvert", NULL);

    gst_bin_add_many(GST_BIN(sink_bin), a_queue, a_rate, a_convert, NULL);
    
  }

  if(!video_pad && !audio_pad) { //If the sink has no capabilities

    gst_bin_remove(GST_BIN(sink_bin), sink);
    gst_object_unref(sink_bin);

    return ERROR;

  }

  if(video_pad) gst_object_unref(video_pad);
  if(audio_pad) gst_object_unref(audio_pad);

  gboolean link_res = TRUE;
  
  if(v_rate) { // If the source has video capabilites...

        /* Setting video specific caps */
    v_rate_caps = gst_caps_new_simple("video/x-raw",
                             "framerate", GST_TYPE_FRACTION, fps.num, fps.den,
                             NULL);
    v_convert_caps = gst_caps_new_simple("video/x-raw",
                             "format", G_TYPE_STRING, v_format,
                             NULL);
    v_scale_caps = gst_caps_new_simple("video/x-raw",
                             "width", G_TYPE_INT, width,
                             "height", G_TYPE_INT, height,
                             NULL);

    g_free(v_format);
    
    link_res = link_res && gst_element_link(v_queue, v_rate) &&
               gst_element_link_filtered(v_rate, v_convert, v_rate_caps) &&
               gst_element_link_filtered(v_convert, v_scale, v_convert_caps) &&
               gst_element_link_filtered(v_scale, v_convert2, v_scale_caps) &&
               gst_element_link(v_convert2, sink);
  
    gst_caps_unref(v_rate_caps);
    gst_caps_unref(v_convert_caps);
    gst_caps_unref(v_scale_caps);

    if(!link_res)
      gst_bin_remove_many(GST_BIN(sink_bin), v_queue, v_rate, v_convert, v_scale, v_convert2, sink, NULL);

  }

  if(a_rate) { // If the source has audio capabilites...

    /* Setting video specific caps */
    a_rate_caps = gst_caps_new_simple("audio/x-raw",
                             "rate", G_TYPE_INT, rate,
                             NULL);
    a_convert_caps = gst_caps_new_simple("audio/x-raw",
                             "format", G_TYPE_STRING, a_format,
                             "channels", G_TYPE_INT, channels,
                             NULL);

    g_free(a_format);
      
    link_res = link_res && gst_element_link(a_queue, a_rate) &&
               gst_element_link_filtered(a_rate, a_convert, a_rate_caps) &&
               gst_element_link_filtered(a_convert, sink, a_convert_caps);
  
    gst_caps_unref(a_rate_caps);
    gst_caps_unref(a_convert_caps);

    if(!link_res)
      gst_bin_remove_many(GST_BIN(sink_bin), a_queue, a_rate, a_convert, sink, NULL);

  }

  if(!link_res){
      gst_object_unref(GST_OBJECT(sink_bin));
      return ERROR;
  }
  
  if(gst_ctx->outputs[id] == NULL) {

    gst_bin_add(GST_BIN(gst_ctx->pipeline), sink_bin);

    /* Link video/audio streams to the video-mixer/audio-mixer */
    
    if(v_rate) {
      video_pad = gst_element_get_static_pad(v_queue, "sink");
      gst_element_add_pad(sink_bin, gst_ghost_pad_new("video_sink", video_pad));
      gst_object_unref(GST_OBJECT(video_pad));
      link_res = link_res && gst_element_link(gst_ctx->video_tee, sink_bin);
    }

    if(a_rate) {
      audio_pad = gst_element_get_static_pad(a_queue, "sink");
      gst_element_add_pad(sink_bin, gst_ghost_pad_new("audio_sink", audio_pad));
      gst_object_unref(GST_OBJECT(audio_pad));
      link_res = link_res && gst_element_link(gst_ctx->audio_tee, sink_bin);
    }

    if(!link_res){
      gst_bin_remove(GST_BIN(gst_ctx->pipeline), sink_bin);
      return ERROR;
    }
      
    gst_ctx->outputs[id] = sink_bin;
    gst_ctx->nb_outputs++;
    return OK;
    
  }

  gst_bin_remove(GST_BIN(sink_bin), sink);
  gst_object_unref(sink_bin);
  
  return ERROR;
  
}
