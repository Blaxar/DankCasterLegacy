#ifndef DKCST_BACKEND_H
#define DKCST_BACKEND_H

#include "dkc_app.h"
#include "dkc_rc.h"

#define BACKEND_NAME_LENGTH 32

typedef struct _DkcBackend {

  pthread_mutex_t lock;
  uint8_t type_id;
  void* ctx;
  char* name;
  dkc_rc (*init)(void** ctx);
  DKCST_SOURCE_CBS();
  DKCST_SCENE_CBS();
  DKCST_SINK_CBS();
  dkc_rc (*uninit)(void** ctx);
  
} DkcBackend;

DkcBackend* dkc_create_backend(const char* type, DkcParams* params);
dkc_rc dkc_delete_backend(DkcBackend* bkn);


#endif //DKCST_BACKEND_H
