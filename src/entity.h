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
class Inventory;

class Entity {
 public:
  Entity(MOEntity* entity, const std::map<std::string, std::string>& attributes);
  virtual ~Entity();

  // Handling the underlying scene entity
  virtual void SetSceneEntity(MOEntity* scene_entity);
  virtual MOEntity* SceneEntity() { return scene_entity_; }

  // Actions
  virtual bool CollideWith(Entity* entity) { return false; }
  virtual bool ReceiveInventory(const Inventory& inventory) { return false; }

  void RemoveAtEndOfUpdate() { remove_at_end_of_update_ = true; }
  void CancelRemoveAtEndOfUpdate() { remove_at_end_of_update_ = false; }
  bool ShouldRemoveAtEndOfUpdate() { return remove_at_end_of_update_; }
  
  // Static helper functions
  static Entity* FromSceneEntity(MOEntity* scene_entity);

 protected:
  MOEntity* scene_entity_;
  bool remove_at_end_of_update_;
};

}  // namespace newt