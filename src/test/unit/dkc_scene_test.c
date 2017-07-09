#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <libdkc/dkc.h>
#include <libdkc/dkc_scene_internal.h>
#include <libdkc/dkc_source_internal.h>

void dkc_scenemgr_create_test(void) {

  DkcSceneMgr* scn_mgr = NULL;
  scn_mgr = dkc_scenemgr_create((DkcSceneCBs){NULL,NULL,NULL,NULL,NULL});
  
  CU_ASSERT_NOT_EQUAL(scn_mgr, NULL);
  CU_ASSERT_EQUAL(scn_mgr->nb_scenes, 0);
  for(int i=0; i<NB_SCENES; i++)
    CU_ASSERT_EQUAL(scn_mgr->scenes[i], NULL);
  
}

void dkc_scenemgr_delete_test(void) {

  DkcSceneMgr* scn_mgr = dkc_scenemgr_create((DkcSceneCBs){NULL,NULL,NULL,NULL,NULL});
  
  scn_mgr->scenes[0] = 0xdeadbeef;
  CU_ASSERT_EQUAL(dkc_scenemgr_delete(scn_mgr), ERROR);
  scn_mgr->scenes[0] = NULL;
  CU_ASSERT_EQUAL(dkc_scenemgr_delete(scn_mgr), OK);
  
}

void dkc_scene_create_test(void) {

  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSceneMgr *scn_mgr = app->scn_mgr;
  
  DkcScene* scn = NULL;
  scn = dkc_scene_create(scn_mgr);
  CU_ASSERT_NOT_EQUAL(scn, NULL);
  CU_ASSERT_EQUAL(scn->scn_mgr, scn_mgr);
  CU_ASSERT_EQUAL(scn->nb_sources, 0);
  for(int i=0; i<NB_WRP_SOURCES; i++)
    CU_ASSERT_EQUAL(scn->sources[i],NULL);

  dkc_app_delete(app, NULL);
  
}

void dkc_scene_delete_test(void) {

  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSceneMgr *scn_mgr = app->scn_mgr;
    
  DkcScene* scn = dkc_scene_create(scn_mgr);
  uint8_t id = scn->id;

  /* When not all source slots are free*/
  scn->sources[0] = 0xdeadbeef;
  CU_ASSERT_EQUAL(dkc_scene_delete(scn), ERROR);

  /* Good case scenario */
  scn->sources[0] = NULL;
  CU_ASSERT_EQUAL(dkc_scene_delete(scn), OK);
  CU_ASSERT_EQUAL(scn_mgr->scenes[id], NULL);

  dkc_app_delete(app, NULL);
  
}

void dkc_source_wrap_test(void) {

  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSceneMgr *scn_mgr = app->scn_mgr;
  DkcScene *scn = dkc_scene_create(scn_mgr);
  DkcSource* src = dkc_source_create(app->src_mgr, DUMMY_SRC, "whatever", "dummy", NULL);
  DkcWrappedSource* wrpd_src = NULL;

  /* Good case scenario */
  wrpd_src = dkc_source_wrap(scn, src);
  CU_ASSERT_NOT_EQUAL(wrpd_src, NULL);
  CU_ASSERT_EQUAL(wrpd_src->scn, scn);
  CU_ASSERT_EQUAL(scn->nb_sources, 1);
  CU_ASSERT_EQUAL(scn->sources[0], wrpd_src);  
  CU_ASSERT_EQUAL(wrpd_src->source_id, src->id);

  dkc_app_delete(app, NULL);
  
}

void dkc_source_unwrap_test(void) {

  DkcApp* app = dkc_app_create("dummy", NULL, NULL);
  DkcSceneMgr *scn_mgr = app->scn_mgr;
  DkcScene *scn = dkc_scene_create(scn_mgr);
  DkcSource* src = dkc_source_create(app->src_mgr, DUMMY_SRC, "whatever", "dummy", NULL);
  DkcWrappedSource* wrpd_src = dkc_source_wrap(scn, src);

  CU_ASSERT_EQUAL(dkc_source_unwrap(wrpd_src), OK);
  CU_ASSERT_EQUAL(scn->nb_sources, 0);
  CU_ASSERT_EQUAL(scn->sources[0], NULL);

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
