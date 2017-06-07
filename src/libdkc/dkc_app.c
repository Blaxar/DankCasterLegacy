#include <libdkc/dkc_app_internal.h>
#include <stdlib.h>
#include <stdio.h>

G_DEFINE_TYPE (DkcApp, dkc_app, G_TYPE_OBJECT)

enum
{
  PROP_BACKEND = 1,
  PROP_PARAMS,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = { NULL, };

static void dkc_app_constructed (GObject *obj);

static void
dkc_app_set_property (GObject      *object,
                      guint         property_id,
                      const GValue *value,
                      GParamSpec   *pspec)
{
  DkcApp *self = DKC_APP (object);

  switch (property_id)
    {
    case PROP_BACKEND:
      g_free (self->bkn_type);
      self->bkn_type = g_value_dup_string(value);
      break;
      
    case PROP_PARAMS:
      if(self->params) dkc_params_unref (self->params);
      self->params = g_value_get_pointer(value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
dkc_app_get_property (GObject    *object,
                      guint       property_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
  DkcApp *self = DKC_APP (object);

  switch (property_id)
    {

    case PROP_BACKEND:
      g_value_set_string(value, self->bkn_type);
      break;
    
    case PROP_PARAMS:
      dkc_params_ref(self->params);
      g_value_set_pointer(value, self->params);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}



static void
dkc_app_class_init (DkcAppClass *klass)
{
  
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->set_property = dkc_app_set_property;
  object_class->get_property = dkc_app_get_property;

  obj_properties[PROP_BACKEND] =
    g_param_spec_string ("backend",
                         "Backend",
                         "Name of the backend type to be instanciated.",
                         NULL  /* default value */,
                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  
  obj_properties[PROP_PARAMS] =
    g_param_spec_pointer ("params",
                          "Params",
                          "Parameters for pipeline context initilization.",
                          G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class,
                                     N_PROPERTIES,
                                     obj_properties);

  object_class->constructed = dkc_app_constructed;
  
}

static void
dkc_app_init (DkcApp *self)
{

}

static void
dkc_app_constructed (GObject *obj)
{

  DkcApp* app = DKC_APP(obj);

  app->backend = dkc_backend_create(app->bkn_type, app->params);
  if(!app->backend){ free(app); g_error("Backend creation failed."); }
  app->src_mgr = dkc_sourcemgr_create(
                     (DkcSourceCBs) {app->backend->ctx,
                                     app->backend->create_source,
                                     app->backend->delete_source});
  if(!app->src_mgr) { free(app); g_error("Source manager creation failed."); }
  
  app->scn_mgr = dkc_scenemgr_create(
                     (DkcSceneCBs) {app->backend->ctx,
                                    app->backend->create_scene,
                                    app->backend->delete_scene,
                                    app->backend->wrap_source,
                                    app->backend->unwrap_source});
  if(!app->scn_mgr) { free(app); g_error("Scene manager creation failed."); }
  
  app->snk_mgr = dkc_sinkmgr_create(
                     (DkcSinkCBs) {app->backend->ctx,
                                   app->backend->create_sink,
                                   app->backend->delete_sink});
  if(!app->snk_mgr) { free(app); g_error("Sink manager creation failed."); }

  GObjectClass* klass = g_type_class_peek_parent(G_OBJECT_GET_CLASS(obj));
  if(klass) klass->constructed(obj);

}

DkcApp* dkc_app_create(const char* bkn_type, DkcParams* params) {

  return g_object_new (DKC_TYPE_APP, "backend", bkn_type, "params", params, NULL); //malloc(sizeof(DkcApp));
  
}

dkc_rc dkc_app_delete(DkcApp* app) {

  if(!dkc_sinkmgr_delete(app->snk_mgr)) return ERROR;
  if(!dkc_scenemgr_delete(app->scn_mgr)) return ERROR;
  if(!dkc_sourcemgr_delete(app->src_mgr)) return ERROR;
  if(!dkc_backend_delete(app->backend)) return ERROR;
  
  g_object_unref(app);

  return OK;
  
}

dkc_rc dkc_app_start(DkcApp* app) {

    return app->backend->start(app->backend->ctx);

}

dkc_rc dkc_app_stop(DkcApp* app){

    return app->backend->stop(app->backend->ctx);

}
