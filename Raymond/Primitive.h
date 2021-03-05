#pragma once

#include <memory> // Shared Pointers

#include "Object.h"
#include "Material.h"



class Primitive :
	public ObjectBase
{
public:
	Primitive();
	~Primitive();

	//properties
	std::shared_ptr<BaseMaterial> material;

	// Methods
	virtual std::vector<float> intersect_t(Ray &) const = 0;
	virtual Tuple normal_at(Tuple &) const = 0;

};

class Sphere :
	public Primitive
{
public:
	Sphere();
	Sphere(std::string);
	~Sphere();

	//properties
	float radius;

	// Methods
	virtual std::vector<float> intersect_t(Ray &) const override;
	virtual Tuple normal_at(Tuple &) const override;
};
