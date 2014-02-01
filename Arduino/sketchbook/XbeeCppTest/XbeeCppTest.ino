#include <stdio.h>
#include <uVector.h>
#include <XbeeS6.h>
#include <string.h>

//#include <XBeeS6.h>

//#include "XbeeS6.h"
#define BAUD_RATE 9600

uint8_t my_buffer[20];

uint16_t dummy = 0x1419;

Tx64Packet tx_packet;
uVector myvector;

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
  
  memcpy(my_buffer, dummy, sizeof(dummy));
  Serial.println(my_buffer[0], HEX);
  Serial.println(my_buffer[1], HEX);
  
}
int j = 0;
void loop()
{
  while(j <  20)
  {
  uint16_t length = tx_packet.packet_buf();
  uint16_t i;
  for(i = 0; i < length; i++)
  {
    Serial.print(tx_buffer[i], HEX); 
    Serial.print(" ");
  }
  Serial.print('\n');
  j++;
  }
}
