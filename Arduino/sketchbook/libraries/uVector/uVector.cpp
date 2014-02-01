/*
 * uVector.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: Mike
 */
#include "Arduino.h"
#include "uVector.h"

/**
 * could use calloc instead but this is for clarity
 */
uVector::uVector(int s):sz(s), elem((uint8_t *)malloc(s)), space(s) {
	for(int i = 0; i < sz; ++i )
	{
		elem[i] = 0;
	}
}


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

void uVector::push_back(const uint8_t data) {
	if(space == 0) reserve(8);
	else if (sz == space) reserve(2*space);
	elem[sz] = data;
	++sz;
}


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



