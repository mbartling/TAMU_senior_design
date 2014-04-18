#include "/home/walter/mavlink/arduino_include/common/mavlink.h"
#include "/home/walter/mavlink/arduino_include/ardupilotmega/ardupilotmega.h"
#include "/home/walter/workspace/routing/route.h" //defines num_waypoints and lats[] and lons[]

HardwareSerial Uart = HardwareSerial();\
int fixed_yaw = 0;

void setup(){
  Uart.begin(58824);
  do{
    send_heartbeat();
    send_clear_mission();    
  }while(read_mission_ack() != 0);
  
  int current_waypoint = 0;
  do{
    Serial.println("sending mission count");
    send_mission_count(num_waypoints*2 + 1 );
  }while(read_mission_request() != current_waypoint);
  
  current_waypoint++;  
  do{
    Serial.println("sending dummy condition yaw 0");
    send_condition_yaw(current_waypoint-1, fixed_yaw);
  }while(read_mission_request() != current_waypoint);
    
  for(int i = 0; i < num_waypoints; i++){
    current_waypoint++;
    do{
      Serial.println("sending condition yaw");
      send_condition_yaw(current_waypoint-1, fixed_yaw);
    }while(read_mission_request() != current_waypoint);
    current_waypoint++;
    if(i == num_waypoints-1){
      do{
        Serial.print("sending waypoint ");
        Serial.println(i+1);
        send_waypoint(current_waypoint-1, lats[i], lons[i], 15);
      }while(read_mission_ack() != 0);
    }
    else{
      do{
        Serial.println("sending waypoint");
        send_waypoint(current_waypoint-1, lats[i], lons[i], 15);
      }while(read_mission_request() != current_waypoint);
    }
  }
}

void loop(){
  Serial.println("loop");
  delay(1000);
  
}

void send_mission_count(uint16_t count){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
  mavlink_msg_mission_count_pack(100, 200, &msg, 1, 0, count);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  
  Uart.write(buf, len);
}

void send_condition_yaw(uint16_t seq, float degree){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
  mavlink_msg_mission_item_pack(100, 200, &msg, 1, 0, seq, MAV_FRAME_GLOBAL, MAV_CMD_CONDITION_YAW, 1, 1, degree, 0, 0, 0, 0, 0, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  
  Uart.write(buf, len);
}

void send_waypoint(uint16_t seq, float lat, float lon, float alt){
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
  mavlink_msg_mission_item_pack(100, 200, &msg, 1, 0, seq, MAV_FRAME_GLOBAL, MAV_CMD_NAV_WAYPOINT, 1, 1, 0, 0, 0, 0, lat, lon, alt);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  
  Uart.write(buf, len);
}


void send_clear_mission(){
  mavlink_message_t msg;
  //mavlink_status_t status1;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
  mavlink_msg_mission_clear_all_pack(100, 200, &msg, 1, 0);
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  
  Uart.write(buf, len);
}

void send_heartbeat(){
  // Initialize the required buffers
  mavlink_message_t msg;
  mavlink_status_t status1;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
  // Pack the message
  mavlink_msg_heartbeat_pack(100, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);
   
  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
   
  // Send the message with the standard UART send function
  Uart.write(buf, len);
}

int read_mission_ack(){
  mavlink_message_t msg;
  mavlink_status_t status1;
 
  while(Uart.available()){
    uint8_t c = Uart.read();
    char ch = c;
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status1)){
      switch(msg.msgid){
        case MAVLINK_MSG_ID_MISSION_ACK:
          Serial.print("target system");
          Serial.println(mavlink_msg_mission_ack_get_target_system(&msg));
          Serial.print("target component");
          Serial.println(mavlink_msg_mission_ack_get_target_component(&msg));
          Serial.print("ack type");
          Serial.println(mavlink_msg_mission_ack_get_type(&msg));
          return mavlink_msg_mission_ack_get_type(&msg); //returns 0 if accepted 
        default:
          Serial.println("nothing");
      }     
    }
  }
  
  return -1;
}

int read_mission_request(){
  mavlink_message_t msg;
  mavlink_status_t status1;
 
  while(Uart.available()){
    uint8_t c = Uart.read();
    char ch = c;
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status1)){
      switch(msg.msgid){
        case MAVLINK_MSG_ID_MISSION_REQUEST:
          Serial.print("sequence number ");
          Serial.println(mavlink_msg_mission_request_get_seq(&msg));
          return mavlink_msg_mission_request_get_seq(&msg); 
        default:
          Serial.println("nothing");
      }     
    }
  }
  return -1;
}
