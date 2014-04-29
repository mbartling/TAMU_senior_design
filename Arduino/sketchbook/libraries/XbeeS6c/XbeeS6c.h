#ifndef XBEES6C_H
#define XBEES6C_H

/**
 * @file XbeeS6c.h
 * @author Michael Bartling (michael.bartling15+AMP_RF@gmail.com)
 * @brief C based Xbee S6B library
 */
#include <stdint.h>

#define ID_TX64_REQ   0x00
#define ID_REMOTE_AT  0x07
#define ID_AT_COMMAND 0x08
#define ID_TX_IPV4    0x20
#define DISABLE_ACK   0		/* 1: disable acknowledgement */
#define APPLY_CHANGES 0x02  /* for remote AT Commands*/

/*Need to calculate 2 things, first is the length and second is the checksum which is from byte three to the end*/
/**
 * Generic Api_frame can accept both local and remote api_frames
 */
typedef struct Api_frame
{
  uint8_t  _api_fid;			//!< Frame type
  uint8_t  _api_seqno;			//!< Sequence number of the frame
  void *   _api_command; 		//!< Either local or remote frame
  uint16_t _payload_length;		//!< How much data were sending
  void *   _payload;			//!< Pointer to payload
  
} Api_frame_t;

/**
 * @name Address64
 * @brief 64 bit address of remote node
 */
typedef struct Address64
{
  uint32_t _upper;
  uint32_t _lower;
} Address64_t;

/*
typedef struct command_Tx64
{
  Address64_t _tx_dst;
  uint8_t     _tx_opts = DISABLE_ACK;
} command_Tx64_t;
*/
/**
 * @name command_Remote
 * All remote frames have this structure
 */
typedef struct command_Remote
{
  Address64_t _cmd_dst_addr;
  uint8_t     _cmd_opts;
} command_Remote_t;

/**
 * @name increment_frame
 * @brief increment the sequence number of a packet
 * @param packet
 */

void increment_frame(Api_frame &packet);

/**
 * @name req_frame
 * @brief Initialize a packet
 * @return the expected length*/
uint16_t req_frame(Api_frame &packet, uint8_t api_fid, uint8_t *payload, uint16_t payload_length);
void set_dst_address(uint32_t upper, uint32_t lower);

/**
 * @name prepare2send
 * @brief Prepare a packet for sending to Xbee
 * @param buffer
 * @param packet
 * @return
 */
int prepare2send(uint8_t* buffer, Api_frame_t packet);


#endif /*XBEES6C_H*/
