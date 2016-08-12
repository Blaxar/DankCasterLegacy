#include <libdkcst/dkcst_source.h>
#include <libdkcst/sources/dummy.h>
#include <stdlib.h>

/* Private context */

struct dummy_ctx {
	uint16_t width, height;
	pix_fmt_t pix_fmt;
	float fps;
	uint8_t nb_channels;
	uint16_t sample_rate;
};

/* Private methods */

uint16_t get_width(void* ctx) {
    return ((struct dummy_ctx*)ctx)->width;
}

uint16_t get_height(void* ctx) {
	return ((struct dummy_ctx*)ctx)->height;
}

pix_fmt_t get_pix_fmt(void* ctx) {
	return ((struct dummy_ctx*)ctx)->pix_fmt;
}

float get_fps(void* ctx) {
	return ((struct dummy_ctx*)ctx)->fps;
}

uint16_t get_sample_rate(void* ctx) {
	return ((struct dummy_ctx*)ctx)->sample_rate;
}

uint8_t get_nb_channels(void* ctx) {
	return ((struct dummy_ctx*)ctx)->nb_channels;
}

dkcst_source_type dummy_type = {
	.type             = "BOTH",
    .name             = "dummy",
    .get_width        = get_width,
    .get_height       = get_height,
    .get_pix_fmt      = get_pix_fmt,
    .get_fps          = get_fps,
    .get_sample_rate  = get_sample_rate,
	.get_nb_channels  = get_nb_channels,
};

/* Creation and deletion functions. */

uint8_t dkcst_source_create(dkcst_source* src, void* params) {
	struct dkcst_source_dummy_params* dummy_params = params;
	src->ctx = malloc(sizeof(struct dummy_ctx));
	((struct dummy_ctx*)src->ctx)->width = dummy_params->width;
	((struct dummy_ctx*)src->ctx)->height = dummy_params->height;
	((struct dummy_ctx*)src->ctx)->pix_fmt = dummy_params->pix_fmt;
	((struct dummy_ctx*)src->ctx)->fps = dummy_params->fps;
	((struct dummy_ctx*)src->ctx)->nb_channels = dummy_params->nb_channels;
	((struct dummy_ctx*)src->ctx)->sample_rate = dummy_params->sample_rate;
	
	return 0;
}

uint8_t dkcst_source_delete(dkcst_source* src) {
	free(src->ctx);
	
	return 0;
}

dkcst_source_type dkcst_source_get_type(void) {
	return dummy_type;
}
