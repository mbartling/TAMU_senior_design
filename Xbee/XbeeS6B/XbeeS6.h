#ifndef XBEES6_H__
#define XBEES6_H__
#include "Arduino.h"
#include <stdint.h>
#include <ostream>
#include "../Debug/general_debug.h"
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

/**
 * Todo Create a deconstructor?
 */
class Tx64Packet
{
	private:
		uint8_t _sf;
		uint16_t _length;
		uint8_t _API_frame_id;
		uint8_t _seqno;
		Address64_t _dst_address;
		uint8_t _tx_opts;
		std::vector<uint8_t> _payload;
		uint8_t _checksum;

	public:
		int calc_chkSum();
		int set_Address(uint64_t new_address);
		uint64_t get_Address();
//		unsigned long int get_Address();

		void push_back(uint8_t byteMe);
//		uint16_t operator<<( uint8_t* buffer, const Tx64Packet* packet);
		uint16_t packet_buf();
};


#endif /* XBEES6_H__ */
