#include <libdkcst/dkcst_params.h>

DkCst_rc DkCst_create_param_pack(DkCst_params **params) {

	(*params) = malloc(sizeof(DkCst_params));
	(*params)->first = NULL;
	(*params)->nb_params = 0;
	
	return OK;
	
}

DkCst_rc DkCst_set_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], int value) {
	if(params->first == NULL) { //No param in the list yet
		params->first = malloc(sizeof(DkCst_param));
		params->first->type = INT;
		memcpy(params->first->name, name, MAX_PARAM_NAME_LENGTH);
		params->first->data.int_value = value;
		params->first->next = NULL;
		params->nb_params++;
		return OK;
	} else {
		DkCst_param* prev_param = NULL;
		DkCst_param* param = params->first;
		while(param != NULL){ //Iterate over existing params
			if(strcmp(name, param->name) == 0) { //Param with this name already exists
				if(param->type == INT) //Update if types do match
					param->data.int_value = value;
			    else //Something is wrong, types are differents
					return ERROR;
				return OK;
			}
			prev_param = param;
			param=param->next;
		}
		//at this point, no existing param with this name, add it to the list.
		prev_param->next=malloc(sizeof(DkCst_param));
		prev_param->next->type = INT;
		memcpy(prev_param->next->name, name, MAX_PARAM_NAME_LENGTH);
		prev_param->next->data.int_value = value;
		prev_param->next->next = NULL;
		params->nb_params++;
		return OK;
	}
}

DkCst_rc DkCst_set_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], float value) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_set_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], char* value, uint16_t length) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_get_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], int* value) {
	DkCst_param* param = params->first;
	while(param != NULL){ //Iterate over existing params
		if(strcmp(name, param->name) == 0) { //Param with this name already exists
			if(param->type == INT) //return value if types do match
				(*value) = param->data.int_value;
			else //Something is wrong, types are differents
				return ERROR;
			return OK;
		}
		param=param->next;
	}
	//at this point, no existing param with this name
	return ERROR;
}

DkCst_rc DkCst_get_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], float* value) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_get_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], char** value, uint16_t* length) {
	return NOT_IMPLEMENTED;
}

DkCst_rc DkCst_unset_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH]) {
	DkCst_param* prev_param = NULL;
	DkCst_param* param = params->first;
	while(param != NULL){ //Iterate over existing params
		if(strcmp(name, param->name) == 0) { //Param with this name already exists
			if(param->type == INT) { //remove param if types do match
				prev_param->next=param->next;
				free(param);
			} else //Something is wrong, types are differents
				return ERROR;
			return OK;
		}
		prev_param = param;
		param=param->next;
	}
	//at this point, no existing param with this name
	return ERROR;
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
