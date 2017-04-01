#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <pthread.h>
#include <stdlib.h>

#include <libdkc/dkc_params.h>

void dkc_params_create_test(void){
    
  DkcParams *params = NULL;
  
  params = dkc_params_create();
  CU_ASSERT_NOT_EQUAL(params, NULL);
  CU_ASSERT_EQUAL(params->first, NULL);
  CU_ASSERT_EQUAL(params->nb_params, 0);
  CU_ASSERT_EQUAL(params->refs, 1);
  
}

void dkc_params_set_int_test(void){
  
  DkcParams *params = dkc_params_create();

  CU_ASSERT_EQUAL(dkc_params_set_int(params, "long long long long long long l", 42), OK);
  CU_ASSERT_EQUAL(params->nb_params,1);
  CU_ASSERT_EQUAL(params->first->data.int_value,42);
  CU_ASSERT_EQUAL(strcmp(params->first->name, "long long long long long long l"), 0);
  CU_ASSERT_EQUAL(dkc_params_set_int(params, "long long long long long long l", -43), OK);
  CU_ASSERT_EQUAL(params->first->data.int_value,-43);
  CU_ASSERT_EQUAL(strcmp(params->first->name, "long long long long long long l"), 0);
  CU_ASSERT_EQUAL(dkc_params_set_int(params, "some parameter", 3), OK);
  CU_ASSERT_EQUAL(params->nb_params,2);
  CU_ASSERT_EQUAL(params->first->next->data.int_value,3);
  CU_ASSERT_EQUAL(strcmp(params->first->next->name, "some parameter"), 0);
  CU_ASSERT_EQUAL(dkc_params_set_int(params, "some other parameter", 4), OK);
  CU_ASSERT_EQUAL(params->nb_params,3);
  CU_ASSERT_EQUAL(params->first->next->next->data.int_value,4);
  CU_ASSERT_EQUAL(strcmp(params->first->next->next->name, "some other parameter"), 0);

}

void dkc_params_set_float_test(void){

  DkcParams *params = dkc_params_create();
  
  CU_ASSERT_EQUAL(dkc_params_set_float(params, "long long long long long long l", (float)42.2), OK);
  CU_ASSERT_EQUAL(params->nb_params,1);
  CU_ASSERT_EQUAL(params->first->data.float_value, (float)42.2);
  CU_ASSERT_EQUAL(strcmp(params->first->name, "long long long long long long l"), 0);
  CU_ASSERT_EQUAL(dkc_params_set_float(params, "long long long long long long l", (float)-43.1), OK);
  CU_ASSERT_EQUAL(params->first->data.float_value, (float)-43.1);
  CU_ASSERT_EQUAL(strcmp(params->first->name, "long long long long long long l"), 0);
  CU_ASSERT_EQUAL(dkc_params_set_float(params, "some parameter", (float)3), OK);
  CU_ASSERT_EQUAL(params->nb_params,2);
  CU_ASSERT_EQUAL(params->first->next->data.float_value,(float)3);
  CU_ASSERT_EQUAL(strcmp(params->first->next->name, "some parameter"), 0);
  CU_ASSERT_EQUAL(dkc_params_set_float(params, "some other parameter", (float)4), OK);
  CU_ASSERT_EQUAL(params->nb_params,3);
  CU_ASSERT_EQUAL(params->first->next->next->data.float_value,(float)4);
  CU_ASSERT_EQUAL(strcmp(params->first->next->next->name, "some other parameter"), 0);
  
}

void dkc_params_set_string_test(void){

  DkcParams *params = dkc_params_create();
  
  CU_ASSERT_EQUAL(dkc_params_set_string(params, "long long long long long long l", "Some string here."), OK);
  CU_ASSERT_EQUAL(params->nb_params,1);
  CU_ASSERT_EQUAL(strcmp(params->first->data.string_value.str,"Some string here."), 0);
  CU_ASSERT_EQUAL(strcmp(params->first->name, "long long long long long long l"), 0);
  CU_ASSERT_EQUAL(dkc_params_set_string(params, "long long long long long long l", "Another string."), OK);
  CU_ASSERT_EQUAL(strcmp(params->first->data.string_value.str,"Another string."), 0);
  CU_ASSERT_EQUAL(strcmp(params->first->name, "long long long long long long l"), 0);
  CU_ASSERT_EQUAL(dkc_params_set_string(params, "stupid string", "The brown fox jumps over the whatever idk m8"), OK);
  CU_ASSERT_EQUAL(params->nb_params,2);
  CU_ASSERT_EQUAL(strcmp(params->first->next->data.string_value.str, "The brown fox jumps over the whatever idk m8"),0);
  CU_ASSERT_EQUAL(strcmp(params->first->next->name, "stupid string"), 0);
  CU_ASSERT_EQUAL(dkc_params_set_string(params, "really mean parameter", "You'd be a huge disappointment to your parents, good thing they are both dead."), OK);
  CU_ASSERT_EQUAL(params->nb_params,3);
  CU_ASSERT_EQUAL(strcmp(params->first->next->next->data.string_value.str,"You'd be a huge disappointment to your parents, good thing they are both dead."),0);
  CU_ASSERT_EQUAL(strcmp(params->first->next->next->name, "really mean parameter"), 0);
}

