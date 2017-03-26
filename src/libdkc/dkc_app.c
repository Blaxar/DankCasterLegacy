#include <libdkc/dkc_app.h>
#include <libdkc/dkc_backend.h>
#include <libdkc/dkc_source.h>
#include <libdkc/dkc_scene.h>
#include <libdkc/dkc_sink.h>
#include <stdlib.h>

dkc_rc dkc_create_app(DkcApp** app) {

  *app = malloc(sizeof(DkcApp));
  if (*app == NULL) return ERROR;

  if(!dkc_rc_ok(dkc_create_backend(&(*app)->backend, "gst", NULL))) return ERROR;
  if(!dkc_rc_ok(dkc_create_source_mgr(&(*app)->src_mgr,
                       (DkcSourceCBs) {(*app)->backend->ctx,
                                       (*app)->backend->create_source,
                                       (*app)->backend->delete_source}))) return ERROR;
  if(!dkc_rc_ok(dkc_create_scene_mgr(&(*app)->scn_mgr,
                       (DkcSceneCBs) {(*app)->backend->ctx,
                                      (*app)->backend->create_scene,
                                      (*app)->backend->delete_scene,
                                      (*app)->backend->wrap_source,
                                      (*app)->backend->unwrap_source}))) return ERROR;
  if(!dkc_rc_ok(dkc_create_sink_mgr(&(*app)->snk_mgr,
                       (DkcSinkCBs) {(*app)->backend->ctx,
                                     (*app)->backend->create_sink,
                                     (*app)->backend->delete_sink}))) return ERROR;
  
  return OK;
  
}

dkc_rc dkc_delete_app(DkcApp** app) {

  if(!dkc_rc_ok(dkc_delete_sink_mgr(&(*app)->snk_mgr))) return ERROR;
  if(!dkc_rc_ok(dkc_delete_scene_mgr(&(*app)->scn_mgr))) return ERROR;
  if(!dkc_rc_ok(dkc_delete_source_mgr(&(*app)->src_mgr))) return ERROR;
  if(!dkc_rc_ok(dkc_delete_backend(&(*app)->backend))) return ERROR;
  
  free(*app);
  *app = NULL;

  return OK;
  
}

dkc_rc dkc_start_app(DkcApp* app) {

}

dkc_rc dkc_stop_app(DkcApp* app){

}
