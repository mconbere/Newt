// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#pragma once

#include <map>
#include <string>

#include "entity.h"

class MOEntity;

namespace newt {

class Player : public Entity {
 public:
  Player(MOEntity* entity, const std::map<std::string, std::string>& attributes);
  
  virtual bool RespondToAction(const Action& action);
};

}  // namespace newt