void dkc_params_get_int_test(void){

  DkcParams *params;
  params = dkc_params_create();

  dkc_params_set_int(params, "some parameter", 3);
  dkc_params_set_int(params, "some other parameter", 4);
  int value;
  CU_ASSERT_EQUAL(dkc_params_get_int(params, "some parameter", &value), OK);
  CU_ASSERT_EQUAL(value, 3);
  CU_ASSERT_EQUAL(dkc_params_get_int(params, "some other parameter", &value), OK);
  CU_ASSERT_EQUAL(value, 4);
  CU_ASSERT_EQUAL(dkc_params_get_int(params, "this one does not exist", &value), ERROR);
  
}

void dkc_params_get_float_test(void){

  DkcParams *params;
  dkc_params_create(&params);

  dkc_params_set_float(params, "some parameter", (float)3.14);
  dkc_params_set_float(params, "some other parameter", (float)4.9999);
  float value;
  CU_ASSERT_EQUAL(dkc_params_get_float(params, "some parameter", &value), OK);
  CU_ASSERT_EQUAL(value, (float)3.14);
  CU_ASSERT_EQUAL(dkc_params_get_float(params, "some other parameter", &value), OK);
  CU_ASSERT_EQUAL(value, (float)4.9999);
  CU_ASSERT_EQUAL(dkc_params_get_float(params, "this one does not exist", &value), ERROR);
  
}

void dkc_params_get_string_test(void){

  DkcParams *params;
  dkc_params_create(&params);

  dkc_params_set_string(params, "some parameter", "smok wed evryday");
  dkc_params_set_string(params, "some other parameter", "O SHIT WADDUP");
  char *value;
  CU_ASSERT_EQUAL(dkc_params_get_string(params, "some parameter", &value), OK);
  CU_ASSERT_EQUAL(strcmp("smok wed evryday", value), 0);
  CU_ASSERT_EQUAL(dkc_params_get_string(params, "some other parameter", &value), OK);
  CU_ASSERT_EQUAL(strcmp("O SHIT WADDUP", value), 0);
  CU_ASSERT_EQUAL(dkc_params_get_string(params, "this one does not exist", &value), ERROR);
    
}

void dkc_params_unset_test(void){

  DkcParams *params;
  dkc_params_create(&params);

  dkc_params_set_string(params, "some string parameter", "smok wed evryday");
  dkc_params_set_float(params, "some float parameter", (float)3.14);
  dkc_params_set_string(params, "some other string  parameter", "O SHIT WADDUP");
  dkc_params_set_int(params, "some int parameter", (int)420);
  dkc_params_set_float(params, "some other float parameter", (float)41.3);
  dkc_params_set_int(params, "some other int parameter", (int)-9999);

  float f_value;
  CU_ASSERT_EQUAL(dkc_params_unset(params, "some other float parameter"),OK);
  CU_ASSERT_EQUAL(params->nb_params,5);
  CU_ASSERT_EQUAL(dkc_params_get_float(params, "some other float parameter", &f_value), ERROR);
  CU_ASSERT_EQUAL(dkc_params_unset(params, "some other float parameter"),ERROR);

  char *s_value;
  CU_ASSERT_EQUAL(dkc_params_unset(params, "some string parameter"), OK);
  CU_ASSERT_EQUAL(params->nb_params,4);
  CU_ASSERT_EQUAL(dkc_params_get_string(params, "some string parameter", &s_value), ERROR);
  CU_ASSERT_EQUAL(dkc_params_unset(params, "some string parameter"), ERROR);

  int i_value;
  CU_ASSERT_EQUAL(dkc_params_unset(params, "some other int parameter"), OK);
  CU_ASSERT_EQUAL(params->nb_params,3);
  CU_ASSERT_EQUAL(dkc_params_get_int(params, "some other int parameter", &i_value), ERROR);
  CU_ASSERT_EQUAL(dkc_params_unset(params, "some other int parameter"), ERROR);

  CU_ASSERT_EQUAL(dkc_params_get_string(params, "some other string  parameter", &s_value), OK);
  CU_ASSERT_EQUAL(strcmp("O SHIT WADDUP", s_value), 0);

  CU_ASSERT_EQUAL(dkc_params_get_float(params, "some float parameter", &f_value), OK);
  CU_ASSERT_EQUAL(f_value, (float)3.14);

  CU_ASSERT_EQUAL(dkc_params_get_int(params, "some int parameter", &i_value), OK);
  CU_ASSERT_EQUAL(i_value, (int)420);

  CU_ASSERT_EQUAL(dkc_params_unset(params, "some other string  parameter"),OK);
  CU_ASSERT_EQUAL(dkc_params_unset(params, "some float parameter"),OK);
  CU_ASSERT_EQUAL(dkc_params_unset(params, "some int parameter"),OK);
  CU_ASSERT_EQUAL(params->nb_params, 0);
  
}

