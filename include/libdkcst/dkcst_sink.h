#ifndef DKCST_SINK_H
#define DKCST_SINK_H

#include "dkcst_app.h"

#define NB_SINKS 255

struct DkCstSink_s;
struct DkCstSinkMgr_s;

typedef struct DkCstSink_s{

	pthread_mutex_t lock;
	struct DkCstSinkMgr_s* snk_mgr;
	uint8_t id;
	char* type;
	
} DkCstSink;

typedef struct DkCstSinkMgr_s{

	pthread_mutex_t lock;
    DkCstApp * app;
	DkCstSink* sinks[NB_SINKS];
	uint8_t nb_sinks;
	
} DkCstSinkMgr;

DkCst_rc DkCst_create_sink_mgr(DkCstApp * app, DkCstSinkMgr** snk_mgr);
DkCst_rc DkCst_delete_sink_mgr(DkCstSinkMgr** snk_mgr);

DkCst_rc DkCst_create_sink(DkCstSinkMgr* snk_mgr, const char* type, DkCstParams* params, DkCstSink** snk, const char* name);
DkCst_rc DkCst_delete_sink(DkCstSink**  snk);

#endif //DKCST_SINK_H	
