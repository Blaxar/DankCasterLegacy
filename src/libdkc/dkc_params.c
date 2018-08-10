#include <libdkc/dkc_params.h>
#include <string.h>
#include <stdlib.h>

DkcParams* dkc_params_wrap(const char* name, ...) {

  DkcParams* params = NULL;

  va_list args;
  va_start(args, name);
  params = dkc_params_vwrap(name, args);
  va_end(args);

  return params;

}
  
DkcParams* dkc_params_vwrap(const char* name, va_list args) {

  DkcParams* params = NULL;
  char* nname = name;
  DkcParamType type;
  params = dkc_params_create();

  while(nname != NULL){

    type = va_arg(args, DkcParamType);

    switch(type){
    case DKC_TYPE_INT: {
      dkc_params_set_int(params, nname, va_arg(args, int));
    }
    break;
    case DKC_TYPE_FLOAT: {
      dkc_params_set_float(params, nname, va_arg(args, double));
    }
    break;
    case DKC_TYPE_STRING: {
      dkc_params_set_string(params, nname, va_arg(args, char*));
    }
    break;
    case DKC_TYPE_FRACTION: {
        dkc_params_set_fraction(params, nname,
                                (DkcFraction){va_arg(args, int), va_arg(args, int)});
    }
    break;
    default: {

    }
    break;
    }

    nname = va_arg(args, char*);

  }

  return params;

}

gboolean dkc_params_pop_all(DkcParams *params, dkc_param_cb param_cb, void* ctx) {

  if(params == NULL) return ERROR;

  DkcParam* param = params->first;
  DkcParam* rparam = NULL;

  while(param != NULL){ //Iterate over existing params

    rparam = param;
    param=param->next;
    gboolean rc;

    switch(rparam->type){
    case DKC_TYPE_INT: {
      int value;
      dkc_params_get_int(params, rparam->name, &value);
      param_cb(rparam->name, rparam->type, &value, ctx);
    }
    break;
    case DKC_TYPE_FLOAT: {
      float value;
      dkc_params_get_float(params, rparam->name, &value);
      param_cb(rparam->name, rparam->type, &value, ctx);
    }
    break;
    case DKC_TYPE_STRING: {
      char *value;
      dkc_params_get_string(params, rparam->name, &value);
      param_cb(rparam->name, rparam->type, value, ctx);
      free(value);
    }
    break;
    case DKC_TYPE_FRACTION: {
      DkcFraction *value;
      dkc_params_get_fraction(params, rparam->name, &value);
      param_cb(rparam->name, rparam->type, value, ctx);
      free(value);
    }
    }
  
    params->nb_params--;

  }
 
  return OK;

}

int dkc_params_fetch_int(DkcParams *params, const char* name, int default_value) {

  if(params == NULL) return default_value;

  int value;
  if(dkc_params_get_int(params, name, &value))
    return value;
  else
    return default_value;

}

float dkc_params_fetch_float(DkcParams *params, const char* name, float default_value) {

  if(params == NULL) return default_value;

  float value;
  if(dkc_params_get_float(params, name, &value))
    return value;
  else
    return default_value;

}

char* dkc_params_fetch_string(DkcParams *params, const char* name, char* default_value) {

  if(params == NULL) return default_value;

  char* value;
  if(!dkc_params_get_string(params, name, &value)) {
    value = malloc((strlen(default_value)+1)*sizeof(char));
    memcpy(value, default_value, (strlen(default_value)+1)*sizeof(char));
  }

  return value;

}

DkcFraction dkc_params_fetch_fraction(DkcParams *params, const char* name, DkcFraction default_value) {

  if(params == NULL) return default_value;

  DkcFraction value;
  if(dkc_params_get_fraction(params, name, &value)) {
    return value;
  } else 
    return default_value;

}

DkcParams* dkc_params_create() {

  DkcParams* params = malloc(sizeof(DkcParams));
  params->first = NULL;
  params->nb_params = 0;
  params->refs = 1;

  return params;

}

