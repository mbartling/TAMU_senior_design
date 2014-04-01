#include <uVector.h>
#include <XbeeS6.h>
#include "/home/walter/mavlink/arduino_include/common/mavlink.h"

HardwareSerial Uart = HardwareSerial();

int lat, lon;
int32_t last_lat, last_lon;
//Tx64Packet tx_packet;
//static uint8_t *tx_buf;
uint8_t received_buf[16];
int received_buf_index;
const int TRANSMIT_LENGTH = 25;
uint8_t transmit_buf[TRANSMIT_LENGTH];


void setup() {
    Serial.begin(57600);
    Uart.begin(58824);  //ardupilot
    Serial2.begin(58824);  //xbee
    lat = 0;
    lon = 0;
    last_lat = 0;
    last_lon = 0;
    received_buf_index = 0;
//    transmit_buf_index = 0;
//    tx_packet.set_Address(0x00000000c0a80166);
//    tx_buf = get_buffer();
    //transmit_request_packet_header(0xc0a80166);
    transmit_request_packet_header(0xffffffff);
}

void loop(){
   read_from_mavlink();
   if (lat != 0 && lon != 0){
//     Serial.print(lat);
//     Serial.print(" ");
//     Serial.println(lon);
     last_lat = lat;
     last_lon = lon;
   }
   if(read_from_xbee() == 1){
     for(int i = 0; i < 16; i++){
       Serial.print(received_buf[i], HEX);
       Serial.print(" ");
     }
     Serial.println();
     
     Serial.print("received xbee packet ");
     Serial.print(last_lat);
     Serial.print(" ");
     Serial.println(last_lon);

     //read_from_mavlink();
//     tx_packet.push_back(received_buf[11]);
//     tx_packet.push_back(received_buf[12]);
//     tx_packet.push_back(lat >> 24) ;
//     tx_packet.push_back(lat >> 16);
//     tx_packet.push_back(lat >> 8);
//     tx_packet.push_back(lat);
//     tx_packet.push_back(lon >> 24);
//     tx_packet.push_back(lon >> 16);
//     tx_packet.push_back(lon >> 8);
//     tx_packet.push_back(lon);
//     
//     Serial.println("here");
//     uint16_t length = tx_packet.prepare2send();
//
//     Serial.print("Packet: ");
//     for(int i = 0; i < length; i++){
//       Serial.print(tx_buf[i], HEX);
//       Serial.print(" ");
//      }
//      Serial.println();
//      tx_packet.clear_payload();
      //Serial.print(lat);
      //Serial.print(" ");
      //Serial.println(lon);
      transmit_buf[14] = received_buf[11];
      transmit_buf[15] = received_buf[12];
      transmit_buf[16] = last_lat >> 24;
      transmit_buf[17] = last_lat >> 16;
      transmit_buf[18] = last_lat >> 8;
      transmit_buf[19] = last_lat;
      transmit_buf[20] = last_lon >> 24;
      transmit_buf[21] = last_lon >> 16;
      transmit_buf[22] = last_lon >> 8;
      transmit_buf[23] = last_lon;
      calculate_checksum();
      for(int i = 0; i < TRANSMIT_LENGTH; i++){
        Serial.print(transmit_buf[i], HEX);
        Serial.print(" ");
      }
       Serial.println();
       //Serial2.write(transmit_buf, TRANSMIT_LENGTH);
       Serial2.write(0x7E);
       for(int i = 1; i < TRANSMIT_LENGTH; i++)
         switch(transmit_buf[i])
         {
           case 0x7E:
           {
            Serial2.write(0x7D);
            Serial2.write(transmit_buf[i]^0x20); 
           }
           case 0x7D:
           {
            Serial2.write(0x7D);
            Serial2.write(transmit_buf[i]^0x20); 
           }
           case 0x11:
           {
            Serial2.write(0x7D);
            Serial2.write(transmit_buf[i]^0x20); 
           }
           case 0x13:
           {
            Serial2.write(0x7D);
            Serial2.write(transmit_buf[i]^0x20); 
           }
           default: Serial2.write(transmit_buf[i]);
         }
   }
}

int read_from_xbee(){
  char c;
  if(Serial2.available()){
    c = Serial2.read();
    if( c == 0x7E) {
      received_buf_index = 0;
      received_buf[received_buf_index++] = c;
      if(received_buf[3] == 0x89)
        return 0;
      else 
        return 1;
    }
    else{
        received_buf[received_buf_index++] = c;
        return 0;
    }  
 }
}

void read_from_mavlink(){
  // Initialize the required buffers
  mavlink_message_t msg;
  mavlink_status_t status1;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
  lat = 0;
  lon = 0;
   
  // Pack the message
  mavlink_msg_heartbeat_pack(100, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);
   
  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
   
  // Send the message with the standard UART send function
  Uart.write(buf, len);
  while(Uart.available()){
    uint8_t c = Uart.read();
    //Serial.println("available");
    char ch = c;
    //Serial.println(ch, HEX);
    
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status1)){
      //Serial.println("message parsed");
      switch(msg.msgid){
        //case MAVLINK_MSG_ID_GPS_RAW_INT:
          //Serial.print("raw lat: ");
          //Serial.println(mavlink_msg_gps_raw_int_get_lat(&msg));
          //Serial.print("raw lon: ");
          //Serial.println(mavlink_msg_gps_raw_int_get_lon(&msg));
          //Serial.println("");
          //break;
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
          //Serial.print("lat: ");
          //Serial.println(mavlink_msg_global_position_int_get_lat(&msg));
          //Serial.print("lon: ");
          //Serial.println(mavlink_msg_global_position_int_get_lon(&msg));
          //Serial.print("alt: ");
          //Serial.println(mavlink_msg_global_position_int_get_alt(&msg));
          //Serial.print("compass: ");
          //Serial.println(mavlink_msg_global_position_int_get_hdg(&msg));
          //Serial.println("");
          lat = mavlink_msg_global_position_int_get_lat(&msg);
          lon = mavlink_msg_global_position_int_get_lon(&msg);
          break;       
      }      
    }
    
  }
}

void transmit_request_packet_header(uint32_t address_32bit){
  transmit_buf[0] = 0x7E;
  transmit_buf[1] = 0x00;
  transmit_buf[2] = 0x15;
  transmit_buf[3] = 0x00;
  transmit_buf[4] = 0x01;
  transmit_buf[5] = 0x00;
  transmit_buf[6] = 0x00;
  transmit_buf[7] = 0x00;
  transmit_buf[8] = 0x00;
  transmit_buf[9] = (address_32bit & 0xFF000000) >> 24;
  transmit_buf[10] = (address_32bit & 0x00FF0000) >> 16;
  transmit_buf[11] = (address_32bit & 0x0000FF00) >> 8;
  transmit_buf[12] = (address_32bit & 0x000000FF);
  transmit_buf[13] = 0x00;
}

void calculate_checksum(){
  uint8_t sum = 0;
  for(int i = 3; i < TRANSMIT_LENGTH-1; i++){
    //Serial.println(transmit_buf[i], HEX);
    sum += transmit_buf[i];
  }
  
  sum = 0xFF - sum;
  transmit_buf[TRANSMIT_LENGTH-1] = sum;  
}
