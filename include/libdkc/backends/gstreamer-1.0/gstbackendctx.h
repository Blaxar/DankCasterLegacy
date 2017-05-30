#ifndef GSTBACKENDCTX_H
#define GSTBACKENDCTX_H

#include <libdkc/dkc_params.h>
#include <gst/gst.h>

#define NB_INPUTS 255
#define NB_SCENES 255
#define NB_OUTPUTS 255

typedef struct {

  GstElement* sources[NB_INPUTS]; //wrapped sources
  uint8_t nb_sources;
  GstElement* video_mixer;
  GstElement* audio_mixer;
  
} GstScene;

typedef struct {

  GstElement* pipeline;
  GstElement* inputs[NB_INPUTS];
  uint8_t nb_inputs;
  GstScene* scenes[NB_SCENES];
  uint8_t nb_scenes;
  GstElement* outputs[NB_OUTPUTS];
  uint8_t nb_outputs;
  GstElement* video_selector;
  GstElement* video_tee;
  GstElement* audio_selector;
  GstElement* audio_tee;
  GstPad* v_caps;
  GstPad* a_caps;
  
} GstBackendCtx;

#endif //GSTBACKENDCTX_H
