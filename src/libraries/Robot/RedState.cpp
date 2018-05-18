#include "RedState.h"
#include "CRobot.h"

const String RedState::s_RstateID = "RED";

// Constructor
RedState::RedState()
{
  //CMachineState::CMachineState();
}

RedState::~RedState()
{

}

// Generic functions
void RedState::update()
{

}

void RedState::run()
{
  RGBL led = TheRobot::Instance()->getPLED();
  Serial.println("Encendiendo: RED");
  digitalWrite(led.pinR, 255);
  digitalWrite(led.pinG, 0);
  digitalWrite(led.pinB, 0);
}

bool RedState::onEnter()
{
  bool success = true;

  m_isActive = true;
  Serial.println("Entering RED state!");

  return success;
}

bool RedState::onExit()
{
  m_isActive = false;

  return true;
}
