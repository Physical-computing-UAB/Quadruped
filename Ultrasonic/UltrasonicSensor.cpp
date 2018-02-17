#include "UltrasonicSensor.hpp"

// Constructors
UltrasonicSensor::UltrasonicSensorArray()
{
  m_array = new UltrasonicSensor[3];
}

  UltrasonicSensorArray(int num);
  // Destructor
  ~UltrasonicSensorArray();
