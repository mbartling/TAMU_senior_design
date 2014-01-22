#ifndef XBEES6_H__
#define XBEES6_H__
#include <stdint.h>
#include <ostream>
#include "general_debug.h"
#include <vector>
#include <string.h>
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
		int set_Address(uint64_t new_address);
		uint64_t get_Address();

		friend ostream& operator<<(ostream& os, const Tx64Packet& packet);
};


#endif /* XBEES6_H__ */
