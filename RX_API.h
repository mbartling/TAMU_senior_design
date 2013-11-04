#ifndef RX_API_h
#include <stdint.h>
 
#define RF_RESERVED              0x00           /* To be implemented Later  */
//#define RX_RF_DATA_FRAME 0xB0                 /* ipv4 RX mode             *
#define RX_RF_DATA_FRAME         0x80           /* Rx receive Packet 64 bit */
#define RX_RF_PROTOCOL_UDP       0x00           /* NOT USED                 */
#define RX_RF_PROTOCOL_TCP       0x01           /* NOT USED                 */
#define RX_RF_STATUS             RF_RESERVED
#define RX_RF_OPTION             RF_RESERVED
#define RX_RF_MAX_PAYLOAD_SIZE   8              /* Since only using small   */
                                                /* Packet size, dummy+RSSI  */
#define RX_RF_MAX_FD_SIZE        8              /* For getting initial pkt  */
 
#define RX_RF_ADDR_OFFSET        4  - 1  
#define RX_RF_RSSI_OFFSET        12 - 1
#define RX_RF_OPTI_OFFSET        13 - 1
#define RX_RF_DATA_OFFSET        14 - 1
 
/**
 *
 *
 *
 *
 *
 */ 
typedef struct apiFrameUnion
{
    uint16_t api_length;
    uint8_t  api_id;
    uint8_t  api_fd[RX_RF_MAX_FD_SIZE];
 
} _apiFrameUnion;
 
 
/**
 * /brief 
 * 
 *
 *
 *
 */  
typedef struct apiFrame64bit
{
//    uint8_t  api_sf     = 0x7E;
//    uint16_t api_length;
    uint8_t  api_id;
    uint64_t api_src_address;
    uint8_t  api_RSSI;
    uint8_t  api_opt;//     = RX_RF_OPTION;
    uint8_t  api_payload[RX_RF_MAX_PAYLOAD_SIZE];
 
 
} _apiFrame64bit;
 
void Xbee_HandleRxAPIFrame(_apiFrameUnion * apiFramePtr, _apiFrame64bit * apiFrame64bitPtr );
 
#endif
