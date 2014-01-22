#ifndef XBEES6_H__
#define XBEES6_H__
#include <stdint.h>
#include <ostream>
#include "general_debug.h"
#include <vector>
#include <string.h>

#define TX_BUFFER_SIZE 512

/* Separate the tx_buffer so we do not need to handle the streaming functions in the class*/
uint8_t tx_buffer[TX_BUFFER_SIZE];

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
//		unsigned long int get_Address();

		void push_back(uint8_t byteMe);
		std::ostream& operator<<(std::ostream& os, const Tx64Packet& packet);
};


#endif /* XBEES6_H__ */
