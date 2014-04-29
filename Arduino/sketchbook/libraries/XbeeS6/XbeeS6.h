#ifndef XBEES6_H__
#define XBEES6_H__
#include "Arduino.h"
//#include <StandardCplusplus.h>
#include <stdint.h>

//#include <general_debug.h>
#include <uVector.h>
#include <string.h>

// Change this size to fit your code: we used a size of 64 bytes
#define TX_BUFFER_SIZE 512

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

uint8_t * get_buffer();

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
	void clear_payload();
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
	void incSeqno();

	uint8_t getSf() const ;

	void setSf(uint8_t sf) ;

	uint8_t getTxOpts() const;


	void setTxOpts(uint8_t txOpts) ;
};

//Rx packets will be generic
class RxPacket
{
private:
	uint8_t _sf;
	uint16_t _length; //!< Number of bytes between length and Checksum
	//		uint8_t _API_frame_id;
	uVector _API_frame;


//protected:
	uVector _msgQ;

public:

	/**
	 * Constructors
	 */
	RxPacket();

	RxPacket(const Tx64Packet& ); //Copy constructor

	uint8_t operator[](int n){ return _msgQ[n];}
	const uint8_t operator[](int n) const {return _msgQ[n]; }

	void set_length(uint16_t length);
	void set_length(uint8_t lenH, uint8_t lenL);
	uint16_t getlength() const ;


	void push_back(uint8_t byteMe); //Fill the receive buffer
	uint16_t prepare2send(); //To NUC
	uint16_t packet_buf() const;

	int process();
	uint8_t getApiFrameId() const ; //API_frame[0]
	uint8_t getApiFrame(int i) ;
	const uint8_t getApiFrame(int i) const;

	uint8_t getChecksum() const ; //API_frame[length];
	void resize(int newsize);
	void clear_msgQ();
	void clear_API_frame();
	void clear(); //clear all buffers

	/**
	 * Parsing Methods
	 */
	int Rx64Parse();
	int ATResonseParse();

};


/**
 * Remote AT Commands
 * =================================================== */
class RemoteAT
{
private:

	uint16_t _length; //!< Number of bytes between length and Checksum
	uint8_t _API_frame_id;
	uint8_t _seqno;
	Address64_t _dst_address;
	uint8_t _tx_opts;
	uVector _payload; //This will be the AT command and Parameter Value part of the API Packet
	uint8_t _checksum;
	uVector _sendBuff;

public:

	/**
	 * Constructors
	 */

	RemoteAT();
	RemoteAT(Address64_t *dst_address);
	RemoteAT(const Tx64Packet& );

	uint8_t operator[](int n){ return _sendBuff[n];}
	const uint8_t operator[](int n) const {return _sendBuff[n]; }

	int calc_chkSum();
	int set_Address(uint64_t new_address);
	uint64_t get_Address() const;
	//		unsigned long int get_Address();

	void push_back(uint8_t byteMe);
	void clear_payload();
	//		uint16_t operator<<( uint8_t* buffer, const Tx64Packet* packet);
	uint16_t prepare2send();



	uint8_t getChecksum() const ;


	void calcLength();
	uint16_t getLength() const ;

	//void setLength(uint16_t length) ;

	void clear_sendBuff() ;
	void clear();
};

#endif /* XBEES6_H__ */
