#pragma once

#include "Object.h"
#include "Color.h"
#include "Matrix.h"

class Light :
	public ObjectBase
{
public:
	Light();
	Light(Color);
	~Light();

	virtual std::vector<float> intersect_t(Ray &) const = 0;

	//properties
	Color color;

	// Methods
	Tuple position() const;
};

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(Tuple, Color);
	~PointLight();

	// Methods
	virtual std::vector<float> intersect_t(Ray &) const;
};

