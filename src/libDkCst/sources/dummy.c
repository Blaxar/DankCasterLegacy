#include <libdkcst/dkcst_source.h>
#include <libdkcst/sources/dummy.h>
#include <stdlib.h>

/* Private context */

typedef struct {
	uint16_t width, height;
	DkCst_pix_fmt pix_fmt;
	float fps;
	uint8_t nb_channels;
	uint16_t sample_rate;
} dummy_ctx;

/* Private methods */

uint16_t get_width(void* ctx) {
    return ((dummy_ctx*)ctx)->width;
}

uint16_t get_height(void* ctx) {
	return ((dummy_ctx*)ctx)->height;
}

DkCst_pix_fmt get_pix_fmt(void* ctx) {
	return ((dummy_ctx*)ctx)->pix_fmt;
}

float get_fps(void* ctx) {
	return ((dummy_ctx*)ctx)->fps;
}

uint16_t get_sample_rate(void* ctx) {
	return ((dummy_ctx*)ctx)->sample_rate;
}

uint8_t get_nb_channels(void* ctx) {
	return ((dummy_ctx*)ctx)->nb_channels;
}

uint32_t alloc_video_buffer (void* ctx, void** buf) {
	uint16_t width = ((dummy_ctx*)ctx)->width;
	uint16_t height = ((dummy_ctx*)ctx)->height;
    *buf = malloc(width*height*3);
	return width*height*3;
}

uint32_t free_video_buffer (void* ctx, void** buf) {
	free(*buf);
	return 0;
}

uint32_t copy_video_data (void* ctx, void* buf) {

	uint16_t width = ((dummy_ctx*)ctx)->width;
	uint16_t height = ((dummy_ctx*)ctx)->height;
	unsigned char rgb[3] = {255, 0, 0};
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			memcpy(&buf[(y*width+x)*sizeof(rgb)], rgb, sizeof(rgb));
		}
	}

}

uint32_t alloc_audio_buffer (void* ctx, void** buf) {
	return 0;
}

uint32_t free_audio_buffer (void* ctx, void** buf) {
	return 0;
}

uint32_t copy_audio_data (void* ctx, void* buf) {
	return 0;
}

DkCst_source_type dummy_type = {
	.type               = BOTH,
    .name               = "dummy",
    .get_width          = get_width,
    .get_height         = get_height,
    .get_pix_fmt        = get_pix_fmt,
    .get_fps            = get_fps,
    .get_sample_rate    = get_sample_rate,
	.get_nb_channels    = get_nb_channels,
	.alloc_video_buffer = alloc_video_buffer,
	.free_video_buffer  = free_video_buffer,
	.copy_video_data    = copy_video_data,
	.alloc_audio_buffer = alloc_audio_buffer,
	.free_audio_buffer  = free_audio_buffer,
	.copy_audio_data    = copy_audio_data
};

/* Creation and deletion functions. */

uint8_t DkCst_source_create(DkCst_source* src, void* params) {

	struct DkCst_source_dummy_params* dummy_params = params;
	src->ctx = malloc(sizeof(dummy_ctx));
	((dummy_ctx*)src->ctx)->width = dummy_params->width;
	((dummy_ctx*)src->ctx)->height = dummy_params->height;
	((dummy_ctx*)src->ctx)->pix_fmt = dummy_params->pix_fmt;
	((dummy_ctx*)src->ctx)->fps = dummy_params->fps;
	((dummy_ctx*)src->ctx)->nb_channels = dummy_params->nb_channels;
	((dummy_ctx*)src->ctx)->sample_rate = dummy_params->sample_rate;
	
	return 0;
}

uint8_t DkCst_source_delete(DkCst_source* src) {
	free(src->ctx);
	
	return 0;
}

DkCst_source_type DkCst_source_get_type(void) {
	return dummy_type;
}
