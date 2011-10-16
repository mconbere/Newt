// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "game.h"

#include "entity.h"

namespace newt {

Game::Game() {
}
  
void Game::onCreateScene() {
  MGame::onCreateScene();

  MEngine* engine = MEngine::getInstance();
  MScene* scene = engine->getLevel()->getCurrentScene();

  scene->setExternalSceneRep(this);
}

void Game::update() {
  // First, call the original update method.
  MGame::update();

  vector<pair<Entity *, Entity *> > pairs;
  GetCollidingEntityPairs(&pairs);
  
  for (vector<pair<Entity *, Entity *> >::iterator it = pairs.begin();
       it != pairs.end(); ++it) {
    printf("Got collision: %p, %p\n", it->first, it->second);
  }
}

void Game::onAddNewEntity(MScene* scene, MOEntity* scene_entity, const map<string, string>& attributes) {
  printf("Adding object Entity %p with attributes:\n", scene_entity);
  for (map<string, string>::const_iterator it = attributes.begin(); it != attributes.end(); ++it) {
    printf("  %s: %s\n", it->first.c_str(), it->second.c_str());
  }
  new Entity(scene_entity);
}

void Game::onRemoveEntity(MScene* scene, MOEntity* scene_entity) {
  // TODO(mconbere): I'm not sure what the use of this will be, but it seemed
  // smart to add it while I was in the scene code. I can't hurt, and will
  // probably come in handy
  Entity* entity = Entity::FromSceneEntity(scene_entity);
  (void)entity;
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