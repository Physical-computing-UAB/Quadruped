#include <Servo.h>

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




void setup() {
  Serial.begin(115200);

  // Attach servos
  S_camH.attach(2);
  S_camV.attach(3);
}



// Set servo positions
void updateServos(){
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
}


void fooBB(String var){

  // Process var.....
  Serial.print("Message BB received-> ");
  Serial.println(var);
}
void fooCC(String var){

  // Process var.....
  Serial.print("Message CC received-> ");
  Serial.println(var);
}




// Read char by char recieved by the usb serial comunication
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
      if(serialHeader == "bb"){
        fooBB(serialBody);
      }else
      if(serialHeader == "cc"){
        fooCC(serialBody);
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




void loop() {

  readSerial();
  updateServos();

  delay(10);

}
