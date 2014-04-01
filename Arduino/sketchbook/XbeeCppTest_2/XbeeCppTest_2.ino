#include <stdio.h>
#include <uVector.h>
#include <XbeeS6.h>
#include <string.h>
#include <Time.h>

//#include <XBeeS6.h>

//#include "XbeeS6.h"
//#define BAUD_RATE 115200
#define BAUD_RATE 57600

uint8_t my_buffer[20];

uint16_t dummy = 0x1419;

uint8_t dummyRx[] = {0x7E, 0x00, 0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x00, 0x67, 0x2E, 0x00, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00};

Tx64Packet tx_packet;
RxPacket rx_packet;

uVector myvector;
static uint8_t * tx_buffer1;

HardwareSerial Uart = HardwareSerial();
//unsigned long baud = 115200;
unsigned long baud = 57600;
const int reset_pin = 4;
const int led_pin = 11;  // 11=Teensy 2.0, 6=Teensy 1.0, 16=Benito
const int led_on = HIGH;
const int led_off = LOW;

volatile time_t start_time;
time_t current_time;
void setup()
{
  //start_time = now();
  
  pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, led_off);
	digitalWrite(reset_pin, HIGH);
	pinMode(reset_pin, OUTPUT);
  Serial.begin(BAUD_RATE);
  Serial.println("Starting the Receiver!");
  second(start_time);
  Serial.print("Starting time: ");
  Serial.println(start_time);
  
  tx_packet.set_Address(0x00000000C0A80165);  
  //tx_packet.set_Address(0x00000000FFFFFFFF);
  tx_packet.push_back(0x15);
  tx_packet.push_back(0x16);
  myvector.push_back(0x15);
  myvector.push_back(0x16);
  Serial.println(myvector[0], HEX);
  Serial.println(myvector[1], HEX);
  printf("printf test\n");
  
  memcpy(my_buffer, &dummy, sizeof(dummy));
  Serial.println(my_buffer[0], HEX);
  Serial.println(my_buffer[1], HEX);
  
  tx_buffer1 = get_buffer();
  
  Uart.begin(BAUD_RATE);
  
  Serial.println("=======================");
  Serial.println("?: for Receive mode HEX");
  Serial.println("#: for Receive mode BYTE (for Config)");
  Serial.println("$: for TX Mode");
  Serial.println("=======================");
  Uart.flush();
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

  //while(j <  20)
  //{
  if (Serial.available()) {
    //Uart.flush();
	c = Serial.read();
    if(c == '?') {enable = 3; return;}

    if(c == '$' ) {enable  =1; return;}
    if(c == '#' ) {enable = 2; return; }
    if(c == '^') {Uart.flush(); return; }
    Uart.write(c);
    digitalWrite(led_pin, led_on);
    led_on_time = millis();
    return;
  }
  if(enable == 1)
  {
    delay(500);
    uint16_t length = tx_packet.prepare2send();
    uint16_t i;
  
  for(i = 0; i < length; i++)
  {
    //Uart.print(tx_buffer1[i], BYTE);
//    delayMicroseconds(10);
    Uart.write(tx_buffer1[i]);

  }
  for(i = 0; i < length; i++)
  {
    

    Serial.print(tx_buffer1[i], HEX); 
    Serial.print(" ");
  }
  Serial.println(' ');
  j++;
  tx_packet.clear_payload();
  tx_packet.push_back( (uint8_t) j);
  Uart.flush();
  Serial.println("response");
  while(Uart.available())
	{

		Serial.print(Uart.read(),HEX);
		Serial.print(" ");
	}
  Serial.println("EOR");
//  tx_packet.push_back( (uint8_t) j+1);
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
