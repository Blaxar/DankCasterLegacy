#include <libdkc/dkc_scene_internal.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

G_DEFINE_TYPE (DkcSceneMgr, dkc_scene_mgr, G_TYPE_OBJECT)

enum
{
  PROP_BACKEND_CTX = 1,
  PROP_CREATE_SCENE_FUNC,
  PROP_DELETE_SCENE_FUNC,
  PROP_WRAP_SOURCE_FUNC,
  PROP_UNWRAP_SOURCE_FUNC,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void dkc_scene_mgr_constructed (GObject *obj);

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

  obj_properties[PROP_BACKEND_CTX] =
    g_param_spec_pointer ("backend_ctx",
                         "Backend Context Pointer",
                         "Pointer to backend context to be used.",
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  
  obj_properties[PROP_CREATE_SCENE_FUNC] =
    g_param_spec_pointer ("create_scene_func",
                          "create_scene() function",
                          "Pointer to create_scene() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  obj_properties[PROP_DELETE_SCENE_FUNC] =
    g_param_spec_pointer ("delete_scene_func",
                          "delete_scene() function",
                          "Pointer to delete_scene() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  obj_properties[PROP_WRAP_SOURCE_FUNC] =
    g_param_spec_pointer ("wrap_source_func",
                          "wrap_source() function",
                          "Pointer to wrap_source() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  obj_properties[PROP_UNWRAP_SOURCE_FUNC] =
    g_param_spec_pointer ("unwrap_source_func",
                          "unwrap_source() function",
                          "Pointer to unwrap_source() backend implementation.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     N_PROPERTIES,
                                     obj_properties);

  object_class->constructed = dkc_scene_mgr_constructed;
  
}

static void
dkc_scene_mgr_init (DkcSceneMgr *self)
{

}

static void
dkc_scene_mgr_constructed (GObject *obj)
{

  DkcSceneMgr* scn_mgr = DKC_SCENE_MGR(obj);
  
  if(pthread_mutex_init(&scn_mgr->lock, NULL) != 0){ free(scn_mgr); g_error("Scene Mgr creation failed."); }
  for(int i=0 ; i<NB_SCENES; i++) {
    scn_mgr->scenes[i] = NULL;
  }
  scn_mgr->nb_scenes=0;

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->constructed(obj);

}

/* Scene handling */

DkcSceneMgr* dkc_scenemgr_create(DkcSceneCBs scn_cbs) {

  return g_object_new (DKC_TYPE_SCENE_MGR,
                       "backend_ctx", scn_cbs.bkn_ctx,
                       "create_scene_func", scn_cbs.create_scene,
                       "delete_scene_func", scn_cbs.delete_scene,
                       "wrap_source_func", scn_cbs.wrap_source,
                       "unwrap_source_func", scn_cbs.unwrap_source,
                       NULL);
  
}

dkc_rc dkc_scenemgr_delete(DkcSceneMgr* scn_mgr) {

  pthread_mutex_destroy(&scn_mgr->lock);
  for(int i=0; i<NB_SCENES; i++) {
    if(scn_mgr->scenes[i] != NULL) return ERROR;
  }
  free(scn_mgr);
  return OK;
  
}

DkcScene* dkc_scene_create(DkcSceneMgr* scn_mgr){

  DkcScene* scn = NULL;
  pthread_mutex_lock(&scn_mgr->lock);
  
  for(int i=0; i<NB_SCENES; i++) {
    if(scn_mgr->scenes[i] == NULL) {
      scn_mgr->scenes[i] = malloc(sizeof(DkcScene));
      if(pthread_mutex_init(&scn_mgr->scenes[i]->lock, NULL) != 0 ||
         scn_mgr->create_scene(scn_mgr->bkn_ctx, i) != OK){
        free(scn_mgr->scenes[i]);
        scn_mgr->scenes[i] = NULL;
        pthread_mutex_unlock(&scn_mgr->lock);
        return NULL;
      }
      scn_mgr->scenes[i]->scn_mgr = scn_mgr;
      for(int j=0 ; j<NB_WRP_SOURCES; j++) {
        scn_mgr->scenes[i]->sources[j] = NULL;
      }
      scn = scn_mgr->scenes[i];
      scn->id = i;
      scn_mgr->nb_scenes++;
      pthread_mutex_unlock(&scn_mgr->lock);
      return scn;
    }
  }
  pthread_mutex_unlock(&scn_mgr->lock);
  return scn;
  
}

dkc_rc dkc_scene_delete(DkcScene* scn){

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
  pthread_mutex_destroy(&scn_mgr->scenes[id]->lock);
  free(scn_mgr->scenes[id]);
  scn_mgr->scenes[id] = NULL;
  scn_mgr->nb_scenes--;
  pthread_mutex_unlock(&scn_mgr->lock);
  return OK;
  
}

/* Source wrapping */

DkcWrappedSource* dkc_source_wrap(DkcScene* scn, DkcSource* src) {
    
  DkcWrappedSource* wrpd_src = NULL;
  DkcSceneMgr* scn_mgr = scn->scn_mgr;
  pthread_mutex_lock(&scn->lock);
  
  for(int i=0; i<NB_WRP_SOURCES; i++) {
    if(scn->sources[i] == NULL) {
      scn->sources[i] = malloc(sizeof(DkcWrappedSource));
      if(pthread_mutex_init(&scn->sources[i]->lock, NULL) != 0 ||
         scn_mgr->wrap_source(scn_mgr->bkn_ctx, scn->id, src->id, i) != OK){
        free(scn->sources[i]);
        scn->sources[i] = NULL;
        pthread_mutex_unlock(&scn->lock);
        return NULL;
      }
      scn->sources[i]->scn = scn;
      scn->sources[i]->id = i;
      scn->sources[i]->source_id = src->id;
      wrpd_src = scn->sources[i];
      scn->nb_sources++;
      pthread_mutex_unlock(&scn->lock);
      return wrpd_src;
    }
  }
  pthread_mutex_unlock(&scn->lock);
  return wrpd_src;
  
}

dkc_rc dkc_source_unwrap(DkcWrappedSource* wrpd_src) {

  DkcScene* scn = wrpd_src->scn;
  DkcSceneMgr* scn_mgr = scn->scn_mgr;
  uint8_t id = wrpd_src->id;
  pthread_mutex_lock(&scn->lock);
  if(scn_mgr->unwrap_source(scn_mgr->bkn_ctx, id) != OK) {
    pthread_mutex_unlock(&scn->lock);
    return ERROR;
  }
  pthread_mutex_destroy(&wrpd_src->lock);
  free(scn->sources[id]);
  scn->sources[id] = NULL;
  scn->nb_sources--;

  pthread_mutex_unlock(&scn->lock);
  return OK;
  
}

DkcScene* dkc_app_scene_create(DkcApp* app) {
  if(app == NULL) return NULL;
  return dkc_scene_create(APP_SCN_MGR(app));
}
