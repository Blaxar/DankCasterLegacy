#ifndef DKCST_PARAMS_H
#define DKCST_PARAMS_H

#include "dkcst_rc.h"
#include <inttypes.h>
#include <stddef.h>

#define MAX_PARAM_NAME_LENGTH 32

struct DkCst_param_s;

typedef struct DkCst_param_s {

    enum DkCst_param_type_e {
		INT = 0,
		FLOAT,
		STRING,
	} type;
	
	char name[MAX_PARAM_NAME_LENGTH];
		
	union DkCst_param_data {
		int int_value ;
		float float_value;
		struct string_param {
			char* str;
			uint16_t length;
		} string_value;
	} data;

	struct DkCst_param_s* next;
	
} DkCst_param;

typedef struct dkcst_param_s {

	DkCst_param* first;
	uint8_t nb_params;
	
} DkCst_params;

DkCst_rc DkCst_create_param_pack(DkCst_params **params);

DkCst_rc DkCst_set_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], int value);
DkCst_rc DkCst_set_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], float value);
DkCst_rc DkCst_set_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], char* value, uint16_t length);

DkCst_rc DkCst_get_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], int* value);
DkCst_rc DkCst_get_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], float* value);
DkCst_rc DkCst_get_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], char** value, uint16_t* length);

DkCst_rc DkCst_unset_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH]);
DkCst_rc DkCst_unset_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH]);
DkCst_rc DkCst_unset_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH]);

DkCst_rc DkCst_delete_param_pack(DkCst_params **params);

#endif //DKCST_PARAMS_H	
