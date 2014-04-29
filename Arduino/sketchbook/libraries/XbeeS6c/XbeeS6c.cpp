/**
 * @file XbeeS6c.cpp
 * @author Michael Bartling (michael.bartling15+AMP_RF@gmail.com)
 * @brief This is a much more lightweight version of the Xbee S6 Code
 *
 * todo: Rename file to c file
 */

//Need to ifdef ARDUINO or something here
#include "Arduino.h"

#include "XbeeS6c.h"
#include <stdlib.h>
#include <string.h>

#define DEBUG_MODE 1
#ifdef DEBUG_MODE
#include <stdio.h>
#define PRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
#define PRINTF(...) /* Dummy */
#endif
/*Need to calculate 2 things, first is the length and second is the checksum which is from byte three to the end*/
/*
   typedef struct Api_frame
   {
   uint8_t _api_fid;
   uint8_t _api_seqno;
   void *  _api_command;
   int16_t _payload_length;
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

// Lets keep a private copy of the current address and payload
Address64_t private_dst_address;
command_Remote_t private_remote;

/*For incrementing the frame counter */
void increment_frame(Api_frame &packet)
{
	uint8_t seqno;
	seqno = packet._api_seqno ++;
	if( seqno == 0 ) seqno++;
	packet._api_seqno = seqno;

}

/* Return the expected length*/
/**
 * @name req_frame
 * @brief Configure a packet for a specific frame type
 * @param packet Pointer to a packet
 * @param api_fid Frame type requested
 * @param payload pointer to payload data
 * @param payload_length number of bytes of payload to copy
 * @return number of bytes in packet
 */
uint16_t req_frame(Api_frame &packet, uint8_t api_fid, uint8_t *payload, uint16_t payload_length)
{
	uint16_t length;
	switch(api_fid)
	{
	//Note, these lengths do not include the 0x7E and the Packet Length Fields in the header
	case ID_TX64_REQ:
	{
		length = 11;
		packet._api_fid = api_fid;
		packet._api_command = &private_remote;
		private_remote._cmd_opts = DISABLE_ACK;
		packet._payload = &payload[0];
		break;
	}
	case ID_REMOTE_AT:
	{
		length = 11;
		if(payload_length < 2) return -1; //Should have gotten at least the 2 bytes "DL" part for ATDL
		packet._api_fid = api_fid;
		packet._api_command = &private_remote;
		private_remote._cmd_opts = APPLY_CHANGES;
		packet._payload = &payload[0];
		break;

	}
	case ID_AT_COMMAND:
	{
		length = 2;
		if(payload_length < 2) return -1; //Should have gotten at least the 2 bytes "DL" part for ATDL
		packet._api_fid = api_fid;
		packet._api_command = NULL; //Then check against null in the command processing
		packet._payload = &payload[0];
		break;
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
	packet._payload_length = payload_length;
	return length + payload_length;
}

/**
 * @name set_dst_address
 * @brief set the 64 bit destination address
 * @note need to check endianness
 * @param upper word
 * @param lower word
 */
void set_dst_address(uint32_t upper, uint32_t lower)
{
	private_dst_address._upper = upper;
	private_dst_address._lower = lower;
	private_remote._cmd_dst_addr = private_dst_address;
}

/**
 * @name prepare2send
 * @brief prepare the packet buffer for sending
 * @param buffer
 * @param packet
 * @return
 */
int prepare2send(uint8_t* buffer, Api_frame_t packet)
{
	uint16_t length;
	uint8_t chkSum;
	int i;
	int offset = 0;
	switch(packet._api_fid)
	{
	case ID_TX64_REQ:
	{
		length = 11;
		break;
	}
	case ID_REMOTE_AT:
	{
		length = 11;
		break;
	}
	case ID_AT_COMMAND:
	{
		length = 2;
		break;
	}
	default:
		PRINTF("Failed to prepare 2 send\n");
		return -1; // Invalid api_id
	}
	memcpy(&buffer[offset], &(packet._api_fid), sizeof(packet._api_fid));
	offset += sizeof(packet._api_fid);

	memcpy(&buffer[offset], &(packet._api_seqno), sizeof(packet._api_seqno));
	offset += sizeof(packet._api_seqno);
	PRINTF("Offset is %d\n", offset);

	//Alignment issues?
	if(packet._api_command != NULL)
	{
		memcpy(&buffer[offset], &(((command_Remote_t *)packet._api_command)->_cmd_dst_addr),
				sizeof(((command_Remote_t *)packet._api_command)->_cmd_dst_addr));
		offset += sizeof(((command_Remote_t *)packet._api_command)->_cmd_dst_addr);

		memcpy(&buffer[offset], &(((command_Remote_t *)packet._api_command)->_cmd_opts),
				sizeof(((command_Remote_t *)packet._api_command)->_cmd_opts));
		offset += sizeof(((command_Remote_t *)packet._api_command)->_cmd_opts);

		PRINTF("remote Packet cmd %d\n", sizeof(((command_Remote_t *)packet._api_command)->_cmd_opts));
		PRINTF("remote Packet address %d\n",sizeof(((command_Remote_t *)packet._api_command)->_cmd_dst_addr));
		PRINTF("Sizeof Remote %d\n", sizeof(command_Remote_t));
		PRINTF("Sizeof api_cmd %d\n", sizeof((command_Remote_t *)packet._api_command));
		//offset += sizeof(command_Remote_t);
	}//If we have a remote field



	PRINTF("Offset is %d\n", offset);
	memcpy(&buffer[offset], packet._payload, packet._payload_length);
	offset += packet._payload_length; //Check this value against the length returned from form frame
	chkSum = 0;
	for( i = 0; i < offset; i++)
	{
		chkSum += buffer[i];
		PRINTF("%X ", chkSum);
	}
	//Note that the checksum adds one extra byte to the length shich is not counted in the returned length
	//
	chkSum = 0xFF - chkSum;
	PRINTF("Checksum: %x\n", chkSum);
	buffer[offset] = chkSum;
	offset += sizeof(chkSum);
	return offset;

}
/*
void escape_chars( uint8_t temp_byte, uint16_t &byte_cnt)
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

