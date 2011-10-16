// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#pragma once

#include <string>

#include "action.h"

namespace newt {

class Collide : public Action {
 public:
  Collide(Entity* sender, Entity* receiver) : Action(sender, receiver) {}

  virtual const std::string& Name() const;

  static const std::string& ClassName();

 private:
};

}  // namespace newt