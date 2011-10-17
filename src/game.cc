// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "game.h"

#include "entity.h"
#include "player.h"
#include "gem.h"

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
    // Every action has an equal and opposite reaction. Call collision in
    // both directions.
    it->first->CollideWith(it->second);
    it->second->CollideWith(it->first);
  }
  
  // Clean up removed objects
  MEngine* engine = MEngine::getInstance();
  MScene* scene = engine->getLevel()->getCurrentScene();
  size_t entities_count = scene->getEntitiesNumber();
  for (size_t i = 0; i < entities_count; ++i) {
    Entity* entity = Entity::FromSceneEntity(scene->getEntityByIndex((unsigned int)i));
    if (entity && entity->ShouldRemoveAtEndOfUpdate()) {
      scene->deleteObject(scene->getEntityByIndex((unsigned int)i));
    }
  }
}

void Game::onAddNewEntity(MScene* scene, MOEntity* scene_entity, const map<string, string>& attributes) {
  map<string, string>::const_iterator name = attributes.find("name");
  if (name != attributes.end()) {
    if (name->second == "Player") {
      new Player(scene_entity, attributes);
    } else if (name->second == "Gem") {
      new Gem(scene_entity, attributes);
    }
    // add new else if clauses for new object types. At some point this should be switched to registered class
    // logic.
  } else {
    // Make a default base entity for all other object. The default object
    // doesn't currently do anything, and if it's problematic in the future
    // it can be removed.
    new Entity(scene_entity, attributes);
  }
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
    if (first && second) {
      pairs->push_back(pair<Entity*, Entity*>(first, second));
    }
  }
}

}  // namespace newt