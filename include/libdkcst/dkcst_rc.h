#ifndef DKCST_RC_H
#define DKCST_RC_H

typedef enum {
  OK = 0,
  WAIT,
  NOT_IMPLEMENTED,
  ERROR,
} DkCst_rc;

int DkCst_rc_ok(DkCst_rc rc);

#endif //DKCST_RC_H	
