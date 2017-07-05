#include <libdkc/dkc_sink_internal.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

G_DEFINE_TYPE (DkcSinkMgr, dkc_sink_mgr, G_TYPE_OBJECT)
G_DEFINE_TYPE (DkcSink, dkc_sink, G_TYPE_OBJECT)

enum
{
  PROP_BACKEND_CTX = 1,
  PROP_CREATE_SINK_FUNC,
  PROP_DELETE_SINK_FUNC,
  SNK_MGR_N_PROPERTIES
};

enum
{
  PROP_SINK_MGR_PTR = 1,
  PROP_SINK_ID,
  SNK_N_PROPERTIES,
};

static GParamSpec *snk_mgr_properties[SNK_MGR_N_PROPERTIES] = { NULL, };
static GParamSpec *snk_properties[SNK_N_PROPERTIES] = { NULL, };

/* DkcSinkMgr GObject methods */

static void
dkc_sink_mgr_set_property (GObject      *object,
                            guint         property_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
  
  DkcSinkMgr *self = DKC_SINK_MGR (object);

  switch (property_id)
  {
      
    case PROP_BACKEND_CTX:
      self->bkn_ctx = g_value_get_pointer(value);
      break;

    case PROP_CREATE_SINK_FUNC:
      self->create_sink = g_value_get_pointer(value);
      break;

    case PROP_DELETE_SINK_FUNC:
      self->delete_sink = g_value_get_pointer(value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
}

static void
dkc_sink_mgr_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
  DkcSinkMgr *self = DKC_SINK_MGR (object);

  switch (property_id)
  {
    
    case PROP_BACKEND_CTX:
      g_value_set_pointer(value, self->bkn_ctx);
      break;

    case PROP_CREATE_SINK_FUNC:
      g_value_set_pointer(value, self->create_sink);
      break;

    case PROP_DELETE_SINK_FUNC:
      g_value_set_pointer(value, self->delete_sink);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
}

static void
dkc_sink_mgr_constructed (GObject *obj)
{

  DkcSinkMgr* snk_mgr = DKC_SINK_MGR(obj);
  
  if(pthread_mutex_init(&snk_mgr->lock, NULL) != 0){ g_error("Sink Mgr creation failed."); }
  for(int i=0 ; i<NB_SINKS; i++) {
    snk_mgr->sinks[i] = NULL;
  }
  snk_mgr->nb_sinks=0;

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->constructed(obj);

}

static void dkc_sink_mgr_dispose (GObject *obj) {

  DkcSinkMgr* snk_mgr = DKC_SINK_MGR(obj);
  pthread_mutex_destroy(&snk_mgr->lock);
  
}

static void
dkc_sink_mgr_class_init (DkcSinkMgrClass *klass)
{

  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = dkc_sink_mgr_set_property;
  object_class->get_property = dkc_sink_mgr_get_property;

  snk_mgr_properties[PROP_BACKEND_CTX] =
    g_param_spec_pointer ("backend_ctx",
                         "Backend Context Pointer",
                         "Pointer to backend context to be used.",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  
  snk_mgr_properties[PROP_CREATE_SINK_FUNC] =
    g_param_spec_pointer ("create_sink_func",
                          "create_sink() function",
                          "Pointer to create_sink() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  snk_mgr_properties[PROP_DELETE_SINK_FUNC] =
    g_param_spec_pointer ("delete_sink_func",
                          "delete_sink() function",
                          "Pointer to delete_sink() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     SNK_MGR_N_PROPERTIES,
                                     snk_mgr_properties);

  object_class->constructed = dkc_sink_mgr_constructed;
  object_class->dispose = dkc_sink_mgr_dispose;
  
}

static void
dkc_sink_mgr_init (DkcSinkMgr *self)
{

}

/* DkcSink GObject methods */

static void
dkc_sink_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
  
  DkcSink *self = DKC_SINK (object);

  switch (property_id)
  {
      
    case PROP_SINK_MGR_PTR:
      self->snk_mgr = g_value_get_pointer(value);
      break;

    case PROP_SINK_ID:
      self->id = g_value_get_uchar(value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
  
}

static void
dkc_sink_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
  
  DkcSink *self = DKC_SINK (object);

  switch (property_id)
  {
    
    case PROP_SINK_MGR_PTR:
      g_value_set_pointer(value, self->snk_mgr);
      break;

    case PROP_SINK_ID:
      g_value_set_uchar(value, self->id);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
  
}

static void
dkc_sink_constructed (GObject *obj)
{

  DkcSink* snk = DKC_SINK(obj);
  
  if(pthread_mutex_init(&snk->lock, NULL) != 0) g_error("Sink creation failed.");

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->constructed(obj);

}

static void dkc_sink_dispose (GObject *obj) {

  DkcSink* snk = DKC_SINK(obj);
  pthread_mutex_destroy(&snk->lock);

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->dispose(obj);
  
}

static void
dkc_sink_class_init (DkcSinkClass *klass)
{
  
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = dkc_sink_set_property;
  object_class->get_property = dkc_sink_get_property;

  snk_properties[PROP_SINK_MGR_PTR] =
    g_param_spec_pointer ("snk_mgr",
                          "SinkMgr",
                          "Pointer to parent SinkMgr.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  snk_properties[PROP_SINK_ID] =
    g_param_spec_uchar ("id",
                        "Sink ID",
                        "ID of the sink.",
                        0, 255, 0,
                        G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     SNK_N_PROPERTIES,
                                     snk_properties);

  object_class->constructed = dkc_sink_constructed;
  object_class->dispose = dkc_sink_dispose;
  
}

static void
dkc_sink_init (DkcSink *self)
{

}

/* Sink handling */

DkcSinkMgr* dkc_sinkmgr_create(DkcSinkCBs snk_cbs) {

  return g_object_new (DKC_TYPE_SINK_MGR,
                       "backend_ctx", snk_cbs.bkn_ctx,
                       "create_sink_func", snk_cbs.create_sink,
                       "delete_sink_func", snk_cbs.delete_sink,
                       NULL);
  
}

dkc_rc dkc_sinkmgr_delete(DkcSinkMgr*  snk_mgr) {

  for(int i=0; i<NB_SINKS; i++) {
    if(snk_mgr->sinks[i] != NULL) return ERROR;
  }
  g_object_unref(snk_mgr);
  return OK;
  
}

DkcSink* vdkc_sink_create(DkcSinkMgr* snk_mgr, DkcSinkType snk_type, const char* uri, const char* name, va_list args) {

  DkcParams* params = NULL;
  char* fname = va_arg(args, char*);
  if(fname) params = vdkc_params_wrap(fname, args);

  DkcSink* snk;
  pthread_mutex_lock(&snk_mgr->lock);
  
  for(int j=0; j<NB_SINKS; j++) {
    if(snk_mgr->sinks[j] == NULL) {
      snk_mgr->sinks[j] = g_object_new (DKC_TYPE_SINK, "snk_mgr", snk_mgr, "id", j, NULL);
      if(snk_mgr->create_sink(snk_mgr->bkn_ctx, j, snk_type, uri, name, params) != OK){
        free(snk_mgr->sinks[j]);
        snk_mgr->sinks[j] = NULL;
        pthread_mutex_unlock(&snk_mgr->lock);
        return NULL;
      }
      snk_mgr->nb_sinks++;
      pthread_mutex_unlock(&snk_mgr->lock);
      return snk_mgr->sinks[j];
    }
  }

  pthread_mutex_unlock(&snk_mgr->lock);
  return NULL;
  
}

DkcSink* dkc_sink_create(DkcSinkMgr* snk_mgr, DkcSinkType snk_type, const char* uri, const char* name, ...) {

  DkcSink* sink = NULL;
  
  va_list args;
  va_start(args, name);
  sink = vdkc_sink_create(snk_mgr, snk_type, uri, name, args);
  va_end(args);

  return sink;
  
}


dkc_rc dkc_sink_delete(DkcSink* snk) {

  pthread_mutex_lock(&snk->snk_mgr->lock);
  
  DkcSinkMgr* snk_mgr = snk->snk_mgr;
  uint8_t id = snk->id;
  if(snk_mgr->delete_sink(snk_mgr->bkn_ctx, id) != OK) {
    pthread_mutex_unlock(&snk->snk_mgr->lock);
    return ERROR;
  }
  g_object_unref(snk_mgr->sinks[id]);
  snk_mgr->sinks[id] = NULL;
  snk_mgr->nb_sinks--;

  pthread_mutex_unlock(&snk->snk_mgr->lock);
  return OK;
  
}

DkcSink* dkc_app_sink_create(DkcApp* app, DkcSinkType snk_type, const char* uri, const char* name, ...) {

  if(app == NULL) return NULL;

  DkcSink* sink = NULL;
    
  va_list args;
  va_start(args, name);
  sink = vdkc_sink_create(app->snk_mgr, snk_type, uri, name, args);
  va_end(args);
  
  return sink;
  
}
