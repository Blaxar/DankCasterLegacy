#ifndef DKCST_RC_H
#define DKCST_RC_H

typedef enum {
  ERROR=0,
  OK=1
} dkc_rc;

typedef enum {
  ERRD_APP = 0,
  ERRD_SOURCE,
  ERRD_SCENE,
  ERRD_SINK,
  ERRD_MISC,
} ErrorDomain;

typedef enum {
  ERRC_MAX_CAPACITY = 0,
} ErrorCode;

#endif //DKCST_RC_H	
