#include <Xbee_wifi_RX_API.h>
#define INPUT  /*Input*/
#define OUTPUT /*Output*/
/**
 * \brief Parse received frames for known packet protocols
 * \note Currently this code only supports RX64 frames: see Digi Xbee S6 User Guide 
 * \param INPUT _apiFrameUnion * apiFramePtr input a raw api frame to be parsed
 * \param OUTPUT _apiFrame64bit * apiFrame64bitPtr output a struct of parsed API mode 2 frame data
 */
 
void Xbee_HandleRxAPIFrame( INPUT _apiFrameUnion * apiFramePtr, OUTPUT _apiFrame64bit * apiFrame64bitPtr )
{
    switch(apiFramePtr->api_id)
    {
        case RX_RF_DATA_FRAME:
            /* process recieved Data Frames */
            uint8_t * baseDataPtr               = &( apiFramePtr->api_fd );
            apiFrame64bitPtr->api_id            =    apiFramePtr->api_id  ;
            apiFrame64bitPtr->api_src_address   = (uint64_t)baseDataPtr[RX_RF_ADDR_OFFSET];
            apiFrame64bitPtr->api_RSSI          = (uint8_t )baseDataPtr[RX_RF_RSSI_OFFSET];
            
            
            //find length of data from offsets
            for(i = 0; i < RX_RF_MAX_PAYLOAD_SIZE; i++)
             {
                /* Fill our payload */
                apiFrame64bitPtr->api_payload[i] = (uint8_t )baseDataPtr[RX_RF_DATA_OFFSET+i];
                
             } // End for(i = 0; i < RX_RF_MAX_PAYLOAD_SIZE; i++)
 
            apiFrame64bitPtr->api_src_address    = (uint64_t )baseDataPtr[RX_RF_ADDR_OFFSET];
             
          /* Can add other Frame protocols here */
//        case RX_IO_SAMPLE_FRAME:

          /* For all frame protocols not specified, do the default NOTHING*/
        default:
            //Discard all of the other frames because they are not worthy 
            break /* into dance */;
 
    } // END Xbee_HandleRxAPIFrame
 
}
