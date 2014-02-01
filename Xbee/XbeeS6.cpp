#include "XbeeS6.h"
#define BYTE_MASK(inVal, offset) (uint8_t)((inVal >> offset) & 0xFF)

int the_endianness;

/**
 * This should be fine.
 */
enum {
	BIG_ENDIAN,
	LITTLE_ENDIAN
};


/* This will help with debugging
 * We can use this to figure out bytes are being packed.
 */
int check_endianness()
{
	int i = 1;
	char * ptr = (char *) &i;

	if( ptr[0] == 1)
	{
		the_endianness = LITTLE_ENDIAN;
		return LITTLE_ENDIAN;
	}
	else
	{
		the_endianness = BIG_ENDIAN;
		return BIG_ENDIAN;
	}
}

/**
 * Set the destination address
 */
int Tx64Packet::set_Address(uint64_t new_address)
{
	dst_address.b0 = BYTE_MASK(new_address, 0);
	dst_address.b1 = BYTE_MASK(new_address, 8);
	dst_address.b2 = BYTE_MASK(new_address, 16);
	dst_address.b3 = BYTE_MASK(new_address, 24);
	dst_address.b4 = BYTE_MASK(new_address, 32);
	dst_address.b5 = BYTE_MASK(new_address, 40);
	dst_address.b6 = BYTE_MASK(new_address, 48);
	dst_address.b7 = BYTE_MASK(new_address, 56);

	return dst_address.b0 + dst_address.b1 + dst_address.b2 + dst_address.b3 + dst_address.b4 + dst_address.b5 + dst_address.b6 + dst_address.b7;
	/*
	   dst_address = {new_address};
	 */
}

/**
 * Manually Calculate the checksum
 */
int Tx64Packet::calc_chkSum()
{
	uint8_t sum;

	sum += sf;
	sum += (length & 0xFF) + ((length >> 8) & 0xFF);
	sum += API_frame_id;
	sum += seqno;
	sum += dst_address.b0;
	sum += dst_address.b1;
	sum += dst_address.b2;		
	sum += dst_address.b3;		
	sum += dst_address.b4;		
	sum += dst_address.b5;		
	sum += dst_address.b6;		
	sum += dst_address.b7;
	sum += tx_opts;

	for(std::vector<uint8_t>::iterator it = payload.begin(); it != payload.end(); ++it)
	{
		sum += *it;
	}

	sum = 0xFF - sum; // Final Part in checksum calculation
	return sum;
}

/**
 * Get the 64 bit address as a uint64_t
 */
uint64_t Tx64Packet::get_Address()
{
	uint64_t temp;
	if(sizeof(temp) != sizeof(dst_address))
	{
		PRINTF("Size of Address do not match!\n");
		return -1;
	}

	memcpy(&temp, &dst_address, sizeof(temp));
	return temp;
}



/**
 * Can override this function for the std stream to trick the arduino into forming a packet
 * or we could write this to a global buffer or something.
 *
 * Another Option is to replace is stream with the arduino stream library
 */

