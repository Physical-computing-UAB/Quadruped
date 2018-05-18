#ifndef BlueState_h
#define BlueState_h

#include "CMachineState.h"

class BlueState : public CMachineState
{
public:
  BlueState(){};
  ~BlueState(){};

  // Generic functions
  void update();
  void run();

  bool onEnter();
  bool onExit();

  String getStateID() const { return s_BstateID; };
private:
  static const String s_BstateID;
};

#endif
