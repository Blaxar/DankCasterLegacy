#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <libdkc/dkc.h>
#include <libdkc/dkc_source.h>

void dkc_create_source_mgr_test(void) {
  
  dkc_init();
  DkcApp* app;
  dkc_create_app(&app);
  
  DkcSourceMgr* src_mgr;
  CU_ASSERT_EQUAL(dkc_create_source_mgr(&src_mgr, (DkcSourceCBs){NULL,NULL,NULL}), OK);
  CU_ASSERT_EQUAL(src_mgr->nb_sources,0);
  for(int i=0; i<NB_SOURCES; i++)
  CU_ASSERT_EQUAL(src_mgr->sources[i], NULL);

  dkc_delete_app(&app);
  dkc_terminate();
}

void dkc_delete_source_mgr_test(void) {

  dkc_init();
  DkcApp* app;
  dkc_create_app(&app);
  
  DkcSourceMgr* src_mgr;
  dkc_create_source_mgr(&src_mgr, (DkcSourceCBs){NULL,NULL,NULL});
  src_mgr->sources[0] = 0xdeadbeef;
  CU_ASSERT_EQUAL(dkc_delete_source_mgr(&src_mgr), ERROR);
  src_mgr->sources[0] = NULL;
  CU_ASSERT_EQUAL(dkc_delete_source_mgr(&src_mgr), OK);
  CU_ASSERT_NOT_EQUAL(src_mgr, NULL);

  dkc_delete_app(&app);
  dkc_terminate();
  
}

void dkc_create_source_test(void) {

  dkc_init();
  DkcApp* app;
  dkc_create_app(&app);
  
  DkcSourceMgr *src_mgr;
  dkc_create_source_mgr(&src_mgr, (DkcSourceCBs){NULL,NULL,NULL});

  DkcSource* src;
  //CU_ASSERT_EQUAL(dkc_create_source(src_mgr, "ymmud", &src, "somename", NULL), ERROR); // When there is no such source type.
  CU_ASSERT_EQUAL(dkc_create_source(src_mgr, DUMMY_SRC, "whatever", &src, "somename", NULL), OK); // When there is the source type.
  CU_ASSERT_EQUAL(src->src_mgr, src_mgr);
  CU_ASSERT_EQUAL(src_mgr->nb_sources, 1);

  dkc_delete_app(&app);
  dkc_terminate();
  
}

void dkc_delete_source_test(void){

  dkc_init();
  DkcApp* app;
  dkc_create_app(&app);
  
  DkcSourceMgr *src_mgr, *src_mgr2;
  dkc_create_source_mgr(&src_mgr, (DkcSourceCBs){NULL,NULL,NULL});
  
  DkcSource* src;
  dkc_create_source(src_mgr, DUMMY_SRC, "whatever", &src, "somename", NULL);

  uint8_t id = src->id;

  /* Good case scenario */
  CU_ASSERT_EQUAL(dkc_delete_source(&src), OK);
  CU_ASSERT_EQUAL(src_mgr->nb_sources, 0);
  CU_ASSERT_EQUAL(src_mgr->sources[id],NULL);

  dkc_delete_app(&app);
  dkc_terminate();
  
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
    pSuite = CU_add_suite( "dkc_source_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */

    if ((NULL == CU_add_test(pSuite, "dkc_create_source_mgr_test", dkc_create_source_mgr_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_delete_source_mgr_test", dkc_delete_source_mgr_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_create_source_test", dkc_create_source_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_delete_source_test", dkc_delete_source_test))
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
