// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#include "on-collision-behavior.h"

#include "MEngine.h"

OnCollisionBehavior::OnCollisionBehavior(MObject3d* parentObject)
    : MBehavior(parentObject),
      object_("none"),
      function_("none") {}

OnCollisionBehavior::OnCollisionBehavior(const OnCollisionBehavior& behavior,
                                         MObject3d* parentObject)
    : MBehavior(parentObject),
      object_(behavior.object_),
      function_(behavior.function_) {}

unsigned int OnCollisionBehavior::getVariablesNumber() {
  return 2;
}

MVariable OnCollisionBehavior::getVariable(unsigned int id) {
  switch (id) {
    case 0:
      return MVariable("With object", &object_, M_VARIABLE_STRING);
    case 1:
      return MVariable("Run function", &function_, M_VARIABLE_STRING);
    default:
      return MVariable("NULL", NULL, M_VARIABLE_NULL);
  }
}

void OnCollisionBehavior::update() {
  MEngine* engine = MEngine::getInstance();
  if (!engine->getGame()->isRunning()) return;

  const char* name = object_.getData();
  if (!name || !strcmp(name, "none")) return;
  const char* function = function_.getData();
  if (!function || !strcmp(function, "none")) return;

  MScene* scene = engine->getLevel()->getCurrentScene();
  MObject3d* object = scene->getObjectByName(name);
  if (!object) return;
  MObject3d* parent = getParentObject();
  if (!parent) return;

  if (object->getType() != M_OBJECT3D_ENTITY ||
      parent->getType() != M_OBJECT3D_ENTITY) return;

  MPhysicsProperties* object_props =
      static_cast<MOEntity*>(object)->getPhysicsProperties();
  MPhysicsProperties* parent_props =
      static_cast<MOEntity*>(parent)->getPhysicsProperties();
  if (!object_props || !parent_props) return;

  MPhysicsContext* physics = engine->getPhysicsContext();
  if (physics->isObjectsCollision(object_props->getCollisionObjectId(),
                                  parent_props->getCollisionObjectId())) {
    engine->getScriptContext()->callFunction(function);
  }
}
