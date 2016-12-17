#include <libdkcst/dkcst_source.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DkCst_rc DkCst_create_source_mgr(DkCstApp* app, DkCstSourceMgr** src_mgr) {

    (*src_mgr) = malloc(sizeof(DkCstSourceMgr));
	if(pthread_mutex_init(&(*src_mgr)->lock, NULL) != 0){ free(*src_mgr); return ERROR; }
	app->src_mgr = *src_mgr;
	(*src_mgr)->app = app;
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
	(*src_mgr)->app->src_mgr = NULL;
	free(*src_mgr);
	return OK;
	
}

DkCst_rc DkCst_create_source(DkCstSourceMgr* src_mgr, const char* type, DkCstParams* params, DkCstSource** src, const char* name) {

	pthread_mutex_lock(&src_mgr->lock);
	
    for(int j=0; j<NB_SOURCES; j++) {
	    if(src_mgr->sources[j] == NULL) {
		    src_mgr->sources[j] = malloc(sizeof(DkCstSource));
		    if(pthread_mutex_init(&src_mgr->sources[j]->lock, NULL) != 0){
			    free(src_mgr->sources[j]);
			    src_mgr->sources[j] = NULL;
			    pthread_mutex_unlock(&src_mgr->lock);
			    return ERROR;
			}
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


DkCst_rc DkCst_delete_source(DkCstSource** src) {

	pthread_mutex_lock(&(*src)->src_mgr->lock);
	
	pthread_mutex_destroy(&(*src)->lock);
    DkCstSourceMgr* src_mgr = (*src)->src_mgr;
	uint8_t id = (*src)->id;
	free(src_mgr->sources[id]);
	src_mgr->sources[id] = NULL;
	src_mgr->nb_sources--;

	pthread_mutex_unlock(&(*src)->src_mgr->lock);
	
	return OK;
	
}
