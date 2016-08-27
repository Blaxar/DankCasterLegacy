#include <stdlib.h>
#include <stdio.h>
#include <libdkcst/dkcst_source.h>
#include <libdkcst/sources/dummy.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[]){

	printf("Initializing DkCst\n");
    DkCst_init();
	
	printf("Initialized DkCst"); 
    DkCst_source_mgr* src_mgr;
	DkCst_create_source_mgr(&src_mgr);
	printf("Created Mgr");
	struct DkCst_source_dummy_params params = {
	    .width       = 960,
		.height      = 540,
		.pix_fmt     = RGB24,
		.fps         = 30.0,
		.nb_channels = 2,
		.sample_rate = 48000,
	};
		
    DkCst_source* src = DkCst_create_source(src_mgr, "dummy", &params);
	printf("%d\n", src->id);
	printf("%f\n", src->get_fps(src->ctx));

	void* video_data;
	int size = src->alloc_video_buffer(src->ctx, &video_data);
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
	DkCst_delete_source(src_mgr, src);
	DkCst_delete_source_mgr(&src_mgr);
	DkCst_terminate();
	
	return 0; 
	
}
