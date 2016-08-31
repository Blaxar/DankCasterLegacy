#include <libdkcst/dkcst_scene.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Scene handling */


DkCst_rc DkCst_create_scene_mgr(DkCst_scene_mgr* src_mgr, DkCst_scene_mgr** scn_mgr) {

	(*scn_mgr) = malloc(sizeof(DkCst_scene_mgr));
	for(int i=0 ; i<NB_SCENES; i++) {
		(*scn_mgr)->scenes[i] = NULL;
	}
	(*scn_mgr)->nb_scenes=0;
	
}

DkCst_rc DkCst_delete_scene_mgr(DkCst_scene_mgr** scn_mgr) {

	for(int i=0; i<NB_SCENES; i++) {
		if((*scn_mgr)->scenes[i] != NULL) return ERROR;
	}
	free(*scn_mgr);
	return OK;
}

DkCst_rc DkCst_create_scene(DkCst_scene_mgr* scn_mgr, DkCst_scene** scn){

	for(int i=0; i<NB_SCENES; i++) {
		if(scn_mgr->scenes[i] == NULL) {
			scn_mgr->scenes[i] = malloc(sizeof(DkCst_scene));
			for(int j=0 ; j<NB_WRP_SOURCES; j++) {
				scn_mgr->scenes[i]->sources[j] = NULL;
			}
			(*scn) = scn_mgr->scenes[i];
			scn_mgr->nb_scenes++;
			return OK;
		}
	}
	return ERROR;
	
}

DkCst_rc DkCst_delete_scene(DkCst_scene_mgr* scn_mgr, DkCst_scene** scn){

	uint8_t id = (*scn)->id;
    for(int i=0 ; i<NB_WRP_SOURCES; i++) {
		if(scn_mgr->scenes[id]->sources[i] != NULL) return ERROR;
	}
	free(scn_mgr->scenes[id]);
	scn_mgr->nb_scenes--;
	return OK;
	
}

/* Source wrapping */

DkCst_rc DkCst_wrap_source(DkCst_scene* scn,
	                   DkCst_source* src,
					   DkCst_wrapped_source** wrpd_src) {

	for(int i=0; i<NB_WRP_SOURCES; i++) {
		if(scn->sources[i] == NULL) {
			scn->sources[i] = malloc(sizeof(DkCst_wrapped_source));
			scn->sources[i]->id = i;
			scn->sources[i]->source_id = src->id;
			(*wrpd_src) = scn->sources[i];
		    scn->nb_sources++;
			return OK;
		}
	}
	return ERROR;
	
}

DkCst_rc DkCst_unwrap_source(DkCst_scene* scn,
					     DkCst_wrapped_source** wrpd_src) {
	uint8_t id = (*wrpd_src)->id;
    free(scn->sources[id]);
	scn->nb_sources--;

}

