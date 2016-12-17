#include <libdkcst/dkcst.h>
#include <libdkcst/dkcst_backend.h>

DkCst_rc DkCst_init(void) {

	if (!DkCst_rc_ok(DkCst_ready_all_source_types())) return ERROR;
	if (!DkCst_rc_ok(DkCst_register_all_source_types())) return ERROR;
	
	return OK;
	
}

DkCst_rc DkCst_terminate(void) {

	return DkCst_unregister_all_backends();
	
}
