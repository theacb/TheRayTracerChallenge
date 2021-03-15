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
	virtual std::vector<double> local_intersect_t(const Ray &) const = 0;
	virtual Tuple local_normal_at(const Tuple &) const = 0;

};

class Sphere :
	public Primitive
{
public:
	Sphere();
	Sphere(std::string);
	~Sphere();

	//properties
	double radius;

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray &) const override;
	virtual Tuple local_normal_at(const Tuple &) const override;
};

class TestShape :
	public Primitive
{
public:
	TestShape();
	TestShape(std::string);
	~TestShape();

	//properties
	double radius;

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray &) const override;
	virtual Tuple local_normal_at(const Tuple &) const override;
};
