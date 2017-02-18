#include <libdkc/dkc_sink.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dkc_rc dkc_create_sink_mgr(DkcSinkMgr** snk_mgr, DkcSinkCBs snk_cbs) {

  (*snk_mgr) = malloc(sizeof(DkcSinkMgr));
  if(pthread_mutex_init(&(*snk_mgr)->lock, NULL) != 0){ free(*snk_mgr); return ERROR; }
  (*snk_mgr)->create_sink = snk_cbs.create_sink;
  (*snk_mgr)->delete_sink = snk_cbs.delete_sink;
    for(int i=0; i<NB_SINKS; i++) {
    (*snk_mgr)->sinks[i] = NULL;
  }
  (*snk_mgr)->nb_sinks = 0;
  return OK;
  
}

dkc_rc dkc_delete_sink_mgr(DkcSinkMgr**  snk_mgr) {

  pthread_mutex_destroy(&(*snk_mgr)->lock);
  for(int i=0; i<NB_SINKS; i++) {
    if((*snk_mgr)->sinks[i] != NULL) return ERROR;
  }
  free(*snk_mgr);
  return OK;
  
}

dkc_rc dkc_create_sink(DkcSinkMgr* snk_mgr, const char* type, DkcSink** snk, const char* name, DkcParams* params) {

  pthread_mutex_lock(&snk_mgr->lock);
  
  for(int j=0; j<NB_SINKS; j++) {
    if(snk_mgr->sinks[j] == NULL) {
      snk_mgr->sinks[j] = malloc(sizeof(DkcSink));
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


dkc_rc dkc_delete_sink(DkcSink** snk) {

  pthread_mutex_lock(&(*snk)->snk_mgr->lock);
  
  pthread_mutex_destroy(&(*snk)->lock);
  DkcSinkMgr* snk_mgr = (*snk)->snk_mgr;
  uint8_t id = (*snk)->id;
  free(snk_mgr->sinks[id]);
  snk_mgr->sinks[id] = NULL;
  snk_mgr->nb_sinks--;

  pthread_mutex_unlock(&(*snk)->snk_mgr->lock);
  
  return OK;
  
}
