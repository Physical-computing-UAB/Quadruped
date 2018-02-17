#ifndef Sensor_hpp
#define Sensor_hpp

// Included libraries.
#include "Arduino.h"

struct UltrasonicSensor
{
  const int echo;
  const int trigger;
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

  
private:
  int m_sensorNum;
  UltrasonicSensor* m_array;

};

#endif
