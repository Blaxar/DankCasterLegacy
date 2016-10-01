#include <libdkcst/dkcst_source.h>
#include <stdlib.h>

/* Private context */

typedef struct {
	uint16_t width, height;
	DkCstPixFmt pix_fmt;
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

DkCstPixFmt get_pix_fmt(void* ctx) {
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

DkCst_rc alloc_video_buffer (void* ctx, void** buf, uint32_t* size) {
	uint16_t width = ((dummy_ctx*)ctx)->width;
	uint16_t height = ((dummy_ctx*)ctx)->height;
    *buf = malloc(width*height*3);
	*size = width*height*3;
	return OK;
}

DkCst_rc free_video_buffer (void* ctx, void** buf) {
	free(*buf);
	return OK;
}

DkCst_rc copy_video_data (void* ctx, void* buf) {

	uint16_t width = ((dummy_ctx*)ctx)->width;
	uint16_t height = ((dummy_ctx*)ctx)->height;
	unsigned char rgb[3] = {255, 0, 0};
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			memcpy(&buf[(y*width+x)*sizeof(rgb)], rgb, sizeof(rgb));
		}
	}

}

DkCst_rc alloc_audio_buffer (void* ctx, void** buf, uint32_t* size) {
	*buf = NULL;
	*size = 0;
	return NOT_IMPLEMENTED;
}

DkCst_rc free_audio_buffer (void* ctx, void** buf) {
	return NOT_IMPLEMENTED;
}

DkCst_rc copy_audio_data (void* ctx, void* buf) {
	return NOT_IMPLEMENTED;
}

DkCstSourceType dummy_type = {
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

DkCst_rc DkCst_source_create(DkCstSource* src, DkCstParams* params) {
	src->ctx = malloc(sizeof(dummy_ctx));
	int width, height, nb_channels, sample_rate, pix_fmt;
	DkCst_get_int_param(params, "width", &width);
	((dummy_ctx*)src->ctx)->width = width;
	DkCst_get_int_param(params, "height", &height);
	((dummy_ctx*)src->ctx)->height = height;
	DkCst_get_int_param(params, "pix_fmt", &pix_fmt);
	((dummy_ctx*)src->ctx)->pix_fmt = pix_fmt;
	DkCst_get_float_param(params, "fps", &((dummy_ctx*)src->ctx)->fps);
	DkCst_get_int_param(params, "nb_channels", &nb_channels);
	((dummy_ctx*)src->ctx)->nb_channels = nb_channels;
	DkCst_get_int_param(params, "sample_rate", &sample_rate);
	((dummy_ctx*)src->ctx)->sample_rate = sample_rate;
	return 0;
}

DkCst_rc DkCst_source_delete(DkCstSource* src) {
	free(src->ctx);
	
	return 0;
}

DkCstSourceType DkCst_source_get_type(void) {
	return dummy_type;
}
