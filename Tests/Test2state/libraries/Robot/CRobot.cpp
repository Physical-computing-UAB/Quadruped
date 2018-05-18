#include "CRobot.h"

#include <RedState.h>
#include <BlueState.h>

CRobot* CRobot::s_pInstance = 0;

// === Constructor ===
CRobot::CRobot()
{
  m_serialHandler = SerialHandler();

}

CRobot::~CRobot()
{
  delete m_usArray;
  delete s_pInstance;
}

// === Methods ===
void CRobot::run()
{
  m_mindStates.back()->run();

  //String str = m_usArray->toString();
  //Serial.println(str);

  Serial.println("Run");
};

bool CRobot::init()
{
  p_led = {.pinR = 3, .valueR = 255,
          .pinG = 5, .valueG = 255,
          .pinB = 6, .valueB = 255};

  Serial.println("Preparing default state");
  pushState(new RedState());

  // ==== Sensors and pin initalization ====
  // === RGB LED ==
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  // === Ultrasonic Sensor Array ===
  m_usArray = new UltrasonicSensorArray();
  m_usArray->setPinsTo(0, 8, 9);
  m_usArray->setPinsTo(1, 10, 11);
  m_usArray->setPinsTo(2, 12, 13);
  m_usArray->initPins();

  Serial.println("Init done, Robot created.");
  return true;
};

void CRobot::handleEvents()
{
  // La idea es que aqui se gestionen los eventos de cada periferico.
  // Que se 'lean' los pines que interesen
  Serial.println("Handling events");

  // send data only when you receive data:
  String msg;
  char head = m_serialHandler.readSerial(msg);
  Serial.println(msg[0]);

  switch (head) {
    case 'C':
      if (msg[0] == 'b')
      {
        changeState(new BlueState());
      }else if(msg[0] == 'r')
      {
        changeState(new RedState());
      }
    break;
    case 'M':
        Serial.println("Moviendo");
    break;
    default:
        Serial.println("Nothing to do");
  }

  // Ultrasonic Sensor Array
  // TODO: m_usArray->measure();
  delay(1000);
  Serial.println(" - Done handling events!");
};

void CRobot::update()
{
  //m_mindStates.back()->update();
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
      delete m_mindStates.back();
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
      //free(m_mindStates.back());
      delete m_mindStates.back();
      m_mindStates.pop_back();
    }
  }
};

// === Getters & Setters ===
