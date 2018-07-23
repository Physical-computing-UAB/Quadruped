#include "CStateControlled.h"
#include "CRobot.h"

const String CStateControlled::s_CONTROLLEDstateID = "Controlled";

// Constructor
CStateControlled::CStateControlled()
{

}

CStateControlled::~CStateControlled()
{

}

// Generic functions
void CStateControlled::update()
{

}

void CStateControlled::run()
{

}

bool CStateControlled::onEnter()
{
  bool success = true;

  m_isActive = true;
  Serial.println("Entering Controlled state!");

  return success;
}

bool CStateControlled::onExit()
{
  m_isActive = false;

  return true;
}
