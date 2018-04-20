#include "SerialHandler.h"



// ===============================================================================
// ================== Initializations ============================================
// ===============================================================================

// Constaructor
SerialHandler::SerialHandler()
{
	// TODO
}

char SerialHandler::readSerial(String &msg){
	
	while(Serial.available() > 0){
		  
		received = Serial.read();
		
		if(received == ';'){              // End of the current message
		  //Get message
		  serialHeader = serialBuffer[0];
		  serialBody = serialBuffer.substring(1);
		  
		  msg = serialBody;
		  
		  // Clear the buffer
		  serialBuffer = "";

		  // Send Arduino Ready signal
		  Serial.println("1");
		  
		  return serialHeader;
		}else{          // Not the end of current message
		  serialBuffer += received; //Add the received char to the buffer
		}
  }
  
	return "";
}
