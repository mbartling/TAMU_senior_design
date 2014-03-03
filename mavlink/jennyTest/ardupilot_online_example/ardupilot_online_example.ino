#include "/home/walter/mavlink/jennyTest/libraries/mavlink/include/mavlink.h"

HardwareSerial Uart = HardwareSerial();

void setup() {
    Serial.begin(57600);
    Uart.begin(57600);
}

void loop(){
  // Define the system type, in this case an airplane
  int system_type = MAV_FIXED_WING;
  int autopilot_type = MAV_AUTOPILOT_GENERIC;
   
  // Initialize the required buffers
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
   
  // Pack the message
  // mavlink_message_heartbeat_pack(system id, component id, message container, system type, MAV_AUTOPILOT_GENERIC)
  mavlink_msg_heartbeat_pack(100, 200, &msg, system_type, autopilot_type);
   
  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
   
  // Send the message with the standard UART send function
  uart0_send(buf, len);
}
