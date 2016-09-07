#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <libdkcst/dkcst_source.h>
#include <libdkcst/sources/dummy.h>

void DkCst_init_test(void){
	CU_ASSERT_EQUAL(DkCst_init(), OK);
}

void DkCst_create_source_mgr_test(void) {
	DkCst_init();
	
	DkCst_source_mgr* src_mgr;
	CU_ASSERT_EQUAL(DkCst_create_source_mgr(&src_mgr), OK);
	CU_ASSERT_EQUAL(src_mgr->nb_sources,0);
	for(int i=0; i<NB_SOURCES; i++)
		CU_ASSERT_EQUAL(src_mgr->sources[i], NULL);

	DkCst_terminate();
}

void DkCst_delete_source_mgr_test(void) {

	DkCst_init();
	
	DkCst_source_mgr* src_mgr;
    DkCst_create_source_mgr(&src_mgr);
	src_mgr->sources[0] = 0xdeadbeef;
	CU_ASSERT_EQUAL(DkCst_delete_source_mgr(&src_mgr), ERROR);
	src_mgr->sources[0] = NULL;
	CU_ASSERT_EQUAL(DkCst_delete_source_mgr(&src_mgr), OK);
	CU_ASSERT_NOT_EQUAL(src_mgr, NULL);
	
	DkCst_terminate();
	
}

void DkCst_create_source_test(void) {

	DkCst_init();
	
	DkCst_source_mgr *src_mgr;
    DkCst_create_source_mgr(&src_mgr);
	
	struct DkCst_source_dummy_params params = {
	    .width       = 960,
		.height      = 540,
		.pix_fmt     = RGB24,
		.fps         = 30.0,
		.nb_channels = 2,
		.sample_rate = 48000,
	};

    DkCst_source* src;
    CU_ASSERT_EQUAL(DkCst_create_source(src_mgr, "ymmud", &params, &src), ERROR); // When there is no such source type.
    CU_ASSERT_EQUAL(DkCst_create_source(src_mgr, "dummy", &params, &src), OK); // When there is the source type.
	CU_ASSERT_EQUAL(src->src_mgr, src_mgr);
	CU_ASSERT_EQUAL(src_mgr->nb_sources, 1);

	DkCst_terminate();
	
}

void DkCst_delete_source_test(void){

	DkCst_init();
	
	DkCst_source_mgr *src_mgr, *src_mgr2;
    DkCst_create_source_mgr(&src_mgr);
	
	struct DkCst_source_dummy_params params = {
	    .width       = 960,
		.height      = 540,
		.pix_fmt     = RGB24,
		.fps         = 30.0,
		.nb_channels = 2,
		.sample_rate = 48000,
	};
		
    DkCst_source* src;
	DkCst_create_source(src_mgr, "dummy", &params, &src);

	uint8_t id = src->id;

	/* Good case scenario */
    CU_ASSERT_EQUAL(DkCst_delete_source(src_mgr, &src), OK);
	CU_ASSERT_EQUAL(src_mgr->nb_sources, 0);
	CU_ASSERT_EQUAL(src_mgr->sources[id],NULL);

	/* When the source doesn't belong to the good manager */
	DkCst_create_source_mgr(&src_mgr2);
	CU_ASSERT_EQUAL(DkCst_delete_source(src_mgr2, &src), ERROR);

	DkCst_terminate();
	
}

void DkCst_register_source_type_test(void){

	CU_ASSERT_EQUAL(DkCst_register_source_type("dummy"), OK);
	CU_ASSERT_EQUAL(DkCst_register_source_type("ymmud"), ERROR);
	
}

void DkCst_register_all_source_types_test(void){
	CU_ASSERT_EQUAL(DkCst_register_all_source_types(), OK);
}

void DkCst_unregister_all_source_types_test(void){
	CU_ASSERT_EQUAL(DkCst_unregister_all_source_types(), OK);
}

void DkCst_ready_all_source_types_test(void){

	CU_ASSERT_EQUAL(DkCst_ready_all_source_types(), OK);
	
}

void DkCst_terminate_test(void){
	CU_ASSERT_EQUAL(DkCst_terminate(), OK);
}



int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main ( void )
{

	CU_pSuite pSuite = NULL;
	
    /* initialize the CUnit test registry */
   if ( CUE_SUCCESS != CU_initialize_registry() )
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite( "dkcst_sources_test_suite", init_suite, clean_suite );
   if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */

   if ( (NULL == CU_add_test(pSuite, "DkCst_ready_all_source_types_test", DkCst_unregister_all_source_types_test)) ||
	    (NULL == CU_add_test(pSuite, "DkCst_register_source_type_test", DkCst_register_source_type_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_register_all_source_types_test", DkCst_register_all_source_types_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_unregister_all_source_types_test", DkCst_unregister_all_source_types_test)) ||
	    (NULL == CU_add_test(pSuite, "DkCst_init_test", DkCst_init_test)) ||
        (NULL == CU_add_test(pSuite, "DkCst_create_source_mgr_test", DkCst_create_source_mgr_test)) ||
        (NULL == CU_add_test(pSuite, "DkCst_delete_source_mgr_test", DkCst_delete_source_mgr_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_create_source_test", DkCst_create_source_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_delete_source_test", DkCst_delete_source_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_terminate_test", DkCst_terminate_test))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   // Run all tests using the basic interface
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
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