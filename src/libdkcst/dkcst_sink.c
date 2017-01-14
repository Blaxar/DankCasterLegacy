#include <libdkcst/dkcst_sink.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DkCst_rc DkCst_create_sink_mgr(DkCstApp* app, DkCstSinkMgr** snk_mgr) {

    (*snk_mgr) = malloc(sizeof(DkCstSinkMgr));
	if(pthread_mutex_init(&(*snk_mgr)->lock, NULL) != 0){ free(*snk_mgr); return ERROR; }
	app->snk_mgr = *snk_mgr;
	(*snk_mgr)->app = app;
    for(int i=0; i<NB_SINKS; i++) {
		(*snk_mgr)->sinks[i] = NULL;
	}
	(*snk_mgr)->nb_sinks = 0;
	return OK;
	
}

DkCst_rc DkCst_delete_sink_mgr(DkCstSinkMgr**  snk_mgr) {

	pthread_mutex_destroy(&(*snk_mgr)->lock);
	for(int i=0; i<NB_SINKS; i++) {
		if((*snk_mgr)->sinks[i] != NULL) return ERROR;
	}
	(*snk_mgr)->app->snk_mgr = NULL;
	free(*snk_mgr);
	return OK;
	
}

DkCst_rc DkCst_create_sink(DkCstSinkMgr* snk_mgr, const char* type, DkCstParams* params, DkCstSink** snk, const char* name) {

	pthread_mutex_lock(&snk_mgr->lock);
	
    for(int j=0; j<NB_SINKS; j++) {
	    if(snk_mgr->sinks[j] == NULL) {
		    snk_mgr->sinks[j] = malloc(sizeof(DkCstSink));
		    if(pthread_mutex_init(&snk_mgr->sinks[j]->lock, NULL) != 0){
			    free(snk_mgr->sinks[j]);
			    snk_mgr->sinks[j] = NULL;
			    pthread_mutex_unlock(&snk_mgr->lock);
			    return ERROR;
			}
			snk_mgr->nb_sinks++;
			(*snk) = snk_mgr->sinks[j];
			(*snk)->snk_mgr = snk_mgr;
			pthread_mutex_unlock(&snk_mgr->lock);
			return OK;
		}
	}
	(*snk) = NULL;
	pthread_mutex_unlock(&snk_mgr->lock);
    return ERROR;
	
}


DkCst_rc DkCst_delete_sink(DkCstSink** snk) {

	pthread_mutex_lock(&(*snk)->snk_mgr->lock);
	
	pthread_mutex_destroy(&(*snk)->lock);
    DkCstSinkMgr* snk_mgr = (*snk)->snk_mgr;
	uint8_t id = (*snk)->id;
	free(snk_mgr->sinks[id]);
	snk_mgr->sinks[id] = NULL;
	snk_mgr->nb_sinks--;

	pthread_mutex_unlock(&(*snk)->snk_mgr->lock);
	
	return OK;
	
}