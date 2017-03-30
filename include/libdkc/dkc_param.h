#ifndef DKCST_PARAM_H
#define DKCST_PARAM_H

#include <inttypes.h>
#include <stddef.h>
#include <stdarg.h>
#include "dkc_rc.h"

#define MAX_PARAM_NAME_LENGTH 32

struct _DkcParam;

typedef enum {
  INT = 0, //int
  FLOAT,   //float    
  STRING,  //char*
} DkcParamType;

typedef struct _DkcParam {

  DkcParamType type;
  
  char* name;
    
  union DkcParam_data {
  int int_value ;
  float float_value;
  struct string_param {
      char* str;
    } string_value;
  } data;

  struct _DkcParam* next;
  
} DkcParam;

typedef struct _DkcParams {

  int refs;
  struct _DkcParam* first;
  uint8_t nb_params;
  
} DkcParams;

typedef dkc_rc (*dkc_param_cb)(const char* name, DkcParamType type, void* value, void* ctx);

DkcParams* dkc_wrap_params(const char* name, ...);
dkc_rc dkc_pop_every_param(DkcParams *params, dkc_param_cb param_cb, void* ctx);

int dkc_pop_int_param(DkcParams *params, const char* name, int default_value);
float dkc_pop_float_param(DkcParams *params, const char* name, float default_value);
char* dkc_pop_string_param(DkcParams *params, const char* name, char* default_value);

DkcParams* dkc_create_param_pack();

dkc_rc dkc_set_int_param(DkcParams *params, const char* name, int value);
dkc_rc dkc_set_float_param(DkcParams *params, const char* name, float value);
dkc_rc dkc_set_string_param(DkcParams *params, const char* name, char* value);

dkc_rc dkc_get_int_param(DkcParams *params, const char* name, int* value);
dkc_rc dkc_get_float_param(DkcParams *params, const char* name, float* value);
dkc_rc dkc_get_string_param(DkcParams *params, const char* name, char** value);

dkc_rc dkc_unset_param(DkcParams *params, const char* name);

dkc_rc dkc_delete_param_pack(DkcParams *params);
dkc_rc dkc_params_ref(DkcParams *params);
dkc_rc dkc_params_unref(DkcParams *params);

#endif //DKCST_PARAM_H	
