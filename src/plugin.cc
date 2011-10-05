// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#include "plugin.h"

#include "MEngine.h"
#include "MLevel.h"
#include "MScene.h"
#include "MScriptContext.h"
#include "MVector3.h"

#define RETURN_VALUE_UNLESS_NOTNULL(x, v) if (!(x)) return (v);

namespace {

int asdf(void) {
  MEngine* engine = MEngine::getInstance();
  engine->getLevel()->getCurrentScene()->setGravity(MVector3(0.98, 0, -0.98));
  return 0;
}

}  // namespace

int DeleteObject() {
  MEngine* engine = MEngine::getInstance();
  RETURN_VALUE_UNLESS_NOTNULL(engine, 0);
  MLevel* level = engine->getLevel();
  RETURN_VALUE_UNLESS_NOTNULL(level, 0);
  MScene* scene = level->getCurrentScene();
  RETURN_VALUE_UNLESS_NOTNULL(scene, 0);
  MScriptContext* script = engine->getScriptContext();
  RETURN_VALUE_UNLESS_NOTNULL(script, 0);
  if (script->getArgsNumber() != 1) return 0;
  unsigned int id = static_cast<unsigned int>(script->getInteger(0));
  
  if (id < scene->getObjectsNumber()) {
    MObject3d* object = scene->getObjectByIndex(id);
    RETURN_VALUE_UNLESS_NOTNULL(object, 0);
    if (object->getType() == M_OBJECT3D_ENTITY) {
      MOEntity* entity = static_cast<MOEntity*>(object);
      MPhysicsProperties* physics_props = entity->getPhysicsProperties();
      MPhysicsContext* physics = engine->getPhysicsContext();
      if (physics_props && physics) {
        unsigned int physics_id = physics_props->getCollisionObjectId();
        physics->deleteObject(&physics_id);
      }
    }
    scene->deleteObject(object);
  }
  return 0;
}

void StartPlugin() {
  MEngine* engine = MEngine::getInstance();
  engine->getScriptContext()->addFunction("asdf", asdf);
  engine->getScriptContext()->addFunction("delete", DeleteObject);
}

void EndPlugin() {
}
