#include "CRobot.h"

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

  String str = m_usArray->toString();
  Serial.println(str);

  Serial.println("Making action");
};

bool CRobot::init()
{
  RedState* initState = (RedState*) malloc(sizeof(RedState));
  m_mindStates.push_back(initState);

  // TODO: Fix hardcoded pins
  m_pinB = 5; m_pinR = 6; m_pinG = 7;

  // Sensors and pin initalization
  // ==== RGB LED ===
  pinMode(m_pinR, OUTPUT);
  pinMode(m_pinG, OUTPUT);
  pinMode(m_pinB, OUTPUT);

  // ==== Ultrasonic Sensor Array ====
  m_usArray = (UltrasonicSensorArray*) malloc(sizeof(UltrasonicSensorArray(3)));
  // setPinsTo(int sensorId, int echo, int trigger)
  m_usArray->setPinsTo(0, 8, 9);
  m_usArray->setPinsTo(1, 10, 11);
  m_usArray->setPinsTo(2, 12, 13);
  m_usArray->initPins();

  Serial.println("Init done");
  return true;
};

void CRobot::handleEvents()
{
  m_usArray->measure();

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
int CRobot::getPinForColor(const char pcolor)
{
  switch (pcolor)
  {
    case 'R':
      return m_pinR;
    break;
    case 'G':
      return m_pinG;
    break;
    case 'B':
      return m_pinB;
    break;
    default:
      return 0;
  }
}
