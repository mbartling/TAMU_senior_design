#include "Arduino.h"
#include "XbeeS6.h"
//#include <iterator>
#define DEBUG_MODE 1
#if DEBUG_MODE
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else /* Not DEBUG_MODE */
#define PRINTF(...) /* Dummy */
#endif /* DEBUG_MODE */

#define BYTE_MASK(inVal, offset) (uint8_t)((inVal >> offset) & 0xFF)

#define PROCESS_COMMANDS() /* DUMMY */

int the_endianness;

/**
 * This should be fine.
 */
enum {
	BIG_ENDIAN,
	LITTLE_ENDIAN
};

//tx_buffer[TX_BUFFER_SIZE];
uint8_t tx_buffer[TX_BUFFER_SIZE];

uint8_t * get_buffer()
{
	return &tx_buffer[0];
}
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
	// Will write little endian but need Big endian
	//	_dst_address.b0 = BYTE_MASK(new_address, 0);
	//	_dst_address.b1 = BYTE_MASK(new_address, 8);
	//	_dst_address.b2 = BYTE_MASK(new_address, 16);
	//	_dst_address.b3 = BYTE_MASK(new_address, 24);
	//	_dst_address.b4 = BYTE_MASK(new_address, 32);
	//	_dst_address.b5 = BYTE_MASK(new_address, 40);
	//	_dst_address.b6 = BYTE_MASK(new_address, 48);
	//	_dst_address.b7 = BYTE_MASK(new_address, 56);

	_dst_address.b0 = BYTE_MASK(new_address, 56);
	_dst_address.b1 = BYTE_MASK(new_address, 48);
	_dst_address.b2 = BYTE_MASK(new_address, 40);
	_dst_address.b3 = BYTE_MASK(new_address, 32);
	_dst_address.b4 = BYTE_MASK(new_address, 24);
	_dst_address.b5 = BYTE_MASK(new_address, 16);
	_dst_address.b6 = BYTE_MASK(new_address, 8);
	_dst_address.b7 = BYTE_MASK(new_address, 0);

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

	//	sum += _sf;
	//	sum += (_length & 0xFF) + ((_length >> 8) & 0xFF);
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

uint16_t Tx64Packet::packet_buf() const
{
	//uint64_t temp = packet.get_Address();
	uint16_t byte_cnt;

	/* Need to byteswap the packet length first */
	uint16_t temp;
	temp = (_length >> 8) | (_length << 8);



	memcpy(&tx_buffer[byte_cnt], &_sf			 , sizeof(_sf			    ) ) ; byte_cnt += sizeof(_sf			  );  //buffer += sizeof(packet->sf			);
	memcpy(&tx_buffer[byte_cnt], &temp			 , sizeof(temp		    ) ) ; byte_cnt += sizeof(_length		  );  //buffer += sizeof(packet->length		);
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
	PRINTF("%d", byte_cnt);
	return byte_cnt;
}

/**
 * Just a wrapper function
 */
void Tx64Packet::push_back(uint8_t byteMe) {

	_payload.push_back(byteMe);
	_length += 1;
}

uint8_t Tx64Packet::getApiFrameId() const {
	return _API_frame_id;
}

void Tx64Packet::setApiFrameId(uint8_t apiFrameId) {
	_API_frame_id = apiFrameId;
}

uint8_t Tx64Packet::getChecksum() const {
	return _checksum;
}


uint16_t Tx64Packet::Tx64Packet::getLength() const {
	return _length;
}

void Tx64Packet::setLength(uint16_t length) {
	_length = length;
}

uint8_t Tx64Packet::getSeqno() const {
	return _seqno;
}

void Tx64Packet::setSeqno(uint8_t seqno) {
	_seqno = seqno;
}

uint8_t Tx64Packet::getSf() const {
	return _sf;
}

void Tx64Packet::setSf(uint8_t sf) {
	_sf = sf;
}

uint8_t Tx64Packet::getTxOpts() const {
	return _tx_opts;
}

uint16_t Tx64Packet::prepare2send() {

	calc_chkSum();
	return packet_buf();

}

void Tx64Packet::clear_payload() {
	_payload.clear();
}

void Tx64Packet::setTxOpts(uint8_t txOpts) {
	_tx_opts = txOpts;
}


/*===============================================*/
/* Begin RX Code */
/*===============================================*/


RxPacket::RxPacket() {
	_sf = 0x7E;
	_length = 0;
}

void RxPacket::set_length(uint16_t length) {

	_length = length;
}

void RxPacket::set_length(uint8_t lenH, uint8_t lenL) {
	_length = ((uint16_t) lenL) | ( lenH ) ; //Will this cheat the byteswap just with casting?
}

uint16_t RxPacket::getlength() const {
	return _length;
}

void RxPacket::push_back(uint8_t byteMe) {
	_API_frame.push_back(byteMe);
}

int  RxPacket::process()
{
	switch(getApiFrameId())
	{
	case 0x80:
		return Rx64Parse();
	default:
		clear();	//Clear the buffers
		return -1; //API frame not supported
	}
}
uint16_t RxPacket::prepare2send() {
}

uint16_t RxPacket::packet_buf() const {
}

uint8_t RxPacket::getApiFrameId() const {
	return _msgQ[0];
}

uint8_t RxPacket::getChecksum() const {
	return _msgQ[ _length + 1];
}

void RxPacket::clear_msgQ() {
	_msgQ.clear();
}

void RxPacket::clear_API_frame() {
	_API_frame.clear();
}

void RxPacket::clear() {
	_API_frame.clear();
	_msgQ.clear();
}

//return the number of bytes packed into the Queue (not counting the commands chars)
int RxPacket::Rx64Parse()
{
	int i;
	clear_msgQ();
	_msgQ.push_back(_sf);
	_msgQ.push_back(0xDE);
	_msgQ.push_back(0xAD);
	/* Put the address in the msg Q with command A*/
	_msgQ.push_back(' ');
	_msgQ.push_back('A');

	//offset 8 + 1
	for(i = 1; i < 9; i++)
	{
		_msgQ.push_back(_API_frame[i]);
	}

	/* Write the RSSI information */
	_msgQ.push_back(' ');
	_msgQ.push_back('R');
//	i++; //want to increment i
	_msgQ.push_back(_API_frame[i]);

	//skip the options byte
	i += 2;

	/* Push the RF Data*/
	_msgQ.push_back(' ');
	_msgQ.push_back('D');

	while(i <= _length)
	{
		/**
		 * NOTE: This is where we would put the process commands function
		 */
		if(i > _length)
		{
			break;
		}
		PROCESS_COMMANDS();

		_msgQ.push_back( _API_frame[i] );
		i++;
	}

	// Put a newline to make it easier for us to parse in python
	_msgQ.push_back('\n');

	return i;
}

