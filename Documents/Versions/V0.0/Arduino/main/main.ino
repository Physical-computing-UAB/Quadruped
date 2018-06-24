#include <Servo.h>

float TODEG = 57.3; // Conversion from rad to deg

// ======================== Serial comunication variables ============================
String serialBuffer = "";  // Buffer for serial comunication
char received;             // Char received
String serialHeader;       // Header of the message (2 chars)
String serialBody;         // The body of the message recieved
// ===================================================================================


// ======================== Camera Servos ============================================
//Servo S_camH;   // Horizontal axis servo
//Servo S_camV;   // Vertical axis servo
int Ang_camH = 90;
int Ang_camV = 90;
// ===================================================================================


// ======================== Legs Servos & distances ===================================

// Distances of the leg
float coxa =  6.1;  // cm
float femur = 7.8;  // cm
float tibia = 13.6;  // cm

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
double offset1RF = 36;
double offset2RF = 105;
double offset3RF  = 45;

double offset1LF = 47;
double offset2LF = 45;
double offset3LF = 140;

double offset1LB = 51;
double offset2LB = 107;
double offset3LB = 45;

double offset1RB = 51;
double offset2RB = 83;
double offset3RB = 128;

// ===================================================================================


// ======================== Setup ====================================================
void setup() {
  Serial.begin(115200);

  // Attach servos
  /*S_camH.attach(4);
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
  RFx = 12;
  RFy = 8;
  RFz = 12;
  
  LFx = 12;
  LFy = 8;
  LFz = 12;

  RBx = 12;
  RBy = 8;
  RBz = 12;

  LBx = 12;
  LBy = 8;
  LBz = 12;

  movLegs(RFx,RFy,RFz, RBx,RBy,RBz, LFx,LFy,LFz, LBx,LBy,LBz, 10,10);
}
// ===================================================================================



// ======================== Functions called via Serial ===================================

// Set camera servo positions
void updateCServos(){
  //S_camH.write(Ang_camH);
  //S_camV.write(Ang_camV);
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
  Serial.print(" S");
  Serial.print(S_RF2.read());
  Serial.print(",");
  Serial.println(S_RF3.read());
}
void test2(String var){

  //Serial.print("Message CC received-> ");
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
  int a[12] = {10,-2,-1,-1,1,3,5,7,9,11,13,15};
  
  movLegs(RFx,RFy,a[4],      15,4,a[11],     LFx,LFy,a[10],        LBx,LBy,a[5],    div,time);
  movLegs(RFx,RFy,a[3],      15,4,0,      LFx,LFy,a[9],        LBx,LBy,a[6],    div,time);
  movLegs(RFx,RFy,a[2],      15,8,a[1],      LFx,LFy,a[8],        LBx,LBy,a[7],    div,time);
  
  movLegs(15,4,a[1],        RBx,RBy,a[2],    LFx,LFy,a[7],        LBx,LBy,a[8],    div,time);
  movLegs(15,4,a[0],        RBx,RBy,a[3],    LFx,LFy,a[6],        LBx,LBy,a[9],    div,time);
  movLegs(15,8,a[11],       RBx,RBy,a[4],    LFx,LFy,a[5],        LBx,LBy,a[10],    div,time);
  
  movLegs(RFx,RFy,a[10],      RBx,RBy,a[5],    LFx,LFy,a[4],        15,4,a[11],     div,time);
  movLegs(RFx,RFy,a[9],     RBx,RBy,a[6],    LFx,LFy,a[3],         15,4,0,      div,time);
  movLegs(RFx,RFy,a[8],      RBx,RBy,a[7],    LFx,LFy,a[2],       15,8,a[1],      div,time);
  
  movLegs(RFx,RFy,a[7],      RBx,RBy,a[8],    15,4,a[1],          LBx,LBy,a[2],    div,time);
  movLegs(RFx,RFy,a[6],      RBx,RBy,a[9],    15,4,a[0],          LBx,LBy,a[3],    div,time);
  movLegs(RFx,RFy,a[5],      RBx,RBy,a[10],    15,8,a[11],         LBx,LBy,a[4],    div,time);
}

void testIK1(int div, int time){
  movLegs(25,2,25,     RBx,RBy,RBz,     LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
  Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());
  
  movLegs(18,2,18,      RBx,RBy,RBz,      LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
  Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());

  movLegs(18,7,18,      RBx,RBy,RBz,      LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
  Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());

  movLegs(18,2,18,      RBx,RBy,RBz,      LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
  Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());

  movLegs(25,2,25,      RBx,RBy,RBz,      LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
  Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());
  
  movLegs(25,7,25,      RBx,RBy,RBz,      LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
  Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());

  movLegs(18,7,18,      RBx,RBy,RBz,      LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
  Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());

  movLegs(18,2,18,      RBx,RBy,RBz,      LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
  Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());
}


void testIK2(int div, int time){
  Serial.println("Centro");
  movLegs(13,8,13,     13,8,13,     13,8,13,        13,8,13,    div,time);
  Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());
  delay(300);
  Serial.println("Derecha");
  movLegs(18,8,0,     18,8,0,     18,8,0,        18,8,0,    div,time);
    Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());
  delay(300);/*
  Serial.println("Centro");
  movLegs(13,8,13,    RBx,RBy,RBz,     LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
    delay(300);
  Serial.println("Izquierda");
  movLegs(0,8,18,     RBx,RBy,RBz,     LFx,LFy,LFz,        LBx,LBy,LBz,    div,time);
    Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());
  delay(300);*/
}

