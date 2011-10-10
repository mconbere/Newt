// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#include "plugin.h"

#include "MEngine.h"

#include "on-collision-behavior.h"

namespace {

int asdf(void) {
  MEngine* engine = MEngine::getInstance();
  engine->getLevel()->getCurrentScene()->setGravity(MVector3(0.98, 0, -0.98));
  return 0;
}

}  // namespace

void StartPlugin() {
  MEngine* engine = MEngine::getInstance();
  engine->getScriptContext()->addFunction("asdf", asdf);
  engine->getBehaviorManager()->addBehavior(
      OnCollisionBehavior::GetStaticName(), M_OBJECT3D,
      OnCollisionBehavior::GetNew);
}

void EndPlugin() {
}
