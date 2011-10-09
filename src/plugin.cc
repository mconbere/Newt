// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#include "plugin.h"

#include "MEngine.h"
#include "MLevel.h"
#include "MScene.h"
#include "MScriptContext.h"
#include "MVector3.h"

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
}

void EndPlugin() {
}
