#ifndef TYPES_H
#define TYPES_H

#include <Windows.h>

struct VERTEX {
	VERTEX(float x, float y, float z, float r, float g, float b, float a) {
		this->X = x;
		this->Y = y;
		this->Z = z;
		this->R = r;
		this->G = g;
		this->B = b;
		this->A = a;
	}

	FLOAT X, Y, Z;
	FLOAT R, G, B, A;
};

#endif