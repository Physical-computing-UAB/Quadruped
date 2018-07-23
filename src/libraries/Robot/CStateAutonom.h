#ifndef CStateAutonom_h
#define CStateAutonom_h

#include "CMachineState.h"

class CStateAutonom : public CMachineState
{
public:
  CStateAutonom(){};
  ~CStateAutonom(){};

  // Generic functions
  void handleEvents();
  void update();
  void run();

  bool onEnter();
  bool onExit();

  String getStateID() const { return s_AUTOstateID; };
private:
  static const String s_AUTOstateID;
};

#endif
