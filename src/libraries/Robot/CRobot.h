#ifndef CRobot_h
#define CRobot_h

// Include libraries.
#include <Arduino.h>
#include <UltrasonicSensor.h>
#include "SerialHandler.h"
#include <vector>
#include "IKEngine.h"
#include <Servo.h>

// Peripherics
typedef struct{
    int alpha = 0;
    int beta = 0;
    Servo pan;
    Servo tilt;
} Camera;

class CMachineState;
class CRobot {
public:
  // Metodo Instance del Singleton
  static CRobot* Instance()
  {
    if (s_pInstance == 0)
    {
      Serial.println("Making place for Robot");
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
  // === Getters ===
  UltrasonicSensorArray* getUSA() const { return m_usArray; };
  IKEngine* getIKEngine() { return m_IKEngine; };
  Camera* getCamera() { return m_camera; };
  Coordinate getTarget() const { return m_target; };
  // === Setters ===
  void setUltrasonicSA( UltrasonicSensorArray* pUlrasonicSA ){ m_usArray = pUlrasonicSA; }const;

private:
  // Un constructor privado permite usar Singletons, eso es:
  // una sola instancia de CRobot en todo el programa
  CRobot();
  static CRobot* s_pInstance;

  // === Attributes ===
  std::vector<CMachineState*> m_mindStates; // Stack
  SerialHandler m_serialHandler;

  // Peripherics
  UltrasonicSensorArray* m_usArray;
  IKEngine* m_IKEngine;

  Camera* m_camera;
  Coordinate m_target;
};

typedef CRobot TheRobot;

#endif
