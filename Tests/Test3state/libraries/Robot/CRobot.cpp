#include "CRobot.h"

#include "States/RedState.h"

CRobot* CRobot::s_pInstance = 0;

// === Constructor ===
CRobot::CRobot()
{
  if (init())
  {
    Serial.println("Robot created.");
  }
}

// === Methods ===
void CRobot::run()
{
  m_mindStates.back()->run();
  Serial.println("Making action");
};

bool CRobot::init()
{
  RedState* initState = (RedState*) malloc(sizeof(RedState));
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

// Getters & Setters
int getPinForColor(const char pcolor)
{
  switch (pcolor)
  {
    case "R":
      return pinR;
    break;
    case "G":
      return pinG;
    break;
    case "B":
      return pinB;
    break;
    default:
      return 0;
  }
}

void CRobot::setLedColor(int R, int G, int B)
{
  analogWrite(pinR, R);
  analogWrite(pinG, G);
  analogWrite(pinB, B);
}
