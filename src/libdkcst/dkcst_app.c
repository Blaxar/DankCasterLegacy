#include <libdkcst/dkcst_app.h>
#include <stdlib.h>

DkCst_rc DkCst_create_app(DkCstApp** app) {

	*app = malloc(sizeof(DkCstApp));
	if (*app == NULL) return ERROR;

	(*app)->backend = NULL;
	(*app)->src_mgr = NULL;
    (*app)->scn_mgr = NULL;
    (*app)->snk_mgr = NULL;
	
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
