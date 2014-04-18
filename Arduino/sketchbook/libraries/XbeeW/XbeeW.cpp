#include "Arduino.h"
#include "XbeeW.h"

Address64::Address64()
{
	memset(_address, 0, sizeof(_address));
}

Address64::Address64(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
	memset(_address, 0, sizeof(_address));
	_address[0] = first_octet;
	_address[1] = second_octet;
	_address[2] = third_octet;
	_address[3] = fourth_octet;
}

Address64::Address64(uint32_t address)
{
	memset(_address, 0, sizeof(_address));
	memcpy(_address, &address, sizeof(_address));
}

Address64::Address64(const uint8_t *address)
{
	memset(_address, 0, sizeof(_address));
	memcpy(_address, address, sizeof(_address));
}

Address64& Address64::operator=(const uint8_t *address)
{
	memcpy(_address, address, sizeof(_address));
	return *this;
}

Address64& Address64::operator=(uint32_t address)
{
	memcpy(_address, (const uint8_t *)&address, sizeof(_address));
	return *this;
}

bool Address64::operator==(const uint8_t* addr)
{
	return memcmp(addr, _address, sizeof(_address)) == 0;
}

size_t Address64::printTo(Print& p) const
{
	size_t n = 0;
	for (int i =0; i < 4; i++)
	{
		n += p.print( 0 , BYTE);
	}
	for (int i =4; i < 8; i++)
	{
		n += p.print(_address[i], BYTE);
	}
	return n;
}

Api_frame::Api_frame()
{
	//memset(_fid, 0, sizeof(_fid));
	//memset(_seqno, 1, sizeof(_seqno));
	_fid = 0;
	_seqno = 1;
	memset(_payload,0,sizeof(_payload));
}


Api_frame::Api_frame(uint8_t fid)
{
	_fid = fid;
	_seqno = 1;
	memset(_payload,0,sizeof(_payload));
}

void Api_frame::set_fid(uint8_t fid)
{
	_fid = fid;
}
Api_frame& Api_frame::operator++(){ 
	++_seqno; 
	if (_seqno == 0)
		_seqno++;
	return *this; 
}
void Api_frame::clear()
{
	memset(_payload,0,sizeof(_payload));
}
size_t Api_frame::length() const
{
	size_t n = 2;
	int i;
	while(i < 63)
	{
		if(_payload[i] == '\r' && _payload[i+1] == '\n') return n;

		n += 1;
		i++;
	}
	return n;
}
size_t Api_frame::printTo(Print& p) const
{
	size_t n = 0;
	int i;
	n += p.print(_fid, BYTE);
	n += p.print(_seqno, BYTE);

	while(i < 63)
	{
		if(_payload[i] == '\r' && _payload[i+1] == '\n') return n;

		n += p.print(_payload[i], BYTE);
		i++;
	}
	return n;
}

Remote_node::Remote_node(uint8_t address)
{
	_cmd_opts = 0;
	_chksum = 0;
	_dst_address = (uint32_t)(0xC0A80100 | address);
}
Remote_node::Remote_node(uint8_t const *address)
{
	_cmd_opts = 0;
	_chksum = 0;
	uint32_t temp;
	memcpy(&temp, address, sizeof(temp));
	_dst_address = temp;
}
Remote_node::Remote_node(Address64 address)
{
	_cmd_opts = 0;
	_chksum = 0;
	_dst_address = address;
}

uint8_t Remote_node::checksum() const
{
	uint16_t chksum = 0;
	int i;

	chksum += _frame.get_fid();
	chksum += _frame.get_seqno();
	chksum += _cmd_opts;
	for (i = 0; i < 8; i++)
	{
		chksum += _dst_address[i]; //_dst_address._address[i];
	}
	i = 0;
	while(i < 63)
	{
		if(_frame[i] == '\r' && _frame[i+1] == '\n') break;
		chksum += _frame[i];
	}

	chksum = 0xFF - (chksum & 0xFF);
	//_chksum = chksum;
	return chksum;

}

size_t Remote_node::printTo(Print& p) const
{
	size_t n = 0;
	int i;
	uint16_t length = 9;
	uint16_t lengthS;
	uint8_t chksum = checksum();
	length += _frame.length();
	lengthS = ((length >> 8) | (length << 8));
	n += p.print(0x7E, BYTE);
	n += p.print(lengthS, BYTE);
	n += p.print(_frame.get_fid(), BYTE);
	n += p.print(_frame.get_seqno(), BYTE);

	n += p.print(_cmd_opts, BYTE);
	for (i = 0; i < 8; i++)
	{
		n  += p.print(_dst_address[i], BYTE); //_dst_address._address[i];
	}
	length = length - 11;
	for( i = 0; i < length; i++)
	{
		switch(_frame[i])
		{
			case 0x7E:
				n += p.print(0x7D ,BYTE);
				n += p.print(_frame[i] ^ 0x20, BYTE);
				break;
			case 0x7D:
				n += p.print(0x7D ,BYTE);
				n += p.print(_frame[i], BYTE);
				break;
			case 0x11:
				n += p.print(0x7D ,BYTE);
				n += p.print(_frame[i] ^ 0x20, BYTE);
				break;
			case 0x13:
				n += p.print(0x7D ,BYTE);
				n += p.print(_frame[i] ^ 0x20, BYTE);
				break;
			default:
				n += p.print(_frame[i], BYTE);
				break;

		}
		n += p.print(chksum, BYTE);
		return n;
	}
}
