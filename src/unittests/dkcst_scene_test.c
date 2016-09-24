#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include <libdkcst/dkcst_scene.h>

DkCst_source_mgr *g_src_mgr, *g_src_mgr2;

DkCst_params* g_params;

DkCst_source *g_src, *g_src2;


void DkCst_create_scene_mgr_test(void) {

	DkCst_scene_mgr* scn_mgr;
	CU_ASSERT_EQUAL(DkCst_create_scene_mgr(g_src_mgr, &scn_mgr), OK);
	CU_ASSERT_EQUAL(scn_mgr->nb_scenes,0);
	for(int i=0; i<NB_SCENES; i++)
		CU_ASSERT_EQUAL(scn_mgr->scenes[i], NULL);

	CU_ASSERT_EQUAL(scn_mgr->src_mgr, g_src_mgr);
	
}

void DkCst_delete_scene_mgr_test(void) {

	DkCst_scene_mgr* scn_mgr;
	DkCst_create_scene_mgr(g_src_mgr, &scn_mgr);
	scn_mgr->scenes[0] = 0xdeadbeef;
	CU_ASSERT_EQUAL(DkCst_delete_scene_mgr(&scn_mgr),ERROR);
	scn_mgr->scenes[0] = NULL;
	CU_ASSERT_EQUAL(DkCst_delete_scene_mgr(&scn_mgr),OK);
	
}

void DkCst_create_scene_test(void) {

	DkCst_scene_mgr *scn_mgr, *scn_mgr2;
	DkCst_scene* scn = NULL;
	DkCst_create_scene_mgr(g_src_mgr, &scn_mgr);

	CU_ASSERT_EQUAL(DkCst_create_scene(scn_mgr, &scn),OK);
	CU_ASSERT_EQUAL(scn->scn_mgr,scn_mgr);
	CU_ASSERT_EQUAL(scn->nb_sources,0);
	for(int i=0; i<NB_WRP_SOURCES; i++)
		CU_ASSERT_EQUAL(scn->sources[i],NULL);
	
}

void DkCst_delete_scene_test(void) {

	DkCst_scene_mgr *scn_mgr, *scn_mgr2;
	DkCst_scene* scn;
	DkCst_create_scene_mgr(g_src_mgr, &scn_mgr);
    DkCst_create_scene(scn_mgr, &scn);
	uint8_t id = scn->id;

	/* When not all source slots are free*/
	scn->sources[0] = 0xdeadbeef;
	CU_ASSERT_EQUAL(DkCst_delete_scene(&scn),ERROR);

	/* Good case scenario */
	scn->sources[0] = NULL;
	CU_ASSERT_EQUAL(DkCst_delete_scene(&scn),OK);
	CU_ASSERT_EQUAL(scn_mgr->scenes[id],NULL);
	
}

void DkCst_wrap_source_test(void) {

	DkCst_scene_mgr *scn_mgr, *scn_mgr2;
	DkCst_scene *scn, *scn2;
	DkCst_wrapped_source* wrpd_src;
	DkCst_create_scene_mgr(g_src_mgr, &scn_mgr);
    DkCst_create_scene(scn_mgr, &scn);

	/* Good case scenario */
    CU_ASSERT_EQUAL(DkCst_wrap_source(scn, g_src, &wrpd_src),OK);
	CU_ASSERT_EQUAL(wrpd_src->scn, scn);
	CU_ASSERT_EQUAL(scn->nb_sources,1);
	CU_ASSERT_EQUAL(scn->sources[0],wrpd_src);	
	CU_ASSERT_EQUAL(wrpd_src->source_id,g_src->id);

	/* When the source and the scene don't belong to linked managers */
	DkCst_create_scene_mgr(g_src_mgr2, &scn_mgr2);
	DkCst_create_scene(scn_mgr2, &scn2);
	CU_ASSERT_EQUAL(DkCst_wrap_source(scn, g_src2, &wrpd_src),ERROR);
	CU_ASSERT_EQUAL(DkCst_wrap_source(scn2, g_src, &wrpd_src),ERROR);
	
}

