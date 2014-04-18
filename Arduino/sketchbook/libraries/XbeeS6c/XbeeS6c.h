#ifndef __XBEES6C_H__
#define __XBEES6C_H__

#include <stdint.h>

#define ID_TX64_REQ   0x00
#define ID_REMOTE_AT  0x07
#define ID_AT_COMMAND 0x08
#define ID_TX_IPV4    0x20
#define DISABLE_ACK   0
#define APPLY_CHANGES 0x02

/*Need to calculate 2 things, first is the length and second is the checksum which is from byte three to the end*/
typedef struct Api_frame
{
  uint8_t  _api_fid;
  uint8_t  _api_seqno;
  void *   _api_command;
  uint16_t _payload_length;
  void *   _payload;
  
} Api_frame_t;

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
typedef struct command_Remote
{
  Address64_t _cmd_dst_addr;
  uint8_t     _cmd_opts;
} command_Remote_t;

void increment_frame(Api_frame &packet);
/* Return the expected length*/
uint16_t req_frame(Api_frame &packet, uint8_t api_fid, uint8_t *payload, uint16_t payload_length);
void set_dst_address(uint32_t upper, uint32_t lower);

int prepare2send(uint8_t* buffer, Api_frame_t packet);


#endif /*__XBEES6C_H__*/
