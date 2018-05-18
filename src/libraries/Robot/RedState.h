#ifndef RedState_h
#define RedState_h

#include "CMachineState.h"

class RedState:public CMachineState
{
public:
  RedState();
  ~RedState();

  // Generic functions
  void update();
  void run();

  bool onEnter();
  bool onExit();

  String getStateID() const { return s_RstateID; };
private:

  static const String s_RstateID;
};

#endif
