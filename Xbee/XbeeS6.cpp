#include "XbeeS6.h"
#define BYTE_MASK(inVal, offset) (uint8_t)((inVal >> offset) & 0xFF)

int the_endianness;
enum {
	BIG_ENDIAN,
	LITTLE_ENDIAN
};


/* This will help with debugging */
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

	return sum;
}

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

std::ostream& Tx64Packet:: operator <<(std::ostream& os, const Tx64Packet& packet)
{
	//uint64_t temp = packet.get_Address();
	os<<packet.sf;
	os<<packet.length;
	os<<packet.API_frame_id;
	os<<packet.seqno;

	//os<< temp;//dst_address;
	/* Write out the Address */
	os<<packet.dst_address.b0;
	os<<packet.dst_address.b1;
	os<<packet.dst_address.b2;
	os<<packet.dst_address.b3;
	os<<packet.dst_address.b4;
	os<<packet.dst_address.b5;
	os<<packet.dst_address.b6;
	os<<packet.dst_address.b7;

	os<<packet.tx_opts;
	for(std::vector<uint8_t>::iterator it = packet.payload.begin(); it != packet.payload.end(); ++it)
	{
		os << *it;
	}
	os<<packet.checksum;

	return os;
}

void Tx64Packet::push_back(uint8_t byteMe) {

	payload.push_back(byteMe);
}
