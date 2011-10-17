// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#pragma once

#include <set>

#include "refcount.h"

namespace newt {

class Trigger;

class TriggerListener {
 public:
  virtual ~TriggerListener() {}
  virtual void OnTriggerUpdate(const Trigger& t) = 0;
};
  
class Trigger : public Refcount {
 public:
  Trigger();

  // Adds an entity to the trigger. The entity may then Activate() or
  // Deactivate() the trigger.
  void AddEntity(const void* entity);
  void RemoveEntity(const void* entity);

  void Activate(const void* entity);
  void Deactivate(const void* entity);

  // Adds a listener to the trigger. Whenever the trigger is changed, all
  // listeners will be updated
  void AddListener(TriggerListener* listener);
  void RemoveListener(TriggerListener* listener);

  inline int ActiveEntities() const { return active_; }
  inline int TotalEntities() const { return total_; }

private:
  void UpdateListeners() const;

#ifdef DEBUG
  std::set<const void*> active_entities_;
  std::set<const void*> entities_;
#endif
  std::set<TriggerListener*> listeners_;
  int active_;
  int total_;
};
  
}  // namespace newt