void dkc_params_delete_test(void){

  DkcParams *params;
  dkc_params_create(&params);

  dkc_params_set_string(params, "some string parameter", "smok wed evryday");
  dkc_params_set_float(params, "some float parameter", (float)3.14);
  dkc_params_set_int(params, "some int parameter", (int)420);

  CU_ASSERT_EQUAL(dkc_params_delete(params), OK);
  
}

void dkc_params_wrap_test(void){

  float f_value;
  char* s_value;
  int i_value;
  
  DkcParams* params = dkc_params_wrap("some string parameter", STRING, "smok wed evryday",
                                      "some float parameter", FLOAT, (float)3.14,
                                      "some int parameter", INT, (int)420, NULL);

  CU_ASSERT_EQUAL(dkc_params_get_string(params, "some string parameter", &s_value), OK);
  CU_ASSERT_EQUAL(strcmp("smok wed evryday", s_value), 0);

  CU_ASSERT_EQUAL(dkc_params_get_float(params, "some float parameter", &f_value), OK);
  CU_ASSERT_EQUAL(f_value, (float)3.14);

  CU_ASSERT_EQUAL(dkc_params_get_int(params, "some int parameter", &i_value), OK);
  CU_ASSERT_EQUAL(i_value, (int)420);

  dkc_params_delete(params);
  
}


void dkc_params_pop_int_test(void) {

  int value;
  DkcParams* params;
  
  params = dkc_params_create();
  dkc_params_set_int(params, "some int parameter", 420);
  
  value = dkc_params_pop_int(params, "some int parameter", 69);
  CU_ASSERT_EQUAL(value, (int)420);
  value = dkc_params_pop_int(params, "I do not exist", 69);
  CU_ASSERT_EQUAL(value, (int)69);

  dkc_params_unset(params, "some int parameter");
  dkc_params_delete(params);
  
}

void dkc_params_pop_float_test(void) {

  float value;
  DkcParams* params;

  params = dkc_params_create();
  dkc_params_set_float(params, "some float parameter", (float)3.14);
  
  value = dkc_params_pop_float(params, "some float parameter", (float)-6.9);
  CU_ASSERT_EQUAL(value, (float)3.14);
  value = dkc_params_pop_float(params, "I do not exist", (float)-6.9);
  CU_ASSERT_EQUAL(value, (float)-6.9);

  dkc_params_unset(params, "some float parameter");
  dkc_params_delete(params);
  
}

void dkc_params_pop_string_test(void) {
    
  char* value;
  DkcParams* params;

  params = dkc_params_create();
  dkc_params_set_string(params, "some string parameter", "I am a simple string therefore neither me nor any of my kind "
                                                         "are able to comprehend the concept of capitalism.");
  
  value = dkc_params_pop_string(params, "some string parameter", "Your mom is fat tho.");
  CU_ASSERT_EQUAL(strcmp(value, "I am a simple string therefore neither me nor any of my kind "
                                "are able to comprehend the concept of capitalism."), 0);
  free(value);
  value = dkc_params_pop_string(params, "I do not exist", "Your mom is fat tho.");
  CU_ASSERT_EQUAL(strcmp(value, "Your mom is fat tho."), 0);
  free(value);

  dkc_params_unset(params, "some string parameter");
  dkc_params_delete(params);
  
}

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void* run_tests(void* arg) {

  CU_basic_run_tests();
  
}

int main ( void )
{

    pthread_t tid;
    int err;

  CU_pSuite pSuite = NULL;
  
    /* initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite( "dkc_param_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */

    if( (NULL == CU_add_test(pSuite, "dkc_params_create_test", dkc_params_create_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_set_int_test", dkc_params_set_int_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_set_float_test", dkc_params_set_float_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_set_string_test", dkc_params_set_string_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_get_int_test", dkc_params_get_int_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_get_float_test", dkc_params_get_float_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_get_string_test", dkc_params_get_string_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_unset_test", dkc_params_unset_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_delete_test", dkc_params_delete_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_wrap_test", dkc_params_wrap_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_pop_int_test", dkc_params_pop_int_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_pop_float_test", dkc_params_pop_float_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_params_pop_string_test", dkc_params_pop_string_test))
    )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);

    err = pthread_create(&tid, NULL, &run_tests, NULL);
    if (err != 0) printf("\ncan't create thread :[%s]", strerror(err));

    pthread_join(tid, NULL);
  
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");
    /*
    // Run all tests using the automated interface
    CU_automated_run_tests();
    CU_list_tests_to_file();

    // Run all tests using the console interface
    CU_console_run_tests();
    */
    /* Clean up registry and return */
    CU_cleanup_registry();

    return CU_get_error();

}
