#include <libdkc/dkc_source_internal.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

G_DEFINE_TYPE (DkcSourceMgr, dkc_source_mgr, G_TYPE_OBJECT)
G_DEFINE_TYPE (DkcSource, dkc_source, G_TYPE_OBJECT)

enum
{
  PROP_BACKEND_CTX = 1,
  PROP_CREATE_SOURCE_FUNC,
  PROP_DELETE_SOURCE_FUNC,
  SRC_MGR_N_PROPERTIES
};

enum
{
  PROP_SOURCE_MGR_PTR = 1,
  PROP_SOURCE_ID,
  SRC_N_PROPERTIES,
};

static GParamSpec *src_mgr_properties[SRC_MGR_N_PROPERTIES] = { NULL, };
static GParamSpec *src_properties[SRC_N_PROPERTIES] = { NULL, };

/* DkcSourceMgr GObject methods */

static void
dkc_source_mgr_set_property (GObject      *object,
                            guint         property_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
  
  DkcSourceMgr *self = DKC_SOURCE_MGR (object);

  switch (property_id)
  {
      
    case PROP_BACKEND_CTX:
      self->bkn_ctx = g_value_get_pointer(value);
      break;

    case PROP_CREATE_SOURCE_FUNC:
      self->create_source = g_value_get_pointer(value);
      break;

    case PROP_DELETE_SOURCE_FUNC:
      self->delete_source = g_value_get_pointer(value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
}

static void
dkc_source_mgr_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
  DkcSourceMgr *self = DKC_SOURCE_MGR (object);

  switch (property_id)
  {
    
    case PROP_BACKEND_CTX:
      g_value_set_pointer(value, self->bkn_ctx);
      break;

    case PROP_CREATE_SOURCE_FUNC:
      g_value_set_pointer(value, self->create_source);
      break;

    case PROP_DELETE_SOURCE_FUNC:
      g_value_set_pointer(value, self->delete_source);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
}

static void
dkc_source_mgr_constructed (GObject *obj)
{

  DkcSourceMgr* src_mgr = DKC_SOURCE_MGR(obj);
  
  if(pthread_mutex_init(&src_mgr->lock, NULL) != 0){ g_error("Source Mgr creation failed."); }
  for(int i=0 ; i<NB_SOURCES; i++) {
    src_mgr->sources[i] = NULL;
  }
  src_mgr->nb_sources=0;

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->constructed(obj);

}

static void dkc_source_mgr_dispose (GObject *obj) {

  DkcSourceMgr* src_mgr = DKC_SOURCE_MGR(obj);
  pthread_mutex_destroy(&src_mgr->lock);
  
}

static void
dkc_source_mgr_class_init (DkcSourceMgrClass *klass)
{

  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = dkc_source_mgr_set_property;
  object_class->get_property = dkc_source_mgr_get_property;

  src_mgr_properties[PROP_BACKEND_CTX] =
    g_param_spec_pointer ("backend_ctx",
                         "Backend Context Pointer",
                         "Pointer to backend context to be used.",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  
  src_mgr_properties[PROP_CREATE_SOURCE_FUNC] =
    g_param_spec_pointer ("create_source_func",
                          "create_source() function",
                          "Pointer to create_source() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  src_mgr_properties[PROP_DELETE_SOURCE_FUNC] =
    g_param_spec_pointer ("delete_source_func",
                          "delete_source() function",
                          "Pointer to delete_source() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     SRC_MGR_N_PROPERTIES,
                                     src_mgr_properties);

  object_class->constructed = dkc_source_mgr_constructed;
  object_class->dispose = dkc_source_mgr_dispose;
  
}

static void
dkc_source_mgr_init (DkcSourceMgr *self)
{

}

/* DkcSource GObject methods */

static void
dkc_source_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
  
  DkcSource *self = DKC_SOURCE (object);

  switch (property_id)
  {
      
    case PROP_SOURCE_MGR_PTR:
      self->src_mgr = g_value_get_pointer(value);
      break;

    case PROP_SOURCE_ID:
      self->id = g_value_get_uchar(value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
  
}

static void
dkc_source_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
  
  DkcSource *self = DKC_SOURCE (object);

  switch (property_id)
  {
    
    case PROP_SOURCE_MGR_PTR:
      g_value_set_pointer(value, self->src_mgr);
      break;

    case PROP_SOURCE_ID:
      g_value_set_uchar(value, self->id);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
  
}

static void
dkc_source_constructed (GObject *obj)
{

  DkcSource* src = DKC_SOURCE(obj);
  
  if(pthread_mutex_init(&src->lock, NULL) != 0) g_error("Source creation failed.");

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->constructed(obj);

}

static void dkc_source_dispose (GObject *obj) {

  DkcSource* src = DKC_SOURCE(obj);
  pthread_mutex_destroy(&src->lock);

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->dispose(obj);
  
}

static void
dkc_source_class_init (DkcSourceClass *klass)
{
  
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = dkc_source_set_property;
  object_class->get_property = dkc_source_get_property;

  src_properties[PROP_SOURCE_MGR_PTR] =
    g_param_spec_pointer ("src_mgr",
                          "SourceMgr",
                          "Pointer to parent SourceMgr.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  src_properties[PROP_SOURCE_ID] =
    g_param_spec_uchar ("id",
                        "Source ID",
                        "ID of the source.",
                        0, 255, 0,
                        G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     SRC_N_PROPERTIES,
                                     src_properties);

  object_class->constructed = dkc_source_constructed;
  object_class->dispose = dkc_source_dispose;
  
}

static void
dkc_source_init (DkcSource *self)
{

}

/* Source handling */

DkcSourceMgr* dkc_sourcemgr_create(DkcSourceCBs src_cbs) {

  return g_object_new (DKC_TYPE_SOURCE_MGR,
                       "backend_ctx", src_cbs.bkn_ctx,
                       "create_source_func", src_cbs.create_source,
                       "delete_source_func", src_cbs.delete_source,
                       NULL);
  
}

dkc_rc dkc_sourcemgr_delete(DkcSourceMgr*  src_mgr) {

  for(int i=0; i<NB_SOURCES; i++) {
    if(src_mgr->sources[i] != NULL) return ERROR;
  }
  g_object_unref(src_mgr);
  return OK;
  
}


DkcSource* dkc_source_create(DkcSourceMgr* src_mgr, DkcSourceType src_type, const char* uri, const char* name, ...) {

  DkcSource* source = NULL;
  
  va_list args;
  va_start(args, name);
  source = dkc_source_vcreate(src_mgr, src_type, uri, name, args);
  va_end(args);

  return source;
  
}

DkcSource* dkc_source_vcreate(DkcSourceMgr* src_mgr, DkcSourceType src_type, const char* uri, const char* name, va_list args) {

  DkcParams* params = NULL;
  char* fname = va_arg(args, char*);
  if(fname) params = dkc_params_vwrap(fname, args);

  return dkc_source_pcreate(src_mgr, src_type, uri, name, params);
  
}

DkcSource* dkc_source_pcreate(DkcSourceMgr* src_mgr, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params) {
  
  pthread_mutex_lock(&src_mgr->lock);
  
  for(int j=0; j<NB_SOURCES; j++) {
    if(src_mgr->sources[j] == NULL) {
      src_mgr->sources[j] = g_object_new (DKC_TYPE_SOURCE, "src_mgr", src_mgr, "id", j, NULL);
      if(!src_mgr->create_source(src_mgr->bkn_ctx, j, src_type, uri, name, params)) {
        g_object_unref(src_mgr->sources[j]);
        src_mgr->sources[j] = NULL;
        pthread_mutex_unlock(&src_mgr->lock);
        return NULL;
      }
      src_mgr->nb_sources++;
      pthread_mutex_unlock(&src_mgr->lock);
      return src_mgr->sources[j];
    }
  }
  
  pthread_mutex_unlock(&src_mgr->lock);
  return NULL;
  
}

dkc_rc dkc_source_delete(DkcSource* src) {

  pthread_mutex_lock(&src->src_mgr->lock);
  
  DkcSourceMgr* src_mgr = src->src_mgr;
  uint8_t id = src->id;
  if(src_mgr->delete_source(src_mgr->bkn_ctx, id) != OK) {
    pthread_mutex_unlock(&src->src_mgr->lock);
    return ERROR;
  }
  g_object_unref(src_mgr->sources[id]);
  src_mgr->sources[id] = NULL;
  src_mgr->nb_sources--;
  
  pthread_mutex_unlock(&src->src_mgr->lock);
  return OK;
  
}

DkcSource* dkc_app_source_create(DkcApp* app, DkcSourceType src_type, const char* uri, const char* name, ...) {
  
    if(app == NULL) return NULL;

    DkcSource* source = NULL;
    
    va_list args;
    va_start(args, name);
    source = dkc_source_vcreate(app->src_mgr, src_type, uri, name, args);
    va_end(args);
    
    return source;
    
}

DkcSource* dkc_app_source_pcreate(DkcApp* app, DkcSourceType src_type, const char* uri, const char* name, DkcParams* params) {
  
    if(app == NULL) return NULL;
    
    return dkc_source_pcreate(app->src_mgr, src_type, uri, name, params);
    
}
