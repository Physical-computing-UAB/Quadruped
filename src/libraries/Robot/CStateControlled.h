#ifndef CStateControlled_h
#define CStateControlled_h

#include "CMachineState.h"

class CStateControlled:public CMachineState
{
public:
  CStateControlled();
  ~CStateControlled();

  // Generic functions
  void handleEvents(){};
  void update();
  void run();

  bool onEnter();
  bool onExit();

  String getStateID() const { return s_CONTROLLEDstateID; };
private:

  static const String s_CONTROLLEDstateID;
};

#endif
