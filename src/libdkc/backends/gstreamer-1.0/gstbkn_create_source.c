#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/gstreamer-1.0/dkc_gst_sources.h>
#include <libdkc/backends/gstreamer-1.0/gstbackendctx.h>
#include <stdlib.h>

gboolean gstbkn_create_source(void* ctx, uint8_t id,  DkcSourceType src_type, const char* uri, const char* name, DkcParams* params) {

  GstBackendCtx* gst_ctx = (GstBackendCtx*) ctx;

  /* Base elements */
  GstElement* source_bin = NULL;
  GstElement* source = NULL;

  /* Video specific elements */
  GstElement* v_rate = NULL;
  GstElement* v_convert = NULL;
  GstElement* v_scale = NULL;
  GstElement* v_tee = NULL;

  /* Audio specific elements */
  GstElement* a_rate = NULL;
  GstElement* a_convert = NULL;
  GstElement* a_tee = NULL;

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
  int width = dkc_params_fetch_int(params, "width", 480);
  int height  = dkc_params_fetch_int(params, "height", 360);
  DkcFraction fps = dkc_params_fetch_fraction(params, "framerate", (DkcFraction){25, 1});
  gchar* v_format = dkc_params_fetch_string(params, "videoformat", "NV12");
  int channels = dkc_params_fetch_int(params, "channels", 2);
  int rate = dkc_params_fetch_int(params, "rate", 48000);
  gchar* a_format = dkc_params_fetch_string(params, "audioformat", "S16LE");

  if(gst_ctx->inputs[id] != NULL) {
    g_critical("Source [%d] already exists.", id);
    return ERROR;
  }

  source_bin = gst_bin_new(name);

  dkc_params_ref(params);

  switch(src_type) {

    case DUMMY_SRC:
      gstbkn_create_source_dummy(&source, params);
    break;
    case VIDEO_DEV_SRC:
      gstbkn_create_source_v4l2(&source, uri, params);
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

  video_pad = gst_element_get_static_pad(source, "video_src");
  if(video_pad){ // If the source has video capabilites...

    gchar* video_tee_name = g_strconcat(name, "_video_tee", NULL);

    /* Making video specific elements */
    v_rate = gst_element_factory_make("videorate", NULL);
    v_convert = gst_element_factory_make("videoconvert", NULL);
    v_scale = gst_element_factory_make("videoscale", NULL);
    v_tee = gst_element_factory_make("tee", video_tee_name);

    gst_bin_add_many(GST_BIN(source_bin), v_rate, v_convert, v_scale, v_tee, NULL);

    g_object_set(v_tee, "allow-not-linked", TRUE, NULL);

    g_free(video_tee_name);

  }

  audio_pad = gst_element_get_static_pad(source, "audio_src");
  if(audio_pad){ // If the source has audio capabilites...

    gchar* audio_tee_name = g_strconcat(name, "_audio_tee", NULL);

    /* Making audio specific elements */
    a_rate = gst_element_factory_make("audiorate", NULL);
    a_convert = gst_element_factory_make("audioconvert", NULL);
    a_tee = gst_element_factory_make("tee", audio_tee_name);

    gst_bin_add_many(GST_BIN(source_bin), a_rate, a_convert, a_tee, NULL);

    g_object_set(a_tee, "allow-not-linked", TRUE, NULL);

    g_free(audio_tee_name);

  }

  if(!video_pad && !audio_pad) { // If the source has no capabilities

    gst_bin_remove(GST_BIN(source_bin), source);
    gst_object_unref(source_bin);

    g_critical("Source [%d] has no capabilities.", id);

    return ERROR;

  }

  if(video_pad) gst_object_unref(video_pad);
  if(audio_pad) gst_object_unref(audio_pad);

  gboolean link_err = FALSE;

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

    if(!(gst_element_link(source, v_rate) &&
         gst_element_link_filtered(v_rate, v_convert, v_rate_caps) &&
         gst_element_link_filtered(v_convert, v_scale, v_convert_caps) &&
         gst_element_link_filtered(v_scale, v_tee, v_scale_caps))) {
      g_critical("Could not link source [%d] video formatting elements together.", id);
      gst_bin_remove_many(GST_BIN(source_bin), source, v_rate, v_convert, v_scale, v_tee, NULL);
      link_err = TRUE;
    }

    gst_caps_unref(v_rate_caps);
    gst_caps_unref(v_convert_caps);
    gst_caps_unref(v_scale_caps);

  }

  if(a_rate) { // If the source has audio capabilites...

    /* Setting audio specific caps */
    a_rate_caps = gst_caps_new_simple("audio/x-raw",
                             "rate", G_TYPE_INT, rate,
                             NULL);
    a_convert_caps = gst_caps_new_simple("audio/x-raw",
                             "format", G_TYPE_STRING, a_format,
                             "channels", G_TYPE_INT, channels,
                             NULL);

    g_free(a_format);

    if(!(gst_element_link(source, a_rate) &&
         gst_element_link_filtered(a_rate, a_convert, a_rate_caps) &&
         gst_element_link_filtered(a_convert, a_tee, a_convert_caps))) {
      g_critical("Could not link source [%d] audio formatting elements together.", id);
      gst_bin_remove_many(GST_BIN(source_bin), source, a_rate, a_convert, a_tee, NULL);
      link_err = TRUE;
    }

    gst_caps_unref(a_rate_caps);
    gst_caps_unref(a_convert_caps);

  }

  if(link_err){
      gst_object_unref(GST_OBJECT(source_bin));
      return ERROR;
  }

  gst_ctx->inputs[id] = source_bin;
  gst_ctx->nb_inputs++;
  gst_element_set_locked_state(source_bin, TRUE);
  gst_bin_add(GST_BIN(gst_ctx->pipeline), source_bin);
  return OK;

}
