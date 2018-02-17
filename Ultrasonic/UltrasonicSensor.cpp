#include "UltrasonicSensor.hpp"

/*==============
  Public Methods
  ==============*/

// Constructors
UltrasonicSensorArray::UltrasonicSensorArray()
{
  m_sensorNum = 3;
  initSensors();
}

UltrasonicSensorArray::UltrasonicSensorArray(int num)
{
  m_sensorNum = num;
  initSensors();
}

// Destructor
UltrasonicSensorArray::~UltrasonicSensorArray()
{
  delete m_sensorArray;
}

/*===============
  Private Methods
  ===============*/
void UltrasonicSensorArray::initSensors()
{
  m_sensorArray = new UltrasonicSensor[m_sensorNum];

  for(int i = 0; i<m_sensorNum; i++)
  {
    m_sensorArray[i].echo = 0;
    m_sensorArray[i].trigger = 0;
    m_sensorArray[i].duration = 0;
    m_sensorArray[i].distance = 0;
  }
}

