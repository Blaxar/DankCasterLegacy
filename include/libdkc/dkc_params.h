#ifndef DKCST_PARAMS_H
#define DKCST_PARAMS_H

#include <inttypes.h>
#include <stddef.h>
#include <stdarg.h>
#include "dkc_rc.h"

struct _DkcParam;

typedef enum {
  INT = 0, //int
  FLOAT,   //float    
  STRING,  //char*
  FRACTION,
} DkcParamType;

typedef struct _DkcFraction {
    int num, den;
} DkcFraction;

typedef struct _DkcParam {

  DkcParamType type;
  
  char* name;
    
  union DkcParam_data {
  int int_value ;
  float float_value;
  char* string_value;
    DkcFraction fraction_value;
  } data;

  struct _DkcParam* next;
  
} DkcParam;

typedef struct _DkcParams {

  int refs;
  struct _DkcParam* first;
  uint8_t nb_params;
  
} DkcParams;

typedef dkc_rc (*dkc_param_cb)(const char* name, DkcParamType type, void* value, void* ctx);

DkcParams* dkc_params_wrap(const char* name, ...);
dkc_rc dkc_params_pop_all(DkcParams *params, dkc_param_cb param_cb, void* ctx);

int dkc_params_fetch_int(DkcParams *params, const char* name, int default_value);
float dkc_params_fetch_float(DkcParams *params, const char* name, float default_value);
char* dkc_params_fetch_string(DkcParams *params, const char* name, char* default_value);
DkcFraction dkc_params_fetch_fraction(DkcParams *params, const char* name, DkcFraction value);

DkcParams* dkc_params_create();

dkc_rc dkc_params_set_int(DkcParams *params, const char* name, int value);
dkc_rc dkc_params_set_float(DkcParams *params, const char* name, float value);
dkc_rc dkc_params_set_string(DkcParams *params, const char* name, char* value);
dkc_rc dkc_params_set_fraction(DkcParams *params, const char* name, DkcFraction value);

dkc_rc dkc_params_get_int(DkcParams *params, const char* name, int* value);
dkc_rc dkc_params_get_float(DkcParams *params, const char* name, float* value);
dkc_rc dkc_params_get_string(DkcParams *params, const char* name, char** value);
dkc_rc dkc_params_get_fraction(DkcParams *params, const char* name, DkcFraction* value);

dkc_rc dkc_params_unset(DkcParams *params, const char* name);

dkc_rc dkc_params_delete(DkcParams *params);
dkc_rc dkc_params_ref(DkcParams *params);
dkc_rc dkc_params_unref(DkcParams *params);

#endif //DKCST_PARAMS_H	
