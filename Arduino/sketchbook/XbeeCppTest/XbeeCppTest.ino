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
void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.println("Starting the Receiver!");
  
  tx_packet.set_Address(0x00000000C0A80064);
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
}
int j = 0;
int k = 0;
void loop()
{
  
  while(j <  20)
  {
  
  uint16_t length = tx_packet.prepare2send();
  uint16_t i;
  for(i = 0; i < length; i++)
  {
    Serial.print(tx_buffer1[i], HEX); 
    Serial.print(" ");
  }
  Serial.print('\n');
  j++;
  tx_packet.clear_payload();
  tx_packet.push_back( (uint8_t) j);
  }
  
  if(k == 0)
  {
  Serial.println("Testing Rx Capabilities!");
  
  rx_packet.set_length(0x0010);
  for(int jk = 3; jk < 20; jk++)
  {
     rx_packet.push_back( dummyRx[jk]);
  }
  
  int status = rx_packet.process();
  Serial.print("RX parsing returned: ");
  Serial.println(status, HEX);
  
  for(int jk = 0; jk <= rx_packet.getlength(); jk++)
  {
   Serial.print(rx_packet[jk], BYTE); 
  }
    for(int jk = 0; jk <= rx_packet.getlength(); jk++)
  {
   Serial.print(rx_packet[jk], HEX); 
  }
  
  rx_packet.clear();
  //Serial.print('\n');
  //Serial.println("Done!");

   k = 1;
  }
  
}
