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
} DkcParam_type;

typedef struct _DkcParam {

  DkcParam_type type;
  
  char name[MAX_PARAM_NAME_LENGTH];
    
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

  struct _DkcParam* first;
  uint8_t nb_params;
  
} DkcParams;

dkc_rc dkc_get_params(va_list* list, ...);

dkc_rc dkc_create_param_pack(DkcParams **params);

dkc_rc dkc_set_int_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], int value);
dkc_rc dkc_set_float_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], float value);
dkc_rc dkc_set_string_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], char* value);

dkc_rc dkc_get_int_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], int* value);
dkc_rc dkc_get_float_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], float* value);
dkc_rc dkc_get_string_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH], char** value);

dkc_rc dkc_unset_param(DkcParams *params, char name[MAX_PARAM_NAME_LENGTH]);

dkc_rc dkc_delete_param_pack(DkcParams **params);

#endif //DKCST_PARAM_H	
