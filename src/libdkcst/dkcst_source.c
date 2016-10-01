#include <libdkcst/dkcst_source.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGISTER_SOURCE(x)                   \
    {                                        \
        DkCst_register_source("##x##");      \
    }

DkCstSourceHandler DkCst_registered_sources[NB_SOURCE_TYPES];
uint8_t DkCst_nb_registered_sources = 0;

DkCst_rc DkCst_create_source_mgr(DkCstSourceMgr** src_mgr) {

    (*src_mgr) = malloc(sizeof(DkCstSourceMgr));
	if(pthread_mutex_init(&(*src_mgr)->lock, NULL) != 0){ free(*src_mgr); return ERROR; }
    for(int i=0; i<NB_SOURCES; i++) {
		(*src_mgr)->sources[i] = NULL;
	}
	(*src_mgr)->nb_sources = 0;
	return OK;
	
}

DkCst_rc DkCst_delete_source_mgr(DkCstSourceMgr**  src_mgr) {

	pthread_mutex_destroy(&(*src_mgr)->lock);
	for(int i=0; i<NB_SOURCES; i++) {
		if((*src_mgr)->sources[i] != NULL) return ERROR;
	}
	free(*src_mgr);
	return OK;
	
}

DkCst_rc DkCst_create_source(DkCstSourceMgr* src_mgr, const char* type, DkCstParams* params, DkCstSource** src) {

	pthread_mutex_lock(&src_mgr->lock);
	
	for(int i=0; i<DkCst_nb_registered_sources; i++) {
		if(strcmp(DkCst_registered_sources[i].DkCst_source_get_type().name, type) == 0) {
			for(int j=0; j<NB_SOURCES; j++) {
				if(src_mgr->sources[j] == NULL) {
					src_mgr->sources[j] = malloc(sizeof(DkCstSource));
					if(pthread_mutex_init(&src_mgr->sources[j]->lock, NULL) != 0){
						free(src_mgr->sources[j]);
						src_mgr->sources[j] = NULL;
						pthread_mutex_unlock(&src_mgr->lock);
						return ERROR;
					}
					src_mgr->sources[j]->src_mgr            = src_mgr;
					src_mgr->sources[j]->id                 = j;
					src_mgr->sources[j]->type_id            = i;
					src_mgr->sources[j]->type               = DkCst_registered_sources[i].DkCst_source_get_type().type;
					src_mgr->sources[j]->name               = DkCst_registered_sources[i].DkCst_source_get_type().name;
					src_mgr->sources[j]->get_width          = DkCst_registered_sources[i].DkCst_source_get_type().get_width;
					src_mgr->sources[j]->get_height         = DkCst_registered_sources[i].DkCst_source_get_type().get_height;
					src_mgr->sources[j]->get_pix_fmt        = DkCst_registered_sources[i].DkCst_source_get_type().get_pix_fmt;
					src_mgr->sources[j]->get_fps            = DkCst_registered_sources[i].DkCst_source_get_type().get_fps;
					src_mgr->sources[j]->get_sample_rate    = DkCst_registered_sources[i].DkCst_source_get_type().get_sample_rate;
					src_mgr->sources[j]->get_nb_channels    = DkCst_registered_sources[i].DkCst_source_get_type().get_nb_channels;
					src_mgr->sources[j]->alloc_video_buffer = DkCst_registered_sources[i].DkCst_source_get_type().alloc_video_buffer;
					src_mgr->sources[j]->free_video_buffer  = DkCst_registered_sources[i].DkCst_source_get_type().free_video_buffer;
					src_mgr->sources[j]->copy_video_data    = DkCst_registered_sources[i].DkCst_source_get_type().copy_video_data;
					src_mgr->sources[j]->alloc_audio_buffer = DkCst_registered_sources[i].DkCst_source_get_type().alloc_audio_buffer;
					src_mgr->sources[j]->free_audio_buffer  = DkCst_registered_sources[i].DkCst_source_get_type().free_audio_buffer;
					src_mgr->sources[j]->copy_audio_data    = DkCst_registered_sources[i].DkCst_source_get_type().copy_audio_data;
					DkCst_registered_sources[i].DkCst_source_create(src_mgr->sources[j], params);
					src_mgr->nb_sources++;
					(*src) = src_mgr->sources[j];
					pthread_mutex_unlock(&src_mgr->lock);
					return OK;
				}
			}
			(*src) = NULL;
			pthread_mutex_unlock(&src_mgr->lock);
			return ERROR;
		}
	}
	(*src) = NULL;
	pthread_mutex_unlock(&src_mgr->lock);
    return ERROR;
	
}


