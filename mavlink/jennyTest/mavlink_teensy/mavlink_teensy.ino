#include "/home/walter/mavlink/include/common/mavlink.h"

HardwareSerial Uart = HardwareSerial();

void setup() {
	Serial.begin(57600);
        Uart.begin(115200);
}

void loop() { 
	// Define the system type (see mavlink_types.h for list of possible types) 
	int system_type = MAV_TYPE_QUADROTOR;
	int autopilot_type = MAV_AUTOPILOT_GENERIC;
	
	// Initialize the required buffers 
	mavlink_message_t msg; 
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
	
	// Pack the message
	// mavlink_message_heartbeat_pack(system id, component id, message container, system type, MAV_AUTOPILOT_GENERIC)
	mavlink_msg_heartbeat_pack(100, 200, &msg, system_type, autopilot_type, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);	
	// Copy the message to send buffer 
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
	
	// Send the message (.write sends as bytes) 
	Uart.write(buf, len);
	
	comm_receive();
}

void comm_receive() { 
	mavlink_message_t msg; 
	mavlink_status_t status;

        char startbit = 0xFE;
        char buf[MAVLINK_MAX_PACKET_LEN];
	
	//receive data over serial 
	/*while(Uart.available() > 0) { 
		uint8_t c = Uart.read();
		
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

        int recsize = Uart.readBytesUntil(startbit, buf, MAVLINK_MAX_PACKET_LEN);
        
}


