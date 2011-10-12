// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "game.h"

#include "entity.h"

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
  
void Game::GetCollidingEntityPairs(vector<pair<Entity*, Entity*> >* pairs) {
  if (!pairs) return;

  MEngine* engine = MEngine::getInstance();
  MScene* scene = engine->getLevel()->getCurrentScene();

  vector<pair<MOEntity*, MOEntity*> > scene_entity_pairs;
  scene->getCollidingEntityPairs(&scene_entity_pairs);

  for (vector<pair<MOEntity*, MOEntity*> >::iterator it =
       scene_entity_pairs.begin(); it != scene_entity_pairs.end(); ++it) {
    Entity* first = Entity::FromSceneEntity(it->first);
    Entity* second = Entity::FromSceneEntity(it->second);
    pairs->push_back(pair<Entity*, Entity*>(first, second));
  }
}

}  // namespace newt