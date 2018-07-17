#ifndef DKC_GST_SOURCES_H
#define DKC_GST_SOURCES_H

#include <libdkc/dkc_params.h>
#include <gst/gst.h>

gboolean gstbkn_create_source_dummy(GstElement** source, DkcParams* params);

gboolean gstbkn_create_source_v4l2(GstElement** source, const char* uri, DkcParams* params);

#endif //DKC_GST_SOURCES_H
