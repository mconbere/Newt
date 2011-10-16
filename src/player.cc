// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#include "player.h"

#include "action.h"

using namespace std;

namespace newt {

Player::Player(MOEntity* entity, const map<string, string>& attributes)
    : Entity(entity, attributes) {}
  
bool Player::RespondToAction(const Action& action) {
  if (action.Name() == "Collide") {
    printf("Player collision between %p and %p responded to\n", action.Sender(), action.Receiver());
    return true;
  }
  
  return Entity::RespondToAction(action);
}

}  // namespace newt