//#include <uVector.h>
//#include <XbeeS6.h>
#include "/home/walter/mavlink/arduino_include/common/mavlink.h"
#include <Servo.h>

HardwareSerial Uart = HardwareSerial();
Servo myServo;
int servoPos = 0;
int servoPin = 23;

int lat, lon;
int32_t last_lat, last_lon;
//Tx64Packet tx_packet;
//static uint8_t *tx_buf;
uint8_t received_buf[16];
int received_buf_index;
const int TRANSMIT_LENGTH = 26;
uint8_t transmit_buf[TRANSMIT_LENGTH];

int num_packets;

long previousMillis = 0;
long interval = 50; //in ms
void setup() {
    Serial.begin(57600);
    Uart.begin(58824);  //ardupilot
    Serial2.begin(58824);  //xbee
    lat = 0;
    lon = 0;
    last_lat = 0;
    last_lon = 0;
    received_buf_index = 0;
    transmit_request_packet_header(0xffffffff);
    
    num_packets = 0;
    myServo.attach(servoPin);
}

void loop(){
	int i =0;
	for(i = 0; i < 2; i++)
	{
   	read_from_mavlink();
   	if (lat != 0 && lon != 0){
   	  last_lat = lat;
   	  last_lon = lon;
   	}
   //if(read_from_xbee() == 1){
      //transmit_buf[14] = received_buf[11];// last byte of received addr
      //transmit_buf[15] = received_buf[12];// RSSI
      transmit_buf[14] = 0xDE;// last byte of received addr
      transmit_buf[15] = 0xAD;// RSSI
      transmit_buf[16] = servoPos & 0xFF;        // polarization
      transmit_buf[17] = last_lat >> 24;
      transmit_buf[18] = last_lat >> 16;
      transmit_buf[19] = last_lat >> 8;
      transmit_buf[20] = last_lat;
      transmit_buf[21] = last_lon >> 24;
      transmit_buf[22] = last_lon >> 16;
      transmit_buf[23] = last_lon >> 8;
      transmit_buf[24] = last_lon;
         //dump_xbee_buffer();
      calculate_checksum();
      /*for(int i = 0; i < TRANSMIT_LENGTH; i++){
        Serial.print(transmit_buf[i], HEX);
        Serial.print(" ");
      }
       Serial.println();*/
       digitalWrite(13, HIGH);
       //Serial2.write(transmit_buf, TRANSMIT_LENGTH);
       for(int i = 0; i < TRANSMIT_LENGTH; i++)
         Serial2.write(transmit_buf[i]);
       digitalWrite(13, LOW);
       
       //Serial.print("servo pos: ");
#if DEBUG_MODE
       Serial.print(servoPos);
       Serial.print(" ");
       //Serial.print("rssi: ");
       Serial.println(received_buf[12]);
#endif
       num_packets++;
         //dump_xbee_buffer();
     //  if (num_packets > 65){
         turn_motor();
         num_packets = 0;
	 }
         delay(2000);
     //  }
   }
}

void turn_motor(){
  if(servoPos == 0){
    servoPos = 90;
    myServo.write(servoPos);
  }
  else if(servoPos == 90){
    servoPos = 0;
    myServo.write(servoPos); 
  }
         //dump_xbee_buffer();
}

void dump_xbee_buffer(){
  while(Serial2.available()){
    Serial2.read();
  }
}

int read_from_xbee(){
  char c;
  while(Serial2.available()){
    c = Serial2.read();
    //Serial.print(c, HEX);
    //Serial.print(" ");
    if(c == 0x7E) {
      //Serial.println("");
      received_buf_index = 0;
      received_buf[received_buf_index++] = c;
      if(received_buf[3] == 0x89)
        return 0;
      else 
        return 1;
    }
    else{
        received_buf[received_buf_index++] = c;
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
  transmit_buf[2] = 0x16;
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
