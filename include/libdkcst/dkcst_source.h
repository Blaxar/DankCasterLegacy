#ifndef DKCST_SOURCE_H
#define DKCST_SOURCE_H

#include "dkcst_app.h"
#include "dkcst_rc.h"

#define NB_SOURCES 255

typedef struct _DkCstSource {

	pthread_mutex_t lock;
	struct _DkCstSourceMgr* src_mgr;
	uint8_t id;
	char* type;
	
} DkCstSource;

typedef struct _DkCstSourceMgr {

	pthread_mutex_t lock;
    DkCstSource* sources[NB_SOURCES];
	uint8_t nb_sources;
	DKCST_SOURCE_CBS();
	
} DkCstSourceMgr;

typedef struct _DkCstSourceCBs {

	DKCST_SOURCE_CBS();
	
} DkCstSourceCBs;

DkCst_rc DkCst_create_source_mgr(DkCstSourceMgr** src_mgr, DkCstSourceCBs src_cbs);
DkCst_rc DkCst_delete_source_mgr(DkCstSourceMgr** src_mgr);

DkCst_rc DkCst_create_source(DkCstSourceMgr* src_mgr, const char* type, DkCstParams* params, DkCstSource** src, const char* name);
DkCst_rc DkCst_delete_source(DkCstSource**  src);

#endif //DKCST_SOURCE_H	
