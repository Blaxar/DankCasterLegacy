#include <libdkcst/dkcst_params.h>

DkCst_rc DkCst_create_param_pack(DkCst_params **params) {

	(*params) = malloc(sizeof(DkCst_params));
	(*params)->first = NULL;
	(*params)->nb_params = 0;
	
	return OK;
	
}

DkCst_rc DkCst_set_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], int value) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_set_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], float value) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_set_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], char* value, uint16_t length) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_get_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], int* value) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_get_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], float* value) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_get_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], char** value, uint16_t* length) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_unset_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH]) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_unset_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH]) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_unset_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH]) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_delete_param_pack(DkCst_params **params) {
    return NOT_IMPLEMENTED;
}
