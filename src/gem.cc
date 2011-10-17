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
  if (entity->ReceiveInventory(inventory_)) {
    inventory_.Clear();
    RemoveAtEndOfUpdate();
  }
  return true;
}

}  // namespace newt