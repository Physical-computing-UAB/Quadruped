#ifndef CRobot_h
#define CRobot_h

// Include libraries.
#include <Arduino.h>
#include <UltrasonicSensor.h>
#include <vector>

class CMachineState; // A promess that CMachineState exists
class CRobot {
public:
  // Constructor
  CRobot();
  // Destructor
  ~CRobot(){};

  bool init();
  void handleEvents();
  void update();
  void behave();

  // State transitions
  void pushState(CMachineState* pState);
  void changeState(CMachineState* pState);
  void popState();

  // Getters & Setters
  UltrasonicSensorArray getUSA(){ return m_usArray; };

  void setUltrasonicSA( UltrasonicSensorArray* pUlrasonicSA ){ m_usArray = pUlrasonicSA; }const;

private:
  std::vector<CMachineState*> m_mindStates; // Stack
  UltrasonicSensorArray* m_usArray;
};

#endif
