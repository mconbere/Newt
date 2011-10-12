// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "MEngine.h"

namespace newt {

class Entity;

class Game : public MGame {
public:
  Game();

  virtual void update();
  
  void GetCollidingEntityPairs(vector<pair<Entity*, Entity*> >* pairs);
};
  
}  // namespace newt