void DkCst_unwrap_source_test(void) {

	DkCst_scene_mgr *scn_mgr, *scn_mgr2;
	DkCst_scene *scn, *scn2;
	DkCst_wrapped_source* wrpd_src;
	DkCst_create_scene_mgr(g_src_mgr, &scn_mgr);
    DkCst_create_scene(scn_mgr, &scn);

	/* Good case scenario */
	DkCst_wrap_source(scn, g_src, &wrpd_src);
    CU_ASSERT_EQUAL(DkCst_unwrap_source(&wrpd_src),OK);
	CU_ASSERT_EQUAL(scn->nb_sources,0);
	CU_ASSERT_EQUAL(scn->sources[0],NULL);
	
}


int init_suite(void) {

	if (!DkCst_rc_ok(DkCst_create_param_pack(&g_params))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(g_params, "width", 960))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(g_params, "height", 540))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(g_params, "pix_fmt", RGB24))) return 1;
	if (!DkCst_rc_ok(DkCst_set_float_param(g_params, "fps", 30.0))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(g_params, "nb_channels", 2))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(g_params, "sample_rate", 48000))) return 1;
	
	if (!DkCst_rc_ok(DkCst_init())) return 1;
	if (!DkCst_rc_ok(DkCst_create_source_mgr(&g_src_mgr))) return 1;
	if (!DkCst_rc_ok(DkCst_create_source(g_src_mgr, "dummy", g_params, &g_src))) return 1;
	if (!DkCst_rc_ok(DkCst_create_source_mgr(&g_src_mgr2))) return 1;
	if (!DkCst_rc_ok(DkCst_create_source(g_src_mgr2, "dummy", g_params, &g_src2))) return 1;
	
	return 0;
	
}
int clean_suite(void) {
	
	if (!DkCst_rc_ok(DkCst_delete_source(&g_src))) return 1;
	if (!DkCst_rc_ok(DkCst_delete_source_mgr(&g_src_mgr))) return 1;
	if (!DkCst_rc_ok(DkCst_delete_source(&g_src2))) return 1;
	if (!DkCst_rc_ok(DkCst_delete_source_mgr(&g_src_mgr2))) return 1;
	if (!DkCst_rc_ok(DkCst_terminate())) return 1;

	if (!DkCst_rc_ok(DkCst_unset_param(g_params, "width"))) return 1;
	if (!DkCst_rc_ok(DkCst_unset_param(g_params, "height"))) return 1;
	if (!DkCst_rc_ok(DkCst_unset_param(g_params, "pix_fmt"))) return 1;
	if (!DkCst_rc_ok(DkCst_unset_param(g_params, "fps"))) return 1;
	if (!DkCst_rc_ok(DkCst_unset_param(g_params, "nb_channels"))) return 1;
	if (!DkCst_rc_ok(DkCst_unset_param(g_params, "sample_rate"))) return 1;
	if (!DkCst_rc_ok(DkCst_delete_param_pack(&g_params))) return 1;
	
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
    pSuite = CU_add_suite( "dkcst_scene_test_suite", init_suite, clean_suite );
    if ( NULL == pSuite ) {
      CU_cleanup_registry();
      return CU_get_error();
    }

    /* add the tests to the suite */

    if ( (NULL == CU_add_test(pSuite, "DkCst_create_scene_mgr_test", DkCst_create_scene_mgr_test)) ||
	    (NULL == CU_add_test(pSuite, "DkCst_delete_scene_mgr_test", DkCst_delete_scene_mgr_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_create_scene_test", DkCst_create_scene_test)) ||
		(NULL == CU_add_test(pSuite, "DkCst_delete_scene_test", DkCst_delete_scene_test)) ||
	    (NULL == CU_add_test(pSuite, "DkCst_wrap_source_test", DkCst_wrap_source_test)) ||
        (NULL == CU_add_test(pSuite, "DkCst_unwrap_source_test", DkCst_unwrap_source_test))
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
