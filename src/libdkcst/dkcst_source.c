#include <libdkcst/dkcst_source.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGISTER_SOURCE(x)                   \
    {                                        \
        dkcst_register_source("##x##");      \
    }

dkcst_source_handler dkcst_registered_sources[NB_SOURCE_TYPES];
uint8_t dkcst_nb_registered_sources = 0;

void dkcst_create_source_mgr(dkcst_source_mgr** src_mgr) {

    (*src_mgr) = malloc(sizeof(dkcst_source_mgr));
	(*src_mgr)->nb_sources = 0;
	
}

void dkcst_delete_source_mgr(dkcst_source_mgr**  src_mgr) {

	free(*src_mgr);
	
}

const dkcst_source* dkcst_create_source(dkcst_source_mgr* src_mgr, const char* type, const void* params) {

	for(int i=0; i<dkcst_nb_registered_sources; i++) {
		if(strcmp(dkcst_registered_sources[i].dkcst_source_get_type().name, type) == 0) {
			src_mgr->sources[src_mgr->nb_sources] = malloc(sizeof(dkcst_source));
			src_mgr->sources[src_mgr->nb_sources]->id = src_mgr->nb_sources;
			src_mgr->sources[src_mgr->nb_sources]->type             = dkcst_registered_sources[i].dkcst_source_get_type().type;
			src_mgr->sources[src_mgr->nb_sources]->name             = dkcst_registered_sources[i].dkcst_source_get_type().name;
			src_mgr->sources[src_mgr->nb_sources]->get_width        = dkcst_registered_sources[i].dkcst_source_get_type().get_width;
			src_mgr->sources[src_mgr->nb_sources]->get_height       = dkcst_registered_sources[i].dkcst_source_get_type().get_height;
			src_mgr->sources[src_mgr->nb_sources]->get_pix_fmt      = dkcst_registered_sources[i].dkcst_source_get_type().get_pix_fmt;
			src_mgr->sources[src_mgr->nb_sources]->get_fps          = dkcst_registered_sources[i].dkcst_source_get_type().get_fps;
			src_mgr->sources[src_mgr->nb_sources]->get_sample_rate  = dkcst_registered_sources[i].dkcst_source_get_type().get_sample_rate;
			src_mgr->sources[src_mgr->nb_sources]->get_nb_channels  = dkcst_registered_sources[i].dkcst_source_get_type().get_nb_channels;
			dkcst_registered_sources[src_mgr->nb_sources].dkcst_source_create(src_mgr->sources[src_mgr->nb_sources], params);
			return src_mgr->sources[src_mgr->nb_sources];
		}
	}

    return NULL;
	
}


uint8_t dkcst_delete_source(dkcst_source_mgr* src_mgr, const dkcst_source* src) {

	
	
}


void dkcst_register_source_type(const char* src_name) {

   void *tmp_module;
   uint8_t (*tmp_source_create)(dkcst_source* src, void* params);
   uint8_t (*tmp_source_delete)(dkcst_source* src);
   dkcst_source_type (*tmp_source_get_type)(void);
   int rc = 0;

   char path[80];
   sprintf(path, "libdkcst_%s_source.so", src_name);
   
   tmp_module = dlopen(path, RTLD_NOW);
   if (tmp_module == NULL){
      printf("Could not open module %s", path);
      return;
   }
   
   tmp_source_create = dlsym(tmp_module, "dkcst_source_create");
   if (tmp_source_create == NULL) {
      dlclose(tmp_module);
      printf("Could not find symbol dkcst_source_create in %s", path);
	  return;
   }

   tmp_source_delete = dlsym(tmp_module, "dkcst_source_delete");
   if (tmp_source_delete == NULL) {
      dlclose(tmp_module);
      printf("Could not find symbol dkcst_source_delete in %s", path);
	  return;
   }

   tmp_source_get_type = dlsym(tmp_module, "dkcst_source_get_type");
   if (tmp_source_get_type == NULL) {
      dlclose(tmp_module);
      printf("Could not find symbol dkcst_source_get_type in %s", path);
	  return;
   }
   
   sprintf(dkcst_registered_sources[dkcst_nb_registered_sources].name, src_name);
   dkcst_registered_sources[dkcst_nb_registered_sources].module = tmp_module;
   dkcst_registered_sources[dkcst_nb_registered_sources].dkcst_source_create = tmp_source_create;
   dkcst_registered_sources[dkcst_nb_registered_sources].dkcst_source_delete = tmp_source_delete;
   dkcst_registered_sources[dkcst_nb_registered_sources].dkcst_source_get_type = tmp_source_get_type;
   dkcst_nb_registered_sources++;
   return;

}

void dkcst_ready_all_source_types(void) {

	for(int i=0; i<NB_SOURCE_TYPES; i++) {

	    memset(dkcst_registered_sources[i].name, 0, 32);
		dkcst_registered_sources[i].module = NULL;
	    dkcst_registered_sources[i].dkcst_source_create = NULL;
	    dkcst_registered_sources[i].dkcst_source_delete = NULL;
		
	}
	
}

void dkcst_register_all_source_types(void) {

    dkcst_register_source_type("dummy");
	
}

void dkcst_unregister_all_source_types(void) {

	for(int i=0; i<NB_SOURCE_TYPES; i++) {

		if(dkcst_registered_sources[i].module != NULL) dlclose(dkcst_registered_sources[i].module);
	    memset(dkcst_registered_sources[i].name, 0, 32);

	}

	dkcst_nb_registered_sources = 0;

}

void dkcst_init(void) {

	dkcst_ready_all_source_types();
	dkcst_register_source_type("dummy");
	
}

void dkcst_terminate(void) {
	dkcst_unregister_all_source_types();
}
