#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <libdkc/dkc.h>
#include <libdkc/dkc_sink.h>

void dkc_create_sink_mgr_test(void) {
  
  dkc_init();
  DkcApp* app;
  dkc_create_app(&app);
  
  DkcSinkMgr* snk_mgr;
  CU_ASSERT_EQUAL(dkc_create_sink_mgr(&snk_mgr, (DkcSinkCBs){NULL,NULL}), OK);
  CU_ASSERT_EQUAL(snk_mgr->nb_sinks,0);
  for(int i=0; i<NB_SINKS; i++)
  CU_ASSERT_EQUAL(snk_mgr->sinks[i], NULL);

  dkc_delete_app(&app);
  dkc_terminate();
}

void dkc_delete_sink_mgr_test(void) {

  dkc_init();
  DkcApp* app;
  dkc_create_app(&app);
  
  DkcSinkMgr* snk_mgr;
  dkc_create_sink_mgr(&snk_mgr, (DkcSinkCBs){NULL,NULL});
  snk_mgr->sinks[0] = 0xdeadbeef;
  CU_ASSERT_EQUAL(dkc_delete_sink_mgr(&snk_mgr), ERROR);
  snk_mgr->sinks[0] = NULL;
  CU_ASSERT_EQUAL(dkc_delete_sink_mgr(&snk_mgr), OK);
  CU_ASSERT_NOT_EQUAL(snk_mgr, NULL);

  dkc_delete_app(&app);
  dkc_terminate();
  
}

void dkc_create_sink_test(void) {

  dkc_init();
  DkcApp* app;
  dkc_create_app(&app);
  
  DkcSinkMgr *snk_mgr;
  dkc_create_sink_mgr(&snk_mgr, (DkcSinkCBs){NULL,NULL});

  DkcSink* snk;
  //CU_ASSERT_EQUAL(dkc_create_sink(snk_mgr, "ymmud", &snk, "somename", NULL), ERROR); // When there is no such sink type.
  CU_ASSERT_EQUAL(dkc_create_sink(snk_mgr, "dummy", &snk, "somename", NULL), OK); // When there is the sink type.
  CU_ASSERT_EQUAL(snk->snk_mgr, snk_mgr);
  CU_ASSERT_EQUAL(snk_mgr->nb_sinks, 1);

  dkc_delete_app(&app);
  dkc_terminate();
  
}

void dkc_delete_sink_test(void){

  dkc_init();
  DkcApp* app;
  dkc_create_app(&app);
  
  DkcSinkMgr *snk_mgr, *snk_mgr2;
  dkc_create_sink_mgr(&snk_mgr, (DkcSinkCBs){NULL,NULL});
  
  DkcSink* snk;
  dkc_create_sink(snk_mgr, "dummy", &snk, "somename", NULL);

  uint8_t id = snk->id;

  /* Good case scenario */
  CU_ASSERT_EQUAL(dkc_delete_sink(&snk), OK);
  CU_ASSERT_EQUAL(snk_mgr->nb_sinks, 0);
  CU_ASSERT_EQUAL(snk_mgr->sinks[id],NULL);

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
    pSuite = CU_add_suite( "dkc_sink_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */

    if ((NULL == CU_add_test(pSuite, "dkc_create_sink_mgr_test", dkc_create_sink_mgr_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_delete_sink_mgr_test", dkc_delete_sink_mgr_test)) ||
    (NULL == CU_add_test(pSuite, "dkc_create_sink_test", dkc_create_sink_test)) ||
    (NULL == CU_add_test(pSuite, "dkc_delete_sink_test", dkc_delete_sink_test))
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
