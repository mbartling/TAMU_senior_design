/*
 * uVector.h
 *
 *  Created on: Feb 1, 2014
 *      Author: Mike
 */

#ifndef UVECTOR_H_
#define UVECTOR_H_

#include "Arduino.h"

class uVector
{
	int sz;
	uint8_t *elem;
	int space; //size plus free space

public:
	uVector(): sz(0), elem(0), space(0){}
	uVector(int s);

	uVector(const uVector&);
	uVector& operator =(const uVector&);

	~uVector(){ delete[] elem; }

	uint8_t operator[](int n){ return elem[n];}
	const uint8_t operator[](int n) const {return elem[n]; }

	int size() const {return sz;}
	int capacity() const {return space;}

	void resize(int newsize);
	void push_back(const uint8_t data);

	void reserve(int newalloc);

};



#endif /* UVECTOR_H_ */
