#include <libdkc/dkc_scene_internal.h>
#include <libdkc/dkc_source_internal.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

G_DEFINE_TYPE (DkcSceneMgr, dkc_scene_mgr, G_TYPE_OBJECT)
G_DEFINE_TYPE (DkcScene, dkc_scene, G_TYPE_OBJECT)
G_DEFINE_TYPE (DkcWrappedSource, dkc_wrapped_source, G_TYPE_OBJECT)

enum
{
  PROP_BACKEND_CTX = 1,
  PROP_CREATE_SCENE_FUNC,
  PROP_DELETE_SCENE_FUNC,
  PROP_WRAP_SOURCE_FUNC,
  PROP_UNWRAP_SOURCE_FUNC,
  SCN_MGR_N_PROPERTIES
};

enum
{
  PROP_SCENE_MGR_PTR = 1,
  PROP_SCENE_ID,
  SCN_N_PROPERTIES,
};

enum
{
  PROP_SCENE_PTR = 1,
  PROP_WRPD_SOURCE_ID,
  PROP_SOURCE_ID,
  PROP_X, PROP_Y,
  PROP_WIDTH, PROP_HEIGHT,
  PROP_VOLUME,
  WRPD_SRC_N_PROPERTIES
};

static GParamSpec *scn_mgr_properties[SCN_MGR_N_PROPERTIES] = { NULL, };
static GParamSpec *scn_properties[SCN_N_PROPERTIES] = { NULL, };
static GParamSpec *wrpd_src_properties[WRPD_SRC_N_PROPERTIES] = { NULL, };

static void dkc_scene_mgr_constructed (GObject *obj);
static void dkc_scene_mgr_dispose (GObject *obj);

static void dkc_scene_constructed (GObject *obj);
static void dkc_scene_dispose (GObject *obj);

static void dkc_wrapped_source_constructed (GObject *obj);
static void dkc_wrapped_source_dispose (GObject *obj);


/* DkcSceneMgr GObject methods */

