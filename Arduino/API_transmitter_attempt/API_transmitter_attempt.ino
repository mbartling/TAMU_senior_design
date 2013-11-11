#include <XBee.h>
//192.168.1.11
//0x00 00 00 00 C0 A8 01 0B
//#define DST_ADDR_H 0x0013a200
//#define DST_ADDR_L 0x403e0f30

#define DST_ADDR_H 0x00000000
//#define DST_ADDR_L 0xC0A8010B
#define DST_ADDR_L 0xC0A8000B
#define BAUD_RATE 9600
/*
This example is for Series 2 XBee
 Sends a ZB TX request with the value of analogRead(photoRes) and checks the status response for success
*/

// create the XBee object
XBee xbee = XBee();

//Note 0x50 = 'P' in ascii
uint8_t payload[] = {0x50, 0, 0 };
//byte dstAddr[8] = {0x00, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x00, 0x16};
byte dstAddr[8] = {0x00, 0x00, 0x00, 0x00, 0xC0, 0xA8, 0x00, 0x0B};
byte optionAck = 0x00;
byte len_H = 0x00;
//byte len_L = 0x14;
byte len_L = 0x14;
byte TX_REQUEST = 0x00;
byte theFrameNumSent = 0x01;
// SH + SL Address of receiving XBee
//XBeeAddress64 addr64 = XBeeAddress64(DST_ADDR_H, DST_ADDR_L);
//ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
//ZBTxStatusResponse txStatus = ZBTxStatusResponse();

byte packettest[20];
byte idCmd       = 0x49; //I
byte NodeID      = 0x34;
byte P_forPayload= 0x50; //P
int photoRes 	 = 0;
int photoRes_pin = 0;
int statusLed 	 = 6;
int errorLed 	 = 6;

HardwareSerial Uart = HardwareSerial();

void setup() {
  pinMode(statusLed, OUTPUT);
  pinMode(errorLed, OUTPUT);

  Serial.begin(BAUD_RATE);
  Uart.begin(BAUD_RATE);
  //xbee.setSerial(Serial);
  xbee.setSerial(Uart);
}

