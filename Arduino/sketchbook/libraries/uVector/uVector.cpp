/**
 * @file uVector.cpp
 * @Author Michael Bartling (michael.bartling15+AMP_RF@gmail.com
 * @date Feb 1, 2014
 * @brief Simple vector class for microcontrollers
 */
#include "Arduino.h"
#include <uVector.h>

/**
 * could use calloc instead but this is for clarity
 * Constructor
 */
uVector::uVector(int s):sz(s), elem((uint8_t *)malloc(s)), space(s) {
	for(int i = 0; i < sz; ++i )
	{
		elem[i] = 0;
	}
}

/**
 * @name Assignment Operator
 * @brief Element Access
 *
 * Modified from Stroustrups example vector Class
 */
uVector& uVector::operator =(const uVector& cv) {

	if(this == &cv) return *this;

	if(cv.sz <= space)
	{
		for(int i = 0; i < cv.sz; ++i)
		{
			elem[i] = cv.elem[i];
		}
		sz = cv.sz;
		return *this;
	}

	uint8_t *p = (uint8_t *)malloc(cv.sz);
	for(int i = 0; i < cv.sz; ++i)
	{
		p[i] = cv.elem[i];
	}
	free(elem);
	space = sz = cv.sz;
	elem = p;
	return *this;

}

void uVector::resize(int newsize) {
	reserve(newsize);
	for(int i = sz; i < newsize; ++i)
	{
		elem[i] = 0;
	}

	sz = newsize;
}

/**
 * @name push_back
 * @brief insert data to contiguous memory block
 * @param data
 */
void uVector::push_back(const uint8_t data) {
	if(space == 0) reserve(16); //Xbee packets take at minimum 16 bytes
	else if (sz == space) reserve(2*space);
	elem[sz] = data;
	++sz;
}

/**
 * @name reserve
 * @brief Allocate more memory
 * @param newalloc number of bytes to reallocate
 */
void uVector::reserve(int newalloc)
{
	if(newalloc <= space)
	{
		return;
	}

	uint8_t * ptr = (uint8_t *)malloc(newalloc);
	for(int i = 0; i < sz; ++i )
	{
		ptr[i] = elem[i];
	}
	free(elem);
	elem = ptr;
	space = newalloc;

}

/**
 * @name changeVal
 * @brief replace element at index i with the newval
 * @param i
 * @param newval
 */
void uVector::changeVal(int i, uint8_t newval)
{
	if(i <= sz)
	{
		elem[i] = newval;
	}
}

/**
 * @name clear
 * @brief clear the allocated memory and free it for other use
 */
void uVector::clear()
{

	if(elem != NULL || sz != 0 || space != 0) //Or if space or sz != 0 (dont want to free something if it isnt there
	{
		free(elem);
	}
	sz = 0;
	space = 0;

}

