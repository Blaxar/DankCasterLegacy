#include <stdlib.h>
#include <stdio.h>
#include <libdkcst/dkcst_source.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){

	printf("Initializing DkCst\n");
    DkCst_init();
	
	printf("Initialized DkCst\n"); 
    DkCstSourceMgr* src_mgr;
	DkCst_create_source_mgr(&src_mgr);
	printf("Created Mgr\n");

	DkCstParams* params;
	if (!DkCst_rc_ok(DkCst_create_param_pack(&params))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(params, "width", 960))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(params, "height", 540))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(params, "pix_fmt", RGB24))) return 1;
	if (!DkCst_rc_ok(DkCst_set_float_param(params, "fps", 30.0))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(params, "nb_channels", 2))) return 1;
	if (!DkCst_rc_ok(DkCst_set_int_param(params, "sample_rate", 48000))) return 1;
		
    DkCstSource* src;
	if(! DkCst_rc_ok(DkCst_create_source(src_mgr, "dummy", params, &src))) {
		printf("Source not created\n");
		return 1;
	}
	printf("%d\n", src->id);
	printf("%f\n", src->get_fps(src->ctx));

	void* video_data;
	int size;
	src->alloc_video_buffer(src->ctx, &video_data, &size);
	printf("%d\n", size);
	src->copy_video_data(src->ctx, video_data);
	
	void* nullptr = NULL;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {

		printf("SDL_Init Error: %s\n", SDL_GetError());
		return 1;
		
	}

	SDL_Window *win = SDL_CreateWindow("Hello World", 100, 100, 960, 540, SDL_WINDOW_SHOWN);
	if(win == nullptr) {
		printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ren == nullptr) {
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		return 1;
	}

	unsigned char* bytes = nullptr;
	SDL_Texture *tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 960, 540);
	int pitch = 0;
	SDL_LockTexture(tex, nullptr, (void**) &bytes, &pitch);
	memcpy(bytes, video_data, size);
	SDL_UnlockTexture(tex);

	for (int i=0; i < 20; i++) {
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, tex, NULL, NULL);
        SDL_RenderPresent(ren);
        SDL_Delay(100);
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	src->free_video_buffer(src->ctx, &video_data);
	DkCst_delete_source(&src);
	
    DkCst_unset_param(params, "width");
	DkCst_unset_param(params, "height");
	DkCst_unset_param(params, "pix_fmt");
	DkCst_unset_param(params, "fps");
	DkCst_unset_param(params, "nb_channels");
    DkCst_unset_param(params, "sample_rate");
	DkCst_delete_param_pack(&params);
	
	DkCst_delete_source_mgr(&src_mgr);
	DkCst_terminate();
	
	return 0; 
	
}
