#include <libdkcst/dkcst_backend.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

DkCstBackendHandler DkCst_registered_backends[NB_BACKEND_TYPES];
uint8_t DkCst_nb_registered_backends = 0;

DkCst_rc DkCst_create_backend(DkCstApp* app, const char* backend, DkCstParams* params, DkCstBackend** bkn) {
	
	for(int i=0; i<DkCst_nb_registered_backends; i++) {
		if(strcmp(DkCst_registered_backends[i].name, backend) == 0) {
			
		    app->backend = malloc(sizeof(DkCstBackend));
			(*bkn) = app->backend;
			return OK;
					
		}
	}
	(*bkn) = NULL;
    return ERROR;
	
}


DkCst_rc DkCst_delete_backend(DkCstBackend** bkn) {

	pthread_mutex_destroy(&(*bkn)->lock);
	DkCstApp* app = (*bkn)->app;
	uint8_t type_id = (*bkn)->type_id;
    DkCst_registered_backends[type_id].DkCst_backend_delete(*bkn);
	app->backend = NULL;
	
	return OK;
	
}


DkCst_rc DkCst_register_backend(const char* backend) {

   void *tmp_module;
   DkCst_rc (*tmp_backend_create)(DkCstBackend* bkn, DkCstParams* params);
   DkCst_rc (*tmp_backend_delete)(DkCstBackend* bkn);
   int rc = 0;

   char path[80];
   sprintf(path, "libDkCst_%s_backend.so", backend);
   
   tmp_module = dlopen(path, RTLD_NOW);
   if (tmp_module == NULL){
      printf("Could not open module %s\n", path);
      return ERROR;
   }
   
   tmp_backend_create = dlsym(tmp_module, "DkCst_backend_create");
   if (tmp_backend_create == NULL) {
      dlclose(tmp_module);
      printf("Could not find symbol DkCst_backend_create in %s\n", path);
	  return ERROR;
   }

   tmp_backend_delete = dlsym(tmp_module, "DkCst_backend_delete");
   if (tmp_backend_delete == NULL) {
      dlclose(tmp_module);
      printf("Could not find symbol DkCst_backend_delete in %s\n", path);
	  return ERROR;
   }
   
   sprintf(DkCst_registered_backends[DkCst_nb_registered_backends].name, backend);
   DkCst_registered_backends[DkCst_nb_registered_backends].module = tmp_module;
   DkCst_registered_backends[DkCst_nb_registered_backends].DkCst_backend_create = tmp_backend_create;
   DkCst_registered_backends[DkCst_nb_registered_backends].DkCst_backend_delete = tmp_backend_delete;
   DkCst_nb_registered_backends++;
   return OK;

}

DkCst_rc DkCst_ready_all_backends(void) {

	for(int i=0; i<NB_BACKEND_TYPES; i++) {

	    memset(DkCst_registered_backends[i].name, 0, 32);
		DkCst_registered_backends[i].module = NULL;
		
	}
	return OK;
	
}

DkCst_rc DkCst_register_all_backends(void) {

    DkCst_register_backend("dummy");
	return OK;
	
}

DkCst_rc DkCst_unregister_all_backends(void) {

	for(int i=0; i<NB_BACKEND_TYPES; i++) {

		if(DkCst_registered_backends[i].module != NULL) dlclose(DkCst_registered_backends[i].module);
	    memset(DkCst_registered_backends[i].name, 0, BACKEND_NAME_LENGTH);

	}

	DkCst_nb_registered_backends = 0;
	return OK;

}
