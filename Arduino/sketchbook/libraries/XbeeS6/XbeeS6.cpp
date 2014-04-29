/**
 * @file XbeeS6.cpp
 * @Author Michael Bartling (michael.bartling15+AMP_RF@gmail.com)
 * @date 2/1/2014
 * @brief Dynamically allocated Xbee S6B packet code
 *
 */

#include "Arduino.h"
#include "XbeeS6.h"

/**
 * Set Debug mode for system validation
 */
#define DEBUG_MODE 0
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


/*static */ uint8_t tx_buffer[TX_BUFFER_SIZE];

/**
 * @name get_buffer
 * @brief get access to packet buffer
 * @return
 */
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
 * @name set_Address
 *
 * @brief Set the destination address
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
 * Checksum = 0xFF - (sum(bytes in api frame) & 0xFF)
 * api frame = bytes between length and checksum
 *
 * Note: Our Teensy was saturating uint8_ts rather than mod 256, so we
 * went with a larger int to hold all of the data before byte masking it.
 *
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

/**
 * Constructors
 */
Tx64Packet::Tx64Packet() {
	_sf = 0x7E; 			//!< Start Frame
	_length = 11;			//!< Initial TX64 Packet length
	_API_frame_id = 0x00;	//!< TX64 Frame ID
	_seqno = 1;				//!< Start at seqno > 0 to get tx status packet
	memcpy(&_dst_address, &broadcastAddress, sizeof(broadcastAddress));
	_tx_opts = 0x01;		//!< Disable Ack
}

Tx64Packet::Tx64Packet(Address64_t* dst_address) {
	_sf = 0x7E;				//!< Start Frame
	_length = 11;			//!< Initial TX64 Packet length
	_API_frame_id = 0x00;	//!< TX64 Frame ID
	_seqno = 1;				//!< Start at seqno > 0 to get tx status packet
	memcpy(&_dst_address, dst_address, sizeof(*dst_address));
	_tx_opts = 0x01;		//!< Disable Ack
}

Tx64Packet::Tx64Packet(uint8_t seqno) {
	_sf = 0x7E;				//!< Start Frame
	_length = 11;			//!< Initial TX64 Packet length
	_API_frame_id = 0x00;	//!< TX64 Frame ID
	_seqno = seqno;			//!< Start at seqno > 0 to get tx status packet
	memcpy(&_dst_address, &broadcastAddress, sizeof(broadcastAddress));
	_tx_opts = 0x01;		//!< Disable Ack
}

Tx64Packet::Tx64Packet(uint8_t seqno, Address64_t* dst_address) {
	_sf = 0x7E;				//!< Start Frame
	//_length = 11;			//!< Initial TX64 Packet length
	_API_frame_id = 0x00;	//!< TX64 Frame ID
	_seqno = seqno;			//!< Start at seqno > 0 to get tx status packet
	memcpy(&_dst_address, dst_address, sizeof(*dst_address));
	_tx_opts = 0x01;		//!< Disable Ack
}

/**
 * @name packet_buf
 * @brief form the packet
 *
 * @note We need to calculate the checksum before adding in the escape character handling
 * escape characters are handled as insert 0x7D before escape character and xor escape character with 0x20
 * Can override this function for the std stream to trick the arduino into forming a packet
 * or we could write this to a global buffer or something.
 *
 * Another Option is to replace is stream with the arduino stream library
 *
 * consider making this function private
 */

uint16_t Tx64Packet::packet_buf() const
{
	/*
	 * Declare Variables
	 */
	uint16_t byte_cnt;
	uint8_t temp_byte;
	/* Need to byteswap the packet length first */
	uint16_t temp;
	temp = (_length >> 8) | (_length << 8);


	/* Not the most efficient way of copying data to packetbuf
	 * Had issues with teensy 3.0 moving this memory so brute forced it*/
	memcpy(&tx_buffer[byte_cnt], &_sf			 , sizeof(_sf			    ) ) ; byte_cnt += sizeof(_sf			  );  //buffer += sizeof(packet->sf			);
	memcpy(&tx_buffer[byte_cnt], &temp			 , sizeof(temp		    ) ) ; byte_cnt += sizeof(_length		  );  //buffer += sizeof(packet->length		);

	/* All Bytes below this technically need to be checked for escape Characters TODO*/
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

	/* Check Data for escape characters*/
	for(int i = 0; i < _payload.size(); i++)
	{
		//		memcpy(&tx_buffer[byte_cnt], &(*it), sizeof(*it));
		//		byte_cnt += sizeof(*it);
		temp_byte = _payload[i];
 		 switch(temp_byte)
		{
		case 0x7E:
			tx_buffer[byte_cnt] = 0x7D;
			byte_cnt++;
			tx_buffer[byte_cnt] = temp_byte ^ 0x20;
			byte_cnt++;
			break;
		case 0x7D:
			tx_buffer[byte_cnt] = 0x7D;
			byte_cnt++;
			tx_buffer[byte_cnt] = temp_byte ^ 0x20;
			byte_cnt++;
			break;
		case 0x11:
			tx_buffer[byte_cnt] = 0x7D;
			byte_cnt++;
			tx_buffer[byte_cnt] = temp_byte ^ 0x20;
			byte_cnt++;
			break;
		case 0x13:
			tx_buffer[byte_cnt] = 0x7D;
			byte_cnt++;
			tx_buffer[byte_cnt] = temp_byte ^ 0x20;
			byte_cnt++;
			break;
		default:
			tx_buffer[byte_cnt] = temp_byte;
			byte_cnt++;
			break;
		}

		//tx_buffer[byte_cnt] = _payload[i];
		//byte_cnt++;
	}
	//os<<packet.checksum;
	memcpy(&tx_buffer[byte_cnt], &_checksum, sizeof(_checksum));
	byte_cnt += sizeof(_checksum);
	PRINTF("%d", byte_cnt);
	return byte_cnt;
}

