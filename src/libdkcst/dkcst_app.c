#include <libdkcst/dkcst_app.h>
#include <libdkcst/dkcst_backend.h>
#include <libdkcst/dkcst_source.h>
#include <libdkcst/dkcst_scene.h>
#include <libdkcst/dkcst_sink.h>
#include <stdlib.h>

DkCst_rc DkCst_create_app(DkCstApp** app) {

  *app = malloc(sizeof(DkCstApp));
  if (*app == NULL) return ERROR;

  if(!DkCst_rc_ok(DkCst_create_backend(&(*app)->backend, "gst", NULL))) return ERROR;
  if(!DkCst_rc_ok(DkCst_create_source_mgr(&(*app)->src_mgr,
                      (DkCstSourceCBs) {(*app)->backend->create_source,
                                        (*app)->backend->delete_source}))) return ERROR;
    if(!DkCst_rc_ok(DkCst_create_scene_mgr(&(*app)->scn_mgr,
                       (DkCstSceneCBs) {(*app)->backend->create_scene,
                                        (*app)->backend->delete_scene,
                                        (*app)->backend->wrap_source,
                                        (*app)->backend->unwrap_source}))) return ERROR;
    if(!DkCst_rc_ok(DkCst_create_sink_mgr(&(*app)->snk_mgr,
                              (DkCstSinkCBs) {(*app)->backend->create_sink,
                                              (*app)->backend->delete_sink}))) return ERROR;
  
  return OK;
  
}

DkCst_rc DkCst_delete_app(DkCstApp** app) {

  if(!DkCst_rc_ok(DkCst_delete_sink_mgr(&(*app)->snk_mgr))) return ERROR;
  if(!DkCst_rc_ok(DkCst_delete_scene_mgr(&(*app)->scn_mgr))) return ERROR;
  if(!DkCst_rc_ok(DkCst_delete_source_mgr(&(*app)->src_mgr))) return ERROR;
  if(!DkCst_rc_ok(DkCst_delete_backend(&(*app)->backend))) return ERROR;
  
  free(*app);
  *app = NULL;

  return OK;
  
}

DkCst_rc DkCst_start_app(DkCstApp* app) {

}

DkCst_rc DkCst_stop_app(DkCstApp* app){

}
