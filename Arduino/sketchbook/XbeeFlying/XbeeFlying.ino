#include <uVector.h>
#include <XbeeS6.h>
#include "/home/walter/mavlink/arduino_include/common/mavlink.h"

HardwareSerial MavSerial = HardwareSerial();
HardwareSerial Xbee = HardwareXbee();

#define ADDRESS_BYTE 11
#define RSSI_BYTE 12
int lat, lon;
int32_t last_lat, last_lon;
Tx64Packet tx_packet;
static uint8_t *tx_buf;
uint8_t received_buf[16];
int received_buf_index;
const int TRANSMIT_LENGTH = 25;
uint8_t transmit_buf[TRANSMIT_LENGTH];


void setup() {
	Serial.begin(57600);
	MavSerial.begin(58824);  //ardupilot
	Xbee.begin(58824);  //xbee
	lat = 0;
	lon = 0;
	last_lat = 0;
	last_lon = 0;
	received_buf_index = 0;
	//    transmit_buf_index = 0;
	tx_packet.set_Address(0x00000000c0a80166);
	tx_buf = get_buffer();
	//transmit_request_packet_header(0xc0a80166);
	transmit_request_packet_header(0xffffffff);
}

void loop(){
	read_from_mavlink();
	if (lat != 0 && lon != 0){
		//Serial.print(lat);
		//Serial.print(" ");
		//Serial.println(lon);
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
		tx_packet.push_back(received_buf[ADDRESS_BYTE]);
		tx_packet.push_back(received_buf[RSSI_BYTE]);
		tx_packet.push_back(lat >> 24) ;
		tx_packet.push_back(lat >> 16);
		tx_packet.push_back(lat >> 8);
		tx_packet.push_back(lat);
		tx_packet.push_back(lon >> 24);
		tx_packet.push_back(lon >> 16);
		tx_packet.push_back(lon >> 8);
		tx_packet.push_back(lon);

		uint16_t length = tx_packet.prepare2send();

		Serial.print("Packet: ");
		for(int i = 0; i < length; i++)
		{
			Xbee.write(tx_buf[i]);
		}
		for(int i = 0; i < length; i++){
			Serial.print(tx_buf[i], HEX);
			Serial.print(" ");
		}
		Serial.println();
		tx_packet.clear_payload();
	}

	/* This function will have issues if it finds a 7E in the payload*/
	/* Need to read in all requested payload bytes to be safe*/
	int read_from_xbee(){
		char c;
		int lenH, lenL;
		int len;
		//if (Xbee.available()>3) // then read msg len worth of bytes
		if(Xbee.available())
		{
			c = Xbee.read();
			//lenH = Xbee.read();
			//lenL = Xbee.read();
			if( c == 0x7E) {
				//7E May not mean Start Frame So wait until we get a valid Rx Packet
				received_buf[received_buf_index++] = c;
				if(received_buf[3] == 0x80) //If we receive a valid Rx64 Packet return
				{
					received_buf_index = 0;
					received_buf[received_buf_index++] = c;
					return 1;
					//return received_buf_index; //This way we forward payloads
				}
				else 
					return 0;
			}
			else{
				received_buf[received_buf_index++] = c;
				return 0;
			}  
		}// end if Xbee Available
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
		MavSerial.write(buf, len);
		while(MavSerial.available()){
			uint8_t c = MavSerial.read();
			char ch = c;

			if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status1)){
				//Serial.println("message parsed");
				switch(msg.msgid){
					case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
						lat = mavlink_msg_global_position_int_get_lat(&msg);
						lon = mavlink_msg_global_position_int_get_lon(&msg);
						break;       
					default:
						Serial.print("Unknown Comand from Mavlink Parse");
				}      
			}

		}
	}
