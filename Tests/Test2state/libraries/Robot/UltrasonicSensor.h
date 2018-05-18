#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

// Included libraries.
#include <Arduino.h>

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
  
  // Getters
  int getDistanceOf(int s) const; // Default is in centimeters.
  long getDurationOf(int s) const;

   // Miscellaneous
  void initPins();
  void measure();
  void measure(int s);
  String toString();
  
  // Setters
  void setPinsTo(int s, int echo, int trigger);

private:
  void initSensors();
  
  int m_sensorNum;
  UltrasonicSensor* m_sensorArray;
};

#endif
