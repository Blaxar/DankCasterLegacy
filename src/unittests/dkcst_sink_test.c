#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <libdkcst/dkcst.h>
#include <libdkcst/dkcst_sink.h>

void DkCst_create_sink_mgr_test(void) {
  
  DkCst_init();
  DkCstApp* app;
  DkCst_create_app(&app);
  
  DkCstSinkMgr* snk_mgr;
  CU_ASSERT_EQUAL(DkCst_create_sink_mgr(&snk_mgr, (DkCstSinkCBs){NULL,NULL}), OK);
  CU_ASSERT_EQUAL(snk_mgr->nb_sinks,0);
  for(int i=0; i<NB_SINKS; i++)
  CU_ASSERT_EQUAL(snk_mgr->sinks[i], NULL);

  DkCst_delete_app(&app);
  DkCst_terminate();
}

void DkCst_delete_sink_mgr_test(void) {

  DkCst_init();
  DkCstApp* app;
  DkCst_create_app(&app);
  
  DkCstSinkMgr* snk_mgr;
  DkCst_create_sink_mgr(&snk_mgr, (DkCstSinkCBs){NULL,NULL});
  snk_mgr->sinks[0] = 0xdeadbeef;
  CU_ASSERT_EQUAL(DkCst_delete_sink_mgr(&snk_mgr), ERROR);
  snk_mgr->sinks[0] = NULL;
  CU_ASSERT_EQUAL(DkCst_delete_sink_mgr(&snk_mgr), OK);
  CU_ASSERT_NOT_EQUAL(snk_mgr, NULL);

  DkCst_delete_app(&app);
  DkCst_terminate();
  
}

void DkCst_create_sink_test(void) {

  DkCst_init();
  DkCstApp* app;
  DkCst_create_app(&app);
  
  DkCstSinkMgr *snk_mgr;
  DkCst_create_sink_mgr(&snk_mgr, (DkCstSinkCBs){NULL,NULL});

  DkCstParams* params;
  DkCst_create_param_pack(&params);
  DkCst_set_int_param(params, "width", 960);
  DkCst_set_int_param(params, "height", 540);
  DkCst_set_int_param(params, "pix_fmt", 2);
  DkCst_set_float_param(params, "fps", 30.0);
  DkCst_set_int_param(params, "nb_channels", 2);
  DkCst_set_int_param(params, "sample_rate", 48000);

  DkCstSink* snk;
  //CU_ASSERT_EQUAL(DkCst_create_sink(snk_mgr, "ymmud", params, &snk, "somename"), ERROR); // When there is no such sink type.
  CU_ASSERT_EQUAL(DkCst_create_sink(snk_mgr, "dummy", params, &snk, "somename"), OK); // When there is the sink type.
  CU_ASSERT_EQUAL(snk->snk_mgr, snk_mgr);
  CU_ASSERT_EQUAL(snk_mgr->nb_sinks, 1);

  DkCst_delete_app(&app);
  DkCst_terminate();
  
}

void DkCst_delete_sink_test(void){

  DkCst_init();
  DkCstApp* app;
  DkCst_create_app(&app);
  
  DkCstSinkMgr *snk_mgr, *snk_mgr2;
  DkCst_create_sink_mgr(&snk_mgr, (DkCstSinkCBs){NULL,NULL});
  
  DkCstParams* params;
  DkCst_create_param_pack(&params);
  DkCst_set_int_param(params, "width", 960);
  DkCst_set_int_param(params, "height", 540);
  DkCst_set_int_param(params, "pix_fmt", 2);
  DkCst_set_float_param(params, "fps", 30.0);
  DkCst_set_int_param(params, "nb_channels", 2);
  DkCst_set_int_param(params, "sample_rate", 48000);
  
  DkCstSink* snk;
  DkCst_create_sink(snk_mgr, "dummy", params, &snk, "somename");

  uint8_t id = snk->id;

  /* Good case scenario */
  CU_ASSERT_EQUAL(DkCst_delete_sink(&snk), OK);
  CU_ASSERT_EQUAL(snk_mgr->nb_sinks, 0);
  CU_ASSERT_EQUAL(snk_mgr->sinks[id],NULL);

  DkCst_delete_app(&app);
  DkCst_terminate();
  
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
    pSuite = CU_add_suite( "dkcst_sink_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */

    if ((NULL == CU_add_test(pSuite, "DkCst_create_sink_mgr_test", DkCst_create_sink_mgr_test)) ||
        (NULL == CU_add_test(pSuite, "DkCst_delete_sink_mgr_test", DkCst_delete_sink_mgr_test)) ||
    (NULL == CU_add_test(pSuite, "DkCst_create_sink_test", DkCst_create_sink_test)) ||
    (NULL == CU_add_test(pSuite, "DkCst_delete_sink_test", DkCst_delete_sink_test))
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
