#include "Arduino.h"
#include "XbeeS6.h"
//#include <iterator>

#if DEBUG_MODE
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else /* Not DEBUG_MODE */
#define PRINTF(...) /* Dummy */
#endif /* DEBUG_MODE */

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
	_dst_address.b0 = BYTE_MASK(new_address, 0);
	_dst_address.b1 = BYTE_MASK(new_address, 8);
	_dst_address.b2 = BYTE_MASK(new_address, 16);
	_dst_address.b3 = BYTE_MASK(new_address, 24);
	_dst_address.b4 = BYTE_MASK(new_address, 32);
	_dst_address.b5 = BYTE_MASK(new_address, 40);
	_dst_address.b6 = BYTE_MASK(new_address, 48);
	_dst_address.b7 = BYTE_MASK(new_address, 56);

	return _dst_address.b0 + _dst_address.b1 + _dst_address.b2 + _dst_address.b3 + _dst_address.b4 + _dst_address.b5 + _dst_address.b6 + _dst_address.b7;
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

	sum += _sf;
	sum += (_length & 0xFF) + ((_length >> 8) & 0xFF);
	sum += _API_frame_id;
	sum += _seqno;
	sum += _dst_address.b0;
	sum += _dst_address.b1;
	sum += _dst_address.b2;		
	sum += _dst_address.b3;		
	sum += _dst_address.b4;		
	sum += _dst_address.b5;		
	sum += _dst_address.b6;		
	sum += _dst_address.b7;
	sum += _tx_opts;

	for(int i = 0; i < _payload.size(); i++)
	{
		sum += _payload[i];
	}

	sum = 0xFF - sum; // Final Part in checksum calculation
	_checksum = sum;
	return sum;
}

/**
 * Get the 64 bit address as a uint64_t
 */
uint64_t Tx64Packet::get_Address()
{
	uint64_t temp;
	if(sizeof(temp) != sizeof(_dst_address))
	{
		PRINTF("Size of Address do not match!\n");
		return -1;
	}

	memcpy(&temp, &_dst_address, sizeof(temp));
	return temp;
}

Tx64Packet::Tx64Packet() {
	_sf = 0x7E;
	_length = 11;
	_API_frame_id = 0x00;
	_seqno = 1;
	memcpy(&_dst_address, &broadcastAddress, sizeof(broadcastAddress));
	_tx_opts = 0x00;
}

Tx64Packet::Tx64Packet(Address64_t* dst_address) {
	_sf = 0x7E;
	_length = 11;
	_API_frame_id = 0x00;
	_seqno = 1;
	memcpy(&_dst_address, dst_address, sizeof(*dst_address));
	_tx_opts = 0x00;
}

Tx64Packet::Tx64Packet(uint8_t seqno) {
	_sf = 0x7E;
	_length = 11;
	_API_frame_id = 0x00;
	_seqno = seqno;
	memcpy(&_dst_address, &broadcastAddress, sizeof(broadcastAddress));
	_tx_opts = 0x00;
}

Tx64Packet::Tx64Packet(uint8_t seqno, Address64_t* dst_address) {
	_sf = 0x7E;
	_length = 11;
	_API_frame_id = 0x00;
	_seqno = seqno;
	memcpy(&_dst_address, dst_address, sizeof(*dst_address));
	_tx_opts = 0x00;
}

/**
 * Can override this function for the std stream to trick the arduino into forming a packet
 * or we could write this to a global buffer or something.
 *
 * Another Option is to replace is stream with the arduino stream library
 */

uint16_t Tx64Packet::packet_buf()
{
	//uint64_t temp = packet.get_Address();
	uint16_t byte_cnt;

	calc_chkSum();

	memcpy(&tx_buffer[byte_cnt], &_sf			, sizeof(_sf			    ) ) ; byte_cnt += sizeof(_sf			  );  //buffer += sizeof(packet->sf			);
	memcpy(&tx_buffer[byte_cnt], &_length		, sizeof(_length		    ) ) ; byte_cnt += sizeof(_length		  );  //buffer += sizeof(packet->length		); 
	memcpy(&tx_buffer[byte_cnt], &_API_frame_id  , sizeof(_API_frame_id   ) ) ; byte_cnt += sizeof(_API_frame_id );  //buffer += sizeof(packet->API_frame_id);  
	memcpy(&tx_buffer[byte_cnt], &_seqno         , sizeof(_seqno		    ) ) ; byte_cnt += sizeof(_seqno		  );  //buffer += sizeof(packet->seqno		); 

	/* Writbuffer, e out the Address */                                                                                 //
	memcpy(&tx_buffer[byte_cnt], &_dst_address.b0, sizeof(_dst_address.b0))   ; byte_cnt += sizeof(_dst_address.b0); //buffer += sizeof(packet->dst_address.b0);
	memcpy(&tx_buffer[byte_cnt], &_dst_address.b1, sizeof(_dst_address.b1))   ; byte_cnt += sizeof(_dst_address.b1); //buffer += sizeof(packet->dst_address.b1);
	memcpy(&tx_buffer[byte_cnt], &_dst_address.b2, sizeof(_dst_address.b2))   ; byte_cnt += sizeof(_dst_address.b2); //buffer += sizeof(packet->dst_address.b2);
	memcpy(&tx_buffer[byte_cnt], &_dst_address.b3, sizeof(_dst_address.b3))   ; byte_cnt += sizeof(_dst_address.b3); //buffer += sizeof(packet->dst_address.b3);
	memcpy(&tx_buffer[byte_cnt], &_dst_address.b4, sizeof(_dst_address.b4))   ; byte_cnt += sizeof(_dst_address.b4); //buffer += sizeof(packet->dst_address.b4);
	memcpy(&tx_buffer[byte_cnt], &_dst_address.b5, sizeof(_dst_address.b5))   ; byte_cnt += sizeof(_dst_address.b5); //buffer += sizeof(packet->dst_address.b5);
	memcpy(&tx_buffer[byte_cnt], &_dst_address.b6, sizeof(_dst_address.b6))   ; byte_cnt += sizeof(_dst_address.b6); //buffer += sizeof(packet->dst_address.b6);
	memcpy(&tx_buffer[byte_cnt], &_dst_address.b7, sizeof(_dst_address.b7))   ; byte_cnt += sizeof(_dst_address.b7); //buffer += sizeof(packet->dst_address.b7);

	memcpy(&tx_buffer[byte_cnt], &_tx_opts, sizeof(_tx_opts)); byte_cnt += sizeof(_tx_opts);
	for(int i = 0; i < _payload.size(); i++)
	{
//		memcpy(&tx_buffer[byte_cnt], &(*it), sizeof(*it));
//		byte_cnt += sizeof(*it);
		tx_buffer[byte_cnt] = _payload[i];
		byte_cnt++;
	}
	//os<<packet.checksum;
	memcpy(&tx_buffer[byte_cnt], &_checksum, sizeof(_checksum));
	byte_cnt += sizeof(_checksum);

	return byte_cnt;
}

/**
 * Just a wrapper function
 */
void Tx64Packet::push_back(uint8_t byteMe) {

	_payload.push_back(byteMe);
	_length += 1;
}
