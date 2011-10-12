// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

class MScene;
class MMeshRef;
class MOEntity;

namespace newt {

class Entity {
 public:
  Entity(MScene* scene, MMeshRef* mesh_ref);
  virtual ~Entity();

  void SetSceneEntity(MOEntity* scene_entity);
  MOEntity* SceneEntity() { return scene_entity_; }

  static Entity* FromSceneEntity(MOEntity* scene_entity);

 protected:
  MOEntity* scene_entity_;
};

}  // namespace newt