
#define BYTE_MASK(inVal, offset) (uint8_t)((inVal >> offset) & 0xFF)

int the_endianness;
enum
{
	BIG_ENDIAN,
	LITTLE_ENDIAN
};

typedef struct Address64
{
	uint8_t b0;
	uint8_t b1;
	uint8_t b2;
	uint8_t b3;
	uint8_t b4;
	uint8_t b5;
	uint8_t b6;
	uint8_t b7;
} Address64_t;

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

:
class Tx64Packet
{
	private: 
		uint8_t sf;
		uint16_t length;
		uint8_t API_frame_id;
		uint8_t seqno;
		Address64_t dst_address;
		uint8_t tx_opts;
		std::vector<uint8_t> payload;
		uint8_t checksum;

	public:
		int calc_chkSum();
		int set_Address(uint64_t new_address)
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

		uint64_t get_Address()
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


		friend ostream& operator<<(ostream& os, const Tx64Packet& packet)
};

ostream& operator<<(ostream& os, const Tx64Packet& packet)
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
