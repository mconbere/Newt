// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "gem.h"

using namespace std;

namespace newt {

Gem::Gem(MOEntity* entity, const map<string, string>& attributes)
    : Entity(entity, attributes) {
  inventory_.AddItem("gem", 1);
}
  
bool Gem::CollideWith(Entity* entity) {
  entity->ReceiveInventory(&inventory_);

  if (inventory_.IsEmpty()) {
    RemoveAtEndOfUpdate();
  }

  return true;
}

}  // namespace newt