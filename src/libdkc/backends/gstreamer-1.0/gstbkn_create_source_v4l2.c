#include <libdkc/backends/gstreamer-1.0/dkc_gst_sources.h>

gboolean gstbkn_create_source_v4l2(GstElement** source, const char* uri, DkcParams* params) {

  GstElement *video_src = NULL;
  GstElement *filter = NULL;
  GstPad *video_src_pad = NULL;
  
  *source = gst_bin_new(NULL);
  
  video_src = gst_element_factory_make("v4l2src", NULL);

  gst_bin_add_many(GST_BIN(*source), video_src,
                     NULL);
  
  g_object_set(video_src, "device", uri, NULL);
  
  video_src_pad = gst_element_get_static_pad(video_src, "src");
  gst_element_add_pad(*source, gst_ghost_pad_new("video_src", video_src_pad));
  gst_object_unref(GST_OBJECT(video_src_pad));

  if(params) dkc_params_unref(params);
  
  return OK;
  
}
