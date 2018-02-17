// Primer sensor ultrasonido
const int trigPin1 = 11;
const int echoPin1 = 10;
// Segundo sensor ultrasonido
const int trigPin2 = 7;
const int echoPin2 = 6;
// Tercer sensor ultrasonido
const int trigPin3 = 9;
const int echoPin3 = 8;

// Variables
long duration1;
long duration2;
long duration3;
int distance1;
int distance2;
int distance3;

void setup() {
  // Inicializamos los pines del arduino como corresponda
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  
  Serial.begin(9600); // Para imprimir por pantalla.

}

void loop() {
  // Mantenemos el pin 'trig' a bajo voltaje por 2 microsegundos para evitar inconvenientes. 
  digitalWrite(trigPin1, LOW);
  digitalWrite(trigPin2, LOW);
  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);

  // Mantenemos en alto (HIGH) el pin 'trig' de cada sensor por 10 microsegundos.
  // Primero
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Leemos cada pin 'echo' que retorna la duración del recorrido de la onda de sonido.
  duration1 = pulseIn(echoPin1, HIGH); 
  
  // Segundo
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Leemos cada pin 'echo' que retorna la duración del recorrido de la onda de sonido.
  duration2 = pulseIn(echoPin2, HIGH);

  // Tercero
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
   // Leemos cada pin 'echo' que retorna la duración del recorrido de la onda de sonido.
  duration3 = pulseIn(echoPin3, HIGH);
  
  // Imprimimos por pantalla la duración
  Serial.print("Duration 1: ");
  Serial.print(duration1);
  Serial.print(" | Duration 2: ");
  Serial.print(duration2);
  Serial.print(" | Duration 3: ");
  Serial.println(duration3);
  

  // Calculamos la distancia sabiendo que la velocidad del sonido es 340 m/S o 0.035 cm/us y divimos entre 2 porque el viaje es de ida y vuelta.
  distance1 = duration1*0.034/2;
  distance2 = duration2*0.034/2;
  distance3 = duration3*0.034/2;
  
  // Imprimimos por pantalla la distancia
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" | Distance 2: ");
  Serial.print(distance2);
  Serial.print(" | Distance 3: ");
  Serial.println(distance3);
}
