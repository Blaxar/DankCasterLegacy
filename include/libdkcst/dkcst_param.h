#ifndef DKCST_PARAM_H
#define DKCST_PARAM_H

#include <inttypes.h>
#include <stddef.h>
#include "dkcst_rc.h"

#define MAX_PARAM_NAME_LENGTH 32

struct _DkCstParam;

typedef enum {
  INT = 0,
  FLOAT,
  STRING,
} DkCstParam_type;

typedef struct _DkCstParam {

  DkCstParam_type type;
  
  char name[MAX_PARAM_NAME_LENGTH];
    
  union DkCstParam_data {
  int int_value ;
  float float_value;
  struct string_param {
      char* str;
    } string_value;
  } data;

  struct _DkCstParam* next;
  
} DkCstParam;

typedef struct _DkCstParams {

  struct _DkCstParam* first;
  uint8_t nb_params;
  
} DkCstParams;

DkCst_rc DkCst_create_param_pack(DkCstParams **params);

DkCst_rc DkCst_set_int_param(DkCstParams *params, char name[MAX_PARAM_NAME_LENGTH], int value);
DkCst_rc DkCst_set_float_param(DkCstParams *params, char name[MAX_PARAM_NAME_LENGTH], float value);
DkCst_rc DkCst_set_string_param(DkCstParams *params, char name[MAX_PARAM_NAME_LENGTH], char* value);

DkCst_rc DkCst_get_int_param(DkCstParams *params, char name[MAX_PARAM_NAME_LENGTH], int* value);
DkCst_rc DkCst_get_float_param(DkCstParams *params, char name[MAX_PARAM_NAME_LENGTH], float* value);
DkCst_rc DkCst_get_string_param(DkCstParams *params, char name[MAX_PARAM_NAME_LENGTH], char** value);

DkCst_rc DkCst_unset_param(DkCstParams *params, char name[MAX_PARAM_NAME_LENGTH]);

DkCst_rc DkCst_delete_param_pack(DkCstParams **params);

#endif //DKCST_PARAM_H	
