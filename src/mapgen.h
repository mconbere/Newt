// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#pragma once

class MMeshRef;
class MScene;

namespace newt {

class MapGen {
 public:
  MapGen() {}

  void SetRoom(MMeshRef* room_mesh) { room_mesh_ = room_mesh; }

  bool CreateScene(MScene* scene);

 private:
  MMeshRef* room_mesh_;
};

}  // namespace newt
