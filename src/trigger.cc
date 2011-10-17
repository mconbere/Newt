// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#include "trigger.h"

#include <cassert>

namespace newt {

Trigger::Trigger() : active_(0), total_(0) {}

void Trigger::AddEntity(const void* entity) {
#ifdef DEBUG
  assert(total_ == entities_.size());
  assert(entities_.insert(entity).second);
#else
  (void) entity;
#endif
  ++total_;
  UpdateListeners();
}

void Trigger::Activate(const void* entity) {
#ifdef DEBUG
  assert(entities_.find(entity) != entities_.end());
  assert(active_ == active_entities_.size());
  assert(active_entities_.insert(entity).second);
#else
  (void) entity;
#endif
  ++active_;
  UpdateListeners();
}

void Trigger::Deactivate(const void* entity) {
#ifdef DEBUG
  assert(entities_.find(entity) != entities_.end());
  assert(active_ == active_entities_.size());
  assert(active_entities_.erase(entity) == 1);
#else
  (void) entity;
#endif
  --active_;
  UpdateListeners();
}

void Trigger::AddListener(TriggerListener* listener) {
  assert(listeners_.insert(listener).second);
}

void Trigger::RemoveListener(TriggerListener* listener) {
  assert(listeners_.erase(listener) == 1);
}

void Trigger::UpdateListeners() const {
  for (std::set<TriggerListener*>::const_iterator it = listeners_.begin();
       it != listeners_.end(); ++it) {
    (*it)->OnTriggerUpdate(*this);
  }
}

}  // namespace newt
