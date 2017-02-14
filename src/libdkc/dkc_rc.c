#include <libdkc/dkc_rc.h>

#include <stdio.h>

#define REGISTER_RC_MESSAGE(rcode, msg) if(rc == rcode){ sprintf(s,"%s\n",msg); return;}

int dkc_rc_ok(dkc_rc rc) {return rc == OK;}

void dkc_rc_message(dkc_rc rc, char* s) {

  REGISTER_RC_MESSAGE(OK, "Operation went OK.")
  REGISTER_RC_MESSAGE(NOT_IMPLEMENTED, "Operation is not implemented.")
  sprintf(s,"%s\n","Unknown return code."); return;
  
}
