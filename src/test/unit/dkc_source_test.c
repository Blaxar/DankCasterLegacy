#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <libdkc/dkc.h>
#include <libdkc/dkc_source_internal.h>

void dkc_sourcemgr_create_test(void) {
  
  DkcSourceMgr* src_mgr = dkc_sourcemgr_create((DkcSourceCBs){NULL,NULL,NULL}, NULL);
  CU_ASSERT_NOT_EQUAL(src_mgr, NULL);
  CU_ASSERT_EQUAL(src_mgr->nb_sources, 0);
  for(int i=0; i<NB_SOURCES; i++)
    CU_ASSERT_EQUAL(src_mgr->sources[i], NULL);
  
}

void dkc_sourcemgr_delete_test(void) {

  GError* gerr = NULL;
  
  DkcSourceMgr* src_mgr = dkc_sourcemgr_create((DkcSourceCBs){NULL,NULL,NULL}, NULL);
  src_mgr->sources[0] = 0xdeadbeef;
  CU_ASSERT_EQUAL(dkc_sourcemgr_delete(src_mgr, &gerr), ERROR);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  CU_ASSERT_EQUAL(gerr->domain, ERRD_SOURCE);
  CU_ASSERT_EQUAL(gerr->code, ERRC_INVALID_MGR_STATE);
  g_clear_error(&gerr);
  
  src_mgr->sources[0] = NULL;
  CU_ASSERT_EQUAL(dkc_sourcemgr_delete(src_mgr, NULL), OK);
  CU_ASSERT_EQUAL(gerr, NULL);

  DkcApp* app = dkc_app_create("dummy", NULL, NULL);

  CU_ASSERT_EQUAL(dkc_sourcemgr_delete(app, &gerr), ERROR);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  CU_ASSERT_EQUAL(gerr->domain, ERRD_SOURCE);
  CU_ASSERT_EQUAL(gerr->code, ERRC_WRONG_MGR_CLASS);
  
  g_clear_error(&gerr);

  dkc_app_delete(app, NULL);
    
}

void dkc_source_create_test(void) {

  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSourceMgr *src_mgr = app->src_mgr;
  GError* gerr = NULL;

  DkcSource* src = NULL;
  for(int i=0; i<NB_SOURCES; i++) { // Create the maximum number of sources allowed
    src = dkc_source_create(src_mgr, DUMMY_SRC, "whatever", "somename", &gerr, NULL);
    CU_ASSERT_NOT_EQUAL(src, NULL);
    CU_ASSERT_EQUAL(src->src_mgr, src_mgr);
    CU_ASSERT_EQUAL(src_mgr->nb_sources, i+1);
    CU_ASSERT_EQUAL(gerr, NULL);
  }

  //Now we should get an over capacity error
  src = dkc_source_create(src_mgr, DUMMY_SRC, "whatever", "somename", &gerr, NULL);
  CU_ASSERT_EQUAL(src, NULL);
  CU_ASSERT_EQUAL(src_mgr->nb_sources, NB_SOURCES);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  CU_ASSERT_EQUAL(gerr->domain, ERRD_SOURCE);
  CU_ASSERT_EQUAL(gerr->code, ERRC_MAX_CAPACITY);
  
  g_clear_error(&gerr);
  
  dkc_app_delete(app, NULL);
  
}

void dkc_source_delete_test(void){

  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSourceMgr *src_mgr = app->src_mgr;
  GError* gerr = NULL;
  
  DkcSource* src = dkc_source_create(src_mgr, DUMMY_SRC, "whatever", "somename", &gerr, NULL);

  uint8_t id = src->id;

  CU_ASSERT_EQUAL(dkc_source_delete(src, &gerr), OK);
  CU_ASSERT_EQUAL(gerr, NULL);
  CU_ASSERT_EQUAL(src_mgr->nb_sources, 0);
  CU_ASSERT_EQUAL(src_mgr->sources[id], NULL);

  CU_ASSERT_EQUAL(dkc_source_delete(src_mgr, &gerr), ERROR);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  CU_ASSERT_EQUAL(gerr->domain, ERRD_SOURCE);
  CU_ASSERT_EQUAL(gerr->code, ERRC_WRONG_CLASS);

  g_clear_error(&gerr);

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
    pSuite = CU_add_suite( "dkc_source_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */

    if ((NULL == CU_add_test(pSuite, "dkc_sourcemgr_create_test", dkc_sourcemgr_create_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_sourcemgr_delete_test", dkc_sourcemgr_delete_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_source_create_test", dkc_source_create_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_source_delete_test", dkc_source_delete_test))
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
