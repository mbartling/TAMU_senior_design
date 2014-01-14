
#include <Arduino.h>
#include <IP64Address.h>

IP64Address::IP64Address()
{
    memset(_address, 0, sizeof(_address));
}

IP64Address::IP64Address(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet)
{
    _address[0] = first_octet;
    _address[1] = second_octet;
    _address[2] = third_octet;
    _address[3] = fourth_octet;
}

IP64Address::IP64Address(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet, uint8_t fifth_octet, uint8_t sixth_octet, uint8_t seventh_octet,uint8_t eigth_octet)
{
    _address[0] = first_octet;
    _address[1] = second_octet;
    _address[2] = third_octet;
    _address[3] = fourth_octet;
    _address[4] = fifth_octet;
    _address[5] = sixth_octet;
    _address[6] = seventh_octet;
    _address[7] = eigth_octet;
}

IP64Address::IP64Address(uint32_t address)
{
    memcpy(_address, &address, sizeof(_address));
}
IP64Address::IP64Address(uint64_t address)
{
    memcpy(_address, &address, sizeof(_address));
}

IP64Address::IP64Address(const uint8_t *address)
{
    memcpy(_address, address, sizeof(_address));
}

IP64Address& IP64Address::operator=(const uint8_t *address)
{
    memcpy(_address, address, sizeof(_address));
    return *this;
}

IP64Address& IP64Address::operator=(uint32_t address)
{
    memcpy(_address, (const uint8_t *)&address, sizeof(_address));
    return *this;
}

IP64Address& IP64Address::operator=(uint64_t address)
{
    memcpy(_address, (const uint8_t *)&address, sizeof(_address));
    return *this;
}

bool IP64Address::operator==(const uint8_t* addr)
{
    return memcmp(addr, _address, sizeof(_address)) == 0;
}

size_t IP64Address::printTo(Print& p) const
{
    size_t n = 0;
    for (int i =0; i < 7; i++)
    {
        n += p.print(_address[i], DEC);
        n += p.print('.');
    }
    n += p.print(_address[7], DEC);
    return n;
}
/**
 * For writing out to the 
 */
/**
size_t IP64Address::printTo(Print& p) const
{
    size_t n = 0;
    for (int i =0; i < 7; i++)
    {
        n += p.print(_address[i], BYTE);
        //n += p.print('.');
    }
    n += p.print(_address[7], BYTE);
    return n;
}
*/
