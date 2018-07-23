#ifndef SerialHandler_h
#define SerialHandler_h

// Include libraries.
#include <Arduino.h>

class SerialHandler{

	public:


		SerialHandler();
		~SerialHandler(){};
		char readSerial(String &msg);	// Read serial if available. Rturn true if available, false if not


	private:

		String serialBuffer = "";  // Buffer for serial comunication
		char received;             // Char received
		char serialHeader;       // Header of the message (2 chars)
		String serialBody;         // The body of the message recieved


};

#endif
