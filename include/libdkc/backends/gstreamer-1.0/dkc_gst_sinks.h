#ifndef DKC_GST_SINKS_H
#define DKC_GST_SINKS_H

#include <libdkc/dkc_params.h>
#include <gst/gst.h>

dkc_rc gstbkn_create_sink_dummy(GstElement** sink, DkcParams* params);

#endif //DKC_GST_SINKS_H
