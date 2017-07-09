#include <stdlib.h>
#include <stdio.h>
#include <libdkc/dkc.h>
#include <libdkc/dkc_source.h>
#include <libdkc/dkc_scene.h>
#include <libdkc/dkc_sink.h>
#include <gst/gst.h>

int main(int argc, char* argv[]){
  
  printf("Initializing Dkc\n");
  dkc_init();
  printf("Initialized Dkc\n");

  GMainLoop* loop = g_main_loop_new (NULL, FALSE);
  
  DkcApp* app;
  
  if(! (app = dkc_app_create("gst", NULL, "width", INT, 640*2,
                                          "height", INT, 360*2,
                                          "framerate", FRACTION, 25, 1,
                                          "videoformat", STRING, "NV12",
                                          "rate", INT, 48000,
                                          "channels", INT, 2,
                                          "audioformat", STRING, "S16LE",
                                          NULL))) printf("Failed to create app.\n");

  if (!app) {
      fprintf(stderr, "Failed to create app.\n");
      return 1;
  }
  printf("Created app.\n");
    
  DkcSource* dummy_src = dkc_app_source_create(app, DUMMY_SRC, "/dev/video0", "dummy",
                                               "width", INT, 640,
                                               "height", INT, 360,
                                               "framerate", FRACTION, 25, 1,
                                               "format", STRING, "NV12",
                                               "audiorate", INT, 48000,
                                               "channels", INT, 2,
                                               "audioformat", STRING, "S16LE",
                                               NULL);
  if (!dummy_src) {
      fprintf(stderr, "Failed to create source.\n");
      return 1;
  }
  printf("Created source.\n");

  DkcSource* v4l2_src = dkc_app_source_create(app, VIDEO_DEV_SRC, "/dev/video0", "webcam",
                                              "width", INT, 752,
                                              "height", INT, 416,
                                              "framerate", FRACTION, 25, 1,
                                              "format", STRING, "YUY2",
                                              NULL);
  if (!v4l2_src) {
      fprintf(stderr, "Failed to create source.\n");
      return 1;
  }
  printf("Created source.\n");

  DkcSink* window_sink = dkc_app_sink_create(app, DUMMY_SNK, "test window", "test_window",
                                                             "width", INT, 640,
                                                             "height", INT, 360,
                                                             "framerate", FRACTION, 25, 1,
                                                             "format", STRING, "NV12",
                                                             "audiorate", INT, 48000,
                                                             "channels", INT, 2,
                                                             "audioformat", STRING, "S16LE",
                                                             NULL);
  if (!window_sink) {
      fprintf(stderr, "Failed to create sink.\n");
      return 1;
  }
  printf("Created sink.\n");
  
  DkcScene* scene = dkc_app_scene_create(app);
  if (!scene) {
      fprintf(stderr, "Failed to create scene.\n");
      return 1;
  }
  printf("Created scene.\n");
  
  DkcWrappedSource* wrpd_dummy_src = dkc_source_wrap(scene, dummy_src);
  if (!wrpd_dummy_src) {
      fprintf(stderr, "Failed to wrap source.\n");
      return 1;
  }
  printf("Wrapped source.\n");

  DkcWrappedSource* wrpd_v4l2_src = dkc_source_wrap(scene, v4l2_src);
  if (!wrpd_v4l2_src) {
      fprintf(stderr, "Failed to wrap source.\n");
      return 1;
  }
  printf("Wrapped source.\n");

  if (!dkc_app_start(app, NULL)) {
      fprintf(stderr, "Failed to start app.\n");
      return 1;
  }
  printf("Started app.\n");

  g_main_loop_run(loop);

  if (!dkc_app_stop(app, NULL)) {
      fprintf(stderr, "Failed to stop app.\n");
      return 1;
  }
  printf("Stopped app.\n");
  
  printf("Terminating Dkc\n");
  dkc_terminate();
  printf("Terminated Dkc\n");
  
  return 0; 
  
}
