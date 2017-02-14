#ifndef GSTBACKENDCTX_H
#define GSTBACKENDCTX_H

#include <libdkc/dkc_param.h>
#include <gst/gst.h>

#define NB_INPUTS 255
#define NB_OUTPUTS 255

typedef struct {

  GstElement* pipeline;
  GstElement* scenemanager;
  GstElement* audiomixer;
  GstElement* inputs[NB_INPUTS];
  uint8_t nb_inputs;
  GstElement* outputs[NB_OUTPUTS];
  uint8_t nb_outputs;
  
} GstBackendCtx;

// Bin handling

dkc_rc gstbkn_make_scenemanager_bin(GstElement** bin);
dkc_rc gstbkn_make_audiomixer_bin(GstElement** bin);

#endif //GSTBACKENDCTX_H
