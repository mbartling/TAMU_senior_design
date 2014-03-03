// Arduino MAVLink test code.

#include "/home/walter/mavlink/include/ardupilotmega/mavlink.h"

HardwareSerial Uart = HardwareSerial();
void setup() {
  Serial.begin(57600);
  Uart.begin(57600);
}

void loop() { 
  // Define the system type (see mavlink_types.h for list of possible types) 

  // Initialize the required buffers 
  mavlink_message_t msg; 
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // Pack the message
  // mavlink_message_heartbeat_pack(system id, component id, message container, system type, MAV_AUTOPILOT_GENERIC)
  mavlink_msg_heartbeat_pack(100, 200, &msg, MAV_TYPE_QUADROTOR, MAV_AUTOPILOT_GENERIC, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);

  // Copy the message to send buffer 
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

  for(int i = 0; i < len; i++)
   {
   Serial.print(buf[i], HEX);
   Serial.print(' ');
   }
  Serial.println();
  // Send the message (.write sends as bytes) 
  Serial.println();
  Serial.println();
  Uart.write(buf, len);
  //mavlink_msg_heartbeat_send(MAVLINK_COMM_0, system_type, autopilot_type);
  //uart0_send(buf, len);
  
  delay(1000);
  comm_receive();
}

void comm_receive() { 
  mavlink_message_t msg;
  mavlink_status_t status;

  //while(Uart.available() > 0)
  {
    uint8_t c;
    //c=222;
    //c= Uart.read();
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    int buffer_size=0;
    for(int i=0; i< MAVLINK_MAX_PACKET_LEN && Uart.available() > 0; i++, buffer_size=i)
     {
     buffer[i]=Uart.read();
     //Serial.print("c: ");
     //Serial.println(buffer[i]);
     }
     for(int i=0; i < buffer_size ; i++)
     {
       Serial.println(mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status));
      if(mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status))
       {
         Serial.print("please work");
       } 
     }
     /*
    while(Uart.available() > 0){
      c = Uart.read();
      //Serial.println(c);
      //Serial.print("c: ");
      Serial.println(c, HEX);
      if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status))
      {
        Serial.println("Hi!!!!!!!!!!!!!!!!!!");
        switch(msg.msgid)
        {
        case MAVLINK_MSG_ID_HEARTBEAT:
          {
            // E.g. read GCS heartbeat and go into
            // comm lost mode if timer times out
          }
          break;
        case MAVLINK_MSG_ID_GPS_STATUS:
        case MAVLINK_MSG_ID_PARAM_VALUE:
          Serial.println("Hi");
          break;
        default:
          //Do nothing
          break;
        } 
      }
    }*/
    /*Serial.println(packet.lat);
     Serial.println(packet.lon);
     Serial.println(packet.hdg);
     Serial.println();
     Serial.println();
     Serial.println();
     Serial.println();*/
  }
  //receive data over serial 
  /*while(Serial.available() > 0) { 
   		uint8_t c = Serial.read();
   		
   		//try to get a new message 
   		if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) { 
   			// Handle message
   			switch(msg.msgid) {
   			        case MAVLINK_MSG_ID_SET_MODE: {
   			        	// set mode
   			        }
   			        break;
   			        case MAVLINK_MSG_ID_ACTION:
   					// EXECUTE ACTION
   				break;
   				default:
   					//Do nothing
   				break;
   			}
   
   		} 
   		// And get the next one
   	}*/
}




void construct_pack(uint8_t &buf)
{
  
  
  
}
