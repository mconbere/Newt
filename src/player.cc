// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "player.h"

#include "entity.h"

using namespace std;

namespace newt {

Player::Player(MOEntity* entity, const map<string, string>& attributes)
    : Entity(entity, attributes) {}
  
bool Player::CollideWith(Entity* entity) {
  return true;
}

bool Player::ReceiveInventory(Inventory* inventory) {
  // Accept the entire inventory.
  inventory_.AddInventory(*inventory);
  inventory->Clear();

  printf("Player inventory updated:\n");
  for (map<string, int>::const_iterator it = inventory_.ItemsMap().begin();
       it != inventory_.ItemsMap().end(); ++it) {
    printf("  %s: %d\n", it->first.c_str(), it->second);
  }

  return true;
}

}  // namespace newt