#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <libdkc/dkc.h>
#include <libdkc/dkc_scene_internal.h>
#include <libdkc/dkc_source_internal.h>

void dkc_scenemgr_create_test(void) {
  
  DkcSceneMgr* scn_mgr = NULL;
  scn_mgr = dkc_scenemgr_create((DkcSceneCBs){NULL,NULL,NULL,NULL,NULL}, NULL);
  
  CU_ASSERT_NOT_EQUAL(scn_mgr, NULL);
  CU_ASSERT_EQUAL(scn_mgr->nb_scenes, 0);
  for(int i=0; i<NB_SCENES; i++)
    CU_ASSERT_EQUAL(scn_mgr->scenes[i], NULL);
  
}

void dkc_scenemgr_delete_test(void) {

  GError* gerr = NULL;
  
  DkcSceneMgr* scn_mgr = dkc_scenemgr_create((DkcSceneCBs){NULL,NULL,NULL,NULL,NULL}, NULL);
  
  scn_mgr->scenes[0] = 0xdeadbeef;
  CU_ASSERT_EQUAL(dkc_scenemgr_delete(scn_mgr, &gerr), ERROR);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_INVALID_MGR_STATE);
  }
  g_clear_error(&gerr);
  
  scn_mgr->scenes[0] = NULL;
  CU_ASSERT_EQUAL(dkc_scenemgr_delete(scn_mgr, &gerr), OK);
  CU_ASSERT_EQUAL(gerr, NULL);
  
  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  
  CU_ASSERT_EQUAL(dkc_scenemgr_delete(app, &gerr), ERROR);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_WRONG_MGR_CLASS);
  }
  g_clear_error(&gerr);
  
  dkc_app_delete(app, NULL);
  
}

void dkc_scene_create_test(void) {

  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSceneMgr *scn_mgr = app->scn_mgr;
  GError* gerr = NULL;
  
  DkcScene* scn = NULL;
  for(int i=0; i<NB_SCENES; i++) { // Create the maximum number of scenes allowed
    scn = dkc_scene_create(scn_mgr, &gerr);
    CU_ASSERT_NOT_EQUAL(scn, NULL);
    CU_ASSERT_EQUAL(scn->scn_mgr, scn_mgr);
    CU_ASSERT_EQUAL(scn_mgr->nb_scenes, i+1);
    CU_ASSERT_EQUAL(gerr, NULL);
  }

  // Now we should get an over capacity error
  scn = dkc_scene_create(scn_mgr, &gerr);
  CU_ASSERT_EQUAL(scn, NULL);
  CU_ASSERT_EQUAL(scn_mgr->nb_scenes, NB_SCENES);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_MAX_CAPACITY);
  }
  g_clear_error(&gerr);

  /* Wrong object scenario */
  CU_ASSERT_EQUAL(dkc_scene_create(app, &gerr), NULL);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_WRONG_MGR_CLASS);
  }
  g_clear_error(&gerr);

  dkc_app_delete(app, NULL);
  
}

void dkc_scene_delete_test(void) {

  GError* gerr;
  
  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSceneMgr *scn_mgr = app->scn_mgr;
  
  DkcScene* scn = dkc_scene_create(scn_mgr, NULL);
  uint8_t id = scn->id;

  /* When not all source slots are free */
  scn->sources[0] = 0xdeadbeef;
  CU_ASSERT_EQUAL(dkc_scene_delete(scn, &gerr), ERROR);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_INVALID_STATE);
  }
  g_clear_error(&gerr);

  /* Good case scenario */
  scn->sources[0] = NULL;
  CU_ASSERT_EQUAL(dkc_scene_delete(scn, &gerr), OK);
  CU_ASSERT_EQUAL(scn_mgr->scenes[id], NULL);
  CU_ASSERT_EQUAL(gerr, NULL);

  /* Wrong object scenario */
  CU_ASSERT_EQUAL(dkc_scene_delete(scn_mgr, &gerr), ERROR);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_WRONG_CLASS);
  }
  g_clear_error(&gerr);

  dkc_app_delete(app, NULL);
  
}

