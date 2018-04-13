#ifndef CRobot_h
#define CRobot_h

// Include libraries.
#include <Arduino.h>
#include <UltrasonicSensor.h>

class CRobot {
public:
  // Constructor
  CRobot (){};
  // Destructor
  ~CRobot(){};

  // Miscellaneous
  virtual bool init(){ Serial.println("Init done"); return true; };
  virtual void handleEvents(){ Serial.println("Handling events"); };
  virtual void update(){ Serial.println("Updating.."); };
  virtual void move(){ Serial.println("Init done"); };

  // Getters & Setters
  UltrasonicSensorArray getUSA(){ return m_usArray; };

  void setUSA( UltrasonicSensorArray& usarray ){}const;
private:
  UltrasonicSensorArray m_usArray;
};

#endif