DkCst_rc DkCst_delete_source(DkCstSource** src) {

	pthread_mutex_lock(&(*src)->src_mgr->lock);
	
	pthread_mutex_destroy(&(*src)->lock);
    DkCstSourceMgr* src_mgr = (*src)->src_mgr;
	uint8_t id = (*src)->id;
	uint8_t type_id = (*src)->type_id;
    DkCst_registered_sources[type_id].DkCst_source_delete(src_mgr->sources[id]);
	free(src_mgr->sources[id]);
	src_mgr->sources[id] = NULL;
	src_mgr->nb_sources--;

	pthread_mutex_unlock(&(*src)->src_mgr->lock);
	
	return OK;
	
}


DkCst_rc DkCst_register_source_type(const char* src_name) {

   void *tmp_module;
   DkCst_rc (*tmp_source_create)(DkCstSource* src, DkCstParams* params);
   DkCst_rc (*tmp_source_delete)(DkCstSource* src);
   DkCstSourceType (*tmp_source_get_type)(void);
   int rc = 0;

   char path[80];
   sprintf(path, "libDkCst_%s_source.so", src_name);
   
   tmp_module = dlopen(path, RTLD_NOW);
   if (tmp_module == NULL){
      printf("Could not open module %s\n", path);
      return ERROR;
   }
   
   tmp_source_create = dlsym(tmp_module, "DkCst_source_create");
   if (tmp_source_create == NULL) {
      dlclose(tmp_module);
      printf("Could not find symbol DkCst_source_create in %s\n", path);
	  return ERROR;
   }

   tmp_source_delete = dlsym(tmp_module, "DkCst_source_delete");
   if (tmp_source_delete == NULL) {
      dlclose(tmp_module);
      printf("Could not find symbol DkCst_source_delete in %s\n", path);
	  return ERROR;
   }

   tmp_source_get_type = dlsym(tmp_module, "DkCst_source_get_type");
   if (tmp_source_get_type == NULL) {
      dlclose(tmp_module);
      printf("Could not find symbol DkCst_source_get_type in %s\n", path);
	  return ERROR;
   }
   
   sprintf(DkCst_registered_sources[DkCst_nb_registered_sources].name, src_name);
   DkCst_registered_sources[DkCst_nb_registered_sources].module = tmp_module;
   DkCst_registered_sources[DkCst_nb_registered_sources].DkCst_source_create = tmp_source_create;
   DkCst_registered_sources[DkCst_nb_registered_sources].DkCst_source_delete = tmp_source_delete;
   DkCst_registered_sources[DkCst_nb_registered_sources].DkCst_source_get_type = tmp_source_get_type;
   DkCst_nb_registered_sources++;
   return OK;

}

DkCst_rc DkCst_ready_all_source_types(void) {

	for(int i=0; i<NB_SOURCE_TYPES; i++) {

	    memset(DkCst_registered_sources[i].name, 0, 32);
		DkCst_registered_sources[i].module = NULL;
	    DkCst_registered_sources[i].DkCst_source_create = NULL;
	    DkCst_registered_sources[i].DkCst_source_delete = NULL;
		
	}
	return OK;
	
}

DkCst_rc DkCst_register_all_source_types(void) {

    DkCst_register_source_type("dummy");
	return OK;
	
}

DkCst_rc DkCst_unregister_all_source_types(void) {

	for(int i=0; i<NB_SOURCE_TYPES; i++) {

		if(DkCst_registered_sources[i].module != NULL) dlclose(DkCst_registered_sources[i].module);
	    memset(DkCst_registered_sources[i].name, 0, 32);

	}

	DkCst_nb_registered_sources = 0;
	return OK;

}

DkCst_rc DkCst_init(void) {

	if (!DkCst_rc_ok(DkCst_ready_all_source_types())) return ERROR;
	if (!DkCst_rc_ok(DkCst_register_all_source_types())) return ERROR;
	return OK;
	
}

DkCst_rc DkCst_terminate(void) {
	
	
	return DkCst_unregister_all_source_types();
	
}
