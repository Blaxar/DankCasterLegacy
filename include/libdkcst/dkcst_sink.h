#ifndef DKCST_SINK_H
#define DKCST_SINK_H

#include "dkcst_app.h"
#include "dkcst_rc.h"

#define NB_SINKS 255

typedef struct _DkCstSink {

	pthread_mutex_t lock;
    struct _DkCstSinkMgr* snk_mgr;
	uint8_t id;
	char* type;
	
} DkCstSink;

typedef struct _DkCstSinkMgr {

	pthread_mutex_t lock;
	DkCstSink* sinks[NB_SINKS];
	uint8_t nb_sinks;
	
} DkCstSinkMgr;

DkCst_rc DkCst_create_sink_mgr(DkCstSinkMgr** snk_mgr);
DkCst_rc DkCst_delete_sink_mgr(DkCstSinkMgr** snk_mgr);

DkCst_rc DkCst_create_sink(DkCstSinkMgr* snk_mgr, const char* type, DkCstParams* params, DkCstSink** snk, const char* name);
DkCst_rc DkCst_delete_sink(DkCstSink**  snk);

#endif //DKCST_SINK_H	
