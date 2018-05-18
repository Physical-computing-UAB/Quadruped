#include "BlueState.h"
#include "CRobot.h"

const String BlueState::s_BstateID = "Blue";

// Generic functions
void BlueState::update()
{

}

void BlueState::run()
{
  RGBL led = TheRobot::Instance()->getPLED();
  Serial.println("Encendiendo: Blue");
  digitalWrite(led.pinR, 0);
  digitalWrite(led.pinG, 255);
  digitalWrite(led.pinB, 0);
}

bool BlueState::onEnter()
{
  Serial.println("Entering Blue state!");
  m_isActive = true;

  return true;
}

bool BlueState::onExit()
{
  m_isActive = false;

  return true;
}
