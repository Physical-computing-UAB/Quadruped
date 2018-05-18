#ifndef CMachineState_h
#define CMachineState_h

#include <Arduino.h>
#define MAX_MATRIX_SIZE 32

class CMachineState
{
  public:
    // Constructor
    CMachineState();
    ~CMachineState(){};

    // Generic functions
    virtual void update() = 0;
    virtual void run() = 0;

    virtual bool onEnter() = 0;
    virtual bool onExit() = 0;

    // Getters & Setters
    virtual String getStateID() const = 0;

    // TODO: See if this can be done by CRobot
    // void setMap(int (*pMap)[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE]) { m_map = pMap; };
    void setActiveState(bool pActive) { m_isActive = pActive; };
  protected:
      bool m_isActive;

      //int (*m_map)[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
};

#endif
