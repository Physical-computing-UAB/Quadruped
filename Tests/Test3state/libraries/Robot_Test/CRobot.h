#ifndef CRobot_h
#define CRobot_h

// Include libraries.
#include <Arduino.h>
#include <UltrasonicSensor.h>
#include <vector>

#include "States/RedState.h"

class CRobot {
public:
  // Metodo Instance del Singleton
  static CRobot* Instance()
  {
    if (s_pInstance == 0)
    {
      s_pInstance = (CRobot*) malloc(sizeof(CRobot));
    }
  }

  // Destructor
  ~CRobot(){};

  bool init();
  void handleEvents();
  void update();
  void run();

  // State transitions
  void pushState(CMachineState* pState);
  void changeState(CMachineState* pState);
  void popState();

  // Getters & Setters
  int getPinForColor(const char pcolor);
  UltrasonicSensorArray* getUSA(){ return m_usArray; };

  void setUltrasonicSA( UltrasonicSensorArray* pUlrasonicSA ){ m_usArray = pUlrasonicSA; }const;

private:
  // Un constructor privado permite usar Singletons
  CRobot();
  static CRobot* s_pInstance;

  std::vector<CMachineState*> m_mindStates; // Stack
  UltrasonicSensorArray* m_usArray;
  int m_pinR;
  int m_pinG;
  int m_pinB;
};

#endif
