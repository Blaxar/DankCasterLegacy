#include <libdkc/dkc_source.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DkcSourceMgr* dkc_create_source_mgr(DkcSourceCBs src_cbs) {

  DkcSourceMgr* src_mgr = malloc(sizeof(DkcSourceMgr));
  if(pthread_mutex_init(&src_mgr->lock, NULL) != 0){ free(src_mgr); return NULL; }
  src_mgr->create_source = src_cbs.create_source;
  src_mgr->delete_source = src_cbs.delete_source;
  src_mgr->bkn_ctx = src_cbs.bkn_ctx;
  for(int i=0; i<NB_SOURCES; i++) {
    src_mgr->sources[i] = NULL;
  }
  src_mgr->nb_sources = 0;
  return src_mgr;
  
}

dkc_rc dkc_delete_source_mgr(DkcSourceMgr*  src_mgr) {

  pthread_mutex_destroy(&src_mgr->lock);
  for(int i=0; i<NB_SOURCES; i++) {
    if(src_mgr->sources[i] != NULL) return ERROR;
  }
  free(src_mgr);
  return OK;
  
}

DkcSource* dkc_create_source(DkcSourceMgr* src_mgr, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params) {

  DkcSource* src;
  pthread_mutex_lock(&src_mgr->lock);
  
  for(int j=0; j<NB_SOURCES; j++) {
    if(src_mgr->sources[j] == NULL) {
      src_mgr->sources[j] = malloc(sizeof(DkcSource));
      if(pthread_mutex_init(&src_mgr->sources[j]->lock, NULL) != 0 ||
         src_mgr->create_source(src_mgr->bkn_ctx, j, src_type, uri, name, params) != OK) {
        free(src_mgr->sources[j]);
        src_mgr->sources[j] = NULL;
        pthread_mutex_unlock(&src_mgr->lock);
        return src;
      }
      src_mgr->nb_sources++;
      src = src_mgr->sources[j];
      src->id = j;
      src->src_mgr = src_mgr;
      src->src_type = src_type;
      pthread_mutex_unlock(&src_mgr->lock);
      return src;
    }
  }
  src = NULL;
  
  pthread_mutex_unlock(&src_mgr->lock);
  return src;
  
}


dkc_rc dkc_delete_source(DkcSource* src) {

  pthread_mutex_lock(&src->src_mgr->lock);
  
  DkcSourceMgr* src_mgr = src->src_mgr;
  uint8_t id = src->id;
  if(src_mgr->delete_source(src_mgr->bkn_ctx, id) != OK) {
    pthread_mutex_unlock(&src->src_mgr->lock);
    return ERROR;
  }
  pthread_mutex_destroy(&src->lock);
  free(src_mgr->sources[id]);
  src_mgr->sources[id] = NULL;
  src_mgr->nb_sources--;
  
  pthread_mutex_unlock(&src->src_mgr->lock);
  return OK;
  
}