static void
dkc_scene_mgr_set_property (GObject      *object,
                            guint         property_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
  
  DkcSceneMgr *self = DKC_SCENE_MGR (object);

  switch (property_id)
  {
      
    case PROP_BACKEND_CTX:
      self->bkn_ctx = g_value_get_pointer(value);
      break;

    case PROP_CREATE_SCENE_FUNC:
      self->create_scene = g_value_get_pointer(value);
      break;

    case PROP_DELETE_SCENE_FUNC:
      self->delete_scene = g_value_get_pointer(value);
      break;

    case PROP_WRAP_SOURCE_FUNC:
      self->wrap_source = g_value_get_pointer(value);
      break;

    case PROP_UNWRAP_SOURCE_FUNC:
      self->unwrap_source = g_value_get_pointer(value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
}

static void
dkc_scene_mgr_get_property (GObject    *object,
                            guint       property_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
  DkcSceneMgr *self = DKC_SCENE_MGR (object);

  switch (property_id)
  {
    
    case PROP_BACKEND_CTX:
      g_value_set_pointer(value, self->bkn_ctx);
      break;

    case PROP_CREATE_SCENE_FUNC:
      g_value_set_pointer(value, self->create_scene);
      break;

    case PROP_DELETE_SCENE_FUNC:
      g_value_set_pointer(value, self->delete_scene);
      break;

    case PROP_WRAP_SOURCE_FUNC:
      g_value_set_pointer(value, self->wrap_source);
      break;

    case PROP_UNWRAP_SOURCE_FUNC:
      g_value_set_pointer(value, self->unwrap_source);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
}

static void
dkc_scene_mgr_class_init (DkcSceneMgrClass *klass)
{
  
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = dkc_scene_mgr_set_property;
  object_class->get_property = dkc_scene_mgr_get_property;

  scn_mgr_properties[PROP_BACKEND_CTX] =
    g_param_spec_pointer ("backend_ctx",
                         "Backend Context Pointer",
                         "Pointer to backend context to be used.",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  
  scn_mgr_properties[PROP_CREATE_SCENE_FUNC] =
    g_param_spec_pointer ("create_scene_func",
                          "create_scene() function",
                          "Pointer to create_scene() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  scn_mgr_properties[PROP_DELETE_SCENE_FUNC] =
    g_param_spec_pointer ("delete_scene_func",
                          "delete_scene() function",
                          "Pointer to delete_scene() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  scn_mgr_properties[PROP_WRAP_SOURCE_FUNC] =
    g_param_spec_pointer ("wrap_source_func",
                          "wrap_source() function",
                          "Pointer to wrap_source() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  scn_mgr_properties[PROP_UNWRAP_SOURCE_FUNC] =
    g_param_spec_pointer ("unwrap_source_func",
                          "unwrap_source() function",
                          "Pointer to unwrap_source() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     SCN_MGR_N_PROPERTIES,
                                     scn_mgr_properties);

  object_class->constructed = dkc_scene_mgr_constructed;
  object_class->dispose = dkc_scene_mgr_dispose;
  
}

static void
dkc_scene_mgr_init (DkcSceneMgr *self)
{

}

static void
dkc_scene_mgr_constructed (GObject *obj)
{

  DkcSceneMgr* scn_mgr = DKC_SCENE_MGR(obj);
  
  if(pthread_mutex_init(&scn_mgr->lock, NULL) != 0){ g_error("Scene Mgr creation failed."); }
  for(int i=0 ; i<NB_SCENES; i++) {
    scn_mgr->scenes[i] = NULL;
  }
  scn_mgr->nb_scenes=0;

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->constructed(obj);

}

static void dkc_scene_mgr_dispose (GObject *obj) {

  DkcSceneMgr* scn_mgr = DKC_SCENE_MGR(obj);
  pthread_mutex_destroy(&scn_mgr->lock);

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->dispose(obj);
  
}

/* DkcScene GObject methods */

static void
dkc_scene_set_property (GObject      *object,
                        guint         property_id,
                        const GValue *value,
                        GParamSpec   *pspec)
{
  
  DkcScene *self = DKC_SCENE (object);

  switch (property_id)
  {
      
    case PROP_SCENE_MGR_PTR:
      self->scn_mgr = g_value_get_pointer(value);
      break;

    case PROP_SCENE_ID:
      self->id = g_value_get_uchar(value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
  
}

static void
dkc_scene_get_property (GObject    *object,
                        guint       property_id,
                        GValue     *value,
                        GParamSpec *pspec)
{
  
  DkcScene *self = DKC_SCENE (object);

  switch (property_id)
  {
    
    case PROP_SCENE_MGR_PTR:
      g_value_set_pointer(value, self->scn_mgr);
      break;

    case PROP_SCENE_ID:
      g_value_set_uchar(value, self->id);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
  
}

static void
dkc_scene_class_init (DkcSceneClass *klass)
{
  
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = dkc_scene_set_property;
  object_class->get_property = dkc_scene_get_property;

  scn_properties[PROP_SCENE_MGR_PTR] =
    g_param_spec_pointer ("scn_mgr",
                          "SceneMgr",
                          "Pointer to parent SceneMgr.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  scn_properties[PROP_SCENE_ID] =
    g_param_spec_uchar ("id",
                        "Scene ID",
                        "ID of the scene.",
                        0, 255, 0,
                        G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     SCN_N_PROPERTIES,
                                     scn_properties);

  object_class->constructed = dkc_scene_constructed;
  object_class->dispose = dkc_scene_dispose;
  
}

static void
dkc_scene_init (DkcScene *self)
{

}

static void
dkc_scene_constructed (GObject *obj)
{

  DkcScene* scn = DKC_SCENE(obj);
  
  if(pthread_mutex_init(&scn->lock, NULL) != 0) g_error("Scene creation failed.");

  for(int j=0 ; j<NB_WRP_SOURCES; j++) {
    scn->sources[j] = NULL;
  }

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->constructed(obj);

}

static void dkc_scene_dispose (GObject *obj) {

  DkcScene* scn = DKC_SCENE(obj);
  pthread_mutex_destroy(&scn->lock);
  
}

/* DkcWrappedSource GObject methods */

static void
dkc_wrapped_source_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
  
  DkcWrappedSource *self = DKC_WRAPPED_SOURCE (object);

  switch (property_id)
  {
      
    case PROP_SCENE_PTR:
      self->scn = g_value_get_pointer(value);
      break;

    case PROP_WRPD_SOURCE_ID:
      self->id = g_value_get_uchar(value);
      break;

    case PROP_SOURCE_ID:
      self->source_id = g_value_get_uchar(value);
      break;

    case PROP_X:
      self->x = g_value_get_int(value);
      break;

    case PROP_Y:
      self->y = g_value_get_int(value);
      break;

    case PROP_WIDTH:
      self->width = g_value_get_uint(value);
      break;

    case PROP_HEIGHT:
      self->height = g_value_get_uint(value);
      break;

    case PROP_VOLUME:
      self->volume = g_value_get_float(value);
      break;
      
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
  
}

static void
dkc_wrapped_source_get_property (GObject    *object,
                                 guint       property_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
  
  DkcWrappedSource *self = DKC_WRAPPED_SOURCE (object);

  switch (property_id)
  {
    
    case PROP_SCENE_PTR:
      g_value_set_pointer(value, self->scn);
      break;

    case PROP_WRPD_SOURCE_ID:
      g_value_set_uchar(value, self->id);
      break;

    case PROP_SOURCE_ID:
      g_value_set_uchar(value, self->source_id);
      break;

    case PROP_X:
      g_value_set_int(value, self->x);
      break;

    case PROP_Y:
      g_value_set_int(value, self->y);
      break;

    case PROP_WIDTH:
      g_value_set_uint(value, self->width);
      break;

    case PROP_HEIGHT:
      g_value_set_uint(value, self->height);
      break;

    case PROP_VOLUME:
      g_value_set_float(value, self->volume);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
      
  }
  
}

static void
dkc_wrapped_source_class_init (DkcWrappedSourceClass *klass)
{
  
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = dkc_wrapped_source_set_property;
  object_class->get_property = dkc_wrapped_source_get_property;

  wrpd_src_properties[PROP_SCENE_PTR] =
    g_param_spec_pointer ("scn",
                          "Scene",
                          "Pointer to parent Scene.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  wrpd_src_properties[PROP_WRPD_SOURCE_ID] =
    g_param_spec_uchar ("id",
                        "WrappedSource ID",
                        "ID of the wrapped source.",
                        0, 255, 0,
                        G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  wrpd_src_properties[PROP_SOURCE_ID] =
    g_param_spec_uchar ("src_id",
                        "Source ID",
                        "ID of the original source being wrapped.",
                        0, 255, 0,
                        G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  wrpd_src_properties[PROP_X] =
    g_param_spec_int ("x",
                      "x",
                      "x coordinate of the wrapped source in the scene.",
                      SHRT_MIN, SHRT_MAX, 0, G_PARAM_READWRITE);

  wrpd_src_properties[PROP_Y] =
    g_param_spec_int ("y",
                      "y",
                      "y coordinate of the wrapped source in the scene.",
                      SHRT_MIN, SHRT_MAX, 0, G_PARAM_READWRITE);

  wrpd_src_properties[PROP_WIDTH] =
    g_param_spec_uint ("width",
                       "Width",
                       "Width the wrapped source in the scene.",
                       0, USHRT_MAX, 0, G_PARAM_READWRITE);

  wrpd_src_properties[PROP_HEIGHT] =
    g_param_spec_uint ("height",
                       "Height",
                       "Height of the wrapped source in the scene.",
                       0, USHRT_MAX, 0, G_PARAM_READWRITE);

  wrpd_src_properties[PROP_VOLUME] =
    g_param_spec_float ("volume",
                        "Volume",
                        "Volume of the wrapped source in the scene.",
                        0.0, 1.0, 1.0, G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     WRPD_SRC_N_PROPERTIES,
                                     wrpd_src_properties);

  object_class->constructed = dkc_wrapped_source_constructed;
  object_class->dispose = dkc_wrapped_source_dispose;
  
}

static void
dkc_wrapped_source_init (DkcWrappedSource *self)
{

  self->volume = 1.0;
  
}

static void
dkc_wrapped_source_constructed (GObject *obj)
{

  DkcWrappedSource* wrpd_src = DKC_WRAPPED_SOURCE(obj);
  
  if(pthread_mutex_init(&wrpd_src->lock, NULL) != 0) g_error("Source wrapping failed.");

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->constructed(obj);

}

static void dkc_wrapped_source_dispose (GObject *obj) {

  DkcWrappedSource* wrpd_src = DKC_WRAPPED_SOURCE(obj);
  pthread_mutex_destroy(&wrpd_src->lock);
  
}

/* Scene handling */

DkcSceneMgr* dkc_scenemgr_create(DkcSceneCBs scn_cbs, GError* err) {

  return g_object_new (DKC_TYPE_SCENE_MGR,
                       "backend_ctx", scn_cbs.bkn_ctx,
                       "create_scene_func", scn_cbs.create_scene,
                       "delete_scene_func", scn_cbs.delete_scene,
                       "wrap_source_func", scn_cbs.wrap_source,
                       "unwrap_source_func", scn_cbs.unwrap_source,
                       NULL);
  
}

gboolean dkc_scenemgr_delete(DkcSceneMgr* scn_mgr, GError* err) {

  for(int i=0; i<NB_SCENES; i++) {
    if(scn_mgr->scenes[i] != NULL) return ERROR;
  }
  
  g_object_unref(scn_mgr);
  return OK;
  
}

DkcScene* dkc_scene_create(DkcSceneMgr* scn_mgr, GError* err){

  pthread_mutex_lock(&scn_mgr->lock);
  
  for(int i=0; i<NB_SCENES; i++) {
    if(scn_mgr->scenes[i] == NULL) {
      scn_mgr->scenes[i] = g_object_new (DKC_TYPE_SCENE, "scn_mgr", scn_mgr, "id", i, NULL);
      if(scn_mgr->create_scene(scn_mgr->bkn_ctx, i) != OK){
        g_object_unref(scn_mgr->scenes[i]);
        scn_mgr->scenes[i] = NULL;
        pthread_mutex_unlock(&scn_mgr->lock);
        return NULL;
      }

      scn_mgr->nb_scenes++;
      pthread_mutex_unlock(&scn_mgr->lock);
      return scn_mgr->scenes[i];
    }
  }
  pthread_mutex_unlock(&scn_mgr->lock);
  return NULL;
  
}

gboolean dkc_scene_delete(DkcScene* scn, GError* err){

  DkcSceneMgr* scn_mgr = scn->scn_mgr;
  pthread_mutex_lock(&scn_mgr->lock);
  
  uint8_t id = scn->id;
  for(int i=0 ; i<NB_WRP_SOURCES; i++) {
    if(scn_mgr->scenes[id]->sources[i] != NULL){ pthread_mutex_unlock(&scn_mgr->lock); return ERROR; }
  }
  if(scn_mgr->delete_scene(scn_mgr->bkn_ctx, id) != OK) {
    pthread_mutex_unlock(&scn_mgr->lock);
    return ERROR;
  }
  g_object_unref(scn_mgr->scenes[id]);
  scn_mgr->scenes[id] = NULL;
  scn_mgr->nb_scenes--;
  pthread_mutex_unlock(&scn_mgr->lock);
  return OK;
  
}

/* Source wrapping */

DkcWrappedSource* dkc_source_wrap(DkcScene* scn, DkcSource* src, GError* err) {
    
  DkcWrappedSource* wrpd_src = NULL;
  DkcSceneMgr* scn_mgr = scn->scn_mgr;
  pthread_mutex_lock(&scn->lock);
  
  for(int i=0; i<NB_WRP_SOURCES; i++) {
    if(scn->sources[i] == NULL) {
      scn->sources[i] = g_object_new (DKC_TYPE_WRAPPED_SOURCE, "scn", scn, "src_id", src->id, "id", i, NULL);
      if(scn_mgr->wrap_source(scn_mgr->bkn_ctx, scn->id, src->id, i) != OK){
        scn->sources[i] = NULL;
        pthread_mutex_unlock(&scn->lock);
        return NULL;
      }
      wrpd_src = scn->sources[i];
      scn->nb_sources++;
      pthread_mutex_unlock(&scn->lock);
      return wrpd_src;
    }
  }
  pthread_mutex_unlock(&scn->lock);
  return wrpd_src;
  
}

gboolean dkc_source_unwrap(DkcWrappedSource* wrpd_src, GError* err) {

  DkcScene* scn = wrpd_src->scn;
  DkcSceneMgr* scn_mgr = scn->scn_mgr;
  uint8_t id = wrpd_src->id;
  pthread_mutex_lock(&scn->lock);
  if(scn_mgr->unwrap_source(scn_mgr->bkn_ctx, id) != OK) {
    pthread_mutex_unlock(&scn->lock);
    return ERROR;
  }
  pthread_mutex_destroy(&wrpd_src->lock);
  g_object_unref(scn->sources[id]);
  scn->sources[id] = NULL;
  scn->nb_sources--;

  pthread_mutex_unlock(&scn->lock);
  return OK;
  
}

DkcScene* dkc_app_scene_create(DkcApp* app, GError* err) {
  if(app == NULL) return NULL;
  return dkc_scene_create(app->scn_mgr, err);
}
