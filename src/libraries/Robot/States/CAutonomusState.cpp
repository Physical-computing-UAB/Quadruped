#include "CAutonomusState.h"

// === Constructors ===
CAutonomusState::CAutonomusState()
{
  CMachineState::CMachineState();
  m_stateID = "AUTONOM";
}

CAutonomusState::~CAutonomusState()
{

}

// === Methods ===
void CAutonomusState::behave()
{

};

void CAutonomusState::handleEvents()
{

};

void CAutonomusState::update()
{

};

bool CAutonomusState::onEnter()
{

};

bool CAutonomusState::onExit()
{

};
