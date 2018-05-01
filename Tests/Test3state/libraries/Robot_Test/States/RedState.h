#ifndef RedState_h
#define RedState_h

#include "CMachineState.h"

class RedState : public CMachineState
{
public:
  RedState(){};
  ~RedState(){};

  // Generic functions
  void handleEvents();
  void update();
  void run();

  bool onEnter();
  bool onExit();
private:

};

#endif
