#include <libdkc/backends/gstreamer-1.0/dkc_gst_sinks.h>

dkc_rc gstbkn_create_sink_dummy(GstElement** sink, DkcParams* params) {

  GstElement *video_snk = NULL;
  GstElement *audio_snk = NULL;
  GstPad *video_snk_pad = NULL;
  GstPad *audio_snk_pad = NULL;
  
  *sink = gst_bin_new(NULL);
  
  video_snk = gst_element_factory_make("autovideosink", NULL);
  audio_snk = gst_element_factory_make("autoaudiosink", NULL);

  gst_bin_add_many(GST_BIN(*sink), video_snk, audio_snk,
                   NULL);

  video_snk_pad = gst_element_get_static_pad(video_snk, "sink");
  gst_element_add_pad(*sink, gst_ghost_pad_new("video_sink", video_snk_pad));
  gst_object_unref(GST_OBJECT(video_snk_pad));

  audio_snk_pad = gst_element_get_static_pad(audio_snk, "sink");
  gst_element_add_pad(*sink, gst_ghost_pad_new("audio_sink", audio_snk_pad));
  gst_object_unref(GST_OBJECT(audio_snk_pad));
  
  return OK;
  
}
