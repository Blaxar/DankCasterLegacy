#include <libdkc/dkc_param.h>
#include <string.h>
#include <stdlib.h>

DkcParams* dkc_wrap_params(const char* name, ...) {

  DkcParams* params = NULL;
  char* nname = name;
  DkcParamType type;
  dkc_create_param_pack(&params);
  
  va_list valist;
  va_start(valist, name);
  
  while(nname != NULL){
      
    type = va_arg(valist, DkcParamType);
    
    switch(type){
    case INT: {
      dkc_set_int_param(params, nname, va_arg(valist, int));
    }
    break;
    case FLOAT: {
      dkc_set_float_param(params, nname, va_arg(valist, double));
    }
    break;
    case STRING: {
      dkc_set_string_param(params, nname, va_arg(valist, char*));
    }
    break;
    default: {
        
    }
    break;
    }
    
    nname = va_arg(valist, char*);
        
  }

  return params;
    
}

int dkc_pop_int_param(DkcParams *params, const char* name, int default_value) {
    
  int value;
  if(dkc_rc_ok(dkc_get_int_param(params, name, &value))) {
    dkc_unset_param(params, name);
    return value;
  }else
    return default_value;
  
}

float dkc_pop_float_param(DkcParams *params, const char* name, float default_value) {

  float value;
  if(dkc_rc_ok(dkc_get_float_param(params, name, &value))) {
    dkc_unset_param(params, name);
    return value;
  }else
    return default_value;
    
}

char* dkc_pop_string_param(DkcParams *params, const char* name, char* default_value) {

  char* value;
  if(dkc_rc_ok(dkc_get_string_param(params, name, &value)))
    dkc_unset_param(params, name);
  else {
    value = malloc((strlen(default_value)+1)*sizeof(char));
    memcpy(value, default_value, (strlen(default_value)+1)*sizeof(char));
  }
  
  return value;
}
    
dkc_rc dkc_create_param_pack(DkcParams **params) {

  (*params) = malloc(sizeof(DkcParams));
  (*params)->first = NULL;
  (*params)->nb_params = 0;
  
  return OK;
  
}

dkc_rc dkc_set_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], DkcParamType type, void* value) {
  
  uint16_t name_length = strlen(name);
  if(name_length > MAX_PARAM_NAME_LENGTH-1) return ERROR;
  
  if(params->first == NULL) { //No param in the list yet
    params->first = malloc(sizeof(DkcParam));
    params->first->type = type;
    if(type == INT)
        params->first->data.int_value = *(int*)value;
    else if(type == FLOAT)
      params->first->data.float_value = *(float*)value;
    else if(type == STRING){
      params->first->data.string_value.str = malloc((strlen(value)+1)*sizeof(char));
      memcpy(params->first->data.string_value.str, value, (strlen(value)+1)*sizeof(char));
    }
    memcpy(params->first->name, name, name_length+1);
    params->first->next = NULL;
    params->nb_params++;
    return OK;
  } else {
    DkcParam* prev_param = NULL;
    DkcParam* param = params->first;
    while(param != NULL){ //Iterate over existing params
      if(strcmp(name, param->name) == 0) { //Param with this name already exists
        if(param->type == type){ //Update if types do match
          if(type == INT)
            param->data.int_value = *(int*)value;
          else if(type == FLOAT)
              param->data.float_value = *(float*)value;
          else if(type == STRING) {
            free(param->data.string_value.str);
            param->data.string_value.str = malloc((strlen(value)+1)*sizeof(char));
            memcpy(param->data.string_value.str, value, (strlen(value)+1)*sizeof(char));
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
    prev_param->next=malloc(sizeof(DkcParam));
    prev_param->next->type = type;
    memcpy(prev_param->next->name, name, name_length+1);
    if(type == INT)
      prev_param->next->data.int_value = *(int*)value;
    else if(type == FLOAT)
      prev_param->next->data.float_value = *(float*)value;
    else if(type == STRING) {
        prev_param->next->data.string_value.str = malloc((strlen(value)+1)*sizeof(char));
      memcpy(prev_param->next->data.string_value.str, value, (strlen(value)+1)*sizeof(char));
    }
    prev_param->next->next = NULL;
    params->nb_params++;
    return OK;
  }
}

dkc_rc dkc_set_int_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], int value) {
    return dkc_set_param(params, name, INT, &value);
}

dkc_rc dkc_set_float_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], float value) {
  return dkc_set_param(params, name, FLOAT, &value);
}

dkc_rc dkc_set_string_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], char* value) {
    return dkc_set_param(params, name, STRING, value);
}

dkc_rc dkc_get_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], DkcParamType type, void** value, uint16_t* length) {

  uint16_t name_length = strlen(name);
  if(name_length > MAX_PARAM_NAME_LENGTH-1) return ERROR;
  
    DkcParam* param = params->first;
  while(param != NULL){ //Iterate over existing params
    if(strcmp(name, param->name) == 0) { //Param with this name already exists
      if(param->type == type) //return value if types do match
        if(type == INT)
            **(int**)value = param->data.int_value;
        else if(type == FLOAT)
          **(float**)value = param->data.float_value;
        else if(type == STRING) {
          (*value) = malloc((strlen(param->data.string_value.str)+1)*sizeof(char));
          memcpy(*value, param->data.string_value.str,
               (strlen(param->data.string_value.str)+1)*sizeof(char));
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

dkc_rc dkc_get_int_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], int* value) {
    return dkc_get_param(params, name, INT, &value, NULL);
}

dkc_rc dkc_get_float_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], float* value) {
    return dkc_get_param(params, name, FLOAT, &value, NULL);
}

dkc_rc dkc_get_string_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], char** value) {
    return dkc_get_param(params, name, STRING, value, NULL);
}

dkc_rc dkc_unset_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH]) {

  uint16_t name_length = strlen(name);
  if(name_length > MAX_PARAM_NAME_LENGTH-1) return ERROR;
  
  DkcParam* prev_param = NULL;
  DkcParam* param = params->first;
  while(param != NULL){ //Iterate over existing params
    if(strcmp(name, param->name) == 0) { //Param with this name exists
      if(prev_param != NULL) prev_param->next=param->next;
      else params->first = param->next;
      
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

dkc_rc dkc_delete_param_pack(DkcParams **params) {
  if((*params)->nb_params > 0)
    return ERROR;

  free(*params);
  return OK;
}
