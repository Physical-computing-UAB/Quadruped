#ifndef GreenState_h
#define GreenState_h

#include "CMachineState.h"

class GreenState : public CMachineState
{
public:
  GreenState(){};
  ~GreenState(){};

  // Generic functions
  void handleEvents();
  void update();
  void run();

  bool onEnter();
  bool onExit();
private:

};

#endif
