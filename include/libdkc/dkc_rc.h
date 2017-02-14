#ifndef DKCST_RC_H
#define DKCST_RC_H

typedef enum {
  OK = 0,
  WAIT,
  NOT_IMPLEMENTED,
  ERROR,
} dkc_rc;

int dkc_rc_ok(dkc_rc rc);

#endif //DKCST_RC_H	