//uint16_t Tx64Packet:: operator <<( uint8_t * buffer, Tx64Packet *packet)
//{
//	//uint64_t temp = packet.get_Address();
//	uint16_t byte_cnt;
//
//	memcpy(&buffer[byte_cnt], packet->sf			, sizeof(packet->sf			 ) );   byte_cnt += sizeof(packet->sf			);  //buffer += sizeof(packet->sf			);
//	memcpy(&buffer[byte_cnt], packet->length		, sizeof(packet->length		 ) );   byte_cnt += sizeof(packet->length		);  //buffer += sizeof(packet->length		);
//	memcpy(&buffer[byte_cnt], packet->API_frame_id	, sizeof(packet->API_frame_id) );   byte_cnt += sizeof(packet->API_frame_id );  //buffer += sizeof(packet->API_frame_id);
//	memcpy(&buffer[byte_cnt], packet->seqno		, sizeof(packet->seqno		 ) );   byte_cnt += sizeof(packet->seqno		);  //buffer += sizeof(packet->seqno		);
//
//    /* Writbuffer, e out the Address */                                                                                 //
//	memcpy(&buffer[byte_cnt], packet->dst_address.b0, sizeof(packet->dst_address.b0)); byte_cnt += sizeof(packet->dst_address.b0); //buffer += sizeof(packet->dst_address.b0);
//	memcpy(&buffer[byte_cnt], packet->dst_address.b1, sizeof(packet->dst_address.b1)); byte_cnt += sizeof(packet->dst_address.b1); //buffer += sizeof(packet->dst_address.b1);
//	memcpy(&buffer[byte_cnt], packet->dst_address.b2, sizeof(packet->dst_address.b2)); byte_cnt += sizeof(packet->dst_address.b2); //buffer += sizeof(packet->dst_address.b2);
//	memcpy(&buffer[byte_cnt], packet->dst_address.b3, sizeof(packet->dst_address.b3)); byte_cnt += sizeof(packet->dst_address.b3); //buffer += sizeof(packet->dst_address.b3);
//	memcpy(&buffer[byte_cnt], packet->dst_address.b4, sizeof(packet->dst_address.b4)); byte_cnt += sizeof(packet->dst_address.b4); //buffer += sizeof(packet->dst_address.b4);
//	memcpy(&buffer[byte_cnt], packet->dst_address.b5, sizeof(packet->dst_address.b5)); byte_cnt += sizeof(packet->dst_address.b5); //buffer += sizeof(packet->dst_address.b5);
//	memcpy(&buffer[byte_cnt], packet->dst_address.b6, sizeof(packet->dst_address.b6)); byte_cnt += sizeof(packet->dst_address.b6); //buffer += sizeof(packet->dst_address.b6);
//	memcpy(&buffer[byte_cnt], packet->dst_address.b7, sizeof(packet->dst_address.b7)); byte_cnt += sizeof(packet->dst_address.b7); //buffer += sizeof(packet->dst_address.b7);
//
//	memcpy(&buffer[byte_cnt], packet->tx_opts, sizeof(packet->tx_opts)); byte_cnt += sizeof(packet->tx_opts);
//	for(std::vector<uint8_t>::iterator it = packet->payload.begin(); it != packet->payload.end(); ++it)
//	{
//		//os << *it;
//        memcpy(&buffer[byte_cnt], *it, sizeof(*it));
//        byte_cnt += sizeof(*it);
//	}
//	//os<<packet.checksum;
//    memcpy(&buffer[byte_cnt], packet->checksum, sizeof(packet->checksum));
//    byte_cnt += sizeof(packet->checksum);
//
//	return byte_cnt;
//}
uint16_t Tx64Packet::packet_buf()
{
	//uint64_t temp = packet.get_Address();
	uint16_t byte_cnt;

	memcpy(&tx_buffer[byte_cnt], &sf			, sizeof(sf			    ) ) ; byte_cnt += sizeof(sf			  );  //buffer += sizeof(packet->sf			);
	memcpy(&tx_buffer[byte_cnt], &length		, sizeof(length		    ) ) ; byte_cnt += sizeof(length		  );  //buffer += sizeof(packet->length		); 
	memcpy(&tx_buffer[byte_cnt], &API_frame_id  , sizeof(API_frame_id   ) ) ; byte_cnt += sizeof(API_frame_id );  //buffer += sizeof(packet->API_frame_id);  
	memcpy(&tx_buffer[byte_cnt], &seqno         , sizeof(seqno		    ) ) ; byte_cnt += sizeof(seqno		  );  //buffer += sizeof(packet->seqno		); 

	/* Writbuffer, e out the Address */                                                                                 //
	memcpy(&tx_buffer[byte_cnt], &dst_address.b0, sizeof(dst_address.b0))   ; byte_cnt += sizeof(dst_address.b0); //buffer += sizeof(packet->dst_address.b0);
	memcpy(&tx_buffer[byte_cnt], &dst_address.b1, sizeof(dst_address.b1))   ; byte_cnt += sizeof(dst_address.b1); //buffer += sizeof(packet->dst_address.b1);
	memcpy(&tx_buffer[byte_cnt], &dst_address.b2, sizeof(dst_address.b2))   ; byte_cnt += sizeof(dst_address.b2); //buffer += sizeof(packet->dst_address.b2);
	memcpy(&tx_buffer[byte_cnt], &dst_address.b3, sizeof(dst_address.b3))   ; byte_cnt += sizeof(dst_address.b3); //buffer += sizeof(packet->dst_address.b3);
	memcpy(&tx_buffer[byte_cnt], &dst_address.b4, sizeof(dst_address.b4))   ; byte_cnt += sizeof(dst_address.b4); //buffer += sizeof(packet->dst_address.b4);
	memcpy(&tx_buffer[byte_cnt], &dst_address.b5, sizeof(dst_address.b5))   ; byte_cnt += sizeof(dst_address.b5); //buffer += sizeof(packet->dst_address.b5);
	memcpy(&tx_buffer[byte_cnt], &dst_address.b6, sizeof(dst_address.b6))   ; byte_cnt += sizeof(dst_address.b6); //buffer += sizeof(packet->dst_address.b6);
	memcpy(&tx_buffer[byte_cnt], &dst_address.b7, sizeof(dst_address.b7))   ; byte_cnt += sizeof(dst_address.b7); //buffer += sizeof(packet->dst_address.b7);

	memcpy(&tx_buffer[byte_cnt], &tx_opts, sizeof(tx_opts)); byte_cnt += sizeof(tx_opts);
	for(std::vector<uint8_t>::iterator it = payload.begin(); it != payload.end(); ++it)
	{
		//os << *it;
        //uint8_t temp = *it;
		memcpy(&tx_buffer[byte_cnt], &(*it), sizeof(*it));
		byte_cnt += sizeof(*it);
	}
	//os<<packet.checksum;
	memcpy(&tx_buffer[byte_cnt], &checksum, sizeof(checksum));
	byte_cnt += sizeof(checksum);

	return byte_cnt;
}

/**
 * Just a wrapper function
 */
void Tx64Packet::push_back(uint8_t byteMe) {

	payload.push_back(byteMe);
}
