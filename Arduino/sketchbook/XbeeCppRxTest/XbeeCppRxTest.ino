#include <stdio.h>
#include <uVector.h>
#include <XbeeS6.h>
#include <string.h>

//#include <XBeeS6.h>

//#include "XbeeS6.h"
#define BAUD_RATE 9600

uint8_t my_buffer[20];

uint16_t dummy = 0x1419;

uint8_t dummyRx[] = {0x7E, 0x00, 0x10, 0x80, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x00, 0x67, 0x2E, 0x00, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00};

Tx64Packet tx_packet;
RxPacket rx_packet;

uVector myvector;
uint8_t * tx_buffer1;

HardwareSerial Uart = HardwareSerial();

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.println("Starting the Receiver!");
  
  tx_packet.set_Address(0x00000000C0A8011E);  
//  tx_packet.set_Address(0x00000000FFFFFFFF);
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
  Uart.flush();
}
int j = 0;
int k = 0;
void loop()
{
//  delay(2000);
//  //while(j <  20)
//  //{
//  
//  uint16_t length = tx_packet.prepare2send();
//  uint16_t i;
//  for(i = 0; i < length; i++)
//  {
//    Uart.print(tx_buffer1[i], BYTE);
//
//  }
//  for(i = 0; i < length; i++)
//  {
//    
//
//    Serial.print(tx_buffer1[i], HEX); 
//    Serial.print(" ");
//  }
//  Serial.println(' ');
//  j++;
//  tx_packet.clear_payload();
//  tx_packet.push_back( (uint8_t) j);
// // }
  if(Uart.available() >= 14)
  {
    myvector.clear();
  if(Uart.read() == 0x7E)
  {
    delay(1);
   // while(1) { if(Uart.available() > 2) break; }
    uint8_t lenH = Uart.read();
        delay(1);
    uint8_t lenL = Uart.read();
  uint16_t length = ((uint16_t)lenL | (uint16_t) lenH  << 8);
  
  rx_packet.set_length(lenH, lenL);
  
  //offset in the packet stuff
  //for(int jk = 0; jk < rx_packet.getlength() + 1; jk++)
  int jk;
  //while(jk < rx_packet.getlength() + 1)
  //while(1) { if(Uart.available() > length) break; }
  while(jk < length + 1)
  {
     uint8_t temp = Uart.read();
     rx_packet.push_back( temp );
     delay(1);
     jk++;
  }
  Serial.print("length = ");
  Serial.println(length, HEX);
  //Serial.println("Setting Rx packet length");
  //Serial.println(rx_packet.getlength(), HEX);
 // Serial.print("API Frame Id is: ");
 // Serial.println(rx_packet.getApiFrameId(), HEX);
  
  for(int i = 0; i< length; i++)
  {
     //Serial.print(rx_packet.getApiFrame(i) , HEX);
     myvector.push_back(rx_packet.getApiFrame(i));
//     Serial.print(" "); 
  }
  Serial.println();
  int status = rx_packet.process();
  Serial.print("RX parsing returned: ");
  Serial.println(status);
  
  for(int jk = 0; jk <= rx_packet.getlength(); jk++)
  {
   Serial.print(rx_packet[jk], BYTE); 
  }
    for(int jk = 0; jk <= rx_packet.getlength(); jk++)
  {
   Serial.print(rx_packet[jk], HEX); 
  }
  Serial.println();
  rx_packet.clear();
  //Serial.print('\n');
  //Serial.println("Done!");

    Uart.flush();
  }

  }
  if(myvector.size() > 0)
  {
    for(int i = 0; i< length; i++)
  {
     //Serial.print(rx_packet.getApiFrame(i) , HEX);
     myvector.push_back(rx_packet.getApiFrame(i));
//     Serial.print(" "); 
  }
  Serial.println();
 myvector.clear(); 
  }
}