/**
 * Just a wrapper function
 * lets increment the packet length each time we get a new byte
 */
void Tx64Packet::push_back(uint8_t byteMe) {

	_payload.push_back(byteMe);
	_length += 1;
}
/*
 * Getters and Setters
 */
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
/**
 * @name incSeqno
 * @brief increment the packet sequence number so we can distinguish packets
 *
 * for simplicity, we are ignoring the escape character sequence numbers as well as 0x00.
 * 0x00 does not return a tx_status frame.
 */
void Tx64Packet::incSeqno(){
	switch((uint8_t)(_seqno + 1)){	
		case 0x00:
			_seqno += 2;
			break;
		case 0x7E:
			_seqno += 2;
			break;
		case 0x7D:
			_seqno += 2;
			break;
		case 0x11:
			_seqno += 2;
			break;
		case 0x13:
			_seqno += 2;
			break;
		default:
			_seqno++;
			break;
	}
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

/**
 * @name prepare2send
 * @brief calculate the checksum and prepare the packet buffer
 * @return number of bytes written to the packet buffer
 */
uint16_t Tx64Packet::prepare2send() {


	calc_chkSum();
	return packet_buf();

}

/**
 * reset the uVector payload and packet length
 */
void Tx64Packet::clear_payload() {
	_payload.clear();
	_length = 11;
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
	_length = ((uint16_t) lenH) | ( lenL ) ; //Will this cheat the byteswap just with casting?
}

uint16_t RxPacket::getlength() const {
	return _length;
}

void RxPacket::push_back(uint8_t byteMe) {
	_API_frame.push_back(byteMe);
}
uint8_t RxPacket::getApiFrame(int i)
{
	return _API_frame[i];
}
const uint8_t RxPacket::getApiFrame(int i) const
{
	return _API_frame[i];
}
int  RxPacket::process()
{
	if(_length == 0 ) return -4;
	switch(getApiFrameId())
	{
	case 0x80:
		if( Rx64Parse() > 0){
			_length = _msgQ.size();
			clear_API_frame();
			return 0x80;
		}
		else{
			//This will be a secondary test for debugging
			uint16_t msgQLen;
			msgQLen = _msgQ[1] | (_msgQ[2] << 8);
			if((uint16_t) _msgQ.size() > msgQLen)
			{
				clear();
				return -2; //to signify the fail
			}
			clear(); //We couldnt parse properly so lets clear everything
			return -1;
		}
	default:
		clear();	//Clear the buffers
		return -3; //API frame not supported
	}
}
uint16_t RxPacket::prepare2send() {
	return 0;
}

uint16_t RxPacket::packet_buf() const {
	return 0;
}

uint8_t RxPacket::getApiFrameId() const {
	return _API_frame[0];
}

uint8_t RxPacket::getChecksum() const {
	return _API_frame[ _length + 1];
}

void RxPacket::clear_msgQ() {
	_msgQ.clear();
}

void RxPacket::clear_API_frame() {
	_API_frame.clear();
}

void RxPacket::clear() {
	_length = 0;
	_API_frame.clear();
	_msgQ.clear();
}

//return the number of bytes packed into the Queue (not counting the commands chars)
int RxPacket::Rx64Parse()
{
	int i;
	int msgQLen;
	clear_msgQ();
	_msgQ.push_back(_sf);
	_msgQ.push_back(0xDE); //Dont forget to update this length in the end
	_msgQ.push_back(0xAD); //These two entries will be the length of the msgQ

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

	//Clean up
	//todo Make sure that this is actually a 32 bit int and not a 16 bit int.
	msgQLen = _msgQ.size();
	_msgQ.changeVal(1, (msgQLen >> 24) & 0xFF);
	_msgQ.changeVal(2, (msgQLen >> 16) & 0xFF);
	return i;
}


void RxPacket::resize(int newsize) {
	_API_frame.reserve(newsize);
}
