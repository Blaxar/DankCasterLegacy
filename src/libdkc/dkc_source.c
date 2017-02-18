#include <libdkc/dkc_source.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dkc_rc dkc_create_source_mgr(DkcSourceMgr** src_mgr, DkcSourceCBs src_cbs) {

  (*src_mgr) = malloc(sizeof(DkcSourceMgr));
  if(pthread_mutex_init(&(*src_mgr)->lock, NULL) != 0){ free(*src_mgr); return ERROR; }
  (*src_mgr)->create_source = src_cbs.create_source;
  (*src_mgr)->delete_source = src_cbs.delete_source;
  for(int i=0; i<NB_SOURCES; i++) {
    (*src_mgr)->sources[i] = NULL;
  }
  (*src_mgr)->nb_sources = 0;
  return OK;
  
}

dkc_rc dkc_delete_source_mgr(DkcSourceMgr**  src_mgr) {

  pthread_mutex_destroy(&(*src_mgr)->lock);
  for(int i=0; i<NB_SOURCES; i++) {
    if((*src_mgr)->sources[i] != NULL) return ERROR;
  }
  free(*src_mgr);
  return OK;
  
}

dkc_rc dkc_create_source(DkcSourceMgr* src_mgr, const char* type, DkcSource** src, const char* name, DkcParams* params) {

  pthread_mutex_lock(&src_mgr->lock);
  
  for(int j=0; j<NB_SOURCES; j++) {
    if(src_mgr->sources[j] == NULL) {
      src_mgr->sources[j] = malloc(sizeof(DkcSource));
      if(pthread_mutex_init(&src_mgr->sources[j]->lock, NULL) != 0){
        free(src_mgr->sources[j]);
        src_mgr->sources[j] = NULL;
        pthread_mutex_unlock(&src_mgr->lock);
        return ERROR;
      }
      src_mgr->nb_sources++;
      (*src) = src_mgr->sources[j];
      (*src)->src_mgr = src_mgr;
      pthread_mutex_unlock(&src_mgr->lock);
      return OK;
    }
  }
  (*src) = NULL;
  pthread_mutex_unlock(&src_mgr->lock);
  return ERROR;
  
}


dkc_rc dkc_delete_source(DkcSource** src) {

  pthread_mutex_lock(&(*src)->src_mgr->lock);
  
  pthread_mutex_destroy(&(*src)->lock);
  DkcSourceMgr* src_mgr = (*src)->src_mgr;
  uint8_t id = (*src)->id;
  free(src_mgr->sources[id]);
  src_mgr->sources[id] = NULL;
  src_mgr->nb_sources--;

  pthread_mutex_unlock(&(*src)->src_mgr->lock);
  
  return OK;
  
}
