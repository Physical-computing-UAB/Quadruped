#ifndef CRobot_h
#define CRobot_h

// Include libraries.
#include <Arduino.h>
#include <UltrasonicSensor.h>
#include "SerialHandler.h"
#include "RedState.h"
#include <vector>

// Peripherics

//
typedef struct{
  // R
  int pinR;
  int valueR;
  // G
  int pinG;
  int valueG;
  // B
  int pinB;
  int valueB;
}RGBL;

class CMachineState;
class CRobot {
public:
  // Metodo Instance del Singleton
  static CRobot* Instance()
  {
    if (s_pInstance == 0)
    {
      Serial.println("Making place for Robot");
      //s_pInstance = (CRobot*) malloc(sizeof(CRobot));
      s_pInstance = new CRobot();
    }
    return s_pInstance;
  }

  // Destructor
  ~CRobot();

  bool init();
  void handleEvents();
  void update();
  void run();

  // State transitions
  void pushState(CMachineState* pState);
  void changeState(CMachineState* pState);
  void popState();

  // === Getters & Setters ===
  // Peripherics
  RGBL getPLED() const { return p_led;};
  UltrasonicSensorArray* getUSA() const { return m_usArray; };

  void setUltrasonicSA( UltrasonicSensorArray* pUlrasonicSA ){ m_usArray = pUlrasonicSA; }const;

private:
  // Un constructor privado permite usar Singletons, eso es:
  // una sola instancia de CRobot en todo el programa
  CRobot();
  static CRobot* s_pInstance;

  // === Attributes ===
  std::vector<CMachineState*> m_mindStates; // Stack
  UltrasonicSensorArray* m_usArray;

  SerialHandler m_serialHandler;

  // Peripherics
  RGBL p_led;
};

typedef CRobot TheRobot;

#endif
