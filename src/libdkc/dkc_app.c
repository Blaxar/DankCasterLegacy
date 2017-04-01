#include <libdkc/dkc_app.h>
#include <libdkc/dkc_backend.h>
#include <libdkc/dkc_source.h>
#include <libdkc/dkc_scene.h>
#include <libdkc/dkc_sink.h>
#include <stdlib.h>

DkcApp* dkc_create_app() {

  DkcApp* app = malloc(sizeof(DkcApp));

  if(app->backend = dkc_backend_create("gst", NULL)){ free(app); return NULL;}
  if(app->src_mgr = dkc_sourcemgr_create(
                       (DkcSourceCBs) {app->backend->ctx,
                                       app->backend->create_source,
                                       app->backend->delete_source})){ free(app); return NULL;}
  if(app->scn_mgr = dkc_scenemgr_create(
                       (DkcSceneCBs) {app->backend->ctx,
                                      app->backend->create_scene,
                                      app->backend->delete_scene,
                                      app->backend->wrap_source,
                                      app->backend->unwrap_source})){ free(app); return NULL;}
  if(app->snk_mgr = dkc_sinkmgr_create(
                       (DkcSinkCBs) {app->backend->ctx,
                                     app->backend->create_sink,
                                     app->backend->delete_sink})){ free(app); return NULL;}
  
  return app;
  
}

dkc_rc dkc_delete_app(DkcApp* app) {

  if(!dkc_sinkmgr_delete(app->snk_mgr)) return ERROR;
  if(!dkc_scenemgr_delete(app->scn_mgr)) return ERROR;
  if(!dkc_sourcemgr_delete(app->src_mgr)) return ERROR;
  if(!dkc_backend_delete(app->backend)) return ERROR;
  
  free(app);

  return OK;
  
}

dkc_rc dkc_start_app(DkcApp* app) {

}

dkc_rc dkc_stop_app(DkcApp* app){

}
