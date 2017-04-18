#include <libdkc/dkc_app.h>
#include <libdkc/dkc_backend.h>
#include <libdkc/dkc_source.h>
#include <libdkc/dkc_scene.h>
#include <libdkc/dkc_sink.h>
#include <stdlib.h>
#include <stdio.h>

DkcApp* dkc_app_create(const char* bkn_type, DkcParams* params) {

  DkcApp* app = malloc(sizeof(DkcApp));

  app->backend = dkc_backend_create(bkn_type, params);
  if(!app->backend){ free(app); return NULL; }
  
  app->src_mgr = dkc_sourcemgr_create(
                     (DkcSourceCBs) {app->backend->ctx,
                                     app->backend->create_source,
                                     app->backend->delete_source});
  if(!app->src_mgr) { free(app); return NULL; }
  
  app->scn_mgr = dkc_scenemgr_create(
                     (DkcSceneCBs) {app->backend->ctx,
                                    app->backend->create_scene,
                                    app->backend->delete_scene,
                                    app->backend->wrap_source,
                                    app->backend->unwrap_source});
  if(!app->scn_mgr) { free(app); return NULL; }
  
  app->snk_mgr = dkc_sinkmgr_create(
                     (DkcSinkCBs) {app->backend->ctx,
                                   app->backend->create_sink,
                                   app->backend->delete_sink});
  if(!app->snk_mgr) { free(app); return NULL; }
  
  return app;
  
}

dkc_rc dkc_app_delete(DkcApp* app) {

  if(!dkc_sinkmgr_delete(app->snk_mgr)) return ERROR;
  if(!dkc_scenemgr_delete(app->scn_mgr)) return ERROR;
  if(!dkc_sourcemgr_delete(app->src_mgr)) return ERROR;
  if(!dkc_backend_delete(app->backend)) return ERROR;
  
  free(app);

  return OK;
  
}

dkc_rc dkc_app_start(DkcApp* app) {

}

dkc_rc dkc_app_stop(DkcApp* app){

}
