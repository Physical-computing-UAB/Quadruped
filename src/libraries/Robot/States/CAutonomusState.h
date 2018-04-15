#ifndef CAutonomusState_h
#define CAutonomusState_h

// Include libraries.
#include "CMachineState.h"

class CAutonomusState : public CMachineState
{
public:
  // Constructors
  CAutonomusState();
  ~CAutonomusState();

  // Methods
  void handleEvents();
  void update();
  void behave();

  bool onEnter();
  bool onExit();

private:


};
#endif
