#include "XbeeS6.h"
#define BYTE_MASK(inVal, offset) (uint8_t)((inVal >> offset) & 0xFF)

int the_endianness;

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



Tx64Packet::ostream& operator<<(ostream& os, const Tx64Packet& packet)
{
	os<<packet.sf;
	os<<packet.length;
	os<<packet.API_frame_id;
	os<<packet.seqno;
	os<<packet.get_Address(); //dst_address;
	os<<packet.tx_opts;
	for(std::vector<uint8_t>::iterator it = packet.payload.begin(); it != packet.payload.end(); ++it)
	{
		os << *it;
	}
	os<<packet.checksum;

	return os;
}
