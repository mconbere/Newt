// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "entity.h"

#include "MEngine.h"

namespace newt {

Entity::Entity(MOEntity* entity, const map<string, string>& attributes) {
  SetSceneEntity(entity);
}

Entity::~Entity() {}

void Entity::SetSceneEntity(MOEntity* scene_entity) {
  scene_entity_ = scene_entity;
  if (scene_entity_) {
    scene_entity->setUserPointer(this);
  }
}

Entity* Entity::FromSceneEntity(MOEntity* scene_entity) {
  return (Entity *)scene_entity->getUserPointer();
}
  
}  // namespace newt