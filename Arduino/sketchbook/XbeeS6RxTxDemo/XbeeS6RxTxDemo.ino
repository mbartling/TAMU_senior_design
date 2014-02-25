#include <stdio.h>
#include <uVector.h>
#include <XbeeS6.h>
#include <string.h>
#include <Time.h>

//#include <XBeeS6.h>

//#include "XbeeS6.h"
#define BAUD_RATE 115200

uint8_t my_buffer[20];

uint16_t dummy = 0x1419;

uint8_t dummyRx[] = {0x7E, 0x00, 0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x00, 0x67, 0x2E, 0x00, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00};

Tx64Packet tx_packet;
Tx64Packet mav_packet;

//uVector myvector;
static uint8_t * tx_buffer1;

HardwareSerial Uart = HardwareSerial();
HardwareSerial Mavlink = HardwareSerial2(); //Set appropriately
unsigned long baud = 115200;
const int reset_pin = 4;
const int led_pin = 11;  // 11=Teensy 2.0, 6=Teensy 1.0, 16=Benito
const int led_on = HIGH;
const int led_off = LOW;

void setup()
{
  //start_time = now();
  
  pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, led_off);
	digitalWrite(reset_pin, HIGH);
	pinMode(reset_pin, OUTPUT);
  Mavlink.begin(BAUD_RATE);
  
  tx_packet.set_Address(0x00000000C0A80165);  
  mav_packet.set_Address(SERVER_ADDRESS);
  
  
  //tx_packet.set_Address(0x00000000FFFFFFFF);
  //tx_packet.push_back(0x15);
  //tx_packet.push_back(0x16);
  //myvector.push_back(0x15);
  //myvector.push_back(0x16);
  //Serial.println(myvector[0], HEX);
  //Serial.println(myvector[1], HEX);
  
  //memcpy(my_buffer, &dummy, sizeof(dummy));
  //Serial.println(my_buffer[0], HEX);
  //Serial.println(my_buffer[1], HEX);
  
  tx_buffer1 = get_buffer();
  
  Uart.begin(BAUD_RATE);
 
 /* 
  Serial.println("=======================");
  Serial.println("?: for Receive mode HEX");
  Serial.println("#: for Receive mode BYTE (for Config)");
  Serial.println("$: for TX Mode");
  Serial.println("=======================");
  */
 
  Uart.flush();
  Mavlink.flush();
}
int j = 0;
int k = 0;
volatile int enable;
long led_on_time=0;


void loop()
{
  //static int enable;
  unsigned char c, dtr;
  static unsigned char prev_dtr = 0;

  if (Mavlink.available()) {
  	
  	delay(10); make sure all the data is received
  	//Read the data and directly push into the vector
    while(Mavlink.available())
    {
    	mav_packet.push_back(Mavlink.read());
    }
   
    uint16_t length = mav_packet.prepare2send();
    
    //Uart.write(tx_buffer, length);
	for(i = 0; i < length; i++)
  {
    Uart.write(tx_buffer1[i]);
  }
  
	digitalWrite(led_pin, led_on);
	led_on_time = millis();
	return;
 }
if(Uart.available())
{
 //Need to seek until read 0x7E
  delay(10);
  tx_packet.clear_payload();
  while((c = Uart.read() ) != '\n')
  {
  	tx_packet.push_back(c);
  }
  uint16_t length = tx_packet.prepare2send();
  uint16_t i;
  
  //Uart.write(tx_buffer,length);
  for(i = 0; i < length; i++)
  {
    Uart.write(tx_buffer1[i]);
  }

  tx_packet.clear_payload();
  tx_packet.push_back( (uint8_t) j);
  Uart.flush();
  Serial.println("response");
  //while(Uart.available())
  //{

	//	Serial.print(Uart.read(),HEX);
	//	Serial.print(" ");
	//}

}
//else
if(enable == 2) //For command mode use write for repsponse use print
{
 if (Uart.available()) {
		c = Uart.read();
		Serial.write(c);
                digitalWrite(led_pin, led_on);
		led_on_time = millis();
		return;
	} 
}
//else 
if(enable == 3) //For command mode use write for repsponse use print
{
 if (Uart.available()) {
		c = Uart.read();
//		Serial.write(c);
                 current_time = now();
                //Serial.print("current time: ");
                //Serial.println(current_time);
                if(c == 0x7E) Serial.println();
                Serial.print(c, HEX);
                Serial.print(" ");
		digitalWrite(led_pin, led_on);
		led_on_time = millis();
		return;
	} 
}
    dtr = Serial.dtr();
	if (dtr && !prev_dtr) {
		digitalWrite(reset_pin, LOW);
		delayMicroseconds(250);
		digitalWrite(reset_pin, HIGH);
	}
	prev_dtr = dtr;
	if (millis() - led_on_time > 3) {
		digitalWrite(led_pin, led_off);
	}
	if (Serial.baud() != baud) {
		baud = Serial.baud();
		if (baud == 57600) {
			// This ugly hack is necessary for talking
			// to the arduino bootloader, which actually
			// communicates at 58824 baud (+2.1% error).
			// Teensyduino will configure the UART for
			// the closest baud rate, which is 57143
			// baud (-0.8% error).  Serial communication
			// can tolerate about 2.5% error, so the
			// combined error is too large.  Simply
			// setting the baud rate to the same as
			// arduino's actual baud rate works.
			Uart.begin(58824);
		} else {
			Uart.begin(baud);
		}
}
 // }
  
//  if(k == 0)
//  {
//  Serial.println("Testing Rx Capabilities!");
//  uint16_t length = (dummyRx[2] | dummyRx[1] << 8);
//  Serial.println(length, HEX);
//  rx_packet.set_length(dummyRx[1], dummyRx[2]);
//  Serial.println(rx_packet.getlength(), HEX);
//  //offset in the packet stuff
//  for(int jk = 3; jk < rx_packet.getlength() + 3 + 1; jk++)
//  {
//     rx_packet.push_back( dummyRx[jk]);
//  }
//  
//  int status = rx_packet.process();
//  Serial.print("RX parsing returned: ");
//  Serial.println(status, HEX);
//  
//  for(int jk = 0; jk <= rx_packet.getlength(); jk++)
//  {
//   Serial.print(rx_packet[jk], BYTE); 
//  }
//    for(int jk = 0; jk <= rx_packet.getlength(); jk++)
//  {
//   Serial.print(rx_packet[jk], HEX); 
//  }
//  
//  rx_packet.clear();
//  //Serial.print('\n');
//  //Serial.println("Done!");
//
//   k = 1;
//  }
  
}
