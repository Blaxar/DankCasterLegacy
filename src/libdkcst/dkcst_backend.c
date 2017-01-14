#include <libdkcst/dkcst_backend.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

DkCst_rc DkCst_create_backend(DkCstApp* app, const char* backend, DkCstParams* params, DkCstBackend** bkn) {

	if(app->backend == NULL) {
		
		app->backend = malloc(sizeof(DkCstBackend));
		if(pthread_mutex_init(&((DkCstBackend*)app->backend)->lock, NULL) != 0){ free(app->backend); return ERROR; }
		if(app->backend != NULL && DkCst_rc_ok(((DkCstBackend*)app->backend)->init(&((DkCstBackend*)app->backend)->ctx))){
			(*bkn) = app->backend;
			return OK;
		}

	}	
    return ERROR;
	
}


DkCst_rc DkCst_delete_backend(DkCstBackend** bkn) {
	
	DkCstApp* app = (*bkn)->app;
	pthread_mutex_destroy(&((DkCstBackend*)app->backend)->lock);

	if(DkCst_rc_ok(((DkCstBackend*)app->backend)->uninit(&((DkCstBackend*)app->backend)->ctx))) {
		free(app->backend);
		app->backend = NULL;
	    *bkn = NULL;
	    return OK;
	}

	return ERROR;
	
}