void dkc_source_wrap_test(void) {

  GError* gerr = NULL;
  
  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSceneMgr *scn_mgr = app->scn_mgr;
  DkcScene *scn = dkc_scene_create(scn_mgr, NULL);
  DkcSource* src = dkc_source_create(app->src_mgr, DUMMY_SRC, "whatever", "dummy", NULL, NULL);

  /* Good case scenario */
  DkcWrappedSource* wrpd_src = NULL;
  for(int i=0; i<NB_WRP_SOURCES; i++) { // Wrap the maximum number of sources allowed
    wrpd_src = dkc_source_wrap(scn, src, &gerr, NULL);
    CU_ASSERT_NOT_EQUAL(wrpd_src, NULL);
    CU_ASSERT_EQUAL(wrpd_src->scn, scn);
    CU_ASSERT_EQUAL(scn->nb_sources, i+1);
    CU_ASSERT_EQUAL(scn->sources[i], wrpd_src);  
    CU_ASSERT_EQUAL(wrpd_src->source_id, src->id);
    CU_ASSERT_EQUAL(gerr, NULL);
  }

  // Now we should get an over capacity error
  wrpd_src = dkc_source_wrap(scn, src, &gerr, NULL);
  CU_ASSERT_EQUAL(wrpd_src, NULL);
  CU_ASSERT_EQUAL(scn->nb_sources, NB_WRP_SOURCES);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_MAX_CAPACITY);
  }
  g_clear_error(&gerr);
  
  /* 1st Wrong object scenario */
  wrpd_src = dkc_source_wrap(scn_mgr, src, &gerr, NULL);
  CU_ASSERT_EQUAL(wrpd_src, NULL);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_WRONG_CLASS);
  }
  g_clear_error(&gerr);

  /* 2nd Wrong object scenario */
  wrpd_src = dkc_source_wrap(scn, scn_mgr, &gerr, NULL);
  CU_ASSERT_EQUAL(wrpd_src, NULL);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_WRONG_CLASS);
  }
  g_clear_error(&gerr);

  dkc_app_delete(app, NULL);
  
}

void dkc_source_unwrap_test(void) {

  GError* gerr = NULL;
  
  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSceneMgr *scn_mgr = app->scn_mgr;
  DkcScene *scn = dkc_scene_create(scn_mgr, NULL);
  DkcSource* src = dkc_source_create(app->src_mgr, DUMMY_SRC, "whatever", "dummy", NULL, NULL);
  DkcWrappedSource* wrpd_src = dkc_source_wrap(scn, src, NULL, NULL);

  /* Good case scenario */
  CU_ASSERT_EQUAL(dkc_source_unwrap(&wrpd_src, &gerr), OK);
  CU_ASSERT_EQUAL(wrpd_src, NULL);
  CU_ASSERT_EQUAL(scn->nb_sources, 0);
  CU_ASSERT_EQUAL(scn->sources[0], NULL);
  CU_ASSERT_EQUAL(gerr, NULL);

  CU_ASSERT_EQUAL(dkc_source_unwrap(&scn_mgr, &gerr), ERROR);
  CU_ASSERT_NOT_EQUAL(scn_mgr, NULL);
  CU_ASSERT_NOT_EQUAL(gerr, NULL);
  if(gerr) {
    CU_ASSERT_EQUAL(gerr->domain, ERRD_SCENE);
    CU_ASSERT_EQUAL(gerr->code, ERRC_WRONG_CLASS);
  }
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
    pSuite = CU_add_suite( "dkc_scene_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */

    if( (NULL == CU_add_test(pSuite, "dkc_scenemgr_create_test", dkc_scenemgr_create_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_scenemgr_delete_test", dkc_scenemgr_delete_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_scene_create_test", dkc_scene_create_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_scene_delete_test", dkc_scene_delete_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_source_wrap_test", dkc_source_wrap_test)) ||
        (NULL == CU_add_test(pSuite, "dkc_source_unwrap_test", dkc_source_unwrap_test))
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
