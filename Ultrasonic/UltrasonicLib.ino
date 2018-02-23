#include <UltrasonicSensor.h>

// Create an instance of the array. In this case we will use 3 sensors.
UltrasonicSensorArray sensorArray(3);

void setup() {

  // Set the pins to each sensor starting in '0'. @args: sensor, echo, trigger.
  sensorArray.setPinsTo(0, 10, 11);
  sensorArray.setPinsTo(1, 6, 7);
  sensorArray.setPinsTo(2, 8, 9);

  // Setting Echos to OUPUT and Triggers to INPUT mode.
  sensorArray.initPins();
  
  Serial.begin(9600); // Para imprimir por pantalla.

}

void loop() {

  sensorArray.measure();

  String str = sensorArray.toString();
  Serial.println(str);

}
