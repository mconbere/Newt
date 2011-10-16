// Copyright (c) 2011 Morgan Conbere. All rights reserved.
// Author: Morgan Conbere <mconbere@gmail.com>

#pragma once

#include <string>

class Entity;

namespace newt {

class Action {
 public:
  Action(Entity* sender, Entity* receiver) : sender_(sender), receiver_(receiver) {}
  virtual ~Action() {}

  virtual const std::string& Name() const { return ""; }

  virtual Entity* Sender() const { return sender_; }
  virtual Entity* Receiver() const { return receiver_; }

 private:
  Entity* sender_;
  Entity* receiver_;
};

}  // namespace newt