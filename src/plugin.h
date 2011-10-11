// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#pragma once

#ifdef WIN32
#include <windows.h>
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

extern "C" DLLEXPORT void StartPlugin();
extern "C" DLLEXPORT void EndPlugin();

namespace newt {

class Game;

class Plugin {
public:
  void Start();
  void End();

  static Plugin* GetInstance();
private:
  Game* game_;
};

}  // namespace newt