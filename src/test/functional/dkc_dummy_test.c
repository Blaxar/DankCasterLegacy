#include <stdlib.h>
#include <stdio.h>
#include <libdkc/dkc.h>
#include <libdkc/dkc_source.h>
#include <libdkc/dkc_scene.h>
#include <libdkc/dkc_sink.h>
#include <gst/gst.h>

#include <assert.h>

int main(int argc, char* argv[]){

  GMainLoop* loop = g_main_loop_new (NULL, FALSE);
  
  printf("Initializing Dkc\n");
  dkc_init();
  printf("Initialized Dkc\n");

  DkcApp* app;
  
  if(! (app = dkc_app_create("gst", dkc_params_wrap("width", INT, 640,
                                                    "height", INT, 360,
                                                    "framerate", FRACTION, 25, 1,
                                                    "videoformat", STRING, "RGB32",
                                                    "rate", INT, 48000,
                                                    "channels", INT, 2,
                                                    "audioformat", STRING, "S16LE",
                                                    NULL)))) printf("Failed to create app.\n");

  assert(app != NULL);
  printf("Created app.\n");

  DkcSource* dummy_src = dkc_app_source_create(app, DUMMY_SRC, "/some/uri", "dummy_src",
                                               dkc_params_wrap("width", INT, 640,
                                                               "height", INT, 360,
                                                               "framerate", FRACTION, 25, 1,
                                                               "format", STRING, "NV12",
                                                               "audiorate", INT, 48000,
                                                               "channels", INT, 2,
                                                               "audioformat", STRING, "S16LE",
                                                               NULL));
  assert(dummy_src != NULL);
  printf("Created source.\n");

  DkcSink* window_sink = dkc_app_sink_create(app, DUMMY_SNK, "test window", "test_window",
                                             dkc_params_wrap("width", INT, 640,
                                                             "height", INT, 360,
                                                             "framerate", FRACTION, 25, 1,
                                                             "format", STRING, "NV12",
                                                             "audiorate", INT, 48000,
                                                             "channels", INT, 2,
                                                             "audioformat", STRING, "S16LE",
                                                             NULL));
  assert(window_sink != NULL);
  printf("Created sink.\n");
  
  DkcScene* scene = dkc_app_scene_create(app);
  assert(scene != NULL);
  printf("Created scene.\n");
  
  DkcWrappedSource* wrpd_src = dkc_source_wrap(scene, dummy_src);
  assert(wrpd_src != NULL);
  printf("Wrapped source.\n");
  
  assert(dkc_app_start(app) == OK);
  printf("Started app.\n");

  g_main_loop_run (loop);
  
  printf("Terminating Dkc\n");
  dkc_terminate();
  printf("Terminated Dkc\n");
  
  return 0; 
  
}
