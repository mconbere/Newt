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

  vector<pair<MOEntity *, MOEntity *> > pairs;
  scene->getCollidingEntityPairs(&pairs);
  
  for (vector<pair<MOEntity *, MOEntity *> >::iterator it = pairs.begin();
       it != pairs.end(); ++it) {
    printf("Got collision: %p, %p\n", it->first, it->second);
  }

  // Finally, call the original update method.
  MGame::update();
}

}  // namespace newt