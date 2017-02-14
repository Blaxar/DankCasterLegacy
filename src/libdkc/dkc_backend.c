#include <libdkc/dkc_backend.h>
#include <libdkc/backends/gstreamer-1.0/dkc_gst_backend.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

dkc_rc dkc_create_backend(DkcBackend** bkn, const char* backend, DkcParams* params) {
  
  if(*bkn == NULL) {
    
    *bkn = malloc(sizeof(DkcBackend));
    if(pthread_mutex_init(&(*bkn)->lock, NULL) != 0){ free(*bkn); return ERROR; }
    if(*bkn != NULL ) {

      (*bkn)->init = gstbkn_init;
      (*bkn)->create_source = gstbkn_create_source;
      (*bkn)->delete_source = gstbkn_delete_source;
      (*bkn)->create_scene = gstbkn_create_scene;
      (*bkn)->delete_scene = gstbkn_delete_scene;
      (*bkn)->wrap_source = gstbkn_wrap_source;
      (*bkn)->unwrap_source = gstbkn_unwrap_source;
      (*bkn)->create_sink = gstbkn_create_sink;
      (*bkn)->delete_sink = gstbkn_delete_sink;
      (*bkn)->uninit = gstbkn_uninit;

      if(dkc_rc_ok((*bkn)->init(&(*bkn)->ctx))) return OK;
      else return ERROR;
      
    }

  }
    return ERROR;
  
}


dkc_rc dkc_delete_backend(DkcBackend** bkn) {
  
  pthread_mutex_destroy(&(*bkn)->lock);

  if(dkc_rc_ok((*bkn)->uninit(&(*bkn)->ctx))) {
    if(!dkc_rc_ok((*bkn)->uninit(&(*bkn)->ctx))) return ERROR;
    free(*bkn);
    return OK;
  }
  
  return ERROR;
  
}
