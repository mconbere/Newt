// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#pragma once

#include <map>
#include <string>

class MScene;
class MMeshRef;
class MOEntity;

namespace newt {

class Action;

class Entity {
 public:
  Entity(MOEntity* entity, const std::map<std::string, std::string>& attributes);
  virtual ~Entity();

  // Handling the underlying scene entity
  virtual void SetSceneEntity(MOEntity* scene_entity);
  virtual MOEntity* SceneEntity() { return scene_entity_; }

  // Actions
  virtual bool RespondToAction(const Action& action) { return false; }

  // Static helper functions
  static Entity* FromSceneEntity(MOEntity* scene_entity);

 protected:
  MOEntity* scene_entity_;
};

}  // namespace newt