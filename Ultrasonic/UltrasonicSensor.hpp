#ifndef Sensor_hpp
#define Sensor_hpp

// Included libraries.
#include "Arduino.h"

struct UltrasonicSensor
{
  int echo;
  int trigger;
  long duration;
  int distance;
};

class UltrasonicSensorArray {
public:
  // Constructors
  UltrasonicSensorArray();
  UltrasonicSensorArray(int num);
  // Destructor
  ~UltrasonicSensorArray();

  // GETTERS

  // SETTERS
  void setPinsTo(int sensor, int echo, int trigger);
  
private:
  void initSensors();
  
  int m_sensorNum;
  UltrasonicSensor* m_sensorArray;

  

};

#endif
