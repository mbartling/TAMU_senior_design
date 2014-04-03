#ifundef __XBEES6C_H__
#define __XBEES6C_H__

#define ID_TX64_REQ   0x00
#define ID_REMOTE_AT  0x07
#define ID_AT_COMMAND 0x08
#define ID_TX_IPV4    0x20
#define DISABLE_ACK   0
#define APPLY_CHANGES 0x02

/*Need to calculate 2 things, first is the length and second is the checksum which is from byte three to the end*/
typedef struct Api_frame
{
  uint8_t _api_fid;
  uint8_t _api_seqno;
  void *  _api_command;
  void *  _payload;
  
} Api_frame_t;

typedef struct Address64
{
  uint32_t _upper;
  uint32_t _lower;
} Address64_t
/*
typedef struct command_Tx64
{
  Address64_t _tx_dst;
  uint8_t     _tx_opts = DISABLE_ACK;
} command_Tx64_t;
*/
typedef struct command_Remote /*_AT*/
{
  Address64_t _cmd_dst_addr;
  uint8_t     _cmd_opts;
} command_Remote_t;

#endif /*__XBEES6C_H__*/
