#include <libdkcst/dkcst_scene.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Scene handling */

DkCst_rc DkCst_create_scene_mgr(DkCst_scene_mgr** scn_mgr) {

}

DkCst_rc DkCst_delete_scene_mgr(DkCst_scene_mgr** scn_mgr) {

}

DkCst_rc DkCst_create_scene(DkCst_scene_mgr* scn_mgr, DkCst_scene** scn){

}

DkCst_rc DkCst_delete_scene(DkCst_scene_mgr* scn_mgr, DkCst_scene** scn){

}

/* Source wrapping */

DkCst_rc DkCst_wrap_source(DkCst_scene* scn,
	                   DkCst_source* src,
					   DkCst_wrapped_source** wrpd_src) {

}

DkCst_rc DkCst_unwrap_source(DkCst_scene* scn,
					     DkCst_wrapped_source** wrpd_src) {

}

