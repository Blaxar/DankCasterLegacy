#ifndef DKCST_DUMMY_H
#define DKCST_DUMMY_H

#include "../dkcst_source.h"

struct DkCst_source_dummy_params {
	uint16_t width, height;
    DkCst_pix_fmt pix_fmt;
	float fps;
	uint8_t nb_channels;
	uint16_t sample_rate;
};

#endif //DKCST_DUMMY_H
