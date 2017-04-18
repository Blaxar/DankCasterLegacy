#include <stdlib.h>
#include <stdio.h>
#include <libdkc/dkc.h>
#include <libdkc/dkc_source.h>
#include <libdkc/dkc_scene.h>
#include <libdkc/dkc_sink.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){
  
  printf("Initializing Dkc\n");
  dkc_init();
  printf("Initialized Dkc\n");

  DkcApp* app;
  
  if(! (app = dkc_app_create("gst", dkc_params_wrap("width", INT, 640,
                                                    "height", INT, 360,
                                                    "framerate", FRACTION, 25, 1,
                                                    "format", STRING, "NV12",
                                                    "audiorate", INT, 48000,
                                                    "channels", INT, 2,
                                                    "audioformat", STRING, "S16LE",
                                                    NULL)))) printf("Failed to create app.\n");

  DkcSource* dummy_src = dkc_app_source_create(app, DUMMY_SRC, "/some/uri", "dummy_src",
                                               dkc_params_wrap("width", INT, 640,
                                                               "height", INT, 360,
                                                               "framerate", FRACTION, 25, 1,
                                                               "format", STRING, "NV12",
                                                               "audiorate", INT, 48000,
                                                               "channels", INT, 2,
                                                               "audioformat", STRING, "S16LE",
                                                               NULL));

  DkcScene* scene = dkc_app_scene_create(app);
  DkcWrappedSource* wrpd_src = dkc_source_wrap(scene, dummy_src);
  
  DkcSink* window_sink = dkc_app_sink_create(app, DUMMY_SNK, "test window", "test_window",
                                             dkc_params_wrap("width", INT, 640,
                                                             "height", INT, 360,
                                                             "framerate", FRACTION, 25, 1,
                                                             "format", STRING, "NV12",
                                                             "audiorate", INT, 48000,
                                                             "channels", INT, 2,
                                                             "audioformat", STRING, "S16LE",
                                                             NULL));



  
  printf("Terminating Dkc\n");
  dkc_terminate();
  printf("Terminated Dkc\n");
  
  return 0; 
  
}
