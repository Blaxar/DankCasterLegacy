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
  gboolean (*init)(void** ctx, DkcParams* params);
  DKC_APP_CBS();
  DKC_SOURCE_CBS();
  DKC_SCENE_CBS();
  DKC_SINK_CBS();
  gboolean (*uninit)(void** ctx);
  
} DkcBackend;

DkcBackend* dkc_backend_create(const char* type, DkcParams* params);
gboolean dkc_backend_delete(DkcBackend* bkn);


#endif //DKCST_BACKEND_H
