#include "UltrasonicSensor.h"
#include "matrix.h"
/*==================================
          Public Methods
  ==================================*/

// === Constructors ===
UltrasonicSensorArray::UltrasonicSensorArray()
{
  // By default the number of sensors is one(1).
  m_sensorNum = 1;
}

UltrasonicSensorArray::UltrasonicSensorArray(int num)
{
  m_sensorNum = num;
}

void UltrasonicSensorArray::init()
{
  m_sensorArray = new UltrasonicSensor[m_sensorNum];

  init_sensors();
  init_matrices();
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

float UltrasonicSensorArray::getDurationOf(int s) const
{
  return m_sensorArray[s].duration;
}

Coordinate UltrasonicSensorArray::get_target() const
{
  return m_target;
}

// === Miscellaneous ===
void UltrasonicSensorArray::measure()
{
  // This is what our sensors see, the time that the signal takes to go and return
  // to a from the target.

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
  if (sensor->distance < 150)
  {
    sensor->detect = 1;
  }else{
    sensor->detect = 0;
  }
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
    pinMode(m_sensorArray[s].echo, INPUT);
    pinMode(m_sensorArray[s].trigger, OUTPUT);
}

/*==================================
          Private Methods
  ==================================*/
void UltrasonicSensorArray::init_sensors()
{
  for(int i = 0; i<m_sensorNum; i++)
  {
    m_sensorArray[i].duration = 0;
    m_sensorArray[i].distance = 0;
    m_sensorArray[i].detect = 0;
  }
}

void UltrasonicSensorArray::init_matrices()
{
  Serial.println("Init matrices");
  // Init matrix A and its transpose
  m_A[0][0] = -1; m_A[0][1] = 1; m_A[0][2] = 0;
  m_A[1][0] = 0; m_A[1][1] = -1; m_A[1][2] = 1;
  mat_transpose(m_A, m_Atr, 2, 3); // A = A'
  Serial.println("A:");
  mat_display2x2(m_Atr, 3, 2);

  // Init matrix C and its inverse
  mat_multiply(m_A, m_Atr, m_C, 2, 3, 3, 2); // C = gAA'
  Serial.println("C:");
  mat_display2x2(m_C, 2, 2);
  m_C[0][0] *= pow(m_noiseAmp, 2);
  m_C[0][1] *= pow(m_noiseAmp, 2);
  m_C[1][0] *= pow(m_noiseAmp, 2);
  m_C[1][1] *= pow(m_noiseAmp, 2);
  Serial.println("C:");
  mat_display2x2(m_C, 2, 2);
  mat_inverse2x2(m_C, m_Cinv);
  Serial.println("Cinv:");
  mat_display2x2(m_Cinv, 2, 2);

  // Distances between sensors and Nominal Position m_nominalP and their angles;
  float d = 10;
  m_nominalP.x = 0; m_nominalP.y = 17.5;
  float Antenna0[2]; Antenna0[0] = -d; Antenna0[1] = 0;
  float Antenna1[2]; Antenna1[0] = 0; Antenna1[1] = 0;
  float Antenna2[2]; Antenna2[0] = d; Antenna2[1] = 0;

  float dist_0_nominalP = sqrt(pow((m_nominalP.x-Antenna0[0]),2)+pow((m_nominalP.y-Antenna0[1]),2));
  float dist_1_nominalP = sqrt(pow((m_nominalP.x-Antenna1[0]),2)+pow((m_nominalP.y-Antenna1[1]),2));
  float dist_2_nominalP = sqrt(pow((m_nominalP.x-Antenna2[0]),2)+pow((m_nominalP.y-Antenna2[1]),2));
  m_sensorArray[0].distance_nominalP = dist_0_nominalP;
  m_sensorArray[1].distance_nominalP = dist_1_nominalP;
  m_sensorArray[2].distance_nominalP = dist_2_nominalP;

  float angle_0_nominalP = acos(d/dist_0_nominalP);
  float angle_1_nominalP = acos(0/dist_1_nominalP);
  float angle_2_nominalP = acos(-d/dist_2_nominalP);

  // Init H matrix of coefficients
  m_H[0][0] = (cos(angle_1_nominalP) - cos(angle_0_nominalP))/m_c;
  m_H[0][1] = (sin(angle_1_nominalP) - sin(angle_0_nominalP))/m_c;
  m_H[1][0] = (cos(angle_2_nominalP) - cos(angle_1_nominalP))/m_c;
  m_H[1][1] = (sin(angle_2_nominalP) - sin(angle_1_nominalP))/m_c;
  mat_transpose2x2(m_H, m_Htr, 2, 2);
  mat_display2x2(m_H, 2, 2);

  delay(1000);
}

/*find_target function.
  Given some conditions will use the appropiate estimator for
  target detection.
Estimators available:
BLUE: Best Linear Unbiased Estimator
*/
Coordinate UltrasonicSensorArray::find_target()
{
  this->measure();
  // To find the target let us use different estimators or simple if/else conditions
  if (m_sensorArray[0].detect && m_sensorArray[1].detect && m_sensorArray[2].detect)
  {
    m_target = estimator_BLUE();
  }else{
    m_target = estimator_LINE();
  }

  return m_target;
}


// ================= ESTIMATORS AND DETECTION FUNCTIONS ===================
// Best Linear Unbiased Estimator
Coordinate UltrasonicSensorArray::estimator_BLUE()
{
  //init_matrices();
  // TDOA taus and Epsilon vector
  float tau0 = (m_sensorArray[0].distance - m_sensorArray[0].distance_nominalP)/m_c;
  float tau1 = (m_sensorArray[1].distance - m_sensorArray[1].distance_nominalP)/m_c;
  float tau2 = (m_sensorArray[2].distance - m_sensorArray[2].distance_nominalP)/m_c;

  float E[2][1]; // Column vector with TDOA information
  E[0][0] = tau1 - tau0;
  E[1][0] = tau2 - tau1;

  // BLUE Estimator
  // Denominator of estimator
  float denom[2][2], aux[2][2], aux2[2][2];
  float denomInv[2][2];
  mat_multiply2x2(m_Htr, m_Cinv, aux, 2, 2, 2, 2);
  mat_multiply2x2(aux, m_H, denom, 2, 2, 2, 2);
  mat_inverse2x2(denom, denomInv);

  // Numerator of estimator
  mat_multiply2x2(denomInv, aux, aux2, 2, 2, 2, 2);
  float theta[2][1];
  mat_multiplyCol(aux2, E, theta);

  Coordinate target;
  target.x = m_nominalP.x + theta[0][0];
  target.y = m_nominalP.y + theta[1][0];
  target.y = (target.y > 0) ? target.y : 0;
  return target;
}

// Find target using trigonometric functions
Coordinate UltrasonicSensorArray::estimator_LINE()
{
  Coordinate target;
  target.x = 0;
  target.y = m_sensorArray[1].distance;
  return target;
}
