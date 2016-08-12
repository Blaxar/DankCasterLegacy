#ifndef DKCST_SCENE_H
#define DKCST_SCENE_H

#include "dkcst_source.h"

struct dkcst_wrapped_video_source_t{

    uint8_t source_id;
    uint16_t x, y;
	uint16_t width, height;

};

struct dkcst_wrapped_audio_source_t{

	uint8_t source_id;
    float volume;
	
};

struct dkcst_scene_mgr_t {

	struct dkcst_wrapped_video_source_t video_sources[];
	struct dkcst_wrapped_audio_source_t audio_sources[];
	uint8_t nb_sources;
	
};

dkcst_create_scene_mgr(dkcst_scene_mgr** scn_mgr);
dkcst_delete_scene_mgr(dkcst_scene_mgr** scn_mgr);

dkcst_create_scene(dkcst_scene_mgr* scn_mgr, );
dkcst_delete_scene(dkcst_scene_mgr* scn_mgr, );

#endif //DKCST_SCENE_H	
