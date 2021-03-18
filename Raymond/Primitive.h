#ifndef H_RAYMOND_PRIMITIVE
#define H_RAYMOND_PRIMITIVE

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
	virtual std::vector<double> local_intersect_t(const Ray & r) const = 0;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const = 0;

};

class Sphere :
	public Primitive
{
public:
	Sphere();
	Sphere(std::string name);
	~Sphere();

	// Properties
	double radius;

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;
};

class InfinitePlane :
	public Primitive
{
public:
	InfinitePlane();
	InfinitePlane(std::string name);
	~InfinitePlane();

	// Properties
	Ray ray;

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;
};

class TestShape :
	public Primitive
{
public:
	TestShape();
	TestShape(std::string name);
	~TestShape();

	// Properties
	Ray ray;

	// Methods
	virtual std::vector<double> local_intersect_t(const Ray & r) const override;
	virtual Tuple local_normal_at(const Tuple & object_space_point) const override;
};

#endif
