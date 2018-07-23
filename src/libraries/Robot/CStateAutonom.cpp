#include "CStateAutonom.h"
#include "CRobot.h"

const String CStateAutonom::s_AUTOstateID = "Autonom";

// Class methods
void CStateAutonom::handleEvents()
{

}

void CStateAutonom::update()
{
  TheRobot* Robot = TheRobot::Instance();
  Coordinate target = Robot->getTarget();

  Robot->getIKEngine()->setDir(1);
  if (Robot->getUSA()->getDistanceOf(1) < 30)
  {
    Robot->getIKEngine()->setDir(0);
    Serial.println("Stop.");
  }

  Robot->getIKEngine()->updateIK();
}

void CStateAutonom::run()
{
  TheRobot::Instance()->getIKEngine()->move();
}

bool CStateAutonom::onEnter()
{
  Serial.println("Entering Autonomous state!");
  m_isActive = true;

  return true;
}

bool CStateAutonom::onExit()
{
  m_isActive = false;

  return true;
}
