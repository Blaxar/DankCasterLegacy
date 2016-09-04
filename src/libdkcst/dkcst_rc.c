#include <libdkcst/dkcst_rc.h>

#include <stdio.h>

#define REGISTER_RC_MESSAGE(rcode, msg) if(rc == rcode){ sprintf(s,"%s\n",msg); return;}

int DkCst_rc_ok(DkCst_rc rc) {return rc == OK;}

void DkCst_rc_message(DkCst_rc rc, char* s) {

	REGISTER_RC_MESSAGE(OK, "Operation went OK.")
	REGISTER_RC_MESSAGE(NOT_IMPLEMENTED, "Operation is not implemented.")
	sprintf(s,"%s\n","Unknown return code."); return;
	
}
