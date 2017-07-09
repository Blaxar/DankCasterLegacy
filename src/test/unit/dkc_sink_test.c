#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <libdkc/dkc.h>
#include <libdkc/dkc_sink_internal.h>

void dkc_sinkmgr_create_test(void) {
    
  DkcSinkMgr* snk_mgr = NULL;
  snk_mgr = dkc_sinkmgr_create((DkcSinkCBs){NULL,NULL,NULL});
  CU_ASSERT_NOT_EQUAL(snk_mgr, NULL);
  CU_ASSERT_EQUAL(snk_mgr->nb_sinks,0);
  for(int i=0; i<NB_SINKS; i++)
    CU_ASSERT_EQUAL(snk_mgr->sinks[i], NULL);
  
}

void dkc_sinkmgr_delete_test(void) {

  DkcSinkMgr* snk_mgr = dkc_sinkmgr_create((DkcSinkCBs){NULL,NULL,NULL});
  snk_mgr->sinks[0] = 0xdeadbeef;
  CU_ASSERT_EQUAL(dkc_sinkmgr_delete(snk_mgr), ERROR);
  snk_mgr->sinks[0] = NULL;
  CU_ASSERT_EQUAL(dkc_sinkmgr_delete(snk_mgr), OK);
  
}

void dkc_sink_create_test(void) {

  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSinkMgr *snk_mgr = app->snk_mgr;

  DkcSink* snk = NULL;
  //CU_ASSERT_EQUAL(dkc_sink_create(snk_mgr, "ymmud", &snk, "somename", NULL), ERROR); // When there is no such sink type.
  snk = dkc_sink_create(snk_mgr, DUMMY_SNK, "whatever", "somename", NULL);
  CU_ASSERT_NOT_EQUAL(snk, NULL);
  CU_ASSERT_EQUAL(snk->snk_mgr, snk_mgr);
  CU_ASSERT_EQUAL(snk_mgr->nb_sinks, 1);

  dkc_app_delete(app, NULL);
  
}

void dkc_sink_delete_test(void){

    DkcApp* app = dkc_app_create("dummy", NULL);
  DkcSinkMgr *snk_mgr = app->snk_mgr;
  
  DkcSink* snk = dkc_sink_create(snk_mgr, DUMMY_SNK, "whatever", "somename", NULL);

  uint8_t id = snk->id;

  CU_ASSERT_EQUAL(dkc_sink_delete(snk), OK);
  CU_ASSERT_EQUAL(snk_mgr->nb_sinks, 0);
  CU_ASSERT_EQUAL(snk_mgr->sinks[id], NULL);

  dkc_app_delete(app, NULL);
  
}

int init_suite(void) {
  
  if (!dkc_init()) return 1;
  
  return 0;
  
}

int clean_suite(void) {

  if (!dkc_terminate()) return 1;
  
  return 0;

}

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
    pSuite = CU_add_suite( "dkc_sink_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */

    if ((NULL == CU_add_test(pSuite, "dkc_sinkmgr_create_test", dkc_sinkmgr_create_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_sinkmgr_delete_test", dkc_sinkmgr_delete_test)) ||
    (NULL == CU_add_test(pSuite, "dkc_sink_create_test", dkc_sink_create_test)) ||
    (NULL == CU_add_test(pSuite, "dkc_sink_delete_test", dkc_sink_delete_test))
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
