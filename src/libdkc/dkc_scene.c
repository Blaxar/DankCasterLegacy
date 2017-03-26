#include <libdkc/dkc_scene.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Scene handling */


dkc_rc dkc_create_scene_mgr(DkcSceneMgr** scn_mgr, DkcSceneCBs scn_cbs) {

  (*scn_mgr) = malloc(sizeof(DkcSceneMgr));
  if(pthread_mutex_init(&(*scn_mgr)->lock, NULL) != 0){ free(*scn_mgr); return ERROR; }
  (*scn_mgr)->create_scene = scn_cbs.create_scene;
  (*scn_mgr)->delete_scene = scn_cbs.delete_scene;
  (*scn_mgr)->wrap_source = scn_cbs.wrap_source;
  (*scn_mgr)->unwrap_source = scn_cbs.unwrap_source;
  for(int i=0 ; i<NB_SCENES; i++) {
    (*scn_mgr)->scenes[i] = NULL;
  }
  (*scn_mgr)->nb_scenes=0;
  return OK;
  
}

dkc_rc dkc_delete_scene_mgr(DkcSceneMgr** scn_mgr) {

  pthread_mutex_destroy(&(*scn_mgr)->lock);
  for(int i=0; i<NB_SCENES; i++) {
    if((*scn_mgr)->scenes[i] != NULL) return ERROR;
  }
  free(*scn_mgr);
  return OK;
  
}

dkc_rc dkc_create_scene(DkcSceneMgr* scn_mgr, DkcScene** scn){

  pthread_mutex_lock(&scn_mgr->lock);
  
  for(int i=0; i<NB_SCENES; i++) {
    if(scn_mgr->scenes[i] == NULL) {
      scn_mgr->scenes[i] = malloc(sizeof(DkcScene));
      if(pthread_mutex_init(&scn_mgr->scenes[i]->lock, NULL) != 0 ||
         scn_mgr->create_scene(scn_mgr->bkn_ctx, i) != OK){
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
      (*scn)->id = i;
      scn_mgr->nb_scenes++;
      pthread_mutex_unlock(&scn_mgr->lock);
      return OK;
    }
  }
  pthread_mutex_unlock(&scn_mgr->lock);
  return ERROR;
  
}

dkc_rc dkc_delete_scene(DkcScene** scn){

  DkcSceneMgr* scn_mgr = (*scn)->scn_mgr;
  pthread_mutex_lock(&scn_mgr->lock);
  
  uint8_t id = (*scn)->id;
  for(int i=0 ; i<NB_WRP_SOURCES; i++) {
    if(scn_mgr->scenes[id]->sources[i] != NULL){ pthread_mutex_unlock(&scn_mgr->lock); return ERROR; }
  }
  if(scn_mgr->delete_scene(scn_mgr->bkn_ctx, id) != OK) {
    pthread_mutex_unlock(&scn_mgr->lock);
    return ERROR;
  }
  pthread_mutex_destroy(&scn_mgr->scenes[id]->lock);
  free(scn_mgr->scenes[id]);
  scn_mgr->scenes[id] = NULL;
  scn_mgr->nb_scenes--;
  pthread_mutex_unlock(&scn_mgr->lock);
  return OK;
  
}

/* Source wrapping */

dkc_rc dkc_wrap_source(DkcScene* scn,
                       DkcSource* src,
                       DkcWrappedSource** wrpd_src) {

  DkcSceneMgr* scn_mgr = (*wrpd_src)->scn->scn_mgr;
  pthread_mutex_lock(&scn->lock);
  
  for(int i=0; i<NB_WRP_SOURCES; i++) {
    if(scn->sources[i] == NULL) {
      scn->sources[i] = malloc(sizeof(DkcWrappedSource));
      if(pthread_mutex_init(&scn->sources[i]->lock, NULL) != 0 ||
         scn_mgr->wrap_source(scn_mgr->bkn_ctx, scn->id, src->id, i) != OK){
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

dkc_rc dkc_unwrap_source(DkcWrappedSource** wrpd_src) {

  DkcScene* scn = (*wrpd_src)->scn;
  DkcSceneMgr* scn_mgr = scn->scn_mgr;
  uint8_t id = (*wrpd_src)->id;
  pthread_mutex_lock(&scn->lock);
  if(scn_mgr->unwrap_source(scn_mgr->bkn_ctx, id) != OK) {
    pthread_mutex_unlock(&scn->lock);
    return ERROR;
  }
  pthread_mutex_destroy(&(*wrpd_src)->lock);
  free(scn->sources[id]);
  scn->sources[id] = NULL;
  scn->nb_sources--;

  pthread_mutex_unlock(&scn->lock);
  return OK;
  
}
