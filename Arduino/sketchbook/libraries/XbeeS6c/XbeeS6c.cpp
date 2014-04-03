/*This is a much more lightweight version of the Xbee S6 Code*/
#include "Arduino.h"
#include "XbeeS6c.h"

#define ID_TX64_REQ   0x00
#define ID_REMOTE_AT  0x07
#define ID_AT_COMMAND 0x08
#define ID_TX_IPV4    0x20
#define DISABLE_ACK   0
#define APPLY_CHANGES 0x02

/*Need to calculate 2 things, first is the length and second is the checksum which is from byte three to the end*/
/*
typedef struct Api_frame
{
  uint8_t _api_fid;
  uint8_t _api_seqno;
  void *  _api_command;
  void *  _payload;
  
} Api_frame_t;
*/
/*
typedef struct Address64
{
  uint32_t _upper;
  uint32_t _lower;
} Address64_t
*/
/*
typedef struct command_Tx64
{
  Address64_t _tx_dst;
  uint8_t     _tx_opts = DISABLE_ACK;
} command_Tx64_t;
*/
/*
typedef struct command_Remote
{
  Address64_t _cmd_dst_addr;
  uint8_t     _cmd_opts;
} command_Remote_t;
*/
Address64_t private_dst_address;
command_Remote_t private_remote;

/*For incrementing the frame counter */
void increment_frame(Api_frame &packet)
{
  uint8_t seqno;
  seqno = (packet->_api_seqno)++;
  if( seqno == 0 ) seqno++;
  packet->_api_seqno = seqno;
  
}

/* Return the expected length*/
uint16_t req_frame(Api_frame &packet, uint8_t api_fid, uint8_t *payload, uint16_t payload_length)
{
  uint16_t length;
  switch(api_fid)
  {
    case ID_TX64_REQ:
    {
      length = 13;
      packet->_api_fid = api_fid;
      packet->_api_command = &private_tx64;
      packet->_payload = &payload;
    }
    case ID_REMOTE_AT:
    {
      length = 11;
      if(payload_length < 2) return -1; //Should have gotten at least the 2 bytes "DL" part for ATDL
      packet->_api_fid = api_fid;
      packet->_api_command = &private_tx64;
      private_remote->_cmd_opts = APPLY_CHANGES;
      packet->_payload = &payload;
      
    }
    case ID_AT_COMMAND:
    {
      length = 2;
      if(payload_length < 2) return -1; //Should have gotten at least the 2 bytes "DL" part for ATDL
      packet->_api_fid = api_fid;
      packet->_api_command = NULL; //Then check against null in the command processing
      packet->_payload = &payload;
    }
   /*
    case ID_TX_IPV4:
    {
      length = 12;
    }
    */
    default:
      //Invalid Location
      return -1;
      break;
  }
  return length + payload_length;
}

void set_dst_address(uint32_t upper, uint32_t lower)
{
  private_dst_address._upper = upper;
  private_dst_address._lower = lower;
}

int Tx64_request(uint8_t *payload, uint16_t payload_length);
int Tx64_request(uint8_t *payload, uint16_t payload_length)
{
  
}
/*
void escape_chars(uint8_t temp_byte, uint16_t &byte_cnt)
{
	switch(temp_byte)
	{
		case 0x7E:
			tx_buffer[byte_cnt] = 0x7D;
			byte_cnt++;
			tx_buffer[byte_cnt] = temp_byte ^ 0x20;
			byte_cnt++;
			break;
		case 0x7D:
			tx_buffer[byte_cnt] = 0x7D;
			byte_cnt++;
			tx_buffer[byte_cnt] = temp_byte ^ 0x20;
			byte_cnt++;
			break;
		case 0x11:
			tx_buffer[byte_cnt] = 0x7D;
			byte_cnt++;
			tx_buffer[byte_cnt] = temp_byte ^ 0x20;
			byte_cnt++;
			break;
		case 0x13:
			tx_buffer[byte_cnt] = 0x7D;
			byte_cnt++;
			tx_buffer[byte_cnt] = temp_byte ^ 0x20;
			byte_cnt++;
			break;
		default:
			tx_buffer[byte_cnt] = temp_byte;
			byte_cnt++;
			break;
		}
}

*/
