#include "CRobot.h"

#include <CStateAutonom.h>
#include <CStateControlled.h>

CRobot* CRobot::s_pInstance = 0;
const int NumberOfSensors = 3;
// === Constructor ===
CRobot::CRobot()
{
  m_serialHandler = SerialHandler();

  m_usArray = new UltrasonicSensorArray(NumberOfSensors);

  m_IKEngine = new IKEngine();

}

CRobot::~CRobot()
{
  delete m_usArray;
  delete m_IKEngine;
  delete s_pInstance;
}

// === Methods ===
bool CRobot::init()
{
  Serial.println("Preparing default state");
  pushState(new CStateAutonom());

  // ==== Sensors and pin initalization ====
  // === Ultrasonic Sensor Array ===
  m_usArray->init();
  // Set the pins to each sensor starting in '0'.
  // @args: sensor, echo, trigger.
  m_usArray->setPinsTo(0, 43, 45);
  m_usArray->setPinsTo(1, 47, 49);
  m_usArray->setPinsTo(2, 51, 53);

  // ==== Inverse Kinematic Engine ====
  /* @args: (int RF1, int RF2, int RF3,
        int LF1, int LF2, int LF3,
        int RB1, int RB2, int RB3,
        int LB1, int LB2, int LB3 */
  m_IKEngine->setPins(22, 24, 26,
                     28, 30, 32,
                     34, 36, 38,
                     40, 42, 44);
  // @args: Coxa, Femur, Tibia
  m_IKEngine->setDist(6.1, 7.7, 13.7);
  //m_IKEngine.setLimits();
  // Offset angles of each servo
  m_IKEngine->setAngleOffsets(36, 100, 43,
                            47, 46, 140,
                            51, 105, 39,
                            51, 46, 143);
  m_IKEngine->init();
  // ==== Camera ====
  m_camera->pan.attach(46); // Horizontal
  m_camera->tilt.attach(48); // Vertical


  Serial.println("Robot: Init done.");
  delay(2000);
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
    case 'c':
    // This command tells the robot to move the camera.
    // msg will contain the angles alpha and beta.
      int xxx;
      xxx = msg.substring(0,3).toInt();
      int yyy;
      yyy = msg.substring(3).toInt();

      if(xxx < 181 && xxx > -1 && yyy < 181 && yyy > -1){
        m_camera->alpha = xxx;
        m_camera->beta = yyy;
      }

      m_camera->pan.write(m_camera->alpha);
      m_camera->tilt.write(m_camera->beta);
    break;
    case 'C':
    // This command tells the robot to change its state to the desired state
    // msg contains the state identifier:
    // A: Autonomous
    // C: Controlled
      if (msg[0] == 'A')
      {
        changeState(new CStateAutonom());
      }else if(msg[0] == 'C')
      {
        changeState(new CStateControlled());
      }
    break;
    break;
    default:
        Serial.println("Comfortably numb");
  }

  // Ultrasonic Sensor Array
  m_target = m_usArray->find_target();
  Serial.println("Target at: " + String(m_target.x) + " " + String(m_target.y));
  Serial.println(" - Done handling events!");
};

void CRobot::update()
{
  m_mindStates.back()->update();
  Serial.println("Robot: Update");
};

void CRobot::run()
{
  m_mindStates.back()->run();

  String str = m_usArray->toString();
  Serial.println(str);

  Serial.println("Robot: Run");
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
      delete m_mindStates.back();
      m_mindStates.pop_back();
    }
  }
};

// === Getters & Setters ===
