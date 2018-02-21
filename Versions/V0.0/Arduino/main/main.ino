#include <Servo.h>

float TODEG = 57.3; // Conversion from rad to deg

// ======================== Serial comunication variables ============================
String serialBuffer = "";  // Buffer for serial comunication
char received;             // Char received
String serialHeader;       // Header of the message (2 chars)
String serialBody;         // The body of the message recieved
// ===================================================================================


// ======================== Camera Servos ============================================
Servo S_camH;   // Horizontal axis servo
Servo S_camV;   // Vertical axis servo
int Ang_camH = 90;
int Ang_camV = 90;
// ===================================================================================


// ======================== Legs Servos & distances ===================================

// Distances of the leg
float coxa =  3.7;  // cm
float femur = 5.9;  // cm
float tibia = 9.45;  // cm

// Servos
Servo S_RF1;
Servo S_RF2;
Servo S_RF3;

Servo S_LF1;
Servo S_LF2;
Servo S_LF3;

Servo S_RB1;
Servo S_RB2;
Servo S_RB3;

Servo S_LB1;
Servo S_LB2;
Servo S_LB3;

// IK coords of each leg
double RFx, RFy, RFz;
double LFx, LFy, LFz;
double RBx, RBy, RBz;
double LBx, LBy, LBz;

// Offset angles of each servo
double offset1RF = 135;
double offset2RF = 180;
double offset3RF  = 45;

double offset1LF = 45;
double offset2LF = 0;
double offset3LF = 225;

double offset1LB = 135;
double offset2LB = 180;
double offset3LB = 45;

double offset1RB = 45;
double offset2RB = 0;
double offset3RB = 225;

// ===================================================================================


// ======================== Setup ====================================================
void setup() {
  Serial.begin(115200);

  // Attach servos
  /*S_camH.attach(2);
  S_camV.attach(3);*/
  
  S_RF1.attach(2);
  S_RF2.attach(3);
  S_RF3.attach(4);
  S_LF1.attach(5);
  S_LF2.attach(6);
  S_LF3.attach(7);
  S_RB1.attach(8);
  S_RB2.attach(9);
  S_RB3.attach(10);
  S_LB1.attach(11);
  S_LB2.attach(12);
  S_LB3.attach(13);

  // Init coords
  RFx = 8;
  RFy = 8;
  RFz = 8;
  
  LFx = 8;
  LFy = 8;
  LFz = 8;

  RBx = 8;
  RBy = 8;
  RBz = 8;

  LBx = 8;
  LBy = 8;
  LBz = 8;

  movLegs(RFx,RFy,RFz, RBx,RBy,RBz, LFx,LFy,LFz, LBx,LBy,LBz, 10,10);
}
// ===================================================================================



// ======================== Functions called via Serial ===================================

// Set camera servo positions
void updateCServos(){
  S_camH.write(Ang_camH);
  S_camV.write(Ang_camV);
}


// Move camera servos to the given angles
void controlCamera(String var){
  
  // Var = "xxxyyy" -> xxx = servo angle 1, yyy = servo angle 2

  int xxx = var.substring(0,3).toInt();
  int yyy = var.substring(3).toInt();

  if(xxx < 181 && xxx > -1 && yyy < 181 && yyy > -1){
    Ang_camH = xxx;
    Ang_camV = yyy;
  }
  updateCServos();
}


void test(String var){
  Serial.print(var);
  int x = var.substring(0,2).toInt();
  int y = var.substring(2,4).toInt();
  int z = var.substring(4).toInt();

  movLegs(x,y,z, RBx,RBy,RBz, LFx,LFy,LFz, LBx,LBy,LBz, 20,50);
}
void test2(String var){

  Serial.print("Message CC received-> ");
  FullWalk1(20,50);
}

// ===================================================================================


// ======================== Read serial communication ===============================

void readSerial(){

  while(Serial.available() > 0){
      
    received = Serial.read();
    
    if(received == ';'){              // End of the current message
  
      //Get message
      serialHeader = serialBuffer.substring(0, 2);
      serialBody = serialBuffer.substring(2);
  
      //Process header
      if(serialHeader == "cc"){   // Camera control
        controlCamera(serialBody);
      }else
      if(serialHeader == "t1"){   // Test
        test(serialBody);
      }else
      if(serialHeader == "t2"){
        test2(serialBody);
      }
  
      // Clear the buffer
      serialBuffer = "";

      // Send Arduino Ready signal
      Serial.println("1");
      
    }else{          // Not the end of current message
      serialBuffer += received; //Add the received char to the buffer
    }
  }
}
// ===================================================================================



// ======================== Movements routines =======================================

