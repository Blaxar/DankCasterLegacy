#include <libdkcst/dkcst_scene.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Scene handling */


DkCst_rc DkCst_create_scene_mgr(DkCst_scene_mgr* src_mgr, DkCst_scene_mgr** scn_mgr) {

	(*scn_mgr) = malloc(sizeof(DkCst_scene_mgr));
	if(pthread_mutex_init(&(*scn_mgr)->lock, NULL) != 0){ free(*scn_mgr); return ERROR; }
	for(int i=0 ; i<NB_SCENES; i++) {
		(*scn_mgr)->scenes[i] = NULL;
	}
	(*scn_mgr)->nb_scenes=0;
	(*scn_mgr)->src_mgr=src_mgr;
	return OK;
	
}

DkCst_rc DkCst_delete_scene_mgr(DkCst_scene_mgr** scn_mgr) {

    pthread_mutex_destroy(&(*scn_mgr)->lock);
	for(int i=0; i<NB_SCENES; i++) {
		if((*scn_mgr)->scenes[i] != NULL) return ERROR;
	}
	free(*scn_mgr);
	return OK;
	
}

DkCst_rc DkCst_create_scene(DkCst_scene_mgr* scn_mgr, DkCst_scene** scn){

	pthread_mutex_lock(&scn_mgr->lock);
	
	for(int i=0; i<NB_SCENES; i++) {
		if(scn_mgr->scenes[i] == NULL) {
			scn_mgr->scenes[i] = malloc(sizeof(DkCst_scene));
			if(pthread_mutex_init(&scn_mgr->scenes[i]->lock, NULL) != 0){
				free(scn_mgr->scenes[i]);
				scn_mgr->scenes[i] = NULL;
				pthread_mutex_unlock(&scn_mgr->lock);
				return ERROR;
			}
			scn_mgr->scenes[i]->scn_mgr = scn_mgr;
			for(int j=0 ; j<NB_WRP_SOURCES; j++) {
				scn_mgr->scenes[i]->sources[j] = NULL;
			}
			(*scn) = scn_mgr->scenes[i];
			scn_mgr->nb_scenes++;
			pthread_mutex_unlock(&scn_mgr->lock);
			return OK;
		}
	}
	pthread_mutex_unlock(&scn_mgr->lock);
	return ERROR;
	
}

DkCst_rc DkCst_delete_scene(DkCst_scene** scn){

	DkCst_scene_mgr* scn_mgr = (*scn)->scn_mgr;
	pthread_mutex_lock(&scn_mgr->lock);
	
	uint8_t id = (*scn)->id;
    for(int i=0 ; i<NB_WRP_SOURCES; i++) {
		if(scn_mgr->scenes[id]->sources[i] != NULL){ pthread_mutex_unlock(&scn_mgr->lock); return ERROR; }
	}
	pthread_mutex_destroy(&scn_mgr->scenes[id]->lock);
	free(scn_mgr->scenes[id]);
	scn_mgr->scenes[id] = NULL;
	scn_mgr->nb_scenes--;
	pthread_mutex_unlock(&scn_mgr->lock);
	return OK;
	
}

/* Source wrapping */

DkCst_rc DkCst_wrap_source(DkCst_scene* scn,
	                   DkCst_source* src,
					   DkCst_wrapped_source** wrpd_src) {

	pthread_mutex_lock(&scn->lock);
	
	if(scn->scn_mgr->src_mgr != src->src_mgr){ pthread_mutex_unlock(&scn->lock); return ERROR; }
	
	for(int i=0; i<NB_WRP_SOURCES; i++) {
		if(scn->sources[i] == NULL) {
			scn->sources[i] = malloc(sizeof(DkCst_wrapped_source));
			if(pthread_mutex_init(&scn->sources[i]->lock, NULL) != 0){
				free(scn->sources[i]);
				scn->sources[i] = NULL;
				pthread_mutex_unlock(&scn->lock);
				return ERROR;
			}
		    scn->sources[i]->scn = scn;
			scn->sources[i]->id = i;
			scn->sources[i]->source_id = src->id;
			(*wrpd_src) = scn->sources[i];
		    scn->nb_sources++;
			pthread_mutex_unlock(&scn->lock);
			return OK;
		}
	}
	pthread_mutex_unlock(&scn->lock);
	return ERROR;
	
}

DkCst_rc DkCst_unwrap_source(DkCst_wrapped_source** wrpd_src) {

    DkCst_scene* scn = (*wrpd_src)->scn;
	pthread_mutex_lock(&scn->lock);
	pthread_mutex_destroy(&(*wrpd_src)->lock);
	uint8_t id = (*wrpd_src)->id;
    free(scn->sources[id]);
	scn->sources[id] = NULL;
	scn->nb_sources--;

	pthread_mutex_unlock(&scn->lock);
	return OK;
	
}

