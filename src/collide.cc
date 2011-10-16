// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "collide.h"

#define CLASS_NAME "Collide"

namespace newt {

const std::string& Collide::Name() const {
  return Collide::ClassName();
}

const std::string& Collide::ClassName() {
  static std::string class_name = CLASS_NAME;
  return class_name;
}

}  // namespace newt