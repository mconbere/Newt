// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "newt-game.h"

namespace newt {

Game::Game() {
  
}
  
void Game::update() {
  // Perform all of the Newt related update logic.
  MEngine* engine = MEngine::getInstance();
  MScene* scene = engine->getLevel()->getCurrentScene();

  (void)scene; // inspect collisions for each object in the scene;

  // Finally, call the original update method.
  MGame::update();
}

}  // namespace newt