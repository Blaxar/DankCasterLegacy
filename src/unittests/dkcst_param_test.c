#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <pthread.h>

#include <libdkcst/dkcst_param.h>

void DkCst_create_param_pack_test(void){
	DkCst_params *params;
	CU_ASSERT_EQUAL(DkCst_create_param_pack(&params), OK);
	CU_ASSERT_EQUAL(params->first, NULL);
	CU_ASSERT_EQUAL(params->nb_params, 0);
}

void DkCst_set_int_param_test(void){
	
	DkCst_params *params;
	DkCst_create_param_pack(&params);

    CU_ASSERT_EQUAL(DkCst_set_int_param(params, "long long long long long long lo", 42), ERROR); //one character too far
	CU_ASSERT_EQUAL(DkCst_set_int_param(params, "long long long long long long l", 42), OK); //The maximum length
	CU_ASSERT_EQUAL(params->nb_params,1);
	CU_ASSERT_EQUAL(params->first->data.int_value,42);
	CU_ASSERT_EQUAL(DkCst_set_int_param(params, "long long long long long long l", -43), OK);
	CU_ASSERT_EQUAL(params->first->data.int_value,-43);
	CU_ASSERT_EQUAL(DkCst_set_int_param(params, "some parameter", 3), OK);
	CU_ASSERT_EQUAL(params->nb_params,2);
	CU_ASSERT_EQUAL(params->first->next->data.int_value,3);
	CU_ASSERT_EQUAL(DkCst_set_int_param(params, "some other parameter", 4), OK);
	CU_ASSERT_EQUAL(params->nb_params,3);
	CU_ASSERT_EQUAL(params->first->next->next->data.int_value,4);

}

void DkCst_set_float_param_test(void){

	DkCst_params *params;
	DkCst_create_param_pack(&params);
	
	CU_ASSERT_EQUAL(DkCst_set_float_param(params, "long long long long long long lo", (float)42.2), ERROR); //one character too far
	CU_ASSERT_EQUAL(DkCst_set_float_param(params, "long long long long long long l", (float)42.2), OK); //The maximum length
	CU_ASSERT_EQUAL(params->nb_params,1);
	CU_ASSERT_EQUAL(params->first->data.float_value, (float)42.2);
	CU_ASSERT_EQUAL(DkCst_set_float_param(params, "long long long long long long l", (float)-43.1), OK);
	CU_ASSERT_EQUAL(params->first->data.float_value, (float)-43.1);
	CU_ASSERT_EQUAL(DkCst_set_float_param(params, "some parameter", (float)3), OK);
	CU_ASSERT_EQUAL(params->nb_params,2);
	CU_ASSERT_EQUAL(params->first->next->data.float_value,(float)3);
	CU_ASSERT_EQUAL(DkCst_set_float_param(params, "some other parameter", (float)4), OK);
	CU_ASSERT_EQUAL(params->nb_params,3);
	CU_ASSERT_EQUAL(params->first->next->next->data.float_value,(float)4);
}

void DkCst_set_string_param_test(void){

	DkCst_params *params;
	DkCst_create_param_pack(&params);
	
	CU_ASSERT_EQUAL(DkCst_set_string_param(params, "long long long long long long lo", "Some string here."), ERROR); //one character too far
	CU_ASSERT_EQUAL(DkCst_set_string_param(params, "long long long long long long l", "Some string here."), OK); //The maximum length
	CU_ASSERT_EQUAL(params->nb_params,1);
	CU_ASSERT_EQUAL(strcmp(params->first->data.string_value.str,"Some string here."), 0);
	CU_ASSERT_EQUAL(DkCst_set_string_param(params, "long long long long long long l", "Another string."), OK);
	CU_ASSERT_EQUAL(strcmp(params->first->data.string_value.str,"Another string."), 0);
	CU_ASSERT_EQUAL(DkCst_set_string_param(params, "stupid string", "The brown fox jumps over the whatever idk m8"), OK);
	CU_ASSERT_EQUAL(params->nb_params,2);
	CU_ASSERT_EQUAL(strcmp(params->first->next->data.string_value.str, "The brown fox jumps over the whatever idk m8"),0);
	CU_ASSERT_EQUAL(DkCst_set_string_param(params, "really mean parameter", "You'd be a huge disappointment to your parents, good thing they are both dead."), OK);
	CU_ASSERT_EQUAL(params->nb_params,3);
	CU_ASSERT_EQUAL(strcmp(params->first->next->next->data.string_value.str,"You'd be a huge disappointment to your parents, good thing they are both dead."),0);
}

void DkCst_get_int_param_test(void){
//DkCst_rc DkCst_get_int_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], int* value);
}

void DkCst_get_float_param_test(void){
//DkCst_rc DkCst_get_float_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], float* value);
}

void DkCst_get_string_param_test(void){
//DkCst_rc DkCst_get_string_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH], char** value, uint16_t* length);
}

void DkCst_unset_param_test(void){
//DkCst_rc DkCst_unset_param(DkCst_params *params, char name[MAX_PARAM_NAME_LENGTH]);
}

void DkCst_delete_param_pack_test(void){
//DkCst_rc DkCst_delete_param_pack(DkCst_params **params);
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
    pSuite = CU_add_suite( "dkcst_param_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */

    if ( (NULL == CU_add_test(pSuite, "DkCst_create_param_pack_test", DkCst_create_param_pack_test)) ||
	    (NULL == CU_add_test(pSuite, "DkCst_set_int_param_test", DkCst_set_int_param_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_set_float_param_test", DkCst_set_float_param_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_set_string_param_test", DkCst_set_string_param_test)) ||
	    (NULL == CU_add_test(pSuite, "DkCst_get_int_param_test", DkCst_get_int_param_test)) ||
        (NULL == CU_add_test(pSuite, "DkCst_get_float_param_test", DkCst_get_float_param_test)) ||
        (NULL == CU_add_test(pSuite, "DkCst_get_string_param_test", DkCst_get_string_param_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_unset_param_test", DkCst_unset_param_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_delete_param_pack_test", DkCst_delete_param_pack_test))
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
