#ifndef RedState
#define RedState

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
