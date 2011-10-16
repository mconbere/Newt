// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#include "plugin.h"

#include "MEngine.h"

#include "on-collision-behavior.h"
#include "game.h"

void StartPlugin() {
  newt::Plugin::GetInstance()->Start();
}

void EndPlugin() {
  newt::Plugin::GetInstance()->End();
}

namespace newt {

void Plugin::Start() {
  if (game_) delete game_;
  game_ = new Game();
  
  MEngine* engine = MEngine::getInstance();
  engine->setGame(game_);
}

void Plugin::End() {
  MEngine* engine = MEngine::getInstance();
  engine->setGame(NULL);
  SAFE_DELETE(game_)
}

Plugin* Plugin::GetInstance() {
  static Plugin plugin;
  return &plugin;
}

}  // namespace newt