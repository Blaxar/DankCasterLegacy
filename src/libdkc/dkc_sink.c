#include <libdkc/dkc_sink.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DkcSinkMgr* dkc_create_sink_mgr(DkcSinkCBs snk_cbs) {

  DkcSinkMgr* snk_mgr = malloc(sizeof(DkcSinkMgr));
  if(pthread_mutex_init(&snk_mgr->lock, NULL) != 0){ free(snk_mgr); return NULL; }
  snk_mgr->create_sink = snk_cbs.create_sink;
  snk_mgr->delete_sink = snk_cbs.delete_sink;
  snk_mgr->bkn_ctx = snk_cbs.bkn_ctx;
    for(int i=0; i<NB_SINKS; i++) {
    snk_mgr->sinks[i] = NULL;
  }
  snk_mgr->nb_sinks = 0;
  return snk_mgr;
  
}

dkc_rc dkc_delete_sink_mgr(DkcSinkMgr*  snk_mgr) {

  pthread_mutex_destroy(&snk_mgr->lock);
  for(int i=0; i<NB_SINKS; i++) {
    if(snk_mgr->sinks[i] != NULL) return ERROR;
  }
  free(snk_mgr);
  return OK;
  
}

DkcSink* dkc_create_sink(DkcSinkMgr* snk_mgr, DkcSinkType snk_type, const char* uri, const char* name, DkcParams* params) {

  DkcSink* snk;
  pthread_mutex_lock(&snk_mgr->lock);
  
  for(int j=0; j<NB_SINKS; j++) {
    if(snk_mgr->sinks[j] == NULL) {
      snk_mgr->sinks[j] = malloc(sizeof(DkcSink));
      if(pthread_mutex_init(&snk_mgr->sinks[j]->lock, NULL) != 0 ||
         snk_mgr->create_sink(snk_mgr->bkn_ctx, j, snk_type, uri, name, params) != OK){
        free(snk_mgr->sinks[j]);
        snk_mgr->sinks[j] = NULL;
        pthread_mutex_unlock(&snk_mgr->lock);
        return snk;
      }
      snk_mgr->nb_sinks++;
      snk = snk_mgr->sinks[j];
      snk->snk_mgr = snk_mgr;
      snk->snk_type = snk_type;
      pthread_mutex_unlock(&snk_mgr->lock);
      return snk;
    }
  }
  snk = NULL;
  pthread_mutex_unlock(&snk_mgr->lock);
  return snk;
  
}


dkc_rc dkc_delete_sink(DkcSink* snk) {

  pthread_mutex_lock(&snk->snk_mgr->lock);
  
  DkcSinkMgr* snk_mgr = snk->snk_mgr;
  uint8_t id = snk->id;
  if(snk_mgr->delete_sink(snk_mgr->bkn_ctx, id) != OK) {
    pthread_mutex_unlock(&snk->snk_mgr->lock);
    return ERROR;
  }
  pthread_mutex_destroy(&snk->lock);
  free(snk_mgr->sinks[id]);
  snk_mgr->sinks[id] = NULL;
  snk_mgr->nb_sinks--;

  pthread_mutex_unlock(&snk->snk_mgr->lock);
  return OK;
  
}
