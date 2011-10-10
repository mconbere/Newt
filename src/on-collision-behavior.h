// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#pragma once

#include "MEngine.h"

class MObject3d;

class OnCollisionBehavior : public MBehavior {
 public:
  OnCollisionBehavior(MObject3d* parentObject);
  OnCollisionBehavior(const OnCollisionBehavior& behavior,
                      MObject3d* parentObject);
  virtual ~OnCollisionBehavior() {}

  virtual void destroy() {
    delete this;
  }
  
  static MBehavior* GetNew(MObject3d* parentObject) {
    return new OnCollisionBehavior(parentObject);
  }

  static const char* GetStaticName() {
    return "OnCollision";
  }

  virtual MBehavior* getCopy(MObject3d* parentObject) {
    return new OnCollisionBehavior(*this, parentObject);
  }

  const char* getName() {
    return GetStaticName();
  }

  virtual unsigned int getVariablesNumber();
  virtual MVariable getVariable(unsigned int id);

  virtual void update();
  virtual void runEvent(int param) {}

 private:
  MString object_;
  MString function_;
};
