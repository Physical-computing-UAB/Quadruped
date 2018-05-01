#ifndef CMachineState_h
#define CMachineState_h

#define MAX_MATRIX_SIZE 32

// Include libraries.
#include <Arduino.h>
#include <CRobot.h>

class CMachineState
{
  public:
    // Constructor
    CMachineState();
    virtual ~CMachineState();

    // Generic functions
    virtual void handleEvents() = 0;
    virtual void update() = 0;
    virtual void run() = 0;

    virtual bool onEnter() = 0;
    virtual bool onExit() = 0;

    // Getters & Setters
    String getStateID() const { return m_stateID; };

    // TODO: See if this can be done by CRobot
    // void setMap(int (*pMap)[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]) { m_map = pMap; };
    void setActiveState(bool pActive) { m_isActive = pActive; };
  protected:
      String m_stateID;
      bool m_isActive;

      //int (*m_map)[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
};

#endif