void TxRequest(int data_payload, byte *address, byte frame_id)
{
   long sum;
   //byte sum;
   int i;
   byte a3,a2,a1,a0;
//   Uart.print( 0x7F  ,BYTE);
//   Uart.print( len_H ,BYTE);
//   Uart.print( len_L ,BYTE);
//   Uart.print( TX_REQUEST  ,BYTE);
//   Uart.print( frame_id, BYTE);
//   for(i = 0; i < 8; i++)
//   {
//      Uart.print(address[i],BYTE);
//      sum += address[i]; 
//   }
//   Uart.print(optionAck,BYTE);
//   a3 = (data_payload>>24) & 0xFF;
//   a2 = (data_payload>>16) & 0xFF;
//   a1 = (data_payload>> 8) & 0xFF;
//   a0 = (data_payload>> 0) & 0xFF;
//   Uart.print(a3, BYTE);
//   Uart.print(a2, BYTE);
//   Uart.print(a1, BYTE);
//   Uart.print(a0, BYTE);
//   sum += TX_REQUEST + frame_id + optionAck + a3 + a2 + a1 + a0;
//   Uart.print((0xFF - (sum & 0xFF)), BYTE);
//   Serial.print(frame_id, HEX);   
//   Serial.print(a3, HEX);
//   Serial.print(a2, HEX);
//   Serial.print(a1, HEX);
//   Serial.print(a0, HEX);
//   Serial.print((0xFF - (sum & 0xFF)), HEX);   
//   delay(10); //delay 10 ms
   //theFrameNumSent += 1;
   
//   Uart.write( 0x7E);
//   Uart.write( len_H );
//   Uart.write( len_L );
//   Uart.write( TX_REQUEST );
//   Uart.write( frame_id);
//   for(i = 0; i < 8; i++)
//   {
//      Uart.write(address[i]);
//      sum += address[i]; 
//   }
//   Uart.write(optionAck);
//   a3 = (data_payload>>24) & 0xFF;
//   a2 = (data_payload>>16) & 0xFF;
//   a1 = (data_payload>> 8) & 0xFF;
//   a0 = (data_payload>> 0) & 0xFF;
//   a3 = 0x31;
//   a2 = 0x31;
//   a1 = 0x31;
//   a0 = 0x0D;
//   Uart.write(a3);
//   Uart.write(a2);
//   Uart.write(a1);
//   Uart.write(a0);
//   sum += TX_REQUEST + frame_id + optionAck + a3 + a2 + a1 + a0;
//   Uart.write((0xFF - (sum & 0xFF)));
//   Serial.print(frame_id, HEX);   
//   Serial.print(a3, HEX);
//   Serial.print(a2, HEX);
//   Serial.print(a1, HEX);
//   Serial.print(a0, HEX);
//   Serial.print((0xFF - (sum & 0xFF)), HEX);   
//   delay(10); //delay 10 ms
   a3 = (data_payload>>24) & 0xFF;
   a2 = (data_payload>>16) & 0xFF;
   a1 = (data_payload>> 8) & 0xFF;
   a0 = (data_payload>> 0) & 0xFF;
//   a3 = 0x31;
//   a2 = 0x31;
//   a1 = 0x31;
//   a0 = 0x00;
   Uart.print( 0x7E, BYTE);
   Uart.print( len_H, BYTE );
   Uart.print( len_L, BYTE );
   Uart.print( TX_REQUEST, BYTE );
   Uart.print( frame_id, BYTE);
   for(i = 0; i < 8; i++)
   {
      Uart.print(address[i], BYTE);
      Serial.print(address[i], BYTE);
      sum += address[i]; 
   }
   Uart.print(optionAck, BYTE);
   Uart.print(idCmd, BYTE);
   Uart.print(NodeID,BYTE);
  
   Uart.print(P_forPayload,BYTE);
   Uart.print(a3, BYTE);
   Uart.print(a2, BYTE);
   Uart.print(a1, BYTE);
   Uart.print(a0, BYTE);
   Uart.print(0x0A,BYTE);
   Uart.print(0x0D,BYTE);
   sum += TX_REQUEST + frame_id + optionAck + a3 + a2 + a1 + a0  + P_forPayload + idCmd + NodeID + 0x0A + 0x0D;
   Uart.print((0xFF - (sum & 0xFF) +1), BYTE);
   if(Serial.available())
   {
     Serial.print(frame_id, HEX);   
     Serial.print(a3, HEX);
     Serial.print(a2, HEX);
     Serial.print(a1, HEX);
     Serial.print(a0, HEX);
     Serial.print((0xFF - (sum & 0xFF)), HEX);
     Serial.print('\n');
   }

   packettest[0] = 0x7E;
   packettest[1] = len_H;
   packettest[2] = len_L;
   packettest[3] = TX_REQUEST;
   packettest[4] = frame_id;
   packettest[5] = address[0];
   packettest[6] = address[1];
   packettest[7] = address[2];
   packettest[8] = address[3];
   packettest[9] = address[4];
   packettest[10] = address[5];
   packettest[11] = address[6];
   packettest[12] = address[7];
   packettest[13] = optionAck;
   packettest[14] = a3;
   packettest[15] = a2;
   packettest[16] = a1;
   packettest[17] = a0;
   packettest[18] = (0xFF - (sum & 0xFF));


      
   delay(10); //delay 10 ms
   theFrameNumSent += 1;
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
  // break down 10-bit reading into two bytes and place in payload
  photoRes = analogRead(photoRes_pin);
  //Serial.println(photoRes);
  payload[1] = photoRes >> 8 & 0xff;
  payload[2] = photoRes & 0xff;
 // Get rid of this crap
 // xbee.send(zbTx);
 
  TxRequest(photoRes, dstAddr, theFrameNumSent);
  // flash TX indicator
  flashLed(statusLed, 1, 100);

//  // after sending a tx request, we expect a status response
//  // wait up to half second for the status response
//  if (xbee.readPacket(500)) {
//    // got a response!
//
//    // should be a znet tx status               
//    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
//      xbee.getResponse().getZBTxStatusResponse(txStatus);
//
//      // get the delivery status, the fifth byte
//      if (txStatus.getDeliveryStatus() == SUCCESS) {
//        // success.  time to celebrate
//        flashLed(statusLed, 5, 50);
//      } else {
//        // the remote XBee did not receive our packet. is it powered on?
//        flashLed(errorLed, 3, 500);
//      }
//    }
//  } else if (xbee.getResponse().isError()) {
//    //nss.print("Error reading packet.  Error code: ");  
//    //nss.println(xbee.getResponse().getErrorCode());
//  } else {
//    // local XBee did not provide a timely TX Status Response -- should not happen
//    flashLed(errorLed, 2, 50);
//  }

  delay(100);
}





