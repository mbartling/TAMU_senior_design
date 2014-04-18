//#include <stdio.h>
#include <uVector.h>
#include <XbeeS6.h>
//#include <string.h>
#include <Time.h>



//#include "XbeeS6.h"
#define BAUD_RATE 57600


Tx64Packet tx_packet;

uVector myvector;
static uint8_t * tx_buffer1;
//uint8_t * tx_buffer1;

HardwareSerial Uart = HardwareSerial();
unsigned long baud = 57600;
const int reset_pin = 4;
const int led_pin = 11;  // 11=Teensy 2.0, 6=Teensy 1.0, 16=Benito
const int led_on = HIGH;
const int led_off = LOW;

volatile time_t start_time;
time_t current_time;
long previousMillis = 0;
long interval = 50; //in ms

void setup()
{
  //start_time = now();

  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, led_off);
  digitalWrite(reset_pin, HIGH);
  pinMode(reset_pin, OUTPUT);
  Serial.begin(BAUD_RATE);
  Serial.println(F("Starting the Receiver!"));
  second(start_time);
  //Serial.print("Starting time: ");
  //Serial.println(start_time);

  tx_packet.set_Address(0x00000000C0A80164);  
//  tx_packet.set_Address(0x00000000FFFFFFFF);
  
  tx_buffer1 = get_buffer();

  Uart.begin(BAUD_RATE);

  Serial.println(F("======================="));
  Serial.println(F("?: for Receive mode HEX"));
  Serial.println(F("#: for Receive mode BYTE (for Config)"));
  Serial.println(F("$: for TX Mode"));
  Serial.println(F("======================="));
  Uart.flush();				


}
//https://www.pjrc.com/teensy/benchmark_usb_serial_receive.html
int j = 0;
int k = 0;
volatile int enable;
long led_on_time=0;
void loop()
{
  //static int enable;
  unsigned char c, dtr;
  //int serCount = 0; //See https://www.pjrc.com/teensy/td_serial.html
  static unsigned char prev_dtr = 0;
  unsigned long currentMillis = millis();
  if (Serial.available()) {
    c = Serial.read();
    if(c == '?') {
      enable = 3; 
      return;
    }

    if(c == '$' ) {
      enable  =1; 
      return;
    }
    if(c == '#' ) {
      enable = 2; 
      return; 
    }
    if(c == '^') {
      Uart.flush(); 
      return; 
    }
    Uart.write(c);
    digitalWrite(led_pin, led_on);
    led_on_time = millis();
    return;
  }
  /* Determine Server Configuration*/
  if(currentMillis-previousMillis > interval)
  {
    if(enable == 1)
    {

      uint16_t length = tx_packet.prepare2send();
      uint16_t i;

      for(i = 0; i < length; i++)
      {
        Uart.write(tx_buffer1[i]);
      }
//      for(i = 0; i < length; i++)
//      {
//        Serial.print(tx_buffer1[i], HEX); 
//        Serial.print(" ");
//      }
//      Serial.println(' ');
      j++;
      if(j == 0x7E) j++;
      tx_packet.clear_payload();
      tx_packet.push_back( (uint8_t) j);
      Uart.flush();
      //Serial.println("response");
      while(Uart.available())
      {
        Serial.print(Uart.read(),HEX);
        Serial.print(" ");
      }
    }
    //else
    if(enable == 2) //For command mode use write for repsponse use print
    {
      if (Uart.available()) 
      {
        c = Uart.read();
        Serial.write(c);
        digitalWrite(led_pin, led_on);
        led_on_time = millis();
        return;
      } 
    }
    //else 
    if(enable == 3) //For command mode use write for response use print
    {
      if (Uart.available()) 
      {
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
    previousMillis = currentMillis;
  }
  
  /* Control Bootloader*/
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
    } 
    else {
      Uart.begin(baud);
    }
  }



}

