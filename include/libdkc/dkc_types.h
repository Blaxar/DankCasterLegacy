#ifndef DKCST_TYPES_H
#define DKCST_TYPES_H

typedef enum _DkcSourceType {

    DUMMY_SRC = 0,
    V4L2_SRC,
    X11_SRC,
    ALSA_SRC,
    RTMP_SRC,
    RTSP_SRC,
    FILE_SRC,
    
} DkcSourceType;

typedef enum _DkcSinkType {

    DUMMY_SNK = 0,
    
} DkcSinkType;

#endif //DKCST_TYPES_H	
