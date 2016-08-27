#include <libdkcst/dkcst_scene.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Scene handling */

void DkCst_create_scene_mgr(DkCst_scene_mgr** scn_mgr) {

}

void DkCst_delete_scene_mgr(DkCst_scene_mgr** scn_mgr) {

}

void DkCst_create_scene(DkCst_scene_mgr* scn_mgr, DkCst_scene** scn){

}

void DkCst_delete_scene(DkCst_scene_mgr* scn_mgr, DkCst_scene** scn){

}

/* Source wrapping */

void DkCst_wrap_source(DkCst_scene* scn,
	                   DkCst_source* src,
					   DkCst_wrapped_source** wrpd_src) {

}

void DkCst_unwrap_source(DkCst_scene* scn,
					     DkCst_wrapped_source** wrpd_src) {

}
