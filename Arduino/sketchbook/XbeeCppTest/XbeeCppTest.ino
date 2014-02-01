
#include <XbeeS6.h>
#include <uVector.h>
//#include <XBeeS6.h>

//#include "XbeeS6.h"
#define BAUD_RATE 115200

Tx64Packet tx_packet;

void setup()
{
  Serial.begin(BAUD_RATE);
  Serial.println("Starting the Receiver!");
  
  tx_packet.set_Address(0x00000000C0A80064);
  tx_packet.push_back(0x15);
  tx_packet.push_back(0x16);
}

void loop()
{
  uint16_t length = tx_packet.packet_buf();
  uint16_t i;
  for(i = 0; i < length; i++)
  {
    Serial.print(tx_buffer[i], HEX); 
    Serial.print(" ");
  }
  Serial.print('\n');
}