void testIK3(int div, int time){
  Serial.println("Abajo");
  movLegs(13,5,13,     13,5,13,     13,5,13,        13,5,13,    div,time);
  delay(500);
  Serial.println("Arriba");
  movLegs(13,11,13,     13,11,13,     13,11,13,        13,11,13,    div,time);
  delay(500);
}


void testIK4(int div, int time){
  double xx=12;
  double zz=12;
  double yy=8;
  movLegs(xx,yy,zz,     xx,yy,zz,     xx,yy,zz,        xx,yy,zz,    div,time);
}


void testIK5(int div, int time){
  movLegs(15,3,-2,        RBx,RBy,RBz,    LFx,LFy,LFz,        LBx,LBy,LBz,   div,time);
  Serial.println(S_RF2.read());
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
    S_RF1.write(180 - (offset1RF + IKang1(RFx,RFy,RFz)));
    S_RF2.write(90 + offset2RF - IKang2(RFx,RFy,RFz));
    S_RF3.write((180+offset3RF) - IKang3(RFx,RFy,RFz));

    S_LF1.write(IKang1(LFx,LFy,LFz) + offset1LF);
    S_LF2.write(IKang2(LFx,LFy,LFz) + offset2LF - 90);
    S_LF3.write(offset3LF + (IKang3(LFx,LFy,LFz)-180));
    
    S_RB1.write(IKang1(RBx,RBy,RBz) + offset1RB);
    S_RB2.write(IKang2(RBx,RBy,RBz) + offset2RB - 90);
    S_RB3.write(offset3RB + (IKang3(RBx,RBy,RBz) - 180));
    
    S_LB1.write(180 - (offset1LB + IKang1(LBx,LBy,LBz)));
    S_LB2.write(90 + offset2LB - IKang2(LBx,LBy,LBz));
    S_LB3.write((180+offset3LB) - IKang3(LBx,LBy,LBz));
}

// Angle for servo 1
int IKang1(double x, double y, double z){
  double gamma = atan2(z, x);

    /*Serial.print("x: ");
        Serial.print(x);
        Serial.print(" z: ");
            Serial.print(z);
            Serial.print(" gamma: ");
  Serial.println(gamma*TODEG);*/
  
  return (gamma * TODEG);
}

// Angle for servo 2
int IKang2(double x, double y, double z){
  double g = atan2(z, x);
  double xz = (x-coxa)*cos(g) + (z-coxa)*sin(g);
  double hip = sqrt( pow(y,2) + pow((xz),2));
  
  double alpha1 = acos(y/hip);
  double alpha2 = acos( (pow(femur, 2) + pow(hip,2) - pow(tibia,2))/(2*femur*hip));
  double alpha = alpha1 + alpha2;
  return (alpha * TODEG);
}

// Angle for servo 3
int IKang3(double x, double y, double z){
  double g = atan2(z, x);
  double xz = (x-coxa)*cos(g) + (z-coxa)*sin(g);
  double hip = sqrt( pow(y,2) + pow((xz),2));
  
  double beta = acos(( pow(femur,2) + pow(tibia,2) - pow(hip,2) )/(2*tibia*femur));
          /*Serial.print("beta: ");
            Serial.println(beta * TODEG);*/
  return (beta * TODEG);
}
// ===================================================================================




void loop() {

  //readSerial();
  /*Serial.print("1: ");
  Serial.print(S_RF1.read());
  Serial.print(" 2: ");
  Serial.print(S_RF2.read());
  Serial.print(" 3: ");
  Serial.println(S_RF3.read());*/
  FullWalk1(30,30);
  //testIK5(50,30);
  
  delay(80);

  //a

}
