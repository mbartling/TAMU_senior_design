#include "./../Xbee/RX_API.h"
#include <Xbee.h>

#define BAUD_RATE 9600

_apiFrameUnion * apiFramePtr;
_apiFrame64bit * apiFrame64bitPtr;

/*Declare serial ports */
Xbee xbee = Xbee();
uint16_t length;
uint8_t  lengthSegHigh;
uint8_t  lengthSegLow;

// We only get one LED to play with
int statusLed 	 = 6;
int errorLed 	   = 6;

HardwareSerial Uart = HardwareSerial();

void setup()
{
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);

  Serial.begin(BAUD_RATE);
  Uart.begin(BAUD_RATE);
  //from api_try.ino
  xbee.setSerial(Uart);
  //xbee.begin(BAUD_RATE)
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
	//prolly needs to analogRead()
	if(Uart.available() >= 21) //mae sure we have enough or a mouthful of data
	{
	  /* 7E is the start frame of our packet */
		if(Uart.read()==0x7E)
		{
			lengthSegHigh = Uart.read();
			lengthSegLow = Uart.read();
			
			/* The length are the first two bytes we read after the start frame */
			length = ((lengthSegHigh<<8) & 0xff00) | ((lengthSegLow) & 0x00ff);
			
			/* The api frame id is the next thing we read after the length */
			apiFramePtr->api_id = Uart.read();
			
			/*Everything else for the remainder of the length is data */
			for(int i = 0; i < length; i++) /**\todo Make sure this doesnt need to start from 1 instead of 0*/
			{
				apiFramePtr->api_fd[i] = Uart.read();
			}
		}

    /* Process the API frame for relevant information */
		Xbee_HandleRxAPIFrame(apiFramePtr, apiFrame64bitPtr);
		
		/**\warning some Xbee S6 modules do NOT support RSSI natively; Usually it is possible to fix this with a firmware update */
		Serial.println(apiFrame64bitPtr->api_RSSI);

	}
	
	flashLed(statusLed, 1, 100);

}

