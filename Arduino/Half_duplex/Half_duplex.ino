//#include <XBee.h>

#define DST_ADDR_H 0x00000000
#define DST_ADDR_L 0xC0A8000B
#define BAUD_RATE 9600
/*
This example is for Series 2 XBee
 Sends a ZB TX request with the value of analogRead(photoRes) and checks the status response for success
*/
extern "C"{
	#include "RX_API.h"
//#include "./../../Xbee/Xbee_wifi_RX_API.h"
}
// create the XBee object
//XBee xbee = XBee();

//For the Rx parts
_apiFrameUnion * apiFramePtr;
_apiFrame64bit * apiFrame64bitPtr;


//Tranceive modes
//Mode 1: Broadcast and listen for tranceive mode commands only
//Mode 2: Send back to src address
//Mode 3: Write to Serial
int32_t TxEnable = 0;

//Note 0x50 = 'P' in ascii
uint8_t payload[] = {0x50, 0, 0 };
byte optionAck = 0x00;
byte len_H = 0x00;
byte len_L = 0x0D;
byte TX_REQUEST = 0x00;
byte ack_option = 0x00;
byte theFrameNumSent = 0x01;
// SH + SL Address of receiving XBee
uint8_t dump[] = {0x7E, 0x00, 0x0D, 0x00, 0x01, 0x00, 0x00,0x00,0x00, 0xC0, 0xA8, 0x00,0x64, 0x00, 0x15, 0x16, 0x07};
uint8_t dst_address[] = {0x00, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x01, 0x0B};
//Broadcast api_fdump
uint8_t api_fdump[] = {0x00, 0x01, 0x00, 0x00,0x00,0x00, 0xFF, 0xFF, 0xFF,0xFF, 0x01, 0x15, 0x16};

uint8_t frameBuffer[256];
int statusLed 	 = 6;
int errorLed 	 = 6;

HardwareSerial Uart = HardwareSerial();


void simpleXbeeRead();



void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);

  Serial.begin(BAUD_RATE);
  Uart.begin(BAUD_RATE);
  //xbee.setSerial(Serial);
  //xbee.setSerial(Uart);
  //xbee.begin(BAUD_RATE);
  
  Serial.print("hello");
  apiFrame64bitPtr->api_fd[0] = '1';
}

uint8_t calc_checksum(uint8_t * buffer, uint16_t length)
{
	uint8_t sum = 0;
	uint16_t i;
	for(i = 0; i < length; i++)
	{
		sum += buffer[i];
	}
		sum =  (0xFF - sum); 
		return sum; 

}
int32_t TxFormFrame( uint8_t * TxQueue, uint16_t Api_length, uint8_t *apiFrame)
{
	//Assume big endian
	uint8_t lenH, lenL;
	uint16_t i;
	uint8_t checksum;
	//Mask API_length and split
	lenH = (Api_length >> 8) & 0xFF;
	lenL = (Api_length >> 0) & 0xFF;
	
	//Form Packet
	TxQueue[0] = 0x7E;
	TxQueue[1] = lenH;
	TxQueue[2] = lenL;

	//dump in API frame
	for( i = 0; i < Api_length; i++)
	{
		TxQueue[3+i] = apiFrame[i];
	}
	//Calculate Checksum
	checksum = calc_checksum(apiFrame, Api_length);
	TxQueue[3+Api_length] = checksum;
	

	//Debug
	if(DEBUG_MODE)
	{
		for( i = 0; i < 4+Api_length; i++)
		{
			Serial.print(TxQueue[i], HEX);
			Serial.print(" ");
		}
		Serial.print('\n');
	}

	//apiFrame[1]= apiFrame[1] + 1;
	//if(apiFrame[1] == 0)
	//{
	//	apiFrame[1] = 1;
	//}
	//return the number of bytes in the frame
	// SF + lenH + lenL + ChkSum + API_Frame
	return 4 + Api_length;
}
void TxRequest(uint8_t *TxQueue, int32_t frame_length, byte frame_id)
{
	int i;

	
	for( i = 0; i < frame_length; i++)
	{
		Uart.print(TxQueue[i], BYTE);
		//delay(10);
		//Serial.print(TxQueue[i], HEX);
	}
	//Serial.print('\n');
	//Uart.print('\r\n', BYTE);
	//delay(10);
//	if(Uart.available()

	if(DEBUG_MODE)
	{
		while(Uart.available())
		{
		//	while(Uart.read() != 0x7E)
		//	{
		//	}
			Serial.print(Uart.read(),HEX);
			Serial.print(" ");
			//uint8_t tempbuff[8];
			//for( i = 0; i < 7; i++)
			//{
			//	tempbuff[i] = Uart.read();
			//}
			//for(i = 0; i < 7; i++)
			//{
			//	Serial.print(tempbuff[i], HEX);
			//	Serial.print(" ");
			//}
			//Serial.print('\n');
		}
		Serial.print('\n');
		Serial.println("Done transmitting this frame");
	}
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



void loop() {   
 
	//int32_t frame_length;
  
  	//frame_length = TxFormFrame(frameBuffer, 0x0D , api_fdump);

	//TxRequest(frameBuffer, frame_length, 1);
	simpleXbeeRead();
	processCommand();
  	delay(300);
}

void processCommand( )
{
	int32_t frame_length;
	switch(apiFrame64bitPtr->api_fd[0])
	{
		//Receive only mode
		case '1':
		{
			TxEnable = 0;
		}
		//Broadcast Mode
		case '2': 
		{
			TxEnable = 1;
		}
//		case '3': //idle
//		{
//			TxEnable = 2;
//		}
		default: ;
		
	
	}
	if(TxEnable == 0)
	{

			Serial.print('~ ');
			Serial.print(' A ');
			Serial.print(apiFrame64bitPtr->api_src_address);
			Serial.print(' R ');
			Serial.print(apiFrame64bitPtr->api_RSSI);	
			Serial.print('\n');
	}
	if(TxEnable == 1)
	{
  
  		frame_length = TxFormFrame(frameBuffer, 0x0D , api_fdump);

		TxRequest(frameBuffer, frame_length, 1);
		
	}
	else
	{
		Serial.println('Error TxEnable undefined');
	}


}

/**
 * \brief Simple Arduino Xbee Read function
 * Make sure enough data received on buffer and then when a start frame is received (0x7E) begin forming dummy packet.
 *
 *
 */
void simpleXbeeRead()
{
	uint16_t length;
	uint8_t  lengthSegHigh;
	uint8_t  lengthSegLow;
	//Begin Actual read 
	if(Uart.available() > 20) //Get at least something in the buffer
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
		//Serial.print('~ ');
		//Serial.print('A ');
		//Serial.print(apiFrame64bitPtr->api_src_address);
		//Serial.print('R ');
		//Serial.println(apiFrame64bitPtr->api_RSSI);

	}

}

