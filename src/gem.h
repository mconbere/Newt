// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#pragma once

#include <map>
#include <string>

#include "entity.h"
#include "inventory.h"

class MOEntity;

namespace newt {

class Gem : public Entity {
 public:
  Gem(MOEntity* entity, const std::map<std::string, std::string>& attributes);
  
  virtual bool CollideWith(Entity* entity);

 private:
  Inventory inventory_;
};

}  // namespace newt