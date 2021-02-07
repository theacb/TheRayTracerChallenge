#pragma once
#include "tuple.h"
class Color :
	public Tuple
{
public:
	// Constructors
	Color();
	Color(float);
	Color(float, float, float);
	Color(float, float, float, float);

	// Destrcutor
	~Color();

	// Properties
	float& r() { return this->x; }
	float& g() { return this->y; }
	float& b() { return this->z; }
	float& a() { return this->w; }

	// Methods

	Color multiply(const Color &);
};

