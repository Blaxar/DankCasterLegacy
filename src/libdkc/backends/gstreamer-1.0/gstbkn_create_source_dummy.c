#include <libdkc/backends/gstreamer-1.0/dkc_gst_sources.h>

dkc_rc gstbkn_create_source_dummy(GstElement** source, DkcParams* params) {

  GstElement *video_src = NULL;
  GstElement *audio_src = NULL;
  GstPad *video_src_pad = NULL;
  GstPad *audio_src_pad = NULL;
  
  *source = gst_bin_new(NULL);
  
  video_src = gst_element_factory_make("videotestsrc", NULL);
  audio_src = gst_element_factory_make("audiotestsrc", NULL);

  gst_bin_add_many(GST_BIN(*source), video_src, audio_src,
                     NULL);

  video_src_pad = gst_element_get_static_pad(video_src, "src");
  gst_element_add_pad(*source, gst_ghost_pad_new("video_src", video_src_pad));
  gst_object_unref(GST_OBJECT(video_src_pad));

  audio_src_pad = gst_element_get_static_pad(audio_src, "src");
  gst_element_add_pad(*source, gst_ghost_pad_new("audio_src", audio_src_pad));
  gst_object_unref(GST_OBJECT(audio_src_pad));
  
  return OK;
  
}
