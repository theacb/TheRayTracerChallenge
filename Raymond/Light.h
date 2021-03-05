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
	Light(Color, float);
	~Light();

	virtual std::vector<float> intersect_t(Ray &) const = 0;
	virtual Tuple normal_at(Tuple &) const = 0;

	//properties
	Color color;
	float multiplier;

	// Methods
	Tuple position() const;
};

class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(Tuple, Color);
	PointLight(Tuple, Color, float);
	~PointLight();

	// Methods
	virtual std::vector<float> intersect_t(Ray &) const override;
	virtual Tuple normal_at(Tuple &) const override;
};

//Overloaded Operators
bool operator==(const PointLight &, const PointLight &);
bool operator!=(const PointLight &, const PointLight &);

