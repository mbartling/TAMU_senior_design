/**
 * \author Michael Bartling bart4128@tamu.edu
 * \author Rabih Abifaraj   rabifaraj@tamu.edu
 * \date 11/8/2013
 * \version 1.0
 * 
 * \copyright The MIT License (MIT)
 */


#define BAUD_RATE 9600

/*Declare serial ports */
//Xbee xbee = Xbee();
uint16_t length;
uint8_t  lengthSegHigh;
uint8_t  lengthSegLow;

// We only get one LED to play with
int statusLed 	   = 6;
int errorLed 	   = 6;
uint8_t api_id;
uint8_t api_fd[128];
HardwareSerial Uart = HardwareSerial();

void simpleXbeeRead();

void setup()
{
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);

  Serial.begin(BAUD_RATE);
  Uart.begin(BAUD_RATE);
  //from api_try.ino
  //xbee.setSerial(Uart);
  //xbee.begin(BAUD_RATE)
  Serial.println("Starting the Receiver!");
}

void flashLed(int pin, int times, int wait) {

	for (int i = 0; i < times; i++) {
		digitalWrite(pin, HIGH);
		delay(wait);
		digitalWrite(pin, LOW);

		if (i + 1 < times) {
			delay(wait);
		}
	}
}


void loop()
{
	//Do other functions here
	//
	
	simpleXbeeRead();
	flashLed(statusLed, 1, 100);

}

/**
 * \brief Simple Arduino Xbee Read function
 * Make sure enough data received on buffer and then when a start frame is received (0x7E) begin forming dummy packet.
 *
 *
 */
void simpleXbeeRead()
{
	//Begin Actual read 
	//if(Uart.available() > 20) //Get at least something in the buffer
        if(Uart.available() >= 21)
	{
	  /* 7E is the start frame of our packet */
		if(Uart.read()==0x7E)
		{
			lengthSegHigh = Uart.read();
			lengthSegLow = Uart.read();
			
			/* The length are the first two bytes we read after the start frame */
			length = ((lengthSegHigh<<8) & 0xff00) | ((lengthSegLow) & 0x00ff);
			
			/* The api frame id is the next thing we read after the length */
			api_id = Uart.read();
			
			/*Everything else for the remainder of the length is data */
			for(int i = 0; i < length; i++) /**\todo Make sure this doesnt need to start from 1 instead of 0*/
			{
			        api_fd[i] = Uart.read();
                                //Serial.print(apiFramePtr->api_fd[i]);
                                //Serial.print(" ");
			}
//                        Serial.print('\n');
                        Serial.print("A ");
                        for(int i = 4; i < 7; i++) /**\todo Make sure this doesnt need to start from 1 instead of 0*/
			{
			
                                Serial.print(api_fd[i]);
                                Serial.print(".");
			}
                        Serial.print(api_fd[7]);
                        Serial.print(" ");  //Last part of IP doesnt need .
                        
                        Serial.print("R ");
                        Serial.print(api_fd[8]); //RSSI
                        
                        Serial.print('\n');
		}



	}

}

