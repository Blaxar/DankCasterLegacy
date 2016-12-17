#ifndef DKCST_SOURCE_H
#define DKCST_SOURCE_H

#include "dkcst_app.h"

#define NB_SOURCES 255

struct DkCstSource_s;
struct DkCstSourceMgr_s;

typedef struct DkCstSource_s{

	pthread_mutex_t lock;
	struct DkCstSourceMgr_s* src_mgr;
	uint8_t id;
	char* type;
	
} DkCstSource;

typedef struct DkCstSourceMgr_s{

	pthread_mutex_t lock;
    DkCstApp * app;
	DkCstSource* sources[NB_SOURCES];
	uint8_t nb_sources;
	
} DkCstSourceMgr;

DkCst_rc DkCst_create_source_mgr(DkCstApp * app, DkCstSourceMgr** src_mgr);
DkCst_rc DkCst_delete_source_mgr(DkCstSourceMgr** src_mgr);

DkCst_rc DkCst_create_source(DkCstSourceMgr* src_mgr, const char* type, DkCstParams* params, DkCstSource** src, const char* name);
DkCst_rc DkCst_delete_source(DkCstSource**  src);

#endif //DKCST_SOURCE_H	
