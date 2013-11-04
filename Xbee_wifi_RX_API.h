#ifndef XBEE_WIFI_RX_API_H
#define XBEE_WIFI_RX_API_H

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
 * \brief apiFrameUnion contains the raw api frame data.
 * The basic formation of the api frame is done in the receiver function.
 *
 */ 
typedef struct apiFrameUnion
{
    uint16_t api_length;                  /**< The length of frame we're operating on */
    uint8_t  api_id;                      /**< The type of frame we're operating on   */
    uint8_t  api_fd[RX_RF_MAX_FD_SIZE];   /**< The frame data                         */
 
} _apiFrameUnion;
 
 
/**
 * \brief RX64 api frame data
 * Passed around for easy access of api frame data
 * 
 */  
typedef struct apiFrame64bit
{
//    uint8_t  api_sf     = 0x7E;                 /* Not used */
//    uint16_t api_length;                        /* Not used */
    uint8_t  api_id;                              /**< The type of frame we're operating on   */
    uint64_t api_src_address;                     /**< Who did we receive the packet from     */
    uint8_t  api_RSSI;                            /**< The Received Signal Strength Indicator */
    uint8_t  api_opt;                             //= RX_RF_OPTION;
    uint8_t  api_payload[RX_RF_MAX_PAYLOAD_SIZE]; /**< The actual packet payload*/
 
 
} _apiFrame64bit;
 
void Xbee_HandleRxAPIFrame(_apiFrameUnion * apiFramePtr, _apiFrame64bit * apiFrame64bitPtr );
 
#endif /* XBEE_WIFI_RX_API_H */
