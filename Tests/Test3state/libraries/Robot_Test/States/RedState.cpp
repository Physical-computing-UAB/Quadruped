#include "RedState.h"

#include "../CRobot.h"

const String RedState::s_stateID = "RED";

// Generic functions
void RedState::handleEvents()
{

}

void RedState::update()
{

}

void RedState::run()
{
  analogWrite(CRobot::Instance()->getPinForColor("R"), 255);
  analogWrite(CRobot::Instance()->getPinForColor("G"), 0);
  analogWrite(CRobot::Instance()->getPinForColor("B"), 0);

}

bool RedState::onEnter()
{
  m_isActive = true;
  bool success = true;
}

bool RedState::onExit()
{
  m_isActive = false;

  return true;
}
