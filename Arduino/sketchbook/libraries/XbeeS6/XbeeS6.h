#ifndef XBEES6_H__
#define XBEES6_H__
#include "Arduino.h"
//#include <StandardCplusplus.h>
#include <stdint.h>
//#include <ostream>
//#include <general_debug.h>
#include <uVector.h>
#include <string.h>

#define TX_BUFFER_SIZE 512

/* Separate the tx_buffer so we do not need to handle the streaming functions in the class*/
//static uint8_t tx_buffer[TX_BUFFER_SIZE];
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

const Address64_t broadcastAddress = {0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF };
/**
 * Todo Create a deconstructor?
 */
class Tx64Packet
{
	private:
		uint8_t _sf;
		uint16_t _length; //!< Number of bytes between length and Checksum
		uint8_t _API_frame_id;
		uint8_t _seqno;
		Address64_t _dst_address;
		uint8_t _tx_opts;
		uVector _payload;
		uint8_t _checksum;

	public:

		/**
		 * Constructors
		 */

		Tx64Packet();
		Tx64Packet(Address64_t *dst_address);
		Tx64Packet(uint8_t seqno);
		Tx64Packet(uint8_t seqno, Address64_t *dst_address);
		Tx64Packet(const Tx64Packet& );
		int calc_chkSum();
		int set_Address(uint64_t new_address);
		uint64_t get_Address();
//		unsigned long int get_Address();

		void push_back(uint8_t byteMe);
//		uint16_t operator<<( uint8_t* buffer, const Tx64Packet* packet);
		uint16_t prepare2send();
		uint16_t packet_buf() const;

	uint8_t getApiFrameId() const ;

	void setApiFrameId(uint8_t apiFrameId);

	uint8_t getChecksum() const ;



	uint16_t getLength() const ;

	void setLength(uint16_t length) ;

	uint8_t getSeqno() const ;

	void setSeqno(uint8_t seqno) ;

	uint8_t getSf() const ;

	void setSf(uint8_t sf) ;

	uint8_t getTxOpts() const;


	void setTxOpts(uint8_t txOpts) ;
};


#endif /* XBEES6_H__ */
