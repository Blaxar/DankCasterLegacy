#include <libdkcst/dkcst_params.h>
#include <stdlib.h>

DkCst_rc DkCst_create_param_pack(DkCst_params **params) {

	(*params) = malloc(sizeof(DkCst_params));
	(*params)->first = NULL;
	(*params)->nb_params = 0;
	
	return OK;
	
}

DkCst_rc DkCst_set_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], DkCst_param_type type, void* value, uint16_t length) {
	if(params->first == NULL) { //No param in the list yet
		params->first = malloc(sizeof(DkCst_param));
		if(type == INT)
		    params->first->data.int_value = *(int*)value;
		else if(type == FLOAT)
			params->first->data.float_value = *(float*)value;
		else if(type == STRING) {
			memcpy(params->first->data.string_value.str, value, length);
			params->first->data.string_value.length = length;
		}
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
				if(param->type == type){ //Update if types do match
					if(type == INT)
						param->data.int_value = *(int*)value;
					else if(type == FLOAT)
						prev_param->next->data.float_value = *(float*)value;
					else if(type == STRING) {
						memcpy(prev_param->next->data.string_value.str, value, length);
						prev_param->next->data.string_value.length = length;
					}
				}
			    else //Something is wrong, types are differents
					return ERROR;
				return OK;
			}
			prev_param = param;
			param=param->next;
		}
		//at this point, no existing param with this name, add it to the list.
		prev_param->next=malloc(sizeof(DkCst_param));
		prev_param->next->type = type;
		memcpy(prev_param->next->name, name, MAX_PARAM_NAME_LENGTH);
		if(type == INT)
			prev_param->next->data.int_value = *(int*)value;
		else if(type == FLOAT)
			prev_param->next->data.float_value = *(float*)value;
		else if(type == STRING) {
			memcpy(prev_param->next->data.string_value.str, value, length);
			prev_param->next->data.string_value.length = length;
		}
		prev_param->next->next = NULL;
		params->nb_params++;
		return OK;
	}
}

DkCst_rc DkCst_set_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], int value) {
    return DkCst_set_param(params, name, INT, &value, 0);
}

DkCst_rc DkCst_set_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], float value) {
	return DkCst_set_param(params, name, FLOAT, &value, 0);
}

DkCst_rc DkCst_set_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], char* value, uint16_t length) {
    return DkCst_set_param(params, name, STRING, value, length);
}

DkCst_rc DkCst_get_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], DkCst_param_type type, void** value, uint16_t *length) {
    DkCst_param* param = params->first;
	while(param != NULL){ //Iterate over existing params
		if(strcmp(name, param->name) == 0) { //Param with this name already exists
			if(param->type == type) //return value if types do match
				if(type == INT)
				    **(int**)value = param->data.int_value;
				else if(type == FLOAT)
					**(float**)value = param->data.float_value;
				else if(type == STRING) {
					memcpy(*value, param->data.string_value.str, length);
				    (*length) = length;
				}
			else //Something is wrong, types are differents
				return ERROR;
			return OK;
		}
		param=param->next;
	}
	//at this point, no existing param with this name
	return ERROR;
}

DkCst_rc DkCst_get_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], int* value) {
    return DkCst_get_param(params, name, INT, &value, NULL);
}

DkCst_rc DkCst_get_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], float* value) {
    return DkCst_get_param(params, name, FLOAT, &value, NULL);
}

DkCst_rc DkCst_get_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], char** value, uint16_t* length) {
    return DkCst_get_param(params, name, STRING, &value, length);
}

DkCst_rc DkCst_unset_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH]) {
	DkCst_param* prev_param = NULL;
	DkCst_param* param = params->first;
	while(param != NULL){ //Iterate over existing params
		if(strcmp(name, param->name) == 0) { //Param with this name exists
			prev_param->next=param->next;
			if(param->type == STRING)
				free(param->data.string_value.str);
			free(param);
			params->nb_params--;
			return OK;
		}
		prev_param = param;
		param=param->next;
	}
	//at this point, no existing param with this name
	return ERROR;
}

DkCst_rc DkCst_delete_param_pack(DkCst_params **params) {
	if((*params)->nb_params > 0)
		return ERROR;

	free(*params);
	return OK;
}
