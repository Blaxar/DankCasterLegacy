#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/dkc_gst_sources.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

dkc_rc gstbkn_create_source(void* ctx, uint8_t id,  DkcSourceType src_type, const char* uri, const char* name, DkcParams* params) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;


  /* Base elements */
  GstElement* source_bin = NULL;
  GstElement* source = NULL;

  /* Video specific elements */
  GstElement* v_rate = NULL;
  GstElement* v_convert = NULL;
  GstElement* v_scale = NULL;
  GstElement* v_valve = NULL;

  /* Audio specific elements */
  GstElement* a_rate = NULL;
  GstElement* a_convert = NULL;
  GstElement* a_valve = NULL;

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
  
  source_bin = gst_bin_new(name);

  dkc_params_ref(params);
  
  switch(src_type) {

    case DUMMY_SRC:
      gstbkn_create_source_dummy(&source, params);
    break;
    default:
    break;
      
  }

  dkc_params_unref(params);

  if(!source_bin || !source) {

    // Something went wrong, either source or bin not created
    
    if(source_bin) gst_object_unref(GST_OBJECT(source_bin));
    if(source) gst_object_unref(GST_OBJECT(source));

    return ERROR;

  }

  gst_bin_add(GST_BIN(source_bin), source);
  
  video_pad = gst_element_get_static_pad(source, "video_pad");
  if(video_pad){ // If the source has video capabilites...
  
    /* Making video specific elements */
    v_rate = gst_element_factory_make("videorate", NULL);
    v_convert = gst_element_factory_make("videoconvert", NULL);
    v_scale = gst_element_factory_make("videoscale", NULL);
    v_valve = gst_element_factory_make("valve", NULL);

    /* Setting video specific caps */
    v_rate_caps = gst_caps_new_simple("video/x-raw",
                             "framerate", GST_TYPE_FRACTION, fps, 1,
                             NULL);
    v_convert_caps = gst_caps_new_simple("video/x-raw",
                             "format", G_TYPE_STRING, v_format,
                             NULL);
    v_scale_caps = gst_caps_new_simple("video/x-raw",
                             "width", G_TYPE_INT, width,
                             "height", G_TYPE_INT, height,
                             NULL);

    g_free(v_format);

    gst_bin_add_many(GST_BIN(source_bin), v_rate, v_convert, v_scale, v_valve, NULL);

  }

  audio_pad = gst_element_get_static_pad(source, "audio_pad");
  if(audio_pad){ // If the source has audio capabilites...
  
    /* Making audio specific elements */
    a_rate = gst_element_factory_make("audiorate", NULL);
    a_convert = gst_element_factory_make("audioconvert", NULL);
    a_valve = gst_element_factory_make("valve", NULL);

    /* Setting video specific caps */
    a_rate_caps = gst_caps_new_simple("audio/x-raw",
                             "rate", G_TYPE_INT, rate,
                             NULL);
    a_convert_caps = gst_caps_new_simple("audio/x-raw",
                             "format", G_TYPE_STRING, a_format,
                             "channels", G_TYPE_INT, channels,
                             NULL);

    g_free(a_format);

    gst_bin_add_many(GST_BIN(source_bin), a_rate, a_convert, a_valve, NULL);
    
  }

  if(!video_pad && !audio_pad) { //If the source has no capabilities

    gst_bin_remove(GST_BIN(source_bin), source);
    gst_object_unref(source_bin);

    return ERROR;

  }

  if(video_pad) gst_object_unref(video_pad);
  if(audio_pad) gst_object_unref(audio_pad);

  g_object_set(v_valve, "allow-not-linked", TRUE, NULL);
  g_object_set(a_valve, "allow-not-linked", TRUE, NULL);

  gboolean link_res = TRUE;
  
  if(v_rate) { // If the source has video capabilites...
    
    link_res = link_res && gst_element_link(source, v_rate) &&
               gst_element_link_filtered(v_rate, v_convert, v_rate_caps) &&
               gst_element_link_filtered(v_convert, v_scale, v_convert_caps) &&
               gst_element_link_filtered(v_scale, v_valve, v_scale_caps);
  
    gst_caps_unref(v_rate_caps);
    gst_caps_unref(v_convert_caps);
    gst_caps_unref(v_scale_caps);

    if(!link_res)
      gst_bin_remove_many(GST_BIN(source_bin), source, v_rate, v_convert, v_scale, v_valve, NULL);

  }

  if(a_rate) { // If the source has audio capabilites...
    
    link_res = link_res && gst_element_link(source, a_rate) &&
               gst_element_link_filtered(a_rate, a_convert, a_rate_caps) &&
               gst_element_link_filtered(a_convert, a_valve, a_convert_caps);
  
    gst_caps_unref(a_rate_caps);
    gst_caps_unref(a_convert_caps);

    if(!link_res)
      gst_bin_remove_many(GST_BIN(source_bin), source, a_rate, a_convert, a_valve, NULL);

  }

  if(!link_res){
      gst_object_unref(GST_OBJECT(source_bin));
      return ERROR;
  }
  
  if(gst_ctx->inputs[id] == NULL) {

    gst_bin_add(GST_BIN(gst_ctx->pipeline), source_bin);

    /* Link video/audio streams to the video-mixer/audio-mixer */
    
    if(v_rate) {
      video_pad = gst_element_get_static_pad(v_valve, "src");
      gst_element_add_pad(source_bin, gst_ghost_pad_new("video_src", video_pad));
      gst_object_unref(GST_OBJECT(video_pad));
      gst_element_link(source_bin, gst_ctx->scenemanager);
    }

    if(a_rate) {
      audio_pad = gst_element_get_static_pad(a_valve, "src");
      gst_element_add_pad(source_bin, gst_ghost_pad_new("audio_src", audio_pad));
      gst_object_unref(GST_OBJECT(audio_pad));
      gst_element_link(source_bin, gst_ctx->audiomixer);
    }
      
    gst_ctx->inputs[id] = source_bin;
    return OK;
    
  }

  gst_bin_remove(GST_BIN(source_bin), source);
  gst_object_unref(source_bin);
  
  return ERROR;
  
}
