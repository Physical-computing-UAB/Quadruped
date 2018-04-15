#include "CRobot.h"

#include "States/CAutonomusState.h"

// === Constructor ===
CRobot::CRobot()
{
  if (init())
  {
    Serial.println("Robot created.");
  }
}

// === Methods ===
void CRobot::behave()
{
  m_mindStates.back()->behave();
  Serial.println("Making action");
};

bool CRobot::init()
{
  CAutonomusState* initState = (CAutonomusState*) malloc(sizeof(CAutonomusState));
  m_mindStates.push_back(initState);

  Serial.println("Init done");
  return true;
};

void CRobot::handleEvents()
{
  m_mindStates.back()->handleEvents();
  Serial.println("Handling events");
};

void CRobot::update()
{
  m_mindStates.back()->update();
  Serial.println("Updating..");
};


// === State transitions ===
void CRobot::pushState(CMachineState* pState)
{
  m_mindStates.push_back(pState);

  m_mindStates.back()->onEnter();
};

void CRobot::changeState(CMachineState* pState)
{
  if (!m_mindStates.empty()) // If there is a state
  {
    if(m_mindStates.back()->getStateID() == pState->getStateID()) // If they are the same, do nothing
    {
      return;
    }

    if(m_mindStates.back()->onExit()) // if exiting succesfull
    {
      m_mindStates.back()->setActiveState(false);
      free(m_mindStates.back());
      m_mindStates.pop_back();
    }
  }

  pushState(pState);
};

void CRobot::popState()
{
  if (!m_mindStates.empty())
  {
    if (m_mindStates.back()->onExit())
    {
      free(m_mindStates.back());
      m_mindStates.pop_back();
    }
  }
};
