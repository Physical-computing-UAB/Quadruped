#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

// Included libraries.
#include <Arduino.h>

// ==== Useful structures ====
struct UltrasonicSensor
{
  // The position of the sensor in the sensor network
  float x;
  float y;
  // float z;
  // Variables used by the arduino to measure distance
  int echo;
  int trigger;
  float duration;
  float distance;
  float distance_nominalP;
  // Boolean variable to check if sensor 'sees' something
  bool detect;
};

struct Coordinate
{
  float x;
  float y;
  //TODO: float Z;
};

class UltrasonicSensorArray {
public:
  // Constructors
  UltrasonicSensorArray();
  UltrasonicSensorArray(int num);
  void init();
  // Destructor
  ~UltrasonicSensorArray();

  // Getters
  int getDistanceOf(int s) const; // Default is in centimeters.
  float getDurationOf(int s) const;
  Coordinate get_target() const;

   // Miscellaneous
  Coordinate find_target();
  void init_matrices();
  void measure();
  void measure(int s);
  String toString();

  // Setters
  void setPinsTo(int s, int echo, int trigger);

private:
  void init_sensors();

  // Position estimators
  Coordinate estimator_BLUE();
  Coordinate estimator_LINE();

  int m_sensorNum; // Number of sensors
  UltrasonicSensor* m_sensorArray; // Array containing sensors
  Coordinate m_target; // The x and y value of the target (if any)

  // ===== Values used for the BLUE estimator =====
  // === Useful variables ===
  float m_c = 340.29; // The speed of sound [m/s]
  float m_noiseAmp = 0.0001; // Very tiny, deprecable noise
  Coordinate m_nominalP;

  float m_A[2][3], m_Atr[3][2]; // Matrix A from noise factorization in TDOA
  float m_C[2][2], m_Cinv[2][2]; // Covariance Matrix C
  float m_H[2][2], m_Htr[2][2]; // Coefficients matrix H

};

#endif
