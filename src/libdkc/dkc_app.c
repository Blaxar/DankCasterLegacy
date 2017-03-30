#include <libdkc/dkc_app.h>
#include <libdkc/dkc_backend.h>
#include <libdkc/dkc_source.h>
#include <libdkc/dkc_scene.h>
#include <libdkc/dkc_sink.h>
#include <stdlib.h>

DkcApp* dkc_create_app() {

  DkcApp* app = malloc(sizeof(DkcApp));

  if(app->backend = dkc_create_backend("gst", NULL)){ free(app); return NULL;}
  if(app->src_mgr = dkc_create_source_mgr(
                       (DkcSourceCBs) {app->backend->ctx,
                                       app->backend->create_source,
                                       app->backend->delete_source})){ free(app); return NULL;}
  if(app->scn_mgr = dkc_create_scene_mgr(
                       (DkcSceneCBs) {app->backend->ctx,
                                      app->backend->create_scene,
                                      app->backend->delete_scene,
                                      app->backend->wrap_source,
                                      app->backend->unwrap_source})){ free(app); return NULL;}
  if(app->snk_mgr = dkc_create_sink_mgr(
                       (DkcSinkCBs) {app->backend->ctx,
                                     app->backend->create_sink,
                                     app->backend->delete_sink})){ free(app); return NULL;}
  
  return app;
  
}

dkc_rc dkc_delete_app(DkcApp* app) {

  if(!dkc_delete_sink_mgr(app->snk_mgr)) return ERROR;
  if(!dkc_delete_scene_mgr(app->scn_mgr)) return ERROR;
  if(!dkc_delete_source_mgr(app->src_mgr)) return ERROR;
  if(!dkc_delete_backend(app->backend)) return ERROR;
  
  free(app);

  return OK;
  
}

dkc_rc dkc_start_app(DkcApp* app) {

}

dkc_rc dkc_stop_app(DkcApp* app){

}
