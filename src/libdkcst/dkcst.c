#include <libdkcst/dkcst.h>
#include <libdkcst/dkcst_backend.h>

DkCst_rc DkCst_init(void) {

	if (!DkCst_rc_ok(DkCst_ready_all_backends())) return ERROR;
	if (!DkCst_rc_ok(DkCst_register_all_backends())) return ERROR;
	
	return OK;
	
}

DkCst_rc DkCst_terminate(void) {

	return DkCst_unregister_all_backends();
	
}
