#ifndef DKCST_TYPES_H
#define DKCST_TYPES_H

typedef enum _DkcSourceType {

    DUMMY_SRC = 0,
    VIDEO_DEV_SRC,
    WINDOW_SRC,
    AUDIO_DEV_SRC,
    STREAM_SRC,
    FILE_SRC,
    NB_SRC_TYPES
    
} DkcSourceType;

typedef enum _DkcSinkType {

    DUMMY_SNK = 0,
    VIDEO_DEV_SNK,
    WINDOW_SNK,
    AUDIO_DEV_SNK,
    STREAM_SNK,
    FILE_SNK,
    NB_SNK_TYPES
    
} DkcSinkType;

#endif //DKCST_TYPES_H	
