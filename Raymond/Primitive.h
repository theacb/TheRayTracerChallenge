#pragma once
#include "Object.h"

class Primitive :
	public ObjectBase
{
public:
	Primitive();
	~Primitive();

	// Methods
	virtual std::vector<float> intersect_t(Ray) const = 0;
	virtual Tuple normal_at(Tuple) const = 0;
};

class Sphere :
	public Primitive
{
public:
	Sphere();
	~Sphere();

	//properties
	float radius;

	// Methods
	std::vector<float> intersect_t(Ray) const;
	Tuple normal_at(Tuple) const;
};
