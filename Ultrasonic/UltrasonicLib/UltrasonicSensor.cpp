#include "UltrasonicSensor.h"

/*==================================
          Public Methods
  ==================================*/

// === Constructors ===
UltrasonicSensorArray::UltrasonicSensorArray()
{
  // By default the number of sensors is one(1).
  m_sensorNum = 1;
  initSensors();
}

UltrasonicSensorArray::UltrasonicSensorArray(int num)
{
  m_sensorNum = num;
  initSensors();
}

// === Destructor ===
UltrasonicSensorArray::~UltrasonicSensorArray()
{
  delete m_sensorArray;
}

// === Getters ===
int UltrasonicSensorArray::getDistanceOf(int s) const
{
    return m_sensorArray[s].distance;
}
  
long UltrasonicSensorArray::getDurationOf(int s) const
{
    return m_sensorArray[s].duration;
}

// === Miscellaneous ===
void UltrasonicSensorArray::initPins()
{
    for(int i = 0; i < m_sensorNum; i++)
    {
        pinMode(m_sensorArray[i].echo, INPUT);
        pinMode(m_sensorArray[i].trigger, OUTPUT);
    }
}

void UltrasonicSensorArray::measure()
{
    for(int i = 0; i < m_sensorNum; i++)
    {
      this->measure(i);
    }
}


void UltrasonicSensorArray::measure(int s)
{
  UltrasonicSensor* sensor = &m_sensorArray[s];
  // Keep the trigger low for 2 microseconds for 'safety'
   digitalWrite(sensor->trigger, LOW);
   delayMicroseconds(2);
   
  // Keep the trigger pin HIGH for 10 microseconds.
  digitalWrite(sensor->trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor->trigger, LOW);
  
  // Read the echo pin which returns the duration of the sent signal.
  sensor->duration = pulseIn(sensor->echo, HIGH);
  sensor->distance = sensor->duration*0.034/2;
}

String UltrasonicSensorArray::toString()
{
  String data = "";

  for(int i = 0; i < m_sensorNum; i++)
  {
    data = String(data + String(i) + "- Distance: " + String(getDistanceOf(i)) + " | Duration: " + String(getDurationOf(i)) + "\n");
  }

  return data;
}

// === Setters ===
void  UltrasonicSensorArray::setPinsTo(int s, int echo, int trigger)
{
    m_sensorArray[s].echo = echo;
    m_sensorArray[s].trigger = trigger;  
}
  
/*==================================
          Private Methods
  ==================================*/
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

