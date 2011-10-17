// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#pragma once

#include "MEngine.h"

namespace newt {

class Entity;

class Game : public MGame, public MExternalSceneRep {
public:
  Game();

  virtual void update();
  virtual void onCreateScene();
  virtual void onAddNewEntity(MScene* scene, MOEntity* entity, const map<string, string>& attributes);
  virtual void onRemoveEntity(MScene* scene, MOEntity* entity);

  void GetCollidingEntityPairs(vector<pair<Entity*, Entity*> >* pairs);
};
  
}  // namespace newt