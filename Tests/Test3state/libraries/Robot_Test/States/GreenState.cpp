#include "GreenState.h"

#include "../CRobot.h"

const String GreenState::s_stateID = "GREEN";

// Generic functions
void GreenState::handleEvents()
{

}

void GreenState::update()
{

}

void GreenState::run()
{
  analogWrite(CRobot::Instance()->getPinForColor("R"), 0);
  analogWrite(CRobot::Instance()->getPinForColor("G"), 255);
  analogWrite(CRobot::Instance()->getPinForColor("B"), 0);

}

bool GreenState::onEnter()
{
  m_isActive = true;
  bool success = true;
}

bool GreenState::onExit()
{
  m_isActive = false;

  return true;
}
