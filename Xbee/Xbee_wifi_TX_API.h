/**
 * \brief Xbee wifi S6B TX 64 request
 * \author Michael Bartling bart4128@tamu.edu
 * \author Rabih Abifaraj   rabifaraj@tamu.edu
 * \date 11/4/2013
 * \version 1.0
 * 
 * \copyright The MIT License (MIT)
 */

#ifndef XBEE_WIFI_TX_API_H
#define XBEE_WIFI_TX_API_H

#include <stdint.h>

#define TX_ENABLE_ACK			 0x00	
#define TX_DISABLE_ACK			 0x01

#define TX_RF_START_DELIMITER	 0x7E
#define RF_RESERVED              0x00           /* To be implemented Later  		*/
//#define TX_RF_DATA_FRAME 0xB0                 /* ipv4 TX mode             		*
#define TX_RF_DATA_FRAME         0x00           /* TX transmit Packet 64 bit		*/
#define TX_RF_PROTOCOL_UDP       0x00           /* NOT USED                 		*/
#define TX_RF_PROTOCOL_TCP       0x01           /* NOT USED                 		*/
#define TX_RF_STATUS             RF_RESERVED
#define TX_RF_OPTION             TX_DISABLE_ACK /* For now we do not want auto ack 	*/
#define TX_RF_MAX_PAYLOAD_SIZE   8              /* Since only using small      		*/
                                                /* Packet size, dummy+RSSI      	*/
#define TX_RF_MAX_FD_SIZE        8              /* For getting initial pkt    		*/

#define TX_RF_DELI_OFFSET		 0
#define TX_RF_LENT_OFFSET		 1	
#define TX_RF_IDEN_OFFSET        3
#define TX_RF_FRID_OFFSET        4
#define TX_RF_ADDR_OFFSET        5
#define TX_RF_OPTI_OFFSET        13
#define TX_RF_DATA_OFFSET        14
 
/**
 * \brief apiFrameUnion contains the raw api frame data.
 * The basic formation of the api frame is done in the receiver function.
 *
 */ 
//typedef struct apiFrameUnion
//{
//    uint16_t api_length;                  /**< The length of frame we're operating on */
//    uint8_t  api_id;                      /**< The type of frame we're operating on   */
//    uint8_t  api_fd[TX_RF_MAX_FD_SIZE];   /**< The frame data                         */
// 
//} _apiFrameUnion;

/**
 * \brief Simple transmission request. 
 * We can use this to 
 *
 */
typedef struct simpleTx64Request
{
	int32_t   sTx_status;	  	/**< TX request status						 */
	uint64_t  sTx_dst_address;	/**< Who to send to 						 */
	uint16_t  sTx_num_bytes;    /**< Number of payload bytes				 */
	uint8_t * sTx_payload;		/**< Pointer to data buffer of sTx_num_bytes */

} _simpleTx64Request;


/**
 * \brief TX64 api frame data
 * Passed around for easy access of api frame data
 * 
 */ 
 
typedef struct apiFrameTx64bit
{
    uint8_t  api_id;                              /**< The type of frame we're operating on   */
    uint8_t  api_fid;                             /**< The unique id of the frame		      */
    uint64_t api_dst_address;                     /**< Who do we send to				      */
    //uint8_t  api_RSSI;                          /**< The Received Signal Strength Indicator */
    uint8_t  api_opt;                             //= TX_RF_OPTION;
    //uint8_t  api_payload[TX_RF_MAX_PAYLOAD_SIZE]; /**< The actual packet payload*/
    uint8_t * api_payload;						  /**< The actual packet payload*/
 
} _apiFrameTx64bit;

/**
 * \brief Actual TX 64 packet structure
 * used to send the data.
 * 
 */ 

typedef struct Tx64Packet
{
	uint8_t  		  packet_sf;			/**< Packet Start Frame 0x7E */
	uint16_t 		  packet_length;		/**< Packet length in bytes	 */
	_apiFrameTx64bit *packet_apiFramePtr;	/**< Packet Api Frame		 */
	uint8_t  		  packet_chksum;		/**< Packet Check Sum	 	 */
} _Tx64Packet;

void Xbee_FormTxAPIFrame( _apiFrameTx64bit * apiFrame64bitPtr, _simpleTx64Request * TxRequest);
void Xbee_Broadcast( /**\todo IMPLEMENT THIS */ );

void Xbee_HandleTxCleanup( _simpleTx64Request * TxRequest );

#endif /* XBEE_WIFI_TX_API_H */
