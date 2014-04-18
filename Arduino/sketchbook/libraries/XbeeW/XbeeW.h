#ifndef XbeeW_h
#define XbeeW_h

#include <Printable.h>

class Address64 : public Printable {
private:
	uint8_t _address[8];
	// Access the raw byte array containing the address.  Because this returns a pointer
    // to the internal structure rather than a copy of the address this function should only
    // be used when you know that the usage of the returned uint8_t* will be transient and not
    // stored.
	uint8_t* raw_address(){ return _address; }

public:
	// For now will fake the 64 bit
	Address64();
	Address64(uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4);
	Address64(uint32_t address);
	//Address64(const uint32_t * address);
	Address64(const uint8_t * address);

	// Overloaded cast operator to allow IPAddress objects to be used where a pointer
    // to a four-byte uint8_t array is expected
    operator uint32_t () { return _address[0] | (_address[1] << 8)

	| (_address[2] << 16) | (_address[3] << 24); }
    bool operator==(const Address64& addr) { return _address[0] == addr._address[0]
	&& _address[1] == addr._address[1]
	&& _address[2] == addr._address[2]
	&& _address[3] == addr._address[3]; }
    bool operator==(const uint8_t* addr);

    // Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return _address[index]; };
    uint8_t& operator[](int index) { return _address[index]; };

    // Overloaded copy operators to allow initialization of IPAddress objects from other types
    Address64& operator=(const uint8_t *address);
    Address64& operator=(uint32_t address);

    //friend class Api_frame;
    friend class Remote_node;
    virtual size_t printTo(Print& p) const;
};
/*
class Api_frame: public Printable{
private:
	uint8_t _fid;
	uint8_t _seqno;
	uint8_t _payload[64]; //Max Size Not counting address
	uint8_t * raw_payload(){return _payload; }

public:
	Api_frame();
	Api_frame(uint8_t seqno);

	void set_fid(uint8_t fid);
// Overloaded index operator to allow getting and setting individual octets of the address
     uint8_t operator[](int index) const { return _payload[index]; };
     uint8_t& operator[](int index) { return _payload[index]; };
    
	Api_frame& operator++();
    Api_frame operator++(int)
    {
    	Api_frame tmp(*this);
    	operator++();
    	return tmp;
    }
    void clear();
    virtual size_t length() const;
    virtual size_t printTo(Print& p) const;
    //friend class Remote_node;
};


class Remote_node: public Api_frame{
private:
	Address64 	_dst_address;
	uint8_t 	_cmd_opts;
	uint8_t		_chksum;
public:
	Remote_node():Api_frame(){
		_cmd_opts 	= 0;
		_chksum 	= 0;
	}
	//Remote_node(uint8_t fid):Api_frame(fid){_cmd_opts = 0;}
	Remote_node(uint8_t address);
	Remote_node(uint8_t const *address);
	Remote_node(Address64 address);
	
	void set_fid(uint8_t fid) { Api_frame::set_fid(fid);};
// Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return Api_frame::operator[](index); };
    uint8_t& operator[](int index) { return Api_frame::operator[](index); };
    
    //void clear() { Api_frame::clear(); }
    //virtual size_t length() const {return Api_frame::length() + 7; }
	void 	set_cmd(uint8_t cmd ){ _cmd_opts = cmd; }
	uint8_t checksum() const;
    virtual size_t printTo(Print& p) const;
};
*/


class Api_frame: public Printable{
private:
	uint8_t _fid;
	uint8_t _seqno;
	uint8_t _payload[64]; //Max Size Not counting address
	uint8_t * raw_payload(){return _payload; }

public:
	Api_frame();
	Api_frame(uint8_t seqno);

	void set_fid(uint8_t fid);
	uint8_t get_fid() {return _fid;};
	uint8_t get_fid() const {return _fid;};
	uint8_t get_seqno() {return _seqno;};
	uint8_t get_seqno() const {return _seqno;};
// Overloaded index operator to allow getting and setting individual octets of the address
     uint8_t operator[](int index) const { return _payload[index]; };
     uint8_t& operator[](int index) { return _payload[index]; };
    
	Api_frame& operator++();
    Api_frame operator++(int)
    {
    	Api_frame tmp(*this);
    	operator++();
    	return tmp;
    }
    void clear();
    virtual size_t length() const;
    virtual size_t printTo(Print& p) const;
    //friend class Remote_node;
};


class Remote_node: public Printable{
private:
	Address64 	_dst_address;
	uint8_t 	_cmd_opts;
	uint8_t		_chksum;
	Api_frame	_frame;

public:
	Remote_node(){
		_cmd_opts 	= 0;
		_chksum 	= 0;
	}
	//Remote_node(uint8_t fid):Api_frame(fid){_cmd_opts = 0;}
	Remote_node(uint8_t address);
	Remote_node(uint8_t const *address);
	Remote_node(Address64 address);
	
	void set_fid(uint8_t fid) { _frame.set_fid(fid);};
// Overloaded index operator to allow getting and setting individual octets of the address
    uint8_t operator[](int index) const { return _frame[index]; };
    uint8_t& operator[](int index) { return _frame[index]; };
    
    void clear() { _frame.clear(); }
    virtual size_t length() const {return _frame.length() + 7; }
	void 	set_cmd(uint8_t cmd ){ _cmd_opts = cmd; }
	uint8_t checksum() const;
    virtual size_t printTo(Print& p) const;
};
const Address64 INADDR_NONE(0,0,0,0);





#endif
