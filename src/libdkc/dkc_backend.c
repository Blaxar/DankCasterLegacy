#include <libdkc/dkc_backend.h>
#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

DkcBackend* dkc_backend_create(const char* backend, DkcParams* params) {
  
  DkcBackend* bkn = NULL;
    
  bkn = malloc(sizeof(DkcBackend));
  if(pthread_mutex_init(&bkn->lock, NULL)){ free(bkn); return ERROR; }

  bkn->init = gstbkn_init;
  bkn->create_source = gstbkn_create_source;
  bkn->delete_source = gstbkn_delete_source;
  bkn->create_scene = gstbkn_create_scene;
  bkn->delete_scene = gstbkn_delete_scene;
  bkn->wrap_source = gstbkn_wrap_source;
  bkn->unwrap_source = gstbkn_unwrap_source;
  bkn->create_sink = gstbkn_create_sink;
  bkn->delete_sink = gstbkn_delete_sink;
  bkn->uninit = gstbkn_uninit;

  if(!bkn->init(&bkn->ctx)){
    free(bkn);
    bkn = NULL;
  }

  return bkn;
  
}


dkc_rc dkc_backend_delete(DkcBackend* bkn) {

  if(bkn->uninit(&bkn->ctx)) {
    pthread_mutex_destroy(&bkn->lock);
    free(bkn);
    return OK;
  }
  
  return ERROR;
  
}
