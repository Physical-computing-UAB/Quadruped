#ifndef GreenState_h
#define GreenState_h

#include "CMachineState.h"

class GreenState : public CMachineState
{
public:
  GreenState(){};
  ~GreenState(){};

  // Generic functions
  void update();
  void run();

  bool onEnter();
  bool onExit();

  String getStateID() const { return s_GstateID; };
private:
  static const String s_GstateID;
};

#endif
