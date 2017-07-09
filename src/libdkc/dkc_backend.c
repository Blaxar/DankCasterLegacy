#include <libdkc/dkc_backend.h>
#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <libdkc/backends/dummy/dkc_dummy_backend.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

DkcBackend* dkc_backend_create(const char* backend, DkcParams* params) {
  
  if(!backend) {
    if(params) dkc_params_unref(params);
    return NULL;
  }
  
  DkcBackend* bkn = NULL;
  
  bkn = malloc(sizeof(DkcBackend));
  if(pthread_mutex_init(&bkn->lock, NULL)){ free(bkn); return ERROR; }

  if(strcmp(backend, "gst") == 0) {
    
    bkn->init = gstbkn_init;
    bkn->start = gstbkn_start;
    bkn->stop = gstbkn_stop;
    bkn->create_source = gstbkn_create_source;
    bkn->delete_source = gstbkn_delete_source;
    bkn->create_scene = gstbkn_create_scene;
    bkn->delete_scene = gstbkn_delete_scene;
    bkn->wrap_source = gstbkn_wrap_source;
    bkn->unwrap_source = gstbkn_unwrap_source;
    bkn->create_sink = gstbkn_create_sink;
    bkn->delete_sink = gstbkn_delete_sink;
    bkn->uninit = gstbkn_uninit;

  } else if (strcmp(backend, "dummy") == 0) {

    bkn->init = dummybkn_init;
    bkn->start = dummybkn_start;
    bkn->stop = dummybkn_stop;
    bkn->create_source = dummybkn_create_source;
    bkn->delete_source = dummybkn_delete_source;
    bkn->create_scene = dummybkn_create_scene;
    bkn->delete_scene = dummybkn_delete_scene;
    bkn->wrap_source = dummybkn_wrap_source;
    bkn->unwrap_source = dummybkn_unwrap_source;
    bkn->create_sink = dummybkn_create_sink;
    bkn->delete_sink = dummybkn_delete_sink;
    bkn->uninit = dummybkn_uninit;
      
  }
  
  if(!bkn->init(&bkn->ctx, params)){
    free(bkn);
    bkn = NULL;
  }
  
  return bkn;
  
}


gboolean dkc_backend_delete(DkcBackend* bkn) {

  if(bkn->uninit(&bkn->ctx)) {
    pthread_mutex_destroy(&bkn->lock);
    free(bkn);
    return OK;
  }
  
  return ERROR;
  
}
