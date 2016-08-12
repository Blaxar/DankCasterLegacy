#include <stdlib.h>
#include <stdio.h>
#include <libdkcst/dkcst_source.h>
#include <libdkcst/sources/dummy.h>

int main(int argc, char* argv[]){

    dkcst_init();

    dkcst_source_mgr* src_mgr;
	dkcst_create_source_mgr(&src_mgr);
	
	struct dkcst_source_dummy_params params = {
	    .width       = 1920,
		.height      = 1080,
		.pix_fmt     = 0,
		.fps         = 30.0,
		.nb_channels = 2,
		.sample_rate = 48000,
	};
		
    dkcst_source* src = dkcst_create_source(&src_mgr, "dummy", &params);
	printf("%f\n", src->get_fps(src->ctx));

	dkcst_delete_source_mgr(&src_mgr);
	dkcst_terminate();
	
	return 0;
	
}