void FullWalk1(int div, int time){
  movLegs(8,7,RFz,      8,2,15,     8,7,LFz,        8,7,LBz,    div,time);
  movLegs(8,7,RFz,      8,2,6,      8,7,LFz,        8,7,LBz,    div,time);
  movLegs(8,7,RFz,      8,7,0,      8,7,LFz,        8,7,LBz,    div,time);
  
  movLegs(8,2,2,        8,7,RBz,    8,7,LFz,        8,7,LBz,    div,time);
  movLegs(8,2,8,        8,7,RBz,    8,7,LFz,        8,7,LBz,    div,time);
  movLegs(8,7,18,       8,7,RBz,    8,7,LFz,        8,7,LBz,    div,time);
  
  movLegs(8,7,RFz,      8,7,RBz,    8,7,LFz,        8,2,15,     div,time);
  movLegs(8,7,RFz,      8,7,RBz,    8,7,LFz,        8,2,6,      div,time);
  movLegs(8,7,RFz,      8,7,RBz,    8,7,LFz,        8,7,0,      div,time);
  
  movLegs(8,7,RFz,      8,7,RBz,    8,2,2,          8,7,LBz,    div,time);
  movLegs(8,7,RFz,      8,7,RBz,    8,2,8,          8,7,LBz,    div,time);
  movLegs(8,7,RFz,      8,7,RBz,    8,7,18,         8,7,LBz,    div,time);
}
// ===================================================================================


// ======================== IK Movements functions =============================================

// Linearly move all legs to the given coordinates
void movLegs(double t_RFx, double t_RFy, double t_RFz, double t_RBx, double t_RBy, double t_RBz, double t_LFx, double t_LFy, double t_LFz, double t_LBx, double t_LBy, double t_LBz, int div, int time){
  
  // div -> divisions (number of movements to reach the new coordinates)
  // time-> time between each sub-movement

  double subxRF = (t_RFx - RFx)/div;  // Increment to be added to the current position to reach the destination in "div" movements
  double subyRF = (t_RFy - RFy)/div;
  double subzRF = (t_RFz - RFz)/div;
  double subxRB = (t_RBx - RBx)/div;
  double subyRB = (t_RBy - RBy)/div;
  double subzRB = (t_RBz - RBz)/div;
  double subxLF = (t_LFx - LFx)/div;
  double subyLF = (t_LFy - LFy)/div;
  double subzLF = (t_LFz - LFz)/div;
  double subxLB = (t_LBx - LBx)/div;
  double subyLB = (t_LBy - LBy)/div;
  double subzLB = (t_LBz - LBz)/div;

  // Do the "div" movements
  for(int i = 0; i < div; i++){
    RFx += subxRF;
    RFy += subyRF;
    RFz += subzRF;
    RBx += subxRB;
    RBy += subyRB;
    RBz += subzRB;
    LFx += subxLF;
    LFy += subyLF;
    LFz += subzLF;
    LBx += subxLB;
    LBy += subyLB;
    LBz += subzLB;

    updateLServos();
    
    delay(time);
  } 
}



// Linearly move the body
void moveAll(double x, double y, double z, int div, int time){
  
  double subx = x/div;
  double suby = y/div;
  double subz = z/div;

  for(int i = 0; i < div; i++){
    RFx += subx;
    RFy += suby;
    RFz -= subz;

    LFx -= subx;
    LFy += suby;
    LFz -= subz;

    RBx += subx;
    RBy += suby;
    RBz += subz;

    LBx -= subx;
    LBy += suby;
    LBz += subz;

    updateLServos();
    
    delay(time);
  }
}
// ===================================================================================

// ======================== IK functions =============================================
void updateLServos(){
    // Set the servos to their coords with IK functions
    S_RF1.write(offset1RF - IKang1(RFx,RFy,RFz));
    S_RF2.write(offset2RF - IKang2(RFx,RFy,RFz));
    S_RF3.write(IKang3(RFx,RFy,RFz) - offset3RF);

    S_LF1.write(IKang1(LFx,LFy,LFz) + offset1LF);
    S_LF2.write(IKang2(LFx,LFy,LFz) - offset2LF);
    S_LF3.write(offset3LF - IKang3(LFx,LFy,LFz));

    S_RB1.write(IKang1(RBx,RBy,RBz) + offset1RB );
    S_RB2.write(IKang2(RBx,RBy,RBz) - offset2RB);
    S_RB3.write(offset3RB - IKang3(RBx,RBy,RBz));
    
    S_LB1.write(offset1LB - IKang1(LBx,LBy,LBz));
    S_LB2.write(offset2LB - IKang2(LBx,LBy,LBz));
    S_LB3.write(IKang3(LBx,LBy,LBz) - offset3LB);
}

// Angle for servo 1
int IKang1(double x, double y, double z){
  double gamma = atan2(z, x);
  return (gamma * TODEG);
}

// Angle for servo 2
int IKang2(double x, double y, double z){
  double hip = sqrt( pow(y,2) + pow((x - coxa),2));
  double alpha1 = acos(y/hip);
  double alpha2 = acos( (pow(tibia,2) - pow(femur, 2) - pow(hip,2))/(-2*femur*hip));
  double alpha = alpha1 + alpha2;
  return (alpha * TODEG);
}

// Angle for servo 3
int IKang3(double x, double y, double z){
  double hip = sqrt( pow(y,2) + pow((x - coxa),2));
  double beta = acos(( pow(hip,2) - pow(tibia,2) - pow(femur,2))/(-2*tibia*femur));
  return (beta * TODEG);
}
// ===================================================================================




void loop() {

  readSerial();
  
  delay(10);

}