gboolean dkc_set_param(DkcParams *params, const char* name, DkcParamType type, void* value) {
  
  uint16_t name_length = strlen(name);
  
  if(params->first == NULL) { //No param in the list yet
    params->first = malloc(sizeof(DkcParam));
    params->first->type = type;
    if(type == DKC_TYPE_INT)
        params->first->data.int_value = *(int*)value;
    else if(type == DKC_TYPE_FLOAT)
      params->first->data.float_value = *(float*)value;
    else if(type == DKC_TYPE_STRING){
      params->first->data.string_value = malloc((strlen(value)+1)*sizeof(char));
      memcpy(params->first->data.string_value, value, (strlen(value)+1)*sizeof(char));
    }
    else if(type == DKC_TYPE_FRACTION)
      params->first->data.fraction_value = *(DkcFraction*)value;
    
    params->first->name = malloc((name_length+1)*sizeof(char));
    memcpy(params->first->name, name, name_length+1);
    params->first->next = NULL;
    params->nb_params++;
    return OK;
  } else {
    DkcParam* prev_param = NULL;
    DkcParam* param = params->first;
    while(param != NULL){ //Iterate over existing params
      if(strcmp(name, param->name) == 0) { //Param with this name already exists
        if(param->type == type) { //Update if types do match
          if(type == DKC_TYPE_INT)
            param->data.int_value = *(int*)value;
          else if(type == DKC_TYPE_FLOAT)
              param->data.float_value = *(float*)value;
          else if(type == DKC_TYPE_STRING) {
            free(param->data.string_value);
            param->data.string_value = malloc((strlen(value)+1)*sizeof(char));
            memcpy(param->data.string_value, value, (strlen(value)+1)*sizeof(char));
          }
          else if(type == DKC_TYPE_FRACTION)
            param->data.fraction_value = *(DkcFraction*)value;
        } else return ERROR; //Something is wrong, types are differents
        return OK;
      }
      prev_param = param;
      param=param->next;
    }
    //at this point, no existing param with this name, add it to the list.
    prev_param->next=malloc(sizeof(DkcParam));
    prev_param->next->type = type;
    prev_param->next->name = malloc((name_length+1)*sizeof(char));
    memcpy(prev_param->next->name, name, name_length+1);
    if(type == DKC_TYPE_INT)
      prev_param->next->data.int_value = *(int*)value;
    else if(type == DKC_TYPE_FLOAT)
      prev_param->next->data.float_value = *(float*)value;
    else if(type == DKC_TYPE_STRING) {
      prev_param->next->data.string_value = malloc((strlen(value)+1)*sizeof(char));
      memcpy(prev_param->next->data.string_value, value, (strlen(value)+1)*sizeof(char));
    }
    else if(type == DKC_TYPE_FRACTION)
      prev_param->next->data.fraction_value = *(DkcFraction*)value;
    
    prev_param->next->next = NULL;
    params->nb_params++;
    return OK;
  }
}

gboolean dkc_params_set_int(DkcParams *params, const char* name, int value) {
    return dkc_set_param(params, name, DKC_TYPE_INT, &value);
}

gboolean dkc_params_set_float(DkcParams *params, const char* name, float value) {
  return dkc_set_param(params, name, DKC_TYPE_FLOAT, &value);
}

gboolean dkc_params_set_string(DkcParams *params, const char* name, char* value) {
    return dkc_set_param(params, name, DKC_TYPE_STRING, value);
}

gboolean dkc_params_set_fraction(DkcParams *params, const char* name, DkcFraction value) {
    return dkc_set_param(params, name, DKC_TYPE_FRACTION, &value);
}

gboolean dkc_get_param(DkcParams *params, const char* name, DkcParamType type, void** value) {

  uint16_t name_length = strlen(name);
  
  DkcParam* param = params->first;
  while(param != NULL){ //Iterate over existing params
    if(strcmp(name, param->name) == 0) { //Param with this name already exists
      if(param->type == type) //return value if types do match
        if(type == DKC_TYPE_INT)
          **(int**)value = param->data.int_value;
        else if(type == DKC_TYPE_FLOAT)
          **(float**)value = param->data.float_value;
        else if(type == DKC_TYPE_STRING) {
          (*value) = malloc((strlen(param->data.string_value)+1)*sizeof(char));
          memcpy(*value, param->data.string_value,
                (strlen(param->data.string_value)+1)*sizeof(char));
        }
        else if(type == DKC_TYPE_FRACTION)
          **(DkcFraction**)value = param->data.fraction_value;
      else //Something is wrong, types are differents
        return ERROR;
      return OK;
    }
    param=param->next;
  }
  //at this point, no existing param with this name
  return ERROR;
}

gboolean dkc_params_get_int(DkcParams *params, const char* name, int* value) {
    return dkc_get_param(params, name, DKC_TYPE_INT, &value);
}

gboolean dkc_params_get_float(DkcParams *params, const char* name, float* value) {
    return dkc_get_param(params, name, DKC_TYPE_FLOAT, &value);
}

gboolean dkc_params_get_string(DkcParams *params, const char* name, char** value) {
    return dkc_get_param(params, name, DKC_TYPE_STRING, value);
}

gboolean dkc_params_get_fraction(DkcParams *params, const char* name, DkcFraction* value) {
    return dkc_get_param(params, name, DKC_TYPE_FRACTION, &value);
}

gboolean dkc_params_unset(DkcParams *params, const char* name) {

  if(params == NULL) return ERROR;

  DkcParam* prev_param = NULL;
  DkcParam* param = params->first;

  while(param != NULL){ //Iterate over existing params
    if(strcmp(name, param->name) == 0) { //Param with this name exists
      if(prev_param != NULL) prev_param->next=param->next;
      else params->first = param->next;

      free(param->name);
      if(param->type == DKC_TYPE_STRING)
        free(param->data.string_value);
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

gboolean dkc_params_delete(DkcParams *params) {

  if(params == NULL) return ERROR;

  DkcParam* param = params->first;
  DkcParam* rparam = NULL;

  while(param != NULL){ //Iterate over existing params

    rparam = param;
    param=param->next;

    free(rparam->name);
    if(rparam->type == DKC_TYPE_STRING)
      free(rparam->data.string_value);
    free(rparam);
    params->nb_params--;

  }

  if(params->nb_params > 0)
    return ERROR;

  free(params);
  return OK;

}

gboolean dkc_params_ref(DkcParams *params) {

  params->refs++;

}
    
gboolean dkc_params_unref(DkcParams *params) {

  params->refs--;
  if(params->refs == 0) dkc_params_delete(params);

}
