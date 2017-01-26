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
	if(!DkCst_rc_ok(DkCst_create_source_mgr(&(*app)->src_mgr))) return ERROR;
    if(!DkCst_rc_ok(DkCst_create_scene_mgr(&(*app)->scn_mgr))) return ERROR;
    if(!DkCst_rc_ok(DkCst_create_sink_mgr(&(*app)->snk_mgr))) return ERROR;
	
	return OK;
	
}

DkCst_rc DkCst_delete_app(DkCstApp** app) {

	free(*app);
	*app = NULL;

	return OK;
	
}

DkCst_rc DkCst_start_app(DkCstApp* app) {

}

DkCst_rc DkCst_stop_app(DkCstApp* app){

}
