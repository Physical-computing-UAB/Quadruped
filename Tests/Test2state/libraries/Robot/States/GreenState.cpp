#include "GreenState.h"

const String GreenState::s_GstateID = "GREEN";

// Generic functions
void GreenState::update()
{

}

void GreenState::run()
{
  struct RGBL* led = TheRobot::Instance()->getPLED();

  digitalWrite(led->pinR, 0);
  digitalWrite(led->pinG, 255);
  digitalWrite(led->pinB, 0);
}

bool GreenState::onEnter()
{
  Serial.println("Entering GREEN state!");
  m_isActive = true;

  return true;
}

bool GreenState::onExit()
{
  m_isActive = false;

  return true;
}
