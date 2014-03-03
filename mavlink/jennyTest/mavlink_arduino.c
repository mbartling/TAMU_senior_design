#include "serial.h"
#include "./libraries/mavlink/include/common/mavlink.h" //adjust directory accordingly "mavlink/include/common/mavlink.h"

void com_receive(int* fd);

int main(){

	//open com port
	char *portname = "/dev/ttyUSB0";
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);

	//terminate if unable to open com port
	if (fd < 0)
	{
	        error_message ("error %d opening %s: %s\n", errno, portname, strerror (errno));
	        return;
	}
	
	//configure com port connection
	set_interface_attribs (fd, B57600, 0);  // set speed to 57600 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking


	printf("serial connection established\n");
	
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN]; //buffer for sending and receiving messages
	int recsize = 0; //size of received message
	int i = 0; //loop variable


	while(1){
		//send heartbeat to ardupilot
		mavlink_msg_heartbeat_pack(100, 200, &msg, MAV_QUADROTOR, MAV_AUTOPILOT_GENERIC);
		uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
		write(fd, buf, len);

		//clear buffer
		memset(buf, 0, MAVLINK_MAX_PACKET_LEN);

		//read from serial
		recsize = read(fd, buf, sizeof buf);

		//interpret message if the size of received data is greater than 0
		if(recsize > 0){
			
			mavlink_message_t msg;
			mavlink_status_t status;

#ifdef DEBUG
			printf("Bytes Received: %d\n Datagram: ", (int)recsize);
#endif

			for(i = 0; i < recsize; i++){
				char temp = buf[i];
#ifdef DEBUG
				printf("%02x ", (unsigned char) temp);
#endif

				//use mavlink function to parse message
				if(mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)){
#ifdef DEBUG
					printf("\nReceived packet: SYS: %d, COMP:%d, LEN:%d, MSG ID: %d\n", msg.sysid, msg.compid, msg.len, msg.msgid);
#endif
					//extract data depending on the id number of the messages
					switch(msg.msgid){
//						case MAVLINK_MSG_ID_GPS_RAW_INT:
//							printf("raw lat: %d, raw lon: %d\n", mavlink_msg_gps_raw_int_get_lat(&msg), mavlink_msg_gps_raw_int_get_lon(&msg));
//							break;
						case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
							//printf("lat: %d, lon: %d, alt: %d compass: %d\n", mavlink_msg_global_position_int_get_lat(&msg), mavlink_msg_global_position_int_get_lon(&msg),
							//	mavlink_msg_global_position_int_get_alt(&msg), mavlink_msg_global_position_int_get_hdg(&msg));
							//break;
							printf("lat: %d, lon: %d, alt: %d\n", mavlink_msg_global_position_int_get_lat(&msg), mavlink_msg_global_position_int_get_lon(&msg),
								mavlink_msg_global_position_int_get_alt(&msg));
							break;
						default:
							printf("default\n");
//						//add more cases here for other messages (see https://pixhawk.ethz.ch/mavlink/#DATA_STREAM and header(.h) files in mavlink/include/common/)
					}
				}

			}
			printf("\n\n");
		}

		memset(buf, 0, MAVLINK_MAX_PACKET_LEN);
		sleep(1);
	}

}

